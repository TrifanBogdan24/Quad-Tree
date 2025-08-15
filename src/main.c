#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

#include <math.h>

double compute_block_score(PPM_Image *img, unsigned int x, unsigned int y, unsigned int size)
{
    unsigned long sumR = 0, sumG = 0, sumB = 0;
    unsigned int i, j;
    unsigned int count = size * size;

    // Step 1: sum all channel values in the block
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            sumR += img->grid[i][j].R;
            sumG += img->grid[i][j].G;
            sumB += img->grid[i][j].B;
        }
    }

    // Compute averages
    double avgR = (double)sumR / count;
    double avgG = (double)sumG / count;
    double avgB = (double)sumB / count;

    // Step 2: compute mean squared difference
    double sumSqDiff = 0.0;
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            double dr = avgR - img->grid[i][j].R;
            double dg = avgG - img->grid[i][j].G;
            double db = avgB - img->grid[i][j].B;

            sumSqDiff += dr * dr + dg * dg + db * db;
        }
    }

    // Final mean
    double mean = sumSqDiff / (3.0 * count);
    return mean;
}

PPM_Image read_PPM_image(FILE *fin)
{
    char type[2];
    fread(type, sizeof(char), sizeof(type), fin);

    PPM_Image img;
    fscanf(fin, "%u", &img.width);
    fscanf(fin, "%u", &img.height);
    fscanf(fin, "%u", &img.max_rgb);

    img.grid = (Color **) malloc(img.height * sizeof(Color *));
    for (int i = 0; i < img.height; i++)
        img.grid[i] = (Color *) malloc(img.width * sizeof(Color));
    
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            fread(&img.grid[i][j].R, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].G, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].B, sizeof(unsigned char), 1, fin);
        }
    }

    return img;
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
        double mean = compute_block_score(img, x, y, size);

        if (mean <= factor)
            continue;
        
        node->child_upper_left  = new_tree_node(x         , y         , size/2);
        node->child_upper_right = new_tree_node(x         , y + size/2, size/2);
        node->child_lower_right = new_tree_node(x + size/2, y + size/2, size/2);
        node->child_lower_left  = new_tree_node(x + size/2, y         , size/2); 

        queue_push(&queue, node->child_upper_left);
        queue_push(&queue, node->child_upper_right);
        queue_push(&queue, node->child_lower_right);
        queue_push(&queue, node->child_lower_left);
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


    free(queue);
    fprintf(fout, "%d\n", get_tree_height(root) + 1);
    fprintf(fout, "%d\n", num_leaves);
    fprintf(fout, "%d\n", max_size);
}


void solve_task2(int factor, FILE *fin, FILE *fout)
{
    PPM_Image img = read_PPM_image(fin);
    QuadTree *root = compress_image(&img, factor);

    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    fwrite(&img.width, sizeof(unsigned int), 1, fout);

    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);
        
        if (is_leaf(node)) {
            // Nod frunza
            unsigned char type = 1;
            fwrite(&type, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.R, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.G, sizeof(unsigned char), 1, fout);
            fwrite(&node->color.B, sizeof(unsigned char), 1, fout);
        } else {
            unsigned char type = 0;
            fwrite(&type, sizeof(unsigned char), 1, fout);
        }
    }

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
            Color color;
            fread(&color.R, sizeof(unsigned char), 1, fin);
            fread(&color.G, sizeof(unsigned char), 1, fin);
            fread(&color.B, sizeof(unsigned char), 1, fin);

            // Fill image block with color
            for (int i = x; i < x + size; i++)
                for (int j = y; j < y + size; j++)
                    img.grid[i][j] = color;
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
