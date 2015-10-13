#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <arpa/inet.h>

#include "common.h"
#include "pkt_setting.h"

/***** Parameter Setting ******************/

//#define INTERVAL	60
//#define WINDOWS	3
#define WINDOWS_2	2
//#define HASH		5
//#define KEY_bit	15
#define POWER		61	// Prime = 2^POWER - 1 

//#define T			0.01	// Threshold parameter

#define TOP_N		100
//#define DISCOUNT		1.0 //discount threshold fow ww1

/*******************
#these settings will dominate
#ie, if we want to use compiler-time  -D to set the #define parameters
#we need to comment these setting out!
***********************/



typedef struct tuples {
	uint32_t	so_IP;
	uint32_t	de_IP;
	uint16_t	so_port;
	uint16_t	de_port;
	uint8_t	protocol;
} TUPLE_5;
