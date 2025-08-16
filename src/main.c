#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

#include <math.h>

Color avg_block_color(PPM_Image *img, unsigned int x, unsigned int y, unsigned int size)
{
    unsigned long long sum_R = 0, sum_G = 0, sum_B = 0;

    for (unsigned int i = x; i < x + size; i++) {
        for (unsigned int j = y; j < y + size; j++) {
            sum_R += img->grid[i][j].R;
            sum_G += img->grid[i][j].G;
            sum_B += img->grid[i][j].B;
        }
    }

    Color color;
    // Compute means:
    color.R = (unsigned char)(sum_R / (size * size));
    color.G = (unsigned char)(sum_G / (size * size));
    color.B = (unsigned char)(sum_B / (size * size));

    return color;
}


int compute_block_score(PPM_Image *img, unsigned int x, unsigned int y, unsigned int size)
{
    Color avg_color = avg_block_color(img, x, y, size);

    unsigned long long sumSqDiff = 0;
    for (unsigned int i = x; i < x + size; i++) {
        for (unsigned int j = y; j < y + size; j++) {
            long long dR = (long long)img->grid[i][j].R - (long long)avg_color.R;
            long long dG = (long long)img->grid[i][j].G - (long long)avg_color.G;
            long long dB = (long long)img->grid[i][j].B - (long long)avg_color.B;
            sumSqDiff += dR * dR + dG * dG + dB * dB;
        }
    }

    return (int)(sumSqDiff / (3 * size * size));
}


PPM_Image read_PPM_image(FILE *fin)
{
    char type[3];
    fscanf(fin, "%2s", type);  // Read "P6"
    
    PPM_Image img;
    fscanf(fin, "%u %u", &img.width, &img.height);
    fscanf(fin, "%u", &img.max_rgb);

    fgetc(fin);   // Read new-line charcater

    img.grid = malloc(img.height * sizeof(Color *));
    for (unsigned int i = 0; i < img.height; i++)
        img.grid[i] = malloc(img.width * sizeof(Color));

    // Read RGB values from binary file
    for (unsigned int i = 0; i < img.height; i++) {
        for (unsigned int j = 0; j < img.width; j++) {
            fread(&img.grid[i][j].R, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].G, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].B, sizeof(unsigned char), 1, fin);
        }
    }

    return img;
}

void write_PMM_image(PPM_Image *img, FILE *fout)
{
    fprintf(fout, "P6\n");
    fprintf(fout, "%u %u\n%u\n", img->width, img->height, img->max_rgb);

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            fwrite(&img->grid[i][j].R, sizeof(unsigned char), 1, fout);
            fwrite(&img->grid[i][j].G, sizeof(unsigned char), 1, fout);
            fwrite(&img->grid[i][j].B, sizeof(unsigned char), 1, fout);
        }
    }
}


void write_RGB_values(PPM_Image *img)
{
    FILE *fout = fopen("image-rgb.txt", "wt");   // Write text

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++)
            fprintf(fout, "[%d %d %d]", img->grid[i][j].R, img->grid[i][j].G, img->grid[i][j].B);
        fprintf(fout, "\n");
    }

    fclose(fout);
}


QuadTree *compress_image(PPM_Image *img, int factor)
{
    QuadTree *root = new_tree_node(0, 0, img->height);

    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);
        unsigned int x = node->x;
        unsigned int y = node->y;
        unsigned int size = node->size;
        int mean = compute_block_score(img, x, y, size);

        if (mean <= factor) {
            // Leaf node
            Color avg_color = avg_block_color(img, x, y, size);

            node->color = (Color *) malloc(sizeof(Color));
            *(node->color) = avg_color;     // Copies entire struct
        } else {
            // Non-leaf node
            node->child_upper_left  = new_tree_node(x         , y         , size/2);
            node->child_upper_right = new_tree_node(x         , y + size/2, size/2);
            node->child_lower_right = new_tree_node(x + size/2, y + size/2, size/2);
            node->child_lower_left  = new_tree_node(x + size/2, y         , size/2); 

            queue_push(&queue, node->child_upper_left);
            queue_push(&queue, node->child_upper_right);
            queue_push(&queue, node->child_lower_right);
            queue_push(&queue, node->child_lower_left);
        }

    }

    free(queue);
    return root;
}


void solve_task1(int factor, FILE *fin, FILE *fout)
{
    PPM_Image img = read_PPM_image(fin);
    QuadTree *root = compress_image(&img, factor);

    int num_leaves = 0;
    int max_size = 0;

    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);
        if (is_leaf(node)) {
            num_leaves++;
            max_size = MAX(max_size, node->size);
            continue;
        }

        queue_push(&queue, node->child_upper_left);
        queue_push(&queue, node->child_upper_right);
        queue_push(&queue, node->child_lower_right);
        queue_push(&queue, node->child_lower_left);
    }


    fprintf(fout, "%d\n", get_tree_height(root));
    fprintf(fout, "%d\n", num_leaves);
    fprintf(fout, "%d\n", max_size);

    delete_image(&img);
    delete_tree(&root);
    free(queue);
}


