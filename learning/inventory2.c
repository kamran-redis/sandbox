#include <stdio.h>
#include <stdlib.h>
#include "readline.h"

#define NAME_LEN 25
#define MAX_PARTS 100

struct part
{
    int number;
    char name[NAME_LEN + 1];
    int on_hand;
    struct part *next;
};

struct part *inventory = NULL;

int num_parts = 0;

struct part *find_part(int number);
void insert(void);
void search(void);
void update(void);
void print(void);

int main(void)
{
    char code;
    for (;;)
    {
        printf("Enter operation code: ");
        scanf(" %c", &code);
        while (getchar() != '\n')
            ;
        switch (code)
        {
        case 'i':
            insert();
            break;
        case 's':
            search();
            break;
        case 'u':
            update();
            break;
        case 'p':
            print();
            break;
        case 'q':
            return 0;
        default:
            printf("Illegal code\n");
        }
    }
    printf("\n");
}

struct part *find_part(int number)
{
    struct part *p;

    for (p = inventory; p != NULL && number > p->number; p = p->next)
        ;

    if (p != NULL && number == p->number)
        return p;
    return NULL;
}

void insert(void)
{
    struct part *cur, *prev, *new_node;
    new_node = malloc(sizeof(struct part));
    if (new_node == NULL)
    {
        printf("Insert: Database is full \n");
        return;
    }

    printf("Part Number: ");
    scanf("%d", &new_node->number);

    for (cur = inventory, prev = NULL; cur != NULL && new_node->number > cur->number; prev = cur, cur = cur->next)
        ;
    if (cur != NULL && new_node->number == cur->number)
    {
        printf("Part already exists\n");
        free(new_node);
        return;
    }

    printf("Enter part name: ");
    read_line(new_node->name, NAME_LEN);
    printf("Quantity: ");
    scanf("%d", &new_node->on_hand);

    new_node->next = cur;
    if (prev == NULL)
    {
        inventory = new_node;
    }
    else
    {
        prev->next = new_node;
    }
}

void search(void)
{
    int number;
    struct part *p;

    printf("Enter part number: ");
    scanf("%d", &number);
    p = find_part(number);
    if (p != NULL)
    {
        printf("Part found %s %d %d\n", p->name, p->number, p->on_hand);
    }
    else
    {
        printf("Part not found \n");
    }
}

void update(void)
{
    int number, change;
    struct part *p;
    printf("Enter part number: ");
    scanf("%d", &number);
    p = find_part(number);
    if (p!=NULL)
    {
        printf("Change in quantity: ");
        scanf("%d", &change);
        p->on_hand += change;
    }
    else
    {
        printf("Part not found \n");
    }
}

void print(void)
{
    struct part *cur; 
    printf("Part Number   Part Name   Q on hand\n");
    for (cur = inventory; cur !=NULL; cur = cur->next)
    {
        printf("%7d        %-25s %11d\n", cur->number, cur->name, cur->on_hand);
    }
}
