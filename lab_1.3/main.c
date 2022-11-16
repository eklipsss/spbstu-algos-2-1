#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#pragma warning(disable:4996)

int StringCounter(char const* f) {
	FILE* file;
	fopen_s(&file, f, "r");
	if (file == NULL) return NULL;
	int counter = 0;
	char sym;
	while (!feof(file)) {
		sym = fgetc(file);
		if (sym == '\n') counter++;
	}
	return counter;
}

int Lettercheck(char c) {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return 1;
	return 0;
}

int CompareStructures(Date* date_current, Date* date_next) {
	int pointer = 0;
	if (date_next->year > date_current->year) pointer = 1; // next > current
	if (date_next->year < date_current->year) pointer = -1; // next < current
	if (date_next->year == date_current->year) {
		if (date_next->month > date_current->month) pointer = 1; // next > current
		if (date_next->month < date_current->month) pointer = -1; // next < current
		if (date_next->month == date_current->month) {
			if (date_next->day > date_current->day) pointer = 1; // next > current
			if (date_next->day < date_current->day) pointer = -1; // next < current
			if (date_next->day == date_current->day) pointer = 0; // next = current
		}
	}
	return pointer;
}

void Insert(Profile* profile, Date* date, List** list) {
	List* new, * prev, * cur;
	new = (List*)malloc(sizeof(List));
	if (new == NULL) printf("No memory avaible.\n");
	else {
		new->profile = profile;
		new->date = date;
		new->next = NULL;
		prev = NULL;
		cur = *list;
		while (cur != NULL && (CompareStructures(date, cur->date) == -1)){ 
			//пока дата элемента для вставки date больше, чем дата текущего элемента списка, или пока не закончится список
			prev = cur;
			cur = cur->next;
		}
		if (prev == NULL) {
			new->next = *list;
			*list = new;
		}
		else {
			prev->next = new;
			new->next = cur;
		}
	}
}

List* CreateListFromFile(char const* filename, int k) {
	List* start = NULL; 
	int len = 0;
	char* a = NULL; 
	FILE* f;
	char t;
	fopen_s(&f, filename, "r");
	if (f == NULL) return NULL;
	a = (char*)malloc(sizeof(char));
	if (a == NULL) return NULL;
	for (int j=1; j<=k; j++) {
		Profile* profile = (Profile*)malloc(sizeof(Profile));
		if (profile == NULL) {
			printf("memory allocation error\n");
			exit(1);
		}
		Date* date = (Date*)malloc(sizeof(Date));
		if (date == NULL) {
			printf("memory allocation error\n");
			exit(1);
		}
		int i = 0;
		while ((i <= 2) && (fscanf(f, "%c", &t) == 1)) { // пока удалось считать и не заполены поля ФИО
				if (Lettercheck(t) == 1) { // проверка символа (доходим до знака пробела и переходим к записи в соответствующее поле)
					len++;
					char* p = (char*)realloc(a, len * sizeof(char));
					if (p == NULL) {
						printf("memory allocation error\n");
						exit(1);
					}
					a = p;
					a[len - 1] = t; // добавление символа в строку
				}
				else {
					// если строка заполнена, то добавляем сиимвол, таким образом удается избежать записи побочных элементов (таких как знак переноса строки "\n") в поле 
					if (Lettercheck(a[0])) { 
						len++;
						char* p = (char*)realloc(a, len * sizeof(char));
						if (p == NULL) {
							printf("memory allocation error\n");
							exit(1);
						}
						a = p;
						a[len - 1] = '\0';
						if (i == 0) profile->surname = a;
						if (i == 1) profile->name = a;
						if (i == 2) profile->patronymic = a;
						len = 0;
						a = (char*)malloc(sizeof(char));
						if (a == NULL) {
							printf("memory allocation error\n");
							exit(1);
						}
						i++;
					}
				}
		}
		if (!fscanf(f, "%i %i %i", &date->day, &date->month, &date->year)) exit(1);
		Insert(profile, date, &start);
	}
	fclose(f);
	return start;
}

int ListIsEmpty(List* list) {
	return list == NULL;
}

void PrintStructureList(List* list) {
	char* s = list->profile->surname;
	int i = 0;
	while (Lettercheck(s[i]) == 1) {
		printf("%c", s[i]);
		i++;
	}
	printf(" ");
	s = list->profile->name;
	i = 0;
	while (Lettercheck(s[i]) == 1) {
		printf("%c", s[i]);
		i++;
	}
	printf(" ");
	s = list->profile->patronymic;
	i = 0;
	while (Lettercheck(s[i]) == 1) {
		printf("%c", s[i]);
		i++;
	}
	printf(" ");
	printf("%i %i %i", list->date->day, list->date->month, list->date->year);
	puts("");
}

void PrintWholeList(List* list) {
	if (ListIsEmpty(list)) {
		puts("List is empty.");
	}
	else {
		puts("The list is:");
		while (list != NULL) {
			PrintStructureList(list);
			list = list->next;
		}
	}
}

void CheckDate(List* list, Date* input_date) {
	int count = 0;
	while (list != NULL) {
		if ((list->date->day == input_date->day) && (list->date->month == input_date->month) && (list->date->year == input_date->year)) {
			count++;
			if (count == 1) printf("\nCoincidences: \n");
			PrintStructureList(list);
		}
		list = list->next;
	}
	printf("\nNumber of coincidences: %i", count);
}

Date* InputDate() {
	Date* input_date = (Date*)malloc(sizeof(Date));
	if (input_date == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	printf("");
	printf("Put the day: ");
	if (!scanf("%i", &input_date->day)) exit(1);
	printf("Put the month: ");
	if (!scanf("%i", &input_date->month)) exit(1);
	printf("Put the year: ");
	if (!scanf("%i", &input_date->year)) exit(1);
	return input_date;
}
