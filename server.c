#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_STOCKS 100

struct stock{
    char name[51];
    int baseVal;
    int currVal;
    int incr;
};

struct stock stocks[MAX_STOCKS];

extern void setRunning(int);
extern void setSignal(char);
extern int isRunning();
extern char getSignal();
void loop();
void readStocks();
void willIncr();
void writeDefault();
void processStocks();
void updateRecords();
void printStocks();
void commit();

int turn=0;

int main(){
    setRunning(1);
    system("rm common/records/*");
    printf("Stocks server started successfuly!!\n");
    loop();
    printf("Stocks server exiting!\n");
    return 0;
}

void loop(){
    while(1){
        if(getSignal()=='w')
            printf("Waiting for another process to finish access!\n");
        else if(getSignal()=='q')
            break;
        else{
            turn++;
            readStocks();
            willIncr();
            processStocks();
            printStocks();
            updateRecords();
            commit();
            printf("Stock Values updated!!\n");
        }
        sleep(3);
    }
}

void printStocks(){
	int i;
	for(i=0;i<MAX_STOCKS;i++){
		if(strcmp(stocks[i].name,"")==0)
			break;
		else
			printf("%s\t%d\n",stocks[i].name,stocks[i].currVal);
	}
}

void readStocks(){
    FILE *f=fopen("common/stocks.dat","r+");
    fread(stocks,sizeof(struct stock),MAX_STOCKS,f);
    int i;
    /*for(i=0; i<MAX_STOCKS; i++)
        stocks[i].currVal=stocks[i].baseVal;*/
    fclose(f);
}

void writeDefault(){
    strcpy(stocks[0].name,"ONGC");
    stocks[0].baseVal=2568;
    stocks[0].currVal=4789;
    stocks[0].incr=1;
    strcpy(stocks[1].name,"ICICI");
    stocks[1].baseVal=656;
    stocks[1].currVal=667;
    stocks[1].incr=1;
    strcpy(stocks[2].name,"SBI");
    stocks[2].baseVal=2579;
    stocks[2].currVal=2345;
    stocks[2].incr=0;
    FILE *f=fopen("common/stocks.dat","w+");
    fwrite(stocks,sizeof(struct stock),MAX_STOCKS,f);
    fclose(f);
}

void willIncr(){
    int i;
    for(i=0; i<MAX_STOCKS; i++){
        int n=stocks[i].currVal%75;
        if(n>(rand()%75))
            stocks[i].incr=0;
        else
            stocks[i].incr=1;
    }
}

void processStocks(){
    int i;
    for(i=0; i<MAX_STOCKS; i++){
        float f=(rand()%5)/100.0;
        if(stocks[i].incr)
            stocks[i].currVal*=(1+f);
        else
            stocks[i].currVal*=(1-f);
    }
}

void commit(){
    FILE *f;
    f=fopen("common/stocks.dat","w+");
    int i;
    for(i=0; i<MAX_STOCKS; i++)
        stocks[i].baseVal=stocks[i].currVal;
    fwrite(stocks,sizeof(struct stock),MAX_STOCKS,f);
    fclose(f);
}

void updateRecords(){
    int i;
    for(i=0; i<MAX_STOCKS; i++){
        if(strcmp(stocks[i].name,"")!=0){
            char path[76]="common/records/";
            strcat(path,stocks[i].name);
            strcat(path,".txt");
            FILE *f=fopen(path,"a");
            fprintf(f,"%d\t%d\n",turn,stocks[i].currVal);
            fclose(f);
        }
    }
}
