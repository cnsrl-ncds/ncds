typedef struct top_value {
	TUPLE_5			tuples;
	uint64_t			value;
	short			match;
	struct top_value	*pre_value;
	struct top_value	*next_value;
} TOP_VALUE;

extern TOP_VALUE *find_top( KEY_VALUE *key_list, uint64_t *sketch_table, int *a, int *b, int *c, int *d, int number_key );

extern TOP_VALUE *create_top_node( TUPLE_5 *tuples, uint64_t value );

extern void *add_top_node( TOP_VALUE *list, TUPLE_5 *tuples, uint64_t value );

extern void PRINT_top_list( TOP_VALUE *list );

extern void FREE_top_list( TOP_VALUE *list );

extern void FPRINT_top_list( TOP_VALUE *list, char *name_buf, int start_time, int start_time_ns, int interval);
