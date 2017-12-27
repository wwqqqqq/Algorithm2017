/*
实现FFT算法，对n的取值分别为4、16、32、60(注意当n取值不为2的整数幂时的处理方法)
随机生成2n个实数值（a0、a1、…、an-1）和（b0、b1、…、bn-1）分别作为多项式A(x)和B(x)的系数向量
使用FFT计算多项式A(x)与多项式B(x)的乘积，统计算法运行所需时间，与普通乘法进行比较，画出时间曲线。
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define INPUTPATH "../input/input.txt"
#define RESULTPATH "../output/result.txt"
#define TIMEPATH "../output/time.txt"
#define MAX 8192
#define PI 3.1415926535898
#define NUM 10

int const input_size[NUM]={4,16,32,60,128,256,512,1024,2048,4096};

typedef struct{
    double r;
    double i;
}Complex;

Complex func(int m,int n)
{
    //calculate exp(m*pi/n)


    Complex result;
    result.r = cos((double)m/n*PI);
    result.i = sin((double)m/n*PI);

    return result;

}

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
    for(i=0;i<MAX;i++)
    {
        fprintf(input_file,"%d.%d\n",rand()%201-100,rand()%100);
    }
    fclose(input_file);
}

void dft(int n,Complex a[n],Complex y[n])
{

    int i,j,k,m=0;
    Complex w;
    w.r = 1.0; w.i = 0.0;


    if(n==1)
    {
        y[0].r=a[0].r;
        y[0].i=0.0;
        return;
    }
    Complex a0[n/2];
    Complex a1[n/2];
    Complex y0[n/2];
    Complex y1[n/2];
    for(i=0;i<n/2;i++)
    {
        a0[i].r=a[2*i].r;
        a1[i].r=a[2*i+1].r;
        a0[i].i=a[2*i].i;
        a1[i].i=a[2*i+1].i;
    }
    dft(n/2,a0,y0);
    dft(n/2,a1,y1);
    for(k=0;k<n/2;k++)
    {
        y[k].r=y0[k].r+w.r*y1[k].r-w.i*y1[k].i;
        y[k].i=y0[k].i+w.i*y1[k].r+w.r*y1[k].i;
        y[k+n/2].r=y0[k].r-w.r*y1[k].r+w.i*y1[k].i;
        y[k+n/2].i=y0[k].i-w.i*y1[k].r-w.r*y1[k].i;
        m=m+2;
        w=func(m,n);
    }
}

void dft_rev(int n, Complex y[n],Complex a[n])
{
    int i,j,k,m=0;
    Complex w;
    w.r = 1.0; w.i = 0.0;
    if(n==1)
    {
        a[0].r=y[0].r;
        a[0].i=y[0].i;
        return;
    }
    Complex a0[n/2];
    Complex a1[n/2];
    Complex y0[n/2];
    Complex y1[n/2];
    for(i=0;i<n/2;i++)
    {
        y0[i].r=y[2*i].r;
        y0[i].i=y[2*i].i;
        y1[i].r=y[2*i+1].r;
        y1[i].i=y[2*i+1].i;
    }
    dft_rev(n/2,y0,a0);
    dft_rev(n/2,y1,a1);
    for(k=0;k<n/2;k++)
    {
        a[k].r=a0[k].r+w.r*a1[k].r-w.i*a1[k].i;
        a[k].i=a0[k].i+w.i*a1[k].r+w.r*a1[k].i;
        a[k+n/2].r=a0[k].r-w.r*a1[k].r+w.i*a1[k].i;
        a[k+n/2].i=a0[k].i-w.i*a1[k].r-w.r*a1[k].i;
        m=m-2;
        w=func(m,n);
    }
}

int isnotpowerof2(int n)
{
    if(n%2&&n!=1)
        return 1;
    if(n==1) return 0;
    return isnotpowerof2(n/2);
}

int nearest2power(int n)
{
    if(n%2) return nearest2power(n+1);
    if(isnotpowerof2(n))
        return nearest2power(n+2);
    else return n;
}

int main(void)
{
    int t=50;
    while(t--){
    int i,j;
    FILE *result_file;
    result_file=fopen(RESULTPATH,"w");
    generate_input();
    for(i=0;i<NUM;i++)
    {
        int n=input_size[i];
        int n2;
        time_t st,ed;
        if(isnotpowerof2(n)) n2=2*nearest2power(n);
        else n2=2*n;
        Complex a[n2];
        Complex b[n2];
        Complex c[n2];
        Complex ya[n2];
        Complex yb[n2];
        Complex yc[n2];


        FILE *input_file, *time_file;
        input_file = fopen(INPUTPATH,"r");
        if(input_file==NULL)
        {
            printf("Err(fun: main): open input file fails!\n");
            exit(1);
        }
        for(j=0;j<n;j++)
        {
            fscanf(input_file,"%lf",&a[j].r);
            a[j].i=0.0;
        }
        for(j=0;j<n;j++)
        {
            fscanf(input_file,"%lf",&b[j].r);
            b[j].i=0.0;
        }
        fclose(input_file);
        for(j=n;j<n2;j++)
        {
            a[j].r=0.0;
            a[j].i=0.0;
            b[j].r=0.0;
            b[j].i=0.0;
        }

        st = clock();//开始计时

        dft(n2,b,yb);
        dft(n2,a,ya);

        //点乘
        for(j=0;j<n2;j++)
        {
            yc[j].r=ya[j].r*yb[j].r-ya[j].i*yb[j].i;
            yc[j].i=ya[j].i*yb[j].r+ya[j].r*yb[j].i;
        }

        dft_rev(n2,yc,c);

        ed = clock();//停止计时

        time_file = fopen(TIMEPATH,"a");
        result_file=fopen(RESULTPATH,"a");

        fprintf(time_file,"n=%d\ttime=%.1f ms\n",n,difftime(ed,st));
        fprintf(result_file,"n=%d\t",n);
        for(j=0;j<n2;j++)
        {
            fprintf(result_file,"%.2lf\t",c[j].r/n2);
        }
        fprintf(result_file,"\n");
        fclose(result_file);
        fclose(time_file);

    }
    }
    printf("Done!\n");
    return 0;
}
