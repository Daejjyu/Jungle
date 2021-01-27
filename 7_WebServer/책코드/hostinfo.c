#include "csapp.h"
int main(int argc, char **argv)
{
    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;
    if (argc != 2){
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }
    //1. 왜 0으로 초기화 해야 할까?! 
    //2. 왜 addrinfo의 사이즈만큼 초기화해야 할까? >> 구조체 사이즈만큼의 공간 모두 초기화
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET; //address family internet >> IPv4
    //SOCK_STREAM  스트림 , TCP 프롤토콜의 전송 방식
    //SOCK_DGRAM   데이터 그램, UDP 프로토콜의 전송 방식
    //SOCK_RAW     RAW 소켓, 가공하지 않은 소켓
    hints.ai_socktype = SOCK_STREAM; 
    //argv[1] : 우리가 입력한 url 혹은 ip_address
    //0 if okay nonzero error code on error >> rc는 에러 파악에 사용!
    if((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0){ // argv[1] - domain, NULL - 호스트 이름을 주소로 변환하고 싶지 않을 때는 host를 NULL로 설정
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }
    flags = NI_NUMERICHOST; //Display address string instead of domain name
    printf("## %d\n",flags);
    for (p = listp; p; p = p->ai_next){
        //printf("# %s\n",buf); // buf
        Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        //buf를 통해 nameinfo받아온다.
        printf("%s\n", buf); 
    }
    //clean up 
    //addrinfo구조체를 다 썻으면 free시켜주자!
    Freeaddrinfo(listp);
    exit(0);
}