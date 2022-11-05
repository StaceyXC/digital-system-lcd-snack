#include "snack.h"

Node *CreateEmptyList(int x, int y , int len)//初始化首尾
{
    Node *head;
    head = (Node *)malloc(sizeof(Node));
    head->xh=x;
    head->yh=y;
    head->next =(Node *)malloc(sizeof(Node));
    head->next->xh=x;
    head->next->yh=y-len;
    head->next->next=NULL;

    return head;
}
void DestroyList(Node *head)
{
    Node *p;

    while (head->next != NULL)
    {
        p = head->next;
        free(head);
        head = p;
    }
    free(head);
}
void ListInsert(Node *head, int x, int y) //插在头的后一个
{
    Node *p = head;
    p = p->next;
    Node *s = (Node *)malloc(sizeof(Node));
    s->next=p->next
    p->next = s;
    s->xh = x;
    s->yh = y;
}
void DeleteNode(Node *head) //删除最后一个节点
{
    Node *p = head;
    Node *temp;

    while (p->next != NULL)
    {
        temp = p;
        p = p->next;
    }
    temp->next = NULL;
    free(p);
}
