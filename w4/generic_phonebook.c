#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5

typedef struct {
	void *key;
	void *value;
} Entry;

typedef struct {
	Entry *entries;
	int size, total;
	Entry (*makeNode) (void*,void*);
	int (*compare) (void*,void*);
} SymbolTable;

SymbolTable createSymbolTable(Entry (*makeNode)(void*,void*), int (*compare)(void*,void*));
void dropSymbolTable(SymbolTable *tab);
void addEntry(void *key, void *value, SymbolTable *book);
Entry* getEntry(void *key, SymbolTable book);

Entry makePhone(void *name, void *phone) {
	Entry res;
	res.key = strdup((char*)name);
	res.value = malloc(sizeof(long));
	memcpy(res.value,phone,sizeof(long));
	return res;
}

int comparePhone(void *key1, void *key2) {
	return strcmp((char*)key1, (char*)key2);
}


int main() {
	int i,n,choice;
	long sdt;
	char name[30];
	char dem[30];
	Entry *entry;
	SymbolTable book = createSymbolTable(makePhone,comparePhone);
	sdt = 8532827592;
	addEntry("Dung",&sdt,&book);
	sdt = 4651172852;
	addEntry("Hoa",&sdt,&book);
	sdt = 6172356216;
	addEntry("Manh",&sdt,&book);

	do {
		printf("\n");
		printf("1.Add phone\n");
		printf("2.Search phone\n");
		printf("3.Print\n");
		printf("4.Exit\n");
		printf("Your choice: \n");
		scanf("%d",&choice);
		switch(choice) {
			case 1:;
				long sdt;
				printf("Name: ");
				gets(name);
				gets(dem);
				printf("Phone number: ");
				scanf("%ld",&sdt);
				addEntry(name,&sdt,&book);
				break;
			case 2:;
				char name[100];
				printf("Name: ");
				gets(name);
				gets(dem);
				entry = getEntry(name,book);
				if (entry != NULL) {
					printf("The phone number of %s is %ld.\n",name,*((long*)entry->value));
				}
				else printf("Not found the phone number for %s.\n",name);
				break;
			case 3:
				n = book.total;
				for( i=0; i<n; i++) {
					printf("%-30s%10ld\n",*((char*)book.entries[i].key),*((long*)book.entries[i].value));
				}
				break;
			case 4:
				dropSymbolTable(&book);
				break;

		}
	} while (choice != 4);

	return 0;
}

SymbolTable createSymbolTable(Entry (*makeNode)(void*, void*), int (*compare)(void*, void*)) {
	SymbolTable tab;
	tab.size = INITIAL_SIZE;
	tab.total = 0;
	tab.entries = (Entry*)malloc(INITIAL_SIZE*(sizeof(Entry)));
	tab.makeNode = makeNode;
	tab.compare = compare;
	return tab;
}

void dropSymbolTable(SymbolTable *tab) {
	int i;
	for (i=0; i<tab->total; i++) {
		free(tab->entries[i].key);
		free(tab->entries[i].value);
	}
	free(tab->entries);
	tab->total = 0;
	tab->size = 0;
}

int binarySearch(Entry *entries, int left, int right, void *key, int *found, int (*compare)(void *, void *))
{
    if (left > right)
    {
        *found = 0;
        return left;
    }

    int mid = (left + right) / 2;
    int cmp = compare(key, entries[mid].key);

    if (cmp == 0)
    {
        *found = 1;
        return mid;
    }
    else if (cmp > 0)
    {
        return binarySearch(entries, mid + 1, right, key, found, compare);
    }
    else
    {
        return binarySearch(entries, left, mid - 1, key, found, compare);
    }
}

void addEntry(void *key, void *value, SymbolTable *book) {
	int found,pos;
	if(book->total > 0) 
		pos = binarySearch(book->entries,0,book->total - 1, key, &found, book->compare);
	else {
		found = 0;
		pos = 0;
	}
	if(found) {
		free(book->entries[pos].key);
		free(book->entries[pos].value);
		book->entries[pos] = book->makeNode(key,value);
	}
	else { 
		if (book->total == INITIAL_SIZE) { // cap phat them vung nho + cap nhat book->size
			Entry *p;
			int newsize = INITIAL_SIZE+INCREMENTAL_SIZE;
			p = (Entry*)malloc(newsize*(sizeof(Entry)));
			memcpy(p,book->entries,(book->total)*sizeof(Entry));
			free(book->entries);
			book->entries = p;
			book->size = newsize;
		}
		if(pos < book->total) { // dich chuyen cac book->entries sau vi tri pos 1 don vi
			memcpy(&book->entries[pos+1],&book->entries[pos],(book->total - pos)*sizeof(Entry));
		}
		book->entries[pos] = book->makeNode(key,value);
		book->total++;
	}
}

Entry* getEntry(void *key, SymbolTable book) {
	int found, pos;
	pos = binarySearch(book.entries,0,book.total - 1,key,&found,book.compare);
	if(found) {
		return &(book.entries[pos]);
	}
	else return NULL;
}