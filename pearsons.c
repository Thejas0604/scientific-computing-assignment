#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Slightly optimized cosine/pearson-style similarity
double pearson_correlation(double *A, double *B, unsigned int size)
{
    double dot_p = 0.0;
    double mag_a = 0.0;
    double mag_b = 0.0;

    // pointer-based traversal (faster than indexing)
    double *pa = A;
    double *pb = B;

    for (unsigned int i = 0; i < size; i++, pa++, pb++)
    {
        double a = *pa;
        double b = *pb;

        // skip useless operations for sparse data
        if (a == 0.0 && b == 0.0)
            continue;

        dot_p += a * b;
        mag_a += a * a;
        mag_b += b * b;
    }

    if (mag_a == 0.0 || mag_b == 0.0)
        return 0.0;

    // compute sqrt once each
    double denom = sqrt(mag_a * mag_b);

    if (denom == 0.0 || isnan(denom))
        return 0.0;

    return dot_p / denom;
}


// similarity for all users
void calc_similarity(double *normalizeduser,
                     double *normalized_matrix,
                     double *similarity,
                     int No_of_users,
                     int No_of_movies)
{
    for (int i = 0; i < No_of_users; i++)
    {
        double *other_user = &normalized_matrix[i * No_of_movies];

        similarity[i] = pearson_correlation(
            normalizeduser,
            other_user,
            No_of_movies
        );
    }
}