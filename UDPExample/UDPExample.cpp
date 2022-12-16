#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
using namespace std;

int cl_count = 0;
struct clients
{
    string order;
    sockaddr_in senderAddr;
};

clients client_order[30];


int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup provalena s mistake " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket provalena s mistake " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "svyaz provalena s mistake " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    int senderAddrSize = sizeof(client_order[cl_count].senderAddr);

    cout << "Polychenie dannix.." << endl;
    while (true)
    {
        int time_order = 0;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&client_order[cl_count].senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom provalena s mistake " << WSAGetLastError() << endl;
            return 4;
        }
        cl_count++;
        receiveBuf[bytesReceived] = '\0';
        cout << "Polychenie dannix " << client_order[cl_count - 1].senderAddr.sin_addr.s_host << endl;
        cout << "Dannie: " << receiveBuf << endl;
        string buf = receiveBuf;
        if (buf.find("burger") != -1)
        {
            time_order += 5000;
        }
        if (buf.find("sprite") != -1)
        {
            time_order += 1000;
        }
        if (buf.find("potato") != -1)
        {
            time_order += 3000;
        }
        const size_t sendBufSize = 1024;
        char sendBuf[sendBufSize] = "Podojdite ";
        char sendBuf_1[sendBufSize];
        _itoa(time_order / 1000, sendBuf_1, sendBufSize);
        strcat(sendBuf, sendBuf_1);
        strcat(sendBuf, " secundi\n");
        int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&client_order[cl_count - 1].senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "otpravka provalena s mistake " << WSAGetLastError() << endl;
            return 4;
        }
        _itoa(time_order, sendBuf_1, sendBufSize);
        sendto(udpSocket, sendBuf_1, strlen(sendBuf_1), 0, (SOCKADDR*)&client_order[cl_count - 1].senderAddr, senderAddrSize);
        Sleep(time_order);
        char sendBuf_2[sendBufSize] = "Zakaz gotov , Spacibo\n";
        sendto(udpSocket, sendBuf_2, strlen(sendBuf_2), 0, (SOCKADDR*)&client_order[cl_count - 1].senderAddr, senderAddrSize);
    }

    closesocket(udpSocket);
    WSACleanup();
}

