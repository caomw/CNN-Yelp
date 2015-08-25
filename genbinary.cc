#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

FILE *dataF = fopen("data", "r");

int N, total;

char *text;

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "missing parameters\n");
        return -1;
    }
    FILE *sampleF = fopen(argv[2], "w");
    srand(time(NULL));
    N = total = 0;
    int star;
    size_t buffer_size;
    int prob;
    sscanf(argv[1], "%d", &prob);
    while (fscanf(dataF, "%d", &star) != EOF) {
        fgetc(dataF);
        getline(&text, &buffer_size, dataF);
        //printf("%d\n", N);
        //if (star != 1 && star != 5) continue;
        if (star == 3) continue;
        ++ N;
        if (star == 1 || star == 2)
            star = 0;
        else if (star == 4 || star == 5)
            star = 1;
        if (rand() % 1000 + 1 > prob) continue;
        ++ total;
        fprintf(sampleF, "%d\n", star);
        fprintf(sampleF, "%s", text);
    }
    double ratio = (double)total / N * 100.0;
    printf("#reviews in sample = %d\n", total);
    printf("ratio = %.3lf%%\n", ratio);
    fclose(dataF);
    fclose(sampleF);
    return 0;
}
