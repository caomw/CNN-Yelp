#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>

using namespace std;

#define MAX_MVd     50
#define MAX_MVn     125000
#define MAX_DICT_WORD    100
#define MAX_WORD    1000
#define MAX_NUM_WORD    150

int WVn, WVd;

double vectors[MAX_MVn][MAX_MVd];

#define TrieAlphabet    (26+10+1)


struct TrieNode {
    double* vector;
    struct TrieNode *next[TrieAlphabet];
};

TrieNode *Trie;

unordered_map<char, int> char2int;

void initTrie() {
    char2int.clear();
    int index = 0;
    for (int i = 0; i < 26; ++ i) {
        char c = char(i + 'a');
        char2int[c] = index ++;
    }
    for (int i = 0; i < 10; ++ i) {
        char c = char(i + '0');
        char2int[i] = index ++;
    }
    char2int['_'] = index ++;
    Trie = (struct TrieNode *)calloc(1, sizeof(struct TrieNode));
}

double *query_word(char *Wptr) {
    struct TrieNode *Tptr = Trie;
    for (; *Wptr; ++ Wptr) {
        int index = char2int[*Wptr];
        Tptr = Tptr->next[index];
        if (Tptr == NULL) return NULL;
    }
    return Tptr->vector;
}

void insertTrie(char *Wptr, double *vector) {
    struct TrieNode *Tptr = Trie;
    for (; *Wptr; ++ Wptr) {
        //printf("%c", *Wptr);
        int index = char2int[*Wptr];
        if (Tptr->next[index] == NULL)
            Tptr->next[index] = (struct TrieNode *)calloc(1, sizeof(struct TrieNode));
        Tptr = Tptr->next[index];
    }
    Tptr->vector = vector;
}

char Word[MAX_DICT_WORD];

void loading_wordvectors(char *filepath) {
    initTrie();
    printf("initialized Trie\n");
    FILE *f = fopen(filepath, "r");
    fscanf(f, "%d %d", &WVn, &WVd);
    int maxlen = -1;
    for (int i = 0; i < WVn; ++ i) {
        fscanf(f, "%s", Word);
        int l = strlen(Word);
        if (l > maxlen)
            maxlen = l;
        for (int j = 0; j < WVd; ++ j)
            fscanf(f, "%lf", &vectors[i][j]);
       insertTrie(Word, vectors[i]); 
    }
    fclose(f);
    fprintf(stdout, "longest word = %d\n", maxlen);

    /*
    for (int i = 0; i < WVn; ++ i)
        printf("%s\n", WV[i].word);
    */
}

/*
double *query_word(char *word) {
    int L = 0, R = WVn;
    while (L + 1 < R) {
        int mid = (L + R) >> 1;
        if (strcmp(word, WV[mid].word) < 0)
            R = mid;
        else
            L = mid;
    }
    if (strcmp(WV[L].word, word) != 0) {
        //fprintf(stderr, "binary search: cannnot find the word %s\n", word);
        return NULL;
    }
    return WV[L].vector;
}
*/

char *buffer;

double *list[MAX_NUM_WORD];

void process(char *in_path, char *out_path) {
    FILE *f_in = fopen(in_path, "r"); 
    FILE *f_out = fopen(out_path, "w"); 
    
    int star;
    size_t buffer_size;
    char word[MAX_WORD];
    fprintf(f_out, "%d %d\n", WVd, MAX_NUM_WORD);
    int N = 0;
    while (fscanf(f_in, "%d", &star) != EOF) {
        fgetc(f_in);
        getline(&buffer, &buffer_size, f_in);
        int counter = 0;
        int offset = 0;
        while (sscanf(buffer + offset, "%s", word) != EOF) {
            //printf("%s\n", word);
            double *vector = query_word(word);
            if (vector != NULL) {
                list[counter] = vector;
                ++ counter;
                if (counter >= MAX_NUM_WORD) break;
            }
            offset += 1 + strlen(word);
        }
        if (counter == 0) continue;
        ++ N;
        if (N % 1000 == 0) printf("%d\n", N);
        fprintf(f_out, "%d\n", star);
        counter = (counter > MAX_NUM_WORD) ? MAX_NUM_WORD : counter;
        for (int i = 0; i < counter; ++ i) {
            for (int j = 0; j < WVd - 1; ++ j)
                fprintf(f_out, "%.6lf ", list[i][j]);
            fprintf(f_out, "%.6lf\n", list[i][WVd - 1]);
        }
        if (counter < MAX_NUM_WORD) {
            int k = MAX_NUM_WORD - counter;
            while (k --) {
                for (int j = 0; j < WVd - 1; ++ j)
                    fprintf(f_out, "%.6lf ", double(0));
                fprintf(f_out, "%.6lf\n", double(0));
            }
        }
    }

    fclose(f_in);
    fclose(f_out);
}

// ./xxx word_vectors_file input_file output_file
int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "missing parameters\n");
        return -1;
    }
    printf("loading word vectors...\n");
    loading_wordvectors(argv[1]);
    printf("complete!\n");

    process(argv[2], argv[3]);

    return 0;
}
