#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"jval.h"

// typedef struct SV{
// 	char name[50];
// 	double grade;
// };

JRB classKeyName;
JRB classKeyGrade;
JRB ptr,p;

int main() {
	classKeyName = make_jrb();
	classKeyGrade = make_jrb();
	int choice;
	char name[50];
	double grade;
	char dem[10];
	do {
		printf("------------------------------\n");
		printf("1.Add SV\n");
		printf("2.Search by name\n");
		printf("3.Search by name and delete\n");
		printf("4.Search by grade\n");
		printf("5.Sorting\n");
		printf("6.Print list\n");
		printf("7.Exit\n");
		printf("Your choice: ");
		scanf("%d",&choice);
		gets(dem);
		switch(choice) {
			case 1:
				printf("Name: ");
				gets(name);
				printf("Grade: ");
				scanf("%lf",&grade);
				jrb_insert_str(classKeyName,strdup(name),new_jval_d(grade));
				jrb_insert_dbl(classKeyGrade,grade,new_jval_s(strdup(name)));
				break;
			case 2:
				printf("Name: ");
				gets(name);
				ptr = jrb_find_str(classKeyName,name);
				if(ptr == NULL)
					printf("The name you've inputed is invalid.\n");
				else {
					printf("The name you've inputed is valid.\n");
					printf("%s\t%.1f\n", jval_s(ptr->key),jval_d(ptr->val));
				}
				break;
			case 3: 
				printf("Name: \n");
				gets(name);
				ptr = jrb_find_str(classKeyName,name);
				if(ptr == NULL)
					printf("The name you've inputed is invalid.\n");
				else {
					p = jrb_find_dbl(classKeyGrade,jval_d(ptr->val));
					printf("The name you've inputed is valid.\n");
					jrb_delete_node(ptr);
					jrb_delete_node(p);
					printf("Deleted successfully!\n");
				}
				break;
			case 4:
				printf("Grade: ");
				scanf("%lf",&grade);
				ptr = jrb_find_dbl(classKeyGrade,grade);	
				if(ptr == NULL) {
					printf("The grade you've inputed is invalid.\n");
				}
				else {
					printf("The grade you've inputed is valid.\n");
					printf("%s\t%.1f\n", jval_s(ptr->val),jval_d(ptr->key));
				}
				break;
			case 5:
				// Cay JRB da sap xep san theo mac dinh, ket qua se duoc the hien o case 6
				break;
			case 6:
				printf("**************************\n");
				printf("\nThe class sorted by name: \n");
				jrb_traverse(ptr,classKeyName){
					printf("%s\t%.1f\n", jval_s(ptr->key),jval_d(ptr->val));
				}
				printf("**************************\n");
				printf("\nThe class sorted by grade: \n");
				jrb_traverse(ptr,classKeyGrade) {
					printf("%.1f\t%s\n", jval_d(ptr->key),jval_s(ptr->val));
				}
				break;
			case 7:
				jrb_free_tree(classKeyName);
				jrb_free_tree(classKeyGrade);
				printf("Good bye.\n");
		}
	} while(choice != 7);
	return 0;
}