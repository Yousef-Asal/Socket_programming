// C++ program to show the example of client application in
// socket programming

//first we include the header files we need for our code
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>
#include <tchar.h>

using namespace std;

int main(int argc, char* argv[]){

// creating a socket variable of type socket 
SOCKET clientSocket;
// the port number the we are going to connect through (should be the same as the server)
int port = 8080;

/////////Initialize WSA

// create an object of wsadata datatype
WSADATA wsaData;
// a variable to store the return value of the wsastartup function
int wsaerr ;
//concatenate the bits of these two numbers to generate the versioning for wsastartup
WORD wVersionRequested = MAKEWORD(2, 2);
// calls the wsa startup function which takes two arguments the first is the version of the wsa we want to run 
// and the second id the a refrence to the wsa data and that holds the required configurations and data to start the wsa
// WSAStartup returns 0 if it initialized fine and if it encountered an error it returns a number associated with that error 
// so we assigned the function to a variable to get the return value and check it
wsaerr = WSAStartup(wVersionRequested, &wsaData);
// here we check if the returned value is not equal to zero so the dll is not found and the wsa didn't initialize properly
// and if it returned zero so the dll is found and we print the status of the system
if(wsaerr !=0){
    cout << "the winsock dll not found" << endl;
    return 0 ;
}
else{
    cout << "the winsock dll found" << endl;
    cout << "the status:" << wsaData.szSystemStatus<<endl;
}


/////////Create Socket

// creating a socket variable of type socket 
clientSocket = INVALID_SOCKET;
// calling the socket function which is responsible for creating the socket and pass the three params 
// to define which protocol to be used
clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
// here we check for errors like before if the socket is invalid we print the error responsible for that
// and wsa cleanup free the dll for other programs so we won't take resources that we don't use
if (clientSocket == INVALID_SOCKET){
    cout << "error at socket(): " << WSAGetLastError() << endl;
    WSACleanup();
    return 0;
}
else{
    cout << "socket() id ok"<<endl;
}
/////////Connect to the socket

// creating a socket address variable of type sockaddr_in 
sockaddr_in clientService;
//assigning values to the variable we created
clientService.sin_family = AF_INET;    //the family of tcp and udp protocols
// define the ip address for the s_addr attribute in the sin_addr parameter in our service
InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);  
//clientService.sin_addr.s_addr = htonl(INADDR_ANY);
// adding the port number to our service using htons which put our port in the right order of the TCP network 
clientService.sin_port = htons(port);
// we call the connect function and pass the required arguments to it the clientSocket is the socket we created in the previous step
// and we pass the service which is of type socketaddr_in and we cast it to be of type SOCKADDR 
// and we call the function in an if statement to check for errors as this fuction returns zero if it runs successfully and 
// return SOCKET_ERROR if there was a problem
//and if there is a problem we cleanup and close the socket to free any allocated resources for other programs
if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR){
    cout << "connect() failed: " << endl;
    WSACleanup();
    return 0;

}
else{
    cout << "client: connect() is ok" <<endl;
}
}


// note that in order for this file to compile the editor or (vscode) should load the ws2_32 library to connect to the dll
// responsible for the socket program so either you include this library in the tasks.json
// or you should provide the library directly when you compile by adding this -lws2_32
// so the command will look like this (g++ server.cpp -o server.exe -lws2_32)