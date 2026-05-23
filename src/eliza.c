#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "eliza_templates.h"
#include "eliza.h"
#include "tree.h"

static const char *stristr(const char *haystack, const char *needle) {
    if (!*needle) return haystack;
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack, *n = needle;
            while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) h++, n++;
            if (!*n) return haystack;
        }
    }
    return NULL;
}

static int contains_word(const char *input, const char *word) {
    const char *p = stristr(input, word);
    if (!p) return 0;
    if (p > input && isalpha((unsigned char)*(p-1))) return 0;
    int wlen = strlen(word);
    if (isalpha((unsigned char)p[wlen])) return 0;
    return 1;
}

static int any_contains(const char *input, const char **words, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (contains_word(input, words[i])) return 1;
    return 0;
}

void extract_features(const char *input, double *features) {
    for (int i = 0; i < NUM_FEATURES; i++) features[i] = 0.0;

    features[0] = any_contains(input, family_words, (int)N_FAMILY);

    int has_feeling = 0;
    for (size_t i = 0; i < N_FEELING_PHRASES; i++) {
        if (stristr(input, feeling_phrases[i])) { has_feeling = 1; break; }
    }
    int has_statement = 0;
    for (size_t i = 0; i < N_STATEMENT_PHRASES; i++) {
        if (stristr(input, statement_phrases[i])) { has_statement = 1; break; }
    }
    features[1] = (has_feeling || has_statement) ? 1.0 : 0.0;

    features[2] = contains_word(input, "you") ? 1.0 : 0.0;

    features[3] = (strchr(input, '?') != NULL) ? 1.0 : 0.0;

    features[4] = any_contains(input, because_words, N_BECAUSE);

    features[5] = any_contains(input, absolutist_words, N_ABSOLUTIST);

    features[6] = any_contains(input, emotion_words, N_EMOTIONS);

    features[7] = any_contains(input, dream_words, N_DREAM);

    features[8] = any_contains(input, strong_emotion_words, N_STRONG_EMOTION);

    int len = strlen(input);
    features[9] = (len < 15) ? 0.0 : (len < 80) ? 0.5 : 1.0;

    features[10] = any_contains(input, sorry_words, N_SORRY);

    features[11] = any_contains(input, negation_words, N_NEGATION);

    features[12] = any_contains(input, agreement_words, N_AGREEMENT);

    features[13] = any_contains(input, help_words, N_HELP);

    features[14] = any_contains(input, relationship_words, N_RELATIONSHIP);

    features[15] = any_contains(input, work_school_words, N_WORK_SCHOOL);
    features[16] = any_contains(input, health_body_words, N_HEALTH_BODY);
    features[17] = any_contains(input, future_words, N_FUTURE);
    features[18] = any_contains(input, gratitude_words, N_GRATITUDE);
    features[19] = any_contains(input, existential_words, N_EXISTENTIAL);
    features[20] = any_contains(input, trauma_words, N_TRAUMA);
}

//pronoun reflection
static const char *reflect_word(const char *word, int *changed) {
    *changed = 0;
    size_t len = strlen(word);
    char lower[64];
    for (size_t i = 0; i < len && i < 63; i++) lower[i] = tolower((unsigned char)word[i]);
    lower[len < 63 ? len : 63] = '\0';

    if (strcmp(lower, "i") == 0) {
        *changed = 1; 
        return "you"; 
    }
    if (strcmp(lower, "me") == 0) {
        *changed = 1; 
        return "you"; 
    }
    if (strcmp(lower, "my") == 0) {
        *changed = 1; 
        return "your"; 
    }
    if (strcmp(lower, "mine") == 0) {
        *changed = 1; 
        return "yours"; 
    }
    if (strcmp(lower, "myself") == 0) {
        *changed = 1; 
        return "yourself"; 
    }
    if (strcmp(lower, "am") == 0) {
        *changed = 1; 
        return "are"; 
    }
    if (strcmp(lower, "was") == 0) {
        *changed = 1; 
        return "were"; 
    }
    if (strcmp(lower, "i'm") == 0) {
        *changed = 1; 
        return "you're"; 
    }
    if (strcmp(lower, "i've") == 0) {
        *changed = 1; 
        return "you've"; 
    }
    if (strcmp(lower, "i'll") == 0) {
        *changed = 1; 
        return "you'll"; 
    }
    if (strcmp(lower, "i'd") == 0) {
        *changed = 1; 
        return "you'd"; 
    }
    if (strcmp(lower, "you") == 0) {
        *changed = 1; 
        return "I"; 
    }
    if (strcmp(lower, "your") == 0) {
        *changed = 1; 
        return "my"; 
    }
    if (strcmp(lower, "yours") == 0) {
        *changed = 1; 
        return "mine"; 
    }
    if (strcmp(lower, "yourself") == 0) {
        *changed = 1; 
        return "myself"; 
    }
    if (strcmp(lower, "are") == 0) {
        *changed = 1; 
        return "am"; 
    }
    if (strcmp(lower, "were") == 0) {
        *changed = 1; 
        return "was"; 
    }
    if (strcmp(lower, "you're") == 0) {
        *changed = 1; 
        return "i'm"; 
    }
    if (strcmp(lower, "you've") == 0) {
        *changed = 1; 
        return "i've"; 
    }
    if (strcmp(lower, "you'll") == 0) {
        *changed = 1; 
        return "i'll"; 
    }
    if (strcmp(lower, "you'd") == 0) {
        *changed = 1; 
        return "i'd"; 
    }

    return NULL;
}

