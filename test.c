#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encryptDecrypt(char *input, char *output) {
	char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
	
	int i;
	for(i = 0; i < strlen(input); i++) {
		output[i] = input[i] ^ key[i % (sizeof(key)/sizeof(char))];
	}
}

int main(int args, char* argv[])
{
	FILE * fileIn;
	FILE * fileOut;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fileIn = fopen(argv[1], "r");
	if (fileIn == NULL)
	{
		printf("Error while loading input file\n");
		exit(EXIT_FAILURE);
	}

	fileOut = fopen(argv[2], "w+");
	if (fileOut == NULL)
	{
		printf("Error while loading output file\n");
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, fileIn)) != -1)
	{
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
		char decrypted[len];
		encryptDecrypt(line, decrypted);
		fputs(decrypted, fileOut);
	}

	fclose(fileIn);
	fclose(fileOut);
	if (line)
	   free(line);
	exit(EXIT_SUCCESS);
}
