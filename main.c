
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct customers{
    char name[21];
    int TimeAfterOpen;
    char stylist[21];
    int LoyaltyPt;
    int TimeItTook;
    int leavingtime;
}customers;
typedef struct heap{
    customers* node;
    int capacity;
    int currentSize;
}heap;

heap* initHeap(int i);
void percolateUp(heap* Ptr, int index);
void insert(heap* Ptr, customers* item);
int compare(customers a, customers b);
void swap(customers* a, customers* b);
int isStylist(char NameStylist[21], char arrStylist[][21], int Numstylist);
int findstylist(heap* ptr, customers* line);
void MergeArray(customers* arr, int start, int mid, int end);
void MergeSort(customers* arr, int start, int end);
customers removeMax(heap* ptr);

int main(){
    int NumCustomers, Numstylist;
    char arrStylist[10][21];
    scanf("%d %d", &NumCustomers, &Numstylist);
    for(int i=0; i<=Numstylist; i++)
    {
        scanf("%s", &arrStylist[i][21]);

    }
    heap* ptr[10];
    for(int j=0; j<Numstylist; j++)
    {
        ptr[j] = initHeap(10);
    }
    for(int k=0; k<NumCustomers; k++)
    {
        customers* line = malloc(sizeof(customers));
        scanf("%d %s %s %d %d", &line->TimeAfterOpen, line->name , line->stylist, &line->LoyaltyPt, &line->TimeItTook  );
        //case of there being no preference
        if(strcmp(line->stylist, "NONE")==0 || isStylist(line->stylist,arrStylist,Numstylist)){
            int i, index=0;
            int smallest=100001;
            for(i=0; i<Numstylist; i++)
            {
                index=findstylist(ptr[i], line);
                if(index<smallest)
                {
                    smallest=index;
                    index=i;
                }
            }
            strcpy(line->stylist, arrStylist[index]);
            insert(ptr[index], line);
        }
        customers* Customers = malloc(sizeof(customers) * NumCustomers);
        int index = 0;
        for(int i = 0; i < Numstylist; i++){
            do{
                customers tmp = removeMax(ptr[i]);
                if(tmp.LoyaltyPt == -1){
                    //freeHeap(stylistHeap[i]);
                    break;
                }
                Customers[index] = tmp;
                index++;
                
            }while(1);
        }
        
        MergeSort(Customers, 0, NumCustomers-1);
        
        for(int i = 0; i < NumCustomers; i++)
            printf("%s %d %d %s\n", Customers->name, Customers->leavingtime, Customers->LoyaltyPt, Customers->stylist);

       // free(allCustomers);
        
          
        
    }
    return 0;
}
int findstylist(heap* ptr, customers* line)
{
    int count=0, i=0;
    for(i=1; i<ptr->currentSize; i++)
    {
        if(ptr->node[i].leavingtime < line->TimeAfterOpen)
            count ++;
        else
            break;
    }
    return ptr->currentSize - count;
    
}
int isStylist(char NameStylist[21], char arrStylist[][21], int Numstylist)
{
    int i;
    for(i=0; i<Numstylist; i++)
    {
        if(strcmp(NameStylist, &arrStylist[i][21])==0)
            return 1;
    }
    return 0;
}
heap* initHeap(int i){
    heap* myHeap = (heap*)malloc(sizeof(heap));
    myHeap->node=(customers*)malloc(sizeof(customers)*10);
    myHeap->capacity = i;
    myHeap->currentSize = 0;
    return myHeap;
}
void insert(heap* ptr, customers* item) {

    if(ptr->currentSize == ptr->capacity){
        ptr->node=realloc(ptr->node, sizeof(customers*)* (2*ptr->capacity+1));
        ptr->capacity *=2 ;
    }
    
    
    ptr->currentSize++;
    //insert at bottom
    ptr->node[ptr->currentSize] = *item;

    if(ptr->currentSize == 1)
    {
        ptr->node[ptr->currentSize].leavingtime = ptr->node[ptr->currentSize].TimeItTook + ptr->node[ptr->currentSize].TimeAfterOpen;

    }
    else {
        for(int i=2; i<=ptr->currentSize; i++)
        {
            ptr->node->leavingtime=ptr->node[i/2].leavingtime+ptr->node[i].TimeItTook;
        }
    }
    percolateUp(ptr, ptr->currentSize);
}
void percolateUp(heap* Ptr, int index)
{

    if (index == 1) return;

    int pIndex = index/2;

    if(Ptr->node[index].TimeAfterOpen < Ptr->node[pIndex].leavingtime)
    {
        int point=0;
        point=compare(Ptr->node[index/2], Ptr->node[index]);
        if(point<0 && (index/2 != 1)){
            
            swap(&Ptr->node[index],&Ptr->node[index/2] ) ;
            
            
            
            percolateUp(Ptr, index/2);
        }
    }
    
}

