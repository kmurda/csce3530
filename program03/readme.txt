PROGRAM 03
CSCE 3530
INTRO TO COMPUTER NETWORKS

Authors:
Kyle Upton
Edmund Sannda

=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~=!~

1. INSTRUCTIONS
	
	a. On csce01 type "gcc server.c" followed by "./a.out"
	b. On csce02 type "gcc client.c" followed by "./a.out 49155"
		-the server is looking for connections on port 49155
	

2. PROBLEMS ENCOUNTERED
	Using the read() and write() functions proved to be more difficult, ultimately we chose to use send() and recv(),
	which handled the data types better in our experience.
	
	The tcp_segment fields are are correctly populated and the checksum function works correctly.
