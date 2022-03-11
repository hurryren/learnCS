#include <stdio.h>
#include <stdlib.h>

struct list_test {
	int data;
	struct list_test *next;
};


void print_list(struct list_test *head)
{
	if (head == NULL)
		return;
	struct list_test * temp;
	
	int i=0;
	for(temp = head; temp != NULL; temp = temp->next,i++) {
		printf("list[%d] = [%d]\n",i, temp->data);
	}

}

void reverse(struct list_test **head){
	struct list_test *cur, *next, *prev;

	printf("head addr=[%p]\n",*head);
	cur = *head;
	next = cur->next;
	cur->next = NULL;
	while(next != NULL) {
		prev = cur;
		cur = next;
		next = cur->next;
		
		cur->next = prev;
		if (next == NULL)
			break;
		printf("prev=[%d]; cur=[%d]; next=[%d]\n",prev->data, cur->data, next->data);

	}
	
	*head = cur;
	print_list(*head);	
	printf("head addr=[%p]\n",*head);
}

void list_init(struct list_test *entry)
{
	struct list_test *temp, *cur;
	cur = entry;
	int i=0;


	for(i = 1; i < 5; i++) {
		temp = (struct list_test *)malloc(sizeof(struct list_test));
		temp->data = i;
		temp->next = NULL;
		cur->next = temp;
		cur = temp;
	}
}

void list_free(struct list_test *head)
{
	struct list_test *cur, *next;
	
	cur = head->next;

	while(cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

int main(){
	struct list_test **head2;
	struct list_test *head1;
	struct list_test head;
	head.data = 0;
	head.next = NULL;

	head1=&head;
	head2=&head1;
	
	list_init(head1);
	print_list(head1);
	
	
	printf("main addr=[%p]\n",*head2);
	reverse(head2);
	printf("main addr=[%p]\n",*head2);
	print_list(*head2);




	return 0;
}


		

	
	
