/*
a)本次实验要求实现的是附加一个x.size信息的红黑即顺序统计树
b)输入数据要求是互不相同的正整数
c)自行设计一个较方便的函数，使得在检查时能够显示树的详细信息，以便能够观察到结点插入、删除前后树的结构变化(包括结点颜色，秩,父子节点关系等信息)
  每行输出一个key的信息
d)第二个实验要求删除的第n/3小的节点和第n/4小的节点的n是动态变化的，而不是静态的，执行一次删除操作n的值减少1，例如n为12，首先删除12/3=4小节点
  然后删除12/4=3小的节点
e)由于删除节点等函数代码量比较大,要注意代码可读性和条理性,注释清楚实现过程
 1)实现红黑树左旋操作 LEFT-ROTATE(T, x)
   实现右旋操作  RIGHT-ROTATE(T, x)
 2)实现红黑树插入节点的操作 RB-INSERT(T, z)以及插入之后修正为红黑书的的算法 RB-INSERT-FIXUP(T, x)
 （在函数实现过程中对于 case1 case2 case3 的三部分代码要注释清楚）
 3)实现红黑树删除节点的操作 RB-DELETE(T, z)以及插入之后修正为红黑书的的算法 RB-DELETE-FIXUP(T, x)
 （在函数实现过程中对于 case1 case2 case3 case4的四部分代码要注释清楚）
 4)实现按要求数据构建顺序统计树的操作
 5)实现遍历输出构建好的红黑树的操作
 6)实现查找顺序统计树的第i小关键字的操作OS-SELECT(T->root,i)
*/
#include <stdlib.h>
#include <stdio.h>

typedef enum{black, red} RBColor;

typedef struct rb_node{
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
    int key;
    RBColor color;
    int size;//实现顺序统计树附加信息
}RBNode;

typedef struct rb_tree{
    RBNode *root;
    //int n;//size of tree
    RBNode *nil;
}*RBTree;

RBTree Init_RBTree(void);
void Del_RBTree(RBTree T, RBNode *root);
RBNode *Create_RBNode(RBTree T, int key);
RBNode *OS_select(RBNode *x,int i);
int OS_rank(RBTree T, RBNode *x);
void RBNode_print(RBTree T, RBNode *x, FILE *stream);
void left_rotate(RBTree T, RBNode *x);
void right_rotate(RBTree T, RBNode *x);
void RB_insert_fixup(RBTree T, RBNode *z);
void RB_insert(RBTree T, RBNode *z);
void RB_transplant(RBTree T, RBNode *u, RBNode *v);
RBNode *RB_minimum(RBTree T, RBNode *root);
void RB_delete_fixup(RBTree T, RBNode *x);
void RB_delete(RBTree T, RBNode *z);
void RBTree_print_preorder(RBTree T, RBNode *x, FILE *stream);
void RBTree_print_inorder(RBTree T, RBNode *x, FILE *stream);
void RBTree_print_postorder(RBTree T, RBNode *x, FILE *stream);


RBTree Init_RBTree(void)
{
                                                                            //printf("init_rbtree begins\n");
    RBTree T;
    T = malloc(sizeof(struct rb_tree));
    T->nil = malloc(sizeof(RBNode));
    T->nil->left = T->nil;
    T->nil->right = T->nil;
    T->nil->parent = T->nil;
    T->nil->color = black;
    T->nil->size = 0;
    T->root = T->nil;
                                                                            //printf("init_rbtree ends\n");
    return T;
}

void Del_RBTree(RBTree T, RBNode *root)
{
                                                                            //printf("del_rbtree begins\n");
    if(T->root == T->nil) //rb tree empty
    {
        free(T->nil);
        free(T);
        return;
    }
    if(root == T->nil)
    {
        return;
    }
    Del_RBTree(T, root->left);
    Del_RBTree(T, root->right);
    if(root == T->root)
    {
        free(T->nil);
        free(T);
    }
                                                                            //printf("deleted key = %d\n",root->key);
    free(root);
                                                                            //printf("del_rbtree ends\n");
}

RBNode *Create_RBNode(RBTree T, int key)
{
                                                                            //printf("create node begins\nkey = %d\n",key);
    RBNode *x;
    x = malloc(sizeof(RBNode));
    x->left = T->nil;
    x->right = T->nil;
    x->parent = T->nil;
    x->size = 1;
    x->key = key;
                                                                            //printf("create node ends\n");
    return x;
}

RBNode *OS_select(RBNode *x,int i)
{
    int r = x->left->size+1;
    if(i == r)
        return x;
    if(i < r)
        return OS_select(x->left,i);
    return OS_select(x->right,i-r);
}