static void reflect_pronouns(const char *input, char *out, int size) {
    char buf[2048];
    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *words[256];
    int nw = 0;
    char *tok = strtok(buf, " \t");
    while (tok && nw < 256) {
        words[nw++] = tok;
        tok = strtok(NULL, " \t");
    }

    int oi = 0;
    for (int i = 0; i < nw && oi < size - 1; i++) {
        if (i > 0) out[oi++] = ' ';

        int changed;
        const char *repl = reflect_word(words[i], &changed);

        if (repl) {
            int cap = 0;
            if (i == 0) {
                cap = 1;
            } else if (isupper((unsigned char)words[i][0])) {
                cap = 1;
            }
            const char *src = repl;
            if (cap) {
                out[oi++] = toupper((unsigned char)src[0]);
                src++;
            }
            while (*src && oi < size - 1) out[oi++] = *src++;
        } else {
            const char *src = words[i];
            while (*src && oi < size - 1) out[oi++] = *src++;
        }
    }
    out[oi] = '\0';
}

//memory
void memory_init(Memory *mem) {
    mem->count = 0;
    mem->next = 0;
    mem->turn = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mem->items[i].keyword[0] = '\0';
        mem->items[i].phrase[0] = '\0';
        mem->items[i].turn = -1;
    }
}

static int is_stop_word(const char *word) {
    const char *stop[] = {
        "like", "so", "very", "really", "just", "that", "this",
        "a", "an", "the", "and", "or", "but", "in", "on", "at",
        "to", "for", "of", "with", "by", "about", "because",
        "too", "also", "much", "many", "some", "any", "all",
        "it", "its", "itself", "up", "down", "out", "off", "over"
    };
    int n = sizeof(stop) / sizeof(stop[0]);
    for (int i = 0; i < n; i++)
        if (strcmp(word, stop[i]) == 0) return 1;
    return 0;
}

static void memory_add(Memory *mem, const char *input) {
    mem->turn++;
    const char *emotion = NULL;
    const char *family = NULL;
    for (size_t i = 0; i < N_EMOTIONS; i++) {
        if (contains_word(input, emotion_words[i])) { emotion = emotion_words[i]; break; }
    }
    for (size_t i = 0; i < N_FAMILY; i++) {
        if (contains_word(input, family_words[i])) { family = family_words[i]; break; }
    }

    const char *kw = NULL;
    char phrase[256] = "";

    if (family) {
        kw = family;
        snprintf(phrase, sizeof(phrase), "your %s", family);
    } else if (emotion) {
        kw = emotion;
        snprintf(phrase, sizeof(phrase), "feeling %s", emotion);
    } else {
        char after[256];
        const char *feel[] = { "i feel", "i am", "i'm", "i think", "i want", "i need" };
        after[0] = '\0';
        for (size_t i = 0; i < 6; i++) {
            const char *p = stristr(input, feel[i]);
            if (p) {
                const char *rest = p + strlen(feel[i]);
                while (*rest == ' ') rest++;
                int j;
                for (j = 0; j < 255 && rest[j] && rest[j] != '.' && rest[j] != '!'; j++)
                    after[j] = rest[j];
                after[j] = '\0';
                break;
            }
        }
        if (after[0]) {
            char *first = after;
            while (*first == ' ') first++;
            char *space = strchr(first, ' ');
            if (space) *space = '\0';
            if (strlen(first) > 1 && !is_stop_word(first)) {
                kw = first;
                snprintf(phrase, sizeof(phrase), "%s", after);
            }
        }
    }

    if (!kw || strlen(kw) < 2) return;
    if (contains_word(input, "hello") || contains_word(input, "hi") || contains_word(input, "hey")) return;

    int slot = mem->next;
    strncpy(mem->items[slot].keyword, kw, 63);
    mem->items[slot].keyword[63] = '\0';
    const char *phrase_src = phrase[0] ? phrase : kw;
    size_t plen = strlen(phrase_src);
    if (plen > 255) plen = 255;
    memcpy(mem->items[slot].phrase, phrase_src, plen);
    mem->items[slot].phrase[plen] = '\0';
    mem->items[slot].turn = mem->turn;
    mem->next = (mem->next + 1) % MEMORY_SIZE;
    if (mem->count < MEMORY_SIZE) mem->count++;
}

