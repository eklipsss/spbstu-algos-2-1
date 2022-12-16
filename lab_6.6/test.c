#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

int main() {
	Tree* tree = NULL;
	printf("The tree is empty\n\nEnter the corresponding number:\nAdd Leaf to Tree - 1\nDelete Leaf from Tree - 2\nPrint the Tree - 3\nExit - any other number\n\n");
	int input = 1;
	while (input == 1 || input == 2 || input == 3) {
		if (!scanf("%d", &input)) exit(1);
		if (input == 1) { 
			printf(" - add leaf to tree\n");
			AddLeafToTree(&tree); 
		}
		if (input == 2) {
			printf(" - delete leaf from tree\n");
			DeleteLeafFromTree(&tree);
		}
		if (input == 3) {
			printf("  Tree is: ");
			if (tree == NULL) printf("*empty*");
			else PrintTree_CenteredBypass(tree);
			printf("\n");
		}
	}
	return 0;
}