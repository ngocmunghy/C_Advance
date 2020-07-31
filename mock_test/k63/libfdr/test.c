#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	FILE *f;
	f = fopen("abc.txt","r");

	// int numOfE;
	// fscanf(f,"%d",&numOfE);
	char *token;
	char str[50];
	fgets(str,50,f);
	token = strtok(str," ");
	int i = atoi(token);
	printf("%d\n", i);
	while(token != NULL) {
		token = strtok(NULL," ");
		if (token == NULL)
			break;
		int j = atoi(token);
		printf("%d\n",j);
	}
		// 	token = strtok(NULL," ");
		// int j = atoi(token);
		// printf("%d\n",j);

		// 		token = strtok(NULL," ");
		// j = atoi(token);
		// printf("%d\n",j);

		// 		token = strtok(NULL," ");
		// j = atoi(token);
		// printf("%d\n",j);

	fclose(f);

	return 0;
}