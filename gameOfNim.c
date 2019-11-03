#include<stdio.h>
#include<stdlib.h> //stdlib rand() is necessary for "botE" & "botM",
#include<time.h>   //otherwise can work without it
                   //time.h is needed for rand() seeding
#define abs(x) (x)>=0 ? (x):-(x)
//game of nim

struct field{
    int len, *pa;
};

struct field buildf(int len, int *a){//playfield constructor
    struct field f; f.len=len; f.pa=&a[0];
    return f;
}

void mam(struct field f){//make a move
    int x, n;
    printf("Enter heap num and stone quantity: ");
    scanf("%d %d", &n, &x); n--;
    if(-1<n && n<f.len){
        if(0<x && x<=f.pa[n])f.pa[n]-=x;
        else{
            printf("This heap have only %d stones\n", f.pa[n]);
            mam(f);
        }
    }
    else{
        printf("There is only %d heaps\n", f.len);
        mam(f);
    }
}

void bm(struct field f, int n, int x){//bot move
    printf("Enter heap num and stone quantity: %d %d\n", n, x);
    n--;
    f.pa[n]-=x;
}

void botE(struct field f){//bot Easy
    int n, x;             //TO DO: extend field size
    srand(time(NULL));
    if(f.pa[0]==0)n=2;
    else if(f.pa[1]==0)n=1;
    else n=rand()%f.len+1;
    x=rand()%f.pa[n-1]+1;
    bm(f, n, x);
}

void botD(struct field f){//bot Difficult
    if(f.pa[0]==f.pa[1])bm(f, 1, 1); //TO DO: extend field size
    else if(f.pa[0]==0)bm(f, 2, f.pa[1]);
    else if(f.pa[1]==0)bm(f, 1, f.pa[0]);
    else if(f.pa[0]>f.pa[1])bm(f, 1, f.pa[0]-f.pa[1]);
    else if(f.pa[1]>f.pa[0])bm(f, 2, f.pa[1]-f.pa[0]);
}

void botM(struct field f){//bot Medium
    int n=rand()%2;
    if(n%2==0)botE(f);
    else botD(f);
}

int nonEmptyHeaps(struct field f){//!!!it's faster to use counter
    int i, n=0;
    for(i=0; i<f.len; i++)if(f.pa[i]>0)n++;
    return n;
}

void playNim(struct field f, int pn, int *players){
    int n=-1, i;
    while(nonEmptyHeaps(f)>0){//TO DO: add counter instead
        n=(n+1)%pn;
        printf("Heaps:");
            for(i=0; i<f.len; i++)printf(" %d", f.pa[i]);
        printf("\nplayer's %d turn\n", n+1);
            if(players[n]==0)mam(f);
            else if(players[n]==1)botE(f);
            else if(players[n]==2)botM(f);
            else if(players[n]==3)botD(f);
    }
    printf("player %d won", n+1);
}

void start(){//TO DO: add checking for user input
    int pn, i, size;
    printf("How many players are there? ");
    scanf("%d", &pn);
    int p[pn];//player data
    printf("0 - human, 1 - AI easy, 2 - AI normal, 3 - AI hard\n");
    for(i=0; i<pn; i++){
        printf("Who is player %d? ", i+1);
        scanf("%d", &p[i]);
    }
    printf("With how many heaps do you wish to play? ");
    printf("\n*:currently AI can play only with 2.\n");
    scanf("%d", &size);
    int stones[size];
    for(i=0; i<size; i++){
        printf("How many stones are in a heap #%d? ", i+1);
        scanf("%d", &stones[i]);
    }
    struct field f=buildf(size, stones);
    playNim(f, pn, &p[0]);
}

int main() {
    start();
}
