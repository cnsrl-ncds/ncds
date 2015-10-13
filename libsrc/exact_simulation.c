#include "kary.h"
#include "save_key.h"
#include "find_top_variance.h"
#include "exact_simulation.h"


EXACT_VALUE
*exact_save_key( EXACT_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len, int interval ){
	if( list == NULL ){
		list = exact_create_node( tuple5, orig_len, interval );
	}
	else {
		exact_add_node( list, tuple5, orig_len, interval );
	}
	return list;
}

EXACT_VALUE
*exact_create_node( TUPLE_5 *tuple5, uint32_t orig_len, int interval ){
	int i;
	EXACT_VALUE *new_node = (EXACT_VALUE*)malloc(sizeof(EXACT_VALUE));

	new_node->tuples.so_IP	 = tuple5->so_IP;
	new_node->tuples.de_IP	 = tuple5->de_IP;
	new_node->tuples.so_port	 = tuple5->so_port;
	new_node->tuples.de_port	 = tuple5->de_port;
	new_node->tuples.protocol = tuple5->protocol;

	new_node->length_value = (uint64_t*)malloc( (WINDOWS+1) * sizeof( uint64_t ) );
	for( i = 0; i < (WINDOWS+1); i++ ){
		new_node->length_value[i] = 0;
	}

	new_node->length_value[interval] = orig_len;
	new_node->forecast = 0;

	new_node->next_key = NULL;

	return new_node;
}

void
exact_add_node( EXACT_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len, int interval ){

	while( list != NULL ){
		if( list->tuples.so_IP == tuple5->so_IP ){
			list->length_value[interval] += orig_len;
			return;
		}
		else {
			if( list->next_key == NULL ){
				EXACT_VALUE *new_node = exact_create_node( tuple5, orig_len, interval );
				list->next_key = new_node;
				return;
			}
			else {
				list = list->next_key;
			}
		}
	}
}
/*
void
FREE_list( KEY_VALUE *list ){
	if( list->next_key != NULL ){
		FREE_list(list->next_key);
	}
	free(list);
}
*/
void
exact_PRINT_list( EXACT_VALUE *list, int interval ){
	char abuf[INET_ADDRSTRLEN];
int i;
	while( list != NULL ){
//		printf("%s\t Total length=%ld\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->length_value[interval] );
printf("%s\t Total length=%ld\t", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->length_value[0] );
for(i=1;i<(WINDOWS+1);i++){
printf("%ld\t", list->length_value[i] );
}
	printf("\n");
		printf("%lf\t", list->forecast );
		printf("%lf\n", list->error );
		list = list->next_key;
	}
}
/*
int
count_key( KEY_VALUE *list ){
	int counter = 0;
	while( list != NULL ){
		counter += 1;
		list = list->next_key;
	}
	return counter;
}*/

void
exact_error( EXACT_VALUE *list, int interval ){
	int interval_counter;

	while( list != NULL ){
		/* Moving Average */
		list->forecast = 0;
		for( interval_counter = 0; interval_counter < interval; interval_counter++ ){
			list->forecast += ((double) list->length_value[interval_counter] );
		}
		for( interval_counter = (interval+1); interval_counter < (WINDOWS+1); interval_counter++ ){
			list->forecast += ((double) list->length_value[interval_counter] );
		}
		list->forecast = ( list->forecast / (double)(WINDOWS) );
		list->error = ((double)list->length_value[interval]) - list->forecast;

		list = list->next_key;
	}
}

TOP_VARIANCE
*exact_variance_list( EXACT_VALUE *exact_list, int interval, double threshold ){
	double	variance;
	double	sum = 0;
	uint64_t	KEY = 0;
//	KEY = (1 << 12);

	EXACT_VALUE	*temp_list = exact_list;
	TOP_VARIANCE	*variance_list = NULL;

	while( temp_list != NULL ){
		if( temp_list->length_value[interval] != 0 ){
			sum += temp_list->error;
			KEY++;
		}
		temp_list = temp_list->next_key;
	}

	while( exact_list != NULL ){
		if( exact_list->length_value[interval] != 0 ){
			variance = (( exact_list->error ) - ( sum / KEY )) * KEY / ( KEY - 1 );
			if( variance >= threshold ){
				if( variance_list == NULL ){
					variance_list = create_variance_node( &exact_list->tuples, variance );
				}
				else {
					variance_list = add_variance_node( variance_list, &exact_list->tuples, variance );
				}
			}				
		}
		exact_list = exact_list->next_key;
	}

	return variance_list;
}

void
exact_list_clr( EXACT_VALUE *list, int interval ){
	while( list != NULL ){
		list->length_value[interval]=0;
		list = list->next_key;
	}
}