static int memory_get_oldest(Memory *mem, int max_age) {
    int best = -1;
    int oldest_turn = 999999;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (mem->items[i].turn < 0) continue;
        int age = mem->turn - mem->items[i].turn;
        if (age > 0 && age <= max_age && mem->items[i].turn < oldest_turn) {
            oldest_turn = mem->items[i].turn;
            best = i;
        }
    }
    return best;
}

static void build_memory_ref(Memory *mem, char *out, int size) {
    int idx = memory_get_oldest(mem, 10);
    if (idx < 0) { out[0] = '\0'; return; }

    const char *templates[] = {
        "Earlier you mentioned [PHRASE]. Tell me more about that.",
        "You brought up [PHRASE] before. How do you feel about that now?",
        "I remember you mentioned [PHRASE]. Can we explore that further?",
        "You spoke about [PHRASE] earlier. What comes to mind about it now?",
    };
    int n = sizeof(templates) / sizeof(templates[0]);
    const char *tmpl = templates[rand() % n];

    char phrase[256];
    const char *src = mem->items[idx].phrase;
    int pi = 0;
    while (*src && pi < 255) phrase[pi++] = *src++;
    phrase[pi] = '\0';

    out[0] = '\0';
    const char *p = tmpl;
    int oi = 0;
    while (*p && oi < size - 1) {
        if (strncmp(p, "[PHRASE]", 8) == 0) {
            const char *ph = phrase;
            while (*ph && oi < size - 1) out[oi++] = *ph++;
            p += 8;
        } else {
            out[oi++] = *p++;
        }
    }
    out[oi] = '\0';
}

//silence dection
static int is_silence(const char *input) {
    int len = strlen(input);
    if (len == 0) return 1;

    int alpha = 0;
    for (int i = 0; input[i]; i++) {
        if (isalpha((unsigned char)input[i])) alpha++;
    }
    if (alpha == 0) return 1;

    if (len < 4) return 1;

    for (size_t i = 0; i < N_SILENCE; i++) {
        if (strcmp(input, silence_words[i]) == 0) return 1;
    }

    if (strcmp(input, "...") == 0) return 1;
    if (strcmp(input, "??") == 0 || strcmp(input, "???") == 0) return 1;

    return 0;
}

//priority keyword override
static int check_priority(const char *input) {
    for (size_t i = 0; i < N_CRISIS; i++) {
        if (stristr(input, crisis_phrases[i])) return CRISIS_RESPONSE;
    }
    if (contains_word(input, "hate") || contains_word(input, "despise"))
        return CRISIS_RESPONSE;
    return -1;
}

static const char **template_lists[NUM_LABELS];
static int template_counts[NUM_LABELS];
static int fallback_last = -1;

static void init_templates(void) {
    template_lists[GREETING] = greeting_templates;
    template_counts[GREETING] = N_GREETING_T;
    template_lists[REFLECT_FEELING] = reflect_feeling_templates;
    template_counts[REFLECT_FEELING] = N_REFLECT_FEELING_T;
    template_lists[REFLECT_STATEMENT] = reflect_statement_templates;
    template_counts[REFLECT_STATEMENT] = N_REFLECT_STATEMENT_T;
    template_lists[QUESTION_OPEN] = question_open_templates;
    template_counts[QUESTION_OPEN] = N_QUESTION_OPEN_T;
    template_lists[QUESTION_FEELING] = question_feeling_templates;
    template_counts[QUESTION_FEELING] = N_QUESTION_FEELING_T;
    template_lists[ENCOURAGE] = encourage_templates;
    template_counts[ENCOURAGE] = N_ENCOURAGE_T;
    template_lists[FAMILY_REFLECT] = family_templates;
    template_counts[FAMILY_REFLECT] = (int)N_FAMILY_T;
    template_lists[DREAM_REFLECT] = dream_templates;
    template_counts[DREAM_REFLECT] = N_DREAM_T;
    template_lists[TRANSPERENCE] = transference_templates;
    template_counts[TRANSPERENCE] = N_TRANSPERENCE_T;
    template_lists[CRISIS_RESPONSE] = crisis_templates;
    template_counts[CRISIS_RESPONSE] = N_CRISIS_T;
    template_lists[WORK_SCHOOL] = work_school_templates;
    template_counts[WORK_SCHOOL] = N_WORK_SCHOOL_T;
    template_lists[HEALTH_BODY] = health_body_templates;
    template_counts[HEALTH_BODY] = N_HEALTH_BODY_T;
    template_lists[SELF_ESTEEM] = self_esteem_templates;
    template_counts[SELF_ESTEEM] = N_SELF_ESTEEM_T;
    template_lists[EXISTENTIAL] = existential_templates;
    template_counts[EXISTENTIAL] = N_EXISTENTIAL_T;
    template_lists[DEFAULT_KIND] = default_templates;
    template_counts[DEFAULT_KIND] = N_DEFAULT_T;
}

