#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* for sbrk */

/* As written in The C Programming Language by K&R */

typedef long Align_t;   /* for alignment to long boundary */

typedef union header {  /* block header */
    struct {
        union header *ptr;  /* next block if on free list */
        unsigned size;      /* size of this block */
    } s;
    Align_t x;    /* Force alignment to worst-case boundary */
} Header_t;

static Header_t base;     /* empty list to get started */
static Header_t *freePtr = NULL;    /* start of list of free memory blocks */

/*
 * Minimum number of units to request. Don't make too small as ifetching memory
 * will get intensive otherwise
 */
void myfree(void *ap)
{
    Header_t *blockPtr, *hPtr;

    blockPtr = ((Header_t *) ap) - 1; /* Point to block header */

    for (hPtr = freePtr;
         !(blockPtr > hPtr && blockPtr < hPtr->s.ptr);
         hPtr = hPtr->s.ptr) {
        if (hPtr >= hPtr->s.ptr
            && (blockPtr > hPtr || blockPtr < hPtr->s.ptr)) {
            break;  /* freed block at start or end of arena */
        }
    }

    if (blockPtr + blockPtr->s.size == hPtr->s.ptr) {
        /* Join to upper free block */
        blockPtr->s.size += hPtr->s.ptr->s.size;
        blockPtr->s.ptr   = hPtr->s.ptr->s.ptr;
    } else {
        blockPtr->s.ptr   = hPtr->s.ptr;
    }

    if (hPtr + hPtr->s.size == blockPtr) {
        /* Join to lower free block */
        hPtr->s.size += blockPtr->s.size;
        hPtr->s.ptr   = blockPtr->s.ptr;
    } else {
        hPtr->s.ptr   = blockPtr;
    }

    freePtr = hPtr;
}

#define NALLOC 1024

/*
 * Obtain storage from the operating system
 * Since asking the system for memory is a relatively expensive operation, we
 * don't want to do that on every call to malloc, so morecore requests at lEast
 * NALLOC units, which is chopped up as needed.
 */
static Header_t *morecore(unsigned nunits)
{
    void *currentPtr;
    Header_t *up;
    intptr_t increment;

    if (nunits < NALLOC) {
        nunits = NALLOC;
    }

    increment = nunits * sizeof(Header_t);

    printf("Increasing the program's data space by %li bytes...\n",
                                                          (long int) increment);
    currentPtr = sbrk(increment);

    if (currentPtr == (void *) -1) {    /* no space at all */
        return NULL;
    }

    up = (Header_t *) currentPtr;
    up->s.size = nunits;
    myfree((void *) (up + 1));  /* Add one so that we point at the free space */
    return freePtr;
}

/*
 * mymalloc: general-purpose storage allocator
 */
void *mymalloc(unsigned nbytes)
{
    Header_t *hPtr, *prevPtr;
    unsigned nunits;

    nunits = ((nbytes + sizeof(Header_t) - 1) / sizeof(Header_t)) + 1;

    printf("Preparing to allocate %d units (16 bytes/unit)"
           " (Header + data)...\n", nunits);

    prevPtr = freePtr;

    if (prevPtr == NULL) {
        printf("No free list yet, point it to base\n");

        base.s.ptr = &base;
        freePtr    = &base;
        prevPtr    = &base;

        base.s.size = 0;
    }

    for (hPtr = prevPtr->s.ptr; ; prevPtr = hPtr, hPtr = hPtr->s.ptr) {
        if (hPtr->s.size >= nunits) {
            printf("A block that is big enough to fit >= %d units was found!\n",
                                                                        nunits);
            if (hPtr->s.size == nunits) {
                printf("It fits exactly...");
                prevPtr->s.ptr = hPtr->s.ptr;
            } else {
                printf("It's a little big, use the tail end of it!");
                hPtr->s.size -= nunits;
                hPtr         += hPtr->s.size;
                hPtr->s.size  = nunits;
            }

            freePtr = prevPtr;

            /* Free space within block begins one unit beyond header */
            return (void *)(hPtr + 1);
        }

        if (hPtr == freePtr) {   /* wrapped around free list */
            hPtr = morecore(nunits);
            if (hPtr == NULL) {
                return NULL;    /* none left */
            }
        }
    }
}

/*****************************************************************************/
typedef struct {
    float array[20];
} custom_t;

int main(void)
{
    custom_t *c;

    printf("sizeof(Header_t *) = %lu\n", sizeof(Header_t *));
    printf("sizeof(unsigned) = %lu\n",   sizeof(unsigned));
    printf("sizeof(Align_t) = %lu\n",    sizeof(Align_t));
    printf("sizeof(Header_t) = %lu\n",   sizeof(Header_t));
    printf("Requesting sizeof(custom_t) = %lu bytes...\n", sizeof(custom_t));

    c = mymalloc(sizeof(custom_t));

    c->array[0] = 5;
    printf("c->array[0] = %f\n", c->array[0]);

    myfree(c);

    exit(0);
}
