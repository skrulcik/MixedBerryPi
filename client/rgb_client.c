/* rgb_client.c - Command line utility for MixedBerryPi RGB controller
 *
 * Scott Krulcik 12/26/15
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csapp.h"

#define RGB_PORT "8080"
#define RGB_HOST "newpi.local"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex color (no '#')>\n", argv[0]);
        return 1;
    }

    char c, *color, message[MAXBUF];
    int clientfd = Open_clientfd(RGB_HOST, RGB_PORT);
    rio_t rio;
    Rio_readinitb(&rio, clientfd);

    color = argv[1];
    sprintf(message, "POST / HTTP/1.1\r\n");
    strcat(message, "Host: newpi.local:8080\r\n");
    sprintf(message, "%sContent-Length: %lu\r\n", message, 9 + strlen(color));
    strcat(message, "\r\n");
    sprintf(message, "%scolor=%%23%s", message, color);
    
    printf("Request:\n%s\n\n\n", message);

    Rio_writen(clientfd, message, strlen(message));

    printf("Response:\n");
    while (Rio_readnb(&rio, &c, 1) == 1) 
        printf("%c", c);
    printf("\n\n");
    Close(clientfd);


    return 0;
}





