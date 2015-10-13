#include "kary.h"
#include "sketch.h"
#include "save_key.h"
#include "find_top_variance.h"
#include "change_detection.h"
#include "summary.h"

TOP_VARIANCE
*find_variance( KEY_VALUE *key_list, double *error_table, int *a, int *b, int *c, int *d, int number_key, double threshold ){
 	double		query_variance;
	TOP_VARIANCE	*top_list = NULL;

	while( key_list != NULL ){
		query_variance = ESTIMATE_est( error_table, key_list->tuples.so_IP, a, b, c, d);
		if( query_variance >= threshold ){
			if( top_list == NULL ){
				top_list = create_variance_node( &key_list->tuples, query_variance );
			}
			else {
				top_list = add_variance_node( top_list, &key_list->tuples, query_variance );
			}
		}
		key_list = key_list->next_key;
	}

	return top_list;
}

TOP_VARIANCE
*create_variance_node( TUPLE_5 *tuples, double value ){
	TOP_VARIANCE *new_node = (TOP_VARIANCE*)malloc(sizeof(TOP_VARIANCE));

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
*add_variance_node( TOP_VARIANCE *list, TUPLE_5 *tuples, double value ){
	TOP_VARIANCE *temp = list;
	TOP_VARIANCE *new_node = create_variance_node( tuples, value );

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
PRINT_variance_list( TOP_VARIANCE *list ){
	int	number = 1;
	char abuf[INET_ADDRSTRLEN];

	int		temp_number = 0;
	double	temp_value = 0;

	while( list != NULL ){
		if( temp_value == list->value ){
			printf("     %s\tvalue = %f\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
		}
		else {
			printf("%3d. %s\tvalue = %f\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
			temp_value = list->value;
			temp_number = number;
		}
		list = list->next_value;
		number++;
	}
}

void
FREE_variance_list( TOP_VARIANCE *list ){
	if( list->next_value != NULL ){
		FREE_variance_list(list->next_value);
	}
	free(list);
}

void
FPRINT_variance_list( TOP_VARIANCE *list, char *name_buf, int start_time, int start_time_ns, int interval, double threshold ){
	int		number = 1;

	int		temp_number = 0;
	double	temp_value = 0;

	char abuf[INET_ADDRSTRLEN];

	char file_buf[60];
	char file_buf2[10];

	sprintf(file_buf,"%s/%s_", name_buf, name_buf);
	sprintf(file_buf2,"%d.txt", interval);
	strcat(file_buf, file_buf2);

	FILE *output_Ptr;
	output_Ptr = fopen( file_buf, "w" );

	Fprint_interval_info( output_Ptr, start_time + INTERVAL, start_time_ns, interval );
	fprintf(output_Ptr, "threshold = %lf\n", threshold);

	while( list != NULL ){
		if( temp_value == list->value ){
			fprintf(output_Ptr, "     %s\tvalue = %f\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
		}
		else {
			fprintf(output_Ptr, "%3d. %s\tvalue = %f\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
			temp_value = list->value;
			temp_number = number;
		}
		list = list->next_value;
		number++;
	}
	fclose(output_Ptr);
}
