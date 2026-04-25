#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "pearsons.h"
#include "kmeans.h"
#include "predictions.h"
#include "sorting.h"

#define No_of_movies 300


void recommender_compute(int userid,
                         double *utility_matrix,
                         double *normalized_matrix,
                         char *movienames,
                         char *moviegenres,
                         int No_of_users)
{
    int i;
    clock_t t = clock();

    int k = 16;

    // ✅ Use existing row instead of reading CSV again
    double *newuser = &utility_matrix[userid * No_of_movies];
    double *normalizednewuser = &normalized_matrix[userid * No_of_movies];

    // Allocate similarity
    double *similarity = malloc(sizeof(double) * No_of_users);

    // Compute similarity
    calc_similarity(normalizednewuser, normalized_matrix,
                    similarity, No_of_users, No_of_movies);

    // Handle NaNs safely (avoid crashes in batch)
    for (i = 0; i < No_of_users; i++) {
        if (isnan(similarity[i])) {
            similarity[i] = 0.0;
        }
    }

    // KMeans clustering
    double *centroids = malloc(sizeof(double) * k);
    int *cluster_assignment = malloc(sizeof(int) * No_of_users);

    // Improved centroid initialization
    for (i = 0; i < k; i++) {
        int attempts = 0;
        while (attempts < 5) {
            int idx = rand() % No_of_users;
            int duplicate = 0;

            for (int m = 0; m < i; m++) {
                if (fabs(centroids[m] - similarity[idx]) < 1e-6) {
                    duplicate = 1;
                    break;
                }
            }

            if (!duplicate) {
                centroids[i] = similarity[idx];
                break;
            }
            attempts++;
        }
    }

    kmeans(1, similarity, No_of_users, k,
           centroids, cluster_assignment);

    // Extract most similar cluster
    int *similar_users = malloc(sizeof(int) * No_of_users);
    int no_of_susers = 0;

    double max = -1.0;
    int maxid = 0;

    for (i = 0; i < k; i++) {
        if (centroids[i] > max) {
            max = centroids[i];
            maxid = i;
        }
    }

    for (i = 0; i < No_of_users; i++) {
        if (cluster_assignment[i] == maxid) {
            similar_users[no_of_susers++] = i;
        }
    }

    if (no_of_susers == 0) {
        // fallback: use all users if clustering fails
        for (i = 0; i < No_of_users; i++) {
            similar_users[i] = i;
        }
        no_of_susers = No_of_users;
    }

    // Prediction
    int *recommended_movies = malloc(sizeof(int) * No_of_movies);
    double *predicted_ratings = malloc(sizeof(double) * No_of_movies);

    int no_of_recommended_movies =
        make_prediction(newuser, similar_users, no_of_susers,
                        similarity, utility_matrix,
                        recommended_movies, predicted_ratings,
                        No_of_movies);

    // Sort recommendations (assumes you replaced with qsort inside)
    sort(recommended_movies, predicted_ratings,
         no_of_recommended_movies);

    // Print top 10 only (non-interactive)
    int limit = (no_of_recommended_movies < 10)
                    ? no_of_recommended_movies
                    : 10;

    printf("\nUser %d Top Recommendations:\n", userid);

    for (i = 0; i < limit; i++) {
        printf("%d. %s %s",
               i + 1,
               &movienames[recommended_movies[i] * 1024],
               &moviegenres[recommended_movies[i] * 1024]);
    }

    // Timing
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("\nTime taken: %.4f sec\n", time_taken);

    // Free only locally allocated memory
    free(similarity);
    free(centroids);
    free(cluster_assignment);
    free(similar_users);
    free(recommended_movies);
    free(predicted_ratings);
}