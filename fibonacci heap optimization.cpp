#include<iostream>
#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ctime>
using namespace std;
const double minimum=1.7e-308;//changed
const double maximum=1.7e308;
const char *input="123.txt";
const char *output="output.txt";
struct fibonacci_heap_node{
    double key;
    int degree;
    bool mark;
    int vertex;//changed
    fibonacci_heap_node* parent;
    fibonacci_heap_node* left;
    fibonacci_heap_node* right;
    fibonacci_heap_node* child;
};
struct fibonacci_heap_head{
    fibonacci_heap_node* min;
    int n;
    fibonacci_heap_head(){
        n=0;
        min=NULL;
    }
};
//同样约定，结点从1开始编号
struct edge{
    int vertex;
    double key;
    edge* next;
    edge(int v,double k,edge* e=NULL){
        vertex=v;
        key=k;
        next=e;
    }
};
int fib_log_size;
fibonacci_heap_node** fib_B;
fibonacci_heap_node* sentinel;

void matrix_to_mapchain(double** matrix,edge** E,int size);
double** get_matrix(int size);
void format_matrix(double** matrix,int size);
fibonacci_heap_head* make_fib_heap();
fibonacci_heap_node* fib_heap_insert(fibonacci_heap_head* H,double k,int vertex);
void fib_heap_insert(fibonacci_heap_head* H,fibonacci_heap_node* x);
fibonacci_heap_head* fib_heap_union(fibonacci_heap_head* H1,fibonacci_heap_head* H2);
fibonacci_heap_node* fib_heap_extract_min(fibonacci_heap_head* H);
void consolidate_without_sentinel(fibonacci_heap_head* H);
void consolidate_with_sentinel(fibonacci_heap_head* H);
void fib_heap_link(fibonacci_heap_head* H,fibonacci_heap_node* y,fibonacci_heap_node* x);
void fib_heap_decrease_key(fibonacci_heap_head* H,fibonacci_heap_node* x,double k);
void cut(fibonacci_heap_head* H,fibonacci_heap_node* x,fibonacci_heap_node* y);
void cascading_cut(fibonacci_heap_head* H,fibonacci_heap_node* y);
fibonacci_heap_node* fib_heap_delete(fibonacci_heap_head* H,fibonacci_heap_node* x);
void initialize(fibonacci_heap_head* H,fibonacci_heap_node** relate,int source,int size);
void dijkstra(fibonacci_heap_head* H,edge** E,double** matrix,fibonacci_heap_node** relate,int source,int size);
void fib_heap_print_vertex(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep);
void fib_heap_print_parent(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep);
void fib_heap_print_key(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep);
void print_mapchain(edge** E,int size);
void print_shortest_value(fibonacci_heap_node** relate,int source,int size);
void fib_heap_print_vertex_to_txt(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep);
//内存动态管理,没用到
fibonacci_heap_node** get_B(){
    return fib_B;
}
fibonacci_heap_node* get_fib_sentinel(){
    return sentinel;
}

