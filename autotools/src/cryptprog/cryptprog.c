#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define MAX(a,b) ((a) > (b) ? a : b)
#define STR_LEN 100

int chnmotif(char * motif, char* ch);
int freeThread(int *status,int size);
int alreadyTreated(char *fileName,char *rep);
void feed(int size);
void *cryptingThread(void *arg);

static char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array
char* key_crypt;
int sizeKeyCrypt;

char (*fileName)[2*STR_LEN];
char (*fileNameOut)[2*STR_LEN];
int *bSize;

int canal_son[5][2];
int canal_father[5][2];
char mesaj_son[5];
char mesaj_father[5][10];

struct HANDLERS {
   int thread_id;
   pthread_mutex_t *mutex;
};

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
        fflush(fileOut);
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
    sizeKeyCrypt = strlen(key_crypt);

    //encryptDecrypt(argv[1],argv[2]);//encrypàt or decrypt the file
    char *folder_path= argv[1];
    
    int nbFile = 0;
    struct dirent* file = NULL;
    struct stat data;
    DIR* folder = opendir(folder_path);

    if (folder == NULL) {
        perror("folder");
        return;
    }
   //get number of file to work on 
    while ((file = readdir(folder)) != NULL){
        if ((chnmotif(".decrypt",file->d_name)) != 1&&strcmp(".",file->d_name)!=0 
              &&strcmp("..",file->d_name)!=0&&file->d_type!=DT_DIR&&
                                                     alreadyTreated(file->d_name,folder_path)==0)
       {
            nbFile++;
        }
    }
    closedir(folder);

    folder = opendir(folder_path);

    fileName =  malloc(nbFile*sizeof(*fileName));
    fileNameOut =  malloc(nbFile*sizeof(*fileNameOut));
    bSize = (int*) malloc(nbFile*sizeof(int));

    nbFile = 0;//get file to work on
    while ((file = readdir(folder)) != NULL){
        if ((chnmotif(".decrypt",file->d_name)) != 1&&strcmp(".",file->d_name)!=0 &&
              strcmp("..",file->d_name)!=0&&file->d_type!=DT_DIR&&
                                                   alreadyTreated(file->d_name,folder_path)==0) 
        {
            sprintf(fileName[nbFile],"%s/%s",folder_path,file->d_name);
            if (chnmotif(".crypt",file->d_name) >= 1)
            {
               char temp[STR_LEN];
               sscanf(file->d_name,"%s",temp);
               temp[strlen(file->d_name)-6] = '\0';
               sprintf(fileNameOut[nbFile],"%s/%s.decrypt",folder_path,temp);
            }
            else
               sprintf(fileNameOut[nbFile],"%s/%s.crypt",folder_path,file->d_name);

            stat(fileName[nbFile],&data) ; //get size of file
            bSize[nbFile] = data.st_size;
            printf("File %d :  %s,size %d\n",nbFile,file->d_name,bSize[nbFile]);
            syslog(LOG_NOTICE,"File %d :  %s,size %d\n",nbFile,file->d_name,bSize[nbFile]);
            nbFile++;
        }
    }
    closedir(folder);

    int i;
    pthread_t thread[5];
    // init mutex
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for(i=0;i<5;i++)
    {
        //create thread
        pipe(canal_father[i]);
        pipe(canal_son[i]);
        struct HANDLERS *hand = malloc(sizeof(struct HANDLERS));
        hand->thread_id=i;
        hand->mutex=&mutex;
        if (pthread_create(&thread[i], NULL, cryptingThread,(void *)hand) != 0)
             exit(EXIT_FAILURE);
    }
 
    //launch feeding function
   feed(nbFile);

    for(i=0;i<5;i++)
    {
        //join thread
        pthread_join(thread[i],NULL);
    }

    free(fileName);
    free(fileNameOut);
    free(bSize);
    closelog();
    exit(EXIT_SUCCESS);
}

