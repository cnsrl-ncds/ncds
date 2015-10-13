#include <stdint.h>

typedef struct pcap_hdr {
	uint32_t	magic_number;   /* magic number */
	uint16_t	version_major;  /* major version number */
	uint16_t	version_minor;  /* minor version number */
	int32_t	thiszone;       /* GMT to local correction */
	uint32_t	sigfigs;        /* accuracy of timestamps */
	uint32_t	snaplen;        /* max length of captured packets, in octets */
	uint32_t	network;        /* data link type */
} PCAP_HDR;

typedef struct pcap_pkt_hdr {
	uint32_t	ts_sec;         /* timestamp seconds */
	uint32_t	ts_usec;        /* timestamp microseconds */
	uint32_t	incl_len;       /* number of octets of packet saved in file */
	uint32_t	orig_len;       /* actual length of packet */
} PCAP_PKT_HDR;

typedef struct ether_hdr {
	uint8_t	de_MAC[6];
	uint8_t	so_MAC[6];
	uint8_t	ethernet_type[2];
} ETHER_HDR;

typedef struct IP_V4_hdr {
	uint8_t	version;		/* 4-bit version + 4-bit header_length */
	uint8_t	type_service;
	uint8_t	total_length[2];
	uint16_t	identification;
	uint16_t	fragment;		/* 3-bit IP_Flags + 13-bit Fragment offset */
	uint8_t	TTL;
	uint8_t	Protocol;
	uint16_t	Header_checksum;
	uint8_t	so_IP[4];
	uint8_t	de_IP[4];
} IP_V4_HDR;

typedef struct IP_V6_hdr {
	uint32_t	version;		/* 4-bit version + 8-bit traffic class + 20-bit flow label */
	uint16_t	payload_length;
	uint8_t	next_header;
	uint8_t	hop_limit;
	uint16_t	so_IP[8];
	uint16_t	de_IP[8];
} IP_V6_HDR;

typedef struct TCP_hdr {
	uint16_t	so_port;
	uint16_t	de_port;
} TCP_HDR;
