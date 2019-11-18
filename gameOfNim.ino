/*
  gameOfNim
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define abs(x) ((x)>=0) ? (x):-(x)
const int in=12, confirm=13;//input ports
//int readinp(int pin){return analogRead(pin);}
//#define read readinp(in)
#define s(x) (String)(x)

// initialize the display
LiquidCrystal_I2C lcd(0x27,16,2); //adress, columns, rows

void print(String s, int row){
    for(int i=0; i<16; i++){lcd.setCursor(i, row-1); lcd.print(" ");}
    lcd.setCursor(0, row-1);
    lcd.print(s);
    if(s.length()>16){
        for(int i=0; i<s.length()-16; i++){delay(250); lcd.scrollDisplayLeft();}
        for(int i=0; i<s.length()-16; i++){/*delay(250);*/ lcd.scrollDisplayRight();}
    }
}

int readinp(){//Use "in" pin button to enter value and "confirm" pin btn to send it
    int out=0, t=0;
    while(digitalRead(confirm)==HIGH){
        if(digitalRead(in)==LOW && t==0){
            out++; t=1;
            for(int i=0; i<16; i++){lcd.setCursor(i, 1); lcd.print(" ");}
            lcd.setCursor(0, 1);
            lcd.print(out);
        }
        if(digitalRead(in)==HIGH)t=0;
    }
    return out;
}

//class Bot;
class Field{
    public:
        int len, *pa, nhlen, *nheaps;
        
        Field(int l, int *a){len=l; pa=a;}
        
        void mam(){//make a move
            int x, n;
            print("Enter heap num:", 1);
            n=readinp(); n--;
            print("Enter stone quantity:", 1);
            x=readinp(); 
            if(-1<n && n<len){
                if(0<x && x<=pa[n]){
                    pa[n]-=x;
                    tryemp(n);
                    print(s(n)+" "+s(x), 2);
                }
                else{
                    print("This heap has only "+s(pa[n])+" stones", 1);
                    mam();
                }
            }
            else{
                print("There are only "+s(len)+" heaps", 1);
                mam();
            }
        }
        
        void tryemp(int n){if(pa[n]==0){pa[n]=pa[0]; pa++; len--;}}
};

class Bot{
    public:
        Field *f;
        Bot(class Field *field){f=field;}
        void bm(int n, int x){//bot move
            print("Enter heap num:", 1);
            print(s(n), 2);
            print("Enter stone quantity:", 1);
            print(s(x), 2);
            f->pa[n]-=x; f->tryemp(n);
        }
        void botE(){//bot Easy
            randomSeed(micros());
            int n=random(f->len), x=random(f->pa[n])+1;
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
            randomSeed(micros());
            int n=random(2);
            if(n%2==0)botE();
            else botD();
        }
        void bmam(int n){//bot make a move
            if(n==1)botE();
            else if(n==2)botM();
            else if(n==3)botD();
        }
};

void playNim(class Field *f, int pn, int *players, class Bot *bot){
            int n=-1, i;
            while(f->len>0){
                n=(n+1)%pn;
                String s="Heaps:";
                for(i=0; i<(f->len); i++){
                    s+=" "+s(f->pa[i]);
                }
                print(s, 1);
                print("player's "+s(n+1)+" turn", 2);
                if(players[n]==0)f->mam();
                else bot->bmam(players[n]);
            }
            print("player "+s(n+1)+" won", 1);
            print("player "+s(n+1)+" won", 2);
}

void start(){
    int pn, size;
    print("How many players are there?", 1);
    pn=readinp(); pn=abs(pn);
    int p[pn];//player data
    for(int i=0; i<pn; i++){
        print("0-human, AI:1-easy, 2-normal, 3-hard", 1);
        print("Who is player "+s(i+1)+"?", 1);
        p[i]=readinp(); p[i]=abs(p[i])%4; print(s(p[i]), 2);
    }
    print("With how many heaps do you wish to play?", 1);
    print("AI hard&normal can play only with 2", 1);
    size=readinp(); size=abs(size); if(size==0)size++; print(s(size), 2);
    int stones[size];
    for(int i=0; i<size; i++){
        print("How many stones are in a heap #"+s(i+1)+"?", 1);
        stones[i]=readinp(); stones[i]=abs(stones[i]);
        if(stones[i]==0)stones[i]++;
        print(s(stones[i]), 2);
    }
    Field f(size, stones); Bot bot(&f);
    playNim(&f, pn, &p[0], &bot);
}


void setup() {
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);
    //int sensorValue = analogRead(A0);
    //pinMode(9, OUTPUT);
    pinMode(in, INPUT);
    pinMode(confirm, INPUT);
}


void loop() {
    start();
}
