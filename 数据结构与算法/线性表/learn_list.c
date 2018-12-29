#include<stdio.h>

typedef int ElementType;

typedef struct{
    ElementType element;
    Node * next;
    Node * prev;
}Node;

typedef struct{
    Node * head;
    Node * last;
    int len;
}List;

void headPush(Node * node,List * list)
{
    if(list.head != NULL)
    {
        
    }
    list.head = node
}

void headPop()
{

}

int list_len(List * l)
{
    return l.len;
}

int list_is_empty(List * l)
{
    return l.len == 0;
}




