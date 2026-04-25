//This file normalizes all matrices
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

double calc_average(const double *utility_matrix, int No_of_movies) { //inputs: utility matrix and user id
	double sum = 0.0;
	size_t count = 0;
	for(size_t i = 0; i < (size_t)No_of_movies; i++) {
		if(utility_matrix[i] != 0) {
			sum += utility_matrix[i];
			count++;
		}
	}
	if(count == 0) return 0.0; // Avoid division by zero
	return sum / count;
}

void normalize_matrix(const double *utility_matrix, double *normalized_matrix, int No_of_users, int No_of_movies) { //inputs: utility matrix and new matrix to save normalized ratings
	for(size_t i = 0; i < (size_t)No_of_users; i++) {
		double average = calc_average(&utility_matrix[i * No_of_movies], No_of_movies);
		for(size_t j = 0; j < (size_t)No_of_movies; j++) {
			size_t idx = i * No_of_movies + j;
			if(utility_matrix[idx] == 0) {
				normalized_matrix[idx] = 0;
			} else {
				normalized_matrix[idx] = utility_matrix[idx] - average;
			}
		}
	}
}

void normalize(const double *user, double *normalizeduser, int No_of_movies) { //inputs: rating vector of new user and new vector to save normalized ratings
	double sum = 0.0;
	size_t count = 0;
	for(size_t i = 0; i < (size_t)No_of_movies; i++) {
		if(user[i] != 0) {
		sum += user[i];
		count++;
		}
	}
	double average = (count == 0) ? 0.0 : (sum / count);
	for(size_t i = 0; i < (size_t)No_of_movies; i++) {
		if(user[i] == 0) {
		normalizeduser[i] = 0; //rating becomes zero if not rated
		} else {
		normalizeduser[i] = user[i] - average; //subtract average rating from current rating
		}
	}
}
