/*
** client.c -- a stream socket client demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#define PORT "9035" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
// get sockaddr, IPv4 or IPv6:
using namespace std;
void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
   return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{

    char temp_name[MAXDATASIZE], f_name [MAXDATASIZE];
    memset(f_name, '\0', MAXDATASIZE);
    char input[100];
    cout<<"Please Enter Your Plate:";
    cin>>input;
    strcpy(f_name , input);
    cout <<"FNAME="<<f_name<<endl;
    //write(1, "Please Enter Your File Name: ",30);
    //read(0,temp_name , MAXDATASIZE);
    //strncat(f_name , temp_name , strlen(temp_name)-1);
    //write(1, f_name , strlen(f_name));
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    if (argc != 3) {
        //fprintf(stderr,"usage: client hostname\n");
        write(1 , "Not Enough Parameters on Input....Please Try Again\n", strlen("Not Enough Parameters on Input....Please Try Again\n"));
        exit(1);
}
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        write(1 , "Fail to Connect to Main Server...Please Try Again\n" , strlen("Fail to Connect to Main Server...Please Try Again\n"));
        return 1;
}
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            //perror("client: socket");
            write( 1 , "Client Socket\n" , strlen("Client Socket\n"));
continue; }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            //perror("client: connect");
            write( 1 , "Client: Connect\n" , strlen("Client: Connect\n"));
continue; }
break; }
    if (p == NULL) {
        //fprintf(stderr, "client: failed to connect\n");
        write(1 , "Client: Failed to Connect\n", strlen("Client: Failed to Connect\n"));
        return 2;
}
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    //printf("client: connecting to %s\n", s);
    //write(1 , "Client Connecting to " , strlen("Client Connecting to "));
    write( 1, s , strlen(s));
    write( 1 , "\n" , strlen("\n"));
    freeaddrinfo(servinfo); // all done with this structure
//     if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
//         perror("recv");
// exit(1); }
//   send(sockfd , "CADFALL96" , MAXDATASIZE - 1, 0);
//   write(1 , "RECEIVED:" , MAXDATASIZE);
//   write(1 , buf , strlen(buf));
if ((numbytes = send(sockfd, f_name, MAXDATASIZE-1, 0)) == -1) {
    //perror("recv");
    write(1 , "Recv\n" , strlen("Recv\n"));
exit(1); }
recv(sockfd, buf,MAXDATASIZE-1, 0);
//write(1 , buf , strlen(buf));
//printf("Data Received: [%s]\n" , buf);
cout <<"RESULT="<<buf<<endl;


    buf[numbytes] = '\0';
    //printf("client: received '%s'\n",buf);
    close(sockfd);
    write(1 , "Client Has Completely Downloaded File\n" , strlen("Client Has Completely Downloaded File\n"));
return 0; }
