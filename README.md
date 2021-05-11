## B+ Tree

- 처음 접하는 C 언어를 이용, B+ Tree를 [구현](https://www.notion.so/B-B-173ec475f24c41129f97da97a078a48d)
    - 메모리라는 제한된 공간을 효율적으로 사용하기 위해 디스크를 사용
    - 디스크 접근 시간을 줄이는 인덱싱과 트리 구조의 아름다움을 깨달음

## Malloc Lab

- C를 이용해 Malloc 기능 구현하며 동적 메모리 할당의 과정을 이해함
    - 전체 메모리 공간을 관리하는 Implicit List, 가용 가능한 공간만 관리하는 Explicit Free List, 필요 공간의 크기에 따라 다른 List에 배정하는 Segregated Free List를 구현
    - 상기의 방법들과 First-fit, Next-fit, Best-fit 방식을 혼용으로 하며 단편화를 해결
    - 컴퓨터에서 최적화란 장단점이 있는 방법론 중 가장 최적의 선택을 하는 것이라는 점을 깨달음

## Tiny Web

- 소켓을 이용하여 서버와 클라이언트 간의 통신을 직접 구현하여 봄
    - TCP / UDP 방식에 대한 이해
    - HTTP의 Get/Post method의 동작 방식에 대한 이해

## Pintos

- OS를 직접 구현해보는 과정으로 컴퓨터의 동작에 대해 깊게 이해
    - 선점 방식 스케줄러를 이용해 스레드 시스템을 [구현](https://www.notion.so/Pintos-1-THREADS-40183d38e31b476ea821a81f98e0d5c2)
    - 64bit OS의 syscall을 [구현](https://www.notion.so/Pintos-2-User-Programs-7a7e3736b143401b9ca75c79e8a9af63)
    - Hash를 이용한 가상 메모리 Page의 할당 및 교체를 [구현](https://www.notion.so/Pintos-3-VIRTUAL-MEMORY-8979853ccd3a491a83e24a62ac2f941f)
    - Fat 방식으로 Subdirectory와 파일의 Soft Links를 [구현](https://www.notion.so/Pintos-4-FILE-SYSTEM-3a7c3b79e5dc499bbc49813021fd2b88)
