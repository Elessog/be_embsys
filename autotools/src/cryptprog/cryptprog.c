#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define STR_LEN 100

int chnmotif(char * motif, char* ch);

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
        syslog(LOG_NOTICE, "End (de)crypting %s in %s . Number of Bytes : %d",fIn,fOut,i);

        fclose(fileIn);
	fclose(fileOut);
}

int main(int args, char* argv[])
{
    // init syslog
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("cryptoserver", LOG_CONS|LOG_PID|LOG_NDELAY, LOG_LOCAL1);

    if (args == 2)
      key_crypt = key;
    else if ( args == 3)
       key_crypt = argv[2];
    else
    {
       printf("Usage: %s folder  [key]\n",argv[0]);
       if (args==2)
           printf("second arg : %s\n",argv[1]);
       exit(EXIT_FAILURE);
    }
    //printf ("%s %d\n",key_crypt,strlen(key_crypt));
    sizeKeyCrypt = strlen(key_crypt);

    //encryptDecrypt(argv[1],argv[2]);//encrypàt or decrypt the file
    char *folder_path= argv[1];
    char (*fileName)[2*STR_LEN];
    char (*fileNameOut)[2*STR_LEN];
    int *bSize;
    int nbFile = 0;
    struct dirent* file = NULL;
    struct stat data;
    DIR* folder = opendir(folder_path);


    if (folder == NULL) {
        perror("folder");
        return;
    }
   
    while ((file = readdir(folder)) != NULL){
        if ((chnmotif(".decrypt",file->d_name)) != 1&&strcmp(".",file->d_name)!=0 
              &&strcmp("..",file->d_name)!=0&&file->d_type!=DT_DIR)
       {
            nbFile++;
        }
    }
    closedir(folder);

    folder = opendir(folder_path);

    fileName =  malloc(nbFile*sizeof(*fileName));
    fileNameOut =  malloc(nbFile*sizeof(*fileNameOut));
    bSize = (int*) malloc(nbFile*sizeof(int));

   
    nbFile = 0;
    while ((file = readdir(folder)) != NULL){
        if ((chnmotif(".decrypt",file->d_name)) != 1&&strcmp(".",file->d_name)!=0 &&
              strcmp("..",file->d_name)!=0&&file->d_type!=DT_DIR) 
        {
            sprintf(fileName[nbFile],"%s/%s",folder_path,file->d_name);

            if (chnmotif(".crypt",file->d_name) >= 1)
            {
               char temp[STR_LEN];
               sscanf(file->d_name,"%s",temp);
               temp[strlen(file->d_name)-6] = '\0';
               //printf("%s %s\n",temp,file->d_name);
               sprintf(fileNameOut[nbFile],"%s/%s.decrypt",folder_path,temp);
            }
            else
               sprintf(fileNameOut[nbFile],"%s/%s.crypt",folder_path,file->d_name);

            stat(fileName[nbFile],&data) ; //get size of file
            bSize[nbFile] = data.st_size;
            syslog(LOG_NOTICE, "Detection of file : %s size :%d"
                        ,fileName[nbFile],bSize[nbFile]);
            //printf("%s  %d\n",file->d_name,bSize[nbFile]);
            nbFile++;
        }
    }
    closedir(folder);
    int i;

    for (i=0; i<nbFile ; i++)
        encryptDecrypt(fileName[i],fileNameOut[i]);

    free(fileName);
    free(fileNameOut);
    free(bSize);
    closelog();
    exit(EXIT_SUCCESS);
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
