int open_clientfd(char *hostname, char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;
    memset(&hints, 0, sizeof(struct addrinfo));
    //getaddrinfo에서 hints(어떻게 만들어야 하는지 정보를 얻어서)를 얻어서 주소 구조체를 만들고 이 시작점을 listp에 저장
    //ai: address info
    //SOCK_STREAM  스트림 , TCP 프롤토콜의 전송 방식
    //SOCK_DGRAM   데이터 그램, UDP 프로토콜의 전송 방식
    //SOCK_RAW     RAW 소켓, 가공하지 않은 소켓
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG; //AI_ADDRCONFIG?
    Getaddrinfo(hostname, port, &hints, &listp);//listp에 result 반환
    for(p = listp; p; p = p->ai_next){
        //create socket descriptor
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        //if socket created succesfully, connect to the server
        //903p 참고
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break;
        //failed to connect server
        Close(clientfd);
    }
    //listp가 가리키는 getaddressinfo struct를 free시켜주자. 
    //malloc했으면 free를 해야 메모리 누수를 막을 수 있는 것 처럼!
    Freeaddrinfo(listp);
    if (!p)
        return -1;
    else
        return clientfd;
}
/* $end open_clientfd */
/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_listenfd */
int open_listenfd(char *port) 
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;
    memset(&hints, 0, sizeof(struct addrinfo));
    //SOCK_STREAM  스트림 , TCP 프롤토콜의 전송 방식
    //SOCK_DGRAM   데이터 그램, UDP 프로토콜의 전송 방식
    //SOCK_RAW     RAW 소켓, 가공하지 않은 소켓
    hints.ai_socktype = SOCK_STREAM;
    //clientfd의 flag에 AI_PASSIVE가 더해진다.
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;
    //자신-local에서만 찾는다.
    Getaddrinfo(NULL, port, &hints, &listp);
    for(p = listp; p; p->ai_next){
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        //커널은 기존에 bind로 할당된 소켓자원을 프로세스가 재 사용할 수 있도록 허락한다.
        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
        //socket으로 만든 socket은 아직 주소가 정해져있지 않다.
        //socket에게 어디서 연락을 기다리는지 알아보자.
        //소켓에 들어올 수 있는 주소값을 소켓 식별자에 연결시켜준다. 900p
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        Close(listenfd);
    }
    //address info struct는 이제 필요가 없어졌다. free 시키자.
    Freeaddrinfo(listp); 
    if(!p)
        return -1;
    if(listen(listenfd, LISTENQ) < 0){
        Close(listenfd);
        return -1;
    }
    return listenfd;
}