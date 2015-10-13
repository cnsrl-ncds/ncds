extern uint64_t query( uint32_t key, uint64_t *sketch_table, int *a, int *b, int *c, int *d, int number_key );

extern void forecast_error_sketch( uint64_t *sketch_table, double *forecast_table, double *error_table );

extern double find_threshold( double *error_table );
