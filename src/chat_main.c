#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"
#include "eliza.h"

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    const char *infile = "eliza.tree";
    if (argc > 1) infile = argv[1];

    TreeNode *tree = load_tree(infile);
    if (!tree) {
        fprintf(stderr, "error: could not load tree from '%s'\n", infile);
        fprintf(stderr, "train one first: ./eliza-train [filename]\n");
        return 1;
    }

    printf("Loaded tree from '%s'\n", infile);
    run_interactive(tree);

    free_tree(tree);
    return 0;
}