int OS_rank(RBTree T, RBNode *x)
{
    int r = x->left->size+1;
    RBNode *y = x;
    while(y!=T->root)
    {
        if(y==y->parent->right)
            r = r + y->parent->left->size + 1;
        y = y->parent;
    }
    return r;
}

void RBNode_print(RBTree T, RBNode *x, FILE *stream)
{
    //结点颜色，秩,父子节点关系

    if(x == T->nil)
    {
        fprintf(stream, "NIL\n");
        return;
    }

    fprintf(stream, "key: %-4d\t",x->key);

    fprintf(stream, "color: ");
    if(x->color==red) fprintf(stream, "RED  \t");
    else fprintf(stream, "BLACK\t");

    fprintf(stream, "rank: %-2d\tsize: %-2d\tparent: ",OS_rank(T,x),x->size);
    if(x->parent == T->nil) fprintf(stream, "NIL \t");
    else fprintf(stream, "%-4d\t",x->parent->key);

    fprintf(stream, "lchild: ");
    if(x->left == T->nil) fprintf(stream, "NIL \t");
    else fprintf(stream, "%-4d\t",x->left->key);

    fprintf(stream, "rchild: ");
    if(x->right == T->nil) fprintf(stream, "NIL \n");
    else fprintf(stream, "%-4d\n",x->right->key);

}

void left_rotate(RBTree T, RBNode *x)
{
    if(x == T->nil) return;
    RBNode *y = x->right; //set y
    if(y == T->nil) return;
    x->right = y->left; //turn y's left subtree into x's right subtree
    if(y->left != T->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if(x->parent == T->nil)
        T->root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    y->size = x->size;
    x->size = x->left->size + x->right->size +1;
}

void right_rotate(RBTree T, RBNode *x)
{
    if(x == T->nil) return;
    RBNode *y = x->left;
    if(y == T->nil) return;
    x->left = y->right;
    if(y->right != T->nil)
        y->right->parent = x;
    y->parent = x->parent;
    if(x->parent == T->nil)
        T->root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
    y->size = x->size;
    x->size = x->left->size + x->right->size +1;
}

void RB_insert_fixup(RBTree T, RBNode *z)
{
                                                                            //printf("insert fixup begins\n");
    RBNode *y;
    while(z->parent->color == red)
    {
        if(z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;
            if(y->color == red) //case 1
            {
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->right) //case 2
                {
                    z = z->parent;
                    left_rotate(T,z);
                }
                //case 3
                z->parent->color = black;
                z->parent->parent->color = red;
                right_rotate(T,z->parent->parent);
            }

        }
        else
        {
            y = z->parent->parent->left;
            if(y->color == red) //case 1
            {
                z->parent->color = black;
                y->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->left) //case 2
                {
                    z = z->parent;
                    right_rotate(T,z);
                }
                //case 3
                z->parent->color = black;
                z->parent->parent->color = red;
                left_rotate(T,z->parent->parent);
            }
        }
    }
    T->root->color = black;
}

