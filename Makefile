bench: bench.c recommender.o
	gcc -O2 bench.c recommender.o -o bench -lm

prof:
	gcc -pg -O2 bench.c recommender.c -o bench -lm