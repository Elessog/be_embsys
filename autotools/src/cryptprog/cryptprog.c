#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

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
            syslog(LOG_ERR, "Error while reading input file : %s", fIn);
	    exit(EXIT_FAILURE);
	}

	fileOut = fopen(fOut, "w+");
	if (fileOut == NULL)
	{
            printf("Error while loading output file\n");
            syslog(LOG_ERR, "Error while creating output file : %s", fOut);
	    exit(EXIT_FAILURE);
	}

        syslog(LOG_NOTICE, "Start (de)crypting %s", fIn);
	while ((toCrypt = fgetc(fileIn))!=EOF)
	{
            decrypted = toCrypt ^ key_crypt[i % (sizeKeyCrypt)];
            i++;
            fputc(decrypted, fileOut);
	}
        syslog(LOG_NOTICE, "End (de)crypting %s in %s . Number of Bytes : %d", fIn,fOut,i);

        fclose(fileIn);
	fclose(fileOut);
}

int main(int args, char* argv[])
{

    // init syslog
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("cryptoserver", LOG_CONS|LOG_PID|LOG_NDELAY, LOG_LOCAL1);

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

   closelog();
    exit(EXIT_SUCCESS);
}
