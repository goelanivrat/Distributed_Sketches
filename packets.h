#pragma once
#include "switch.h"
#define MAX_TIME 50000
#define MIN_TIME 0

typedef struct packets {
	int pId;
	int route[NUM_SWITCHES];
	int header[S_D];
	int epochId;
}pkt;

BOOL resetFlag;
void generatePackets();
void packetScheduler();
void swap(int* a, int* b);
void getRoute(int arr[], int n);
void transmitPacket(pkt* pack1);
void printPacketHeader(pkt* pack);
void resetPacketHeader(pkt* packHeader[]);
void hashFunc(cm_s* cmm, int x, pkt* pack);
int checkOverflow(cm_s* sketch, int x, int y);
void setHeader(pkt* packet);
char* getPacketRouteString(int a[]);