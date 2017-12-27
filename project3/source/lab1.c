#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "RBTree.c"
//12、24、36、48、60
#include <string.h>
#define INPUT_FILE "../input/input.txt"
#define PRE_RESULT_FILE "preorder.txt"
#define IN_RESULT_FILE "inorder.txt"
#define POST_RESULT_FILE "postorder.txt"
#define DEL_DATA_FILE "delete_data.txt"
#define TIME_FILE1 "time1.txt"
#define TIME_FILE2 "time2.txt"
#define MAX 60

const int datasize[5]={12,24,36,48,60};

FILE *open_outputfile(int n, const char *filename);
void generate_input(void);

int main(void)
{
    FILE *input;
    int i;
    int data[MAX];
    generate_input();
    input = fopen(INPUT_FILE,"r");
    if(input==NULL)
    {
        printf("open input file fail!\n");
        exit(0);
    }
    for(i=0;i<MAX;i++)
    {
        fscanf(input,"%d",&data[i]);
    }
    fclose(input);

    for(i=0;i<5;i++)
    {
        int n=datasize[i];
        int j;
        FILE *result;
        FILE *time;
        time = open_outputfile(n, TIME_FILE1);
        if(time==NULL)
        {
            printf("open time1.txt fail! size = %d\n",n);
            exit(0);
        }
        //实验1部分：初始化一棵红黑树并向其中插入结点
        RBTree T=Init_RBTree();
        clock_t st_t, ed_t;
        clock_t st, ed;
        st_t = clock();
        st = st_t;
        for(j=0;j<n;j++)
        {
            RBNode *x = Create_RBNode(T,data[j]);
            RB_insert(T,x);
            if(j%10==9)
            {
                //count time
                ed = clock();
                fprintf(time,"insert %d~%d:\tst=%d\ted=%d\t%.3lf\n",j-8,j+1,st,ed,difftime(ed,st));
                st = ed;
            }

                                                                             //printf("insert key %d\n",x->key);
                                                                             //RBTree_print_preorder(T,T->root);
        }
        ed_t = clock();
        ed = ed_t;
        if(j%10!=0)
            fprintf(time, "insert %d~%d:\tst=%d\ted=%d\t%.3lf\n",(j/10)*10+1,j,st,ed,difftime(ed,st));
        fprintf(time,"total time:\tst=%d\ted=%d\t%.3lf\n",st_t,ed_t,difftime(ed_t,st_t));
        fclose(time);
        //printf("pre-order:\n");
        result = open_outputfile(n, PRE_RESULT_FILE);
        RBTree_print_preorder(T, T->root, result);
        fclose(result);
        //printf("in-order:\n");
        result = open_outputfile(n, IN_RESULT_FILE);
        RBTree_print_inorder(T,T->root, result);
        fclose(result);
        //printf("post-order:\n");
        result = open_outputfile(n, POST_RESULT_FILE);
        RBTree_print_postorder(T,T->root, result);
        fclose(result);

        printf("lab 1 done!\n");


        //实验2部分：删除n/3, n/4结点
        result = open_outputfile(n, DEL_DATA_FILE);
        int t = T->root->size;
        RBNode *x = OS_select(T->root,t/3);
        printf("n=%d\tn/3=%d delete rank %d, key %d\n",t,t/3,OS_rank(T,x),x->key);
        RB_delete(T, x);
        RBNode_print(T,x,result);
        free(x);
        t = T->root->size;
        x = OS_select(T->root,t/4);
        printf("n=%d\tn/4=%d delete rank %d, key %d\n",t,t/4,OS_rank(T,x),x->key);
        RB_delete(T, x);
        RBNode_print(T,x,result);
        free(x);
        fclose(result);


        Del_RBTree(T,T->root);
        printf("here %d\n",n);
    }
    printf("???");
    return 0;
}


FILE *open_outputfile(int n, const char *filename)
{
    char filepath[20];
    sprintf(filepath,"../output/size%d/%s\0",n,filename);
    FILE *outputfile;
    outputfile = fopen(filepath, "w");
    if(outputfile==NULL)
    {
        printf("open file %s fail!\n",filepath);
        exit(1);
    }
}

void generate_input(void)
{
    FILE *input;
    int temp[MAX];
    int i,j;
    if((input=fopen(INPUT_FILE,"w"))==NULL)
    {
        printf("open input file fail!\n");
        exit(0);
    }
    srand((unsigned)time(NULL));
    for(i=0;i<MAX;i++)
    {
        //generate ith input
        int t;
        while(1){
            t=rand()%1000+1;//1~1000
            for(j=0;j<i;j++)
            {
                if(temp[j]==t) break;
            }
            if(j==i) break;
        }
        fprintf(input,"%d\n",t);
        temp[i]=t;
    }
    fclose(input);
}
