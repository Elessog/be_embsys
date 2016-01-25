#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encryptDecrypt(char* fIn,char* fOut)
{

	FILE * fileIn;
	FILE * fileOut;
        int toCrypt;
        int i=0;
        char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
        char decrypted;

	fileIn = fopen(fIn, "r");
	if (fileIn == NULL)
	{
	    printf("Error while loading input file\n");
	    exit(EXIT_FAILURE);
	}

	fileOut = fopen(fOut, "w+");
	if (fileOut == NULL)
	{
            printf("Error while loading output file\n");
	    exit(EXIT_FAILURE);
	}

	while ((toCrypt = fgetc(fileIn))!=EOF)
	{
            decrypted = toCrypt ^ key[i % (sizeof(key)/sizeof(char))];
            i++;
            fputc(decrypted, fileOut);
	}
        printf("taille char %d\n",i);

        fclose(fileIn);
	fclose(fileOut);
}

int main(int args, char* argv[])
{
    if (args!=3)
    {
       printf("Usage: %s fileIn fileout\n",argv[0]);
       if (args==2)
           printf("second arg : %s\n",argv[1]);
       exit(EXIT_FAILURE);
    }

    encryptDecrypt(argv[1],argv[2]);

    exit(EXIT_SUCCESS);
}