void RB_insert(RBTree T, RBNode *z)
{
                                                                            //printf("insert node begins\nkey = %d\n",z->key);
    RBNode *y = T->nil;
    RBNode *x = T->root;
    while(x!=T->nil)
    {
        y = x;
        if(z->key<x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    z->parent = y;
    if(y==T->nil)
    {
        T->root = z;
    }
    else if(z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    while(y!=T->nil)
    {
        y = y->parent;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = red;
    z->size = 1;
    RBNode *temp = z->parent;
    while(temp!=T->nil)
    {
        temp->size++;
        temp = temp->parent;
    }                                                                        //RBNode_print(T, z);
    RB_insert_fixup(T,z);
                                                                            //RBNode_print(T, z);
                                                                            //printf("insert node ends\n");
}

void RB_transplant(RBTree T, RBNode *u, RBNode *v)
{
    RBNode *temp = u->parent;
                                                                                //RBNode_print(T,v);
                                                                                //RBNode_print(T,u->parent);
    if(u->parent == T->nil)
    {
        T->root = v;
    }
    else if(u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    if(v!=T->nil) v->parent = u->parent;
    while(temp!=T->nil)
    {
        temp->size = temp->left->size + temp->right->size +1;
        temp = temp->parent;
    }
                                                                                //RBNode_print(T,v);
                                                                                //RBNode_print(T,v->parent);
}

RBNode *RB_minimum(RBTree T, RBNode *root)
{
    RBNode *x = root;
    if(x == T->nil) return T->nil;
    while(x->left!=T->nil)
        x = x->left;
    return x;
}


void RB_delete_fixup(RBTree T, RBNode *x)
{
    RBNode *w;
    while(x!=T->root && x->color == black)
    {
        if(x == x->parent->left)
        {
            w = x->parent->right;
            if(w->color == red) //case 1
            {
                w->color = black;
                x->parent->color = red;
                left_rotate(T,x->parent);
                w = x->parent->right;
            }
            if(w->left->color == black && w->right->color == black) //case 2
            {
                w->color = red;
                x = x->parent;
            }
            else if(w->right->color == black) //case 3
            {
                w->left->color = black;
                w->color = red;
                right_rotate(T,w);
                w = x->parent->right;
            }
            //case 4
            w->color = x->parent->color;
            x->parent->color = black;
            w->right->color = black;
            left_rotate(T,x->parent);
            x = T->root;
        }
        else
        {
            w = x->parent->left;
            if(w->color == red) //case 1
            {
                w->color = black;
                x->parent->color = red;
                left_rotate(T,x->parent);
                w = x->parent->left;
            }
            if(w->left->color == black && w->right->color == black) //case 2
            {
                w->color = red;
                x = x->parent;
            }
            else if(w->left->color == black) //case 3
            {
                w->right->color = black;
                w->color = red;
                right_rotate(T,w);
                w = x->parent->left;
            }
            //case 4
            w->color = x->parent->color;
            x->parent->color = black;
            w->left->color = black;
            left_rotate(T,x->parent);
            x = T->root;
        }
    }
    x->color = black;
}

void RB_delete(RBTree T, RBNode *z)
{
    RBNode *x, *y = z;
    RBColor y_origin_color = y->color;
    if(z->left == T->nil)
    {
        x = z->right;
        RB_transplant(T,z,z->right);
    }
    else if(z->right == T->nil)
    {
        x = z->left;
        RB_transplant(T,z,z->left);
    }
    else
    {
        y = RB_minimum(T, z->right);
                                                                                    //RBNode_print(T,y);
        y_origin_color = y->color;
        x = y->right;
        if(y->parent == z)
        {
            if(x!=T->nil) x->parent = y;
        }
        else
        {
            RB_transplant(T,y,y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_transplant(T,z,y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
        RBNode *temp = y;
        while(y!=T->nil)
        {
            y->size = y->left->size + y->right->size + 1;
            y = y->parent;
        }
    }
    if(y_origin_color == black)
    {
        RB_delete_fixup(T,x);
    }
                                                                                //RBTree_print_preorder(T,z->parent);
                                                                                //RBTree_print_preorder(T,T->root);
}


void RBTree_print_preorder(RBTree T, RBNode *x, FILE *stream)
{
    if(x == T->nil) return;
                                                                         //printf("print preoder begins from key %d\n",x->key);
    RBNode_print(T,x,stream);
    RBTree_print_preorder(T,x->left,stream);
    RBTree_print_preorder(T,x->right,stream);
}

void RBTree_print_inorder(RBTree T, RBNode *x, FILE *stream)
{
    if(x == T->nil) return;
    RBTree_print_inorder(T,x->left,stream);
    RBNode_print(T,x,stream);
    RBTree_print_inorder(T,x->right,stream);
}

void RBTree_print_postorder(RBTree T, RBNode *x, FILE *stream)
{
    if(x == T->nil) return;
    RBTree_print_postorder(T,x->left,stream);
    RBTree_print_postorder(T,x->right,stream);
    RBNode_print(T,x,stream);
}
/*
int test(void)
{
    int keys[10] = {3,62,1,98,32,12,5,55,48,2};
    int i;
    RBTree T=Init_RBTree();
    for(i=0;i<10;i++)
    {
        RBNode *x = Create_RBNode(T,keys[i]);
        RB_insert(T,x);
                                                                             //printf("insert key %d\n",x->key);
                                                                             //RBTree_print_preorder(T,T->root);
    }
    RBTree_print_preorder(T, T->root);
    while((i=T->root->size)>=5)
    {
        RBNode *x = OS_select(T->root,i/3);
        printf("i/3=%d delete rank %d, key %d\n",i/3,OS_rank(T,x),x->key);
        RB_delete(T, x);
        RBTree_print_preorder(T, T->root);
        free(x);
        i--;
        x = OS_select(T->root,i/4);
        printf("i/4=%d delete rank %d, key %d\n",i/4,OS_rank(T,x),x->key);
        RB_delete(T, x);
        RBTree_print_preorder(T, T->root);
        free(x);
    }
    //RBTree_print_preorder(T, T->root);
    Del_RBTree(T,T->root);
    return 0;
}
*/
