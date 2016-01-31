#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
char* key_crypt;
int sizeKeyCrypt;

void encryptDecrypt(char* fIn,char* fOut)
{

	FILE * fileIn;
	FILE * fileOut;
        int toCrypt;
        int i=0;

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
            decrypted = toCrypt ^ key_crypt[i % (sizeKeyCrypt)];
            i++;
            fputc(decrypted, fileOut);
	}
        printf("taille char %d\n",i);

        fclose(fileIn);
	fclose(fileOut);
}

int main(int args, char* argv[])
{
    if (args == 3)
      key_crypt = key;
    else if ( args == 4)
       key_crypt = argv[3];
    else
    {
       printf("Usage: %s fileIn fileout [key]\n",argv[0]);
       if (args==2)
           printf("second arg : %s\n",argv[1]);
       exit(EXIT_FAILURE);
    }
    //printf ("%s %d\n",key_crypt,strlen(key_crypt));
    sizeKeyCrypt = strlen(key_crypt);

    encryptDecrypt(argv[1],argv[2]);//encrypàt or decrypt the file

    exit(EXIT_SUCCESS);
}
