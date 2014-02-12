/****************************************************************
 * packet.c														*
 *																*
 * Description: This file contains the functions for packets.	*
 *																*
 ***************************************************************/

#include "log.h"
#include "packet.h"
#include <string.h>
#include <netinet/in.h>

void fillPacketHeader(PacketHeader *ph, const char *buf)
{
	//Log(log, "Method call: fillPacketHeader\n");
	//Log(log, "PacketHeader: \n");
  uint32_t hostLong;
  uint16_t hostShort;
  memcpy(&hostShort, buf, 2);
  ph->magicNum = ntohs(hostShort);
	//Log(log, "   magicNum: %u\n", ph->magicNum);

  ph->version = buf[2];
	//Log(log, "   version: %u\n", ph->version);

  ph->type = buf[3];
	//Log(log, "   type: %u\n", ph->type);

  memcpy(&hostShort, buf + 4, 2);
  ph->headerLen = ntohs(hostShort);
	//Log(log, "   headerLen: %u\n", ph->headerLen);

  memcpy(&hostShort, buf + 6,2);
  ph->packetLen = ntohs(hostShort);
	//Log(log, "   packetLen: %u\n", ph->packetLen);

  memcpy(&hostLong, buf + 8, 4);
  ph->seqNum = ntohl(hostLong);
	//Log(log, "   seqNum: %u\n", ph->seqNum);

  memcpy(&hostLong, buf + 12, 4);
  ph->ackNum =ntohl(hostLong);
	//Log(log, "   ackNum: %u\n", ph->ackNum);
}

void createPacketHeader(PacketHeader *ph, unsigned int type, 
						unsigned int headerLen, unsigned int packetLen, 
						unsigned int seqNum, unsigned int ackNum){
	//Log(log, "Method call: createPacketHeader\n");

	ph->magicNum = 15441;
  ph->version = 1;
  ph->type = type;
  ph->headerLen = headerLen;
  ph->packetLen = packetLen;
  ph->seqNum = seqNum;
  ph->ackNum = ackNum;
/*
  Log(log, "createPacketHeader ph:\n");
	Log(log, "   Magic num: %u\n", ph->magicNum);
	Log(log, "   Version: %u\n", ph->version);
	Log(log, "   Type: %u\n", ph->type);
	Log(log, "   HeaderLen: %u\n", ph->headerLen);
	Log(log, "   PacketLen: %u\n", ph->packetLen);
	Log(log, "   Seq num: %u\n", ph->seqNum);
	Log(log, "   Ack num: %u\n", ph->ackNum);
	*/
}

void putHeader(const PacketHeader *ph, char *sendbuf){
	//Log(log, "Method call: putHeader\n");

  uint32_t netLong;
  uint16_t netShort = htons(ph->magicNum);
  memcpy(&(sendbuf[0]), &netShort,2);
  sendbuf[2] = (ph->version) & 0xff;
  sendbuf[3] = (ph->type) & 0xff;
  netShort = htons(ph->headerLen);
  memcpy(&(sendbuf[4]), &(netShort), 2);
  netShort = htons(ph->packetLen);
  memcpy(&(sendbuf[6]), &(netShort), 2);
  netLong = htonl(ph->seqNum);
  memcpy(&(sendbuf[8]), &(netLong), 4);
  netLong = htonl(ph->ackNum);
  memcpy(&(sendbuf[12]), &(netLong), 4);
/*
	Log(log, "putHeader ph:\n");
  Log(log, "   Magic num: %u\n", ph->magicNum);
  Log(log, "   Version: %u\n", ph->version);
  Log(log, "   Type: %u\n", ph->type);
  Log(log, "   HeaderLen: %u\n", ph->headerLen);
  Log(log, "   PacketLen: %u\n", ph->packetLen);
  Log(log, "   Seq num: %u\n", ph->seqNum);
  Log(log, "   Ack num: %u\n", ph->ackNum);
	*/
}
