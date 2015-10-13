#include <math.h>

#include "kary.h"
#include "test.h"
#include "sketch.h"
#include "save_key.h"
#include "change_detection.h"

uint64_t
query( uint32_t key, uint64_t *sketch_table, int *a, int *b, int *c, int *d, int number_key ){
	int i;
	uint32_t *temp = malloc( HASH * sizeof( uint32_t ) );
	uint32_t *value = malloc( HASH * sizeof( uint64_t ) );

	for(  i = 0; i < HASH; i++  ){
		*( temp + i ) = FourWise(key, *( a + i ), *( b + i ), *( c + i ), *( d + i ));
		*( temp + i ) = (*( temp + i ) & ( 0xFFFFFFFF >> (32-KEY_bit)));
		*( value + i ) = *( sketch_table + i*number_key + *( temp + i ) );
	}
	
	int query_value = *( value );
	for(  i = 1; i < HASH; i++  ){
		if( query_value > *( value + i ) ){
			query_value = *( value + i );
		}
	}
	free(temp);
	free(value);
	return query_value;
}

void
forecast_error_sketch( uint64_t *sketch_table, double *forecast_table, double *error_table ){
	int hash_counter, key_counter;

	uint64_t KEY;
	KEY = (1 << KEY_bit);

	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			*( error_table + hash_counter*KEY + key_counter ) = \
			(double)*( sketch_table + hash_counter*KEY + key_counter ) - *( forecast_table + hash_counter*KEY + key_counter );
		}
	}
}

double
find_threshold( double *error_table ){
	double estimate_f2, threshold;

	estimate_f2 = ESTIMATE_F2_est( error_table );
	threshold = T * sqrt( estimate_f2 );
	return(threshold);
}
