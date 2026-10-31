#include <stropts.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "aenuser.h"
#include "structs.h"
#include "bitfields.h"
#include "extern.h"

static void hack_packet(char *, int);
static void hexdump(char *, int, char *, char *);
static void d_ethernet(char *, int, char *);
static void d_arp(char *, int, char *);
static void d_ip(char *, int, char *);
static void d_tcp(char *, int, char *);
static char *p_ip_address(unsigned char *);

static void
barf(int sig)
{
    aen_config.mode = oldmode;
    (void) ioctl(aenfd, AEN_SET_CONFIG, &aen_config);
    (void) kill(getpid(), sig);
    exit(1);
}

void
trace_packets()
{
    struct strbuf ctl, data;
    char packet[1518];
    aen_status_t aen_status;
    int zero=0;

    aen_config.flags |= AEN_RAW_DATA;

    if (trace_permiscious)
    {
	int i;

	oldmode = aen_config.mode;
	aen_config.mode |= AEN_MODE_PROM;
	for (i=0; i < NSIG; ++i)
	    (void) sigset(i, barf);
    }

    if (ioctl(aenfd, AEN_SET_CONFIG, &aen_config) < 0)
    {
	(void) fprintf(stderr, "%s: couldn't set configuration: %s (%s)\n",
		       program_name, device_name, strerror(errno));
	exit(1);
    }

    ctl.maxlen = sizeof(aen_status);
    ctl.buf = (char *)&aen_status;

    data.maxlen = sizeof(packet);
    data.buf = packet;

    for(;;)
    {
	getmsg(aenfd, &ctl, &data, &zero);
#if 0
	hexdump(ctl.buf, ctl.len, "CTL:\n");
#endif
	if (packet_hacking)
	    hack_packet(data.buf, data.len);
	else
	{
	    hexdump(data.buf, data.len, "ETHERNET PACKET:\n", "");
	}
    }
}

static void
hack_packet(char *packet, int length)
{
    char prefix[100];

    prefix[0] = '\0';
    d_ethernet(packet, length, prefix);
}

static void
d_ethernet(char *packet, int length, char *prefix)
{
    struct ether_header *eh = (struct ether_header *)packet;
    char *data = &packet[sizeof(struct ether_header)];
    int data_length = length - sizeof(struct ether_header) - 4;
    static struct Bitfields BF_ether_type[] =
    {
	{ B_type, ETHERTYPE_PUP, "PUP", },
	{ B_type, ETHERTYPE_IP, "IP", },
	{ B_type, ETHERTYPE_ARP, "ARP", },
	{ B_type, ETHERTYPE_REVARP, "REVARP", },
	{ B_end, 0, NULL, },
    };

    (void) printf("%sETHERNET PACKET:\n", prefix);
    (void) printf("%sEthernet Destination Address: %s\n", prefix,
		  p_ethernet_address(eh->ether_dhost));
    (void) printf("%sEthernet Source Address: %s\n", prefix,
		  p_ethernet_address(eh->ether_shost));
    (void) printf("%sEthernet Type: %s\n", prefix,
		  compilebitfield(eh->ether_type, BF_ether_type));

    /* <IMPLEMENT> Handle ethernet trailers */

    strcat(prefix, " ");

    switch (eh->ether_type)
    {
    case ETHERTYPE_ARP:
	d_arp(data, data_length, prefix);
	break;

    case ETHERTYPE_IP:
	d_ip(data, data_length, prefix);
	break;

    case ETHERTYPE_PUP:
    case ETHERTYPE_REVARP:
    default:
	hexdump(data, data_length, "DATA:\n", prefix);
	break;
    }

    prefix[strlen(prefix)-1] = '\0';

    (void) printf("%sEthernet CRC: 0x%04x\n", prefix,
		  *(int *)&packet[length-4]);
}

char *
p_ethernet_address(unsigned char *address)
{
    static char address_string[sizeof("XX:XX:XX:XX:XX:XX:")];

    (void) sprintf(address_string, "%02x:%02x:%02x:%02x:%02x:%02x",
		   address[0], address[1], address[2], address[3], address[4],
		   address[5]);

    return address_string;
}

static void
d_arp(char *packet, int length, char *prefix)
{
    hexdump(packet, length, "ARP:\n", prefix);
}

