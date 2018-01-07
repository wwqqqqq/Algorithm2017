/*实验2：实现求所有点对最短路径的Johnson算法。
  生成连通的无向图，图的顶点数 N 的取值分别为： 8、16、32、64,  128、256 ，边的数目为 NlogN,
  随机生成 NlogN 条边，统计算法所需运行时间 ，画出时间曲线。
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INPUT 1
#define OUTPUT 0
#define FALSE 0
#define TRUE 1
#define INF 100000
#define MAX 500
#define NIL -1
#define M 50000
#define TIMES 500

int G[257][257];// The adjacency-matrix representation of G
int d[257];
int h[257];
int parent[256];
int D[256][256];
int P[256][256];

const int v_size[6]={8,16,32,64,128,256};//顶点数
const int e_size[6]={24,64,160,384,896,2048};//边数=NlogN

FILE *open_file(int ioselect, int n,const char *filename,const char *mode);
void generate_input(int n);
void random_input(int n);
void read_input(int n);
void init_matrix(int n);
int Bellman_Ford(int v,int s);
void init_single_source(int v,int s);
void relax(int i,int j);
void Johnson(int n);
void dijkstra(int v,int s);

FILE *f;
FILE *TIME;

void print_path(int i,int j)
{
    if(i==j)
    {
        fprintf(f,"%d",i);
        return;
    }
    int p=P[i][j];
    if(p==i)
    {
        fprintf(f,"%d, %d",i,j);
        return;
    }
    print_path(i,p);
    fprintf(f,", %d",j);
}

int main(void)
{
    int i;
    for(i=0;i<6;i++)
    {
        int j,k;
        int v=v_size[i];
        int times=TIMES;
        clock_t st,ed;
        double total=0;
        f=open_file(OUTPUT,i,"output2.txt","w");
        TIME=open_file(OUTPUT,i,"time2.txt","w");
        generate_input(i);
        while(times--)
        {
            init_matrix(i);
            read_input(i);
            st=clock();
            Johnson(i);
            ed=clock();
            total=total+difftime(ed,st);
        }
        total=total/TIMES;
        fprintf(TIME,"%.5lf ms\n",total);

        for(j=0;j<v;j++)
        for(k=0;k<v;k++)
        {
            if(j==k) continue;
            if(D[j][k]>M)
            {
                fprintf(f,"%d->%d:UNREACHABLE\n",j,k);
                continue;
            }
            fprintf(f,"%d->%d: length=%-3d\t(",j,k,D[j][k]);
            print_path(j,k);
            fprintf(f,")\n");
        }
        fclose(f);
        fclose(TIME);
    }
}

void d_relax(int s,int i,int j)
{
    if(d[j]>d[i]+G[i][j])
    {
        d[j]=d[i]+G[i][j];
        P[s][j]=i;
    }
}

void dijkstra(int v,int s)
{
    init_single_source(v,s);
    int q[v];
    int i;
    for(i=0;i<v;i++) q[i]=i;
    for(i=v;i>0;i--)
    {
        int j;
        int min=d[i-1];
        int min_ind=i-1;
        for(j=0;j<i-1;j++)
            if(min>d[j])
            {
                min=d[q[j]];
                min_ind=j;
            }
        int temp=q[min_ind];
        q[min_ind]=q[i-1];
        q[i-1]=temp;
        for(j=0;j<v;j++)
        {
            if(G[temp][j]<MAX)
            d_relax(s,temp,j);
        }
    }
}

void Johnson(int n)
{
    int v=v_size[n];
    int s=v,i,j;
    for(i=0;i<v;i++)
    {
        G[s][i]=0;
        G[i][s]=INF;
    }
    G[s][s]=INF;

    if(Bellman_Ford(v+1,s)==FALSE)
    {
        printf("graph contains a negative weight cycle\n");
        exit(1);
    }
    for(i=0;i<=v;i++)
    {
        h[i]=d[i];
    }
    for(i=0;i<=v;i++)
    for(j=0;j<=v;j++)
    {
        if(G[i][j]<MAX)
        {
            G[i][j]=G[i][j]+h[i]-h[j];
        }
    }
    for(i=0;i<v;i++)
    {
        dijkstra(v,i);
        for(j=0;j<v;j++)
            D[i][j]=d[j]+h[j]-h[i];
    }
}

void relax(int i,int j)
{
    if(d[j]>d[i]+G[i][j])
    {
        d[j]=d[i]+G[i][j];
        parent[j]=i;
    }
}

void init_single_source(int v,int s)
{
    int i;
    //int v=v_size[n];
    for(i=0;i<v;i++)
    {
        d[i]=INF;
        parent[i]=NIL;
    }
    d[s]=0;
}

int Bellman_Ford(int v,int s)
{
    init_single_source(v,s);
    int i,j;
    for(i=0;i<v;i++)
    {
        int k;
        for(j=0;j<v;j++)
        for(k=0;k<v;k++)
        {
            if(G[j][k]<MAX)
                relax(j,k);
        }
    }
    for(i=0;i<v;i++)
        for(j=0;j<v;j++)
        if(G[i][j]<MAX)
        {
            if(d[j]>d[i]+G[i][j])
                return FALSE;
        }

    return TRUE;
}


void generate_input(int n)
{
    //init_matrix(n);
    while(1)
    {
        init_matrix(n);
        random_input(n);
        read_input(n);
        if(Bellman_Ford(v_size[n],0)==TRUE)
            break;
    }
}

void random_input(int n)
//随机生成的有向图信息分别存放到对应数据规模文件夹里面的input.txt文件
//每行存放一对节点a,b序号(数字表示)
//顶点数的取值：8、16、32、64、128、256，弧的数目为 NlogN，随机生成 NlogN条弧
{
    //int i;
    //for(i=0;i<6;i++)
    //{
        int v=v_size[n];
        int e=e_size[n];
        FILE *f=open_file(INPUT,n,"input.txt","w");
        int k=0;
        int tempa[e];
        int tempb[e];
        srand((unsigned)time(NULL));
        while(k<e)
        {
            int j;
            int a=rand()%v;
            int b=rand()%v;
            if(a==b) continue;
            for(j=0;j<k;j++)
            {
                if(tempa[j]==a&&tempb[j]==b)
                    break;
                //if(tempa[j]==b&&tempb[j]=a)
                //    break;
            }
            if(j>=k)
            {
                int length;
                int logn=e/v;
                tempa[k]=a;
                tempb[k]=b;
                k++;
                if(k<=logn) length=-(rand()%logn+1);
                else length = rand()%v+1;
                fprintf(f,"%d,%d\t%d\n",a,b,length);
            }
        }
        fclose(f);
    //}
}

void init_matrix(int n)
//将G[0..v-1][0..v-1]置0
{
    int i,j;
    int v=v_size[n];
    for(i=0;i<v;i++)
    {
        for(j=0;j<v;j++)
            G[i][j]=INF;
    }
}
void read_input(int n)
//根据../size{n+1}/input.txt将所有(a,b)对应的matrix[a][b]置为相应的路径长度
{
    FILE *f=open_file(INPUT,n,"input.txt","r");
    int v=v_size[n];
    int e=e_size[n];
    int i;
    init_matrix(n);
    for(i=0;i<e;i++)
    {
        int a,b;
        int length;
        fscanf(f,"%d,%d",&a,&b);
        if(G[a][b]<MAX) printf("duplicate edge (%d,%d)\n",a,b);
        fscanf(f,"%d",&length);
        G[a][b]=length;
        //G[b][a]=length;
    }
    fclose(f);
}
FILE *open_file(int ioselect, int n,const char *filename,const char *mode)
//e.g. open_file(INPUT,1,"input.txt","r") 返回文件指针，打开文件"../Input/size1/input.txt"，打开方式为read
//open_file(OUTPUT,2,"time1.txt","w") 返回文件指针，打开文件"../Output/size2/time1.txt"，打开方式为write
{
    char filepath[50];
    FILE *f;
    sprintf(filepath,"../%s/size%d/%s",(ioselect==INPUT)?"Input":"Output",n+1,filename);
    f = fopen(filepath,mode);
    if(f==NULL)
    {
        printf("Opening file \"%s\" fails! mode = \"%s\"\n",filepath,mode);
        exit(1);
    }
    return f;
}


