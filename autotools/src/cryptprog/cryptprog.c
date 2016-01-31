#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/stat.h>
#include "src/cryptprog/list.h"

int chnmotif(char * motif, char* ch);

static char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
char* key_crypt;
int sizeKeyCrypt;


typedef struct fileName_ {
    int num;
    char name[100];
    char dataInf[300];
    int size;
} nodeFile;

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

T_LIST *getFileList(int *size,char *folder_path,int *bSize){
    // This function look in folder_path for filme to crypt
   // and put the source in a LIST
    struct dirent* file = NULL;
    struct stat data;
    DIR* folder = opendir(folder_path);
    int nbFile = 0;
    T_LIST *listFic = malloc(sizeof(T_LIST));

    if (folder == NULL) {
        perror("folder");
        return NULL;
    }
   
    while ((file = readdir(folder)) != NULL){
        
        if ((chnmotif(".decrypt",file->d_name)) != 1&&strcmp(".",file->d_name)!=0 &&strcmp("..",file->d_name)!=0) {
            nbFile++;
            nodeFile *newNode  = malloc(sizeof(nodeFile));

            newNode->num = nbFile;
            strcpy(newNode->name,file->d_name); 
            sprintf(newNode->dataInf,"%s/%s",folder_path,file->d_name);

            stat(newNode->dataInf,&data) ; //get size of file
            newNode->size = data.st_size;
            *bSize+=newNode->size;
            syslog(LOG_NOTICE, "Detection of file : %s size :%d",newNode->dataInf,newNode->size);
            ajout_tete(newNode,listFic);
        }
    }

    closedir(folder);

    *size = nbFile;

    return listFic;
}


int chnmotif(char * motif, char* ch) {
  int i = 0;
  int found = 0;
  char * m = motif;
  while (*ch != '\0') {
	i = 0;
	m = motif;
	while (*ch == *m && *m != '\0') {
	  ch++; i++; m++;
	}

	if (*m == '\0')
	  found++;
	ch++;
  }
  return found;
}


int compare(int val,nodeFile node){
    return val==node.num;
}
