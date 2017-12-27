#include <stdlib.h>
#define DATA_RANGE 65535
//quick sort
int partition(int n,int A[n],int p,int r)
{
    int x=A[r];
    int i=p-1,j;
    for(j=p;j<r;j++)
        if(x>A[j])
        {
            int temp=A[++i];
            A[i]=A[j];
            A[j]=temp;
        }
    A[r]=A[i+1];
    A[i+1]=x;
    return i+1;
}
void quick_sort(int n,int A[n],int p,int r)
{
    if(p>=r) return;
    int q=partition(n,A,p,r);
    quick_sort(n,A,p,q-1);
    quick_sort(n,A,q+1,r);
}
//counting sort
void counting_sort(int n, int A[n])
{
	int C[DATA_RANGE+1];
	int B[n];
	int i;
	for (i=0;i<=DATA_RANGE;i++)
	{
		C[i] = 0;
	}
	for (i = 0; i < n; i++)
		C[A[i]] = C[A[i]] + 1;
	for (i = 1; i <= DATA_RANGE; i++)
		C[i] = C[i] + C[i - 1];
	for (i = n - 1; i >= 0; i--)
	{
		B[C[A[i]]-1] = A[i];
		C[A[i]]--;
	}
	for (i = 0; i < n; i++)
		A[i] = B[i];
}
//bubble sort
void bubble_sort(int n, int A[n])
{
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		for(j = n - 1; j > i; j--)
			if (A[j] < A[j - 1])
			{
				int temp = A[j];
				A[j] = A[j - 1];
				A[j - 1] = temp;
			}
	}
}
//radix sort
void stable_sort(int n, int A[n][5], int d)
{
	//counting sort???
	int C[10];
	int B[n][5];
	int i,j;
	for (i=0;i<=9;i++)
		C[i] = 0;
	for (i = 0; i < n; i++)
		C[A[i][d]] = C[A[i][d]] + 1;
	for (i = 1; i <= 9; i++)
		C[i] = C[i] + C[i - 1];
    printf("%d\n",C[9]);
	for (i = n - 1; i >= 0; i--)
	{
	    int index=C[A[i][d]]-1;
	    for(j=0;j<5;j++) B[index][j]=A[i][j];
		//B[index] = A[i];
		C[A[i][d]]--;
	}
    for (i = 0; i < n; i++)
        for(j=0;j<5;j++)
		A[i][j] = B[i][j];
}
void radix_sort(int n, int A[n])
{
	int i,j;
	int B[n][5];
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < 5; j++)
		{
			B[i][j] = A[i] % 10;
			A[i] = A[i] / 10;
		}
	}
	for (i = 0; i < 5; i++)
		stable_sort(n, B, i);
	for (i = 0; i < n; i++)
	{
		A[i] = B[i][4];
		for (j = 3; j >= 0; j--) {
			A[i] *= 10;
			A[i] += B[i][j];
		}
	}
}
