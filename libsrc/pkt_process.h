extern int check_trace_type( char *trace_Ptr );

extern int read_pcap( FILE *trace_Ptr, PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5 );

extern int display_trace_info( char *in_trace );

extern int Fdisplay_trace_info( char *in_trace, int interval );

extern void display_trace( PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5 );

extern void Fdisplay_trace( PCAP_PKT_HDR *pcap_pkt_header, TUPLE_5 *tuple5, int interval );

extern void error_1( short set );
