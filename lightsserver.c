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

#define RESP_NC 3
#define RESP_NOTFOUND 2
#define RESP_OK 1 
#define RESP_ERR 0 

#define RPIN 23
#define GPIN 24
#define BPIN 25

/* Private Functions */
void *setup_client_thread(void *);
void handle_client(int connfd);
int get_properties(rio_t *rp, char *buf);
void format_response(char *response_buf, int buf_size, char *headmsg, char *bodymsg, int status);
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

    
    rgb_setup();
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    c.a = 0;
    rs = rgb_init(&c, RPIN, GPIN, BPIN);

    listenfd = Open_listenfd(LISTEN_PORT);
    while (should_receive) {
        connfd_ptr = (int *)Malloc(sizeof(int));
        *connfd_ptr = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_host, MAXLINE,
                client_port, MAXLINE, 0);
        Pthread_create(&tid, NULL, setup_client_thread, connfd_ptr);
    }

    rgb_free(rs);
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
    char buf[MAXLINE], response[MAXBUF];
    rio_t rio;
    Rio_readinitb(&rio, connfd);

    Rio_readlineb(&rio, buf, MAXLINE);
    if (strstr(buf, "POST") == buf) {
#if DEBUG
        printf("=======================================\n");
#endif
        int color_data = 0;
        if (get_properties(&rio, buf)) {
            if (sscanf(buf, "color=%%23%x", &color_data)) {
                printf("Color Request: %x\n", color_data);
                color c;
                color_setint(&c, (color_data<<8) | 0xFF);
                rgb_set(rs, &c);
                format_response(response, MAXBUF, NULL, NULL, RESP_NC);
            } else {
                format_response(response, MAXBUF, NULL, "Error parsing \"color\" property.", RESP_ERR);
            }
        } else {
            format_response(response, MAXBUF, NULL, "Error retrieving properties from request.", RESP_ERR);
        }
#if DEBUG
        printf(response);
        printf("=======================================\n\n\n");
#endif
    } else {
        format_response(response, MAXBUF, NULL, NULL, RESP_NOTFOUND);
    }
    Rio_writen(connfd, response, strlen(response)); 
    Close(connfd);
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

void format_response(char *response_buf, int buf_size, char *headmsg, char *bodymsg, int status) {
    char body[MAXBUF]; body[0] = '\0';
    
    switch (status) {
    case RESP_OK:
        sprintf(response_buf, "HTTP/1.1 200 OK\r\n");
        break;
    case RESP_ERR:
        sprintf(response_buf, "HTTP/1.1 400 Client Error\r\n");
        break;
    case RESP_NC:
        sprintf(response_buf, "HTTP/1.1 204 No Content\r\n\r\n");
        break;
    case RESP_NOTFOUND:
        sprintf(response_buf, "HTTP/1.1 404 Not Found\r\n\r\n");
        break;
    default:
        return;
    }
         
    if (status != RESP_NC) { 
        strcat(response_buf, "Content-type: text/html\r\n");
        strcat(response_buf, "Content-length: ");
        
        sprintf(body, "<!DOCTYPE html><html><head><title>lightsserver Response</title>");
        if (headmsg != NULL)
            strcat(body, headmsg);
        strcat(body, "</head><body>");
        if (bodymsg != NULL)
            strcat(body, bodymsg);
        strcat(body, "</body></html>\n");
        int bodylen = strlen(body);
        if (bodylen > 0 && strlen(response_buf) + 10 + bodylen < buf_size) {
            sprintf(response_buf, "%s%d\r\n\r\n", response_buf, bodylen);
            strcat(response_buf, body);
        } else {
            strcat(response_buf, "0\r\n\r\n");
        }
    }
}

void close_all() {
    should_receive = 0;
    // Close out threads!!
}