//预处理函数
void matrix_to_mapchain(double** matrix,edge** E,int size){//并且将原来从0开始编号的矩阵转换为从1开始编号的邻接链表
    for(int i=0;i<size+1;i++){
        E[i]=NULL;
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i!=j&&matrix[i][j]!=maximum){
                E[i+1]=new edge(j+1,matrix[i][j],E[i+1]);//链条头插
            }
        }
    }
}
double** get_matrix(int size){//from 123.txt
    double **matrix=new double*[size];
    for(int i=0;i<size;i++){
        matrix[i]=new double[size];
    }
    ifstream fin(input);//matrix从文件写入
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            fin>>matrix[i][j];
        }
    }
    return matrix;
}
void format_matrix(double** matrix,int size){
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i==j) matrix[i][j]=0;
            else if(matrix[i][j]==0) matrix[i][j]=maximum;
        }
    }
}
//堆操作函数
fibonacci_heap_head* make_fib_heap(){
    return new fibonacci_heap_head();
}
fibonacci_heap_node* fib_heap_insert(fibonacci_heap_head* H,double k,int vertex=0){//changed//外部只需要提供一个key值即可，其他的均不用管
    fibonacci_heap_node* x=new fibonacci_heap_node();
    x->key=k;
    x->degree=0;
    x->parent=NULL;
    x->child=NULL;
    x->mark=false;
    x->vertex=vertex;//changed
    if(H->min==NULL){
        //create a root list for H containing just x(已完成)
        x->left=x;
        x->right=x;
        H->min=x;
    }else{
        //insert x into H's root list(左插，在H->min左侧)
        //逻辑点1
        x->left=H->min->left;
        x->right=H->min;
        H->min->left->right=x;
        H->min->left=x;
        if(x->key<H->min->key){
            H->min=x;
        }
    }
    (H->n)++;
    return x;
}
void fib_heap_insert(fibonacci_heap_head* H,fibonacci_heap_node* x){//外部需要申请节点x，并对x的key和vertex(//changed)做修改，其他的均不用管
    x->degree=0;
    x->parent=NULL;
    x->child=NULL;
    x->mark=false;
    if(H->min==NULL){
        //create a root list for H containing just x(已完成)
        x->left=x;
        x->right=x;
        H->min=x;
    }else{
        //insert x into H's root list(左插，在H->min左侧)
        //逻辑点1
        x->left=H->min->left;
        x->right=H->min;
        H->min->left->right=x;
        H->min->left=x;
        if(x->key<H->min->key){
            H->min=x;
        }
    }
    (H->n)++;
}
fibonacci_heap_head* fib_heap_union(fibonacci_heap_head* H1,fibonacci_heap_head* H2){//应用在最短路径上时，需注意结点序号vertex不可重复
    //和算法导论上的不一样，算导上的没有显式处理H1->min为空时的情况
    if(H1->min!=NULL&&H2->min!=NULL){
        //逻辑点2
        H1->min->right->left=H2->min->left;
        H2->min->left->right=H1->min->right;
        H1->min->right=H2->min;
        H2->min->left=H1->min;
        if(H1->min->key>H2->min->key){
            (H2->n)+=(H1->n);
            delete H1;
            return H2;
        }else{
            (H1->n)+=(H2->n);
            delete H2;
            return H1;
        }
    }else{
        if(H1->min==NULL){
            delete H1;
            return H2;
        }else{
            delete H2;
            return H1;
        }
    }
}
fibonacci_heap_node* fib_heap_extract_min(fibonacci_heap_head* H){
    fibonacci_heap_node* z=H->min;
    if(z!=NULL){
        fibonacci_heap_node* c=z->child;
        for(int i=0;i<z->degree;i++,c=c->right){//循环条件已经确保了不会出现内存越界访问
            c->parent=NULL;
        }
        //逻辑点3
        //注意和算法导论上的不一样，对于怎么处理孩子节点往根链表中插入，不同的处理方法和顺序影响到紧接着的if语句判断条件
        //add c to the root list of H(c链条将z顶掉)
        //remove z from the root list of H(已完成)
        if(c!=NULL&&z!=z->right){//revised
            z->left->right=c;
            z->right->left=c->left;
            c->left->right=z->right;
            c->left=z->left;
            H->min=z->right;
        }
        //特殊情况处理(revised)
        else if(z==z->right&&c==NULL){//也可以把z->child写成c
            H->min=NULL;
        }else if(z==z->right&&c!=NULL){
            H->min=c;
        }else if(z!=z->right&&c==NULL){
            z->left->right=z->right;
            z->right->left=z->left;
            H->min=z->right;
        }
        consolidate_with_sentinel(H);
        //or consolidate_without_sentinel(H);
        (H->n)--;
    }
    return z;
}
void consolidate_without_sentinel(fibonacci_heap_head* H){//没有哨兵的根链表，consolidate的实现方法，和算法导论上的不一样，区别主要在for each w的遍历上和最后根链表的整理
    if(H->min==NULL) return;
    int size=2*log(H->n)/log(2)+1;//需验证H的最大度数的界(已验证)
    //这里的size还是要大一些为好，因为dijkstra频繁的decrease key会造成结点的度局部堆积，从而大度结点遗留下来，在问题规模变小的时候出现内存越界访问
    fibonacci_heap_node* A[size];
    //也可以声明在堆中，不过实际上不需要，除非大于size一百万
    //fibonacci_heap_node** A=new fibonacci_heap_node*[size];
    for(int i=0;i<size;i++){
        A[i]=NULL;
    }
    int d;
    fibonacci_heap_node *w,*x,*y,*temp;//循环不变量w始终为正在访问的节点的下一个，也就是恰好在边界外，x,y,temp均在边界内，边界即为处理边界
    for(w=H->min->right,A[w->left->degree]=w->left;A[w->degree]!=w;w=w->right){//最重要的是怎么实现for each node w in the root list of H(我写的神之一笔！)
        x=w->left;
        d=x->degree;
        while(A[d]!=NULL&&A[d]!=x){
            y=A[d];
            if(x->key>y->key){//exchange x with y
                temp=x;
                x=y;
                y=temp;
            }
            fib_heap_link(H,y,x);
            A[d++]=NULL;
        }
        A[d]=x;
    }
    {//因为没有哨兵，所以要多执行一次循环体内的代码
        x=w->left;
        d=x->degree;
        while(A[d]!=NULL&&A[d]!=x){
            y=A[d];
            if(x->key>y->key){//exchange x with y
                temp=x;
                x=y;
                y=temp;
            }
            fib_heap_link(H,y,x);
            A[d++]=NULL;
        }
        A[d]=x;
    }
    int i=0;
    fibonacci_heap_node *point,*mark;
    while(A[i]==NULL) i++;//利用A找到第一个根节点，之后利用fib_heap_link时维护好的根链表关系找到最小key，比书上只利用遍历A重建根链表效率高
    for(H->min=mark=point=A[i];point->right!=mark;point=point->right){
        if(point->key<H->min->key){
            H->min=point;
        }
    }
    {//同样，因为没有哨兵，要多执行一次循环体内的代码
        if(point->key<H->min->key){
            H->min=point;
        }
    }
}
void consolidate_with_sentinel(fibonacci_heap_head* H){//和算法导论上的不一样，区别主要在for each w的遍历上和最后根链表的整理
    if(H->min==NULL) return;//revised
    //int size=2*log(H->n)/log(2)+1;//需验证H的最大度数的界(已验证)
    //这里的size还是要大一些为好，因为dijkstra频繁的decrease key会造成结点的度局部堆积，从而大度结点遗留下来，在问题规模变小的时候出现内存越界访问
    fibonacci_heap_node** A=fib_B;
    //也可以声明在堆中，不过实际上不需要，除非大于size一百万
    //fibonacci_heap_node** A=new fibonacci_heap_node*[size];
    for(int i=0;i<fib_log_size;i++){
        A[i]=NULL;
    }
    int d;
    fibonacci_heap_node *w,*x,*y,*temp;//循环不变量w始终为正在访问的节点的下一个，也就是恰好在边界外，x,y,temp均在边界内，边界即为处理边界
    //fibonacci_heap_node* sentinel=fib_heap_insert(H,maximum,0);//在H->min左侧插入哨兵用于之后的循环
    if(H->min==NULL){
        //create a root list for H containing just sentinel(已完成)
        sentinel->left=sentinel;
        sentinel->right=sentinel;
        H->min=sentinel;
    }else{
        //insert sentinel into H's root list(左插，在H->min左侧)
        //逻辑点1
        sentinel->left=H->min->left;
        sentinel->right=H->min;
        H->min->left->right=sentinel;
        H->min->left=sentinel;
        if(sentinel->key<H->min->key){
            H->min=sentinel;
        }
    }
    (H->n)++;
    //恰好循环是从H->min开始向右访问，所以sentinel的位置恰好是循环的最后一次，并且不参与进循环体中，也不会进入处理边界内
    for(w=H->min->right,A[w->left->degree]=w->left;A[w->degree]!=w;w=w->right){//最重要的是怎么实现for each node w in the root list of H(我写的神之一笔！)
        x=w->left;
        d=x->degree;
        while(A[d]!=NULL&&A[d]!=x){
            y=A[d];
            if(x->key>y->key){//exchange x with y
                temp=x;
                x=y;
                y=temp;
            }
            fib_heap_link(H,y,x);
            A[d++]=NULL;
        }
        A[d]=x;
    }
    fibonacci_heap_node *point,*mark;
    for(H->min=mark=point=sentinel->right;point->right!=mark;point=point->right){//利用sentinel找到根链表，直接开始遍历，和算法导论上的不同
        if(point->key<H->min->key){
            H->min=point;
        }
    }
    //删掉哨兵sentinel
    sentinel->right->left=sentinel->left;
    sentinel->left->right=sentinel->right;
    (H->n)--;
    //delete sentinel;
}
void fib_heap_link(fibonacci_heap_head* H,fibonacci_heap_node* y,fibonacci_heap_node* x){//内部非递归简单函数其实可以直接写在父函数中，减少函数调用开销
    //remove y from the root(any) list of H(y isn't H.min)//changed
    if(y->right!=y){//y isn't single
        y->right->left=y->left;
        y->left->right=y->right;
        if(y->parent!=NULL){
            if(y->parent->child==y){
                y->parent->child=y->right;
            }
            y->parent->degree--;
        }
    }else{//y is single(y must not in root)
        y->parent->child=NULL;
        y->parent->degree--;
    }
    //make y a child of x,incrementing x->degree
    if(x->child==NULL){
        x->child=y;
        y->left=y;
        y->right=y;
        y->parent=x;
    }else{
        //逻辑点
        y->left=x->child->left;
        y->right=x->child;
        x->child->left->right=y;
        x->child->left=y;
        y->parent=x;
    }
    (x->degree)++;
    y->mark=false;
}
void fib_heap_decrease_key(fibonacci_heap_head* H,fibonacci_heap_node* x,double k){
    if(k<x->key){
        x->key=k;
        fibonacci_heap_node* y=x->parent;
        if(y!=NULL&&x->key<y->key){
            cut(H,x,y);
            cascading_cut(H,y);
        }
        if(x->key<H->min->key){
            H->min=x;
        }
    }
}
void cut(fibonacci_heap_head* H,fibonacci_heap_node* x,fibonacci_heap_node* y){//切断，从属于fib_heap_decrease_key
    //remove x from the child list of y,decrementing y->degree
    if(x->right==x){//remove
        y->child=NULL;
    }else{
        x->right->left=x->left;
        x->left->right=x->right;
        if(y->child==x){//revised
            y->child=x->right;
        }
    }
    y->degree--;
    //add x to the root list of H(左插)
    //逻辑点(有错误)(revised)
    x->left=H->min->left;
    x->right=H->min;
    H->min->left->right=x;
    H->min->left=x;
    x->parent=NULL;
    x->mark=false;
}
void cascading_cut(fibonacci_heap_head* H,fibonacci_heap_node* y){//级联切断，从属于fib_heap_decrease_key
    fibonacci_heap_node* z=y->parent;
    if(z!=NULL){
        if(y->mark==false){
            y->mark=true;
        }else{
            cut(H,y,z);
            cascading_cut(H,z);
        }
    }
}
fibonacci_heap_node* fib_heap_delete(fibonacci_heap_head* H,fibonacci_heap_node* x){//删除结点，不是单纯的删掉，用到decrease key和extract min联合操作
    fib_heap_decrease_key(H,x,minimum);
    return fib_heap_extract_min(H);
}
//主算法函数
void initialize(fibonacci_heap_head* H,fibonacci_heap_node** relate,int source,int size){//建堆，relate数据关联
    for(int i=1;i<=size;i++){
        relate[i]=fib_heap_insert(H,maximum,i);//在fibonacci堆实现的最小优先队列中，relate一旦设置好，就不用再更改
    }
    fib_heap_decrease_key(H,relate[source],0);
}
void dijkstra(fibonacci_heap_head* H,edge** E,double** matrix,fibonacci_heap_node** relate,int source,int size){
    //initialize(H,relate,source,size);
    fibonacci_heap_node* min;
    edge* temp;
    while(H->n>0){
        min=fib_heap_extract_min(H);
        for(temp=E[min->vertex];temp!=NULL;temp=temp->next){
            fib_heap_decrease_key(H,relate[temp->vertex],relate[min->vertex]->key+matrix[min->vertex-1][temp->vertex-1]);
        }
    }
}
//额外显示函数
void fib_heap_print_vertex(fibonacci_heap_head* H,fibonacci_heap_node* y=NULL,int deep=1){//右向遍历斐波那契堆，并递归输出，输出格式（下兄弟，右孩子）
    static bool mark=false;
    if(y!=NULL){
        if(deep==1){
            fibonacci_heap_node* x=H->min;
            for(;x!=H->min->left;x=x->right){
                printf("%-7d ",x->vertex);mark=false;
                fib_heap_print_vertex(H,x->child,deep+1);
            }
            {
                printf("%-7d ",x->vertex);mark=false;
                fib_heap_print_vertex(H,x->child,deep+1);
            }
        }else{
            for(int i=0;i<y->parent->degree;i++,y=y->right){
                if(i!=0){
                    for(int j=1;j<deep;j++){
                        cout<<"        ";
                    }
                }
                printf("%-7d ",y->vertex);mark=false;
                fib_heap_print_vertex(H,y->child,deep+1);
            }
        }
    }
    if(mark==false){
        cout<<endl;
        mark=true;
    }
}
void fib_heap_print_parent(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep=1){
    static bool mark=false;
    if(y!=NULL){
        if(deep==1){
            fibonacci_heap_node* x=H->min;
            for(;x!=H->min->left;x=x->right){
                if(x->parent!=NULL){
                    cout<<x->parent<<"  ";mark=false;
                }
                else{
                    cout<<x->parent<<"          ";mark=false;
                }
                fib_heap_print_parent(H,x->child,deep+1);
            }
            {
                if(x->parent!=NULL){
                    cout<<x->parent<<"  ";mark=false;
                }
                else{
                    cout<<x->parent<<"          ";mark=false;
                }
                fib_heap_print_parent(H,x->child,deep+1);
            }
        }
        else{
            for(int i=0;i<y->parent->degree;i++,y=y->right){
                if(i!=0){
                    for(int j=1;j<deep;j++){
                        cout<<"           ";
                    }
                }
                if(y->parent!=NULL){
                    cout<<y->parent<<"  ";mark=false;
                }
                else{
                    cout<<y->parent<<"          ";mark=false;
                }
                fib_heap_print_parent(H,y->child,deep+1);
            }
        }
    }
    if(mark==false){
        cout<<endl;
        mark=true;
    }
}
void fib_heap_print_key(fibonacci_heap_head* H,fibonacci_heap_node* y,int deep=1){
    static bool mark=false;
    if(y!=NULL){
        if(deep==1){
            fibonacci_heap_node* x=H->min;
            for(;x!=H->min->left;x=x->right){
                printf("%e  ",x->key);mark=false;
                fib_heap_print_key(H,x->child,deep+1);
            }
            {
                printf("%e  ",x->key);mark=false;
                fib_heap_print_key(H,x->child,deep+1);
            }
        }
        else{
            for(int i=0;i<y->parent->degree;i++,y=y->right){
                if(i!=0){
                    for(int j=1;j<deep;j++){
                        cout<<"               ";
                    }
                }
                printf("%e  ",y->key);mark=false;
                fib_heap_print_key(H,y->child,deep+1);
            }
        }
    }
    if(mark==false){
        cout<<endl;
        mark=true;
    }
}
void print_mapchain(edge** E,int size){
    for(int i=0;i<size+1;i++){
        cout<<i<<" --> ";
        for(edge* temp=E[i];temp!=NULL;temp=temp->next){
            cout<<temp->vertex<<"-"<<temp->key<<" --> ";
        }
        cout<<endl;
    }
}
void print_shortest_value(fibonacci_heap_node** relate,int source,int size){
    for(int i=1;i<=size;i++){
        cout<<source<<"->"<<i<<":"<<relate[i]->key<<endl;
    }
}
int main(){
    clock_t first,finish;
    int size,source;
    cin>>size;
    cin>>source;
    //double A[size+1];//存最后结果，其实没用，因为可以从relate中得到
    //int Atorelate[size+1];//其实没用，可以用A的i值来代表结点编号，只不过用Atorelate增强了鲁棒性而已
    fibonacci_heap_node* fib_relate[size+1];//下标对应图中结点号，如果结点冲突（出现在union时），可以直接用Atorelate来代替下标存储结点号
    edge** E=new edge*[size+1];
    double **matrix=get_matrix(size);
    format_matrix(matrix,size);
    matrix_to_mapchain(matrix,E,size);
    fibonacci_heap_head* fib_H=make_fib_heap();

    initialize(fib_H,fib_relate,source,size);

    {
        fib_log_size=2*log(fib_H->n)/log(2)+1;
        fib_B=new fibonacci_heap_node*[size];
        sentinel=new fibonacci_heap_node;
        sentinel->degree=0;
        sentinel->parent=NULL;
        sentinel->child=NULL;
        sentinel->mark=false;
        sentinel->key=maximum;
        sentinel->vertex=0;
    }

    first=clock();

    dijkstra(fib_H,E,matrix,fib_relate,source,size);

    //print_shortest_value(fib_relate,source,size);

    finish=clock();

    cout<<"use time"<<finish-first<<"ms"<<endl;



/*
    initialize(H,relate,source,size);
    fib_heap_print_vertex(H,H->min);
    cout<<"----------------------------------------"<<endl;
    consolidate_with_sentinel(H);
    freopen(output,"a+",stdout);//输出重定向
    fib_heap_print_vertex(H,H->min);
    freopen("CON","w",stdout);//定向回来
*/
/*
    print_mapchain(E,size);
    cout<<endl;
    initialize(H,relate,source,size);
    fib_heap_print_vertex(H,H->min);
    cout<<"--------------------------------"<<endl;

    // fib_heap_decrease_key(H,relate[6],-8);
    // fib_heap_print(H);

    fibonacci_heap_node* min;
    edge* temp;
    int re;
    while(H->n>0){
        min=fib_heap_extract_min(H);
        for(temp=E[min->vertex];temp!=NULL;temp=temp->next){
            re=fib_heap_decrease_key(H,relate[temp->vertex],relate[min->vertex]->key+matrix[min->vertex-1][temp->vertex-1]);
        }
        cout<<"--------------------------------"<<endl;
        fib_heap_print_vertex(H,H->min);
        cout<<"--------------------------------"<<endl;
        fib_heap_print_parent(H,H->min);
        cout<<"--------------------------------"<<endl;
        fib_heap_print_key(H,H->min);
        cout<<endl;
    }
    print_shortest_value(relate,source,size);
*/
    system("pause");
    return 0;
}
