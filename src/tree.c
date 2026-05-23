#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree.h"

static double gini_impurity(Dataset *data, int *indices, int n) {
    if (n == 0) return 0;
    int *counts = calloc(data->max_label + 1, sizeof(int));
    for (int i = 0; i < n; i++)
        counts[data->labels[indices[i]]]++;
    double impurity = 1.0;
    for (int i = 0; i <= data->max_label; i++) {
        double p = (double)counts[i] / n;
        impurity -= p * p;
    }
    free(counts);
    return impurity;
}

static int majority_label(Dataset *data, int *indices, int n) {
    int *counts = calloc(data->max_label + 1, sizeof(int));
    for (int i = 0; i < n; i++)
        counts[data->labels[indices[i]]]++;
    int best = 0, best_count = 0;
    for (int i = 0; i <= data->max_label; i++) {
        if (counts[i] > best_count) {
            best_count = counts[i];
            best = i;
        }
    }
    free(counts);
    return best;
}

//candidate split from find_best_split
typedef struct {
    int feature_idx;
    double threshold;
    double gain;
    int left_n;
    int right_n;
} Split;

static int compare_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

//find the split that maximises gini reduction
static Split find_best_split(Dataset *data, int *indices, int n) {
    Split best;
    best.feature_idx = -1;
    best.gain = -1.0;

    double parent_gini = gini_impurity(data, indices, n);

    for (int f = 0; f < data->num_features; f++) {
        double *vals = malloc(n * sizeof(double));
        for (int i = 0; i < n; i++)
            vals[i] = data->features[indices[i]][f];

        qsort(vals, n, sizeof(double), compare_double);

        //collect up to 1024 unique values
        double unique_vals[1024];
        int n_unique = 0;
        for (int i = 0; i < n; i++) {
            if (n_unique == 0 || fabs(vals[i] - unique_vals[n_unique-1]) > 1e-9) {
                if (n_unique < 1024) unique_vals[n_unique++] = vals[i];
            }
        }

        //try mid-point between each consecutive pair of values
        for (int s = 0; s < n_unique - 1; s++) {
            double threshold = (unique_vals[s] + unique_vals[s+1]) / 2.0;
            int left_n = 0, right_n = 0;
            for (int i = 0; i < n; i++) {
                if (data->features[indices[i]][f] <= threshold)
                    left_n++;
                else
                    right_n++;
            }
            if (left_n == 0 || right_n == 0) continue;

            int *left_idx = malloc(left_n * sizeof(int));
            int *right_idx = malloc(right_n * sizeof(int));
            int li = 0, ri = 0;
            for (int i = 0; i < n; i++) {
                if (data->features[indices[i]][f] <= threshold)
                    left_idx[li++] = indices[i];
                else
                    right_idx[ri++] = indices[i];
            }

            double gini_left = gini_impurity(data, left_idx, left_n);
            double gini_right = gini_impurity(data, right_idx, right_n);
            double weighted = ((double)left_n / n) * gini_left + ((double)right_n / n) * gini_right;
            double gain = parent_gini - weighted;

            if (gain > best.gain) {
                best.gain = gain;
                best.feature_idx = f;
                best.threshold = threshold;
                best.left_n = left_n;
                best.right_n = right_n;
            }

            free(left_idx);
            free(right_idx);
        }
        free(vals);
    }

    return best;
}

