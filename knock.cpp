#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "option_handler.h"
#define DEFAULT_BUFF_SIZE 160000

using namespace std;

/*!
* @brief: This program creates a socket and makes a http GET request
* to a given host address from a given port. Host and Port inputs are
* required. In addition the user can optionally specify a webpath (-w) 
* and a file to write output to (-f). The program will return 0 if 
* executed or will return a negative value is there was an error.
* For usage: (./knock -H) or (./knock -?)
*/
int main(int argc, char *argv[])
{
    //**Input Handler**//
    OptionHandler::Handler h = OptionHandler::Handler(argc, argv);
    //required options should be contained in a try-catch
    try {
        h.add_option('h', "host",  OptionHandler::REQUIRED, true);
        h.add_option('p', "port",  OptionHandler::REQUIRED, true);
        h.add_option('H', "help", OptionHandler::NONE, true);
        h.add_option('?', "help", OptionHandler::NONE, true);  
    }
    catch (const std::exception & e) {
        cout<< "ERROR: Creating input requirements";
    }
    h.add_option('w', "web", OptionHandler::OPTIONAL, true);
    h.add_option('f', "file", OptionHandler::OPTIONAL, true);

    // get_option returns whether an option was set
    bool help = h.get_option("help");

    // get_argument returns a string passed to an option
    std::string host  = h.get_argument("host"),
                port  = h.get_argument("port"),
                web   = h.get_argument("web"),
                file  = h.get_argument("file");
    
    //** Help argument **// 
    if(help){
        cout << "usage: ./knock -h host -p port [-H] [-w] [-f file]\n";
        return(0);
    }
    //Input check
    if(!h.get_option("host")||!h.get_option("host")){
        cout<<"Missing requiredl host or port input\nusage: ./knock -h host -p port [-H] [-w] [-f file]\n";
        return(0);
    }
    //** END OF Input Handler**//

    //1. Create socket
    struct protoent *pr = getprotobyname("tcp"); //TCP protocol number
    int sockfd = socket(AF_INET , SOCK_STREAM, pr->p_proto); //socket descriptor, an integer (like a file-handle)
    if(sockfd < 0){ 
        printf("\nERROR:\tSocket creation error \n");
        return (-1);
    }

    //2. Create server
    const char *host_cstr = host.c_str();
    int portNum = stoi(port);
    struct hostent *server = gethostbyname(host_cstr);
    if (server == NULL){
      	printf("\nERROR:\t Invalid host input\n");
        return (-2);
    } 

    //3. Setup the server address
    struct sockaddr_in server_addr,cli_addr;    //address structures
    memset(&server_addr, '0', sizeof(server_addr));    // clear address structure
    server_addr.sin_family = AF_INET;    // server byte order
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);  
    server_addr.sin_port = htons(portNum);    // convert short integer value for port must be converted into network byte order

    //4. Connect socket to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("\nERROR:\tConnection Failed \n");
        return (-3);
    }

    //5. Create the HTTP GET Request
    char request[DEFAULT_BUFF_SIZE]={0};
    if(h.get_option("web")){    //has web path
        const char *web_cstr = web.c_str();
        sprintf(request,"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n",web_cstr,host_cstr);
    }
    else{
        sprintf(request,"GET / HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n",host_cstr);
    }
    
    //6. Send the get request
    if(send(sockfd,request,strlen(request),0)<0){
        printf("\nERROR:\tSending GET Request Failed \n");
        return (-4);
    }

    
    //7. Read responce
    char buffer[DEFAULT_BUFF_SIZE] = {0};
    ofstream output_file;
    ssize_t message_size = recv(sockfd, buffer, sizeof(buffer)-1, 0);
    if(h.get_option("file")){
        cout<<"Responce is written to "<<file<<".\n";
        output_file.open (file);
    }
    else{
        cout<<"Responce:\n";
    }
    while(message_size>0){
        //Null terminate buffer
	    buffer[message_size] = '\0';
        if(h.get_option("file")){  //Write responce to file
            output_file<<buffer;
        }
        else{   //Print responce
            cout<<"\n"<<buffer<<"\n";	
        }
        message_size = recv(sockfd, buffer, sizeof(buffer)-1, 0); 
    }
    output_file.close();

    //8. Close socket
    close(sockfd);
    return(0); 
}

