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
#include <sys/stat.h>
#define MAXDATASIZE 100 // max number of bytes we can get at once

long get_file_size(int file_fd) {
    struct stat bufz;
    fstat(file_fd, &bufz);
    return bufz.st_size;
}

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
void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
   return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int Send_File_Info_to_Main_Server(int argc, char *argv[], char sending_info[MAXDATASIZE], char* PORT)
{
  //printf("%s\n",sending_info );
  // write(1,"hajaqasalamonalykom",MAXDATASIZE);
  // write(1,sending_info,MAXDATASIZE);
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  if (argc != 5) {
      //fprintf(stderr,"usage: client hostname\n");
      write(1 , "Not Enough Parameters on Input....Please Try Again\n", strlen("Not Enough Parameters on Input....Please Try Again\n"));
      exit(1);
}
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
      write(1 , "Fail to Connect to Main Server...Please Try Again\n" , strlen("Fail to Connect to Main Server...Please Try Again\n"));
      return 1;
}
  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
              p->ai_protocol)) == -1) {
          //perror("client: socket");
          write(1 ,"Client: Socket\n" , strlen("Client: Socket\n"));
continue; }
      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          //perror("client: connect");
          write(1 ,"Client: Connect\n" , strlen("Client: Connect\n"));
continue; }
break; }
  if (p == NULL) {
      //fprintf(stderr, "client: failed to connect\n");
      write( 1 , "Client Failed to Connect\n" , strlen("Client Failed to Connect\n"));
      return 2;
}
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
          s, sizeof s);
  //printf("client: connecting to %s\n", s);
  write(1 , "DataBase Connecting to: " , strlen("DataBase Connecting to: "));
  write( 1, s , strlen(s));
  write(1 , "\n" , strlen("\n"));
  freeaddrinfo(servinfo); // all done with this structure
  //write(1,"salamon",6);
  //write(1,sending_info,MAXDATASIZE);
  if ((numbytes = send(sockfd, sending_info, MAXDATASIZE-1, 0)) == -1) {
      //perror("recv");
      write( 1, "Recv\n" , strlen("Recv\n"));
exit(1); }
//recv(sockfd, buf,MAXDATASIZE-1, 0);
//write(1 , buf , strlen(buf));


  buf[numbytes] = '\0';
  //printf("client: Has Sent '%s'\n",sending_info);
  write( 1  , "DataBase Has Sent File Info to Main Server\n" , strlen("DataBase Has Sent File Info to Main Server\n"));
  close(sockfd);
  return 0;
}
void Concat_Info(char* file_name,char* partition_number,char* ip,char* port_number, char* concat)
{
  strncat(concat, file_name, strlen(file_name)-1);
  strcat(concat, "|");
  strncat(concat, partition_number, strlen(partition_number)-1);
  strcat(concat, "|");
  strncat(concat, ip, strlen(ip));
  strcat(concat, "|");
  strncat(concat, port_number, strlen(port_number));
}

int listen_to_client(int argc, char* argv[] ,char temp[MAXDATASIZE] , char f_name [MAXDATASIZE])
{

  // write(1,"listen to client",MAXDATASIZE);
  // write(1,temp,MAXDATASIZE);
  // write(1,"Done Listen to Client",MAXDATASIZE);
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
int fdmax;
// maximum file descriptor number
// listening socket descriptor
// newly accept()ed socket descriptor
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

//printf("Server_Port:%s\n" , argv[4]);
if ((rv = getaddrinfo(NULL, argv[4], &hints, &ai)) != 0) {
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
exit(2); }
freeaddrinfo(ai); // all done with this
// listen
if (listen(listener, 10) == -1) {
    //perror("listen");
    write(1 , "Listen\n" , strlen("Listen\n"));
exit(3); }
// add the listener to the master set
FD_SET(listener, &master);
// keep track of the biggest file descriptor
fdmax = listener; // so far, it's this one
// main loop
for(;;) {
    read_fds = master; // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      write(1 , "Select\n" , strlen("Select\n"));

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
                    write( 1  , "Accept\n" , strlen("Accept\n"));
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
                    write(1 , "\n" , strlen("\n")); }
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
                            write(1 , "Recv\n" , strlen("Recv\n"));
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        // we got some data from a client
                        //write(1,buf,MAXDATASIZE);
                        //[Forwarded from Sadegh Hayeri]
                        int FILE_FD = open(f_name, O_RDONLY);
                        char context[MAXDATASIZE];
                        memset(context , '\0' , MAXDATASIZE);
                        //read(FILE_FD, context, MAXDATASIZE);
                        long f_size = get_file_size(FILE_FD);
                        int chunk_num = f_size / MAXDATASIZE + 1;

                        for(int cnt = 0;cnt<chunk_num;cnt++)
                        {
                          memset(context , '\0' , MAXDATASIZE);
                          read(FILE_FD, context, 100);
                          //printf("Context=[%s]\n\n",context);
                          send(i, context, strlen(context), 0);

                        }
                        sleep(1);
                        send(i , "end" , strlen("end") , 0);
                        // while(read(FILE_FD, context, 100))
                        // {
                        //   //print("LENgth=[%d]\n" , length);
                        //   printf("context =[%s]\n" , context);
                        //   send(i, context, strlen(context), 0);
                        //   memset(context , '\0' , MAXDATASIZE);
                        // }
                        // printf("sending end\n");
                        // send(i , "end" , strlen("end") , 0);
                        //write(1,"salam",6);

                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
return 0; }
int main(int argc, char *argv[])
{
  char temp[MAXDATASIZE] , f_name [MAXDATASIZE];
  memset(temp, '\0', MAXDATASIZE);
  char file_name[MAXDATASIZE], partition_number[MAXDATASIZE], port_number[MAXDATASIZE];
  char concat[MAXDATASIZE];
  strcpy(concat ,"S|");//For Detecting Data was sent by Server
  write(1,"Please Enter File Name: ",25);
  read(0,file_name,MAXDATASIZE);
  write(1,"Please Enter Partition Number: ",32);
  read(0,partition_number,MAXDATASIZE);
  Concat_Info(file_name, partition_number, argv[3]/*Server ip*/, argv[4]/*Server Port*/, concat);
  Send_File_Info_to_Main_Server(argc, argv, concat , argv[2]);
  strncat(temp, port_number, strlen(port_number)-1);
  strncat(f_name, file_name, strlen(file_name)-1);
  //printf("Goingtolistenin\n");
  write(1 , ">>>>>>>>>>>>>>>>>>>Listening<<<<<<<<<<<<<<<<<\n" , strlen(">>>>>>>>>>>>>>>>>>>Listening<<<<<<<<<<<<<<<<<\n"));
  int func_res = listen_to_client(argc , argv,temp , f_name);
}
