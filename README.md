1. Setup:
		To compile the server, you will need to have gcc version 4.8 or higher because the server depends on the c++11 compilation flag.

2. Executing the Myhttpd server:
		Once you have logged on to the timberlake server, execute the following commands without quotes in the terminal:
 			Run 'make clean' in the terminal to clean up your environment.
			Run 'make' to create an executable file for the server.
			Run './myhttpd' to run the server.

			Note: If port 8080 is already bound on your computer, pass in the option -p followed by a alternate port number when running the server
			For example:  './myhttpd -p 5050' will run the server on port 5050

3. Testing:
		The server is now running and can be connected with through various options such as Telnet or your Internet browser of choice.

4. Sample queries:
		HTML webpage request:
			using browser: localhost:{port number}/index.html
			Using telnet: GET index.html HTTP/1.0

		JPEG image request:
			using browser: localhost:{port number}/kosar.jpg
			Using telnet: GET kosar.jpg HTTP/1.0			

		HEAD request:
			Using telnet: HEAD kosar.jpg HTTP/1.0		

		Directory request (returns index.html if available):
			using browser: localhost:{port number}/folder/
			Using telnet: GET folder/ HTTP/1.0								
