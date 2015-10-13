#include "kary.h"
#include "common.h"

void
error( char *function, char *input ){
	int i;

	for( i = 0; i < 80;i++ ){
		printf("*");
	}
	putchar('\n');
	
	perror(function);
	printf("%s\n", input);

	for( i = 0; i < 80;i++ ){
		printf("*");
	}
	putchar('\n');
}
