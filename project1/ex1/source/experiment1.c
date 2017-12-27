#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mysort.c"
#define OK 1
#define ERROR -1
#define INSERT_SORT 0
#define HEAP_SORT 1
#define MERGE_SORT 2
#define QUICK_SORT 3
#define INPUT_FILEPATH "../input/input_strings.txt"
typedef int sort_method;
//直接插入排序，堆排序，归并排序，快速排序

int generate_input(void);                    //test!!!
char ** generate_array(int n);          //tested but not sure
time_t sort(int n, char**A,sort_method m);   //not tested yet  (rely on function insert_sort, heap_sort, merge_sort, quick_sort)
int output(int i,int n,char **A,sort_method m,time_t total_time); //not tested yet
void free_array(int n, char **A);            //not tested yet


int main(void)
{
    //2，5，8，11，14，17
    const size[]={4,32,256,2048,16384,131072};
    int i;
    generate_input(); //not tested
    for(i=0;i<6;i++)
    {
        int n;
		time_t total_time;
        sort_method m;
        n=size[i];
        for(m=INSERT_SORT;m<=QUICK_SORT;m++){
            char **A;
            int j;
            A=generate_array(n);
            total_time=sort(n,A,m);
            output(i*3+2,n,A,m,total_time);
            free_array(n, A);
            printf("size = %d\tmethod = %d\ttime=%dms\tDONE!\n",n,m,total_time);///////
        }
    }
}

int generate_input(void)
{
    const int size=131072;
    int i,j,length;
    char letter;
    FILE * input_file;
    input_file=fopen(INPUT_FILEPATH,"w");
    if(input_file==NULL)
    {
        printf("Opening input file fails in function generate_input!\n");
        return ERROR;
    }
    srand((unsigned)time(NULL));
    for(i=0;i<size;i++)
    {
        length=rand()%32+1;  //1~32
        char buffer[length+2];
        for(j=0;j<length;j++)
        {
            letter=rand()%26+'a';  //'a'~'z'
            buffer[j]=letter;
        }
        buffer[j]='\n';
        buffer[j+1]='\0';
        fputs(buffer,input_file);
    }
    fclose(input_file);
    //printf("generate input done!\n");
    return OK;
}

char ** generate_array(int n)
{
    char **A;
    FILE * input_file;
    int i;
    char string[33];
    input_file=fopen(INPUT_FILEPATH,"r");
    if(input_file==NULL)
    {
        printf("Opening input file fails in function generate_array!\n");
        //return ERROR;
    }
    A=malloc(sizeof(char*)*n);
    if(A==NULL)
    {
        printf("malloc fails in function generate_array!\n");
        //return ERROR;
    }
    //printf("generate_array begins! size=%d\n",n);
    for(i=0;i<n;i++)
    {
        fscanf(input_file,"%s",string);
        A[i]=malloc(sizeof(char)*(strlen(string)+1));
        if(A[i]==NULL)
        {
            printf("malloc fails in function generate_array!\n");
            //return ERROR;
        }
        strcpy(A[i],string);
    }
    /*for(i=0;i<n;i++)
        printf("%s\n",A[i]);
    printf("generate array done!\n");*/
    //return OK;
    return A;
}

time_t sort(int n, char**A,sort_method m)
{
    //printf("begining of function sort!\n");
    printf("sorting... size=%d, method=%d\n",n,m);
    time_t start,end,total_time;
    start=clock();
    switch(m)
    {
        case INSERT_SORT: Insert_Sort(n,A); break;
        case HEAP_SORT: Heap_Sort(n,A);break;
        case MERGE_SORT: Merge_Sort(A,0,n-1);break;
        case QUICK_SORT: Quick_Sort(A,0,n-1);break;
        default: printf("Invalid sort method!\n");return;
    }
    end=clock();
    total_time=difftime(end,start);
    //printf("sort function done! time=%d\n",total_time);//
    return total_time;
}

void free_array(int n, char **A)
{
    int i;
    for(i=0;i<n;i++)
        free(A[i]);
    free(A);
}


int output(int i,int n,char **A,sort_method m,time_t total_time)
{
    const char method_name[4][20]={"insert_sort","heap_sort","merge_sort","quick_sort"};
    FILE * result_file;
    FILE * time_file;
    char filename_r[50];
    char filename_time[50];
    //char time_result[50];
    int index;
    sprintf(filename_r,"../output/%s/result_%d.txt",method_name[m],i);
    sprintf(filename_time,"../output/%s/time.txt",method_name[m]);
    //sprintf(time_result,"size = 2^%-2d\ttotal time = %d",i,total_time);
    result_file=fopen(filename_r,"w");
    if(result_file==NULL)
    {
        printf("Opening file result_%d.txt fails in function output!\n",i);
        return ERROR;
    }
    time_file=fopen(filename_time,"a");
    if(time_file==NULL)
    {
        printf("Opening file time.txt fails in function output!\n");
        return ERROR;
    }
    for(index=0;index<n;index++)
    {
        fprintf(result_file,"%s\n",A[index]);
    }
    fclose(result_file);
    fprintf(time_file,"size = 2^%-2d\ttotal time = %dms\n",i,total_time);
    fclose(time_file);
    return OK;
}

