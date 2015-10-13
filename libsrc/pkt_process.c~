#include "kary.h"
#include "pkt_process.h"
//JJ modify to take care of traces without Ethernet header
//  2012-10-26
//YK put this file for discount 1W purpose
int
check_trace_type( char *trace_Ptr ){
	if( ~strcmp( trace_Ptr+strlen( trace_Ptr )-4 ,"pcap") )
		return(1);
	else if( ~strcmp( trace_Ptr+strlen( trace_Ptr )-4 ,"dump") )
		return(1);
	else if( ~strcmp( trace_Ptr+strlen( trace_Ptr )-3 ,"tsh") )
		return(2);
	else
		return(3);
}

int
read_pcap( FILE *trace_Ptr, PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5 ){

	int *dont_care;
	ETHER_HDR	ethernet_header;
	IP_V4_HDR	ip_v4_header;
	IP_V6_HDR	ip_v6_header;
	TCP_HDR	tcp_header;

	/* error Ethernet Header */
/*	if( pcap_pkt_header->incl_len < 14 ){
		dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len) );
		free(dont_care);
		return(0);
	}
*/
	/* Only Ethernet Header */
/*	else if( pcap_pkt_header->incl_len == 14 ){
		fread( &ethernet_header,sizeof(ETHER_HDR),1,trace_Ptr );
		return(0);
	}
*/
	/* error IP_v4 Header */
//	else if( pcap_pkt_header->incl_len < 34 ){
	if( pcap_pkt_header->incl_len < 20 ){
		dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len) );
		printf("IP_v4 Header Failed\n");
		free(dont_care);
		return(0);
	}

	/* Only Ethernet Header + IP_v4 Header */
//	else if( pcap_pkt_header->incl_len == 34 ){
	else if( pcap_pkt_header->incl_len == 20 ){
//		fread( &ethernet_header,sizeof(ETHER_HDR),1,trace_Ptr );
		fread( &ip_v4_header,sizeof(IP_V4_HDR),1,trace_Ptr );
		tcp_header.so_port = 0;
		tcp_header.de_port = 0;
	}

	/* error Protocol Header */
