#include<stdio.h>
#include<stdlib.h>

int main() {
	FILE *f;
	int numOfV, numOfE;
	int id1, id2, weight;
	f = fopen("dothi.txt","r");
	fscanf(f,"%d %d",&numOfV,&numOfE);
	printf("%d\t%d\n", numOfV,numOfE);
	fscanf(f,"%d %d %d", &id1, &id2, &weight);
	printf("%d\t%d\t%d\n",id1, id2, weight);
	fclose(f);
	return 0;
}