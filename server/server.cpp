// C++ program to show the example of server application in
// socket programming

//first we include the header files we need for our code
#include <tchar.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){

// creating a socket variable of type socket 
SOCKET serverSocket = INVALID_SOCKET;
// we create another socket to carry the return value of the accept function since it creates another socket for the connection
SOCKET acceptSocket;
// the port number the we are going to connect through
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

// calling the socket function which is responsible for creating the socket and pass the three params 
// to define which protocol to be used
serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
// here we check for errors like before if the socket is invalid we print the error responsible for that
// and wsa cleanup free the dll for other programs so we won't take resources that we don't use
if (serverSocket == INVALID_SOCKET){
    cout << "error at socket(): " << WSAGetLastError() << endl;
    WSACleanup();
    return 0;
}
else{
    cout << "socket() id ok"<<endl;
}


/////////Bind the socket

// creating a socket address variable of type sockaddr_in 
sockaddr_in service;
//assigning values to the variable we created
service.sin_family = AF_INET ;   //the family of tcp and udp protocols
// define the ip address for the s_addr attribute in the sin_addr parameter in our service
InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);  
//service.sin_addr.s_addr = htonl(INADDR_ANY);
// adding the port number to our service using htons which put our port in the right order of the TCP network 
service.sin_port = htons(port);
// we call the bind function and pass the required arguments to it the serverSocket is the socket we created in the previous step
// and we pass the service which is of type socketaddr_in and we cast it to be of type SOCKADDR 
// and we call the function in an if statement to check for errors as this fuction returns zero if it runs successfully and 
// return SOCKET_ERROR if there was a problem
//and if there is a problem we cleanup and close the socket to free any allocated resources for other programs
if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
    cout << "bind() failed: " << WSAGetLastError() << endl;
    closesocket(serverSocket);
    WSACleanup();
    return 0;

}
else{
    cout << "bind() is ok" <<endl;
}


/////////Listen to the socket

// the listen function is simple takes two params the socket to listen on and how many clients it can accept
// again we added it to an if statement to check and return any errors that occurs
if (listen(serverSocket,1)== SOCKET_ERROR){
    cout << "listen(): error listening on socket" <<WSAGetLastError()<< endl;
}
else{
    cout << "listen() is ok, i'm waiting for connections..." << endl;
}


/////////Accepting connections

// here we call our function and specify the parameters the second and third params are used in case we want to connect
// to certain clients
acceptSocket = accept(serverSocket, NULL, NULL);
// here we check for errors as before and if there is any we cleanup the socket and free the resources
if (acceptSocket == INVALID_SOCKET){
    cout<< "accept failed: " << WSAGetLastError()<< endl;
    WSACleanup();
    return -1;
}
cout << "accepted connection" << endl;



/////////Recieve data

// createing the variable that will hold the message we will recieve and the value is the size of this message
char recieveBuffer[200] = "";
// calling the send function and getting the returned data which is the bytes sent and storing this data in a variable
// notice we pass the accept socket not the server socket and that is because the accept socket is the socket connected to the client
// the server socket is still listening for other clients
int byteCount = recv(acceptSocket, recieveBuffer, 200, 0);
//check if the return value of the recv function equals zero then there is a problem
// and if it returned the bytes recieved we will print that
if(byteCount <0){
    printf("client recieve error %ld.\n",WSAGetLastError());
    return 0;
}
else{
    printf("recieved data %s bytes \n",recieveBuffer);
}


/////////send confirmation
char confirmationBuffer[200] = "message received";
byteCount = send(acceptSocket, confirmationBuffer, 200, 0);

if(byteCount >0){
    cout << "confirmation message sent: "<< endl;
}
else{
    WSACleanup();

}
/////////Close socket
system("pause");
WSACleanup();
}
// note that in order for this file to compile the editor or (vscode) should load the ws2_32 library to connect to the dll
// responsible for the socket program so either you include this library in the tasks.json
// or you should provide the library directly when you compile by adding this -lws2_32
// so the command will look like this (g++ server.cpp -o server.exe -lws2_32)
