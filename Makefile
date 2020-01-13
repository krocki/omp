all: dot

dot: dot.c
	gcc-9 dot.c -O2 -fopenmp -o dot
