#include<stdio.h>
#include "switch.h"
#include<windows.h>
#include "packets.h"

void* switchScheduler(void* switcharg)
{
    cm_s* switch_data = switcharg;

    printf("\nSWITCH %d: Inside switch controller \n", switch_data->switch_id);
    
    while (1)
    {
        if (poll[switch_data->switch_id])
        {
            poll[switch_data->switch_id] = 0;
            printf("\nSWITCH %d : Calling processSketch() \n", switch_data->switch_id);
            processSketch(*switch_data);
            printf("\nSWITCH %d : Process sketch call ended \n", switch_data->switch_id);
        }
    }
}

void printSwitchHeader(cm_s *cmsk) {
    char* count = getSketchCountString(cmsk->count);
    char* packTrack = getPackTrackString(cmsk->packTrack);
    printf("\nSWITCH %d: Printing Switch header - ID: %d Packets: %s Sketch Counters: \n%s",cmsk->id, cmsk->switch_id, packTrack, count); 
    printf("\n **new line** \n");
}

void resetSwitchHeader(cm_s switchHeader[]) {
    
    for (int i = 0; i < NUM_SWITCHES; i++)
    {
        printf("\nSWITCH %d: Reseting switch header...\n", switchHeader[i].switch_id);
        switchHeader[i].switch_status = NULL;
        for (int j = 0; j < MAX_PACKETS; j++)
            switchHeader[i].packTrack[j] = -1;

        for (int j = 0; j < S_D; j++)
            switchHeader[i].hash[j] = 0;

        for (int j = 0; j < S_D; j++)
            for (int k = 0; k < S_W; k++)
                switchHeader[i].count[j][k] = 0;
    
        printf("\nSWITCH %d: Successfully reset switch header!\n", switchHeader[i].switch_id);
    }
}

void resetConDataHeader(cm_s switchHeader[]) {

    for (int i = 0; i < NUM_SWITCHES; i++)
    {
        printf("\nSWITCH %d: Reseting switch header...\n", switchHeader[i].switch_id);
        switchHeader[i].switch_id = -1;
        switchHeader[i].switch_status = NULL;
        switchHeader[i].id = -1;
        for (int j = 0; j < MAX_PACKETS; j++)
            switchHeader[i].packTrack[j] = -1;

        for (int j = 0; j < S_D; j++)
            switchHeader[i].hash[j] = 0;

        for (int j = 0; j < S_D; j++)
            for (int k = 0; k < S_W; k++)
                switchHeader[i].count[j][k] = 0;

        for (int i = 0; i < MAX_FLOW_ID; i++)
            sumPacket[i] = 0;
        
        printf("\nSWITCH %d: Successfully reset switch header!\n", switchHeader[i].switch_id);
    }
}

char* getSketchCountString(int** a)
{
    char* s;
    s = malloc((500) * sizeof(char*));

    int n = 0;

    for (int i = 0; i < S_D; i++) {
        for (int j = 0; j < S_W; j++) {
            n += sprintf_s(&s[n], sizeof(s), "%d", a[i][j]);
            n += sprintf_s(&s[n], sizeof(s), "%c", ' ');
        }
        n += sprintf_s(&s[n], sizeof(s), "%c", '\n');
    }

    return(s);
}

char* getPackTrackString(int a[])
{
    char* s;
    s = malloc((50) * sizeof(char*));

    int n = 0;
    int i = 0;
    while (i < MAX_PACKETS && a[i] != -1){
        n += sprintf_s(&s[n], sizeof(s), "%d", a[i]);
        n += sprintf_s(&s[n], sizeof(s), "%c", ' ');
        i++;
    }

    return(s);
}
