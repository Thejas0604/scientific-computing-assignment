#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// your existing headers
#include "utility_matrix.h"
#include "matrix_normalization.h"
#include "helpers.h"

#define NO_OF_MOVIES 9125
void recommender_compute(int userid,
                         double *utility_matrix,
                         double *normalized_matrix,
                         char *movienames,
                         char *moviegenres,
                         int No_of_users);


int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <start_uid> <end_uid> [repeats]\n", argv[0]);
        return 1;
    }

    int start_uid = atoi(argv[1]);
    int end_uid   = atoi(argv[2]);
    int repeats   = (argc > 3) ? atoi(argv[3]) : 1;

    // 🔹 determine number of users once
    int No_of_users = findusers();

    printf("Users: %d | Movies: %d\n", No_of_users, NO_OF_MOVIES);
    printf("Running from UID %d to %d (%d repeats)\n",
           start_uid, end_uid, repeats);

    // 🔹 allocate memory ONCE
    double *utility_matrix =
        malloc(sizeof(double) * No_of_users * NO_OF_MOVIES);

    double *normalized_matrix =
        malloc(sizeof(double) * No_of_users * NO_OF_MOVIES);

    char *movienames =
        malloc(sizeof(char) * NO_OF_MOVIES * 1024);

    char *moviegenres =
        malloc(sizeof(char) * NO_OF_MOVIES * 1024);

    if (!utility_matrix || !normalized_matrix ||
        !movienames || !moviegenres)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // 🔹 load dataset ONCE
    get_utility_matrix(
        utility_matrix,
        "/mnt/e/coding-projects/scientific-computing-assignment/Dataset/ratings_learn.csv",
        NO_OF_MOVIES,
        No_of_users,
        0 // user id not needed here
    );

    get_movie_names(
        movienames,
        "/mnt/e/coding-projects/scientific-computing-assignment/Dataset/movies.csv"
    );

    get_movie_genres(
        moviegenres,
        "/mnt/e/coding-projects/scientific-computing-assignment/Dataset/movies_genres.csv"
    );

    // 🔹 normalize ONCE
    normalize_matrix(
        utility_matrix,
        normalized_matrix,
        No_of_users,
        NO_OF_MOVIES
    );

    printf("[INFO] Data loaded and normalized\n");

    // 🔹 timing
    clock_t start_time = clock();

    for (int r = 0; r < repeats; r++) {
        for (int uid = start_uid; uid <= end_uid; uid++) {
            recommender_compute(uid,
                                utility_matrix,
                                normalized_matrix,
                                movienames,
                                moviegenres,
                                No_of_users);
        }
    }

    clock_t end_time = clock();

    double elapsed =
        (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nTotal execution time: %.4f seconds\n", elapsed);

    // 🔹 cleanup
    free(utility_matrix);
    free(normalized_matrix);
    free(movienames);
    free(moviegenres);

    return 0;
}