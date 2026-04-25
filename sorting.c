#include <stdlib.h>

// pair structure (movie + rating)
typedef struct
{
	int movie;
	double rating;
} MoviePair;

// comparator (descending order)
int compare_pairs(const void *a, const void *b)
{
	double diff = ((MoviePair *)b)->rating - ((MoviePair *)a)->rating;

	if (diff > 0)
		return 1;
	if (diff < 0)
		return -1;
	return 0;
}

void sort(int *recommended_movies,
		  double *predicted_ratings,
		  int no_of_recommended_movies)
{
	if (no_of_recommended_movies <= 1)
		return;

	// create temporary array of pairs
	MoviePair *pairs = malloc(sizeof(MoviePair) * no_of_recommended_movies);

	// pack data
	for (int i = 0; i < no_of_recommended_movies; i++)
	{
		pairs[i].movie = recommended_movies[i];
		pairs[i].rating = predicted_ratings[i];
	}

	// sort using qsort
	qsort(pairs, no_of_recommended_movies,
		  sizeof(MoviePair), compare_pairs);

	// unpack back into original arrays
	for (int i = 0; i < no_of_recommended_movies; i++)
	{
		recommended_movies[i] = pairs[i].movie;
		predicted_ratings[i] = pairs[i].rating;
	}

	free(pairs);
}