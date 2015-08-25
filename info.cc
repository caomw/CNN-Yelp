#include <cstdio>
#include <cstring>

using namespace std;

FILE *dataF = fopen("data", "r");

int N;

char *text;

int main() {
    N = 0;
    int star;
    int total = 0;
    size_t buffer_size;
    int maxlen = 0;
    int low_star = 10000000;
    int high_star = -1;
    while (fscanf(dataF, "%d", &star) != EOF) {
        fgetc(dataF);
        ++ N;
        total += star;
        if (star > high_star) high_star = star;
        if (star < low_star) low_star = star;
        getline(&text, &buffer_size, dataF);
        int len = int(strlen(text));
        if (len > maxlen) maxlen = len;
    }
    printf("#reviews = %d\n", N);
    printf("lowest star = %d, highest star = %d\n", low_star, high_star);
    printf("average star = %.3lf\n", (double)total / N);
    printf("longest review - #characters = %d\n", maxlen);
    fclose(dataF);
    return 0;
}
