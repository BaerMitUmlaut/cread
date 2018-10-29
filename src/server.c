#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "request.h"

int serverMode() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) return 1;
    const int y = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y));

    struct sockaddr_in client;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_family = AF_INET;
    client.sin_port = htons(16000);

    /* listen for incoming connections */
    if (bind(sock, (struct sockaddr *) &client, sizeof(client)) == -1)
        return 2;
    listen(sock, 1);

    socklen_t len = sizeof(client);
    while (1) {
        /* accept incoming connection */
        int clientSocket = accept(sock, (struct sockaddr *) &client, &len);
        char buf[1024];
        recv(clientSocket, &buf, sizeof(buf), 0);

        /* read request */
        struct request req = { .length = 0, .file = NULL };
        buftoreq(&buf, &req, sizeof(buf));
        char ip[16];
        inet_ntop(AF_INET, &client.sin_addr.s_addr, ip, 16);
        printf("%s requested %u bytes of %s\n", ip, req.length, req.file);

        /* read and send file */
        char filebuf[req.length];
        FILE *handle = fopen(req.file, "r");
        size_t readBytes = fread(filebuf, 1, req.length, handle);
        fclose(handle);
        send(clientSocket, &filebuf, readBytes, 0);

        close(clientSocket);
    }
    close(sock);

    return 0;
}