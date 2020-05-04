#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INITIAL_SIZE 10
#define INCREMENTAL_SIZE 5

typedef struct {
	void *key;
	void *value;
} Entry;

typedef struct {
	Entry *entries;
	int total,size;
	Entry (*makeNode)(void*, void*);
	int (*compare)(void*, void*);
} SymbolTable;

SymbolTable createSymbolTable(Entry (*makeNode)(void*, void*), int (*compare)(void*, void*));
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
	return strcmp((char*)key1,(char*)key2);
}

int main() {
	int i,n;
	long sdt;
	char name[80];
	char dem[30];
	Entry *entry;
	SymbolTable book = createSymbolTable(makePhone,comparePhone);
	sdt = 6666777;
	addEntry("Dung",&sdt,&book);
	sdt = 7777888;
	addEntry("Thang",&sdt,&book);
	sdt = 1234432;
	addEntry("Anh",&sdt,&book);
	sdt = 1234321;
	addEntry("Dat",&sdt,&book);

	// entry = getEntry("Dat",book);
	// printf("%ld\n",*((long*)entry->value));

	do {
		printf("1.Add phone\n2.Search phone\n3.Print list\n4.Exit\nYour choice: ");
		scanf("%d",&n);
		gets(dem);
		switch(n) {
			case 1:
				printf("Name: \n");
				gets(name);
				printf("Number: \n");
				scanf("%ld",&sdt);
				addEntry(name,&sdt,&book);
				break;
			case 2:
				printf("Name: \n");
				gets(name);
				entry = getEntry(name,book);
				if(entry == NULL) {
					printf("Not found number for %s\n",name );
				}
				else printf("Phone number of %s is %ld\n",name,*((long*)entry->value));
				break;
			case 3:
				for (i=0; i<book.total; i++) {
					printf("%-15s%20d\n",book.entries[i].key,*((long*)book.entries[i].value));	
				}
				break;
			case 4:
				dropSymbolTable(&book);
				break;
		}
	} while (n != 4);
	return 0;
}

SymbolTable createSymbolTable(Entry (*makeNode)(void*, void*), int (*compare)(void*, void*)) {
	SymbolTable tab;
	tab.total = 0;
	tab.size = INITIAL_SIZE;
	tab.entries = (Entry*)malloc(INITIAL_SIZE*sizeof(Entry));
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

int binarySearch(Entry *entries, int l, int r, void *key, int *found, int (*compare)(void*, void*)) {
	if (l > r) {
		*found = 0;
		return l;
	}
	int mid = (l+r)/2;
	int res = compare(key,entries[mid].key);
	if (res == 0) {
		*found = 1;
		return mid;
	}
	else if (res < 0) return binarySearch(entries,l,mid-1,key,found,compare);
	return binarySearch(entries,mid+1,r,key,found,compare);
}

void addEntry(void *key, void *value, SymbolTable *book) {
	int found, pos;
	if (book->total > 0 ){
		pos = binarySearch(book->entries,0,book->total -1, key, &found, book->compare);
	}
	else {
		found = 0;
		pos = 0;
	}
	if (found) {
		free(book->entries[pos].key);
		free(book->entries[pos].value);
		book->entries[pos] = book->makeNode(key,value);
	}
	else {
		if (book->total >= book->size) {
			Entry *p;
			int newsize = book->size + INCREMENTAL_SIZE;
			p = (Entry*)malloc(newsize*sizeof(Entry));
			memcpy(p,book->entries,(book->total)*sizeof(Entry));
			free(book->entries);
			book->entries = p;
			book->size = newsize;
		}
		if (pos < book->total) {
			memcpy(&(book->entries[pos+1]),&(book->entries[pos]),(book->total - pos)*sizeof(Entry));
		}
		book->entries[pos] = book->makeNode(key,value);
		book->total++;
	}
}

Entry* getEntry(void *key, SymbolTable book) {
	int i;
	for (i=0; i< book.total; i++) {
		if (*((char*)book.entries[i].key) == *((char*)key)) 
			return &(book.entries[i]);
	}
	return NULL;
}