#include <stdlib.h>
#include <stdio.h>
struct node{
	int value;
	struct node *next;
};

void printLL(struct node*head);
struct node*bubblesort(struct node*startPtr);

int main(){
	struct node *head, *temp, *temp2;
	int num;
	temp = NULL;
	temp2 = NULL;
	head = NULL;
	printf("Enter as many integers as you would like, press ctrl-d when done.\n");
	while(1)
	{
		if(scanf("%d\n", &num) == EOF)
		{
			break;
		}
		temp = malloc(sizeof(struct node));
		if(temp == NULL)
                {
                        printf("Error in malloc\n");
                        exit(1);
                }
		temp->value = num;
		temp->next = NULL;
		if(head == NULL)
		{
			head = malloc(sizeof(struct node));
			if(head == NULL)
			{
				printf("Error in malloc\n");
				exit(1);
			}
			head = temp;
		}
		else
		{
			temp2 = malloc(sizeof(struct node));
			if(temp2 == NULL)
	                {
        	                printf("Error in malloc\n");
                	        exit(1);
               		}
			temp2 = head;
			while(temp2->next != NULL)
			{
				temp2 = temp2->next;
			}
			temp2->next = temp;
		}
	}
	printf("The Linked List Bubble Sorted is:\n");
	printLL(bubblesort(head));
}

void printLL(struct node*head)
{
	while(head != NULL)
	{
		printf("%d\n", head->value);
		head = head->next;
	}
}

struct node*bubblesort(struct node* startPtr)
{
	int sort = 1;
	struct node *previous, *current, *next;
	while(sort == 1)
	{
		sort = 0;
		previous = NULL;
		current = startPtr;
		next = startPtr->next;

		while(next != NULL)
		{
			if(current->value > next->value)
			{
				sort = 1;
				if(previous == NULL)
				{
					struct node *tmp;
					tmp = next->next;
					next->next = current;
					startPtr = next;
					current->next = tmp;
					previous = next;
					break;
				}
				else
				{
					struct node *tmp2;
					tmp2 = next->next;
					previous->next = next;
					next->next = current;
					current->next = tmp2;
					break;
				}
			}
			else
			{
				previous = current;
				current = next;
				next = next->next;
			}
		}
	}
	return startPtr;
}
