/****************************************************************
 * packet.h														*
 *																*
 * Description: This file contains the parameters for packets	*
 *				and the structure and method definitions		*
 *				for packet headers.								*
 *																*
 ***************************************************************/
#ifndef _PACKET_H_
#define _PACKET_H_

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

//Packet Type Codes
#define WHOHAS 0
#define IHAVE 1
#define GET 2
#define DATA 3
#define ACK 4
#define DENIED 5

//Valid Packet Headers
#define MAGICNUM = 15441
#define VERSION = 1

//Packet Header Structure
typedef struct{
  unsigned int magicNum;
  unsigned int version;
  unsigned int type;
  unsigned int headerLen;
  unsigned int packetLen;
  unsigned int seqNum;
  unsigned int ackNum;
}PacketHeader;

//
// fillPacketHeader: Gets each header field from a packet
// Parameters:
//      const char *buf: packet header
//
void fillPacketHeader(PacketHeader *ph, const char *buf);

//
// createPacketHeader: puts parameters into packetHeader struct
// Parameters:
// 		PacketHeader *ph: packet header
// 		unsigned int type: packet type
// 		unsigned int headerLen: header length
// 		unsigned int packetLen: packet length
// 		unsigned int seqNum: sequence number
// 		unsigned int ackNum: acknowledgement number
//
void createPacketHeader(PacketHeader *ph, unsigned int type, 
			unsigned int headerLen, unsigned int packetLen, 
			unsigned int seqNum, unsigned int ackNum);

//
// putHeader : puts the info from packetHeader into the buffer
// Parameters: 
// 		const PacketHeader *ph: packet header
// 		char *sendbuf: send buffer
//
void putHeader(const PacketHeader *ph, char *sendbuf);
#endif
