CC = /opt/homebrew/opt/llvm/bin/clang
CFLAGS = -O3 -arch arm64 -Wall -pedantic -std=c17 -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp -lm
HEADERS = ppm.h scene.h
TARGET = raytracer
SRC = main.c scene.c ppm.c
OBJ = $(SRC:.c=.o)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

clean:
	rm -f *.o
	rm -f main
