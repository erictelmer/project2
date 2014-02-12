/****************************************************************
 *                                                              *
 *  log.c                                                       *
 *                                                              *
 *  Description: This file contains the description and methods *
 *  for creating a log file and writing to it.                  *
 *																															*
 *  Code reused from Lauren's Project 1													*
 *                                                              *
 ***************************************************************/

#include "hashList.h"
#include "log.h"
#include "peer.h"
#include <time.h>
#include <stdio.h>

int LogCreated = 0;

//
// open_log: function to open a new log file
// Parameters:
// 		path: path for file name
// Returns: 
// 		FILE *file: the logfile
//
FILE *open_log(const char *path){

	/*Check that we haven't already created the log*/
	if (LogCreated == 1){
		log = fopen(path, "a");
	} else{
		log = fopen(path, "w");
		LogCreated = 1;
	}

	if (log == NULL){
		fprintf(stdout, "Error creating logfile. \n");
		LogCreated = 0;
		exit(EXIT_FAILURE);
	}

	/*Set buffering for file to be new line*/
	setvbuf(log, NULL, _IOFBF, 0);

	fputs("Log file created at:\n", log);
	Log(log, "\n\n");
	return log;
}


//
// Log: function to write to a log file
//  		Uses a variable list in order to write information/settings
//  		to the log from clients
// Parameters:
// 		message: to write to the log file
// 		file : logfile to write to
// 	
void Log(FILE *log, const char *message, ...){
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "[%x %X] ", timeinfo);

	fprintf(log, buffer);
	fflush(log);
	va_list arg_point;
	va_start(arg_point, message);
	vfprintf(log, message, arg_point);
	va_end(arg_point);
}


//
// close_log: function to close the log file
// Parameters:
// 		file: the logfile
//
void close_log(FILE *log){
	Log(log, "Closing log file\n");
	fclose(log);
}

/* TODO TEST THESE FUNCTIONS
THESE FUNCTIONS ARE USED FOR CONGESTION CONTROL -- MAINLY PRINTING TO THE WINDOW_SIZE_FILE

//
// open_window: function to open a new window_size log file
// Parameters:
//    path: path for file name
// Returns:
//    FILE *file: the window_size file
//
FILE *open_window(const char *path){

    window_size_file = fopen(path, "w");

    if (window_size_file == NULL){
        fprintf(stdout, "Error creating window_size_file.\n");
    }

    setvbuf(window_size_file, NULL, _IOFBF, 0);
    return window_size_file;
}

// write_size: writes the current window size to the file
void write_size(time_t start_time, hashNode *node){
    //get time difference from list->start to node
    double ms = diff(start_time,  mktime(node->timeout)) * 1000;
    fprintf(window_size_file, "f%u\t%f\t%d\n", node->chunkOffset, ms , node->windowSize);
    fflush(window_size_file);
}
*/