void solve_task2(int factor, FILE *fin, FILE *fout)
{
    PPM_Image img = read_PPM_image(fin);
    QuadTree *root = compress_image(&img, factor);

    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    unsigned int grid_size = img.height;
    fwrite(&grid_size, sizeof(unsigned int), 1, fout);

    // Write quad-tree of compressed image
    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);
        
        if (is_leaf(node)) {
            // Leaf node
            unsigned char type = 1;
            fwrite(&type, sizeof(unsigned char), 1, fout);
            fwrite(&node->color->R, sizeof(unsigned char), 1, fout);
            fwrite(&node->color->G, sizeof(unsigned char), 1, fout);
            fwrite(&node->color->B, sizeof(unsigned char), 1, fout);
        } else {
            // Non-leaf node
            unsigned char type = 0;
            fwrite(&type, sizeof(unsigned char), 1, fout);

            queue_push(&queue, node->child_upper_left);
            queue_push(&queue, node->child_upper_right);
            queue_push(&queue, node->child_lower_right);
            queue_push(&queue, node->child_lower_left);
        }
    }


    delete_image(&img);
    delete_tree(&root);
    free(queue);
}




void solve_task3(FILE *fin, FILE *fout)
{
    PPM_Image img;
    img.max_rgb = 255;

    unsigned int grid_size = 0;
    fread(&grid_size, sizeof(unsigned int), 1, fin);

    img.height = grid_size;
    img.width  = grid_size;

    img.grid = (Color **) malloc(img.height * sizeof(Color *));
    for (int i = 0; i < img.height; i++)
        img.grid[i] = (Color *) calloc(img.width, sizeof(Color));
    
    QuadTree *root = new_tree_node(0, 0, grid_size);
    
    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);

        unsigned char node_type = 0;
        fread(&node_type, sizeof(unsigned char), 1, fin);

        unsigned int x = node->x;
        unsigned int y = node->y;
        unsigned int size = node->size;


        if (node_type == 1) {
            // Leaf node
            node->color = (Color *) malloc(sizeof(Color));
            fread(&node->color->R, sizeof(unsigned char), 1, fin);
            fread(&node->color->G, sizeof(unsigned char), 1, fin);
            fread(&node->color->B, sizeof(unsigned char), 1, fin);

            // Fill image block with color
            for (int i = x; i < x + size; i++)
                for (int j = y; j < y + size; j++)
                    img.grid[i][j] = *(node->color);
        } else {
            // Non-leaf node
            node->child_upper_left  = new_tree_node(x         , y         , size/2);
            node->child_upper_right = new_tree_node(x         , y + size/2, size/2);
            node->child_lower_right = new_tree_node(x + size/2, y + size/2, size/2);
            node->child_lower_left  = new_tree_node(x + size/2, y         , size/2); 

            queue_push(&queue, node->child_upper_left);
            queue_push(&queue, node->child_upper_right);
            queue_push(&queue, node->child_lower_right);
            queue_push(&queue, node->child_lower_left);
        }

    }


    write_PMM_image(&img, fout);

    free(queue);
    delete_tree(&root);
    delete_image(&img);
}



void check_command_line_arguments(int argc, char *argv[])
{
    if (argc < 4 || 5 < argc) {
        fprintf(stderr, "[ERR] Invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4) {
        if (!strcmp(argv[1], "-d") && strlen(argv[1]) == strlen("-d"))
            return;
        
        fprintf(stderr, "[INFO] Program called with 3 arguments\n");
        fprintf(stderr, "[ERR]  Expected flag '-d'\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 5) {
        if (!strcmp(argv[1], "-c1") && strlen(argv[1]) == strlen("-c1"))
            return;
        if (!strcmp(argv[1], "-c2") && strlen(argv[1]) == strlen("-c2"))
            return;
        
        fprintf(stderr, "[INFO] Program called with 4 arguments\n");
        fprintf(stderr, "[ERR]  Expected either flag '-c1' or '-c2'\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    check_command_line_arguments(argc, argv);

    FILE *fin = fopen(argv[argc - 2], "rb");    // Read binary
    FILE *fout;

    if (!fin) {
        fprintf(stderr, "[ERR] Failed to open '%s' file\n", argv[argc - 2]);
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "-c1")) {
        fout = fopen(argv[argc - 1], "wt");   // Write text
        int factor = atoi(argv[2]);
        solve_task1(factor, fin, fout);
    }
    else if (!strcmp(argv[1], "-c2")) {
        fout = fopen(argv[argc - 1], "wb");   // Write binary
        int factor = atoi(argv[2]);
        solve_task2(factor, fin, fout);
    } else if (!strcmp(argv[1], "-d")) {
        fout = fopen(argv[argc - 1], "wb");   // Write binary
        solve_task3(fin, fout);
    }


    fclose(fin);
    fclose(fout);
    return 0;
}
