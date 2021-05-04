/*
#include <stdio.h>
#include <stdlib.h> 
#include <limits.h>
#include "switch.h"


typedef struct cms {
    int id;
    int hash[S_D];
    int** count;
}cm_s;



//update the value in sketch
void hashFunc(cm_s* cmm, int x)
{
    for (int i = 0; i < S_D; i++)
    {
        //printf("Inside for, iteration: %d", i);
        cmm->hash[i] = ((a[i] * x + b) % p) % S_W;
        //printf("\n cmm->hash: %d \n", cmm->hash[i]);
        cmm->count[i][cmm->hash[i]]++;
        //printf("Done! \n");
        //for(int i=0;i<S_D;i++)
          //  count[i][hash[i]]++;
    }
    //for(int i=0;i<S_D;i++)

}


//Estimate the value from sketch
int minVal(cm_s* cmm, int x)
{
    int min = INT_MAX;
    for (int i = 0; i < S_D; i++)
        cmm->hash[i] = ((a[i] * x + b) % p) % S_W;
    for (int i = 0; i < S_D; i++)
    {
        if (cmm->count[i][cmm->hash[i]] < min)
            min = cmm->count[i][cmm->hash[i]];
    }
    return min;
}


int main()
{

    //printf("Init \n");
    cm_s* cmSk = malloc(sizeof(cm_s));;
    //printf("After malloc \n");
    for (int i = 0; i < S_D; i++)
        a[i] = (rand() % (p - 1)) + 1;
    b = (rand() % (p - 1)) + 1;

    cmSk->count = malloc(S_D * sizeof(int*));
    for (int i = 0; i < S_D; i++)
        cmSk->count[i] = malloc(S_W * sizeof(int));

    for (int i = 0; i < S_D; i++)
        for (int j = 0; j < S_W; j++)
            cmSk->count[i][j] = 0;


    int arr[10];
    for (int i = 0; i < 10; i++)
        arr[i] = (rand() % 6) + 1;
    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        // printf("Calling for %d time \n", i);
         //printf("Value of array value: %d \n", arr[i]);
        hashFunc(cmSk, arr[i]);

    }
    for (int i = 0; i < S_D; i++)
    {
        for (int j = 0; j < S_W; j++)
        {
            printf("%d ", cmSk->count[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    int f = minVal(cmSk, arr[9]);
    printf("%d", f);
    return 0;
}
*/
