#ifndef CLIENT_H
#define CLIENT

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "socketutil.h"
#include "helper.h"

#define MAXBUF 1024 /*max line limit for message*/


void reader(void* sockFD);

#endif