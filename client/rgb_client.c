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
        fprintf(stderr, "Usage: %s <hex color>\n", argv[0]);
        return 1;
    }

    char message[50];
    int clientfd = Open_clientfd(RGB_HOST, RGB_PORT);
    int color = (int)strtol(argv[1], NULL, 0);
    sprintf(message, "SET %08X\n", color);

    Rio_writen(clientfd, message, strlen(message));

    Close(clientfd);


    return 0;
}





