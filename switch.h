#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#define S_D 6
#define S_W 32
#define NUM_SWITCHES 5
#define MAX_PACKETS 20
#define MAX_COUNTER 7
#define MAX_FLOW_ID 3

int poll[NUM_SWITCHES];

typedef struct cms {
    int  switch_id;
    char* switch_status;
    int id;
    int hash[S_D];
    int** count;
    int packTrack[MAX_PACKETS];
}cm_s;

int p;
int a[S_D], b;
int epoch;
int sumPacket[MAX_FLOW_ID];



cm_s con_data[NUM_SWITCHES];
cm_s d[NUM_SWITCHES];
void pollSwitches();

void* switchScheduler(void* switcharg);
void printSwitchHeader(cm_s* cmsk);
void resetSwitchHeader(cm_s switchHeader[]);
void resetConDataHeader(cm_s switchHeader[]);
char* getSketchCountString(int** a);
char* getPackTrackString(int a[]);