// Question 8 : write here the buggy function without errors
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

#define KNOT_TO_KMH 1.852
int knot_to_kmh_str(float not, size_t size, char * format, char * kmh_str)
{
    float kmh = KNOT_TO_KMH * not;
    snprintf(kmh_str, size, format, kmh);

    return kmh;
}

void signa_ctrlc_over(int signal_number)
{

}

/*
int printf(const char *format, ...)
{
  
    va_list list;
    char *parg;
    typeof(printf) *old_printf;

    // format variable arguments
    va_start(list, format);
    vasprintf(&parg, format, list);
    va_end(list);

 // signals handler
    struct sigaction action;
    action.sa_handler = signa_ctrlc_over;
    sigemptyset(& (action.sa_mask));
    action.sa_flags = 0;
    sigaction(SIGINT, & action, NULL);

 // get a pointer to the function "printf"
    old_printf = dlsym(RTLD_NEXT, "printf");
    int res = (*old_printf)("%s", parg); // and we call the function with previous arguments

    free(parg);
    return res;
}
*/


// Question 12 : write printf with a signal handler

