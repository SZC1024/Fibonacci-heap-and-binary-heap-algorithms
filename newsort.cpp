#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<boost/random.hpp>
using namespace std;
int partition(int* A,int p,int r);
void quick_sort(int* A,int p,int r);
void insertion_sort(int* A,int p,int r);

boost::mt19937 gen;
boost::uniform_int<> dist(1,1000000);
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
        for(int i=q;i>p;i--){
            if(A[i-1]>temp){
                A[i]=A[i-1];
            }else{
                A[i]=temp;
                break;
            }
        }
    }
}
void quick_sort_2(int* A,int p,int r,int t){//改进的quick_sort,依赖于insertion_sort
    if(p<r-t){
        int q=partition(A,p,r);
        quick_sort_2(A,p,q-1,t);
        quick_sort_2(A,q+1,r,t);
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
    int circle;
    double average=0;
    cout<<"测试次数:";
    cin>>circle;


/*寻找快速排序和插入排序的结合点
    for(int t=0;t<=50;t++){
        average=0;
        for(int i=1;i<=circle;i++){
            for(int j=0;j<n;j++){
                A[j]=die();
            }
            first=clock();
            quick_sort_2(A,0,n-1,t);
            finish=clock();
            average+=(finish-first);
        }
        average=average/circle;
        cout<<"t="<<t<<",平均耗时:"<<average<<"ms"<<endl;
    }
*/




/*
    cout<<"quicksort:"<<endl;
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
    cout<<"quick_sort_2"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"第"<<i<<"次测试:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        quick_sort_2(A,0,n-1);
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
    system("pause");
    return 0;
}