#include "kary.h"
#include "save_key.h"
#include "find_top_variance.h"


KEY_VALUE
*save_key( KEY_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len ){
	if( list == NULL ){
		list = create_node( tuple5, orig_len );
	}
	else {
		add_node( list, tuple5, orig_len );
	}
	return list;
}

KEY_VALUE
*create_node( TUPLE_5 *tuple5, uint32_t orig_len ){
	KEY_VALUE *new_node = (KEY_VALUE*)malloc(sizeof(KEY_VALUE));

	new_node->tuples.so_IP	 = tuple5->so_IP;
	new_node->tuples.de_IP	 = tuple5->de_IP;
	new_node->tuples.so_port	 = tuple5->so_port;
	new_node->tuples.de_port	 = tuple5->de_port;
	new_node->tuples.protocol = tuple5->protocol;

	new_node->length_value    = orig_len;

	new_node->F2 = 0;
	new_node->next_key = NULL;

	return new_node;
}

void
add_node( KEY_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len ){

	while( list != NULL ){
		if( list->tuples.so_IP == tuple5->so_IP ){
			list->length_value += orig_len;
			return;
		}
		else {
			if( list->next_key == NULL ){
				KEY_VALUE *new_node = create_node( tuple5, orig_len );
				list->next_key = new_node;
				return;
			}
			else {
				list = list->next_key;
			}
		}
	}
}

void
FREE_list( KEY_VALUE *list ){
	if( list->next_key != NULL ){
		FREE_list(list->next_key);
	}
	free(list);
}

void
PRINT_list( KEY_VALUE *list ){
	char abuf[INET_ADDRSTRLEN];

	while( list != NULL ){
		printf("%s\t Total length=%ld\n", inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->length_value );
		list = list->next_key;
	}
}

int
count_key( KEY_VALUE *list ){
	int counter = 0;
	while( list != NULL ){
		counter += 1;
		list = list->next_key;
	}
	return counter;
}


void
variance_list_to_key_list( TOP_VARIANCE *variance_list, KEY_VALUE *key_list ){
	while( variance_list != NULL ){
		add_node( key_list, &variance_list->tuples, 0 );
		variance_list = variance_list->next_value;
	}
}
