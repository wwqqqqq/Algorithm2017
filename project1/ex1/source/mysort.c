#include <string.h>
//compare
int isLarger(char *m,char *n)
{
    int lm=strlen(m);
    int ln=strlen(n);
    int i;
    if(lm>ln) return 1;
    else if(lm<ln) return 0;
    else
    {
        for(i=0;i<lm;i++)
            if(m[i]>n[i])
                return 1;
            else if(m[i]<n[i])
                return 0;
    }
    return 0;
}
//insert sort
void Insert_Sort(int n,char **A)
{
    //printf("insert sort begins! size=%d\n",n);
    int i,j;
    char *key;
    for(j=1;j<n;j++)
    {
        key=A[j];
        i=j-1;
        while(i>=0&&isLarger(A[i],key))
        {
            A[i+1]=A[i];
            i--;
        }
        A[i+1]=key;
    }
    //printf("insert sort ends!\n");
}
//heap sort
void max_heapify(int n,char **A,int i)
{
    int l=2*i+1;      //left child
    int r=2*i+2;    //right child
    int largest;
    if(l<n&&isLarger(A[l],A[i]))
        largest=l;
    else largest=i;
    if(r<n&&isLarger(A[r],A[largest]))
        largest=r;
    if(largest!=i)
    {
        char *temp=A[i];
        A[i]=A[largest];
        A[largest]=temp;
        max_heapify(n,A,largest);
    }
}
void build_max_heap(int n,char **A)
{
    int i;
    for(i=(n/2)-1;i>=0;i--)
        max_heapify(n,A,i);
}
void Heap_Sort(int n,char **A)
{
    int i;
    build_max_heap(n,A);
    for(i=n-1;i>0;i--)
    {
        char *temp;
        temp=A[0];
        A[0]=A[i];
        A[i]=temp;
        max_heapify(i,A,0);
    }
}
//merge sort
void merge(char **A,int p,int q,int r)
{
    int n1=q-p+1;
    int n2=r-q;
    int i,j,k;
    char *L[n1];
    char *R[n2];
    for(i=0;i<n1;i++)
        L[i]=A[p+i];
    for(j=0;j<n2;j++)
        R[j]=A[q+j+1];
    for(i=0,j=0,k=p;k<=r;k++)
    {
        if(i>=n1){
            while(j<n2)
                A[k++]=R[j++];
            break;
        }
        else if(j>=n2){
            while(i<n1)
                A[k++]=L[i++];
            break;
        }
        else if(isLarger(L[i],R[j]))
            A[k]=R[j++];
        else
            A[k]=L[i++];
    }
}
void Merge_Sort(char **A,int p,int r)
{
    if(p<r)
    {
        int q=(p+r)/2;
        Merge_Sort(A,p,q);
        Merge_Sort(A,q+1,r);
        merge(A,p,q,r);
    }
}
//quick sort
int partition(char **A,int p,int r)
{
    char *x=A[r];
    int i=p-1,j;
    for(j=p;j<r;j++)
        if(isLarger(x,A[j]))
        {
            char *temp=A[++i];
            A[i]=A[j];
            A[j]=temp;
        }
    A[r]=A[i+1];
    A[i+1]=x;
    //for(j=p;j<=r;j++) printf("%s\n",A[j]);
    return i+1;
}
void Quick_Sort(char **A,int p,int r)
{
    if(p>=r) return;
    /*int i;
    for(i=p;i<=r;i++) printf("%s\n",A[i]);*/
    int q=partition(A,p,r);
    //printf("p=%d\tq=%d\tr=%d\n\n",p,q,r);
    Quick_Sort(A,p,q-1);
    Quick_Sort(A,q+1,r);
}
