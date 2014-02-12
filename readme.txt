# README.TXT
# This file contains a brief description of the design for Bittorrent,
# a file transfer application with congestion control.

*** Checkpoint 1: WHOHAS flooding and IHAVE responses ***

	The beginning functionality of our BitTorrent is set up to do the following:
		- Listen for a new request on one socket
		- If receive a WHOHAS, search through hashList for a matching hash
		- If receive user input, send a WHOHAS for every hash in the file
		- If the buffer fills to maximum capacity of 1500, start a new packet

*** Checkpoint 2: Final Submission ***

	The final implementation of the BitTorrent-like file transfer application
	allows for reliable transfer between two peers. We used a hashNodeList
	to track the nodes that are being transferred/requested. Our log functions
	allow for easy debugging of the methods and functions. The packet functions
	reliably fill/create/read&parse packet headers.

	In another iteration of this project, congestion control would be implemented.
	To do so, we would start with a window_size of 1. Every time we transmit a 
	packet and receive an ACK, the window_size would be increased by 1, effectively
	implementing SlowStart. The window_size would continue to increase by 1 for each
	ACK until a LOST is detected (so, when we receive 3 ACKS for the same packet) OR
	until we surpass the SlowStartThreshold. If we lose the packet during SlowStart,
	we reset the window_size to max(currentwindowsize/2, 2). Otherwise, we begin
	CongestionAvoidance. During CongestionAvoidance, we continue to increase the
	congestion_window by 1 packet for every RTT.
