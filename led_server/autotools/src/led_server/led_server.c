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

//#include <opts.h>
#include <signal.h>

#define LED_PATH "/sys/class/gpio/gpio21/value" 
#define LED_EXT_PATH "/sys/class/pwm/pwmchip0/pwm4/duty_cycle" 
#define SIZE_OF_BUFF 50 

int fd_led;
int fd_led_ext;
int listenfd;

int write_buffer(int fd, int value) 
{ 
  int ret; 
  char buffer[20]; 
  int buffer_len; 
  ret = sprintf(buffer,"%d", value); 
  buffer_len = strlen(buffer); 
  ret = write(fd, buffer, buffer_len);
  //printf("message written %s  %d\n",buffer,buffer_len);
  if (ret < 0) 
  { 
    perror("Can't write file "); 
    return ret; 
  } 
  return buffer_len; 
}

void signals_handler(int signal_number){
    //stop led
    write_buffer(fd_led, 0);
    close(listenfd);
    close(fd_led);
    close(fd_led_ext);
    closelog();
    exit(EXIT_SUCCESS);
}

void endProg(){
   write_buffer(fd_led, 0);
   close(fd_led);
   close(fd_led_ext);
}

int main(int argc, char *argv[])
{
    // var
    listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;

    char recv_buff[1024];
    time_t ticks;

    memset(recv_buff, '0', sizeof(recv_buff));

    //signal  handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(&(action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT,& action,NULL);

    

    //open led pin
    
    char buffer[SIZE_OF_BUFF]; 
    snprintf(buffer, SIZE_OF_BUFF, "%s", LED_PATH); 
    if ((fd_led = open(buffer, O_WRONLY)) < 0) //write gpio state 
    { 
        perror("Can't open led file"); 
        return -1; 
    } 


    

    // init syslog
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog ("miniserver", LOG_CONS|LOG_PID|LOG_NDELAY, LOG_LOCAL1);

    // init socket to get a file descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // configure server to listen message comming from in port 5000
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    // bind and listen
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    // listen
    printf("Server is listening...\n");
    write_buffer(fd_led, 1);
    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        int bytes = read(connfd, recv_buff, sizeof(recv_buff));

        //open pwm pin
        char buffer2[SIZE_OF_BUFF]; 
        snprintf(buffer2, SIZE_OF_BUFF, "%s", LED_EXT_PATH); 
        if ((fd_led_ext = open(buffer2, O_WRONLY)) < 0) //write pwms
        { 
           perror("Can't open pwm 4 file"); 
           return -1; 
        } 
	write_buffer(fd_led_ext, atoi(recv_buff));
        fsync(fd_led_ext);
        close(fd_led_ext);
        if (bytes > 0)
        close(connfd);
            
            syslog(LOG_NOTICE, "Message received: %s", recv_buff);
            
    }

    close(listenfd);
    closelog();
    atexit (endProg);
    exit(EXIT_SUCCESS);
}
