#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"


void AddLeafToTree(Tree** tree) {
	if (*tree == NULL) {
		Tree* leaf = (Tree*)malloc(sizeof(Tree));
		if (leaf == NULL) {
			printf("memory allocation error\n");
			exit(1);
		}
		leaf->left = NULL;
		leaf->right = NULL;
		leaf->parent = NULL;
		leaf->node_diff = 0;
		*tree = leaf;
		return;
	}
	Tree* node = *tree;
	while (!((node)->left == NULL && (node)->right == NULL)) {
		if ((node)->left == NULL) {
			Tree* leaf = (Tree*)malloc(sizeof(Tree));
			if (leaf == NULL) {
				printf("memory allocation error\n");
				exit(1);
			}
			leaf->left = NULL;
			leaf->right = NULL;
			leaf->parent = node;
			leaf->node_diff = 0;
			node->node_diff++;
			node->left = leaf;
			return;
		}
		if (node->right == NULL) {
			Tree* leaf = (Tree*)malloc(sizeof(Tree));
			if (leaf == NULL) {
				printf("memory allocation error\n");
				exit(1);
			}
			leaf->left = NULL;
			leaf->right = NULL;
			leaf->parent = node;
			leaf->node_diff = 0;
			node->node_diff--;
			node->right = leaf;
			return;
		}
		if ((node)->node_diff <= 0)		{
			(node)->node_diff++;
			node = (node)->left;
		}
		else {
			(node)->node_diff--;
			node = (node)->right;
		}
	}
	Tree* leaf = (Tree*)malloc(sizeof(Tree));
	if (leaf == NULL) {
		printf("memory allocation error\n");
		exit(1);
	}
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->parent = node;
	leaf->node_diff = 0;
	node->node_diff++;
	node->left = leaf;
}


void DeleteLeafFromTree(Tree** tree) {
	if (*tree == NULL) {
		printf("Error: tree is empty\n");
		return;
	}
	Tree** node = tree;
	while (!((*node)->left == NULL && (*node)->right == NULL)) {
		if ((*node)->left == NULL) {
			(*node)->node_diff++;
			node = &((*node)->right);
			continue;
		}
		if ((*node)->right == NULL) {
			(*node)->node_diff--;
			node = &((*node)->left);
			continue;
		}
		if ((*node)->node_diff >= 0) {
			(*node)->node_diff--;
			node = &((*node)->left);
		}
		else {
			(*node)->node_diff++;
			node = &((*node)->right);
		}
	}
	free(*node);
	*node = NULL;
}

void PrintTree_CenteredBypass(Tree* node) {
	if (node != NULL) {
		PrintTree_CenteredBypass(node->left);
		printf("%d ", node->node_diff);
		PrintTree_CenteredBypass(node->right);
	}
}