static int rand_range(int max) {
    if (max <= 0) return 0;
    return rand() % max;
}

//fallback chain (weighted, no repeat)
static const char *pick_fallback(void) {
    int n = (int)N_FALLBACK;
    int weights[N_FALLBACK];
    for (int i = 0; i < n; i++) {
        weights[i] = (i == fallback_last) ? 1 : 3;
    }
    int total = 0;
    for (int i = 0; i < n; i++) total += weights[i];
    int r = rand() % total;
    int cum = 0;
    for (int i = 0; i < n; i++) {
        cum += weights[i];
        if (r < cum) { fallback_last = i; return fallback_pool[i]; }
    }
    fallback_last = 0;
    return fallback_pool[0];
}

//keyword extraction
static const char *find_first_emotion(const char *input) {
    for (size_t i = 0; i < N_EMOTIONS; i++) {
        if (contains_word(input, emotion_words[i]))
            return emotion_words[i];
    }
    return NULL;
}

static const char *find_first_family(const char *input) {
    for (size_t i = 0; i < N_FAMILY; i++) {
        if (contains_word(input, family_words[i]))
            return family_words[i];
    }
    return NULL;
}

static void extract_after_phrase(const char *input, const char **phrases, int n_phrases, char *out, int out_size) {
    const char *best = NULL;
    int best_len = -1;
    for (int i = 0; i < n_phrases; i++) {
        const char *p = stristr(input, phrases[i]);
        if (p) {
            int plen = strlen(phrases[i]);
            const char *rest = p + plen;
            while (*rest == ' ') rest++;
            int rlen = strlen(rest);
            if (rlen > best_len) {
                best_len = rlen;
                best = rest;
            }
        }
    }
    if (best) {
        int i;
        for (i = 0; i < out_size - 1 && best[i] && best[i] != '.' && best[i] != '!' && best[i] != '?'; i++)
            out[i] = best[i];
        out[i] = '\0';
    } else {
        out[0] = '\0';
    }
}

//template filling
static void fill_template(const char *tmpl, const char *input, char *out, int out_size) {
    const char *emotion = find_first_emotion(input);
    const char *family = find_first_family(input);
    char phrase[256];
    phrase[0] = '\0';

    const char *all_feel_phrases[] = {
        "i feel", "i am feeling", "i'm feeling",
        "i have been feeling", "i've been feeling",
        "i feel so", "i feel very",
        "i am", "i'm"
    };
    extract_after_phrase(input, all_feel_phrases, 9, phrase, sizeof(phrase));

    if (phrase[0] == '\0') {
        const char *all_state_phrases[] = {
            "i think", "i believe", "i want", "i need",
            "i wish", "i hope"
        };
        extract_after_phrase(input, all_state_phrases, 6, phrase, sizeof(phrase));
    }

    if (phrase[0] == '\0') {
        const char *family_check[] = { "my" };
        extract_after_phrase(input, family_check, 1, phrase, sizeof(phrase));
    }

    if (phrase[0] == '\0' && emotion) {
        snprintf(phrase, sizeof(phrase), "%s", emotion);
    }

    if (phrase[0] == '\0') {
        snprintf(phrase, sizeof(phrase), "that");
    }

    char reflected[512] = "";
    reflect_pronouns(input, reflected, sizeof(reflected));

    out[0] = '\0';
    const char *p = tmpl;
    int oi = 0;
    while (*p && oi < out_size - 1) {
        if (strncmp(p, "[EMOTION]", 9) == 0) {
            const char *e = emotion ? emotion : "that way";
            while (*e && oi < out_size - 1) out[oi++] = *e++;
            p += 9;
        } else if (strncmp(p, "[FAMILY]", 8) == 0) {
            const char *f = family ? family : "family";
            while (*f && oi < out_size - 1) out[oi++] = *f++;
            p += 8;
        } else if (strncmp(p, "[PHRASE]", 8) == 0) {
            const char *ph = phrase;
            while (*ph && oi < out_size - 1) out[oi++] = *ph++;
            p += 8;
        } else if (strncmp(p, "[REFLECTED]", 11) == 0) {
            const char *r = reflected;
            while (*r && oi < out_size - 1) out[oi++] = *r++;
            p += 11;
        } else {
            out[oi++] = *p++;
        }
    }
    out[oi] = '\0';
}

