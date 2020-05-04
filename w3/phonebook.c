#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE 10
#define INCREMENTTAL_SIZE 5

typedef struct {
	char name[30];
	long number;
} PhoneEntry;

typedef struct {
	PhoneEntry *entries;
	int total; // so luong phan tu hien co
	int size; // kich thuoc toi da
} PhoneBook;

PhoneBook createPhoneBook();
void dropPhoneBook(PhoneBook *book);
void addPhoneNumber(char *name, long number, PhoneBook *book);
PhoneEntry *getPhoneNumber(char *name, PhoneBook book);

int main() {
	int i,n,choice;
	long sdt;
	char name[30];
	char dem[30];
	PhoneEntry *entry;
	PhoneBook book;
	book = createPhoneBook();
	addPhoneNumber("Anh",83720814,&book);
	addPhoneNumber("Huong",72819203,&book);
	addPhoneNumber("Dung",82937481,&book);

	do {
		printf("\n");
		printf("1.Add phone\n");
		printf("2.Search phone\n");
		printf("3.Print\n");
		printf("4.Exit\n");
		printf("Your choice: \n");
		scanf("%d",&choice);
		switch(choice) {
			case 1:
				printf("Name: ");
				gets(name);
				gets(dem);
				printf("Phone number: ");
				scanf("%ld",&sdt);
				addPhoneNumber(name,sdt,&book);
				break;
			case 2:

				printf("Name: ");
				gets(name);
				gets(dem);
				entry = getPhoneNumber(name,book);
				if (entry != NULL) {
					printf("The phone number is %ld.\n",entry->number);
				}
				break;
			case 3:
				n = book.total;
				for( i=0; i<n; i++) {
					printf("%30s %10ld\n",book.entries[i].name,book.entries[i].number);
				}
				break;
			case 4:
				dropPhoneBook(&book);
				break;

		}
	} while (choice != 4);
	return 0;
}

PhoneBook createPhoneBook() {
	PhoneBook p;
	p.entries = (PhoneEntry*)malloc(INITIAL_SIZE*sizeof(PhoneEntry));
	p.total = 0;
	p.size = INITIAL_SIZE;
	return p;
}

void dropPhoneBook(PhoneBook *book) {
	free(book->entries);
	book->total = 0;
	book->size = 0;
}

int binarySearch(PhoneEntry *entries, int l, int r, char *name, int *found) {
	int i, res;
	if(r<l) {
		*found = 0;
		return l;
	}
	i = (l+r)/2;
	res = strcmp(name, entries[i].name);
	if(res == 0) {
		*found = 1;
		return i;
	}
	else if (res < 0) return binarySearch(entries,l,i-1,name,found);
	else return binarySearch(entries,i+1,r,name,found);
}

void addPhoneNumber(char *name, long number, PhoneBook *book) {
	int found, pos;
	if (book->total > 0) 
		pos = binarySearch(book->entries,0,book->total -1,name,&found);
	else {
		pos = 0;
		found = 0;
	}
	if(found)
		book->entries[pos].number = number;
	else {
		if (book->total == INITIAL_SIZE) {
			PhoneEntry *p;
			int newsize = book->size + INCREMENTTAL_SIZE;
			p = (PhoneEntry*)malloc(newsize*sizeof(PhoneEntry));
			memcpy(book->entries,p,INITIAL_SIZE*sizeof(PhoneEntry));
			free(book->entries);
			book->entries = p;
			book->size = newsize;
		}
		if (pos < book->total) {
			memcpy(&(book->entries[pos+1]), &(book->entries[pos]), (book->total - pos)*sizeof(PhoneEntry));
		}
		strcpy(book->entries[pos].name, name);
		book->entries[pos].number = number;
		book->total++;
	}
}

PhoneEntry *getPhoneNumber(char *name, PhoneBook book) {
	int found, pos;
	pos = binarySearch(book.entries,0,book.total -1,name,&found);
	if (found) {
		return &(book.entries[pos]);
	}
	else {
		printf("Khong tim thay lien lac phu hop.\n");
		return NULL;
	}
}