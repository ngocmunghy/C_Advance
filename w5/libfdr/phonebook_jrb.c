#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jval.h"
#include "jrb.h"


int main() {
	JRB book = make_jrb();
	JRB ptr,ptr1;
	long sdt;
	char name[50];
	char dem[10];
	jrb_insert_str(book,"Huy",new_jval_l(9812587));
	jrb_insert_str(book,"Mai",new_jval_l(7829120));
	jrb_insert_str(book,"An",new_jval_l(8725871));
	int i,n;
	do {
		printf("-------------------\n");
		printf("1.Add phone\n");
		printf("2.Seach phone\n");
		printf("3.Print list\n");
		printf("4.Exit\n");
		printf("Your choice: ");
		scanf("%d",&n);
		gets(dem);
		switch(n) {
			case 1:
				printf("Name: ");
				gets(name);
				printf("Number: ");
				scanf("%ld",&sdt);
				jrb_insert_str(book,strdup(name),new_jval_l(sdt));
				break;
			case 2:
				printf("Name: ");
				gets(name);
				ptr = jrb_find_str(book,name);
				if (ptr == NULL) {
					printf("Not found %s in the phonebook.\n", name);
				}
				else {
					printf("The name \"%s\" is valid.\n",name);
					printf("Name: %-10s Phone: %ld\n",jval_s(ptr->key),jval_l(ptr->val));
					// jrb_delete_node(ptr);
				}
				break;
			case 3:
				jrb_traverse(ptr1, book) {
					printf("Name: %-10s Phone number: %ld\n",jval_s(ptr1->key), jval_l(ptr1->val));
				}
				break;
			case 4:
				jrb_free_tree(book);
				break;
		}
	} while(n != 4);
	return 0;
}