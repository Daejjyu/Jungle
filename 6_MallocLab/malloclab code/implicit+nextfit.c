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
    "team 2",
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
#define DSIZE 8                                        
#define CHUNKSIZE (1<<12)            
#define MAX(x,y) ((x)>(y)? (x) : (y))
#define PACK(size, alloc) ((size)| (alloc)) 
#define GET(p)            (*(unsigned int *)(p)) 
#define PUT(p, val)       (*(unsigned int *)(p) = (val))   
#define GET_SIZE(p)       (GET(p) & ~0x7)
#define GET_ALLOC(p)      (GET(p) & 0x1) 
#define HDRP(bp)          ((char* )(bp) - WSIZE)
#define FTRP(bp)          ((char* )(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define NEXT_BLKP(bp)     ((char*)(bp) + GET_SIZE(((char*)(bp)-WSIZE)))
#define PREV_BLKP(bp)     ((char*)(bp) - GET_SIZE(((char*)(bp)-DSIZE)))

static char *heap_listp;
static char *last_freep;
extern int mm_init(void);
extern void *mm_malloc(size_t size);
extern void mm_free(void *ptr);
static void* coalesce(void *bp);
static void* extend_heap(size_t words);

static void *find_fit(size_t asize);
static void place(void* bp, size_t asize);
void* mm_realloc(void* bp, size_t size);

int mm_init(void)
{
    heap_listp = last_freep +4;
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void*)-1)
        return -1;
    PUT(heap_listp, 0);
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3*WSIZE), PACK(0, 1));
    heap_listp += (2*WSIZE);
    if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
        return -1;
    return 0;
    /*
    저는 init함수에서 패딩과 프롤로그 블럭을 먼저 만들고 
    heap_listp를 프롤로그 블럭의 헤더와 풋터 사이에 위치시키 후에
    next_ptr(이 코드에서 last_freep)를 heap_listp로 초기화해주었습니다.
    
    이 과정을 제일 첫 줄의 코드에서 해주신 것 같은데 초기화 없이도 연산이 되는군요..! 새로운 사실 배워갑니다.

    */
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
    last_freep= bp;
    return coalesce(bp);
    /*
    83라인에서 last_freep를 bp로 옮겨주셨는데 제 코드에는 이 과정이 없었습니다. 그래서 왜 그런지 생각해봤는데 아직 이유를 잘 모르겠습니다.
    next_fit이라서 블럭이 확장될 때마다 last_freep를 옮겨주신걸까요?
    */
}

void mm_free(void* bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
    /*
    제 free와 같습니다:)
    */
}

static void* coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if (prev_alloc && next_alloc)
        return last_freep=bp;
    else if (prev_alloc && !next_alloc){
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc){
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else{
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
        return last_freep=bp;
    /*
        각 분기에서 필요한 경우 last_freep를 bp로 초기화해주셨군요! 
        저는 if else문이 끝나고 나서

        if ((next_ptr > (char*)bp) && (next_ptr < NEXT_BLKP(bp))){
        next_ptr = bp;

        이런 조건을 넣어서 필요한 경우 last_freep를 bp로 옮겨주었습니다.
        참고하시면 좋을 것 같습니다.
    }
        */
}

void* mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char* bp;
    if (size == 0)
        return NULL;
    if (size <= DSIZE)
        asize = 2*DSIZE;
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
    /*
    제 malloc과 같습니다:)
    */
}

static void *find_fit(size_t asize)
{
    void *bp = last_freep;
    while(bp!=PREV_BLKP(last_freep))   {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))){
            return bp;
        }
        bp = NEXT_BLKP(bp);
        if(GET_SIZE(HDRP(bp))==0) {
            bp = heap_listp;         
        }
    }
   return NULL;
   /*
   while문으로 구현하셨군요! for문 두 개로 구현하는 것 보다 훨씬 가독성이 좋은 것 같습니다.

   for (; GET_SIZE(HDRP(next_ptr)) > 0; next_ptr = NEXT_BLKP(next_ptr)){
        if (!GET_ALLOC(HDRP(next_ptr)) && (asize <= GET_SIZE(HDRP(next_ptr)))){
            return next_ptr;
        }
    }
    for (next_ptr = heap_listp; next_ptr < bp; next_ptr = NEXT_BLKP(next_ptr))
    {
        if (!GET_ALLOC(HDRP(next_ptr)) && (asize <= GET_SIZE(HDRP(next_ptr)))){
            return next_ptr;
        }
    }

    같은 내용을 담고 있는 코드인데 저는 이렇게 구현하였습니다. 잘 구현하신 것 같아요!
    
    사이즈가 0인 에필로그 블럭을 만나는 경우에 bp를 heap_listp로 옮겨주신 것 같습니다. 
   */
}

static void place(void* bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    if ((csize - asize) >= (2*DSIZE)){
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        return last_freep=bp;

    }
    else{
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
    /*
    제 코드와 같습니다:)
    */
}

void* mm_realloc(void* bp, size_t size)
{
	void* old_dp = bp;
	void* new_dp;
	size_t copySize;
	new_dp = mm_malloc(size); 
	if (new_dp == NULL)      
		return NULL;
	copySize = *(size_t*)((char*)old_dp - WSIZE);  
	if (size < copySize)
		copySize = size;
	memcpy(new_dp, old_dp, copySize);
	mm_free(old_dp);
	return new_dp;
    /*
    이번 한 주도 고생하셨습니다~!
    전체적으로 잘 만드신 코드인 것 같습니다. 변수명도 명확해서 코드를 읽는데 큰 어려움이 없었습니다.
    find_fit 코드를 특히 잘 만들어주신 것 같습니다:)
    */
}