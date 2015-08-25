#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

#define MAX_WORD    10000
#define MAX_NUM_WORD    10000

char *buffer;

char word[MAX_WORD];

void process(char *in_path) {
    FILE *f_in = fopen(in_path, "r"); 
    
    int star;
    size_t buffer_size;
    int N = 0;
    int total_words = 0;
    int longest_word = -1;
    int longest_sentence = -1;
    int shortest_sentence = 100000000;
    while (fscanf(f_in, "%d", &star) != EOF) {
        //if (N % 100 == 0) printf("%d\n", N);
        ++ N;
        fgetc(f_in);
        getline(&buffer, &buffer_size, f_in);
        int counter = 0;
        int offset = 0;
        while (sscanf(buffer + offset, "%s", word) != EOF) {
            ++ counter;
            int l = strlen(word);

            if (longest_word < l) {
                longest_word = l;
                printf("%s\n", word);
            }
            //longest_word = max(longest_word, l);
            offset += 1 + l;
        }
        longest_sentence = max(longest_sentence, counter);
        shortest_sentence = min(shortest_sentence, counter);
        total_words += counter;
    }
    double avg = (double)total_words / N;
    printf("longest word = %d\n", longest_word);
    printf("#words in shortest sentence = %d\n", shortest_sentence);
    printf("#words in longest sentence = %d\n", longest_sentence);
    printf("#words average = %.4lf\n", avg);

    fclose(f_in);
}

// ./xxx input_file
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "missing parameters\n");
        return -1;
    }
    process(argv[1]);

    return 0;
}
