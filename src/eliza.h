#ifndef ELIZA_H
#define ELIZA_H

#include "tree.h"

#define NUM_FEATURES 21
#define MAX_INPUT 2048
#define MAX_RESPONSE 4096
#define MEMORY_SIZE 5

typedef enum {
    GREETING = 0,
    REFLECT_FEELING,
    REFLECT_STATEMENT,
    QUESTION_OPEN,
    QUESTION_FEELING,
    ENCOURAGE,
    FAMILY_REFLECT,
    DREAM_REFLECT,
    TRANSPERENCE,
    CRISIS_RESPONSE,
    WORK_SCHOOL,
    HEALTH_BODY,
    SELF_ESTEEM,
    EXISTENTIAL,
    DEFAULT_KIND,
    NUM_LABELS
} ResponseLabel;

typedef struct {
    char keyword[64];
    char phrase[256];
    int turn;
} MemoryItem;

typedef struct {
    MemoryItem items[MEMORY_SIZE];
    int count;
    int next;
    int turn;
} Memory;

void memory_init(Memory *mem);
void extract_features(const char *input, double *features);
Dataset *generate_synthetic_data(int count);
char *generate_response(TreeNode *tree, const char *input, Memory *mem);
void run_interactive(TreeNode *tree);

#endif
