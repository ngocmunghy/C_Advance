#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jval.h"
#include "jrb.h"

int main() {
	printf("Hello\n");
	JRB book = make_jrb();
	JRB ptr;
	jrb_insert_str(book,strdup("Hung"),new_jval_l(1111111));
	jrb_insert_str(book,strdup("Mai"),new_jval_l(2222222));
	jrb_insert_str(book, strdup("An"), new_jval_l(1234567));

	jrb_traverse(ptr, book)
		printf("Name: %-10s Phone number: %ld\n",jval_s(ptr->key), jval_l(ptr->val));

	
	return 0;
}