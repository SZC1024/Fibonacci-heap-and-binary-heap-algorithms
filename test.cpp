#include<iostream>
#include<vector>
#include<cstdlib>
#include<queue>
#include<ctime>
#include<fstream>
using namespace std;

queue<int> iq;
/*
void guibing(int p,int r,int t=0){
    int q=(p+r)/2;
    cout<<p<<"-"<<r;
    if(p<r-t){
        guibing(p,q);
        guibing(q+1,r);
    }
}
*/ 

int main(){
    int n;
    cin>>n;
    int i,j;
    int temp;
    ofstream fout("test.txt",ios::trunc);
    for(int t=0;t<140;t++){
        temp=0;
        fout<<t<<" ";
        iq.push(0);
        iq.push(n-1);
        while(!iq.empty()){
            i=iq.front();
            iq.pop();
            j=iq.front();
            iq.pop();
            //cout<<i<<"-"<<j<<"  ";
            if(i<j-t){
                iq.push(i);
                iq.push((i+j)/2);
                iq.push((i+j)/2+1);
                iq.push(j);
            }else{
                temp++;
            }
        }
        //cout<<endl;
        fout<<temp<<endl;
    }
/*
    for(int t=0;t<140;t++){
        temp=0;
        fout<<"t="<<t<<endl;
        iq.push(0);
        iq.push(n-1);
        while(!iq.empty()){
            i=iq.front();
            iq.pop();
            j=iq.front();
            iq.pop();
            //cout<<i<<"-"<<j<<"  ";
            if(i<j-t){
                iq.push(i);
                iq.push((i+j)/2);
                iq.push((i+j)/2+1);
                iq.push(j);
            }else{
                temp++;
            }
        }
        //cout<<endl;
        fout<<"Ҷ�ӽڵ���="<<temp<<endl;
        fout<<"------------------------"<<endl;
    }
*/

    cout<<"end"<<endl;
    system("pause");
    return 0;
}