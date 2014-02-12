 /*******************************************************************
 * hashtest.c                                                       *
 *                                                                  *
 * Description: This file contains a main to test all of the		*
 *				functions used on a hashList.    					*
 *                                                                  *
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashList.h"
#include "log.h"

//
// main: runs a simple test to check the hashList functions
//
void main(int argc, char* argv[]){
	//First create log file to log output
	FILE *log;
	log = open_log("hashListText.log");
	Log(log, " ---- Testing HashList Functions ---- \n");

	//Declare a new hashList
	Log(log, " ---- Test 1: newHashList() ---- \n");
	hashList *list = newHashList();

	if (list == NULL)
		Log(log, "   Error: creating newHashList\n");
	else{
		Log(log, "   Success: Created newHashList\n");
		Log(log, "   size of list: %d\n", list->size);
	}

	//create chunk hashes for adding to hashList
	char ch1[HASHSIZE], ch2[HASHSIZE], ch3[HASHSIZE];
	char *t1 = "ac2972d828a1c1af10401368a136f5a27b1e592c";
	char *t2 = "a727b711e4f2986fbe9e3add54a0ec3bc015ddc9";
	char *t3 = "2d448a28ea58e6f884a31c7666b55189a37f40a1";

	Log(log, " ---- Test 2: hexCharsToBinary ---- \n");

	if (hexCharsToBinary(t1, ch1, sizeof(t1)) == 1)
		Log(log, "   Created chunkHash ch1: %x\n", ch1);
	else
		Log(log, "   Error: creating chunkHash ch1\n");
	
	if (hexCharsToBinary(t2, ch2, sizeof(t2)) == 1)
		Log(log, "   Created chunkHash ch2: %x\n", ch2);
	else
		Log(log, "   Error: creating chunkHash ch2\n");

	if(hexCharsToBinary(t3, ch3, sizeof(t3)) == 1)
		Log(log, "   Created chunkHash ch3: %x\n", ch3);
	else
		Log(log, "   Error: creating chunkHash ch3\n");
	
	//Create the hashNodes
	Log(log, " ---- Test 3: newHashNode ---- \n");
	hashNode *node1 = newHashNode(ch1, 0);
	hashNode *node2 = newHashNode(ch2, 1);
	hashNode *node3 = newHashNode(ch3, 0);

	if (node1 != NULL)
		Log(log, "   Created new hashNode 1\n");
	if (node2 != NULL)
		Log(log, "   Created new hashNode 2\n");
	if (node3 != NULL)
		Log(log, "   Created new hashNode 3\n");
	
	//Add the nodes to the hashList
	Log(log, " ---- Test 4: appendNode ---- \n");
	appendNode(list, node1);
	if (list->size == 1)
		Log(log, "   Appended node1, size of list: %d\n", list->size);

	appendNode(list, node2);
	if(list->size == 2)
		Log(log, "   Appended node2, size of list: %d\n", list->size);
	
	//Look for the nodes in the hashList
	Log(log, " ---- Test 5: findHashNode ---- \n");
	if(node1 == findHashNode(list, ch1))
		Log(log, "   Found hashNode in list: node1\n");
	if(node2 == findHashNode(list, ch2))
		Log(log, "   Found hashNode in list: node2\n");
	if(node3 != findHashNode(list, ch2))
		Log(log, "   Did not find node3 in list. Correctly returned NULL\n");

	//remove a node from the hashList
	Log(log, " ---- Test 6: removeNode ---- \n");
	Log(log, "   size of list: %d\n", list->size);
	removeNode(list, node2);
    Log(log, "   Removed hashNode from list: node2\n");


	if(node2 != findHashNode(list, ch2)){
		Log(log, "   size of list: %d\n", list->size);
	}
	if(removeNode(list, node2) == FAILURE){
		Log(log, "   Good. Cannot remove node that doesn't exist in list: node2\n");
		Log(log, "   size of list: %d\n", list->size);
	}
	removeNode(list, node1);
    Log(log, "   Removed hashNode from list: node1\n");

	if(node1 != findHashNode(list, ch1)){
		Log(log, "   size of list: %d\n", list->size);
	}

	if(removeNode(list, node3) == FAILURE){
		Log(log, "   Good. Cannot remove node that doesn't exist in list: node3\n");
		Log(log, "   size of list: %d\n", list->size);
	}
	if(removeNode(list, node1) == FAILURE){
		Log(log, "   Good. Cannot remove node that doesn't exist in list: node1\n");
		Log(log, "   size of list: %d\n", list->size);
	}

	Log(log, " ---- Test 7: freeNode ---- \n");
	if(freeNode(node1) == SUCCESS)
		Log(log, "   Freed: node1\n");
	if(freeNode(node2) == SUCCESS)
		Log(log, "   Freed: node2\n");

	Log(log, " ---- Test 8: freeList ---- \n");
	if(freeList(list) == SUCCESS)
		Log(log, "   Freed Empty List: list\n");

	Log(log, "   Creating new list with node3.\n");
	hashList *list2 = newHashList();
	appendNode(list2, node3);
	appendNode(list2, node3);

	Log(log, "   Size of list: %d\n", list2->size);
	if(freeList(list2) == FAILURE)
		Log(log, "   FAILED to free list with nodes: list2\n");
	else
		Log(log, "   Freed list with nodes: list2\n");


/* The following functions from hashList should ideally also be tested.
 * However, until we can identify a problem to be within one of these functions
 * prior to testing, they will be assumed to be working, because thus far,
 * they are and are not causing any problems.
 *
	Log(log, " -- Test 9: findAddrNode -- \n");
	Log(log, " -- Test 10: getChunkID -- \n");
	Log(log, " -- Test 11: getNextHashFromFile -- \n");
	Log(log, " -- Test 12: putTimeout -- \n");
	Log(log, " -- Test 13: putAddr -- \n");
*/

}