/*	else if( pcap_pkt_header->incl_len < 38 ){
		fread( &ethernet_header,sizeof(ETHER_HDR),1,trace_Ptr );
		fread( &ip_v4_header,sizeof(IP_V4_HDR),1,trace_Ptr );
		dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len - sizeof(ETHER_HDR) - sizeof(IP_V4_HDR) ) );
		fread( dont_care,( pcap_pkt_header->incl_len - sizeof(ETHER_HDR) - sizeof(IP_V4_HDR) ),1,trace_Ptr );
		free(dont_care);
		tcp_header.so_port = 0;
		tcp_header.de_port = 0;
	}
	else {
		fread( &ethernet_header,sizeof(ETHER_HDR),1,trace_Ptr );
*/
	else {
		fread( &ip_v4_header,sizeof(IP_V4_HDR),1,trace_Ptr );

		/* TCP Packets or UDP Packets*/
		if( ip_v4_header.Protocol == 6 || ip_v4_header.Protocol == 17 ){
			fread( &tcp_header,sizeof(TCP_HDR),1,trace_Ptr );
			dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len - /*sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) - sizeof(TCP_HDR) ) );
			fread( dont_care,( pcap_pkt_header->incl_len - /*sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) - sizeof(TCP_HDR) ),1,trace_Ptr );
			free(dont_care);
		}

		/* IP_v6 Packets*/
		else if( ip_v4_header.Protocol == 41 ){
			if( pcap_pkt_header->incl_len == 74 ){
				fread( &ip_v6_header,sizeof(IP_V6_HDR),1,trace_Ptr );
			}
			else if( pcap_pkt_header->incl_len > 74 ){
				fread( &ip_v6_header,sizeof(IP_V6_HDR),1,trace_Ptr );
				dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len -/* sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) - sizeof(IP_V6_HDR) ) );
				fread( dont_care,( pcap_pkt_header->incl_len -/* sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) - sizeof(IP_V6_HDR) ),1,trace_Ptr );
				free(dont_care);
			}
			else {
				dont_care = malloc( sizeof(uint8_t)*(pcap_pkt_header->incl_len -/* sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) ) );
				fread( dont_care,( pcap_pkt_header->incl_len -/* sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) ),1,trace_Ptr );
				free(dont_care);
			}
		}
			
		/* undefine Packets */
		else {
			dont_care = malloc( sizeof(uint8_t)*( pcap_pkt_header->incl_len - /*sizeof(ETHER_HDR) - */sizeof(IP_V4_HDR) ) );
			fread( dont_care,( pcap_pkt_header->incl_len -/* sizeof(ETHER_HDR) -*/ sizeof(IP_V4_HDR) ),1,trace_Ptr );
			free(dont_care);
			tcp_header.so_port = 0;
			tcp_header.de_port = 0;
		}
	}

	/* Prepare packet information structure */
	memcpy( &tuple5->so_IP, &ip_v4_header.so_IP, 4 );
	memcpy( &tuple5->de_IP, &ip_v4_header.de_IP, 4 );
	tuple5->so_port = ntohs(tcp_header.so_port);
	tuple5->de_port = ntohs(tcp_header.de_port);
	memcpy( &tuple5->protocol, &ip_v4_header.Protocol, 1 );
	/* End of preparation */

	return(0);
}

int
display_trace_info( char *in_trace ) {
	int i, trace_set;
	FILE *trace_Ptr;
	for( i = 0; i <= 100;i++ )
		printf("=");	
	putchar('\n');
	printf("Input file : %s\n",in_trace);
	if( ( trace_Ptr = fopen( in_trace, "r" ) ) == NULL) {
		printf("Can't open %s\n", in_trace);
		error_1(0);
	}
	fclose( trace_Ptr );
	trace_set = check_trace_type(in_trace);
	if( trace_set == 1 ) {
		printf("File Format: pcap");
		putchar('\n');
	}
	else if( trace_set == 2 )
		printf("File Format: tsh\n");		
	else 	error_1(1);
	for( i = 0; i <= 100;i++ )
		printf("=");	
	putchar('\n');
	return(trace_set);
}

int
Fdisplay_trace_info( char *in_trace, int interval ) {
	int i, trace_set;
	FILE *trace_Ptr;

	char file_buf[15]={"Display_PKT_"};
	char file_buf2[3];
	sprintf(file_buf2,"%d",interval);
	strcat(file_buf, file_buf2);

	FILE *output_Ptr;
	output_Ptr = fopen( file_buf, "a" );

	for( i = 0; i <= 100;i++ )
		fprintf(output_Ptr,"=");	
	fprintf(output_Ptr, "\n");
	fprintf(output_Ptr,"Input file : %s\n",in_trace);
	if( ( trace_Ptr = fopen( in_trace, "r" ) ) == NULL) {
		fprintf(output_Ptr,"Can't open %s\n", in_trace);
		error_1(0);
	}
	fclose( trace_Ptr );
	trace_set = check_trace_type(in_trace);
	if( trace_set == 1 ) {
		fprintf(output_Ptr,"File Format: pcap");
		fprintf(output_Ptr, "\n");
	}
	else if( trace_set == 2 )
		fprintf(output_Ptr,"File Format: tsh\n");		
	else 	error_1(1);
	for( i = 0; i <= 100;i++ )
		fprintf(output_Ptr, "=");	
	fprintf(output_Ptr, "\n");

	fclose(output_Ptr);

	return(trace_set);
}

void
display_trace( PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5 ){

	time_t	cal_time;

	struct tm	*ts;
	char		buf[20];
	char		abuf[INET_ADDRSTRLEN];

	/* Print packet information */
	cal_time = pcap_pkt_header->ts_sec;
	ts = localtime(&cal_time);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
	if( pcap_pkt_header->ts_usec == 0 )
		printf("%s.000000", buf);
	else if( pcap_pkt_header->ts_usec < 10 )
		printf("%s.00000%d", buf, pcap_pkt_header->ts_usec);
	else if( pcap_pkt_header->ts_usec < 100 )
		printf("%s.0000%2d", buf, pcap_pkt_header->ts_usec);
	else if( pcap_pkt_header->ts_usec < 1000 )
		printf("%s.000%3d", buf, pcap_pkt_header->ts_usec);
	else if( pcap_pkt_header->ts_usec < 10000 )
		printf("%s.00%4d", buf, pcap_pkt_header->ts_usec);
	else if( pcap_pkt_header->ts_usec < 100000 )
		printf("%s.0%5d", buf, pcap_pkt_header->ts_usec);
	else
		printf("%s.%6d", buf, pcap_pkt_header->ts_usec);

	printf("\t%d\t\t%s:%d\t%s:%d\n", tuple5->protocol, inet_ntop(AF_INET, &tuple5->so_IP, abuf, INET_ADDRSTRLEN), tuple5->so_port, inet_ntop(AF_INET, &tuple5->de_IP, abuf, INET_ADDRSTRLEN), tuple5->de_port);
}

void
Fdisplay_trace( PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5, int interval ){

	time_t	cal_time;

	struct tm	*ts;
	char		buf[20];
	char		abuf[INET_ADDRSTRLEN];

	char file_buf[15]={"Display_PKT_"};
	char file_buf2[3];
	sprintf(file_buf2,"%d",interval);
	strcat(file_buf, file_buf2);

	FILE *output_Ptr;
	output_Ptr = fopen( file_buf, "a" );

	/* Print packet information */
	cal_time = pcap_pkt_header->ts_sec;
	ts = localtime(&cal_time);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ts);
	fprintf(output_Ptr, "%s.%6d\t%d\t\t%s:%d\t%s:%d\n", buf, pcap_pkt_header->ts_usec, tuple5->protocol, inet_ntop(AF_INET, &tuple5->so_IP, abuf, INET_ADDRSTRLEN), tuple5->so_port, inet_ntop(AF_INET, &tuple5->de_IP, abuf, INET_ADDRSTRLEN), tuple5->de_port);

	fclose(output_Ptr);
}

void
error_1( short set ){
	int i;
	for( i = 0; i <= 70;i++ )
		printf("=");
	putchar('\n');

	switch( set ){
		case 1:
			printf("Can't identify input data format\n");
			break;
		case 2:
			printf("Different data format than last trace\n");
			break;
		case 3:
			printf("File error\n");
			break;
		default:
			break;
	}

	printf("Process cancel\n");
	for( i = 0; i <= 70;i++ )
		printf("=");
	putchar('\n');
	exit(0);
}
