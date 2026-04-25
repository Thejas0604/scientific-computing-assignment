CC = gcc
CFLAGS = -O2 -Wall -lm

SRC = bench.c recommender.c pearson.c kmeans.c predictions.c sorting.c utility_matrix.c matrix_normalization.c
OBJ = $(SRC:.c=.o)

TARGET = bench

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -O2 -o $(TARGET) $(OBJ) -lm

%.o: %.c
	$(CC) -O2 -c $<

# profiling build (optional)
prof:
	$(CC) -pg -O2 $(SRC) -o bench -lm

clean:
	rm -f *.o $(TARGET)