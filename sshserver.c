// C server code
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Driver code
int main()
{
        int udpSocket, nBytes;
        char buffer[3000], xor[3000];
        char const key[1024] = "HIDDENKEY";
        struct sockaddr_in serverAddr, clientAddr;
        struct sockaddr_storage serverStorage;
        socklen_t addr_size, client_addr_size;
        int i;

        udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5005);
        serverAddr.sin_addr.s_addr = inet_addr("192.168.56.103");
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

        bind(udpSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        addr_size = sizeof serverStorage;
        puts("[+]Listening... ");
        int count = 0;

        while (1) {
                nBytes = recvfrom(udpSocket, buffer, 3000, 0, (struct sockaddr*)&serverStorage,&addr_size);
                printf("Message no : %d\n", ++count);

                for (i = 0; i < nBytes - 1; i++) {
                        if (buffer[i] != '\n')
                                xor[i] = (char)(buffer[i] ^ key[i]);
                        else
                                xor[i] = buffer[i];
                }

                printf("Encrypted message stored in file : \n");

                // printing some of the character to have a feel of encryption
                for (i = 0; i < 32; ++i)
                        printf("%02X ", xor[i]);
                printf("\n");

                FILE* fp;
                fp = fopen("temp.txt", "w+");

                for (i = 0; i < nBytes - 1; i++) {
                        if (xor[i] != '\n')
                                fprintf(fp, "%X", xor[i]);
                        else
                                fprintf(fp, "%c", xor[i]);
                }

                fclose(fp);
                sendto(udpSocket, xor, nBytes, 0, (struct sockaddr*)&serverStorage,addr_size);
        }

        return 0;
}