#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Optimized collaborative filtering prediction
int make_prediction(double *user,
					int *similar_users,
					int no_of_susers,
					double *similarity,
					double *utility_matrix,
					int *recommended_movies,
					double *predicted_ratings,
					int No_of_movies)
{
	int no_of_recommended_movies = 0;

	for (int i = 0; i < No_of_movies; i++)
	{
		// skip already rated movies early
		if (user[i] != 0.0)
			continue;

		double sum1 = 0.0;
		double sum2 = 0.0;
		int count = 0;

		for (int k = 0; k < no_of_susers; k++)
		{
			int uid = similar_users[k];

			// pointer to this user's row (avoid repeated multiplication)
			double *row = &utility_matrix[uid * No_of_movies];

			double rating = row[i];
			if (rating == 0.0)
				continue;

			double sim = similarity[uid];

			sum1 += sim * rating;
			sum2 += sim;
			count++;
		}

		// stricter condition + safe division
		if (count > 0 && sum2 != 0.0 && !isnan(sum2))
		{
			recommended_movies[no_of_recommended_movies] = i;
			predicted_ratings[no_of_recommended_movies] = sum1 / sum2;
			no_of_recommended_movies++;
		}
	}

	return no_of_recommended_movies;
}

// testing version (same improvements applied)
void test_predictions(double *user,
					  int *similar_users,
					  int no_of_susers,
					  double *similarity,
					  double *utility_matrix,
					  double *predicted_ratings,
					  int No_of_movies)
{
	for (int i = 0; i < No_of_movies; i++)
	{
		if (user[i] == 0.0)
		{
			predicted_ratings[i] = 0.0;
			continue;
		}

		double sum1 = 0.0;
		double sum2 = 0.0;
		int count = 0;

		for (int k = 0; k < no_of_susers; k++)
		{
			int uid = similar_users[k];
			double *row = &utility_matrix[uid * No_of_movies];

			double rating = row[i];
			if (rating == 0.0)
				continue;

			double sim = similarity[uid];

			sum1 += sim * rating;
			sum2 += sim;
			count++;
		}

		if (count > 1 && sum2 != 0.0 && !isnan(sum2))
		{
			predicted_ratings[i] = sum1 / sum2;
		}
		else
		{
			predicted_ratings[i] = 0.0;
		}
	}
}