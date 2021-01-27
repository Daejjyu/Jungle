#include "csapp.h"
int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio; //rio_t -> 865p
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];
    printf("open_clientfd...\n");
    clientfd = Open_clientfd(host, port);
    printf("Connected!\n");
    //The rio readinitb function is called once per open descriptor. It associates the descriptor fd with a
    //read buffer of type rio t at address rp.
    //rio_readinitb함수는 한 개의 빈 버퍼를 설정하고, 이 버퍼와 한 개의 오픈한 파일 식별자를 연결한다.
    //865p~
    Rio_readinitb(&rio, clientfd);
    // sprintf(buf, "GET /godzilla.gif HTTP/1.1\nConnection: keep-alive\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36\nAccept: image/avif,image/webp,image/apng,image/*,*/*;q=0.8\nReferer: http://3.35.208.161:5000/\nAccept-Encoding: gzip, deflate\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n");
    // stdin 으로 입력을 받아서 buf에 저장
    while (Fgets(buf, MAXLINE, stdin) != '\n')
    {
        Rio_writen(clientfd, buf, strlen(buf));
        //printf("buf: %s\n",buf);
        // Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    Rio_readlineb(&rio, buf, MAXLINE);
    printf("buf: %s\n", buf);

    Close(clientfd);
    exit(0);
}