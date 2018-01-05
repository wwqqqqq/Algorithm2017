//实现求有向图的强连通分量的算法。
//有向图的顶点数 N 的取值分别为：8、16、32、64、128、256，弧的数目为 NlogN，随机生成 NlogN条弧
//统计算法所需运行时间，画出时间曲线。
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define INPUT 1
#define OUTPUT 0
#define TIMES 10000
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define NIL -1
#define DFS_G 0
#define DFS_GT 1

int G[256][256];// The adjacency-matrix representation of G
//int GT[256][256];
int color[256];
int parent[256];
int f[256];
int d[256];
int Time;

const int v_size[6]={8,16,32,64,128,256};//顶点数
const int e_size[6]={24,64,160,384,896,2048};//边数=NlogN

FILE *open_file(int ioselect, int n,const char *filename,const char *mode);
void generate_input(void);
void read_input(int n);
void init_matrix(int n);
void SCC(int n);
void DFS(int v,int order[v],int graph);
void DFS_visit(int v,int u,int graph);

clock_t st,ed;
FILE *time1;
FILE *result;
int main(void)
{
    generate_input();
    int i;
    for(i=0;i<6;i++)
    {
        int v=v_size[i];
        int e=e_size[i];
        clock_t st,ed;
        double t;
        int times=TIMES;
        result=open_file(OUTPUT,i,"output1.txt","w");
        time1=open_file(OUTPUT,i,"time1.txt","w");
        read_input(i);
        st=clock();
        while(times--)
            SCC(i);//计算强连通分量
        ed=clock();
        t=difftime(ed,st)/TIMES;
        fprintf(time1,"%.2lf",t);
        fclose(result);
        fclose(time1);
    }
    return 0;
}


/*
STRONGLY-CONNECTED-COMPONENTS.G/
1 call DFS(G) to compute finishing times u.f for each vertex u
2 compute GT
3 call DFS(GT), but in the main loop of DFS, consider the vertices
in order of decreasing u.f (as computed in line 1)
4 output the vertices of each tree in the depth-first forest formed in line 3 as a
separate strongly connected component*/
void SCC(int n)
{
    int v=v_size[n];
    int order[v];
    int i;
    for(i=0;i<v;i++) order[i]=i;
    DFS(v,order,DFS_G);
    for(i=0;i<v;i++)
    {
        int j=f[i]/2;
        order[v-j]=i;
    }
    DFS(v,order,DFS_GT);
}


void DFS(int v,int order[v],int graph)
{
    //int v=v_size[n];
    int i;
    for(i=0;i<v;i++)
    {
        color[i]=WHITE;
        parent[i]=NIL;
    }
    Time=0;
    for(i=0;i<v;i++)
    {
        int j=order[i];
        if(color[j]==WHITE)
        {
            if(graph==DFS_GT)
                fprintf(result,"(");
            DFS_visit(v,j,graph);
            if(graph==DFS_GT)
                fprintf(result,")\n");
        }
    }
}

void DFS_visit(int v,int u,int graph)
{
    Time++;
    d[u]=Time;
    color[u]=GRAY;
    //int v=v_size[n];
    int i;
    if(graph==DFS_GT)
        fprintf(result,"%d",u);
    for(i=0;i<v;i++)
    {
        if(color[i]!=WHITE) continue;
        if(graph==DFS_G&&G[u][i]==1)
        {
            parent[i]=u;
            DFS_visit(v,i,graph);
        }
        else if(graph==DFS_GT&&G[i][u]==1)
        {
            parent[i]=u;
            fprintf(result,", ");
            DFS_visit(v,i,graph);
        }

    }
    color[u]=BLACK;
    Time++;
    f[u]=Time;
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

void generate_input(void)
//随机生成的有向图信息分别存放到对应数据规模文件夹里面的input.txt文件
//每行存放一对节点a,b序号(数字表示)，表示存在一条节点a指向节点b的边。分别读取这五个规模的图数据进行求解最强连通分量的实验。
//顶点数的取值：8、16、32、64、128、256，弧的数目为 NlogN，随机生成 NlogN条弧
{
    int i;
    for(i=0;i<6;i++)
    {
        int v=v_size[i];
        int e=e_size[i];
        FILE *f=open_file(INPUT,i,"input.txt","w");
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
            }
            if(j>=k)
            {
                tempa[k]=a;
                tempb[k]=b;
                k++;
                fprintf(f,"%d,%d\n",a,b);
            }
        }
        fclose(f);
    }
}

void init_matrix(int n)
//将G[0..v-1][0..v-1]置0
{
    int i,j;
    int v=v_size[n];
    for(i=0;i<v;i++)
    {
        for(j=0;j<v;j++)
            G[i][j]=0;
    }
}

void read_input(int n)
//根据../size{n+1}/input.txt将所有(a,b)对应的matrix[a][b]置1
{
    FILE *f=open_file(INPUT,n,"input.txt","r");
    int v=v_size[n];
    int e=e_size[n];
    int i;
    init_matrix(n);
    for(i=0;i<e;i++)
    {
        int a,b;
        fscanf(f,"%d,%d",&a,&b);
        if(G[a][b]==1) printf("duplicate edge (%d,%d)\n",a,b);
        G[a][b]=1;
    }
    fclose(f);
}

