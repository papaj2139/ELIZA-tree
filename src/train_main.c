#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tree.h"
#include "eliza.h"

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    const char *outfile = "eliza.tree";
    int num_samples = 10000;
    int max_depth = 7;
    int min_samples = 10;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                outfile = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requires an argument\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--samples") == 0 || strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                num_samples = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: %s requires an argument\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--max_depth") == 0 || strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc) {
                max_depth = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: %s requires an argument\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--min_samples") == 0 || strcmp(argv[i], "-m") == 0) {
            if (i + 1 < argc) {
                min_samples = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: %s requires an argument\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  -o, --output <file>       Output file (default: eliza.tree)\n");
            printf("  -n, --samples <num>       Number of samples (default: 10000)\n");
            printf("  -d, --max_depth <depth>   Max tree depth (default: 7)\n");
            printf("  -m, --min_samples <num>   Min samples to split (default: 10)\n");
            printf("  -h, --help                Show this help message\n");
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Use --help or -h for usage information.\n");
            return 1;
        }
    }

    printf("training...");
    printf("Generating %d examples...\n", num_samples);

    Dataset *data = generate_synthetic_data(num_samples);
    printf("Training decision tree (max_depth=%d, min_samples=%d)...\n", max_depth, min_samples);
    TreeNode *tree = train_decision_tree(data, max_depth, min_samples);

    save_tree(tree, outfile);
    printf("Tree saved to '%s'\n", outfile);

    free_tree(tree);
    free_dataset(data);
    return 0;
}