//response generation
char *generate_response(TreeNode *tree, const char *input, Memory *mem) {
    //silence check
    if (is_silence(input)) {
        int idx = rand_range(N_SILENCE_R);
        char *resp = malloc(MAX_RESPONSE);
        snprintf(resp, MAX_RESPONSE, "%s", silence_responses[idx]);
        return resp;
    }

    //pwriority keyword override
    int override = check_priority(input);
    if (override >= 0) {
        int label = override;
        if (label < 0 || label >= NUM_LABELS) label = DEFAULT_KIND;
        int idx = rand_range(template_counts[label]);
        const char *tmpl = template_lists[label][idx];
        char *response = malloc(MAX_RESPONSE);
        fill_template(tmpl, input, response, MAX_RESPONSE);
        if (mem) memory_add(mem, input);
        return response;
    }

    //tree prediction
    double features[NUM_FEATURES];
    extract_features(input, features);
    int label = predict_tree(tree, features);
    if (label < 0 || label >= NUM_LABELS) label = DEFAULT_KIND;

    //memory reference
    int use_memory = 0;
    char mem_ref[512] = "";
    if (label != GREETING && label != CRISIS_RESPONSE && mem && mem->count > 0 && rand() % 5 == 0) {
        build_memory_ref(mem, mem_ref, sizeof(mem_ref));
        if (mem_ref[0]) use_memory = 1;
    }

    //build response
    char response[MAX_RESPONSE];
    response[0] = '\0';

    if (use_memory) {
        strncat(response, mem_ref, MAX_RESPONSE - strlen(response) - 1);
        strncat(response, " ", MAX_RESPONSE - strlen(response) - 1);
    }

    char core[MAX_RESPONSE];
    if (label == DEFAULT_KIND) {
        const char *fb = pick_fallback();
        snprintf(core, sizeof(core), "%s", fb);
    } else {
        int idx = rand_range(template_counts[label]);
        const char *tmpl = template_lists[label][idx];
        fill_template(tmpl, input, core, sizeof(core));
    }

    strncat(response, core, MAX_RESPONSE - strlen(response) - 1);

    //update memory
    if (mem) memory_add(mem, input);

    char *result = malloc(strlen(response) + 1);
    strcpy(result, response);
    return result;
}

//data generation
static char *gen_variant(const char *pattern) {
    char *out = malloc(256);
    out[0] = '\0';

    const char *p = pattern;
    int oi = 0;
    while (*p && oi < 255) {
        if (strncmp(p, "[EMOTION]", 9) == 0) {
            const char *e = emotion_words[rand() % (int)N_EMOTIONS];
            while (*e && oi < 255) out[oi++] = *e++;
            p += 9;
        } else if (strncmp(p, "[FAMILY]", 8) == 0) {
            const char *f = family_words[rand() % (int)N_FAMILY];
            while (*f && oi < 255) out[oi++] = *f++;
            p += 8;
        } else {
            out[oi++] = *p++;
        }
    }
    out[oi] = '\0';
    return out;
}

typedef struct {
    const char *pattern;
    int label;
    int weight;
} GenRule;

