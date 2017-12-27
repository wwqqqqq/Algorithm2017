/*
**实现求矩阵链乘问题的算法。
**对n的取值分别为：5、10、20、30，随机生成 n+1 个整数值（p0、p1、…、pn）代表矩阵的规模
**其中第i个矩阵(1≤i ≤ n)的规模为pi-1 × pi
**用动态规划法求出矩阵链乘问题的最优乘法次序，统计算法运行所需时间，画出时间曲线。
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define INPUTPATH "../input/input.txt"
#define RESULTPATH "../output/result.txt"
#define TIMEPATH "../output/time.txt"

#define MAX 1000
#define NUM 14
#define UNINFINITE 10000000

const int input_size[NUM]={5,10,20,30,100,200,300,400,500,600,700,800,900,1000};

void generate_input(void)
{
    int i;
    FILE * input_file;
    srand((unsigned)time(NULL));
    if((input_file=fopen(INPUTPATH, "w"))==NULL)
    {
        printf("Err(fun: generate_input): Open input file fails!\n");
        exit(1);
    }
    for(i=0;i<=MAX;i++)
    {
        fprintf(input_file,"%d\n",rand()%100+1);
    }
    fclose(input_file);
}

void read_input(int n, int p[n+1])
{
    FILE *input_file;
    int i;
    input_file = fopen(INPUTPATH,"r");
    if(input_file==NULL)
    {
        printf("Err(fun: read_input): open input file fails!\n");
        exit(1);
    }
    for(i=0;i<=n;i++)
        fscanf(input_file,"%d",&p[i]);
    fclose(input_file);
}
void matrix_chain_order(int n, int p[n+1],int m[n][n], int s[n-1][n]);
void print_optimal_parens(FILE *result_file, int n, int s[n-1][n],int i, int j);

int m[MAX][MAX];
int s[MAX-1][MAX];
int p[MAX+1];

int main(void)
{
    /*int t=50;
    while(t--){*/
    int i;
    FILE * result_file;
    result_file=fopen(RESULTPATH,"w");
    fclose(result_file);
    generate_input();
    for(i=0;i<NUM;i++)
    {
        int n=input_size[i];
        FILE *result_file, *time_file;
        time_t c_st,c_ed;
        read_input(n,p);
        c_st=clock();
        matrix_chain_order(n,p,m,s);
        c_ed=clock();
        if((result_file=fopen(RESULTPATH,"a"))==NULL)
        {
            printf("Err(fun: main): open result file fails!\n");
            exit(1);
        }
        fprintf(result_file,"n=%d\n",n);
        print_optimal_parens(result_file,n,s,0,n-1);
        fprintf(result_file,"\n");
        printf("\n");
        if((time_file=fopen(TIMEPATH,"a"))==NULL)
        {
            printf("Err(fun: main): open time file fails!\n");
            exit(1);
        }
        fprintf(time_file,"n=%d\ttime=%.1f ms\n",n,difftime(c_ed,c_st));
        printf("n=%d\tstart time=%d\tend time=%d\tdiff time=%.1lf ms\n",n,c_st,c_ed,difftime(c_ed,c_st));
        fclose(result_file);
        fclose(time_file);
    }
    //}
    return 0;
}

void matrix_chain_order(int n, int p[n+1],int m[n][n], int s[n-1][n])
{
    int i,j,k,l;
    for(i=0;i<n;i++)
        m[i][i]=0;
    for(l=2;l<=n;l++)
    {
        for(i=0;i<=n-l;i++)
        {
            j=i+l-1;
            m[i][j]=UNINFINITE;
            for(k=i;k<j;k++)
            {
                int q=m[i][k]+m[k+1][j]+p[i]*p[k+1]*p[j+1];
                if(q<m[i][j])
                {
                    m[i][j]=q;
                    s[i][j]=k;
                }
            }
        }
    }
}
void print_optimal_parens(FILE *result_file, int n, int s[n-1][n],int i, int j)
{
    if(i==j)
    {
        //printf("A%d",i+1);
        fprintf(result_file, "A%d",i+1);
    }
    else
    {
        //printf("(");
        fprintf(result_file,"(");
        print_optimal_parens(result_file,n,s,i,s[i][j]);
        //printf(",");
        fprintf(result_file,",");
        print_optimal_parens(result_file,n,s,s[i][j]+1,j);
        //printf(")");
        fprintf(result_file,")");
    }
}
