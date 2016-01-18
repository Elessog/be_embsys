#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <syslog.h>
#include <util.h>
#define MAX(a,b) ((a) > (b) ? a : b)

int g_fd;
int g_fd2;
int g_argc;

int canal_son[2];
int canal_father[2];
char mesaj_son[20];
char mesaj_father[20];

void signa_ctrlc_over(int signal_number)//TODO
{
   close(g_fd);
   if (g_argc == 5){
     close(g_fd2);
   }
   exit(EXIT_SUCCESS);
}

void send_date(char* buffer){
  if (is_gll(buffer)==0){
    char *time;
    get_time(buffer, time);
    write(canal_father[1],time,6);
    //printf ("father :%s\n",time);
  }
}

void father_process(){
  // read port
    char buff[50];
    fd_set fdset;

    while(1)
    {    
        bzero(buff, sizeof(buff));
    
        FD_ZERO(&fdset);
        FD_SET(g_fd, &fdset);
        if (g_argc==5)
        {
			FD_SET(g_fd2, &fdset);
            int result = select(MAX(g_fd,g_fd2)+1, &fdset, NULL, NULL, NULL);
            //if (result == -1) {
            //  printf("error\n");
            //  exit(1);
            //}
            if (FD_ISSET(g_fd, &fdset))
		    {
				//printf("1\n");
		        int bytes = read (g_fd, buff, sizeof(buff));

		        if (bytes > 0)
		        {
		            printf("1 :%s\n", buff);
                    send_date(buff);
		        }
		        fflush(stdout);
		    }
            bzero(buff, sizeof(buff));
            if (FD_ISSET(g_fd2, &fdset))
		    {
				//printf("2: %d\n", fd2);
		        int bytes = read (g_fd2, buff, sizeof(buff));
				if (bytes == -1)
					perror("read");
				//printf("%d\n", bytes);

		        if (bytes > 0)
		        {
		            printf("2 :%s\n", buff);
                    send_date(buff);
		        }
		        fflush(stdout);
		    }
        }
        else
        {
            select(g_fd+1, &fdset, NULL, NULL, NULL);

            if (FD_ISSET(g_fd, &fdset))
		    {
		        int bytes = read (g_fd, buff, sizeof(buff));

		        if (bytes > 0)
		        {
		            printf("%s\n", buff);
                    send_date(buff);
		            fflush(stdout);
		        }
		    }
        }
    }
}



void child_process(){
   fd_set pdset;
   while(1)
   {
      FD_ZERO(&pdset);
      FD_SET(canal_father[0], &pdset);
      select(canal_father[0]+1, &pdset, NULL, NULL, NULL);
      if (FD_ISSET(canal_father[0], &pdset))
	  {
         read(canal_father[0],mesaj_father,6);
         printf("child sleeping Time: %s ; PID Father %d\n",mesaj_father,getppid());
      }
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
    char * port = NULL;

    char * port2 = NULL;
    g_argc = argc;
    // parse command line
    if (argc != 3 && argc !=5 )
    {   printf("nb arg %d\n",argc); 
        fprintf(stderr, "Invalid usage: reader -p port_name [-s port_name2]\n");
        exit(EXIT_FAILURE);
    }
    pipe(canal_father);
    pipe(canal_son);

 // signals handler
    struct sigaction action;
    action.sa_handler = signa_ctrlc_over;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

    char * options = "p:s:";
    int option;
    while((option = getopt(argc, argv, options)) != -1)
    {
        switch(option)
        {
            case 'p':
                port = optarg;
                break;
            case 's':
                port2 = optarg;
                break;

            case '?':
                fprintf(stderr, "Invalid option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
        printf("arg %c %s\n",option,optarg);
    }

    // open serial port
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    tcflush(fd, TCIOFLUSH);
    printf("Port 1 open %s\n",port);
    //open second serial port if indicated
    g_fd = fd;
    int fd2;
    if (argc==5){
       fd2 = open(port2, O_RDWR | O_NOCTTY);
       if (fd2 == -1)
       {
          perror("open");
          exit(EXIT_FAILURE);
       }
       tcflush(fd2, TCIOFLUSH);
       printf("Port 2 open %s\n",port2);
       g_fd2 =fd2;
    }
    
    int pid; /* PID child */
    int i;
    
    pid = fork();
    switch (pid) {
    case -1:
        printf("Erreur: echec du fork()\n");
        close(fd);
        if (argc==5){
            close(fd2);
        }
        exit(1);
        break;
    case 0:
        child_process();
        break;
    default:
        /* PROCESSUS PERE */
        father_process();
    }
    
    // close serial port
    close(fd);
    if (argc==5){
        close(fd2);
    }
    exit(EXIT_SUCCESS);
}
