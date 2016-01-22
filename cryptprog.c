#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h> 
#include <fcntl.h> 

#define SIZE_OF_BUFF 512

char* decrypt(char* input,char* output)
{

    char key[] = {'K', 'C', 'Q'}; //Can be any chars, and any size array

    int i;
    printf("%d",(int)strlen(input));
    for(i = 0; i < strlen(input); i++)
    {
        printf("%c",input[i]);
        output[i] = input[i] ^ key[i % (sizeof(key)/sizeof(char))];
    }

    return output;
}

int fileManage(char* nameIn, char *nameOut){
    int fd_in;
    int fd_out;
    char buffer[SIZE_OF_BUFF]; 

    if (!(fd_in = open(nameIn, O_RDONLY))){
        perror("Can't open file in "); 
        return -1; 
    }

    if (!(fd_out = open(nameOut, O_WRONLY)) ){
        perror("Can't open file out"); 
        return -1; 
    }
    memset(buffer,'\0',SIZE_OF_BUFF);
    while(read(fd_in, buffer, SIZE_OF_BUFF) > 0)
    {
      printf("1\n");
      char cryptedBuffer[SIZE_OF_BUFF];
      memset(cryptedBuffer,'\0',SIZE_OF_BUFF);
      printf("2\n");
      decrypt(buffer,cryptedBuffer);

      printf("3\n");
      char *toWrite ;
      sprintf(toWrite,"%s",cryptedBuffer);
      printf("4\n");
      write(fd_out,toWrite,strlen(toWrite));
      printf("5\n");
    }
    close(fd_in);
    close(fd_out);

    return 0;
}

int main(int argc,char *argv[]){
    

    if (argc!=3){
        perror("Using cryptprog nameTodecrypt newFile"); 
        return -1;
    }

    fileManage(argv[1],argv[2]);
    

}


