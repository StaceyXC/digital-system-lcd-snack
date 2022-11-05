#ifndef _SNACK_H_
#define _SNACK_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct Node //蛇的节点
{
    int xh;
	int yh;
    struct Node *next;
} Node;

Node* CreateEmptyList(int x, int y , int len);
void DestroyList(Node *head);
void ListInsert(Node *head, int x, int y);
void DeleteNode(Node* head);