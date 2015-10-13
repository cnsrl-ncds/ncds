#include "kary.h"
#include "sketch.h"

uint64_t
MultAddPrime( uint32_t x, uint64_t a, uint64_t b ){
	uint64_t a0, a1, c0, c1, c;
	uint64_t prime;

	prime = (1L << POWER) - 1;

	a0 = (a & 0xFFFFFFFF) * x;
	a1 = (a >> 32) * x;
	c0 = a0 + (a1 << 32);
	c1 = (a0 >> 32) + a1;
	c  = (c0 & prime) + (c1 >> 29) + b;
	return c;
}

uint64_t
FourWise( uint32_t x, uint64_t a, uint64_t b, uint64_t c, uint64_t d ){
	uint64_t h;
	uint64_t prime;

	prime = (1L << POWER) - 1;

	h = MultAddPrime(x, MultAddPrime(x, MultAddPrime(x, a, b), c), d);
	h = ( h & prime ) + ( h >> 61 );
	if( h >= prime ){
		h -= prime;
	}
	return h;
}

double
SUM( double *table ){
	double sum;
	int hash_counter, key_counter;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		sum = 0;
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			sum += *( table + hash_counter*KEY + key_counter );
		}
	}
	return(sum);
}

void
ESTIMATE( double *sketch_table, double *Estimate_table, uint32_t input_key, int *a, int *b, int *c, int *d ){
	double	sum;
	uint64_t	temp;

	uint64_t KEY;
	KEY = (1 << KEY_bit);

	int		i;
	double	estimate_hi;

	sum = SUM(sketch_table);
	for( i = 0; i < HASH; i++  ){
		temp = FourWise(input_key, *( a + i ), *( b + i ), *( c + i ), *( d + i ));
		temp = ( temp & ( 0xFFFFFFFF >> (32-KEY_bit)));
		estimate_hi = *(sketch_table + i*KEY + temp )-( sum / KEY );
		*(Estimate_table + i) = estimate_hi * KEY / ( KEY - 1 );
	}
}

double
ESTIMATE_est( double *sketch_table, uint32_t input_key, int *a, int *b, int *c, int *d ){

	uint64_t KEY;
	KEY = (1 << KEY_bit);

	double *Estimate_table = calloc( HASH, sizeof(double) );
	ESTIMATE( sketch_table, Estimate_table, input_key, a, b, c, d );

	/* Find out estimate_est */
	double sort_temp;
	double *tmp_array = malloc( HASH * sizeof(double) );

	int hash_counter, i1, j1;
	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		*(tmp_array + hash_counter) = *(Estimate_table + hash_counter);
	}
	free(Estimate_table);
	for(i1 = 0; i1 < (HASH-1); i1++){
		for(j1 = 0; j1 < (HASH-i1-1); j1++){
			if( *(tmp_array + j1) < *(tmp_array + j1 + 1)){
				sort_temp = *(tmp_array + j1);
				*(tmp_array + j1) = *(tmp_array + j1 + 1);
				*(tmp_array + j1 + 1) = sort_temp;
			}
		}
	}
	return(*(tmp_array + HASH/2));
}

void
ESTIMATE_F2( double *sketch_table, double *F2_table ){
	double	sum, temp, sum_of_square, f2_hi;

	uint64_t KEY;
	KEY = (1 << KEY_bit);

	sum = SUM(sketch_table);
	int hash_counter, key_counter;
	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		sum_of_square = 0;
		for( key_counter = 0; key_counter < KEY; key_counter++ ){
			temp = (*(sketch_table+hash_counter*KEY+key_counter));
			sum_of_square += temp*temp;
		}
	f2_hi = ( (double)KEY * sum_of_square - sum * sum ) / ( KEY - 1 );
	*(F2_table + hash_counter) = f2_hi;
	}
}

double
ESTIMATE_F2_est( double *sketch_table ){
	double sort_temp;

	double *tmp_array = malloc( HASH * sizeof(double) );
	double *F2_table = calloc( HASH, sizeof(double) );

	ESTIMATE_F2( sketch_table, F2_table );

	int hash_counter, i1, j1;
	for( hash_counter = 0; hash_counter < HASH; hash_counter++ ){
		*(tmp_array + hash_counter) = *(F2_table + hash_counter);
	}
	free(F2_table);
	for(i1 = 0; i1 < (HASH-1); i1++){
		for(j1 = 0; j1 < (HASH-i1-1); j1++){
			if( *(tmp_array + j1) < *(tmp_array + j1 + 1)){
				sort_temp = *(tmp_array + j1);
				*(tmp_array + j1) = *(tmp_array + j1 + 1);
				*(tmp_array + j1 + 1) = sort_temp;
			}
		}
	}
	return(*(tmp_array + HASH/2));
}
