#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#include "header.h"

int NumberOfColumns(FILE* file) {
	char x = 0;
	int count = 0;
	while (!feof(file)) {
		x = fgetc(file);
		if (x == '\n') {
			fseek(file, 0, SEEK_SET);
			return count;
		}
		if (x == '1' || x == '0') count++;
	}
}

int NumberOfLines(FILE* file) {
	char y = 0;
	int count = 0;
	while (!feof(file)) {
		y = fgetc(file);
		if (y == '\n') count++;
	}
	fseek(file, 0, SEEK_SET);
	return count;
}

int CellControl(int x, int y, int** steps, int** lab, int n, int m) {
	if (x < 0 || y < 0 || x >= n || y >= m) return 0;
	if (lab[x][y] == 0 || steps[x][y] == 1) return 0;
	return 1;
}

void RecursivePass(int x, int y, int end_x, int end_y, int** steps, int length, int** lab, int* min_length) {
	if (x == (end_x - 1) && y == (end_y - 1)) {
		if (length < *min_length) *min_length = length;
		return;
	}
	steps[x][y] = 1;
	length++;

	//вниз
	if (CellControl(x + 1, y, steps, lab, end_x, end_y)) {
		RecursivePass(x + 1, y, end_x, end_y, steps, length, lab, min_length);
	}

	//направо
	if (CellControl(x, y + 1, steps, lab, end_x, end_y)) {
		RecursivePass(x, y + 1, end_x, end_y, steps, length, lab, min_length);
	}

	//вверх
	if (CellControl(x - 1, y, steps, lab, end_x, end_y)) {
		RecursivePass(x - 1, y, end_x, end_y, steps, length, lab, min_length);
	}

	//налево
	if (CellControl(x, y - 1, steps, lab, end_x, end_y)) {
		RecursivePass(x, y - 1, end_x, end_y, steps, length, lab, min_length);
	}

	steps[x][y] = 0;
	length--;
}

int** CreateMatrix(int n, int m) {
	int** matrix = (int**)malloc(n * sizeof(int*));
	if (matrix == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	for (int i = 0; i < n; i++) {
		matrix[i] = (int*)malloc(m * sizeof(int));
		if (matrix[i] == NULL) {
			printf("memory allocation error\n");
			exit(1);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

void PrintMatrix(int** matrix, int n, int m) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf("%i ", matrix[i][j]);
		}
		printf("\n");
	}
}

int** CreateMatrixFromFile(FILE* file, int n, int m) {
	char node = 0;
	int** matrix = CreateMatrix(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			//char node = 0;
			if (!fscanf(file, "%c ", &node)) exit(1);
			if (node == '1') matrix[i][j] = 1;
		}
	}
	return matrix;
}

void MazePassage(char const* filename) {
	FILE* file;

	if ((file = fopen(filename, "r")) == NULL) {
		printf("cannot open file\n");
		exit(1);
	}

	int m = NumberOfColumns(file);
	int n = NumberOfLines(file);
	//printf("%i %i ", n, m);
	int** matrix = CreateMatrixFromFile(file, n, m);
	PrintMatrix(matrix, n, m);

	int** visited = CreateMatrix(n, m);

	int start_x = 0;
	int start_y = 0;

	printf("\nPut starting cell\n");
	printf("start_line: ");
	if (!scanf("%i", &start_x)) exit(1);
	printf("start_column: ");
	if (!scanf("%i", &start_y)) exit(1);
	
	int length = 0;
	int min_length = n * m;

	RecursivePass(start_x, start_y, n, m, visited, length, matrix, &min_length);
	printf("\nMin length: %i\n", min_length);

}
