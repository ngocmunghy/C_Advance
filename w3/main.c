#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"jval.h"

Jval* create_array_i(int n) {
	Jval *arr = (Jval*)malloc(n*sizeof(Jval));
	return arr;
}

int jval_compare_int(Jval *x, Jval *y) {
	int m,n;
	m = jval_i(*x);
	n = jval_i(*y);
	if (m == n) return 0;
	return m > n ? 1:-1;
}

void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void exch(Jval* a,Jval* b)
{
	void* temp = (void*)malloc(sizeof(a));
	int i;

	void* x = (void*)a;
	void* y = (void*)b;

	for(i = 0;i<sizeof(a);i++)
	{
		*((char*)temp+i) = *((char*)x+i);
		*((char*)x+i) = *((char*)y+i);
		*((char*)y+i) = *((char*)temp+i);
	}

	return;
}

void sort_gen (Jval a[],int l,int r,int (*compare)(Jval*,Jval*))
{
	if(r <= l) return;
	int i = l-1;int j = r;
	int p = l-1;int q = r;
	while(1) {
		while(compare(&a[++i],&a[r])<0);
		while(compare(&a[--j],&a[r])>0) if(j == l) break;
		if(i >= j) break;
		exch(&a[i],&a[j]);
		if(compare(&a[i],&a[r]) == 0) exch(&a[i],&a[++p]);
		if(compare(&a[j],&a[r]) == 0) exch(&a[j],&a[--q]);
	}
	exch(&a[i],&a[r]);
	j = i-1;
	i = i+1;
	for(int k = l; k <= p; k++) exch(&a[k],&a[j--]);
	for(int k = r-1; k >= q; k--) exch(&a[k],&a[i++]);
	sort_gen(a,l,j,compare);
	sort_gen(a,i,r,compare);
}


int main() {
	srand(time(NULL));
	Jval a,b;
	a = new_jval_i(5);
	b = new_jval_f(3.14);

	int r;
	int i,j;
	int n = 10;
	Jval *A = create_array_i(n);

	for(i=0;i<n;i++) {
		r = rand()%100;
		A[i] = new_jval_i(r);
	}

	for(i=0;i<n;i++) {
		printf("%d	", jval_i(A[i]));
	}

	//sort_gen(A,0,9,jval_compare_int);
	for(i=0;i<n;i++) {
		for(j=0;j<n;j++) {
			if(jval_compare_int(&A[i],&A[j]) < 0)
				swap((int*)&A[i],(int*)&A[j]);
		}
	}

	printf("\n");

	for(i=0;i<n;i++) {
		printf("%d	",jval_i(A[i]));
	}
	printf("\n");
	return 0;
}