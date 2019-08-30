#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define DIM_INPUT 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/***********************************************TERMINAL-INTERFACE************************************************/

void stats(long int iterations,double timeTook){

  if(iterations==0)
    printf("\n<Algorithm statistics>\nArray length: %i\nExecution time: %f sec\n",DIM_INPUT,timeTook);
  else{
    printf("\n<Algorithm statistics>\n");
    printf("Array length: %i\nIterations: %li\nExecution time: %f sec\n",DIM_INPUT,iterations,timeTook);
  }
}

void printMenu(void){
  printf("\nScegliere l'algoritmo di ordinamento da utilizzare:\n\n");
  printf(ANSI_COLOR_YELLOW);
  printf("1]InsertionSort\n2]MergeSort\n3]QuickSort\n4]RandomizedQuickSort\n");
  printf("5]CountingSort\n6]RadixSort\n7]HeapSort\n");
  printf("0]Esci\n");
  printf(ANSI_COLOR_RESET);
}

void printArray(int input[],int n){
  printf(ANSI_COLOR_CYAN);
  for(int i=0;i<n;i++)
    printf("%i ",input[i]);
  printf("\n");
  printf(ANSI_COLOR_RESET);
}

/*********************************************HELP-METHODS******************************************************/
void swap(int input[DIM_INPUT],int i,int j){
  int tmp;
  tmp=input[i];
  input[i]=input[j];
  input[j]=tmp;
}

int greater(int a,int b){
  return a>b?a:b;
}

int biggest(int input[DIM_INPUT]){

  int biggest=0;

  for(int i=0;i<DIM_INPUT;i++){
    if(input[i]>=biggest)
      biggest=input[i];
  }
  return biggest;
}

void shuffle(int input[DIM_INPUT],int l,int h){

  srand(time(NULL));

  for(int i=l;i<=h;i++){
    int randomIndex=rand()%(i+1);
    swap(input,i,randomIndex);
  }
}

/*******************************************DATA-STRUCTURES***************************************************/

int parent(int i){
  return (i-1)/2;
}

int left(int i){
  return (2*i)+1;
}

int rigth(int i){
  return (2*i)+2;
}

int * maxHeapInsert(int *a,int n,int key){

  n=n+1;

  int *output=(int *)malloc(sizeof(int)*n);

  for(int i=0;i<n-1;i++)
    output[i]=a[i];

  output[n-1]=key;

  int i=n-1;
  while(i>0){
    if(output[parent(i)]<output[i]){
        swap(output,parent(i),i);
        i=parent(i);
    }
    else
        break;
  }

  return output;
}

void maxHeapify(int *a,int n,int index){
  int largest=index;
  int leftChild=left(index);
  int rigthChild=rigth(index);

  if(leftChild<n && a[leftChild]>a[largest])
    largest=leftChild;

  if(rigthChild<n && a[rigthChild]>a[largest])
    largest=rigthChild;

  if(largest!=index){
    swap(a,index,largest);
    maxHeapify(a,n,largest);
  }
}

void buildMaxHeap(int *a,int n){
  for(int i=(n/2)-1;i>=0;i--)
    maxHeapify(a,n,i);
}

void maxHeapDelete(int *a,int n){

  n=n-1;
  int deleted=a[0];
  a[0]=0;

  swap(a,0,n);

  int i=0;

  while(i<n){

	int rigthChild=rigth(i);
    int leftChild=left(i);

    int greaterChild;
    int greaterChildindex;

    if(rigth(i)<n && left(i)<n){
      greaterChild=greater(a[rigthChild],a[leftChild]);
      greaterChildindex= a[rigthChild]==greaterChild?rigthChild:leftChild;

      if(a[i]<a[greaterChildindex]){
        swap(a,i,greaterChildindex);
        i=greaterChildindex;
      }
      else
        break;

    }
    else if(left(i)<n){
      if(a[i]<a[greaterChildindex]){
        swap(a,i,greaterChildindex);
        i=greaterChildindex;
      }
      else
        break;
    }
    else
      break;
  }
  a[n]=deleted;
}

/***********************************************ALGORITHMS****************************************************/
void heapSort(int *a,int n){
  buildMaxHeap(a,n);

  for(int i=DIM_INPUT;i>=1;i--)
    maxHeapDelete(a,i);
}

void modifiedCountingSort(int input[DIM_INPUT], int position){

  int count[10];
  for(int i=0;i<10;i++)
    count[i]=0;

  for(int i=0;i<DIM_INPUT;i++)
    ++count[(input[i]/position)%10];

  for(int i=1;i<10;i++)
    count[i]=count[i]+count[i-1];

  int output[DIM_INPUT];
  for(int i=DIM_INPUT-1;i>=0;i--)
    output[--count[(input[i]/position)%10]]=input[i];

  for(int i=0;i<DIM_INPUT;i++)
    input[i]=output[i];
}

void radixSort(int input[DIM_INPUT]){
  int max=biggest(input);
  for(int position=1;(max/position)>0;position=position*10)
    modifiedCountingSort(input,position);
}

