#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <math.h>
#pragma warning(disable:4996)

#include "header.h"

int RoundDown(double r) { 
	int R = 0;
	if (r != 0) {
		R = (fabs(r) / r) * floor(fabs(r));
	}
	return R;
}

void InsertToList(Point* point, List** list) { 
	List* new, * cur;
	new = (List*)malloc(sizeof(List));
	if (new == NULL) {
		printf("No memory avaible.\n");
		exit(1);
	}
	else {
		new->point = point;
		new->next = NULL;
		cur = *list;
		if (*list == NULL) {
			*list = new;
		}
		else {
			while (cur->next != NULL) {
				cur = cur->next;
			}
			cur->next = new;
		}
	}
}

List* CreateListFromFile(char const* filename) { 
	FILE* file;
	if ((file = fopen(filename, "r")) == NULL) {
		printf("cannot open file\n");
		exit(1);
	}
	List* list = NULL;
	int x, y;
	while ((fscanf(file, "%i %i", &x, &y)) != EOF){
		Point* point_t = (Point*)malloc(sizeof(Point));
		if (point_t == NULL) {
			printf("No memory avaible.\n");
			exit(1);
		}
		point_t->x = x;
		point_t->y = y;
		InsertToList(point_t, &list);
	}
	return list;
}

List* CreateListField(int x, int y) { //работает верно
	Point* point = (Point*)malloc(sizeof(Point));
	point->x = x;
	point->y = y;
	List* list = (List*)malloc(sizeof(List));
	if (list == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	list->point = point;
	list->next = NULL;
	return list;
}

int Min(List* dots, int flag_x, int flag_y) {
	int minimum = 0;
	if (flag_x == 1) {
		minimum = dots->point->x;
		dots = dots->next;
		while (dots != NULL) {
			if (dots->point->x < minimum) minimum = dots->point->x;
			dots = dots->next;
		}
	}
	if (flag_y == 1) {
		minimum = dots->point->y;
		dots = dots->next;
		while (dots != NULL) {
			if (dots->point->y < minimum) minimum = dots->point->y;
			dots = dots->next;
		}
	}
	return minimum;
}

int Max(List* dots, int flag_x, int flag_y) {
	int maximum = 0;
	if (flag_x == 1) {
		maximum = dots->point->x;
		dots = dots->next;
		while (dots != NULL) {
			if (dots->point->x > maximum) maximum = dots->point->x;
			dots = dots->next;
		}
	}
	if (flag_y == 1) {
		maximum = dots->point->y;
		dots = dots->next;
		while (dots != NULL) {
			if (dots->point->y > maximum) maximum = dots->point->y;
			dots = dots->next;
		}
	}
	return maximum;
}

List** FormArray(List* dots) {
	List** Array = (List**)malloc(sizeof(List*)*(Max(dots,0,1)-Min(dots,0,1)+1));
	for (int i = 0; i <= (Max(dots, 0, 1) - Min(dots, 0, 1)); i++) {
		Array[i]=NULL;
	}
	return Array;
}

void PrintArray(List** Array, int n, int m) {
	for (int i = 0; i < m-n; i++) {
		List* buf = (List*)malloc(sizeof(List));
		buf = Array[i];
		printf("[ %i ] ", i);
		while (buf != NULL) {
			printf(" %i ", buf->point->x);
			buf = buf->next;
		}
		printf("\n");
	}
}

void InsertPointToArray(List** Array, int x, int y, List* dots) {
	List* list = CreateListField(x, y);
	int k = y - Min(dots,0,1);

	List* new, * cur;
	new = (List*)malloc(sizeof(List));
	if (new == NULL) {
		printf("No memory avaible.\n");
		exit(1);
	}
	else {
		new = list;
		cur = Array[k];
		if (Array[k] == NULL) {
			Array[k] = new;
		}
		else {
			while (cur->next != NULL) {
				cur = cur->next;
			}
			cur->next = new;
		}
	}
}

void InsertLineToArray(Point* point1, Point* point2, List** Array, List* dots) { 
	double x;
	double y;

	if (point2->y == point1->y) {
		for (x = min(point1->x, point2->x); x <= max(point1->x, point2->x); x++) {
			InsertPointToArray(Array, (int)x, point1->y, dots);
		}
		exit(0);
	}
	if (point2->x == point1->x) {
		for (y = min(point1->y, point2->y); y <= max(point1->y, point2->y); y++) {
			InsertPointToArray(Array, point1->x, (int)y, dots);
		}
		exit(0);
	}

	double koeff = ((double)(point2->y - point1->y) / (double)(point2->x - point1->x));

	if ((abs(koeff) >= 1)) {
		if (koeff < 0) { // line 3
			for (y = min(point1->y, point2->y); y < max(point1->y, point2->y); y++) {
				x = (((double)(point2->x - point1->x) * (y - point1->y) / (double)(point2->y - point1->y)) + point1->x);
				if (x == RoundDown(x)) x--;
				else x = RoundDown(x);
				InsertPointToArray(Array, (int)x, (int)y, dots);
			}
		}
		if (koeff > 0) { // line 1
			for (y = min(point1->y, point2->y); y < max(point1->y, point2->y); y++) {
				x = RoundDown((((double)(point2->x - point1->x) * (y - point1->y) / (double)(point2->y - point1->y)) + point1->x));
				InsertPointToArray(Array, (int)x, (int)y, dots);
			}
			
		}
	}
	else {
		if (koeff < 0) { // line 6
			for (x = min(point1->x, point2->x); x < max(point1->x, point2->x); x++) {
				y = (((double)(point2->y - point1->y) * (x - point1->x) / (double)(point2->x - point1->x)) + point1->y);
				if (y == RoundDown(y)) y--;
				else y = RoundDown(y);
				InsertPointToArray(Array, (int)x, (int)y, dots);
			}
		}
		if (koeff >= 0) { // line 2,4
			for (x = min(point1->x, point2->x); x < max(point1->x, point2->x); x++) {
				y = RoundDown(((double)(point2->y - point1->y) * (x - point1->x) / (double)(point2->x - point1->x)) + point1->y);
				InsertPointToArray(Array, (int)x, (int)y, dots);
			}
		}
	}
	//PrintArray(Array, Min(dots, 0, 1), Max(dots, 0, 1));
}

int** DynamicArray(int n, int m) {
	int** A = (int**)malloc(n * sizeof(int*));
	if (A == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	for (int i = 0; i < n; i++) {
		A[i] = (int*)malloc(m * sizeof(int));
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			A[i][j] = 0;
		}
	}
	return A;
}

void DynamicArrayFree(int** A, int n) {
	for (int i = 0; i < n; i++) free(A[i]);
	free(A);
}

void FillingMatrix(int** A, int n, int m) {
	for (int j = m - 1; j > 0; j--) {
		for (int i = 0; i < n; i++) {
			if (A[i][j] == 0) {
				for (int j1 = j; j1 >= 0; j1--) {
					if (A[i][j1] == 1){
						for (int j2 = j; j2 < m; j2++) {
							if (A[i][j2] == 1) {
								for (int i1 = i; i1 >= 0;i1--) {
									if (A[i1][j] == 1) {
										for (int i2 = i; i2 < n; i2++) {
											if (A[i2][j] == 1) {
												A[i][j] = 1;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void PrintMatrix(int** Matrix, int n, int m) {
	for (int j = m-1; j >=0 ; j--) {
		for (int i = 0; i < n; i++) {
			printf(" %i ", Matrix[i][j]);
		}
		printf("\n");
	}
}

void CompleteMatrix(List** Array, List* dots) {
	int delta_x = Max(dots, 1, 0) - Min(dots, 1, 0);
	int delta_y = Max(dots, 0, 1) - Min(dots, 0, 1);
	int** Matrix = DynamicArray(delta_x, delta_y);
	for (int i = 0; i < delta_y; i++) {
		List* buf = (List*)malloc(sizeof(List));
		buf = Array[i];
		while (buf != NULL) {
			Matrix[buf->point->x - Min(dots, 1, 0)][buf->point->y - Min(dots, 0, 1)] = 1;
			buf = buf->next;
		}
	}
	FillingMatrix(Matrix, delta_x, delta_y);
	PrintMatrix(Matrix, delta_x, delta_y);
}


void ActionOnFile(char const* filename) {
	List* list = CreateListFromFile(filename);
	List** Array = FormArray(list);
	List* buf = list;
	while (buf->next != NULL) {
		InsertLineToArray(buf->point, buf->next->point, Array, list);
		buf = buf->next;
		//printf("\n");
	}
	InsertLineToArray(buf->point, list->point, Array, list);
	//PrintArray(Array, Min(list, 0, 1), Max(list, 0, 1));
	CompleteMatrix(Array, list);
}
