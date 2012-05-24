#include <stdio.h>
#define SERVER_STATUS "common/serverStatus.dat" // file where the status of the server is stored

FILE *f;

struct serverStatus{
    int isRunning;
    char signal;
};// this structure stores the running status and used to communicate signals of the server

void setRunning(int state){//this updates the status of the server to specify its running or not
    struct serverStatus status;
    f=fopen(SERVER_STATUS,"w+");
    fread(&status,sizeof(struct serverStatus),1,f);
    status.isRunning=state;
    fwrite(&status,sizeof(struct serverStatus),1,f);
    fclose(f);
}

int isRunning(){// this returns whether the server is currently running or not
    struct serverStatus status;
    f=fopen(SERVER_STATUS,"r");
    fread(&status,sizeof(struct serverStatus),1,f);
    fclose(f);
    return status.isRunning;
}

void setSignal(char sig){//this sets a signal on the server that will used for client-server communication
    struct serverStatus status;
    f=fopen(SERVER_STATUS,"w+");
    fread(&status,sizeof(struct serverStatus),1,f);
    status.signal=sig;
    fwrite(&status,sizeof(struct serverStatus),1,f);
    fclose(f);
}

char getSignal(char sig){//this gets the current signal that has been currently set on the server
    struct serverStatus status;
    f=fopen(SERVER_STATUS,"r");
    fread(&status,sizeof(struct serverStatus),1,f);
    fclose(f);
    return status.signal;
}
