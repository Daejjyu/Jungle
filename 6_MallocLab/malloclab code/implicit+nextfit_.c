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
}