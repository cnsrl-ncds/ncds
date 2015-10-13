extern uint64_t MultAddPrime( uint32_t x, uint64_t a, uint64_t b );

extern uint64_t FourWise( uint32_t x, uint64_t a, uint64_t b, uint64_t c, uint64_t d );

extern double SUM( double *table );

extern void ESTIMATE( double *sketch_table, double *Estimate_table, uint32_t input_key, int *a, int *b, int *c, int *d );

extern double ESTIMATE_est( double *sketch_table, uint32_t input_key, int *a, int *b, int *c, int *d );

extern void ESTIMATE_F2( double *sketch_table, double *F2_table );

extern double ESTIMATE_F2_est( double *sketch_table );
