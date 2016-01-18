#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>

#include <opts.h>
#include <handler.h>

struct DECIMAL_COORD
{
    int time;
    float latitude;
    float longitude;
};

struct DECIMAL_COORD decimal_coord = {0, 0.0, 0.0};
struct HANDLERS handlers;

//-----------------------------------------------------------------------------
void signals_handler(int signal_number)
{
    printf("Signal catched.\n");
    hndclose(&handlers);
    exit(EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
float to_decimal(float value)
{
    int deg = floor(value/100);
    float min = (value - deg*100)/100;

    return deg + min*100/60;
}

//-----------------------------------------------------------------------------
void *convert(void * mut)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*) mut;
    
    while(1)
    {
        sem_wait(handlers.sem);
        int time = handlers.shdata->time;
        float latitude = handlers.shdata->latitude; 
        float longitude = handlers.shdata->longitude;
        sem_post(handlers.sem);

        pthread_mutex_lock(mutex);
        decimal_coord.time = time;
        decimal_coord.latitude = to_decimal(latitude);
        decimal_coord.longitude = to_decimal(longitude);
        pthread_mutex_unlock(mutex);
        usleep(500000);
    }
}

//-----------------------------------------------------------------------------
void *display(void * mut)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*) mut;

    while(1)
    {
        pthread_mutex_lock(mutex);
        printf("time: %d\n", decimal_coord.time);
        printf("latitude: %f\n", decimal_coord.latitude);
        printf("longitude: %f\n", decimal_coord.longitude);
        printf("\n");
        fflush(stdout);
        pthread_mutex_unlock(mutex);
        sleep(1);
    }
}

//-----------------------------------------------------------------------------
int main(int argc, char *argv [])
{
	// parse args
	struct OPTS opts;
	if (parse_args(argc, argv, &opts) == -1)
		exit(EXIT_FAILURE);

	// open handlers
	if (hndopen(opts, &handlers) == -1)
		exit(EXIT_FAILURE);

    // init mutex
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // init pthread
    pthread_t thread1;
    if (pthread_create(&thread1, NULL, display, (void*) &mutex) != 0)
        exit(EXIT_FAILURE);
    
    pthread_t thread2;
    if (pthread_create(&thread2, NULL, convert, (void*) &mutex) != 0)
        exit(EXIT_FAILURE);

    // signals handler
    struct sigaction action;
    action.sa_handler = signals_handler;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

    // join
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
	// close
    hndclose(&handlers);

	exit(EXIT_SUCCESS);
}
