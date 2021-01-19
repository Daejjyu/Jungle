#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "mm.h"
#include "memlib.h"
team_t team = {
    /* Team name */
    "team2",
    /* First member's full name */
    "Jobim",
    /* First member's email address */
    "daejjyu@gmail.com",
    /* Second member's full name (leave blank if none) */
    "Tonky",
    /* Second member's email address (leave blank if none) */
    "Smith"
};
#define WSIZE 4
#define DSIZE 8                                           //8byte 더 받음 2word 8byte
#define CHUNKSIZE (1<<12) //2의 12승

#define MAX(x,y) ((x)>(y)? (x) : (y))
#define PACK(size, alloc) ((size)| (alloc))  //size와alloc을 합친다(size자체가 8단위라서 000이라 상관없음)

#define GET(p)            (*(unsigned int *)(p))           //p를 받는다
#define PUT(p, val)       (*(unsigned int *)(p) = (val))   //value값을 넣는다

#define GET_SIZE(p)       (GET(p) & ~0x7) // 0x 는 16진수를 뜻함. 00000007이니깐 반대면 FFFFFFF8이 된다.
#define GET_ALLOC(p)      (GET(p) & 0x1)  //0001

#define HDRP(bp)          ((char* )(bp) - WSIZE)
#define PREVP(bp)          ((char* )(bp))
#define NEXTP(bp)          ((char* )(bp) + + WSIZE)
#define FTRP(bp)          ((char* )(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp)     ((char*)(bp) + GET_SIZE(((char*)(bp)-WSIZE)))
#define PREV_BLKP(bp)     ((char*)(bp) - GET_SIZE(((char*)(bp)-DSIZE)))\
#define NEXT(bp)          ((char*)(bp) + WSIZE)
#define PREV(bp)          ((char*)(bp))

static char *free_listp;

extern int mm_init(void);
extern void *mm_malloc(size_t size);
extern void mm_free(void *ptr;

static void* coalesce(void *bp);
static void* extend_heap(size_t words);

static void *find_fit(size_t asize);
static void place(void* bp, size_t asize);
void* mm_realloc(void* bp, size_t size);

void ReConnect(void * bp, int next){    
    void * n_prev;
    void * n_next;

    if(next){
        n_prev = PREV(NEXT_BLKP(bp));
        n_next = NEXT(NEXT_BLKP(bp));
    }else{
        n_prev = PREV(bp);
        n_next = NEXT(bp);
    }

    if(n_next!=NULL){
        PREV(n_next) = n_prev;                
        NEXT(n_prev) = n_next;
    }else{
        NEXT(n_prev)=NULL;
    }
}

void RootConnect(void * bp){
    // root next not null
    void *last = NEXT(free_listp);
    if(last!=NULL){
        PREV(last) = bp;
        NEXT(bp)=last;
    }
    // bp and root, no more connect
    PREV(bp)=free_listp;
    NEXT(bp)=NULL;
    NEXT(free_listp)=bp;
}
       

//allocated payload :header data footer / allocated payload : prev next data
int mm_init(void)
{
    if((free_listp = mem_sbrk(6*WSIZE)) == (void*)-1)
        return -1;
    PUT(free_listp, 0); //blank    
    PUT(free_listp + (1*WSIZE), PACK(DSIZE, 1)); // prologue H
    PUT(free_listp + (2*WSIZE), NULL); // prologue PREV
    PUT(free_listp + (3*WSIZE), NULL); // prologue NEXT
    PUT(free_listp + (4*WSIZE), PACK(DSIZE, 1)); // prologue F
    PUT(free_listp + (5*WSIZE), PACK(0, 1)); // blank
    free_listp += (2*WSIZE);
    if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
        return -1;
    return 0;
}


static void* extend_heap(size_t words)
{
    char* bp;
    size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if((long)(bp = mem_sbrk(size))==-1)
        return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return coalesce(bp);
}


void mm_free(void* bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}


static void* coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc){ 
        RootConnect(bp);        
        return bp;
    }
    else if (prev_alloc && !next_alloc){ //with next
        //헤더를 바꾸기 전에 next의 연결상태를 처리
        ReConnect(bp,1);        
        RootConnect(bp);

        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc){//with prev
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);     

        ReConnect(bp,0);
        RootConnect(bp);
    }
    else{
        ReConnect(bp,1);

        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);

        ReConnect(bp,0);
        RootConnect(bp);
    }
    return bp;
}


void* mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char* bp;
    if (size == 0)
        return NULL;
    if (size <= DSIZE)
        asize = 4*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1))/DSIZE);
    if ((bp = find_fit(asize)) != NULL){
        place(bp, asize);                  
        return bp;
    }
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE))==NULL)
        return NULL;
    place(bp, asize);
    return bp;
}


static void *find_fit(size_t asize)
{
    void *bp=free_listp;
    while(bp !=NULL){
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
            return bp;
        }        
        bp =NEXT(bp);
    }

    return NULL;
}


static void place(void* bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    if ((csize - asize) >= (4*DSIZE)){
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
    }
    else{
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}


void* mm_realloc(void* bp, size_t size)
{
	void* old_bp = bp;
	void* new_bp;
	size_t copySize;
	new_bp = mm_malloc(size); // 다른데다가 다시 할당 받기
	if (new_bp == NULL)       // 실패하면 NULL 리턴
		return NULL;
	copySize = *(size_t*)((char*)old_bp - WSIZE);
	if (size < copySize)
		copySize = size;
	memcpy(new_bp, old_bp, copySize);
	mm_free(old_bp);
	return new_bp;
}