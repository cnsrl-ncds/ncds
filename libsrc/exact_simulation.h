typedef struct exact_value {
	TUPLE_5			tuples;
	uint64_t			*length_value;
	double			forecast;
	double			error;
	struct exact_value	*next_key;
} EXACT_VALUE;

extern EXACT_VALUE *exact_save_key( EXACT_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len, int interval );

extern EXACT_VALUE *exact_create_node( TUPLE_5 *tuple5, uint32_t orig_len, int interval );

extern void exact_add_node( EXACT_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len, int interval );

extern void exact_PRINT_list( EXACT_VALUE *list, int interval );

//extern void FREE_list( KEY_VALUE *list );

//extern int count_key( KEY_VALUE *list );

extern void exact_error( EXACT_VALUE *list, int interval );

extern TOP_VARIANCE *exact_variance_list( EXACT_VALUE *exact_list, int interval, double threshold );

extern void exact_list_clr( EXACT_VALUE *list, int interval );
