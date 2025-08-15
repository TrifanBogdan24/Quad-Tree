#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

/*
* (x, y) -> coordinates of upper-left corner
*/
unsigned int compute_mean(RawImage *img, unsigned int x, unsigned int y, unsigned int size)
{
    if (!size) {
        fprintf(stderr, "[ERR] Cannot divide with size=0\n");
        exit(EXIT_FAILURE);
    }

    unsigned long long sum_R = 0;
    unsigned long long sum_G = 0;
    unsigned long long sum_B = 0;

    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            sum_R += img->grid[i][j].R;
            sum_G += img->grid[i][j].G;
            sum_B += img->grid[i][j].B;
        }
    }

    unsigned int mean_R = sum_R / (size * size);
    unsigned int mean_G = sum_G / (size * size);
    unsigned int mean_B = sum_B / (size * size);

    unsigned long long sum_square_diff = 0;


    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            sum_square_diff += (mean_R - img->grid[i][j].R) * (mean_R - img->grid[i][j].R);
            sum_square_diff += (mean_G - img->grid[i][j].G) * (mean_G - img->grid[i][j].G);
            sum_square_diff += (mean_B - img->grid[i][j].B) * (mean_B - img->grid[i][j].B);
        }
    }

    return (sum_square_diff / (3 * size * size));
}

RawImage read_PPM_image(FILE *fin)
{
    char type[2];
    fread(type, sizeof(char), sizeof(type), fin);

    RawImage img;
    fscanf(fin, "%u", &img.width);
    fscanf(fin, "%u", &img.height);
    fscanf(fin, "%u", &img.max_rgb);

    img.grid = (color **) malloc(img.height * sizeof(color *));
    for (int i = 0; i < img.height; i++)
        img.grid[i] = (color *) malloc(img.width * sizeof(color));
    
    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            fread(&img.grid[i][j].R, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].G, sizeof(unsigned char), 1, fin);
            fread(&img.grid[i][j].B, sizeof(unsigned char), 1, fin);
        }
    }

    return img;
}


void solve_task1(int factor, FILE *fin, FILE *fout)
{
    RawImage img = read_PPM_image(fin);
    QuadTree *root = new_tree_node(0, 0, img.height);

    printf("%d\n", compute_mean(&img, 0, 0, img.height/4));
    return;

    Queue *queue = new_empty_queue();
    queue_push(&queue, root);

    while (!is_empty(queue)) {
        QuadTree *node = queue_pop(queue);
        unsigned int x = node->x;
        unsigned int y = node->y;
        unsigned int size = node->size;
        unsigned int mean = compute_mean(&img, x, y, size);
        printf("%d %d\n", size, mean);

        if (mean <= factor)
            continue;
        
        node->child_upper_left  = new_tree_node(x         , y         , size/2);
        node->child_upper_right = new_tree_node(x + size/2, y         , size/2);
        node->child_lower_right = new_tree_node(x + size/2, y + size/2, size/2);
        node->child_lower_left  = new_tree_node(x         , y + size/2, size/2); 

        queue_push(&queue, node->child_upper_left);
        queue_push(&queue, node->child_upper_right);
        queue_push(&queue, node->child_lower_right);
        queue_push(&queue, node->child_lower_left);
    }


    int num_leaves = 0;
    int max_size = 0;
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

    fprintf(fout, "%d\n", get_tree_height(root) + 1);
    fprintf(fout, "%d\n", num_leaves);
    fprintf(fout, "%d\n", max_size);
}


void solve_task2(int factor, FILE *fin, FILE *fout)
{
    // TODO

}

void solve_task3(FILE *fin, FILE *fout)
{
    // TODO

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
    FILE *fout = fopen(argv[argc - 1], "wt");   // Write text

    if (!fin) {
        fprintf(stderr, "[ERR] Failed to open '%s' file\n", argv[argc - 2]);
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "-c1")) {
        int factor = atoi(argv[2]);
        solve_task1(factor, fin, fout);
    }
    else if (!strcmp(argv[1], "-c2")) {
        int factor = atoi(argv[2]);
        solve_task2(factor, fin, fout);
    } else if (!strcmp(argv[1], "-d"))
        solve_task3(fin, fout);

    return 0;
}
