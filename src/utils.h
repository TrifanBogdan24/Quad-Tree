#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#define MAX(x, y) ((x > y) ? x : y)


typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Color;


typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int max_rgb;
    Color **grid;
} PPM_Image;


typedef struct TreeNode {
    unsigned int x, y, grid_size;          // Grid info
    Color *color;
    struct TreeNode *child_upper_left;     // Child 1
    struct TreeNode *child_upper_right;    // Child 2
    struct TreeNode *child_lower_right;    // Chlid 3
    struct TreeNode *child_lower_left;     // Child 4
} QuadTree;



typedef struct ListNode {
    QuadTree *root;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    ListNode *tail;
} Queue;


void delete_image(PPM_Image *img)
{
    for (int i = 0; i < img->height; i++)
        free(img->grid[i]);
    free(img->grid);
}

/*
* (x, y)  coordinates of the upper-left corner
* size    length and width of the (square) matrix of interest in the color grid
*/
QuadTree *new_tree_node(unsigned int x, unsigned  y, unsigned int size)
{
    QuadTree *new_node = (QuadTree *) malloc (sizeof(QuadTree));
    new_node->x = x;
    new_node->y = y;
    new_node->grid_size = size;
    new_node->color = NULL;
    new_node->child_lower_left = NULL;
    new_node->child_lower_right = NULL;
    new_node->child_upper_left = NULL;
    new_node->child_upper_right = NULL;
    return new_node;
}

ListNode *new_list_node(QuadTree *root)
{
    ListNode *new_node = (ListNode *) malloc (sizeof(ListNode));
    new_node->root = root;
    new_node->next = NULL;
    return new_node;
}


int is_leaf(QuadTree *root)
{
    return root
        && !root->child_upper_left
        && !root->child_upper_right
        && !root->child_lower_right
        && !root->child_lower_left;
}


/*
* DFS (Depth-First Search) tree traversal
*/
int get_tree_height(QuadTree *root)
{
    if (!root)
        return 0;
    
    int height_child1 = get_tree_height(root->child_upper_left);
    int height_child2 = get_tree_height(root->child_upper_right);
    int height_child3 = get_tree_height(root->child_lower_right);
    int height_child4 = get_tree_height(root->child_lower_left);

    int max_height = 0;
    max_height = MAX(max_height, height_child1);
    max_height = MAX(max_height, height_child2);
    max_height = MAX(max_height, height_child3);
    max_height = MAX(max_height, height_child4);
    return (max_height + 1);
}

void delete_tree(QuadTree **root)
{
    if (!(*root))
        return;
    
    QuadTree *tmp = *root;
    delete_tree(&(*root)->child_upper_left);
    delete_tree(&(*root)->child_upper_right);
    delete_tree(&(*root)->child_lower_right);
    delete_tree(&(*root)->child_lower_left);

    if ((*root)->color)
        free((*root)->color);
    free(tmp);
}

Queue *new_empty_queue()
{
    return (Queue *) calloc (1, sizeof(Queue));
}

int is_empty(Queue *queue)
{
    return (!queue || !queue->head);
}


QuadTree *queue_pop(Queue *queue)
{
    if (is_empty(queue)) {
        fprintf(stderr, "[ERR] Empty queue\n");
        return NULL;
    }

    QuadTree *root = queue->head->root;
    ListNode *tmp = queue->head;
    queue->head = queue->head->next;
    free(tmp);
    return root; 
}

void queue_push(Queue **queue, QuadTree *root)
{
    if (!(*queue)) {
        // Queue does not exist
        *queue = new_empty_queue();
    }

    if (!(*queue)->head) {
        // Empty queue
        (*queue)->head = (*queue)->tail = new_list_node(root);
        return;
    }

    // Non-empty queue
    (*queue)->tail->next = new_list_node(root);
    (*queue)->tail = (*queue)->tail->next;
}




#endif