static GenRule gen_rules[] = {
    //greeting
    { "hello", GREETING, 6 },
    { "hi", GREETING, 6 },
    { "hey", GREETING, 6 },
    { "how are you", GREETING, 4 },
    { "good morning", GREETING, 3 },
    { "good evening", GREETING, 3 },
    { "sup", GREETING, 3 },
    { "goodbye", GREETING, 2 },
    { "bye", GREETING, 2 },
    { "see you", GREETING, 2 },

    //reflect_feeling
    { "I feel [EMOTION]", REFLECT_FEELING, 18 },
    { "I feel so [EMOTION]", REFLECT_FEELING, 14 },
    { "I am feeling [EMOTION]", REFLECT_FEELING, 12 },
    { "I'm feeling [EMOTION]", REFLECT_FEELING, 14 },
    { "I've been feeling [EMOTION]", REFLECT_FEELING, 10 },
    { "lately I've been [EMOTION]", REFLECT_FEELING, 8 },
    { "I am [EMOTION]", REFLECT_FEELING, 15 },
    { "I am so [EMOTION]", REFLECT_FEELING, 12 },
    { "I'm [EMOTION]", REFLECT_FEELING, 15 },
    { "I'm so [EMOTION]", REFLECT_FEELING, 10 },
    { "I think I'm [EMOTION]", REFLECT_FEELING, 6 },
    { "I'm feeling really [EMOTION]", REFLECT_FEELING, 6 },
    { "everything feels [EMOTION]", REFLECT_FEELING, 4 },
    { "I feel so [EMOTION] lately", REFLECT_FEELING, 5 },
    { "things are [EMOTION]", REFLECT_FEELING, 3 },

    //reflect_statement
    { "I think that's true", REFLECT_STATEMENT, 6 },
    { "I think I know", REFLECT_STATEMENT, 6 },
    { "I believe in that", REFLECT_STATEMENT, 5 },
    { "I want to be better", REFLECT_STATEMENT, 6 },
    { "I wish I could change", REFLECT_STATEMENT, 5 },
    { "I know what you mean", REFLECT_STATEMENT, 4 },
    { "I think you're right", REFLECT_STATEMENT, 4 },
    { "I think about it all the time", REFLECT_STATEMENT, 4 },
    { "I can't stop thinking", REFLECT_STATEMENT, 4 },
    { "I've been through a lot", REFLECT_STATEMENT, 4 },
    { "I feel like giving up", REFLECT_STATEMENT, 3 },
    { "I know I should", REFLECT_STATEMENT, 3 },
    { "I think I need to change", REFLECT_STATEMENT, 3 },

    //question_open
    { "why do I feel this way", QUESTION_OPEN, 6 },
    { "what should I do", QUESTION_OPEN, 6 },
    { "how can I change", QUESTION_OPEN, 5 },
    { "why is this happening", QUESTION_OPEN, 5 },
    { "what does it mean", QUESTION_OPEN, 4 },
    { "should I tell them", QUESTION_OPEN, 4 },
    { "what's wrong with me", QUESTION_OPEN, 5 },
    { "why can't I be normal", QUESTION_OPEN, 4 },
    { "will I ever get better", QUESTION_OPEN, 4 },
    { "is this normal", QUESTION_OPEN, 3 },
    { "what do I do", QUESTION_OPEN, 3 },

    //question_feeling
    { "why am I so [EMOTION]", QUESTION_FEELING, 8 },
    { "why do I feel [EMOTION]", QUESTION_FEELING, 8 },
    { "why am I always [EMOTION]", QUESTION_FEELING, 5 },
    { "why do I get [EMOTION]", QUESTION_FEELING, 4 },
    { "why does this make me [EMOTION]", QUESTION_FEELING, 3 },
    { "why is life so [EMOTION]", QUESTION_FEELING, 3 },

    //encourage
    { "I can't do this", ENCOURAGE, 6 },
    { "I'm not good enough", ENCOURAGE, 6 },
    { "I failed", ENCOURAGE, 5 },
    { "I don't know what to do", ENCOURAGE, 6 },
    { "I give up", ENCOURAGE, 4 },
    { "I'm so tired of this", ENCOURAGE, 5 },
    { "I need help", ENCOURAGE, 6 },
    { "I'm hopeless", ENCOURAGE, 4 },
    { "please help me", ENCOURAGE, 5 },
    { "everything is terrible", ENCOURAGE, 4 },
    { "nothing matters anymore", ENCOURAGE, 3 },
    { "my life is a mess", ENCOURAGE, 4 },
    { "I'm so tired of everything", ENCOURAGE, 4 },
    { "I can't take it anymore", ENCOURAGE, 4 },
    { "leave me alone", ENCOURAGE, 3 },
    { "I don't care anymore", ENCOURAGE, 3 },
    { "help", ENCOURAGE, 4 },
    { "I can't", ENCOURAGE, 3 },
    { "I'm done", ENCOURAGE, 3 },

    //family_reflect
    { "my [FAMILY] is always", FAMILY_REFLECT, 10 },
    { "my [FAMILY] never", FAMILY_REFLECT, 8 },
    { "I hate my [FAMILY]", FAMILY_REFLECT, 6 },
    { "I love my [FAMILY]", FAMILY_REFLECT, 5 },
    { "my [FAMILY] doesn't understand", FAMILY_REFLECT, 8 },
    { "I have issues with my [FAMILY]", FAMILY_REFLECT, 6 },
    { "my [FAMILY] left", FAMILY_REFLECT, 4 },
    { "I miss my [FAMILY]", FAMILY_REFLECT, 4 },
    { "my [FAMILY] is so annoying", FAMILY_REFLECT, 4 },
    { "I wish my [FAMILY] understood", FAMILY_REFLECT, 4 },
    { "my [FAMILY] is controlling", FAMILY_REFLECT, 3 },
    { "my [FAMILY] doesn't care", FAMILY_REFLECT, 4 },
    { "I don't get along with my [FAMILY]", FAMILY_REFLECT, 4 },

    //dream_reflect
    { "I had a dream", DREAM_REFLECT, 6 },
    { "I had a nightmare", DREAM_REFLECT, 5 },
    { "I dreamt about flying", DREAM_REFLECT, 4 },
    { "I keep having dreams", DREAM_REFLECT, 4 },
    { "last night I dreamed", DREAM_REFLECT, 5 },
    { "I had a weird dream", DREAM_REFLECT, 3 },
    { "I keep having nightmares", DREAM_REFLECT, 4 },

    //transperence
    { "you don't understand", TRANSPERENCE, 6 },
    { "you don't get it", TRANSPERENCE, 5 },
    { "you are not listening", TRANSPERENCE, 4 },
    { "you sound like my [FAMILY]", TRANSPERENCE, 5 },
    { "why are you asking", TRANSPERENCE, 4 },
    { "you keep saying that", TRANSPERENCE, 3 },
    { "fuck you", TRANSPERENCE, 4 },
    { "shut up", TRANSPERENCE, 3 },
    { "you're annoying", TRANSPERENCE, 3 },
    { "you don't know me", TRANSPERENCE, 3 },
    { "stop", TRANSPERENCE, 2 },
    { "go away", TRANSPERENCE, 3 },
    { "you can't help me", TRANSPERENCE, 3 },
    { "you don't care", TRANSPERENCE, 3 },

    //default_kind
    { "I don't know", DEFAULT_KIND, 6 },
    { "maybe", DEFAULT_KIND, 4 },
    { "whatever", DEFAULT_KIND, 3 },
    { "I guess so", DEFAULT_KIND, 4 },
    { "not really", DEFAULT_KIND, 4 },
    { "yes", DEFAULT_KIND, 4 },
    { "no", DEFAULT_KIND, 4 },
    { "okay", DEFAULT_KIND, 4 },
    { "tell me more", DEFAULT_KIND, 3 },
    { "I'm not sure", DEFAULT_KIND, 5 },
    { "fine", DEFAULT_KIND, 3 },
    { "alright", DEFAULT_KIND, 3 },
    { "nothing", DEFAULT_KIND, 3 },
    { "I'm fine", DEFAULT_KIND, 4 },
    { "it's complicated", DEFAULT_KIND, 3 },
    { "long story", DEFAULT_KIND, 2 },
    { "forget it", DEFAULT_KIND, 2 },
    { "doesn't matter", DEFAULT_KIND, 3 },
    { "I guess", DEFAULT_KIND, 3 },
    { "same old", DEFAULT_KIND, 2 },

    //work_school
    { "I hate my job", WORK_SCHOOL, 5 },
    { "my job is terrible", WORK_SCHOOL, 4 },
    { "work is stressful", WORK_SCHOOL, 5 },
    { "my boss is a jerk", WORK_SCHOOL, 4 },
    { "I'm so stressed about work", WORK_SCHOOL, 4 },
    { "I can't focus at work", WORK_SCHOOL, 3 },
    { "school is overwhelming", WORK_SCHOOL, 4 },
    { "I'm failing my classes", WORK_SCHOOL, 3 },
    { "my teacher hates me", WORK_SCHOOL, 3 },
    { "I have too much work", WORK_SCHOOL, 3 },
    { "I'm overwhelmed at school", WORK_SCHOOL, 3 },
    { "work is draining me", WORK_SCHOOL, 3 },

    //health_body
    { "I've been sick", HEALTH_BODY, 4 },
    { "my health is bad", HEALTH_BODY, 4 },
    { "I can't sleep", HEALTH_BODY, 5 },
    { "I'm always in pain", HEALTH_BODY, 4 },
    { "my body hurts", HEALTH_BODY, 3 },
    { "I think I need a doctor", HEALTH_BODY, 3 },
    { "I'm worried about my health", HEALTH_BODY, 4 },
    { "my health is getting worse", HEALTH_BODY, 3 },
    { "I have a headache", HEALTH_BODY, 2 },
    { "I can't eat", HEALTH_BODY, 3 },
    { "I'm not sleeping well", HEALTH_BODY, 4 },

    //self_esteem
    { "I'm ugly", SELF_ESTEEM, 4 },
    { "I look terrible", SELF_ESTEEM, 3 },
    { "I'm a loser", SELF_ESTEEM, 4 },
    { "nobody likes me", SELF_ESTEEM, 4 },
    { "everyone hates me", SELF_ESTEEM, 3 },
    { "I'm so ugly", SELF_ESTEEM, 3 },
    { "I hate how I look", SELF_ESTEEM, 3 },
    { "I'm not good at anything", SELF_ESTEEM, 4 },
    { "I always mess everything up", SELF_ESTEEM, 3 },
    { "I'm so awkward", SELF_ESTEEM, 3 },
    { "I hate myself", SELF_ESTEEM, 4 },
    { "I'm not lovable", SELF_ESTEEM, 3 },
    { "I'm a disappointment", SELF_ESTEEM, 3 },
    { "I'm so stupid", SELF_ESTEEM, 3 },

    //existential
    { "what's the point of life", EXISTENTIAL, 4 },
    { "why are we here", EXISTENTIAL, 3 },
    { "life has no meaning", EXISTENTIAL, 4 },
    { "what's it all about", EXISTENTIAL, 3 },
    { "I think about death a lot", EXISTENTIAL, 3 },
    { "do you believe in god", EXISTENTIAL, 2 },
    { "everything feels meaningless", EXISTENTIAL, 4 },
    { "I don't know why I exist", EXISTENTIAL, 3 },
    { "what happens after we die", EXISTENTIAL, 2 },
    { "life feels empty", EXISTENTIAL, 3 },
};

