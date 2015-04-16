
#include "HashTableVoid.h"
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";
// #include "HashTableVoid.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <cstdlib>
// #include <hash_map>
#include <map>

using namespace std;



#include "IRCServer.h"



int QueueLength = 5;
map<string, string> user_pw;
map<string, set<string> > room_user;
map<string, vector<string> > room_msg;

int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void split(char *src, const char *separator, char **dest, int *num)
{
    char *pNext;
    int count = 0; 
    if (src == NULL || strlen(src) == 0) return;
    if (separator == NULL || strlen(separator) == 0) return; 
    pNext = strtok(src,separator);
    while(pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL,separator);
    }
    *num = count;
}

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	char *dest[128];
	int num = 0;
	split(commandLine," ",dest,&num);

	// split(commandLine," ",dest);
	const char * command = dest[0];
	const char * user = dest[1];
	const char * password = dest[2];
	const char * args = "";
	if (num > 3) {
		args = dest[3];
	} 
	if (num > 4) {
		string s = string(dest[3]);
		for (int i = 4; i < num; i++ ){
			s = s+" "+string(dest[i]);
		}
		args = s.c_str();
	}



	// const char * command = "ADD-USER";
	// const char * user = "peter";
	// const char * password = "spider";
	// const char * args = "";

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	// HashTableVoid();
	// Open password file
	
	// FILE* fp;
	// fp = fopen(PASSWORD_FILE,"w");

	// Initialize users in room


	// Initalize message list

}
	
bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	// Here check the password
	// const char * userPw =  user_pw[user].c_str();
	map<string, string>::iterator i = user_pw.find(user);
	if (i == user_pw.end())
		return false;
	else {
		if (strcmp(user_pw[user].c_str(), password) == 0)
			return true;
		else
			return false;
	}
}

// Request: ADD-USER <USER> <PASSWD>\r\n
// Answer: OK\r\n or DENIED\r\n
void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	// Here add a new user. For now always return OK.

	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));
	user_pw[user] = password;

	return;		
}

// Request: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
// Answer: OK\n or DENIED\r\n
void
IRCServer::createRoom(int fd, const char * user, const char * password, const char * args)
{
	// printf("------------------------\n");
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";
		set<string> ur;
		// ur.insert(user);
		room_user[args] = ur;
		vector<string> message;
		room_msg[args] = message;
		const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

// Request: LIST-ROOMS <USER> <PASSWD>\r\n
// Answer: room1\r\n
//         room2\r\n
//         ...
//         \r\n
//         or
//         DENIED\r\n
// string msg =  "";
// 			string s = "\r\n";

// 			vector<string>::iterator itr = ur.begin();
// 			for (; itr != ur.end(); itr++ ){
// 				msg = *itr + s;
// 				write(fd, msg.c_str(), strlen(msg.c_str()));
// 			}
void
IRCServer::listRoom(int fd, const char * user, const char * password, const char * args) {
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";
		map<string, set<string> >::iterator i = room_user.begin();
		string msg = "";
		for ( ;i != room_user.end(); i++) {
			
			msg = i->first + "\r\n";
			write(fd, msg.c_str(), strlen(msg.c_str()));
		}

	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";

		map<string, set<string> >::iterator i = room_user.find(args);
		if (i == room_user.end()) {
			const char * msg =  "ERROR (No room)\r\n";
			write(fd, msg, strlen(msg));
		} else {
			set<string> ur = i->second;
			ur.insert(user);
			room_user[args] = ur;
			const char * msg =  "OK\r\n";
			write(fd, msg, strlen(msg));
		}
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}


}

void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";

		map<string, set<string> >::iterator i = room_user.find(args);
		if (i == room_user.end()) {
			const char * msg =  "DENIED\r\n";
			write(fd, msg, strlen(msg));
		} else {
			set<string> ur = i->second;
			// ur.push_back(user);

			// vector<string>::iterator itr = ur.begin();
			// for (; itr != ur.end(); itr++ ){
			// 	if ((*itr).compare(user) == 0) {
			// 		ur.erase(itr);
			// 		break;
			// 	}
			// }
			if (ur.count(user) == 0) {
				const char * msg =  "ERROR (No user in room)\r\n";
				write(fd, msg, strlen(msg));
			} else {
				ur.erase(user);
				room_user[args] = ur;
				const char * msg =  "OK\r\n";
				write(fd, msg, strlen(msg));
			}
		}
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

// Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
// Answer: OK\n or DENIED\n
void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{
	// char *dest[10];
	// int num = 0;
	// char *tmp;
	// strcpy(tmp, args);
	// // cout << tmp << endl;
	// split(tmp," ",dest,&num);
	string s = string(args);
	string room = s.substr(0, s.find(" "));
	string mesg = s.substr(s.find(" ")+1);
	// cout << dest[0] << dest[1] <<endl;
	
	// string mesg = string(dest[0]);
	// string room = string(dest[1]);
	cout << mesg << room << endl;

	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";
		// map<string, map<string, string> >::iterator i = room_msg.find(dest[1]);
		map<string, set<string> >::iterator i_user = room_user.find(room);

		map<string, vector<string> >::iterator i = room_msg.find(room);
		// cout << i->first << endl;
		if (i == room_msg.end()) {
			const char * message =  "DENIED\r\n";
			write(fd, message, strlen(message));
		} else {
			int flag = 0;
			set<string> ur = i_user->second;
			// for ( ; ur != i_user->second.end(); ur++) {
			// 	// cout << *ur << endl;
			// 	if ((*ur).compare(user) == 0) {
			// 		flag = 1;
			// 		break;
			// 	}
			// }
			if (ur.count(user) == 0) {
				const char * message =  "ERROR (user not in room)\r\n";
				write(fd, message, strlen(message));
			} else {
				vector<string> msg = i->second;
				// cout << mesg << endl;
				string temp = string(user)+" "+mesg;
				// cout << dest[0] << " ---------- " << dest[1] <<endl;
				msg.push_back(temp);
				// cout << temp << endl;
				
				room_msg[room] = msg;
				// cout << user << msg[user] << room << endl;
				const char * message =  "OK\r\n";
				write(fd, message, strlen(message));
			}
		}

		// printf("------------------------\n");
		
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * message =  "ERROR (Wrong password)\r\n";
		write(fd, message, strlen(message));
	}
}

void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args)
{
	// char *dest[10];
	// int num = 0;
	// char *tmp;
	// strcpy(tmp, args);
	// split(tmp," ",dest,&num);
	string s = string(args);
	string number = s.substr(0, s.find(" "));
	string room = s.substr(s.find(" ")+1);

	// string number = string(dest[0]);
	// string room = string(dest[1]);
	// cout << number +" "+ room << endl;

	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";
		map<string, vector<string> >::iterator i = room_msg.find(room);
		map<string, set<string> >::iterator i_user = room_user.find(room);
		// cout << i->first << "--------" << number + " " + room << endl;
		if (i == room_msg.end()) {
			const char * msg =  "ERROR (No room)\r\n";
			write(fd, msg, strlen(msg));
		} else {

			set<string> ur = i_user->second;
			if (ur.count(user) == 0) {
				const char * message =  "ERROR (User not in room)\r\n";
				write(fd, message, strlen(message));
			} else {
				vector<string> msg = i->second;
				string message =  "";
				string s = "\r\n";
				int times = atoi(number.c_str());
				if (times >= msg.size()) {
					const char * message =  "NO-NEW-MESSAGES\r\n";
					write(fd, message, strlen(message));
				} else {
					if (msg[times].find(user) == msg[times].npos) {
						vector<string>::iterator itr = msg.end();
						itr--;
						
						// cout << times << endl;
						for (int i = 0; itr != msg.begin(); itr--, i++) {
							if (i == (times+1)) {
								break;
							}
							char temp[4]; 
							sprintf(temp, "%d ", (i+1));

							write(fd, temp, strlen(temp));
							message = *itr + s;
							// cout << message << endl;
							write(fd, message.c_str(), strlen(message.c_str()));
						}
						write(fd, s.c_str(), strlen(s.c_str()));
					} else {

						for(int i = times+1; i < msg.size(); i++) {
							char temp[4]; 
							sprintf(temp, "%d ", i);
							write(fd, temp, strlen(temp));

							message = msg[i] + s;
							write(fd, message.c_str(), strlen(message.c_str()));
						}
						write(fd, s.c_str(), strlen(s.c_str()));
					}
				}			
			}
			// }
		}
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * message =  "ERROR (Wrong password)\r\n";
		write(fd, message, strlen(message));
	}
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";


		map<string, set<string> >::iterator i = room_user.find(args);
		if (i == room_user.end()) {
			const char * msg =  "ERROR (No room)\r\n";
			write(fd, msg, strlen(msg));
		} else {
			set<string> ur = i->second;
			// ur.push_back(user);
			string msg =  "";
			string s = "\r\n";

			set<string>::iterator itr = ur.begin();
			for (; itr != ur.end(); itr++ ){
				msg = *itr + s;
				write(fd, msg.c_str(), strlen(msg.c_str()));
			}
			write(fd, s.c_str(), strlen(s.c_str()));
			
		}
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{
	if (checkPassword(fd, user, password) == true) {
		// const char * msg =  "password correct!\r\n";
		string msg =  "";
		string s = "\r\n";
		map<string, string>::iterator i = user_pw.begin();
		for ( ; i != user_pw.end(); i++){
			msg = i->first.c_str()+ s;
			write(fd, msg.c_str(), strlen(msg.c_str()));
		}
		write(fd, s.c_str(), strlen(s.c_str()));
			
	} else {
		// const char * msg =  "password not correct!\r\n";
		const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
	}
}

