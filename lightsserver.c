/** lightsserver.c
 * Scott Krulcik
 * 12/21/15
 *
 * Server program meant to run on Raspberry Pi to control RGB LED light strips
 *      as part of MixedBerryPi project.
 */
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "csapp.h"
#include "lightsserver.h"
#include "rgbstrip.h"

#define LISTEN_PORT "8080"
#define RESP_OK "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\n<html><head><title>Color Set</title></head><body><h1>Success!!</h1></body></html>"
#define RESP_ERR "HTTP/1.1 400 Bad Request\r\nContent-Type:text/html\r\n\r\n<html><head><title>Color Not Set</title></head><body><h1>Failure -_-</h1></body></html>"

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


/* get_properties - reads request from `rp` and fills `buf` with the POST
 *     properties. Returns size of properties string if successful, 0 on fail.
 */
int get_properties(rio_t *rp, char *buf) {
    char c, capbuf[MAXLINE];
    int i, content_length = 0;
    do {
        Rio_readlineb(rp, buf, MAXLINE);
        for (i=0; i < strlen(buf); i++)
            capbuf[i] = toupper(buf[i]);
        if (strstr(capbuf, "CONTENT-LENGTH:") != NULL) {
            sscanf(buf, "%*s%d", &content_length);
        }
    } while (strcmp(buf, "\r\n"));
    
    buf[0] = '\0';
    i = 0;
    while (i < content_length && Rio_readnb(rp, &c, 1) == 1) {
        strncat(buf, &c, 1);
        i++;
    }
    assert(strlen(buf) == content_length); // Change to IF to handle errors
    return content_length;
}
    

void handle_client(int connfd) {
    char buf[MAXLINE], *response;
    int color_data = 0;
    rio_t rio;
    Rio_readinitb(&rio, connfd);

    if (get_properties(&rio, buf)) {
        if (sscanf(buf, "color=%%23%x", &color_data)) {
            printf("Color Request: %x\n", color_data);
            color c;
            color_setint(&c, (color_data<<8) | 0xFF);
            rgb_set(rs, &c);
            response = RESP_OK;
        } else {
            response = RESP_ERR;
        }
    } else {
        response = RESP_ERR;
    }
    
    printf(response);

    Rio_writen(connfd, response, strlen(response)); 
}

void close_all() {
    should_receive = 0;
    // Close out threads!!
}

