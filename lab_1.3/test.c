#include "header.h"

int main(void) {
	char* f = "input.txt";
	int k = StringCounter(f);
	List* list = CreateListFromFile(f, k);
	PrintWholeList(list);
	printf("\nDate coincidence check: \n");
	Date* input_date = InputDate();
	CheckDate(list, input_date);
	return 0;
}