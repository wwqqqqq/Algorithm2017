#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mysort.c"
#define OK 1
#define ERROR -1
#define BUBBLE_SORT 0
#define QUICK_SORT 1
#define RADIX_SORT 2
#define COUNTING_SORT 3
#define INPUT_FILEPATH "../input/input_numbers.txt"
typedef int sort_method;
//冒泡排序，快速排序，基数排序，计数排序。

int generate_input(void);                   //not tested yet
int generate_array(int n,int A[n]);          //not tested yet
time_t sort(int n, int A[n],sort_method m);   //not tested yet  (rely on function insert_sort, heap_sort, merge_sort, quick_sort)
int output(int i,int n,int A[n],sort_method m,time_t total_time); //not tested yet


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
        for(m=BUBBLE_SORT;m<=COUNTING_SORT;m++){
            int A[n];
            generate_array(n,A);
            total_time=sort(n,A,m);
            output(3*i+2,n,A,m,total_time);
            printf("size = %d\tmethod = %d\ttime = %dms\tDONE!\n",n,m,total_time);
        }
    }
}

int generate_input(void)
{
    const int size=131072;
    int i,j;
	int x;
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
        x=rand()*2;  //0~65534
        x+=rand()%2; //1~65535
        fprintf(input_file,"%d\n",x);
    }
    fclose(input_file);
    return OK;
}

int generate_array(int n,int A[n])
{
    FILE * input_file;
    int i;
	int x;
    input_file=fopen(INPUT_FILEPATH,"r");
    if(input_file==NULL)
    {
        printf("Opening input file fails in function generate_array!\n");
        return ERROR;
    }
    for(i=0;i<n;i++)
        fscanf(input_file,"%d",&A[i]);
    return OK;
}

time_t sort(int n, int A[n],sort_method m)
{
    time_t start,end,total_time;
    printf("sorting...size=%d, method=%d\n",n,m);
    start=clock();
    switch(m)
    {
        case BUBBLE_SORT: bubble_sort(n,A); break;
        case COUNTING_SORT: counting_sort(n,A);break;
        case RADIX_SORT: radix_sort(n,A);break;
        case QUICK_SORT: quick_sort(n,A,0,n-1);break;
        default: printf("Invalid sort method!\n");return;
    }
    end=clock();
    total_time=difftime(end,start);
    return total_time;
}



int output(int i,int n,int A[n],sort_method m,time_t total_time)
{
    const char method_name[4][20]={"bubble_sort","quick_sort","radix_sort","counting_sort"};
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
        fprintf(result_file,"%d\n",A[index]);
    }
    fclose(result_file);
    fprintf(time_file,"size = 2^%-2d\ttotal time = %dms\n",i,total_time);
    fclose(time_file);
    return OK;
}

