#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

typedef struct colour {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} colour;

typedef struct node {
    colour pixel;
    unsigned char type;              /*  1 for leaf node; 0 for non-leaf node */
    struct node *sub_upper_left;     // Child 1
    struct node *sub_upper_right;    // Child 2
    struct node *sub_lower_right;    // Chlid 3
    struct node *sub_lower_left;     // Child 4
} node, *quad_tree;



#endif