//checked if file has already been crypted or decrypted
int alreadyTreated(char *fileName,char *rep){
    struct dirent* file = NULL;
    struct stat data;
    DIR* folder = opendir(rep);
    char temp[STR_LEN];
    char temp2[STR_LEN];
    sscanf(fileName,"%s",temp);
    if (chnmotif(".crypt",fileName) >= 1){
       temp[strlen(fileName)-6] = '\0';
       sprintf(temp2,"%s.decrypt",temp);
    }
    else{
       sprintf(temp2,"%s.crypt",fileName);
    }
    while ((file = readdir(folder)) != NULL)
    {
        if (strcmp(temp2,file->d_name)==0) {
            closedir(folder) ; 
           return 1;
        }
    }
   closedir(folder) ;
   return 0;
}

void feed(int size)
{
  int fileDone = 0;
  int status[5] = {0,0,0,0,0};
  int freeT;
  //loop
  fd_set pdset;
  while (fileDone<size) {
    freeT = freeThread(status,5);
    if ( freeT != -1)
    {
       //send message to thread[freeT];
       char temp[10];
       sprintf(temp,"%d",fileDone);
       printf("Send to Thread %d  file %s\n",freeT,temp);
       syslog(LOG_NOTICE,"Send to Thread %d  file %s\n",freeT,temp);
       write(canal_father[freeT][1],temp,10);
       fileDone++;
       status[freeT]=1;
    }
    else
   {
     //wait for freeThread message
    FD_ZERO(&pdset);
    int k;
    int max = canal_son[0][0];
    for(k=0;k<5;k++){
       FD_SET(canal_son[k][0], &pdset);
       max = MAX(max,canal_son[k][0]);
    }
    select(max+1, &pdset, NULL, NULL, NULL);

    for(k=0; k<5; k++)
    {
      if (FD_ISSET(canal_son[k][0], &pdset))
      {
         printf("Thread %d is now free\n",k);
         syslog(LOG_NOTICE,"Thread %d is now free\n",k);
         read(canal_son[k][0],&(mesaj_son[k]),1);
         status[k]=0;
      }
    }
   }
  }
  //send sig to thread
  int k;
  char temp[10];
  sprintf(temp,"%d",99999);
  for(k=0;k<5;k++)
     write(canal_father[k][1],temp,10);
}

int freeThread(int *status,int size)
{
  while(size>0)
 {
    if (status[size-1]==0)
      return size-1;
    size--;
  }
  return -1;
}

void *cryptingThread(void *arg){
   struct HANDLERS *hand = (struct HANDLERS*)arg;
   pthread_mutex_t *mutex = hand->mutex;
   int id = hand->thread_id;
   printf("Thread %d is active\n",id);
   syslog(LOG_NOTICE,"Thread %d is active\n",id);
   fd_set pdset;
   while(1)
   {
      FD_ZERO(&pdset);
      FD_SET(canal_father[id][0], &pdset);
      select(canal_father[id][0]+1, &pdset, NULL, NULL, NULL);
      if (FD_ISSET(canal_father[id][0], &pdset))
      {
         read(canal_father[id][0],mesaj_father[id],10);
         int  idFile; 
         int oldFile = 9999999;
         sscanf(mesaj_father[id],"%d",&idFile);
         if (idFile==99999){
            free(hand);
            printf("Thread %d stopping\n",id);
            syslog(LOG_NOTICE,"Thread %d stopping\n",id);
            return;
         }
         else if (idFile==oldFile){
            free(hand);
            printf("Error in pipe comThread %d stopping\n",id);
            syslog(LOG_ERR,"Error in pipe com Thread %d stopping\n",id);
            return;
         }
         oldFile=idFile;
         printf("Thread %d now working on %s\n",id,fileName[idFile]);
         syslog(LOG_NOTICE,"Thread %d now working on %s\n",id,fileName[idFile]);
         encryptDecrypt(fileName[idFile],fileNameOut[idFile]);
         printf("Thread %d finished working on %s\n",id,fileName[idFile]);
         syslog(LOG_NOTICE,"Thread %d finished working on %s\n",id,fileName[idFile]);
         char *c={'\0'};
         write(canal_son[id][1],&c,1);
      }
   }
   free(hand);
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
