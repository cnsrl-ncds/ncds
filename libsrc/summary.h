extern void print_interval_info( int start_time, int start_time_ns, int interval );

extern void Fprint_interval_info( FILE *file_Ptr, int start_time, int start_time_ns, int interval );

extern void print_uint64_table( char *table_name, uint64_t *table );

extern void Fprint_uint64_table( FILE *file_Ptr, char *table_name, uint64_t *table );

extern void print_double_table( char *table_name, double *table );

extern void Fprint_uint64_table( FILE *file_Ptr, char *table_name, uint64_t *table );

extern void Fprint_false_determine( TOP_VARIANCE *variance, TOP_VARIANCE *variance_2, char *name_buf, int time, int time_ns, int interval, double TA );
