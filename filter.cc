#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

#define MAX_MVd     50
#define MAX_MVn     125000
#define MAX_DICT_WORD    100
#define MAX_WORD    1000
#define MAX_NUM_WORD    500

int WVn, WVd;

int maxlen;

struct WVnode{
    double vector[MAX_MVd];
    char word[MAX_DICT_WORD];
} WV[MAX_MVn];


bool cmp(const struct WVnode &A, const struct WVnode &B) {
    return strcmp(A.word, B.word) < 0;
}


void loading_wordvectors(char *filepath) {
    FILE *f = fopen(filepath, "r");
    fscanf(f, "%d %d", &WVn, &WVd);
    int maxlen = -1;
    for (int i = 0; i < WVn; ++ i) {
        fscanf(f, "%s", WV[i].word);
        int l = strlen(WV[i].word);
        if (l > maxlen)
            maxlen = l;
        for (int j = 0; j < WVd; ++ j)
            fscanf(f, "%lf", &WV[i].vector[j]);
    }
    fclose(f);
    fprintf(stdout, "longest word = %d\n", maxlen);
    sort(WV, WV + WVn, cmp);

    /*
    for (int i = 0; i < WVn; ++ i)
        printf("%s\n", WV[i].word);
    */
}

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

char *buffer;

double *list[MAX_NUM_WORD];

void process(char *in_path, char *out_path) {
    FILE *f_in = fopen(in_path, "r"); 
    FILE *f_out = fopen(out_path, "w"); 
    
    int star;
    size_t buffer_size;
    char word[MAX_WORD];
    fprintf(f_out, "%d %d\n", WVd, maxlen);
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
                if (counter > maxlen) break;
            }
            offset += 1 + strlen(word);
        }
        //printf("%s\n", buffer);
        if (counter == 0 || counter > maxlen) continue;
        ++ N;
        if (N % 1000 == 0) printf("%d\n", N);
        fprintf(f_out, "%d\n", star);
        //counter = (counter > maxlen) ? maxlen : counter;
        for (int i = 0; i < counter; ++ i) {
            for (int j = 0; j < WVd - 1; ++ j)
                fprintf(f_out, "%.6lf ", list[i][j]);
            fprintf(f_out, "%.6lf\n", list[i][WVd - 1]);
        }
        if (counter < maxlen) {
            int k = maxlen - counter;
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

// ./xxx word_vectors_file input_file output_file sentence_length
int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "missing parameters\n");
        return -1;
    }
    printf("loading word vectors...\n");
    loading_wordvectors(argv[1]);
    printf("OK!\n");

    sscanf(argv[4], "%d", &maxlen);
    printf("max length of sentences = %d\n", maxlen);

    printf("generate word vectors for sentences...\n");
    process(argv[2], argv[3]);
    printf("OK!\n");

    return 0;
}
