#include "kary.h"
#include "save_key.h"
#include "find_top_variance.h"
#include "summary.h"

/* Print interval infomation include start time, interval number etc.. */
void
print_interval_info( int start_time, int start_time_ns, int interval ){

	char		buf[20];
	char		buf1[20];
	time_t	cal_time;
	struct tm	*ts;

	cal_time = start_time - INTERVAL;
	ts = localtime(&cal_time);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);

	cal_time = start_time;
	ts = localtime(&cal_time);
	strftime(buf1, sizeof(buf1), "%Y-%m-%d %H:%M:%S", ts);

	int i;
	putchar('\n');
	for( i = 0; i <= 60;i++ )
		printf("=");	
	putchar('\n');

	printf("Interval No.%4d\n", interval);
	
	if( start_time_ns == 0 ) {
		printf("Start from:%s.000000\n",buf);
		printf("Stop    at:%s.000000\n", buf1);
	}
	else if( start_time_ns < 10 ) {
		printf("Start from:%s.00000%d\n",buf, start_time_ns);
		printf("Stop    at:%s.00000%d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 100 ) {
		printf("Start from:%s.0000%2d\n",buf, start_time_ns);
		printf("Stop    at:%s.0000%2d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 1000 ) {
		printf("Start from:%s.000%3d\n",buf, start_time_ns);
		printf("Stop    at:%s.000%3d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 10000 ) {
		printf("Start from:%s.00%4d\n",buf, start_time_ns);
		printf("Stop    at:%s.00%4d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 100000 ) {
		printf("Start from:%s.0%5d\n",buf, start_time_ns);
		printf("Stop    at:%s.0%5d\n", buf1, start_time_ns);
	}
	else {
		printf("Start from:%s.%6d\n",buf, start_time_ns);
		printf("Stop    at:%s.%6d\n", buf1, start_time_ns);
	}

	printf("\nTotal Interval time: %d second\n", INTERVAL);

	for( i = 0; i <= 60;i++ )
		printf("=");	
	putchar('\n');
}

/* Print interval infomation to a file */
void
Fprint_interval_info( FILE *file_Ptr, int start_time, int start_time_ns, int interval ){

	char		buf[20];
	char		buf1[20];
	time_t	cal_time;
	struct tm	*ts;

	cal_time = start_time - INTERVAL;
	ts = localtime(&cal_time);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);

	cal_time = start_time;
	ts = localtime(&cal_time);
	strftime(buf1, sizeof(buf1), "%Y-%m-%d %H:%M:%S", ts);

	int i;
	for( i = 0; i <= 60;i++ )
		fprintf( file_Ptr, "=" );	
	fprintf( file_Ptr, "\n" );

	fprintf( file_Ptr, "Interval No.%4d\n", interval );

	if( start_time_ns == 0 ) {
		fprintf( file_Ptr, "Start from:%s.000000\n",buf);
		fprintf( file_Ptr, "Stop    at:%s.000000\n", buf1);
	}
	else if( start_time_ns < 10 ) {
		fprintf( file_Ptr, "Start from:%s.00000%d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.00000%d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 100 ) {
		fprintf( file_Ptr, "Start from:%s.0000%2d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.0000%2d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 1000 ) {
		fprintf( file_Ptr, "Start from:%s.000%3d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.000%3d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 10000 ) {
		fprintf( file_Ptr, "Start from:%s.00%4d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.00%4d\n", buf1, start_time_ns);
	}
	else if( start_time_ns < 100000 ) {
		fprintf( file_Ptr, "Start from:%s.0%5d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.0%5d\n", buf1, start_time_ns);
	}
	else {
		fprintf( file_Ptr, "Start from:%s.%6d\n",buf, start_time_ns);
		fprintf( file_Ptr, "Stop    at:%s.%6d\n", buf1, start_time_ns);
	}

	fprintf( file_Ptr, "\nTotal Interval time: %d second\n", INTERVAL );

	for( i = 0; i <= 60;i++ )
		fprintf( file_Ptr, "=" );	
	fprintf( file_Ptr, "\n" );
}

/* Print the table of data type uint64 */
void
print_uint64_table( char *table_name, uint64_t *table ){
	int i1, i2;

	uint64_t KEY;
	KEY = (1L << KEY_bit);
	
	printf("%s\n\n", table_name);
	for( i1 = 0; i1 < HASH; i1++ ){
		for( i2 = 0; i2 < KEY; i2++ ){
			printf( "%lu\t", *(table + i1*KEY + i2 ) );
		}
		printf("\n");
	}
}

void
Fprint_uint64_table( FILE *file_Ptr, char *table_name, uint64_t *table ){
	int i1, i2;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	fprintf( file_Ptr, "%s\n\n", table_name );
	for( i1 = 0; i1 < HASH; i1++ ){
		for( i2 = 0; i2 < KEY; i2++ ){
			fprintf( file_Ptr, "%lu\t", *(table + i1*KEY + i2 ) );
		}
		fprintf( file_Ptr, "\n" );
	}
}

/* Print the table of data type double */
void
print_double_table( char *table_name, double *table ){
	int i1, i2;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	printf("%s\n\n", table_name);
	for( i1 = 0; i1 < HASH; i1++ ){
		for( i2 = 0; i2 < KEY; i2++ ){
			printf( "%lf\t", *(table + i1*KEY + i2 ) );
		}
		printf("\n");
	}
}

void
Fprint_double_table( FILE *file_Ptr, char *table_name, double *table ){
	int i1, i2;

	uint64_t KEY;
	KEY = (1L << KEY_bit);

	fprintf( file_Ptr, "%s\n\n", table_name );
	for( i1 = 0; i1 < HASH; i1++ ){
		for( i2 = 0; i2 < KEY; i2++ ){
			fprintf( file_Ptr, "%lf\t", *(table + i1*KEY + i2 ) );
		}
		fprintf( file_Ptr, "\n" );
	}
}

/* Determine the False Positive and False Negative */
void
Fprint_false_determine( TOP_VARIANCE *variance, TOP_VARIANCE *variance_2, char *name_buf, int time, int time_ns, int interval, double TA ){
	int i;
	int match;
	int number = 1;
	int counter_total = 0;
	int counter_not_match = 0;

	TOP_VARIANCE *list = variance;
	TOP_VARIANCE *list_2 = variance_2;

	char file_buf[100];
	char file_buf_tmp[10];
	char abuf[INET_ADDRSTRLEN];

	sprintf(file_buf,"Summary/%s/%s_", name_buf, name_buf);
	sprintf(file_buf_tmp,"%d.txt", interval);
	strcat(file_buf, file_buf_tmp);

	FILE *output_Ptr;
	output_Ptr = fopen( file_buf, "w" );

	Fprint_interval_info( output_Ptr, time + INTERVAL, time_ns, interval );
	fprintf( output_Ptr, "threshold = %lf\n", TA);

	while( list != NULL ){
		match = 0;
		counter_total++;
		while( (list_2 != NULL) && (match == 0) ){
			if( list->tuples.so_IP == list_2->tuples.so_IP ){
				match = 1;
				list->match = 1;
				list_2->match = 1;
			}
			else {
				list_2 = list_2->next_value;
			}
		}
		if( match == 0 ){
			counter_not_match++;
		}
		list_2 = variance_2;
		list = list->next_value;
	}

	fprintf( output_Ptr, "Total = %d, False Negative = %d", counter_total, counter_not_match);
	printf( "%s - Total = %d, False Negative = %d", name_buf, counter_total, counter_not_match);

	list = variance;
	list_2 = variance_2;
	counter_not_match = 0;

	while( list_2 != NULL ){
		if( list_2->match == 0 ){
			counter_not_match++;
		}
		list_2 = list_2->next_value;
	}
	fprintf( output_Ptr, ", False Positive = %d\n", counter_not_match);
	printf( ", False Positive = %d\n", counter_not_match);

	fprintf( output_Ptr, "\nFalse Negative IP\n");
	for( i = 0; i <= 60;i++ )
		fprintf( output_Ptr, "-" );	
	fprintf( output_Ptr, "\n" );

	while( list != NULL ){
		if( list->match == 0 ){
			fprintf( output_Ptr, "%3d. %s\tvalue = %f\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
		}
		number++;
		list = list->next_value;
	}

	list = variance;
	list_2 = variance_2;

	fprintf( output_Ptr, "\nFalse Positive IP\n");
	for( i = 0; i <= 60;i++ )
		fprintf( output_Ptr, "-" );	
	fprintf( output_Ptr, "\n" );

	number = 1;
	while( list_2 != NULL ){
		if( list_2->match == 0 ){
			fprintf( output_Ptr, "%3d. %s\tvalue = %f\n",number, inet_ntop(AF_INET, &list_2->tuples.so_IP, abuf, INET_ADDRSTRLEN), list_2->value);
		}
		number++;
		list_2 = list_2->next_value;
	}

	fprintf( output_Ptr, "\nIP Detect list\n");
	for( i = 0; i <= 60;i++ )
		fprintf( output_Ptr, "-" );	
	fprintf( output_Ptr, "\n" );

	number = 1;
	while( list != NULL ){
		if( list->match == 1 ){
			fprintf( output_Ptr, "%3d. %s\tvalue = %f\n",number, inet_ntop(AF_INET, &list->tuples.so_IP, abuf, INET_ADDRSTRLEN), list->value);
			list->match = 0;
		}
		number++;
		list = list->next_value;
	}

	while( variance_2 != NULL ){
		if( variance_2->match == 1 ){
			variance_2->match = 0;
		}
		number++;
		variance_2 = variance_2->next_value;
	}

	fclose(output_Ptr);
}
