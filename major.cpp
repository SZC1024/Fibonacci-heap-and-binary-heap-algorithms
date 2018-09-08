//�Ż���newsort.cpp
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
int mark=0;//���ڼ����ڴ����


int partition(int* A,int p,int r);
void quick_sort(int* A,int p,int r);
void insertion_sort(int* A,int p,int r);
void merge(int* A,int p,int q,int r);
void merge_sort(int* A,int p,int r);
boost::mt19937 gen;
boost::uniform_int<> dist(randmin,randmax);
boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen,dist);




int partition(int* A,int p,int r){//������quick_sort
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
void quick_sort(int* A,int p,int r){//��A[p]��A[r]����ʹ�ÿ�������(��С����)
    if(p<r){
        int q=partition(A,p,r);
        quick_sort(A,p,q-1);
        quick_sort(A,q+1,r);
    }
}
void insertion_sort(int* A,int p,int r){//��A[p]��A[r]����ʹ�ò�������(��С����)
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
void quick_sort_2(int* A,int p,int r,int t=10){//�Ľ���quick_sort,������insertion_sort
    if(p<r-t){
        int q=partition(A,p,r);
        quick_sort_2(A,p,q-1,t);
        quick_sort_2(A,q+1,r,t);
    }else{
        insertion_sort(A,p,r);
    }
}
void merge(int* A,int p,int q,int r){//������merge_sort
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
void merge_sort(int* A,int p,int r){//��A[p]��A[r]����ʹ�ù鲢����(��С����)
    if(p<r){
        int q=(p+r)/2;
        merge_sort(A,p,q);
        merge_sort(A,q+1,r);
        merge(A,p,q,r);
    }
}
//�鲢������Ż���Ҫ�õ��ڴ����
int* getintarray(int* B,int size){//�����ڴ����,����
    mark+=size;
    return B+mark-size;
}
void deleteintarray(int* B,int size){//�����ڴ����,�ͷ�
    mark-=size;
}
void merge_memoryselfcontrol(int* A,int p,int q,int r,int* B){//������merge_sort_memoryselfcontrol_1,�õ��ڴ����
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
void merge_sort_memoryselfcontrol_1(int* A,int p,int r,int* B){//��A[p]��A[r]����ʹ�ù鲢����(��С����),�õ��ڴ����,��û���Ż��ݹ�
    if(p<r){
        int q=(p+r)/2;
        merge_sort_memoryselfcontrol_1(A,p,q,B);
        merge_sort_memoryselfcontrol_1(A,q+1,r,B);
        merge_memoryselfcontrol(A,p,q,r,B);
    }
}
void merge_sort_memoryselfcontrol_2(int* A,int p,int r,int* B,int t=10){//��A[p]��A[r]����ʹ�ù鲢����(��С����),�õ��ڴ����,�õ��ݹ��Ż�
    if(p<r-t){
        int q=(p+r)/2;
        merge_sort_memoryselfcontrol_2(A,p,q,B,t);
        merge_sort_memoryselfcontrol_2(A,q+1,r,B,t);
        merge_memoryselfcontrol(A,p,q,r,B);
    }else{
        insertion_sort(A,p,r);
    }
}
void merge_sort_2(int* A,int p,int r,int t=10){//��A[p]��A[r]����ʹ�ù鲢����(��С����),�õ��ڴ����,�õ��ݹ��Ż�
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
    cout<<"���鳤��:";
    cin>>n;
    int* A;
    A=new int[n];
    int* B=new int[n+2];//���ڹ鲢����,�ڴ����
    int circle;
    double average=0;
    cout<<"���Դ���:";
    cin>>circle;
    ofstream fout("456.txt",ios::trunc);

/*
    //Ѱ�ҵݹ�����Ͳ�������Ĳ��(���С����)
    fout<<"array_length insertion_sort merge_sort_memoryselfcontrol_1 quick_sort"<<endl;
    for(int i=1;i<=n;i++){
        cout<<"���鳤��:"<<i<<endl;
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
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }

        first=clock();
        merge_sort_memoryselfcontrol_1(A,0,n-1,B);
        finish=clock();

        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
*/


/*Ѱ�ҵݹ�����Ͳ�������Ľ�ϵ�
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
        cout<<"t="<<t<<",ƽ����ʱ:"<<average<<"ms"<<endl;
    }
*/
/*
    cout<<"merge_sort:"<<endl;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_memoryselfcontrol_1"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_memoryselfcontrol_1(A,0,n-1,B);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
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
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        quick_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"quick_sort_2:"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        quick_sort_2(A,0,n-1,25);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort:"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort(A,0,n-1);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_2"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_2(A,0,n-1,43);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;
    cout<<"---------------------------------------------------"<<endl;
    cout<<"merge_sort_memoryselfcontrol_2"<<endl;
    average=0;
    for(int i=1;i<=circle;i++){
        cout<<"��"<<i<<"�β���:";
        for(int j=0;j<n;j++){
            A[j]=die();
        }
        first=clock();
        merge_sort_memoryselfcontrol_2(A,0,n-1,B,43);
        finish=clock();
        average+=(finish-first);
        cout<<"��"<<i<<"��������ϣ���ʱ:"<<finish-first<<"ms"<<endl;
    }
    average=average/circle;
    cout<<circle<<"��ƽ����ʱ:"<<average<<"ms"<<endl;







    system("pause");
    return 0;
}