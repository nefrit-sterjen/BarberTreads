#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>

#define _WIN32_WINNT 0x0A00
#include <windows.h>

int clientCounter = 0;
int clientsInDay = 0;
BOOL barberStatus = FALSE;
short timeNextClient, timeCutHair;
HANDLE *clients, *localClients;
HANDLE cMutex;
short chairs = 3;



DWORD WINAPI Client(void* param)
{
    if(clientCounter == 0){
    printf("\n������ ����� %d ������. �� ������ �������, �� ���� � �������", clientsInDay + 1);
    }
    else{
        printf("\n������ ����� %d ������. ������ ����� � �� ������� �� ��������� ����", clientsInDay + 1);
    }
    clientsInDay++;
    clientCounter++;
    if(!barberStatus)
    {
        printf("\n������ ����� �������");
        barberStatus = TRUE;
    }

    WaitForSingleObject(cMutex, INFINITE);
    printf("\n������ �������� ������ %d �������", clientsInDay);
    timeCutHair = 1 + (rand()%(15 - 1 + 1));

    sleep(timeCutHair);

    printf("\n������ ���������");
    HANDLE client = clients[clientCounter];
    clients[clientCounter] = NULL;

    localClients = (HANDLE*)malloc(sizeof(HANDLE) *chairs);

    for(int i = 0; i < chairs; i++)
    {
        localClients[i] = NULL;
    }

    for(int i = 0; i < chairs; i++)
    {
        if(clients[i] != NULL)
        {
            localClients[i] = clients[i];
        }
    }

    for(int i = 0; i < chairs; i++)
    {
        clients[i] = localClients[i];
    }

    clientCounter--;

    if(clientCounter == 0)
    {
        printf("\n�������� ������ ���. ������ ������� �����");
        barberStatus = FALSE;
    }
    else
    {
        printf("\n������ �������� ������ ���������� �������");
    }
    ReleaseMutex(cMutex);

    //��������������� �������

    CloseHandle(client);


    return 0;

}

int main()
{
    setlocale(LC_ALL, "Rus");
    cMutex = CreateMutex(NULL, FALSE, NULL);
    while(TRUE)
    {
        srand(time(NULL));
        clients = (HANDLE*)malloc(sizeof(HANDLE) * chairs);
        timeNextClient = 2 + (rand()%(10 - 2 + 1));

        sleep(timeNextClient);
        if(clientCounter < chairs)
        {
            clients[clientCounter] = CreateThread(NULL, 0, Client, 0, CREATE_SUSPENDED, NULL);
            ResumeThread(clients[clientCounter]);
        }
        else{
            printf("\n������ ��������, ����� ������� � ������...", clientsInDay);
        }

    }
    return 0;
}
