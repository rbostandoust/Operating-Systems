#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAXDATASIZE 100


typedef struct Node{
	struct Node* next;
	char file_name [MAXDATASIZE];
	char partition_number[MAXDATASIZE];
	char ip[MAXDATASIZE];
	char port[MAXDATASIZE];
} Node;

void push_to_list(Node** head, char f[MAXDATASIZE] , char part[MAXDATASIZE], char ipv4[MAXDATASIZE], char port_number[MAXDATASIZE]);
void display_list(Node* head);
#endif
