#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "switch.h"
#include "packets.h"


void packetScheduler() {
	while (1) 
	{
		if (resetFlag)
		{
			printf("PACKET: calling generatePackets(). resetFlag: %d\n", resetFlag);
			resetFlag = FALSE;
			generatePackets();
			printf("PACKET: generatePackets() call done!\n");
		}
	}
}

void generatePackets()
{
	int startTime = 0, endTime = MAX_TIME;
	int time1=0;
	int ct = 0;
	printf("\nPACKET:Inside generate packets\n");
	/*
	while (startTime < endTime) {
		//printf("New start time: %d \n", startTime);
		srand(time(0));
		time1 = (rand() % (endTime - startTime + 1));
		while (time1 == 0)
			time1 = (rand() % (endTime - startTime + 1));		
		ct++;
		//printf("PACKET: Packet Number: %d Time1: %d \n", ct, time1);
		pkt pack = { -1,{-1,-1,-1,-1,-1},{0,0,0,0,0,0} };
		
		pack.pId = ct;
		int route1[NUM_SWITCHES];
		for (int i = 0; i < NUM_SWITCHES; i++)
			route1[i] = i;
		getRoute(route1, NUM_SWITCHES);
		for (int i = 0; i < NUM_SWITCHES; i++)
			pack.route[i] = route1[i];
		printf("\nPACKET: Packet %d generated at time %d. Sending to switch %d.. \n",pack.pId, time1, pack.route[0]);
		//create
		printPacketHeader(&pack);
		transmitPacket(&pack);
		printf("\nPACKET: Packet thread going to sleep!\n");
		Sleep(time1);
		printf("\nPACKET: Packet thread up from sleep\n");
		//printf("Sleep done...\n");
		startTime += time1;
		
	}
	*/
	for (int i = 0; i < MAX_PACKETS; i++)
	{
		ct++;
		pkt pack = { -1,{-1,-1,-1,-1,-1},{0,0,0,0,0,0} };
		pack.pId = ct;
		int route1[NUM_SWITCHES];
		for (int i = 0; i < NUM_SWITCHES; i++)
			route1[i] = i;
		getRoute(route1, NUM_SWITCHES);
		for (int i = 0; i < NUM_SWITCHES; i++)
			pack.route[i] = route1[i];
		printf("\nPACKET: Packet %d generated at time %d. Sending to switch %d.. \n", pack.pId, time1, pack.route[0]);
		printPacketHeader(&pack);
		transmitPacket(&pack);
	}
	printf("\nPACKET: No. of packets: %d \n", ct);
	pollSwitches();
	return 0;
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void getRoute(int arr[], int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		swap(&arr[i], &arr[j]);
	}
}

void transmitPacket(pkt *pack1) {
	for (int i = 0; i < NUM_SWITCHES; i++)
	{
		int switchid = pack1->route[i];
		printf("\nPACKET: i = %d:: Packet %d received at switch %d. Updating switch sketch...\n", i, pack1->pId, switchid);
		if (i == 0)
		{
			pack1->epochId = epoch;
			setHeader(pack1);
		}
		
		d[switchid].switch_id = switchid;
		int j = 0;
		while (j < MAX_PACKETS && d[switchid].packTrack[j] != -1)
			j++;
		if(j<MAX_PACKETS)
			d[switchid].packTrack[j] = pack1->pId;

		int x = (pack1->route[NUM_SWITCHES - 1]) % MAX_FLOW_ID;
		d[switchid].id = x;
		hashFunc(&d[switchid], d[switchid].id, pack1);

		printf("\nPACKET: i = %d:: Updated sketch of switch %d for packet %d...\n", i, switchid, pack1->pId);
		printf("\nPACKET: i = %d:: Printing switch header for packet %d...\n", i, pack1->pId);
		printSwitchHeader(&d[switchid]);
	}
}

void printPacketHeader(pkt* pack) {
	char* route = getPacketRouteString(pack->route);
	printf("\nPACKET: Inside printPacketHeader(): Packet PID : %d Route: %s", pack->pId, route);
	printf("\n**new line**\n");
}

void resetPacketHeader(pkt* packHeader[]) {
	printf("\nPACKET: Reseting packet header...\n");
	for (int i = 0; i < MAX_PACKETS; i++)
	{
		packHeader[i]->pId = -1;
		for (int j = 0; j < NUM_SWITCHES; j++)
			packHeader[i]->route[j] = -1;
	}
	printf("\nPACKET: Successfully reset packet header!\n");
}

char* getPacketRouteString(int a[])
{
	char* s;
	s = malloc((50) * sizeof(char*));

	int n = 0;
	int i = 0;
	while (i < NUM_SWITCHES) {
		n += sprintf_s(&s[n], sizeof(s), "%d", a[i]);
		n += sprintf_s(&s[n], sizeof(s), "%c", ' ');
		i++;
	}

	return(s);
}

//update the value in sketch
void hashFunc(cm_s* cmm, int x, pkt* pack)
{
	printf("\nPACKET: Hash Function called\n");
	for (int i = 0; i < S_D; i++)
	{
		if (pack->header[i] == 1) 
		{
			cmm->hash[i] = ((a[i] * x + b) % p) % S_W;
			printf("\nPACKET: checking for counter overflow\n");
			if (checkOverflow(cmm, i, cmm->hash[i]))
			{

				printf("\nPACKET: Overflow checked... Incrementing the count in the sketch\n");
				cmm->count[i][cmm->hash[i]]++;
				printf("\nPACKET: Setting the header for packet %d header %d to 0\n", pack->pId, i);
				pack->header[i] = 0;
			}
			else
				printf("\nPACKET: Counters Overflowing... Sending to the next switch...\n");
		}
	}
}

int checkOverflow(cm_s *sketch, int x, int y)
{
	if (sketch->count[x][y]+1 < MAX_COUNTER)
		return 1;
	return 0;
}

void setHeader(pkt *packet)
{
	printf("\nPACKET: Initializing all values of packet %d header to 1\n", packet->pId);
	for (int i = 0; i < S_D; i++)
		packet->header[i] = 1;
}