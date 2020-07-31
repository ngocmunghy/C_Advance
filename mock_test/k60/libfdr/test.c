#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

	FILE *f;
	char str[30];
	f = fopen("products.txt","r");

	while(fgets(str,30,f) != NULL) {
		int i = 0;
		char number[3],proName[20];
		while(str[i] != ' ') {
			number[i] = str[i];
			i++;
		}
		i++;
		number[i] = '\0';
		int num = atoi(number);
		int j = i;
		i = 0;
		while(str[j] != '\n') {
			proName[i] = str[j];
			i++;
			j++;
		}
		// i++;
		proName[i] = '\0';
		printf("%d\t%s\n", num,proName);
	}

	fclose(f);

	return 0;
}