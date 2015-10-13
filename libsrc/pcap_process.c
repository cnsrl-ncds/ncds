#include "kary.h"
#include "pcap_process.h"

void
read_pcap_header( FILE *in_trace, PCAP_HDR *pcap_hdr ) {
	char magic_number[8];

	if( fread( pcap_hdr,sizeof(PCAP_HDR),1,in_trace ) == 0 ){
		error( "fread", "In pcap_process.c, function read_pcap_header, pcap_hdr read failed" );
	}

	/* Compare Magic Number in PCAP header
		0xA1B2C3D4 mean identical
		0xD4C3B2A1 mean swapped, and will be swap into identical */
	sprintf(magic_number, "%X", pcap_hdr->magic_number);

	if( strcmp( "D4C3B2A1", magic_number ) == 0 ){
		pcap_hdr->version_major = swap( pcap_hdr->version_major, sizeof(pcap_hdr->version_major) );
		pcap_hdr->version_minor = swap( pcap_hdr->version_minor, sizeof(pcap_hdr->version_minor) );
		pcap_hdr->thiszone = swap( pcap_hdr->thiszone, sizeof(pcap_hdr->thiszone) );
		pcap_hdr->sigfigs = swap( pcap_hdr->sigfigs, sizeof(pcap_hdr->sigfigs) );
		pcap_hdr->snaplen = swap( pcap_hdr->snaplen, sizeof(pcap_hdr->snaplen) );
		pcap_hdr->network = swap( pcap_hdr->network, sizeof(pcap_hdr->network) );
	}

	#ifdef Debug
		#if Debug >= 2
			printf("Pcap version = %d.%d\n", pcap_hdr->version_major, pcap_hdr->version_minor);
		#endif
	#endif
}

void
read_pcap_pkt_header( FILE *in_trace, PCAP_HDR *pcap_hdr, PCAP_PKT_HDR *pcap_pkt_hdr ) {
	char magic_number[8];

	/* Compare Magic Number in PCAP header
		0xA1B2C3D4 mean identical
		0xD4C3B2A1 mean swapped, and the header will be swap into identical */
	sprintf(magic_number, "%X", pcap_hdr->magic_number);

	if( strcmp( "D4C3B2A1", magic_number ) == 0 ){
		pcap_pkt_hdr->ts_sec = swap( pcap_pkt_hdr->ts_sec, sizeof(pcap_pkt_hdr->ts_sec) );
		pcap_pkt_hdr->ts_usec = swap( pcap_pkt_hdr->ts_usec, sizeof(pcap_pkt_hdr->ts_usec) );
		pcap_pkt_hdr->incl_len = swap( pcap_pkt_hdr->incl_len, sizeof(pcap_pkt_hdr->incl_len) );
		pcap_pkt_hdr->orig_len = swap( pcap_pkt_hdr->orig_len, sizeof(pcap_pkt_hdr->orig_len) );
	}
}

uint64_t
swap( uint64_t data, int size ){
	int		i;
	uint64_t	temp = 0x00000000;

	for( i = 0; i < (size/2); i++ ){
		temp = temp | ( ( data & (0x000000FF << (8*i)) ) << ( size-2*i-1 )*8 );
	}
	for( i = 0; i < (size/2); i++ ){
		temp = temp | ( ( data & (0x000000FF << ((size-i-1)*8)) ) >> ( size-2*i-1 )*8 );
	}

	return(temp);
}
