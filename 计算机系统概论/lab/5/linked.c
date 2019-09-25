#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct node
{
	char firstname[20];
	char lastname[20];
	char roomnumber[20];
	struct node *next;
}dir;

dir *CreateList(int n)
{
	dir *head = NULL; 
	dir *end  = NULL;
	dir *p    = NULL;
	int i = 0;
	for(i = 0;i < n;i++)                            //enter a certain number of groups of information
	{
		if(head == NULL)                            //if the head is empty create head
		{
			head = (dir*)malloc(sizeof(dir));       
			gets(head->firstname);                  //in order to distinghuish blank and \n we use gets() instead
			gets(head->lastname);
			gets(head->roomnumber);
			head->next = NULL;
			p = end = head;                         // p == end == head
		}
		else                                        //if the head is not empty add new node into linkedlist
		{
			p->next = (dir*)malloc(sizeof(dir));
			p = p->next;
			gets(p->firstname);
			gets(p->lastname);
			gets(p->roomnumber);
			p->next = NULL;
			end = p;                                //end == p
		}
	} 
	return head;
}


void SearchList(char a[],int n,dir *list,int stat)
{
	dir *find[n+2];                                //the array that store the found node's address(pointer)
	int found = 0;
	int i;
	dir *p;
	for(p = list; p != NULL; p = p->next)
	{
		if(strcmp(a, p->firstname) == 0) {find[found] = p; found++;}    //if firstname or lastname are the same, put the address into array
		else if(strcmp(a, p->lastname) == 0) {find[found] = p; found++;}
	}
	if(found != 0)                                          //judge if '\n'is needed.
		printf("%d\n",found);                               //argument stat is a flag. if stat == 1, '\n'is needed, else we don't need it
	else if(found == 0 && stat != 1)                        // output the number of found node
		printf("%d\n",found);
	else if(found == 0 && stat == 1)
		printf("%d",found);        
	for(i = 0; i < found - 1; i++)
	{
		printf("%s ",find[i]->lastname);
		printf("%s ",find[i]->firstname);
		printf("%s\n",find[i]->roomnumber);
	}
	if(stat == 1 && found != 0)                           //discussion on the last output if '\n' is needed
	{
		printf("%s ",find[i]->lastname);
		printf("%s ",find[i]->firstname);
		printf("%s",find[i]->roomnumber);
	} 
	else if (stat != 1&& found !=0)
	{
		printf("%s ",find[i]->lastname);
		printf("%s ",find[i]->firstname);
		printf("%s\n",find[i]->roomnumber);
	} 

}
int main()
{
	int n;	//number of people
	int i;
	int time;
	char **p = NULL;
	dir *linkedlist;
	scanf("%d",&n);                                      //input the number of people
	getchar();                                           //avoid being affected by '\n'left in buffer
	linkedlist = CreateList(n);
	scanf("%d",&time);                                   //input the number of people we want to find 
	getchar();
	p = (char**)malloc(sizeof(char*)*time);
	for(i = 0; i < time; i++)
	{
		p[i] = (char*)malloc(sizeof(char)*20);          
		gets(p[i]);                                      //input the keyword
	}
	for(i = 0; i < time - 1; i++)
	{
		SearchList(p[i],n,linkedlist,0);
	}
	if(time != 0)
	{
		SearchList(p[i],n,linkedlist,1);                  //the last output, '\n' isn't needed,so stat == 1
	}
	return 0;
}
