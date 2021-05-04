#include<stdio.h>
#include<windows.h>
#include "switch.h"
#include "packets.h"

void* sdnScheduler(void* vargp);
void pollSwitches();
int checkSwitchDataCount(cm_s db);
void processSketch(cm_s db);
void declareHashValues();
int minVal(cm_s* cmm, int x);

void* sdnScheduler(void* vargp)
{
    int i = 1;
    while (1) {
        epoch = i;
        printf("\nCONTROLLER : Calling function to poll Switches, epoch id: %d\n", i);
        resetFlag = TRUE;
        printf("\nCONTROLLER : Controller thread going to sleep!\n");
        Sleep(MAX_TIME);
        printf("\nCONTROLLER : Controller thread up from sleep\n");
        i++;
    }
}

void pollSwitches() {
    for (int i = 0; i < NUM_SWITCHES; i++) {
        poll[i] = 1;
        printf("\nCONTROLLER : pollSwitches() %d - %d\n", i, poll[i]);
    }
}

int checkSwitchDataCount(cm_s db) {
    for (int i = 0; i < NUM_SWITCHES; i++)
        if (con_data[i].switch_id == -1)
            return 0;
    
    return 1;
}

void processSketch(cm_s db) {
    printf("\nCONTROLLER : Inside Process sketch\n");
    
    int id = db.switch_id;
    con_data[id].id = db.id;
    con_data[id].switch_status = db.switch_status;
    for (int i = 0; i < MAX_PACKETS; i++)
        con_data[id].packTrack[i] = db.packTrack[i];
    for (int i = 0; i < S_D; i++)
        con_data[id].hash[i] = db.hash[i];

    con_data[id].count = malloc(S_D * sizeof(int*));
    for (int j = 0; j < S_D; j++)
        con_data[id].count[j] = malloc(S_W * sizeof(int));

    for (int i = 0; i < S_D; i++) {
        for (int j = 0; j < S_W; j++) {
            con_data[id].count[i][j] = db.count[i][j];
        }
    }
    
    printf("\nCONTROLLER : Process Sketch function, switch %d header received  \n", id);
    
    for (int i = 0; i < MAX_FLOW_ID; i++)
        sumPacket[i] += minVal(&con_data[id], i);
    
    con_data[id].switch_id = db.switch_id;
    if (checkSwitchDataCount(db))
    {
        for (int i = 0; i < MAX_FLOW_ID; i++)
            printf("\nCONTROLLER: Number of packets for Flow ID: %d, epoch ID: %d are %d\n", i, epoch, sumPacket[i]);

        printf("\nCONTROLLER : Calling reset function for d...");
        resetSwitchHeader(d);
        printf("\nCONTROLLER : Calling reset function for con_data...");
        resetConDataHeader(con_data);
    }
}

void declareHashValues()
{
    for (int i = 0; i < S_D; i++)
        a[i] = (rand() % (p - 1)) + 1;
    b = (rand() % (p - 1)) + 1;
}

//Estimate the value from sketch
int minVal(cm_s* cmm, int x)
{
    printf("\nCONTROLLER : Inside minVal()\n");
    int min = INT_MAX;
    for (int i = 0; i < S_D; i++) {
        cmm->hash[i] = ((a[i] * x + b) % p) % S_W;
    }
    for (int i = 0; i < S_D; i++)
    {
        if (cmm->count[i][cmm->hash[i]] < min) {
            min = cmm->count[i][cmm->hash[i]];
        }
    }
    printf("\nCONTROLLER : minVal completed... Sending min Value\n");
    return min;
}

int main()
{
    pthread_t switches[NUM_SWITCHES];
    pthread_t sdn_controller;
    pthread_t packet;
    p = 23;
    printf("Helloooooo \n");
    declareHashValues();
    sumPacket[0] = 0;
    sumPacket[1] = 0;
    for (int i = 0; i < NUM_SWITCHES; i++)
        con_data[i].switch_id = -1;
    //Creating Switches
    for (int i = 0; i < NUM_SWITCHES; i++)
    {
        printf("\nCreating switch %d\n", i);
        d[i].switch_id = i;
        d[i].switch_status = "ACTIVE";
        for (int j = 0; j < MAX_PACKETS; j++)
            d[i].packTrack[j] = -1;

        d[i].count = malloc(S_D * sizeof(int*));
        for (int j = 0; j < S_D; j++)
            d[i].count[j] = malloc(S_W * sizeof(int));
        
        for (int j = 0; j < S_D; j++)
            for (int k = 0; k < S_W; k++)
                d[i].count[j][k] = 0;
        
        pthread_create(&switches[i], NULL, switchScheduler, (void*)&d[i]);
    }
    
    pthread_create(&packet, NULL, packetScheduler, NULL);

    //Creating SDN Controller
    pthread_create(&sdn_controller, NULL, sdnScheduler, NULL);
    
    
    pthread_join(packet, NULL);
    pthread_join(switches[NUM_SWITCHES], NULL);
    pthread_join(sdn_controller, NULL);
    printf("\nCONTROLLER: Process Completed\n");

}
