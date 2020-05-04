#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100
#define INCRE 10

typedef struct {
	char ten[80];
	double diem;
} SV;

typedef struct {
	int total;
	int size;
	SV *sv;
} DS;

DS createClass();
void dropClass(DS *class);
void addSV(char *name, double grade, DS *class);
SV* searchByName(char *name, DS class);
void deleteByName(char *name,DS *class);
SV* searchByGrade(double grade, DS class);
void sortByGrade(DS *class);
void print(DS *class);

int main() {
	int n;
	DS class = createClass();
	DS *tmp;
	char name[50];
	char dem[10];
	SV *sv;
	double diem;

	do {
		printf("-------------------\n");
		printf("1.Nhap them\n");
		printf("2.Tim kiem theo ten\n");
		printf("3.Tim kiem theo ten va xoa\n");
		printf("4.Tim kiem theo diem\n");
		printf("5.Sap xep\n");
		printf("6.In danh sach sinh vien\n");
		printf("7.Thoat\n");
		printf("Lua chon cua ban: ");
		scanf("%d",&n);
		gets(dem);
		switch(n) {
			case 1: 
				printf("Ten sinh vien: ");
				gets(name);
				printf("Diem sinh vien: ");
				scanf("%lf",&diem);
				addSV(name,diem,&class);
				break;
			case 2:
				printf("Ten sinh vien muon tim kiem: ");
				gets(name);
				sv = searchByName(name,class);
				if (sv == NULL) {
					printf("Khong tim thay sinh vien co ten %s trong lop !\n",name);
				}
				else {
					printf("Thong tin sinh vien:\n");
					printf("Ten: %s \tDiem:%.1f\n",name, sv->diem);
				}
				break;
			case 3:
				printf("Ten sinh vien muon xoa: ");
				gets(name);
				deleteByName(name,&class);
				break;
			case 4:
				printf("Nhap diem: ");
				scanf("%lf",&diem);
				sv = searchByGrade(diem, class);
				if (sv == NULL) {
					printf("Khong tim thay sinh vien co diem %.1f trong lop !\n",diem);
				}
				else {
					printf("Thong tin sinh vien:\n");
					printf("Ten: %-30s Diem: %.1f\n",sv->ten, diem);
				}
				break;
			case 5:
				sortByGrade(&class);
				//print(tmp);
				break;
			case 6:
				print(&class);
				break;
			case 7:
				dropClass(&class);
				printf("Cam on da su dung dich vu :D\n");
				break;

		}
	} while (n != 7);
	return 0;
}

DS createClass() {
	DS class;
	class.total = 0;
	class.size = MAX;
	class.sv = (SV*)malloc(MAX*sizeof(SV));
	return class;
}

void dropClass(DS *class) {
	free(class->sv);
	class->total = 0;
	class->size = 0;
}

void addSV(char *name, double grade, DS *class) {
	int total = class->total;
	if (class->total >= class->size) {
		int newsize = class->size + INCRE;
		class->sv = (SV*)realloc(class->sv,INCRE*sizeof(SV));
		class->size = newsize;
	}
	strcpy(class->sv[total].ten,name);
	class->sv[total].diem = grade;
	class->total++;
}

SV* searchByName(char *name, DS class) {
	int i;
	for (i=0; i < class.total; i++) {
		if (strcmp(name,class.sv[i].ten) == 0) 
			return &(class.sv[i]);
	}
	return NULL;
}

void deleteByName(char *name,DS *class) {
	int i=0,j;
	if (class->total == 0) {
		printf("No students in class.\n");
		return;
	}
	while(strcmp(name,class->sv[i].ten) != 0) i++;
	if (i == class->total) {
		printf("Not found %s in class.\n",name);
		return;
	}
	else {
		for (j=i; j < class->total; j++) {
			class->sv[i] = class->sv[i+1];
		}
		class->total--;
	}
}

SV* searchByGrade(double grade, DS class) {
	int i;
	for (i=0; i < class.total; i++) {
		if (class.sv[i].diem == grade) 
			return &(class.sv[i]);
	}
	return NULL;
}

void swap(SV *x, SV *y) {
	SV z;
	z = *x;
	*x = *y;
	*y = z;
}

void sortByGrade(DS *tmp) {
	int i,j;
	// DS tmp = createClass();
	// DS *tmp1;
	// for (i=0; i < class->total; i++) {
	// 	addSV(class->sv[i].ten, class->sv[i].diem, &tmp);
	// }
	for (i=0; i < tmp->total; i++) {
		for (j=i+1; j < tmp->total; j++) {
			if (tmp->sv[i].diem > tmp->sv[j].diem) {
				swap(&(tmp->sv[i]), &(tmp->sv[j]));
			}
		}
	}
	//tmp1 = &tmp;
	//return tmp1;
}

void print(DS *class) {
	int i = 0; 
	for (i=0; i< class->total; i++) {
		printf("Ten: %s \tDiem: %.1f\n",class->sv[i].ten, class->sv[i].diem);
	}
}