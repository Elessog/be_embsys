

int getSocketListen(int port){
    listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;

    // init socket to get a file descriptor
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // configure server to listen message comming from in port port
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    // bind and listen
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);
    
    return listenfd;
}

void spin(int socketfd){
    char recv_buff[1024];
    memset(recv_buff, '0', sizeof(recv_buff));
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    int bytes = read(connfd, recv_buff, sizeof(recv_buff));
    close(connfd);
}


int getSocketSend(char *address,int port){
    int sockfd;
    struct sockaddr_in serv_addr;

    // init socket to get a file descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // socket configuration to talk on port port
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, address, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // connect to server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    return sockfd;
}

void sendMessage(char *send_buffer,int socketfd){
    int bytes = write(sockfd, send_buff, strlen(send_buff));

    if(bytes < 0)
    {
        printf("\n Send error \n");
    }
}
