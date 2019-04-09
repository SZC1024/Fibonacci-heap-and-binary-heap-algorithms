//优化自newsort.cpp
#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<boost/random.hpp>
#include<fstream>
using namespace std;
const int maximum=100000000;

int randmin=0;
int randmax=10000000;
int mark=0;//用于简易内存管理


int partition(int* A,int p,int r);
void quick_sort(int* A,int p,int r);
void insertion_sort(int* A,int p,int r);
void merge(int* A,int p,int q,int r);
void merge_sort(int* A,int p,int r);
boost::mt19937 gen;
boost::uniform_int<> dist(randmin,randmax);
boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen,dist);




int partition(int* A,int p,int r){//从属于quick_sort
    int x=A[r];
    int i=p-1;
    int temp;
    for(int j=p;j<r;j++){
        if(A[j]<=x){
            i++;
            temp=A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    temp=A[i+1];
    A[i+1]=A[r];
    A[r]=temp;
    return i+1;
}
void quick_sort(int* A,int p,int r){//将A[p]至A[r]的数使用快速排序(从小到大)
    if(p<r){
        int q=partition(A,p,r);
        quick_sort(A,p,q-1);
        quick_sort(A,q+1,r);
    }
}
void insertion_sort(int* A,int p,int r){//将A[p]至A[r]的数使用插入排序(从小到大)
    int temp;
    for(int q=p+1;q<=r;q++){
        temp=A[q];
        int i=q-1;//(revised)
        for(;A[i]>temp&&i>=p;i--){
            A[i+1]=A[i];
        }
        A[i+1]=temp;
    }
}
void quick_sort_2(int* A,int p,int r,int t=10){//改进的quick_sort,依赖于insertion_sort
    if(p<r-t){
        int q=partition(A,p,r);
        quick_sort_2(A,p,q-1,t);
        quick_sort_2(A,q+1,r,t);
    }else{
        insertion_sort(A,p,r);
    }
}
void merge(int* A,int p,int q,int r){//从属于merge_sort
    int n1=q-p+1;
    int n2=r-q;
    int* L=new int[n1+1];
    int* R=new int[n2+1];
    for(int i=0;i<n1;i++){
        L[i]=A[p+i];
    }
    for(int i=0;i<n2;i++){
        R[i]=A[q+i+1];
    }
    L[n1]=maximum;
    R[n2]=maximum;
    for(int i=0,j=0,k=p;k<=r;k++){
        if(L[i]<=R[j]){
            A[k]=L[i];
            i++;
        }else{
            A[k]=R[j];
            j++;
        }
    }
    delete L;
    delete R;
}
void merge_sort(int* A,int p,int r){//将A[p]至A[r]的数使用归并排序(从小到大)
    if(p<r){
        int q=(p+r)/2;
        merge_sort(A,p,q);
        merge_sort(A,q+1,r);
        merge(A,p,q,r);
    }
}
//归并排序的优化需要用到内存管理
int* getintarray(int* B,int size){//简易内存管理,申请
    mark+=size;
    return B+mark-size;
}
void deleteintarray(int* B,int size){//简易内存管理,释放
    mark-=size;
}
void merge_memoryselfcontrol(int* A,int p,int q,int r,int* B){//从属于merge_sort_memoryselfcontrol_1,用到内存管理
    int n1=q-p+1;
    int n2=r-q;
    int* L=getintarray(B,n1+1);
    int* R=getintarray(B,n2+1);
    // int* L=new int[n1+1];
    // int* R=new int[n2+1];
    for(int i=0;i<n1;i++){
        L[i]=A[p+i];
    }
    for(int i=0;i<n2;i++){
        R[i]=A[q+i+1];
    }
    L[n1]=maximum;
    R[n2]=maximum;
    for(int i=0,j=0,k=p;k<=r;k++){
        if(L[i]<=R[j]){
            A[k]=L[i];
            i++;
        }else{
            A[k]=R[j];
            j++;
        }
    }
    deleteintarray(B,n1+n2+2);
}
void merge_sort_memoryselfcontrol_1(int* A,int p,int r,int* B){//将A[p]至A[r]的数使用归并排序(从小到大),用到内存管理,但没有优化递归
    if(p<r){
        int q=(p+r)/2;
        merge_sort_memoryselfcontrol_1(A,p,q,B);
        merge_sort_memoryselfcontrol_1(A,q+1,r,B);
        merge_memoryselfcontrol(A,p,q,r,B);
    }
}
void merge_sort_memoryselfcontrol_2(int* A,int p,int r,int* B,int t=10){//将A[p]至A[r]的数使用归并排序(从小到大),用到内存管理,用到递归优化
    if(p<r-t){
        int q=(p+r)/2;
        merge_sort_memoryselfcontrol_2(A,p,q,B,t);
        merge_sort_memoryselfcontrol_2(A,q+1,r,B,t);
        merge_memoryselfcontrol(A,p,q,r,B);
    }else{
        insertion_sort(A,p,r);
    }
}
void merge_sort_2(int* A,int p,int r,int t=10){//将A[p]至A[r]的数使用归并排序(从小到大),用到内存管理,用到递归优化
    if(p<r-t){
        int q=(p+r)/2;
        merge_sort_2(A,p,q,t);
        merge_sort_2(A,q+1,r,t);
        merge(A,p,q,r);
    }else{
        insertion_sort(A,p,r);
    }
}




int main(){

    clock_t first,finish;
    
    int n;
    cout<<"数组长度:";
    cin>>n;
    int* A;
    A=new int[n];
    int* B=new int[n+2];//用于归并排序,内存管理
    int circle;
    double average=0;
    cout<<"测试次数:";
    cin>>circle;
    ofstream fout("456.txt",ios::trunc);

/*
    //寻找递归排序和插入排序的差距(针对小数组)
    fout<<"array_length insertion_sort merge_sort_memoryselfcontrol_1 quick_sort"<<endl;
    for(int i=1;i<=n;i++){
        cout<<"数组长度:"<<i<<endl;
        fout<<i<<" ";
        average=0;
        for(int j=0;j<circle;j++){
            for(int k=0;k<i;k++){
                A[k]=die();
            }
            first=clock();
            insertion_sort(A,0,i-1);
            finish=clock();
            average+=(finish-first);
        }
        cout<<"insertion_sort:"<<average<<"ms ";
        fout<<average<<" ";
        average=0;
        for(int j=0;j<circle;j++){
            for(int k=0;k<i;k++){
                A[k]=die();
            }
            first=clock();
            merge_sort(A,0,i-1);
            finish=clock();
            average+=(finish-first);
        }
        cout<<"merge_sort_memoryselfcontrol_1:"<<average<<"ms ";
        fout<<average<<" ";
        average=0;
        for(int j=0;j<circle;j++){
            for(int k=0;k<i;k++){
                A[k]=die();
            }
            first=clock();
            quick_sort(A,0,i-1);
            finish=clock();
            average+=(finish-first);
        }
        cout<<"quick_sort:"<<average<<"ms "<<endl;
        fout<<average<<endl;
    }
*/



/*
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }

        first=clock();
        merge_sort_memoryselfcontrol_1(A,0,n-1,B);
        finish=clock();

        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
*/


/*寻找递归排序和插入排序的结合点
    for(int t=70;t>=0;t--){
        average=0;
        for(int i=1;i<=circle;i++){
            for(int j=0;j<n;j++){
                A[j]=die();
            }
            first=clock();
            merge_sort_2(A,0,n-1,t);
            finish=clock();
            average+=(finish-first);
        }
        average=average/circle;
        cout<<"t="<<t<<",平均耗时:"<<average<<"ms"<<endl;
    }
*/
/*
    cout<<"merge_sort:"<<endl;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_memoryselfcontrol_1"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_memoryselfcontrol_1(A,0,n-1,B);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
*/
    /*
    first=clock();

    quicksort(A,0,n-1);

    finish=clock();

    cout<<finish-first<<endl;


    */

    cout<<"quick_sort:"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        quick_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"quick_sort_2:"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        quick_sort_2(A,0,n-1,25);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort:"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_2"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_2(A,0,n-1,43);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_memoryselfcontrol_2"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_memoryselfcontrol_2(A,0,n-1,B,43);
        finish=clock();
        average+=(finish-first);
        cout<<"第"<<i<<"次排序完毕，耗时:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"次平均用时:"<<average<<"ms"<<endl;







    system("pause");
    return 0;
}