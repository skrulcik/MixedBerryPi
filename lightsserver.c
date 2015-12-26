/** lightsserver.c
 * Scott Krulcik
 * 12/21/15
 *
 * Server program meant to run on Raspberry Pi to control RGB LED light strips
 *      as part of MixedBerryPi project.
 */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "csapp.h"
#include "lightsserver.h"
#include "rgbstrip.h"

#define LISTEN_PORT "8080"
#define SETCMD 'S'
#define GETCMD 'G'

/* Private Functions */
void *setup_client_thread(void *);
void handle_client(int connfd);
void close_all();

static int should_receive = 1;
static rgbstrip *rs;
int main(int argc, char *argv[]) {
    int listenfd, *connfd_ptr;
    char client_host[MAXLINE], client_port[MAXLINE];
    pthread_t tid;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    printf("Lights server starting on port %s....\n", LISTEN_PORT);

    rs = (rgbstrip *)malloc(sizeof(rs));
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    c.a = 0;
    rgb_set(rs, &c);

    listenfd = Open_listenfd(LISTEN_PORT);
    while (should_receive) {
        connfd_ptr = (int *)Malloc(sizeof(int));
        *connfd_ptr = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_host, MAXLINE,
                client_port, MAXLINE, 0);
        Pthread_create(&tid, NULL, setup_client_thread, connfd_ptr);
    }

    printf("Lights server terminating...\n");
    return 0;
}

void *setup_client_thread(void *connfd_ptr) {
    int connfd = *(int *)connfd_ptr;
    Pthread_detach(pthread_self());
    Free(connfd_ptr);
    handle_client(connfd);
    return NULL;
}

void handle_client(int connfd) {
    size_t n;
    char buf[MAXLINE], response[MAXLINE];
    int color_data = 0;
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    memset(response, 0, MAXLINE);

    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        switch (buf[0]) {
            case SETCMD:
                if (sscanf(buf, "%*s %x", &color_data) == 1) {
                    color c;
                    color_setint(&c, color_data);
                    rgb_set(rs, &c);
                } else {
                    char *err_msg = "Failed to set. Original Message:";
                    sprintf(response, "%s %s\n", err_msg, buf);
                }
                break;
            case GETCMD:
                // Get
                sprintf(response, "R:%d G:%d B:%d\n", 234, 12, 86);
                break;
            default:
                sprintf(response, "%s\n", "e Command misunderstood");
                break;
        }
        if (strlen(response) != 0) {
            printf("Sending response: %s\n", response);
            Rio_writen(connfd, response, strlen(response));
        }
        response[0] = '\0';
    }
}

void close_all() {
    should_receive = 0;
    // Close out threads!!
}
