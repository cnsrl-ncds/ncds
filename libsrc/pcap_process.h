extern void read_pcap_header( FILE *in_trace, PCAP_HDR *pcap_hdr );

extern void read_pcap_pkt_header( FILE *in_trace, PCAP_HDR *pcap_hdr, PCAP_PKT_HDR *pcap_pkt_hdr );

extern uint64_t swap( uint64_t data, int size );