void countingSort(int input[DIM_INPUT],int output[DIM_INPUT],int key){

  int count[key+1];
  for(int i=0;i<=key;i++)
    count[i]=0;

  for(int i=0;i<DIM_INPUT;i++){
    count[input[i]]=count[input[i]]+1;
  }

  for(int i=1;i<=key;i++){
    count[i]=count[i]+count[i-1];
  }

  for(int i=DIM_INPUT-1;i>=0;i--){
    count[input[i]]--;
    output[count[input[i]]]=input[i];
  }
}

int partition(int input[DIM_INPUT],int l,int h){

  int pivot=input[h];
  int i=l-1;
  int j=l;

  for(;j<h;j++){
    if (input[j]<=pivot){
      i++;
      swap(input,i,j);
    }
  }
  swap(input,i+1,h);
  return i+1;
}

void quickSort(int input[DIM_INPUT],int l,int h){

  if(l>=h)
    return;
  int j=partition(input,l,h);
  quickSort(input,l,j-1);
  quickSort(input,j+1,h);
}



void merge(int input[DIM_INPUT],int s,int e){

  int h=(s+e)/2;

  int i=s;
  int j=h+1;
  int k=s;

  int tmp[DIM_INPUT];

  while(i<=h && j<=e){
    if(input[i]<input[j])
      tmp[k++]=input[i++];
    else
      tmp[k++]=input[j++];
  }

  while(i<=h){
    tmp[k++]=input[i++];
  }

  while(j<=e){
    tmp[k++]=input[j++];
  }

  for(int index=s;index<=e;index++){
    input[index]=tmp[index];
  }

}

void mergeSort(int input[DIM_INPUT],int s,int e){
  if(e<=s)
    return;

  int h=(s+e)/2;
  mergeSort(input,s,h);
  mergeSort(input,h+1,e);
  merge(input,s,e);
}

long InsertionSort(int input[DIM_INPUT]){
  int i;
  int key;

  long iterations=0;

  for(int j=1;j<DIM_INPUT;j++){
    iterations++;
    key=input[j];
    i=j-1;
    while(i>=0 && input[i]>key){
      input[i+1]=input[i];
      i--;
      iterations++;
    }
    input[i+1]=key;
  }

return iterations;
}

/***********************************************MAIN******************************************************/

int main(int argc,char *argv[]){

  int input[DIM_INPUT];

  printf("\t\t\tBENVENUTO!\nGenerazione di un un array di %i elementi...\n",DIM_INPUT);

  srand(time(NULL));
  for(int i=0;i<DIM_INPUT;i++){
    input[i]=rand()%100;
  }

  printf("\nArray Generato!\n");
  printArray(input,DIM_INPUT);

  int choice;
  bool NoExec=false;

  printMenu();
  scanf("%i",&choice);

  struct timeval start;
  struct timeval end;
  double timestampStart;
  double timestampEnd;

  switch (choice) {
    case 0:
      NoExec=true;
      break;
    case 1:

      gettimeofday(&start, NULL);
      timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

      long iterations=InsertionSort(input);

      gettimeofday(&end, NULL);
      timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

      stats(iterations,(timestampEnd-timestampStart));

      break;

    case 2:

      gettimeofday(&start, NULL);
      timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

      mergeSort(input,0,DIM_INPUT-1);

      gettimeofday(&end, NULL);
      timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

      stats(0,(timestampEnd-timestampStart));

      break;

    case 3:

      gettimeofday(&start, NULL);
      timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

      quickSort(input,0,DIM_INPUT-1);

      gettimeofday(&end, NULL);
      timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

      stats(0,(timestampEnd-timestampStart));

      break;

    case 4:

      gettimeofday(&start, NULL);
      timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

      shuffle(input,0,DIM_INPUT-1);
      quickSort(input,0,DIM_INPUT-1);

      gettimeofday(&end, NULL);
      timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

      stats(0,(timestampEnd-timestampStart));

      break;
    case 5:

      gettimeofday(&start, NULL);
      timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

      int output[DIM_INPUT];
      countingSort(input,output,biggest(input));

      printf("\nArray ordinato!\n");
      printArray(output,DIM_INPUT);

      NoExec=true;
      gettimeofday(&end, NULL);
      timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

      stats(0,(timestampEnd-timestampStart));

      break;
      case 6:

        gettimeofday(&start, NULL);
        timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

        radixSort(input);

        gettimeofday(&end, NULL);
        timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

        stats(0,(timestampEnd-timestampStart));

        break;

      case 7:

        gettimeofday(&start, NULL);
        timestampStart= start.tv_sec + (start.tv_usec / 1000000.0);

        heapSort(input,DIM_INPUT);

        gettimeofday(&end, NULL);
        timestampEnd =  end.tv_sec + (end.tv_usec / 1000000.0);

        stats(0,(timestampEnd-timestampStart));

        break;

      default:
        printf("Opzione non valida!\n");
        NoExec=true;
        break;
  }

  if(!NoExec){
    printf("\nArray Ordinato!\n");
    printArray(input,DIM_INPUT);
  }
  return 0;
}
