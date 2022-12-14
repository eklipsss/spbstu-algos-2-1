#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#pragma warning(disable : 4996)

int NumberCounter(FILE* file) {
	int count = 0;
	while (!feof(file))
	{
		if (fgetc(file) == ' ') count++;
	}
	count++;
	fseek(file, 0, SEEK_SET);
	return count;
}

Node* CreateNode(int data) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) {
		printf("memory allocation error");
		exit(1);
	}
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return(node);
}

int TreeHeight(Node* node) {
	int left, right;
	if (node != NULL) {
		left = (node->left != NULL) ? TreeHeight(node->left) : 0;
		right = (node->right != NULL) ? TreeHeight(node->right) : 0;
		return ((left > right) ? left : right) + 1;
	}
	return 0;
}

void ReadArrayFromFile(FILE* file, int* Array, int N) {
	for (int i = 0; i < N; i++) {
		int a;
		if (!fscanf(file, "%d ", &a)) exit(1);
		Array[i] = a;
	}
}

Node* ReadTreeFromArray(int* Array, int* i, int n) {
	int node, node_left, node_right;
	node = Array[*i];
	node_left = Array[*i + 1];
	node_right = Array[*i + 2];
	Node* tree = CreateNode(node);
	if ((*i + 3) <= n) {
		if (node_left != 0) {
			*i = *i + 3;
			tree->left = ReadTreeFromArray(Array, i, n);
		}
		if (node_right != 0) {
			*i = *i + 3;
			tree->right = ReadTreeFromArray(Array, i, n);
		}
	}
	return tree;
}

int** CreateAuxMartix(int m, int n) {
	int** Matrix = (int**)malloc(n * sizeof(int*));
	if (Matrix == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	for (int i = 0; i < n; i++) {
		Matrix[i] = (int*)malloc(m * sizeof(int));
		if (Matrix[i] == NULL) {
			printf("memory allocation error\n");
			exit(1);
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			Matrix[i][j] = -1;
		}
	}
	return Matrix;
}

void AuxMartixWithHeightDiff(Node* node, int y, int* x, int** Matrix) {
	if (node != NULL) {
		AuxMartixWithHeightDiff(node->left, y + 2, x, Matrix);
		Matrix[y][*x] = node->data;
		Matrix[y + 1][*x] = abs(TreeHeight(node->left) - TreeHeight(node->right));
		(*x)++;
		AuxMartixWithHeightDiff(node->right, y + 2, x, Matrix);
	}
}

void PrintTreeWithHeightDiff(int** Matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (i % 2 != 0) {
				if (Matrix[i][j] == -1) printf("   ");
				else printf("[%i]", Matrix[i][j]);
			}
			else {
				if (Matrix[i][j] == -1) printf("   ");
				else printf(" %i ", Matrix[i][j]);
			}
		}
		printf("\n");
	}
}

void BuildTreeWithHeightDiff(char const* filename) {
	FILE* file;
	file = fopen(filename, "r");
	int n = NumberCounter(file);
	int i = 0;
	int* Array = (int*)malloc(n * sizeof(int));
	Node* root = (Node*)malloc(sizeof(Node));
	ReadArrayFromFile(file, Array, n);
	root = ReadTreeFromArray(Array, &i, n);
	int m = TreeHeight(root)*2;
	int** AuxMatrix = CreateAuxMartix(n, m);
	i = 0;
	AuxMartixWithHeightDiff(root, 0, &i, AuxMatrix);
	PrintTreeWithHeightDiff(AuxMatrix, m, n);
	fclose(file);
}

