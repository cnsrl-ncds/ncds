#include "kary.h"
#include "save_key.h"
#include "find_top.h"
#include "change_detection.h"
#include "find_top_variance.h"
#include "summary.h"

TOP_VALUE
*find_top( KEY_VALUE *key_list, uint64_t *sketch_table, int *a, int *b, int *c, int *d, int number_key ){
	TOP_VALUE	*top_list = NULL;

	while( key_list != NULL ){
		if( top_list == NULL ){
			top_list = create_top_node( &key_list->tuples, query( key_list->tuples.so_IP, sketch_table, a, b, c, d, number_key ) );
		}
		else {
			top_list = add_top_node( top_list, &key_list->tuples, query( key_list->tuples.so_IP, sketch_table, a, b, c, d, number_key ) );
		}
		key_list = key_list->next_key;
	}

	return top_list;
}

TOP_VALUE
*create_top_node( TUPLE_5 *tuples, uint64_t value ){
	TOP_VALUE *new_node = (TOP_VALUE*)malloc(sizeof(TOP_VALUE));

	new_node->tuples.so_IP	 = tuples->so_IP;
	new_node->tuples.de_IP	 = tuples->de_IP;
	new_node->tuples.so_port	 = tuples->so_port;
	new_node->tuples.de_port	 = tuples->de_port;
	new_node->tuples.protocol = tuples->protocol;

	new_node->value = value;
	new_node->match = 0;
	new_node->pre_value  = NULL;
	new_node->next_value = NULL;

	return new_node;
}

void
*add_top_node( TOP_VALUE *list, TUPLE_5 *tuples, uint64_t value ){
	TOP_VALUE *temp = list;
	TOP_VALUE *new_node = create_top_node( tuples, value );

	char		abuf[INET_ADDRSTRLEN];
	while( list != NULL ){
		if( list->value  < value ){
			if( list->pre_value == NULL ){
				new_node->pre_value = NULL;
				new_node->next_value = list;
				list->pre_value = new_node;
				return new_node;
			}
			else {
				list->pre_value->next_value = new_node;
				new_node->pre_value = list->pre_value;
				new_node->next_value = list;
				list->pre_value = new_node;
				return temp;
			}
		}
		else if( (list->value == value) && \
				( atoi(inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN)) > \
				atoi(inet_ntop(AF_INET, &tuples->so_IP, abuf, INET_ADDRSTRLEN)) ) ){
			if( list->pre_value == NULL ){
				new_node->pre_value = NULL;
				new_node->next_value = list;
				list->pre_value = new_node;
				return new_node;
			}
			else {
				list->pre_value->next_value = new_node;
				new_node->pre_value = list->pre_value;
				new_node->next_value = list;
				list->pre_value = new_node;
				return temp;
			}
		}
		else if( list->next_value == NULL ){
			list->next_value = new_node;
			new_node->pre_value = list;
			return temp;
		}
		else {
			list = list->next_value;
		}
	}
	return(NULL);
}

void
PRINT_top_list( TOP_VALUE *list ){
	int	number = 1;
	char abuf[INET_ADDRSTRLEN];

	int		temp_number = 0;
	uint64_t	temp_value = 0;

	while( list != NULL && temp_number < TOP_N ){
		if( temp_value == list->value ){
			printf("     %s\tvalue = %ld\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
		}
		else {
			printf("%3d. %s\tvalue = %ld\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
			temp_value = list->value;
			temp_number = number;
		}
		list = list->next_value;
		number++;
	}
}

void
FREE_top_list( TOP_VALUE *list ){
	if( list->next_value != NULL ){
		FREE_top_list(list->next_value);
	}
	free(list);
}

void
FPRINT_top_list( TOP_VALUE *list, char *name_buf, int start_time, int start_time_ns, int interval){
	int		number = 1;

	int		temp_number = 0;
	uint64_t	temp_value = 0;

	char abuf[INET_ADDRSTRLEN];

	char file_buf[55];
	char file_buf2[10];

	sprintf(file_buf,"%s/%s_", name_buf, name_buf);
	sprintf(file_buf2,"%d.txt", interval);
	strcat(file_buf, file_buf2);

	FILE *output_Ptr;
	output_Ptr = fopen( file_buf, "w" );

	Fprint_interval_info( output_Ptr, start_time, start_time_ns, interval );

	while( list != NULL && temp_number < TOP_N ){
		if( temp_value == list->value ){
			fprintf(output_Ptr, "     %s\tvalue = %ld\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
		}
		else {
			fprintf(output_Ptr, "%3d. %s\tvalue = %ld\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
			temp_value = list->value;
			temp_number = number;
		}
		list = list->next_value;
		number++;
	}
	fclose(output_Ptr);
}
