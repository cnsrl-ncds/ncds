#include "../libsrc/kary.h"
#include "../libsrc/test.h"
#include "../libsrc/sketch.h"
#include "../libsrc/forecast.h"
#include "../libsrc/save_key.h"
#include "../libsrc/find_top.h"
#include "../libsrc/pkt_process.h"
#include "../libsrc/pcap_process.h"
#include "../libsrc/change_detection.h"
#include "../libsrc/find_top_variance.h"
#include "../libsrc/summary.h"
#include "../libsrc/exact_simulation.h"

int
main(int n_arg, char *arg_value[]) {
	int i;
	int trace_number;
	int trace_set = 0;
	int file_counter;
	double j, interval_jump;

	char *trace_name = malloc(20*sizeof(char));

	PCAP_HDR		pcap_hdr;
	PCAP_PKT_HDR	pcap_pkt_header;

	TUPLE_5		tuple5;

	uint32_t	temp;
	uint32_t	interval_time = 0;
	uint32_t	interval_time_usec = 0;

	uint32_t *interval = malloc( sizeof(uint32_t) );
	uint32_t	*work_interval = malloc( sizeof(uint32_t) );
	uint32_t	*work_interval_2pass = malloc( sizeof(uint32_t) );

	*interval = 1;
	*work_interval = 0;

	KEY_VALUE		*key_list = NULL;
	TOP_VALUE		*top_list = NULL;
	EXACT_VALUE	*exact_list = NULL;
	TOP_VARIANCE	*top_variance = NULL;
	TOP_VARIANCE	*top_variance_2 = NULL;
	TOP_VARIANCE	*top_variance_3_1 = NULL;		/* Windows Size == 1 */
	TOP_VARIANCE	*top_variance_3_2 = NULL;		/* Windows Size == 1 */
	TOP_VARIANCE	*top_variance_exact = NULL;

	/* Prepare random number for 4-Universal hash function */
	int *a = malloc( HASH * sizeof( uint64_t ) );
	int *b = malloc( HASH * sizeof( uint64_t ) );
	int *c = malloc( HASH * sizeof( uint64_t ) );
	int *d = malloc( HASH * sizeof( uint64_t ) );

	for( i = 0; i < HASH; i++ ){
		*( a + i )= rand();
		*( b + i )= rand();
		*( c + i )= rand();
		*( d + i )= rand();

		#ifdef Debug
			#if Debug >= 2
				printf("%2d - A[%d]=%d, B[%d]=%d, C[%d]=%d, D[%d]=%d\n", i, i, *(a+i), i, *(b+i), i, *(c+i), i, *(d+i));
			#endif
		#endif

	}

	/* Prepare Sketch array */
	uint64_t KEY;
	KEY = (1 << KEY_bit);
	uint64_t *sketch_table = calloc( (WINDOWS+1) * HASH * KEY, sizeof(uint64_t) );

	double *forecast_table = calloc( HASH * KEY, sizeof(double) );
	double *forecast_table_2 = calloc( HASH * KEY, sizeof(double) );
	double *forecast_table_MA1 = calloc( HASH * KEY, sizeof(double) );//yk for MA1
	double *error_table = calloc( HASH * KEY, sizeof(double) );
	double *error_table_2 = calloc( HASH * KEY, sizeof(double) );
	double *error_table_3 = calloc( HASH * KEY, sizeof(double) );	/* Windows Size == 1 */

	double threshold;
	double threshold_2;
	double threshold_3;	/* Windows Size == 1 */
	
	FILE *info_Ptr;
	FILE *trace_Ptr;
	info_Ptr = fopen( arg_value[1], "r" );
	fscanf( info_Ptr, "%d", &trace_number );

	for( file_counter = 0; file_counter < trace_number; file_counter++ ){
		fscanf( info_Ptr, "%s", trace_name );
		
		/* Check for trace type, 1 = pcap, 2 = tsh, -1 = this trace with different trace type than previous file */
		if( trace_set == 0 )
			trace_set = check_trace_type( trace_name );
		else if ( trace_set != check_trace_type( trace_name ) )
			trace_set = -1;
		/* End of trace type check */
		
		//print simulation papameters
		printf("Trace file:%s\n",trace_name);
		printf("1WWW\n");
		printf("Hash:%d, K_entry:%d, Threshold:%f, Interval:%d, Windows:%d \n",HASH, KEY, T, INTERVAL, WINDOWS);

		/* Pcap Packet Process */
		if( trace_set == 1 ) {
			trace_Ptr = fopen( trace_name, "r" );

			read_pcap_header( trace_Ptr, &pcap_hdr );
			while( fread( &pcap_pkt_header,sizeof(PCAP_PKT_HDR),1,trace_Ptr ) != 0 ) {

				/* Read PCAP pkt header, if it is swapped change it into identical */
				read_pcap_pkt_header( trace_Ptr, &pcap_hdr, &pcap_pkt_header );

				/* Timestamp configure */
				if( interval_time == 0 && interval_time_usec == 0 ){
					interval_time = pcap_pkt_header.ts_sec + INTERVAL;
					/* Attention */	interval_time_usec = pcap_pkt_header.ts_usec;
				}
				if( ( (pcap_pkt_header.ts_sec == interval_time) && (pcap_pkt_header.ts_usec >= interval_time_usec) ) \
				|| ( pcap_pkt_header.ts_sec > interval_time ) ){	

					if( *interval >= (WINDOWS+3) ) {
						variance_list_to_key_list( top_variance_2, key_list );

						if( (*interval)%2 == 0 )
							variance_list_to_key_list( top_variance_3_1, key_list );
						else
							variance_list_to_key_list( top_variance_3_2, key_list );


						top_variance = find_variance( key_list, error_table, a, b, c, d, KEY, threshold );
						FPRINT_variance_list( top_variance, "TOP_Variance",\
										 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold );

						Fprint_false_determine( top_variance_exact, top_variance, "Exact_vs_Direct",\
										 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold );

						if( top_variance != NULL ){
							FREE_variance_list( top_variance );
							top_variance=NULL;
						}
						if( top_variance_exact != NULL ){
							FREE_variance_list( top_variance_exact );
							top_variance_exact=NULL;
						}
						if( top_variance_2 != NULL ){
							FREE_variance_list( top_variance_2 );
							top_variance_2=NULL;
						}
						if( top_variance_3_1 != NULL && ((*interval)%2 == 0 ) ){
							FREE_variance_list( top_variance_3_1 );
							top_variance_3_1=NULL;
						}
						if( top_variance_3_2 != NULL && ((*interval)%2 == 1 ) ){
							FREE_variance_list( top_variance_3_2 );
							top_variance_3_2=NULL;
						}
					}

					if( *interval >= (WINDOWS+2) ) {
						MA(sketch_table, forecast_table, *work_interval);
						forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), forecast_table, error_table );
						threshold = find_threshold( error_table );


						exact_error( exact_list, *work_interval );

						top_variance_exact = exact_variance_list( exact_list, *work_interval, threshold );

						#ifdef Debug
							#if Debug >= 1
								printf("threshold = %lf\n", threshold );
							#endif
						#endif

						FPRINT_variance_list( top_variance_exact, "TOP_Variance_Exact", (interval_time-INTERVAL), interval_time_usec, *interval, threshold );

					}


					if( *interval >= (WINDOWS_2+3) ) {
						top_variance_2 = find_variance( key_list, error_table_2, a, b, c, d, KEY, threshold_2 );
						FPRINT_variance_list( top_variance_2, "TOP_Variance_2W",\
										 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold_2 );

					}


					if( *interval >= (WINDOWS_2+2) ) {
						MA2(sketch_table, forecast_table_2, *work_interval);
						forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), forecast_table_2, error_table_2 );
						threshold_2 = find_threshold( error_table_2 );


						#ifdef Debug
							#if Debug >= 1
								printf("threshold_2 = %lf\n", threshold_2 );
							#endif
						#endif
					}

					if( (*interval >= 3) && ((*interval)%2 == 1) ) {
					 MA1(sketch_table, forecast_table_MA1);//type confert from int to double for sketch table with MA=1
						if( (*work_interval) == 0 )
							forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), (forecast_table_MA1 + WINDOWS*HASH*KEY), error_table_3 );
						else
							forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), (forecast_table_MA1 + ((*work_interval)-1)*HASH*KEY), error_table_3 );

						threshold_3 = find_threshold( error_table_3 );


						#ifdef Debug
							#if Debug >= 1
								printf("threshold_3 = %lf\n", threshold_3 );
							#endif
						#endif						
						top_variance_3_1 = find_variance( key_list, error_table_3, a, b, c, d, KEY, threshold_3 );
						FPRINT_variance_list( top_variance_3_1, "TOP_Variance_1W",\
										 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold_3 );

					}

					if( (*interval >= 3) && ((*interval)%2 == 0) ) {
					MA1(sketch_table, forecast_table_MA1);//type confert from int to double for sketch table with MA=1
						if( (*work_interval) == 0 )
							forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), (forecast_table_MA1 + WINDOWS*HASH*KEY), error_table_3 );
						else
							forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), (forecast_table_MA1 + ((*work_interval)-1)*HASH*KEY), error_table_3 );

						threshold_3 = find_threshold( error_table_3 );


						#ifdef Debug
							#if Debug >= 1
								printf("threshold_3 = %lf\n", threshold_3 );
							#endif
						#endif						
						top_variance_3_2 = find_variance( key_list, error_table_3, a, b, c, d, KEY, threshold_3 );
						FPRINT_variance_list( top_variance_3_2, "TOP_Variance_1W",\
										 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold_3 );

					}


					#ifdef Debug
						#if Debug >= 3
							PRINT_list( key_list );
						#endif
					#endif

					/* Jump more than 1 interval */
					if( (pcap_pkt_header.ts_sec - interval_time) > INTERVAL ){
						interval_jump = ( (pcap_pkt_header.ts_sec - interval_time) / INTERVAL );
						if( (pcap_pkt_header.ts_usec < interval_time_usec) ){
							interval_jump--;
						}
						if( top_variance != NULL ){
							FREE_variance_list( top_variance );
							top_variance=NULL;
						}
						if( top_variance_2 != NULL ){
							FREE_variance_list( top_variance_2 );
							top_variance_2=NULL;
						}
					}
					else {
						interval_jump = 0;
					}
					/****************************/

					for( j = 0; j <= interval_jump; j++ ){
						#ifdef Debug
							#if Debug >= 1
								print_interval_info( interval_time, interval_time_usec, *interval );
								printf("Total Flow = %d\n", count_key( key_list ) );
							#endif
						#endif
						interval_time += INTERVAL;
						*interval = *interval + 1;
						*work_interval = *work_interval +1;
						*work_interval_2pass = *work_interval_2pass + 1;
						if( *work_interval == (WINDOWS+1) ){
							*work_interval = 0;
						}
						if( *work_interval_2pass == (WINDOWS+1) ){
							*work_interval_2pass = 0;
						}

						/* Initial next interval */
						for( i = 0; i < HASH*KEY; i++ ){
							*(sketch_table + (*work_interval)*HASH*KEY + i ) = 0;
						}
						if( key_list != NULL ){
							FREE_list( key_list );
							key_list=NULL;
						}
						exact_list_clr( exact_list, *work_interval );
					}
				}

				/* Packet process */
				read_pcap( trace_Ptr, &pcap_pkt_header, &tuple5 );

				/* Update */
				for(  i = 0; i < HASH; i++  ){
					temp = FourWise(tuple5.so_IP, *( a + i ), *( b + i ), *( c + i ), *( d + i ));
					temp = ( temp & ( 0xFFFFFFFF >> (32-KEY_bit)));
					*((sketch_table + (*work_interval)*HASH*KEY) + i*KEY + temp )+= pcap_pkt_header.orig_len;
				}


				/* Save Key */
				key_list = save_key( key_list, &tuple5, pcap_pkt_header.orig_len );

				exact_list = exact_save_key( exact_list, &tuple5, pcap_pkt_header.orig_len, *work_interval );

			}

			#ifdef Debug
				#if Debug >= 1
					print_interval_info( interval_time, interval_time_usec, *interval );
					printf("Total Flow = %d\n", count_key( key_list ) );
				#endif
			#endif

			if( *interval >= (WINDOWS+3) ) {
				variance_list_to_key_list( top_variance_2, key_list );

				top_variance = find_variance( key_list, error_table, a, b, c, d, KEY, threshold );
				FPRINT_variance_list( top_variance, "TOP_Variance",\
								 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold );

				Fprint_false_determine( top_variance_exact, top_variance, "Exact_vs_Direct",\
								 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold );

				if( top_variance != NULL ){
					FREE_variance_list( top_variance );
					top_variance=NULL;
				}
				if( top_variance_exact != NULL ){
					FREE_variance_list( top_variance_exact );
					top_variance_exact=NULL;
				}
			}

			if( *interval >= (WINDOWS+2) ) {
				MA(sketch_table, forecast_table, *work_interval);
				forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), forecast_table, error_table );
				threshold = find_threshold( error_table );
				#ifdef Debug
					#if Debug >= 1
						printf("threshold = %lf\n", threshold );
					#endif
				#endif

				exact_error( exact_list, *work_interval );

				FPRINT_variance_list( top_variance, "TOP_Variance", (interval_time-INTERVAL), interval_time_usec, *interval, threshold );
				if( top_variance != NULL ){
					FREE_variance_list( top_variance );
					top_variance=NULL;
				}
			}


			if( *interval >= (WINDOWS_2+3) ) {
				top_variance_2 = find_variance( key_list, error_table_2, a, b, c, d, KEY, threshold_2 );
				FPRINT_variance_list( top_variance_2, "TOP_Variance_2W",\
								 (interval_time-INTERVAL*2), interval_time_usec, (*interval-1), threshold_2 );

			}

			if( *interval >= (WINDOWS_2+2) ) {
				MA2(sketch_table, forecast_table_2, *work_interval);
				forecast_error_sketch( (sketch_table + (*work_interval)*HASH*KEY), forecast_table_2, error_table_2 );
				threshold_2 = find_threshold( error_table_2 );
				#ifdef Debug
					#if Debug >= 1
						printf("threshold_2 = %lf\n", threshold_2 );
					#endif
				#endif

			}

			#ifdef Debug
				#if Debug >= 3
					PRINT_list( key_list );
				#endif
			#endif


			if( *work_interval_2pass != -1){
				top_list = find_top( key_list, (sketch_table + (*work_interval_2pass)*HASH*KEY), a, b, c, d, KEY );
				FPRINT_top_list( top_list, "TOP_Value", (interval_time-INTERVAL*2), interval_time_usec, (*interval-1) );
			}

			fclose( trace_Ptr );
		}

		/* TSH Packet Process */
		else if( trace_set == 2 ){ 
			
		}

		/* Trace read error, stop program */
		else if( trace_set == -1 ){
			error_1(2);
			break;
		}
	}
	fclose( info_Ptr );

	free(sketch_table);
	return(0);
}