#define N_GEN_RULES (sizeof(gen_rules)/sizeof(gen_rules[0]))

static int pick_rule(int n_rules, int total_weight) {
    int r = rand() % total_weight;
    int cum = 0;
    for (int j = 0; j < n_rules; j++) {
        cum += gen_rules[j].weight;
        if (r < cum) return j;
    }
    return n_rules - 1;
}

Dataset *generate_synthetic_data(int count) {
    Dataset *data = alloc_dataset(count, NUM_FEATURES, NUM_LABELS - 1);

    int total_weight = 0;
    int n_rules = (int)N_GEN_RULES;
    for (int i = 0; i < n_rules; i++) total_weight += gen_rules[i].weight;

    for (int i = 0; i < count; i++) {
        int rule_idx = pick_rule(n_rules, total_weight);
        char *text = gen_variant(gen_rules[rule_idx].pattern);
        int label = gen_rules[rule_idx].label;

        //25% chance to merge two patterns, creates overlapping, ambiguous examples
        if (rand() % 4 == 0) {
            int r2 = pick_rule(n_rules, total_weight);
            char *text2 = gen_variant(gen_rules[r2].pattern);
            char *merged = malloc(strlen(text) + strlen(text2) + 4);
            sprintf(merged, "%s. %s", text, text2);
            free(text);
            free(text2);
            text = merged;
        }

        //15% chance to flip label to noise
        if (rand() % 100 < 15) {
            int alt_labels[] = {
                DEFAULT_KIND, REFLECT_STATEMENT, ENCOURAGE, QUESTION_OPEN,
                WORK_SCHOOL, HEALTH_BODY, SELF_ESTEEM, EXISTENTIAL
            };
            label = alt_labels[rand() % 8];
        }

        //extract features from (possibly merged) text
        extract_features(text, data->features[i]);
        data->labels[i] = label;

        //10% per-feature corruption: flip binary features
        for (int f = 0; f < NUM_FEATURES; f++) {
            if (f == 9) continue; //keep lenght clean
            if (rand() % 100 < 10)
                data->features[i][f] = (data->features[i][f] > 0.5) ? 0.0 : 1.0;
        }

        free(text);
    }

    return data;
}

//interactive loop
void run_interactive(TreeNode *tree) {
    init_templates();

    Memory mem;
    memory_init(&mem);

    printf("\n");
    printf("ELIZA-Tree:\n");

    char *greeting = generate_response(tree, "hello", &mem);
    printf("ELIZA: %s\n", greeting);
    free(greeting);

    char input[MAX_INPUT];
    while (1) {
        printf("You: ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("ELIZA: Goodbye. Take care of yourself.\n");
            break;
        }

        if (strlen(input) == 0) continue;

        char *response = generate_response(tree, input, &mem);
        printf("ELIZA: %s\n", response);
        free(response);
    }
}
