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
//#define PORT "9035" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
   return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int get_data_from_database(char ip [MAXDATASIZE],char port[MAXDATASIZE], int file_fd)
{
  //printf("%s\n",sending_info );
  // write(1,"hajaqasalamonalykom",MAXDATASIZE);
  // write(1,sending_info,MAXDATASIZE);
  //printf("IP=[%s]...Port=[%s]" , ip , port);
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
//   if (argc != 5) {
//       fprintf(stderr,"usage: client hostname\n");
//       exit(1);
// }
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
      //fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      write(1 , "GetAddrInfo: " , strlen("GetAddrInfo: "));
      write( 1 , gai_strerror(rv) , strlen(gai_strerror(rv)));
      write( 1 , "\n", strlen("\n"));
      return 1;
}
  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
              p->ai_protocol)) == -1) {
          //perror("client: socket");
          write(1 , "Client: Socket\n" , strlen("Client: Socket\n"));
continue; }
      if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
          close(sockfd);
          //perror("client: connect");
          write( 1, "Client: Connect\n" , strlen("Client: Connect\n"));
continue; }
break; }
  if (p == NULL) {
      //fprintf(stderr, "client: failed to connect\n");
      write( 1 , "Client: Failed to Connect\n" , strlen("Client: Failed to Connect\n"));
      return 2;
}
  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
          s, sizeof s);
  //printf("client: connecting to %s\n", s);
  write(1 , "Client Connecting to: " , strlen("Client Connecting to: "));
  write(1 , s , strlen(s));
  write(1 , "\n" , strlen("\n"));
  freeaddrinfo(servinfo); // all done with this structure
  //write(1,"salamon",6);
  //write(1,sending_info,MAXDATASIZE);
  if ((numbytes = send(sockfd, "FileRequest", MAXDATASIZE-1, 0)) == -1) {
      //perror("recv");
      write( 1 , "Recv\n" , strlen("Recv\n"));
exit(1); }
// while(recv(sockfd, buf,MAXDATASIZE-1, 0)){
//   if(strcmp(buf , "end") == 0)
//   {
//     memset(buf , '\0' , MAXDATASIZE);
//     break;
//
//   }
// printf("DATAREC=[%s]\n" , buf);
// // int file_fd = open("out.txt", O_CREAT | O_APPEND |O_TRUNC| O_WRONLY, S_IRWXU);
// write(file_fd , buf , strlen(buf));
// memset(buf , '\0' , MAXDATASIZE);}
//write(1 , buf , strlen(buf));
write(1 , ">>>>>>>>>>>>>>>>>Downloading Packet From Port: " , strlen(">>>>>>>>>>>>>>>>>Downloading Packet From Port: "));
write( 1, port , strlen(port));
write( 1 , "\n" , strlen("\n"));
while(1)
{
  recv(sockfd, buf,MAXDATASIZE-1, 0);
  //if(strcmp(buf + strlen(buf) - 3 , "end") == 0)
  if(strcmp(buf , "end") == 0)
    break;

  //printf(">>>>>%s\n", buf);

  write(file_fd , buf , strlen(buf));
  memset(buf , '\0' , MAXDATASIZE);

}
// recv(sockfd, buf,MAXDATASIZE-1, 0);
// write(file_fd , buf , strlen(buf));
// memset(buf , '\0' , MAXDATASIZE);

  //printf("We are out of WHILE\n");
  buf[numbytes] = '\0';
  //printf("client: Has Sent '%s'\n",sending_info);
  close(sockfd);
  return 0;
}

int main(int argc, char *argv[])
{
    char temp_name[MAXDATASIZE], f_name [MAXDATASIZE];
    memset(f_name, '\0', MAXDATASIZE);
    strcpy(f_name , "C");
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



char * prt ;
char* ipvv4;
  //printf ("Splitting string \"%s\" into tokens:\n",str);
  int file_fd = open("out.txt", O_CREAT | O_APPEND |O_TRUNC| O_WRONLY, S_IRWXU);
  ipvv4 = strtok (buf,"|");
  prt = strtok (NULL, "|");
  //printf("going to database1\n");
  int a = get_data_from_database(ipvv4, prt , file_fd);
  //printf("goingbackfromdatabase1\n");
  while (prt != NULL)
  {
    ipvv4 = strtok (NULL,"|");
    prt = strtok (NULL, "|");
    if(prt != NULL)
    {
      get_data_from_database(ipvv4, prt , file_fd);
    }
    //get_data_from_database(pch);
    //printf("%s\n", pch);
  }


    buf[numbytes] = '\0';
    //printf("client: received '%s'\n",buf);
    close(sockfd);
    write(1 , "Client Has Completely Downloaded File\n" , strlen("Client Has Completely Downloaded File\n"));
return 0; }
