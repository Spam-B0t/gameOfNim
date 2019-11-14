/*
  gameOfNim
 */
#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;

#define abs(x) ((x)>=0) ? (x):-(x)


//class Bot;
class Field{
    public:
        int len, *pa, nhlen, *nheaps;
        
        Field(int l, int *a){len=l; pa=a;}
        
        void mam(){//make a move
            int x, n;
            cout<<"Enter heap num and stone quantity: ";
            cin>>n>>x; n--;
            if(-1<n && n<len){
                if(0<x && x<=pa[n]){
                    pa[n]-=x;
                    tryemp(n);
                }
                else{
                    cout<<"This heap has only "<<pa[n]<<" stones\n";
                    mam();
                }
            }
            else{
                cout<< "There are only "<<len<<" heaps\n";
                mam();
            }
        }
        
        void tryemp(int n){if(pa[n]==0){pa[n]=pa[0]; pa++; len--;}}
};

class Bot{
    public:
        Field *f;
        Bot(Field *field){f=field;}
        void bm(int n, int x){//bot move
            cout<<"Enter heap num and stone quantity: "<<n+1<<" "<<x<<"\n";
            f->pa[n]-=x; f->tryemp(n);
        }
        void botE(){//bot Easy
            srand(time(NULL));
            int n=rand()%(f->len), x=rand()%(f->pa[n])+1;
            bm(n, x);
        }
        void botD(){//bot Difficult
            if(f->pa[0]==f->pa[1])bm(1, 1); //TO DO: extend field size
            else if(f->pa[0]==0)bm(2, f->pa[1]);
            else if(f->pa[1]==0)bm(1, f->pa[0]);
            else if(f->pa[0]>f->pa[1])bm(1, f->pa[0]-f->pa[1]);
            else if(f->pa[1]>f->pa[0])bm(2, f->pa[1]-f->pa[0]);
        }
        void botM(){//bot Medium
            srand(time(NULL));
            int n=rand()%2;
            if(n%2==0)botE();
            else botD();
        }
        void bmam(int n){//bot make a move
            if(n==1)botE();
            else if(n==2)botM();
            else if(n==3)botD();
        }
};

void playNim(Field *f, int pn, int *players, Bot *bot){
            int n=-1, i;
            while(f->len>0){
                n=(n+1)%pn;
                cout<<"Heaps:";
                for(i=0; i<(f->len); i++)cout<<" "<<(f->pa[i]);
                cout<<"\nplayer's "<<n+1<<" turn\n";
                if(players[n]==0)f->mam();
                else bot->bmam(players[n]);
            }
            cout<<"player "<<n+1<<" won";
        }

void start(){
    int pn, size;
    cout<<"How many players are there? ";
    cin>>pn; pn=abs(pn);
    int p[pn];//player data
    cout<<"0 - human, 1 - AI easy, 2 - AI normal, 3 - AI hard\n";
    for(int i=0; i<pn; i++){
        cout<<"Who is player "<<i+1<<"? ";
        cin>>p[i]; p[i]=abs(p[i])%4;
    }
    cout<<"With how many heaps do you wish to play? ";
    cout<<"\n*:currently AI hard&normal can play only with 2.\n";
    cin>>size; size=abs(size); if(size==0)size++;
    int stones[size];
    for(int i=0; i<size; i++){
        cout<<"How many stones are in a heap #"<<i+1<<"? ";
        cin>>stones[i]; stones[i]=abs(stones[i]);
        if(stones[i]==0)stones[i]++;
    }
    Field f(size, stones); Bot bot(&f);
    playNim(&f, pn, &p[0], &bot);
    }


int main(){
    start();
}
