/*
** selectserver.c -- a cheezy multiperson chat server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "linked_list.h"
#define PORT "9035"   // port we're listening on
#define MAXDATASIZE 100 // max number of bytes we can get at once


void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void num_to_string(int n, char s[]) {
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void prepare_info_for_client(Node* head, char* res)
{
  Node* current_node = head;
  char temp [MAXDATASIZE];

	while(current_node){
    strcat(temp, current_node->ip);
    strcat(temp , "|");
		strcat(temp, current_node->port);
    strcat(temp , "|");
		current_node = current_node->next;
	}
  strncpy(res, temp, strlen(temp)-1);

}
void separate_received_info(char rec_info[MAXDATASIZE] , Node** head)
{
  int cnt = 0;
  int index = 0;
  char file_name[MAXDATASIZE], partition_number[MAXDATASIZE], ip[MAXDATASIZE], port_number[MAXDATASIZE];
  while(rec_info[cnt] != '|')
  {
    file_name[index] = rec_info[cnt];
    cnt += 1;
    index += 1;
  }
  file_name[index] = '\0';
  cnt += 1;
  index = 0;
  while(rec_info[cnt] != '|')
  {

    partition_number[index] = rec_info[cnt];
    cnt += 1;
    index += 1;
  }
  partition_number[index] = '\0';

  cnt += 1;
  index = 0;
  while(rec_info[cnt] != '|')
  {
    ip[index] = rec_info[cnt];
    cnt += 1;
    index += 1;
  }
  ip[index] = '\0';
  cnt += 1;
  index = 0;
  while(rec_info[cnt] !=  '\0')
  {
    port_number[index] = rec_info[cnt];
    cnt += 1;
    index += 1;
  }
  port_number[index] = '\0';
  push_to_list(head , file_name , partition_number , ip , port_number);
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
}
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char *argv[])
{
    Node* head = NULL;

    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
int fdmax;

int listener;
int newfd;
struct sockaddr_storage remoteaddr; // client address
socklen_t addrlen;
char buf[256];    // buffer for client data
int nbytes;
char remoteIP[INET6_ADDRSTRLEN];
int yes=1;        // for setsockopt() SO_REUSEADDR, below
int i, j, rv;
struct addrinfo hints, *ai, *p;
FD_ZERO(&master);    // clear the master and temp sets
FD_ZERO(&read_fds);
// get us a socket and bind it
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
if ((rv = getaddrinfo(NULL, argv[1], &hints, &ai)) != 0) {
    //fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
    write(1 , "Fail to Connect to Server...Please Try Again\n" , strlen("Fail to Connect to Server...Please Try Again\n"));
    exit(1);
}
for(p = ai; p != NULL; p = p->ai_next) {
    listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (listener < 0) {
continue; }
    // lose the pesky "address already in use" error message
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
        close(listener);
        continue; }
break; }
// if we got here, it means we didn't get bound
if (p == NULL) {
    //fprintf(stderr, "selectserver: failed to bind\n");
    write(1 , "SelectServer: Failed to Bind\n", strlen("SelectServer: Failed to Bind\n"));
exit(2); }
freeaddrinfo(ai); // all done with this
// listen
if (listen(listener, 10) == -1) {
    //perror("listen");
    write(1 , "Listening" ,strlen("Listening") );
exit(3); }
// add the listener to the master set
FD_SET(listener, &master);
// keep track of the biggest file descriptor
fdmax = listener; // so far, it's this one
// main loop
for(;;) {
    read_fds = master; // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
        //perror("select");
        write(1 , "Select" , strlen("Select"));
exit(4); }
    // run through the existing connections looking for data to read
    for(i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &read_fds)) { // we got one!!
            if (i == listener) {
                // handle new connections
                addrlen = sizeof remoteaddr;
                newfd = accept(listener,
                    (struct sockaddr *)&remoteaddr,
                    &addrlen);
                if (newfd == -1) {
                    //perror("accept");
                    write(1 , "Accept" , strlen("Accept"));
                } else {
                    FD_SET(newfd, &master); // add to master set
                    if (newfd > fdmax) {    // keep track of the max
                        fdmax = newfd;
                    }
                    write(1 , "New Connection From:" , strlen("New Connection From:"));
                    write(1 , inet_ntop(remoteaddr.ss_family,
                    get_in_addr((struct sockaddr*)&remoteaddr),
                    remoteIP, INET6_ADDRSTRLEN) , strlen(inet_ntop(remoteaddr.ss_family,
                    get_in_addr((struct sockaddr*)&remoteaddr),
                    remoteIP, INET6_ADDRSTRLEN)));
                    write(1 , " On Socket:::" , strlen(" On Socket:::"));
                    char ss[MAXDATASIZE];
                    num_to_string(newfd , ss);
                    write(1 ,ss , strlen(ss));
                    write(1 , "\n" , strlen("\n"));}
                    // printf("selectserver: new connection from %s on "
                    //     "socket %d\n",
                    //     inet_ntop(remoteaddr.ss_family,
                    //         get_in_addr((struct sockaddr*)&remoteaddr),
                    //         remoteIP, INET6_ADDRSTRLEN),newfd); }
            } else {
                // handle data from a client
                if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                    // got error or connection closed by client
                    if (nbytes == 0) {
                        // connection closed
                        //write(1,buf,MAXDATASIZE);
                        //write(1,"salamz",7);
                        //printf("selectserver: socket %d hung up\n", i);
                        write(1 , "Socket:::" , strlen("Socket:::"));
                        char ss[MAXDATASIZE];
                        num_to_string(i , ss);
                        write(1 , ss , strlen(ss));
                        write(1 , " Hung Up\n" , strlen(" Hung Up\n"));
                        } else {
                            //perror("recv");
                            write(1 , "Error in Receiving\n" , strlen("Error in Receiving\n"));
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client

                        // char res[MAXDATASIZE];
                        // memset(res, '\0', MAXDATASIZE);
                        // handle(res, buf);
                        // send(i, res, MAXDATASIZE);
                        //write(1,"sexual",MAXDATASIZE);

                        // for (size_t i = 0; i < strlen(buf); i++) {
                        //    printf("%d -> %c\n", buf[i], buf[i]);
                        //  }
                        if(buf[0] == 'S')
                        {
                          //write(1 , "Server" , 7);
                          char received_info[MAXDATASIZE];
                          strcpy(received_info , &buf[2]);
                          //char file_name[MAXDATASIZE], partition_number[MAXDATASIZE], ip[MAXDATASIZE], port_number[MAXDATASIZE];
                          separate_received_info(received_info, &head);
                          write(1 , "Packet Received by DataBase\n" , strlen("Packet Received by DataBase\n"));

                          //display_list(head);
                          //write(1 , received_info , strlen(received_info));
                        }
                        else
                        {
                          //write(1 , "Client" , 7);
                          // char received_info[MAXDATASIZE];
                          // strcpy(received_info , &buf[2]);
                          char res[MAXDATASIZE];
                          //printf("Request From Client is Received\n");
                          prepare_info_for_client(head , res);
                          write(1 , "Packet Sent to Client\n" , strlen("Packet Sent to Client\n"));
                          //printf("info:[%s]\n" ,res );
                          send(i,res,MAXDATASIZE , 0);
                          //write(1 , received_info , strlen(received_info));
                        }


                        //write(1,buf,strlen(buf));
                        //write(1,"." , 2);
                        //write(1,"salam",6);

                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
return 0; }
