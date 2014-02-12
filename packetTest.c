/********************************************************************
 * packetTest.c														*
 *                                                                  *
 * Description: This file contains a main to test all of the        *
 *              functions used on a packetHeader.                   *
 *                                                                  *
 *******************************************************************/

#include "packet.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void print_bytes(const void *object, size_t size)
{
  size_t i;

  printf("[ ");
  for(i = 0; i < size; i++)
  {
    printf("%02x ", ((const unsigned char *) object)[i] & 0xff);
  }
  printf("]\n");
}

//
// main: runs a simple test to check the packetHeader functions
//
int main(){
	//First create a log file to log output
	FILE *log;
	log = open_log("packetHeaderTest.log");
	Log(log, " ---- Testing PacketHeader Functions ---- \n");

	//Declare a new sendbuffer
	char sendbuf[1500];
	
	PacketHeader ph;
	PacketHeader outph;

	//Test createPacketHeader
	Log(log, " ---- Test 1: createPacketHeader ---- \n");
	createPacketHeader(&ph,1,16,24,2,3);

	Log(log, " PacketHeader variables:\n");
	Log(log, "  magicNum: %u\n", ph.magicNum);
	Log(log, "  version: %u\n", ph.version);
	Log(log, "  type: %u\n", ph.type);
	Log(log, "  headerLen: %u\n", ph.headerLen);
	Log(log, "  packetLen: %u\n", ph.packetLen);
	Log(log, "  seqNum: %u\n", ph.seqNum);
	Log(log, "  ackNum: %u\n", ph.ackNum);

	if((ph.magicNum == 15441) && (ph.version == 1) && (ph.type == 1)
			&& (ph.headerLen == 16) && (ph.packetLen == 24)
			&& (ph.seqNum == 2) && (ph.ackNum == 3))
		Log(log, "   GOOD. PacketHeader is as expected\n");
	else
		Log(log, "   BAD. PacketHeader is incorrect\n");

	putHeader(&ph, sendbuf);
	sendbuf[ph.headerLen] = (char)0;

	//Test fillPacketHeader
	Log(log, " ---- Test 2: fillPacketHeader ---- \n");
	fillPacketHeader(&outph,sendbuf);

    Log(log, " Outgoing PacketHeader variables:\n");
    Log(log, "  magicNum: %u\n", outph.magicNum);
    Log(log, "  version: %u\n", outph.version);
    Log(log, "  type: %u\n", outph.type);
    Log(log, "  headerLen: %u\n", outph.headerLen);
    Log(log, "  packetLen: %u\n", outph.packetLen);
    Log(log, "  seqNum: %u\n", outph.seqNum);
    Log(log, "  ackNum: %u\n", outph.ackNum);

    if((ph.magicNum == outph.magicNum) && (ph.version == outph.version) 
			&& (ph.type == outph.type) && (ph.headerLen == outph.headerLen)
			&& (ph.packetLen == outph.packetLen) && (ph.seqNum == outph.seqNum)
			&& (ph.ackNum == outph.ackNum)){
        Log(log, "   GOOD. Outgoing PacketHeader is as expected\n");
		Log(log, "   SUPER GOOD. PacketHeader functions are working correctly\n");
	}
    else
        Log(log, "   BAD. Outgoing PacketHeader is incorrect\n");
	
	return 0;
}

