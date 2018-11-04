//LRU Cache Implementation using hash map and a doubly linked list 

// T table for Pearson hashing from RFC 3074.
#define SIZE 256
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//Currently cache is set to 4 elements
//counter is used to count till 4 (Max cache size used here)
int counter=0;

unsigned char T[256] = {

    251, 175, 119, 215, 81, 14, 79, 191, 103, 49, 181, 143, 186, 157,  0,
    232, 31, 32, 55, 60, 152, 58, 17, 237, 174, 70, 160, 144, 220, 90, 57,
    223, 59,  3, 18, 140, 111, 166, 203, 196, 134, 243, 124, 95, 222, 179,
    197, 65, 180, 48, 36, 15, 107, 46, 233, 130, 165, 30, 123, 161, 209, 23,
    97, 16, 40, 91, 219, 61, 100, 10, 210, 109, 250, 127, 22, 138, 29, 108,
    244, 67, 207,  9, 178, 204, 74, 98, 126, 249, 167, 116, 34, 77, 193,
    200, 121,  5, 20, 113, 71, 35, 128, 13, 182, 94, 25, 226, 227, 199, 75,
    27, 41, 245, 230, 224, 43, 225, 177, 26, 155, 150, 212, 142, 218, 115,
    241, 73, 88, 105, 39, 114, 62, 255, 192, 201, 145, 214, 168, 158, 221,
    148, 154, 122, 12, 84, 82, 163, 44, 139, 228, 236, 205, 242, 217, 11,
    187, 146, 159, 64, 86, 239, 195, 42, 106, 198, 118, 112, 184, 172, 87,
    2, 173, 117, 176, 229, 247, 253, 137, 185, 99, 164, 102, 147, 45, 66,
    231, 52, 141, 211, 194, 206, 246, 238, 56, 110, 78, 248, 63, 240, 189,
    93, 92, 51, 53, 183, 19, 171, 72, 50, 33, 104, 101, 69, 8, 252, 83, 120,
    76, 135, 85, 54, 202, 125, 188, 213, 96, 235, 136, 208, 162, 129, 190,
    132, 156, 38, 47, 1, 7, 254, 24, 4, 216, 131, 89, 21, 28, 133, 37, 153,
    149, 80, 170, 68, 6, 169, 234, 151
};
//Data struct contains the hasharray
//and a pointer is included which points to its location in the cache if present
struct Data
{
int flag;
int key;
char data[100];

struct Cache *ptr;
}*item,*hasharray[SIZE];

//struct Cache holds the cached elements of Data struct(Upto 4 elements).
struct Cache
{
    char data[100];
    int key;
    struct Cache * next;
    struct Cache * prev;

}*head=NULL,*tail=NULL,*temp;

// Pearsong hashing algorithm as described in RFC 3074.
// -> http://www.apps.ietf.org/rfc/rfc3074.html
unsigned char phash_rfc3074(const char *key) {

    size_t len = strlen(key);
    unsigned char hash = len;
    for (size_t i = len; i > 0;) hash = T[hash ^ key[--i]];
    return hash;

}

//Pushing at the front of a doubly linked list
//here the doubly linked list is done on Cache struct
void PushFront(char data[100])
{
   temp = (struct Cache*)malloc(sizeof(struct Cache)); 

    if(head == NULL && tail==NULL)
    {
        temp->next=NULL;
        temp->prev=NULL;
        strncpy(temp->data,data,100);
        head=temp;
        tail=temp;
    }  
    else
    {
        head->prev=temp;
        temp->next=head;
        temp->prev=NULL;
        strncpy(temp->data,data,100);
        head=temp;
    }
}

//Helps insert into the doubly linked list Cache after being called
void insertHashHelper(char data[100],int hashindex)
{
    struct Data *item=(struct Data*)malloc(sizeof(struct Data));
    strncpy(item->data,data,100);
    item->key=hashindex;
    hasharray[hashindex]=item;    
    PushFront(data);
    hasharray[hashindex]->ptr=head;
    hasharray[hashindex]->ptr->key=hashindex;
    hasharray[hashindex]->flag=1;
    
    //checking if Cache is empty
    if(counter<4)
    {   
        counter++;
        
    }
    //Cache is full case
    else if(counter==4)
    {   
        struct Data *temp=(struct Data*)malloc(sizeof(struct Data));
        hasharray[tail->key]->flag=0;
        tail=tail->prev;
        tail->next=NULL;
        
    }
}
//Creating a hashmap and inserting into the Cache if entry is not present in Cache
void insertHash(char data[100],int hashindex)
{

    if(hasharray[hashindex]) //element hash exists but not sure if it is there in the Cache
    {   
       
        if(hasharray[hashindex]->flag==1) //confirms the element existence in the cache
        {
           
            struct Cache* temp1 = (struct Cache*)malloc(sizeof(struct Cache));
            struct Cache* temp2 = (struct Cache*)malloc(sizeof(struct Cache));
            struct Cache* temp3 = (struct Cache*)malloc(sizeof(struct Cache));

            if(head==hasharray[hashindex]->ptr) //present in cache at the head so do nothing
            {
               
            }
            else if (tail==hasharray[hashindex]->ptr) //present in the cache in the tail so swap with head
            {
                
                temp1=tail->prev;
                temp1->next=NULL;
                temp2=head->next;
                tail->next=head;
                head->prev=tail;
                head=tail;
                head->prev=NULL;
                tail=temp1;
                tail->next=NULL;

            }
            else //present somewhere in cache but not in head/tail. so move to the head position
            {
                
                temp2=hasharray[hashindex]->ptr->next;
                temp1=hasharray[hashindex]->ptr->prev;
                temp1->next=temp2;
                temp2->prev=temp1;
                temp3=head;
                head->prev=hasharray[hashindex]->ptr;
                head=hasharray[hashindex]->ptr;
                head->next=temp3;
                head->prev=NULL;
            }

        }
        
        else //element not in cache but hashmap entry exists
        {
            insertHashHelper(data,hashindex);
        }
    }
    else //element not in cache and not in hashmap
    {
        insertHashHelper(data,hashindex);
    
    }
}

//Display all elements in cache, with top element having highest priority
void DisplayAllFromFront()
{
    if(head==NULL&&tail==NULL)
    {
        printf("Can't display an empty List\n");
    }
    else
    {
        struct Cache* temp = (struct Cache*)malloc(sizeof(struct Cache)); 
        temp=head;

        while(temp!=NULL)
        {
            printf("   %s\n",temp->data);
            temp=temp->next;
        }
    }
}

int main()
{

    struct Data *hasharray =calloc(256,sizeof(struct Data));
    char string[100];int hash;
    //repeatedly input values, hashit, enter it in cache
    while(1)
    {
    printf("Enter string :");
    scanf("%s",string);

    hash=phash_rfc3074(string);

    insertHash(string,hash);
 
    DisplayAllFromFront();
    }

    return 1;
}