static void
d_ip(char *packet, int length, char *prefix)
{
    struct ip *iph = (struct ip *)packet;
    char *data = &packet[(iph->ip_hl * 4)];
    int data_length = length - (iph->ip_hl * 4);
    static struct Bitfields BF_IP_TOS[] =
    {
	{ B_bitval, 0xF0, "Precedence", },
	{ B_bitfield, 0x08, "LOWDELAY", },
	{ B_bitfield, 0x04, "HIGHTHROUGHPUT", },
	{ B_bitfield, 0x02, "HIGHRELIABILITY", },
	{ B_end, 0, NULL, },
    };
    static struct Bitfields BF_IP_Offset[] =
    {
	{ B_bitval, 0x0FFF, "Offset", },
	{ B_bitfield, 0x4000, "DONTFRAG", },
	{ B_bitfield, 0x2000, "MOREFRAGS", },
	{ B_end, 0, NULL, },
    };
    static struct Bitfields BF_IP_Proto[] =
    {
	{ B_type, IPPROTO_IP, "IP", },
	{ B_type, IPPROTO_ICMP, "ICMP", },
	{ B_type, IPPROTO_IGMP, "IGMP", },
	{ B_type, IPPROTO_GGP, "GGP", },
	{ B_type, IPPROTO_TCP, "TCP", },
	{ B_type, IPPROTO_EGP, "EGP", },
	{ B_type, IPPROTO_PUP, "PUP", },
	{ B_type, IPPROTO_UDP, "UDP", },
	{ B_type, IPPROTO_IDP, "IDP", },
	{ B_type, IPPROTO_HELLO, "HELLO", },
	{ B_type, IPPROTO_ND, "ND", },
	{ B_type, IPPROTO_RAW, "RAW", },
	{ B_end, 0, NULL, },
    };

    (void) printf("%sINTERNET PROTOCOL PACKET:\n", prefix);
    (void) printf("%sIP Version: %d\n", prefix, iph->ip_v);
    (void) printf("%sIP Header Length: %d, Type Of Service: %s\n", prefix,
		  (iph->ip_hl * 4), compilebitfield(iph->ip_tos, BF_IP_TOS));
    (void) printf("%sIP Total Length: %d, ID: 0x%02x, Offset: %s\n", prefix,
		  iph->ip_len, iph->ip_id,
		  compilebitfield(iph->ip_off, BF_IP_Offset));
    (void) printf("%sIP Time To Live: %u, Protocol: %s\n", prefix, iph->ip_ttl,
		  compilebitfield(iph->ip_p, BF_IP_Proto));
    (void) printf("%sIP Checksum: 0x%02x\n", prefix, iph->ip_sum);
    (void) printf("%sIP Source Address: %s\n", prefix,
		  p_ip_address((unsigned char *)&iph->ip_src));
    (void) printf("%sIP Destination Address: %s\n", prefix,
		  p_ip_address((unsigned char *)&iph->ip_dst));
    /* <IMPLEMENT> Handle options field */

    strcat(prefix, " ");

    switch (iph->ip_p)
    {
    case IPPROTO_TCP:
	d_tcp(data, data_length, prefix);
	break;

    case IPPROTO_IP:
    case IPPROTO_ICMP:
    case IPPROTO_IGMP:
    case IPPROTO_GGP:
    case IPPROTO_EGP:
    case IPPROTO_PUP:
    case IPPROTO_UDP:
    case IPPROTO_IDP:
    case IPPROTO_HELLO:
    case IPPROTO_ND:
    case IPPROTO_RAW:
    default:
	hexdump(data, data_length, "DATA:\n", prefix);
	break;
    }

    prefix[strlen(prefix)-1] = '\0';
}

static void
d_tcp(char *packet, int length, char *prefix)
{
    struct tcphdr *tcph = (struct tcphdr *)packet;
    char *data = &packet[tcph->th_off * 4];
    int data_length = length - (tcph->th_off * 4);
    static struct Bitfields BF_TCP_Flags[] =
    {
	{ B_bitfield, TH_URG, "URG", },
	{ B_bitfield, TH_ACK, "ACK", },
	{ B_bitfield, TH_PUSH, "PUSH", },
	{ B_bitfield, TH_RST, "RST", },
	{ B_bitfield, TH_SYN, "SYN", },
	{ B_bitfield, TH_FIN, "FIN", },
	{ B_end, 0, NULL, },
    };

    (void) printf("%sTCP PACKET:\n", prefix);
    (void) printf("%sTCP Source Port: %d, Destination Port: %d\n", prefix,
		  tcph->th_sport, tcph->th_dport);
    (void) printf("%sTCP Sequence Number: %d, Acknowledgement Number: %d\n",
		  prefix, tcph->th_seq, tcph->th_ack);
    (void) printf("%sTCP Offset: %d, Flags: %s\n", prefix, tcph->th_off * 4,
		  compilebitfield(tcph->th_flags, BF_TCP_Flags));
    (void) printf("%sTCP Window: %d, Checksum: 0x%02x, Urgent Data Ends: %d\n",
		  prefix, tcph->th_win, tcph->th_sum, tcph->th_urp);
}

static char *
p_ip_address(unsigned char *address)
{
    static char address_string[sizeof("XXX.XXX.XXX.XXX")];

    (void) sprintf(address_string, "%u.%u.%u.%u",
		   address[0], address[1], address[2], address[3]);

    return address_string;
}

static void
hexdump(char *buf, int length, char *header, char *prefix)
{
    int i;
    unsigned int hd_address = 0;
    char hd_text[17], *hdp, hd_addrtext[4], *hda;
    static char hd_spaces[]="                                                ";

    printf("%s%s", prefix, header);

    while (length)
    {
	hda = hd_addrtext;

	if (hd_address < 0x1000)
	    *hda++ = '0';

	if (hd_address < 0x100)
	    *hda++ = '0';

	if (hd_address < 0x10)
	    *hda++ = '0';

	*hda = '\0';
	printf("%s%x: ", hd_addrtext, hd_address);

	hdp = hd_text;
	for (i=0; i < 16 && length; ++i, --length, ++hd_address)
	{
	    register unsigned char c = *buf++;

	    *hdp++ = ((c < 20) || ((c > 0x7F) && (c < 0xa0))) ? '.' : c;

	    if (c < 0x10)
		printf("0%x ", c);
	    else
		printf("%x ", c);
	}

	if (i < 16)
	    printf("%s", &hd_spaces[(16*3) - ((16-i)*3)]);

	*hdp = '\0';
	printf("%s\n", hd_text);
    }
}
