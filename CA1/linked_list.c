
#include "linked_list.h"

void push_to_list(Node** head, char f[MAXDATASIZE] , char part[MAXDATASIZE], char ipv4[MAXDATASIZE], char port_number[MAXDATASIZE])
{
  Node* new_node = (Node *)malloc(sizeof(Node));
	strcpy(new_node->file_name , f);
  strcpy(new_node->partition_number , part);
  strcpy(new_node->ip , ipv4);
  strcpy(new_node->port , port_number);
  //printf("partition_number = %s" ,new_node->file_name );
	if(!(*head)){
		new_node->next = NULL;
		(*head) = new_node;
    //printf("We are in head: ");
    //printf("%s\n" , (*head)->partition_number);
    return;
	}
  Node* current_node = *head;
  //if(current_node->next == NULL)
  //{
    if(atoi(current_node->partition_number) > atoi(new_node->partition_number))
      {
        //printf("We are in special case\n");
        (*head) = new_node;
        new_node->next = current_node;
        //current_node->next = NULL;

        return;
      }
  //}
  while(current_node->next != NULL){
		//printf("Our Current Node part:[%s]\n", current_node->partition_number);
    if(atoi(current_node->next->partition_number) > atoi(new_node->partition_number))
    {
      new_node->next = current_node->next;
      current_node->next = new_node;
      return;
    }
		current_node = current_node->next;
	}
  //printf("added to end");
  current_node->next = new_node;
  new_node->next = NULL;

}
void display_list(Node* head)
{
  Node* current_node = head;
	//printf("Linked List:\n");
	while(current_node){
		printf("[%s]-->", current_node->file_name);
    //write(1  , current_node->partition_number , strlen(current_node->partition_number));
		current_node = current_node->next;
	}
	printf("NULL\n");
}