int compare(customers a, customers b) {
    if (a.LoyaltyPt != b.LoyaltyPt)
        return a.LoyaltyPt - b.LoyaltyPt;
    return strcmp(a.name, b.name);
    
}
void swap(customers* a, customers* b)
{
    customers* temp=NULL;
    *temp=*a;
    *b=*temp;
    *a=*b;
}
void percolateDown(heap* Ptr, int index) {
    
    int top;
    if ((2*index+1) <= Ptr->currentSize) {

   
        if(Ptr->node[2*index].LoyaltyPt > Ptr->node[2*index+1].LoyaltyPt){
            top= 2*index;
        }
        else if(Ptr->node[2*index].LoyaltyPt < Ptr->node[2*index+1].LoyaltyPt){
            top=2*index+1 ;
        }
        else{
            if(strcmp(Ptr->node[2*index].stylist, Ptr->node[2*index+1].stylist)>0)
                top=2*index;
            else
                top=2*index+1;
        }
          int value = compare(Ptr->node[index], Ptr->node[top]);
        if (value < 0) {
            swap(&Ptr->node[index],&Ptr->node[top]) ;
            
            
            percolateDown(Ptr, top);
        }
    }

    else if (Ptr->currentSize == 2*index) {

    
        int value = compare(Ptr->node[index], Ptr->node[2*index]);
        if (value < 0){
            swap(&Ptr->node[index],&Ptr->node[ 2*index]);
        }
    }
}
customers removeMax(heap* ptr) {

    customers retval;

    if (ptr->currentSize > 0) {
        retval = ptr->node[1];
        ptr->node[1] = ptr->node[ptr->currentSize];

        ptr->currentSize--;
        percolateDown(ptr, 1);
        retval.LoyaltyPt += retval.TimeItTook / 10;
        int i;
        for(i = 2; i <= ptr->currentSize; i++){
            ptr->node[i].leavingtime = ptr->node[i/2].leavingtime + ptr->node[i].TimeItTook;
        }
        return retval;
    }

    // No value to return, indicate failure with a -1.
    else{
        customers n;
        n.LoyaltyPt = -1;
        return n;
    }
}
void MergeSort(customers* arr, int start, int end)
{
  // making sure there are more than one items to sort
  if(start<end)
  {
    int mid;
    // find the mid value
    mid=(start+end)/2;
    // sort the right side of array
    MergeSort(arr, start, mid);
    // sort the left side of array
    MergeSort(arr,mid+1, end);
    // put everything in one array
    MergeArray(arr, start, mid+1,end);

  }
}
// sort the array using merge sort
void MergeArray(customers* arr, int start, int mid, int end)
{
  // finding the length of the array
  int length=end-start+1;
  // allocating enough space for the array
  customers *temp=(customers*)malloc(length*sizeof(customers));
  //index to the sorted arrays
  int i=start;
  int j=mid;
  //this is the index number of the temp array
  int index=0;
  // copying the sorted values into temp array
  while((i<mid) || (j<=end))
  {
    // filling up the first half of the array
    if(j>end || (i<mid && arr[i].leavingtime<arr[j].leavingtime))
    {
      temp[index]=arr[i];
      i++;
      index++;
    }
        
    // filling up the second half of the array
    else
    {
      temp[index]=arr[j];
      j++;
      index++;
    }
  }
  //copy everthing into the main array
  for(int k=start; k<=end; k++)
  {
    arr[k]=temp[k-start];
  }
  // free the memory
  free(temp);
}
