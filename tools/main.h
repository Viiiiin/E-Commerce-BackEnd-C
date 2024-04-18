#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <gsl/gsl_fit.h>
#include <cassert>
#include <cerrno>
#include "global.h"


#define DEBUG 1000

#define HORIZON 10  // TICKS




void log2db(char* message,int id, Con2DB db,char* domainType, char* function);
int msleep(long msec);
int micro_sleep(long usec);
int long get_nanos(void);
long int get_day_nanos(char* buf);
long int nanos2day(char* buf, long int nanosec);



using namespace std;

#endif
