#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// forward declaration
void recommender_compute(int user_id, double **utility_matrix);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <start_uid> <end_uid> [repeats]\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);
    int end   = atoi(argv[2]);
    int reps  = (argc > 3) ? atoi(argv[3]) : 1;

    // Load + normalize ONCE (important change)
    double **utility_matrix = load_utility_matrix();
    normalize_matrix(utility_matrix);

    clock_t t1 = clock();

    for (int r = 0; r < reps; r++) {
        for (int uid = start; uid <= end; uid++) {
            recommender_compute(uid, utility_matrix);
        }
    }

    clock_t t2 = clock();
    printf("Execution time: %.3f sec\n",
           (double)(t2 - t1) / CLOCKS_PER_SEC);

    free_matrix(utility_matrix);
    return 0;
}