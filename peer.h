/****************************************************************
 * peer.h                                                     	*
 *                                                              *
 * Description: This file contains the parameters for the main 	*
 * 							peer.c BitTorrent file transfer application.		*
 *              This file defines the variables, structures 		*
 *              and method definitions for peer.c								*
 *                                                              *
 ***************************************************************/
#ifndef _PEER_H_
#define _PEER_H_

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "spiffy.h"
#include "bt_parse.h"
#include "input_buffer.h"
#include "log.h"
#include "packet.h"
#include "hashList.h"
//#include "orderedList.h"

#define SUCCESS 1
#define FAILURE -1
#define HASHSIZE 20
#define CHUNKSIZE 524288
#define MAXPACKETSIZE 1500
#define MAXSEQNUM 353
#define HEADERLEN 16
#define SPIFFY 1
#define BUFLEN 1500
#define WINDOWSIZE 8

#define WINDOW_SIZE 8
#define MAX_PACKET_SIZE 1500

static FILE *log;
//File to be used during congestion control
//static FILE *window_size_file;
int globalsock;
bt_config_t *globalconfig;
hashList *globalReqHashes;
time_t getTimeout;
int waiting;

//getTime Structure
struct tm * getTime();

//
// Main: Runs the server
// Parameters: Takes in 6 arguments
// Returns: 0 on success
//
int main(int argc, char **argv);

//
// endDownload: closes the output file if there are no more hashes
// 				left to download, otherwise sends a WHOHAS request
// 				for the remaining hashes
// Parameters:
// 		hashList *downloadingHashes
// 		hashNode *node
//              int      status: -1 if download did not finish
// Returns:
// 		int: SUCCESS or FAILURE
//
int endDownload(hashList *downloadingHashes, hashNode *node, int status);

//
// endUpload: removes the node from uploadingHashes and returns
// Parameters:
// 		hashList *uploadingHashes
// 		hashNode *node
// Returns:
// 		int: SUCCESS or FAILURE
//
int endUpload(hashList *uploadingHashes, hashNode *node);

//
// sendNextData: sends the hashNode of data to the client, if seqNum < 0,
// 				 then increments and sends node->seqNode
// Parameters:
// 		int sock: socket that connection is on
// 		hashNode *node: node with data
// 		FILE *datafile
// 		int seqNum: sequence number
// Returns:
// 		int: SUCCESS or FAILURE
//
int sendNextData(int sock, hashNode *node, FILE *datafile, int seqNum);

//
// monitor: checks for timeouts of uploadingHashes, monitors uploading
// 			connections and fills window
// Parameters:
// 		int sock: socket of connection
// 		bt_config_t *config: parsed from command line
// 		hashList *requestedHashes
// 		hashList *downloading hashes
// 		hashList *uploadingHashes
//
void monitor(int sock, bt_config_t *config, hashList *requestedHashes, 
			 hashList *downloadingHashes, hashList *uploadingHashes);

//
// sendToAllPeers: Sends buffer to everyone in peer_list_file
// Parameters:
//      int sock
//      char *sendbuf
//      unsigned int senddatalen
//      const bt_config_t *config
//
void sendToAllPeers(int sock, char *sendbuf, unsigned int senddatalen, const bt_config_t *config);

// saveData: saves the data from a file to a hashNode
// Parameters: 
// 		const char *data
// 		hashNode *node
// 		FILE *f
// 		int seqNum 
// 		unsigned int datalen
// Returns: 
// 		unsigned int: 
//
unsigned int saveData(const char *data, hashNode *node, FILE *f, int seqNum, unsigned int datalen);

// putData: puts the data into a hashNode and returns the number of bytes read
// Parameters:
// 		char *senddata
// 		hashNode *node
// 		FILE *f
// 		int seqNum
// Returns:
// 		unsigned int: number of bytes read
//
unsigned int putData(char *senddata, hashNode *node, FILE *f, int seqNum);

//
// doIHaveChunk: returns chunkID if it appears in the requestedHashes
// Parameters:
// 		const char *reqHash
// 		const char *has_chunk_file
// Returns:
// 		int: chunkID of requestedHash
//
int doIHaveChunk(const char *reqHash, const char *has_chunk_file);

//
// generateResponsetoIHAVE: adds hash to downloadingHashes if hash appears
// 							in the requestedHashes and puts the hash in body
// Parameters:
// 		const char *data
// 		char *senddata
// 		hashList *requestedHashes
// 		hashList *downloadingHashes
// 		struct sockaddr_in *addr
// Returns:
// 		int: SUCCESS or FAILURE
//
int generateResponseToIHAVE(const char *data, char *senddata, 
							hashList *requestedHashes,
							hashList *downloadingHashes, 
							struct sockaddr_in *addr);

//
// generateResponseToGET: if IHaveChunk, then sets up an uploadConnection
// 						  and sends the first DATA
// Parameters:
// 		const char *data
// 		char *senddata
// 		const bt_config_t *config
// 		hashList*uploadingHashes
// 		struct sockaddr_in *addr
// Returns:
// 		int: SUCCESS or FAILURE
//
int generateResponseToGET(const char *data, char *senddata, 
						  const bt_config_t *config,
						  hashList *uploadingHashes, 
						  struct sockaddr_in *addr);

//
// generateResponseToDATA: returns an ACK to the sender
// Parameters: 
// 		const char *data
// 		char *senddata
// 		const bt_config_t *config
// 		hashList *downloadingHashes
// 		struct sockaddr_in *addr
// 		unsigned int seqNum
// Returns: 
// 		int: ackNum
//
unsigned int generateResponseToDATA(const char *data, char *senddata, 
									const bt_config_t *config,
									hashList *downloadingHashes, 
									struct sockaddr_in *addr, 
									unsigned int seqNum, 
									unsigned int datalen);

//
// generateResponseToACK: sends next data if correct acknowledgement is
// 						  received, otherwise sends lost
// Parameters:
// 		const char *data
// 		char *senddata
// 		const bt_config_t *config
// 		hashList *uploadingHashes
// 		struct sockaddr_in *addr
// 		unsigned int ackNum
// Returns:
// 		int SUCCESS or FAILURE
//
int generateResponseToACK(const char *data, char *senddata,
						  const bt_config_t *config, hashList *uploadingHashes,
						  struct sockaddr_in *addr, unsigned int ackNum);

//
// generateResponseToWHOHAS: checks to see if anyone has data in chunk file
// Parameters:
//      const char *data:
//      char *senddata
//      const bt_config_t *config:
// Returns:
//      int: num of hits
//
int generateResponseToWHOHAS(const char *data, char *senddata, 
							 const bt_config_t *config);

//
// process_inbound_udp: calls respective functions depending upon
// 						the type of message received and then sends
// 						appropriate responses based on GET, WHOHAS,
// 						IHAVE requests
// Parameters:
//      int sock:
//      bt_config_t *config: Command line args
//
void process_inbound_udp(int sock, const bt_config_t *config, 
						 hashList *requestedHashes,
						 hashList *downloadingHashes,
						 hashList *uploadingHashes);

//
// process_get: processes a GET Request and sends WHOHAS with chunks 
// 				appropriately according to the size
// Parameters:
//      char *chunkfile:
//      char *outputfile:
//
void process_get(char *chunkfile, char *outputfile, 
				 const bt_config_t *config);

//
// handle_user_input: parses user input for function calls later
// Parameters:
//      char *line
//      void *cbdata
//
void handle_user_input(char *line, void *cbdata);

//
// peer_run: runs the peer
// Parameters:
//      bt_config_t *config: parsed command line args
//
void peer_run(bt_config_t *config);

#endif
