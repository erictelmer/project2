/********************************************************************
 * orderedListTest.c									            *
 *                                                                  *
 * Description: This file contains a main to test all of the        *
 *              functions used on an orderedList.                   *
 *                                                                  *
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "orderedList.h"
#include "log.h"

#define LOST -1
#define OK 1

//
// main: runs a simple test to check the orderedList functions
//
int main(){
  //First create log file to log output
  FILE *log;
  log = open_log("orderedListTest.log");
  Log(log, " ---- Testing OrderedList Functions ---- \n");

  //Declare a new orderedNode
  Log(log, " ---- Test 1: NewOrderedNode ---- \n");
  int testNum = 42;
  orderedNode *node = newOrderedNode(testNum);
  time_t testTime;
  int seq;
  sleep(1);
  time(&testTime);

  if(node == NULL)
    Log(log, "   Bad. Node is null\n");
  if(node->num == testNum)
    Log(log, "   Good. Created OrderedNode\n");

  if(difftime(node->timeSent, testTime) < 0)
    Log(log, "   Good: difftime=%f\n", difftime(node->timeSent, testTime));
  else
    Log(log, "   Bad. Error in creating newOrderedNode\n");

  Log(log, " ---- Test 2: freeOrderedNode ---- \n");
  freeOrderedNode(node);
  Log(log, "   Good. Freed node\n");

  //Declare a new orderedList
  Log(log, " ---- Test 3: NewOrderedList ---- \n");
  orderedList *ordList = newOrderedList();
  if((ordList->size == 0) && (ordList->start == NULL))
    Log(log, "   Good. Created OrderedList\n");
  else
    Log(log, "   Bad. Did not create new OrderedList\n");

  Log(log, " ---- Test 4: isOListEmpty ---- \n");
  int i = isOListEmpty(ordList);
  if (i == 1)
    Log(log, "   Good. isOListEmpty is correct\n");

  Log(log, " ---- Test 5: freeOrderedList ---- \n");
  freeOrderedList(ordList);
  Log(log, "   Good. Freed ordList\n");

  //Declare a new downloadList
  Log(log, " ---- Test 6: newDownloadList ---- \n");
  downloadList *downList = newDownloadList();
  if((downList->maxRecv == 0) && (downList->missing != NULL))
    Log(log, "   Good. Created newDownloadList\n");
  else
    Log(log, "   Bad. Did not create newDownloadList\n");

  Log(log, " ---- Test 7: freeDownloadList ---- \n");
  freeDownloadList(downList);
  Log(log, "    Good. Freed downloadList\n");

  //Declare a new uploadList
  Log(log, " ---- Test 8: newUploadList ---- \n");
  uploadList *upList = newUploadList();
  if((upList->dup == 0) && (upList->sent != NULL))
    Log(log, "    Good. Created newUploadList\n");
  else
    Log(log, "    Bad. Did not create newUploadList\n");

  Log(log, " ---- Test 9: freeUploadList ---- \n");
  freeUploadList(upList);
  Log(log, "    Good. Freed uploadList\n");

  Log(log, " ---- Test 10: addNum ---- \n");
  ordList = newOrderedList();
  //[0]
  Log(log, "   Adding num 0 to list\n");
  addNum(ordList, 0);
  Log(log, "   ordList size: %d\n", ordList->size);
  if(ordList->start != NULL){
    Log(log, "   ordList startNum: %d\n", ordList->start->num);
    Log(log, "   Good. addNum to an ordList\n");
  }
  else
    Log(log, "   Bad. Error in addNum\n");

  //[0,2]
  Log(log, "   Adding num 2 to list\n");
  addNum(ordList, 2);
  if(ordList->size == 2){
    Log(log, "   ordList size: %d\n", ordList->size);
    Log(log, "   ordered list: %d, %d\n", ordList->start->num, ordList->start->next->num);
  }
  else{
    Log(log, "   Bad. Error in addNum\n");
  }

  //test same num doesnt change list
  addNum(ordList, 2);
  if(ordList->size == 2){
    Log(log, "   Good. Adding same num doesn't affect size\n");
  }
  else{
    Log(log, "    Bad. Error in addNum\n");
  }

  //test if middle number inserts properly
  //[0,1,2]
  addNum(ordList, 1);
  if(ordList->size == 3){
    Log(log, "   Good. ordList has the correct size\n");
    Log(log, "   ordered list: %d ,%d, %d\n", ordList->start->num, ordList->start->next->num, ordList->start->next->next->num);
  }
  else{
    Log(log, "    Bad. Error in addNum\n");
  }

  //Test removing num
  //[0,2]
  Log(log, " ---- Test 11: removeNum ---- \n");
  removeNum(ordList, 1);
  if (ordList->size == 2){
    Log(log, "   Good. removeNum has the correct size\n");
    Log(log, "   ordered list: %d, %d\n", ordList->start->num, ordList->start->next->num);
  }
  else{
    Log(log, "    Bad. Error in removeNum\n");
  }

  //Test removing a number not in the list
  //[0,2]
  removeNum(ordList, 60);
  if(ordList->size == 2){
    Log(log, "   Good. removeNum did not remove an invalid num\n");
    Log(log, "   ordered list: %d, %d\n", ordList->start->num, ordList->start->next->num);
  }
  else
    Log(log, "   Bad. Error in removeNum\n");

  //Test removing on an empty list
  removeNum(ordList, 0);
  removeNum(ordList, 2);
  if(ordList->size == 0)
    Log(log, "   Good. removeNum removed all numbers\n");

  removeNum(ordList, 0);
  if(ordList->size == 0)
    Log(log, "   Good. removeNum functions properly on empty list\n");
  freeOrderedList(ordList);

  //Test uploading ACK and DATA
  Log(log, " ---- Test 12 & Test 13: sentData & receivedACK ---- \n");
  upList = newUploadList();

  for (seq = 0; seq < 8; seq++){
    sentDATA(upList, seq);
  }

  if(receivedACK(upList, 0) == OK)
    Log(log, "   Good. Received ACK 0\n");
  else
    Log(log, "   Bad. Did not receive ACK 0\n");

  if(receivedACK(upList, 2) == OK)
    Log(log, "   Good. Received ACK 2\n");
  else
    Log(log, "   Bad. Did not receive ACK 2\n");

  if(receivedACK(upList, 2) == OK)
    Log(log, "   Good. Received ACK 2\n");
  else
    Log(log, "   Bad. Did not receive ACK 2\n");
 
  if(receivedACK(upList, 2) == OK)
    Log(log, "   Good. Received ACK 2\n");
  else
    Log(log, "   Bad. Did not receive ACK 2\n");

  if(receivedACK(upList, 2) == LOST)
    Log(log, "   Good. Received LOST on ACK 2\n");
  else
    Log(log, "   Bad. Did not receive LOST on ACK 2\n");

  if(receivedACK(upList, 5) == OK)
    Log(log, "   Good. Received ACK 5\n");
  else
    Log(log, "   Bad. Did not receive ACK 5\n");

  if(upList->sent->size == 2)
    Log(log, "   Good. uploadList size: 2\n");
  else
    Log(log, "   Bad. uploadList has incorrect size\n");

  if(upList->sent->start->num == 6)
    Log(log, "   Good. uploadList has correct start num\n");
  else
    Log(log, "   Bad. uploadList has incorrect start num\n");
	
  freeUploadList(upList);

  //Test downloadList
  Log(log, " ---- Test 14: receivedData ---- \n");
  downList = newDownloadList();

  if(receivedDATA(downList, 0) == 0)
    Log(log, "   Good. downloadList received 0\n");
  else
    Log(log, "   Bad. downloadList did not receive 0\n");

  if(receivedDATA(downList, 1) == 1)
    Log(log, "   Good. downloadList received 1\n");
  else
    Log(log, "   Bad. downloadList did not receive 1\n");

  if(receivedDATA(downList, 2) == 2)
    Log(log, "   Good. downloadList received 2\n");
  else
    Log(log, "   Bad. downloadList did not receive 2\n");

  //Testing out of order packets
  if(receivedDATA(downList, 4) == 2)
    Log(log, "   Good. downloadList received 0\n");
  else
    Log(log, "   Bad. downloadList did not receive 0\n");

  if(receivedDATA(downList, 5) == 2)
    Log(log, "   Good. downloadList received 5\n");
  else
    Log(log, "   Bad. downloadList did not receive 5\n");

  if(receivedDATA(downList, 3) == 5)
    Log(log, "   Good. downloadList received 3\n");
  else
    Log(log, "   Bad. downloadList did not receive 3\n");

  //Testing random order behavior
  if(receivedDATA(downList, 6) == 6)
    Log(log, "   Good. downloadList received 6\n");
  else
    Log(log, "   Bad. downloadList did not receive 6\n");

  if(receivedDATA(downList, 10) == 6)
    Log(log, "   Good. downloadList received 10\n");
  else
    Log(log, "   Bad. downloadList did not receive 10\n");

  if(receivedDATA(downList, 7) == 7)
    Log(log, "   Good. downloadList received 7\n");
  else
    Log(log, "   Bad. downloadList did not receive 7\n");

  if(receivedDATA(downList, 8) == 8)
    Log(log, "   Good. downloadList received 8\n");
  else
    Log(log, "   Bad. downloadList did not receive 8\n");

  if(receivedDATA(downList, 11) == 8)
    Log(log, "   Good. downloadList received 11\n");
  else
    Log(log, "   Bad. downloadList did not receive 11\n");

  if(receivedDATA(downList, 9) == 11)
    Log(log, "   Good. downloadList received 9\n");
  else
    Log(log, "   Bad. downloadList did not receive 9\n");

  freeDownloadList(downList);
	

  //Testing getTimoutPacket
  upList = newUploadList();
  sentDATA(upList, 0);
  sleep(10);
  assert(getTimeoutPacket(upList, 5) == 0);

  return 0;
}

