#include "kary.h"
#include "test.h"
#include "sketch.h"

void
print_query( int key, int *sketch_table, double *forecast_table, double *error_table, int *a, int *b, int *c, int *d ){
	int		*temp_sketch = calloc( HASH, sizeof(uint64_t) );
	double	*temp_forecast = calloc( HASH, sizeof(double) );
	double	*temp_query = calloc( HASH, sizeof(double) );
	double	*temp_variance = calloc( HASH, sizeof(double) );

	uint32_t	temp;

	uint64_t KEY;
	KEY = (1 << KEY_bit);

	int		sort_int;
	double	sort_double;

	int		ix, jx;
	double	sum;
	char		abuf[INET_ADDRSTRLEN];

	/* SUM calculation */
	for( ix = 0; ix < HASH; ix++ ){
		sum = 0;
		for( jx = 0; jx < KEY; jx++ ){
			sum += *( error_table + ix*KEY + jx );
		}
	}

	for(  ix = 0; ix < HASH; ix++  ){
		temp = FourWise( key, *( a + ix ), *( b + ix ), *( c + ix ), *( d + ix ));
		printf("%lu\n\n\n",FourWise( key, *( a + ix ), *( b + ix ), *( c + ix ), *( d + ix )));
		temp = (( temp ) << (32-KEY_bit)) >> (32-KEY_bit);
		*(temp_sketch + ix) = *(sketch_table + ix*KEY + temp );
		*(temp_forecast + ix) = *(forecast_table + ix*KEY + temp );
		*(temp_query + ix) = *(error_table + ix*KEY + temp );
		*(temp_variance + ix) = ( *(error_table + ix*KEY + temp )-( sum / KEY ) ) * KEY / ( KEY - 1 );
	}
	for(ix = 0; ix < (HASH-1); ix++){
		for(jx = 0; jx < (HASH-ix-1); jx++){
			if( *(temp_sketch + jx) < *(temp_sketch + jx + 1)){
				sort_int = *(temp_sketch + jx);
				*(temp_sketch + jx) = *(temp_sketch + jx + 1);
				*(temp_sketch + jx + 1) = sort_int;
			}
			if( *(temp_forecast + jx) < *(temp_forecast + jx + 1)){
				sort_double = *(temp_forecast + jx);
				*(temp_forecast + jx) = *(temp_forecast + jx + 1);
				*(temp_forecast + jx + 1) = sort_double;
			}
			if( *(temp_query + jx) < *(temp_query + jx + 1)){
				sort_double = *(temp_query + jx);
				*(temp_query + jx) = *(temp_query + jx + 1);
				*(temp_query + jx + 1) = sort_double;
			}
			if( *(temp_variance + jx) < *(temp_variance + jx + 1)){
				sort_double = *(temp_variance + jx);
				*(temp_variance + jx) = *(temp_variance + jx + 1);
				*(temp_variance + jx + 1) = sort_double;
			}
		}
	}
	printf("%s", inet_ntop(AF_INET, &key, abuf, INET_ADDRSTRLEN) );
	printf("\tquery_sketch = %d", *(temp_sketch + HASH/2) );
	printf("\t\tquery_forecast = %lf\n", *(temp_forecast + HASH/2) );
	printf("\t\tquery_error = %lf", *(temp_query + HASH/2) );
	printf("\tvariance = %lf\n\n", *(temp_variance + HASH/2) );
}
