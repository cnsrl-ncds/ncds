
typedef struct key_value {
	TUPLE_5			tuples;
	uint64_t			length_value;
	double			F2;
	struct key_value	*next_key;
} KEY_VALUE;

KEY_VALUE *save_key( KEY_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len );

extern KEY_VALUE *create_node( TUPLE_5 *tuple5, uint32_t orig_len );

extern void add_node( KEY_VALUE *list, TUPLE_5 *tuple5, uint32_t orig_len );

extern void PRINT_list( KEY_VALUE *list );

extern void FREE_list( KEY_VALUE *list );

extern int count_key( KEY_VALUE *list );

//extern void variance_list_to_key_list( TOP_VARIANCE *variance_list, KEY_VALUE *key_list );