//recursively build a decision tree
//stops when labels are uniform depth limit or min samples reached
static TreeNode *build_tree_recursive(Dataset *data, int *indices, int n, int depth, int max_depth, int min_samples) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->left = NULL;
    node->right = NULL;

    if (n == 0) {
        node->is_leaf = 1;
        node->label = 0;
        node->feature_idx = -1;
        return node;
    }

    //all labels the same
    int same = 1;
    for (int i = 1; i < n; i++) {
        if (data->labels[indices[i]] != data->labels[indices[0]]) {
            same = 0;
            break;
        }
    }

    if (same || depth >= max_depth || n < min_samples) {
        node->is_leaf = 1;
        node->label = majority_label(data, indices, n);
        node->feature_idx = -1;
        return node;
    }

    Split split = find_best_split(data, indices, n);

    if (split.feature_idx == -1 || split.gain <= 0) {
        node->is_leaf = 1;
        node->label = majority_label(data, indices, n);
        node->feature_idx = -1;
        return node;
    }

    node->is_leaf = 0;
    node->feature_idx = split.feature_idx;
    node->threshold = split.threshold;

    int *left_idx = malloc(split.left_n * sizeof(int));
    int *right_idx = malloc(split.right_n * sizeof(int));
    int li = 0, ri = 0;
    for (int i = 0; i < n; i++) {
        if (data->features[indices[i]][split.feature_idx] <= split.threshold)
            left_idx[li++] = indices[i];
        else
            right_idx[ri++] = indices[i];
    }

    node->left = build_tree_recursive(data, left_idx, split.left_n, depth + 1, max_depth, min_samples);
    node->right = build_tree_recursive(data, right_idx, split.right_n, depth + 1, max_depth, min_samples);

    free(left_idx);
    free(right_idx);

    return node;
}

//train a decision tree on the full dataset
TreeNode *train_decision_tree(Dataset *data, int max_depth, int min_samples) {
    int *indices = malloc(data->n * sizeof(int));
    for (int i = 0; i < data->n; i++) indices[i] = i;
    TreeNode *root = build_tree_recursive(data, indices, data->n, 0, max_depth, min_samples);
    free(indices);
    return root;
}

//predict label for a single sample
int predict_tree(TreeNode *node, double *features) {
    if (node->is_leaf) return node->label;
    if (features[node->feature_idx] <= node->threshold)
        return predict_tree(node->left, features);
    else
        return predict_tree(node->right, features);
}

//free the whole tree
void free_tree(TreeNode *node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

//print tree in indented format
void print_tree(TreeNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    if (node->is_leaf) {
        printf("-> label %d\n", node->label);
    } else {
        printf("f[%d] <= %.4f\n", node->feature_idx, node->threshold);
        print_tree(node->left, depth + 1);
        for (int i = 0; i < depth; i++) printf("  ");
        printf("f[%d] >  %.4f\n", node->feature_idx, node->threshold);
        print_tree(node->right, depth + 1);
    }
}

//allocate and zero a dataset
Dataset *alloc_dataset(int n, int num_features, int max_label) {
    Dataset *data = malloc(sizeof(Dataset));
    data->n = n;
    data->num_features = num_features;
    data->max_label = max_label;
    data->features = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        data->features[i] = calloc(num_features, sizeof(double));
    data->labels = malloc(n * sizeof(int));
    return data;
}

//free dataset and all owned memory
void free_dataset(Dataset *data) {
    if (!data) return;
    for (int i = 0; i < data->n; i++)
        free(data->features[i]);
    free(data->features);
    free(data->labels);
    free(data);
}

//recursively write node + children to file
static void save_node(FILE *f, TreeNode *node) {
    if (node->is_leaf) {
        fprintf(f, "L %d\n", node->label);
    } else {
        fprintf(f, "I %d %.10f\n", node->feature_idx, node->threshold);
        save_node(f, node->left);
        save_node(f, node->right);
    }
}

//serialise tree to text file
void save_tree(TreeNode *node, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { fprintf(stderr, "error: cannot write %s\n", filename); return; }
    save_node(f, node);
    fclose(f);
}

//recursively read node + children from file
static TreeNode *load_node(FILE *f) {
    char type;
    if (fscanf(f, " %c", &type) != 1) return NULL;
    TreeNode *n = malloc(sizeof(TreeNode));
    if (type == 'L') {
        int label;
        fscanf(f, "%d", &label);
        n->is_leaf = 1;
        n->label = label;
        n->feature_idx = -1;
        n->left = n->right = NULL;
    } else {
        int idx;
        double thresh;
        fscanf(f, "%d %lf", &idx, &thresh);
        n->is_leaf = 0;
        n->feature_idx = idx;
        n->threshold = thresh;
        n->left = load_node(f);
        n->right = load_node(f);
    }
    return n;
}

//deserialise tree from text file
TreeNode *load_tree(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "error: cannot read %s\n", filename);
        return NULL;
    }
    TreeNode *root = load_node(f);
    fclose(f);
    return root;
}
