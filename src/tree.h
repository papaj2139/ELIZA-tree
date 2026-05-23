#ifndef TREE_H
#define TREE_H

typedef struct TreeNode {
    int feature_idx;
    double threshold;
    int is_leaf;
    int label;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    double **features;
    int *labels;
    int n;
    int num_features;
    int max_label;
} Dataset;

TreeNode *train_decision_tree(Dataset *data, int max_depth, int min_samples);
int predict_tree(TreeNode *node, double *features);
void free_tree(TreeNode *node);
void print_tree(TreeNode *node, int depth);
void save_tree(TreeNode *node, const char *filename);
TreeNode *load_tree(const char *filename);
Dataset *alloc_dataset(int n, int num_features, int max_label);
void free_dataset(Dataset *data);

#endif
