#ifndef HANDLER_H
#define HANDLER_H

#include <shm_writer/shdata.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <semaphore.h>

#include <opts.h>

//-----------------------------------------------------------------------------
struct HANDLERS
{
    int sockfd;
    struct sockaddr_in info_me;
	char * shm;
    sem_t * sem;
	int shmfd;
	struct SHDATA *shdata;
};

//-----------------------------------------------------------------------------
int hndopen(struct OPTS opts, struct HANDLERS *handlers);
void hndclose(struct HANDLERS *handlers);

#endif // HANDLER_H
