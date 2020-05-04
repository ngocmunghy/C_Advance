#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jval.h"
#include "jrb.h"


int main() {
	JRB book = make_jrb();
	JRB ptr;
	long sdt;
	char name[50];
	char dem[10];
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
				jrb_insert_str(book,name,new_jval_l(sdt));
				break;
			case 2:
				printf("Name: ");
				gets(name);
				ptr = jrb_find_str(book,name);
				if (ptr == NULL) {
					printf("Not found %s in the phonebook.\n", name);
				}
				else {
					printf("Found it.\n");
					printf("Name: %-10s Phone: %ld\n",jval_s(ptr->key),jval_l(ptr->val));
				}
				break;
			case 3:
				jrb_rtraverse(ptr, book) {
					printf("Name: %-10s Phone number: %ld\n",jval_s(ptr->key), jval_l(ptr->val));
				}
				break;
			case 4:
				jrb_free_tree(book);
				jrb_delete_node(ptr);
				break;
		}
	} while(n != 4);
	return 0;
}