typedef struct top_variance {
	TUPLE_5			tuples;
	double			value;
	int				match;
	struct top_variance	*pre_value;
	struct top_variance	*next_value;
} TOP_VARIANCE;

extern TOP_VARIANCE *find_variance( KEY_VALUE *key_list, double *error_table, int *a, int *b, int *c, int *d, int number_key, double threshold );

extern TOP_VARIANCE *create_variance_node( TUPLE_5 *tuples, double value );

extern void *add_variance_node( TOP_VARIANCE *list, TUPLE_5 *tuples, double value );

extern void PRINT_variance_list( TOP_VARIANCE *list );

extern void FREE_variance_list( TOP_VARIANCE *list );

extern void FPRINT_variance_list( TOP_VARIANCE *list, char *name_buf, int start_time, int start_time_ns, int interval, double threshold );
