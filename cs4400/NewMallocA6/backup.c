#include "mm.h"

// This is an 8 bit representation of a block header
typedef size_t block_header;

// This is an 8 bit representation of block footer
typedef size_t block_footer;

typedef struct list_node {
   struct list_node *prev;
   struct list_node *next;
} list_node;

// rounds up to a multiple of the alignment for payloads and for block sizes
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

// excess space in block not used for payload
#define OVERHEAD (sizeof(block_header)+sizeof(block_footer))

//returns a pointer to the header to the given payload's block, bp = "Block Payload pointer"
#define HDRP(bp) ((char *)(bp) - sizeof(block_header))

// returns a pointer to the foot header to the given payload's block
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp))-OVERHEAD)

//returns the size of the given block
#define GET_SIZE(p)  (GET(p) & ~0xF)

//returns whether the block is allocated or not
#define GET_ALLOC(p) (GET(p) & 0x1)

//returns a payload pointer for the next block, given a payload pointer
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

// return a payload pointer for the previous block, given a payload pointer
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE((char *)(bp)-OVERHEAD))  // ADDED with block_footer

// healper for the get size and get allocated on it
#define GET(p) (*(size_t *)(p))

// this puts the allocated and size into a block of size_t
#define PUT(p, val) (*(size_t *)(p) = (val))

// this packs the size and allocated to fit in block of type def
#define PACK(size, alloc) ((size) | (alloc))

#define MAX(x, y) ((x) > (y)? (x) : (y))

//pointer to the payload of the first block in our heap
void *first_bp;

// Sets the blocks to the proper allocation
void set_allocated(void *bp, size_t size) {
  //printf("%s\n", "set_allocated called");
  size_t extra_size = GET_SIZE(HDRP(bp)) - size;
  if (extra_size > ALIGN(1 + OVERHEAD)) {
    PUT(HDRP(bp), PACK(size, GET_ALLOC(HDRP(bp))));
    PUT(FTRP(bp), PACK(size, 0));

    PUT(HDRP(NEXT_BLKP(bp)), PACK(extra_size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(extra_size, 0));
  }
  PUT(HDRP(bp), PACK(GET_SIZE(HDRP(bp)), 1));
}

// Takes care of doing the proper combining when block is removed
void *coalesce(void *bp) {
  //printf("%s\n", "coalesce called");
  size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {         //  Case 1
    //printf("%s\n", "CASE 1");
    add_to_free_list((list_node *)bp);
  }
  else if (prev_alloc && !next_alloc) {   // Case 2
    //printf("%s\n", "CASE 2");
     size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
     PUT(HDRP(bp), PACK(size, GET_ALLOC(HDRP(bp))));
     PUT(FTRP(bp), PACK(size, GET_ALLOC(FTRP(bp))));
  }
  else if (!prev_alloc && next_alloc) {   // Case 3 
    //printf("%s\n", "CASE 3");
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, GET_ALLOC(FTRP(bp))));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, GET_ALLOC(HDRP(PREV_BLKP(bp)))));
    bp = PREV_BLKP(bp);
}
else {                                    // Case 4 
    //printf("%s\n", "CASE 4");
    size += (GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp))));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, GET_ALLOC(HDRP(PREV_BLKP(bp)))));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, GET_ALLOC(FTRP(NEXT_BLKP(bp)))));
    bp = PREV_BLKP(bp);
}
  return bp;
}

// creates mm in the given memory
void mm_init(void *heap, size_t heap_size) {
  //printf("mm_init size: %ld\n", heap_size);
  first_bp = heap + sizeof(block_header);

  PUT(HDRP(first_bp), PACK(8, 1));

  first_bp = NEXT_BLKP(first_bp) + 8;
  PUT(HDRP(first_bp), PACK(ALIGN(OVERHEAD), 1));
  PUT(FTRP(first_bp), PACK(ALIGN(OVERHEAD), 1));

  first_bp = NEXT_BLKP(first_bp);
  size_t N = heap_size - OVERHEAD - OVERHEAD;
  PUT(HDRP(first_bp), PACK(ALIGN(N), 0));
  PUT(FTRP(first_bp), PACK(ALIGN(N), 1));

  void *bp = NEXT_BLKP(first_bp);
  PUT(HDRP(bp), PACK(8, 1));
}

// adds a block to memory
void *mm_malloc(size_t size) {
  //printf("mm_malloc size: %ld\n", size);
  //int new_size = ALIGN(size + OVERHEAD);
  int need_size = MAX(size, sizeof(list_node));
  int new_size = ALIGN(need_size + OVERHEAD);

  void *bp = first_bp;
  while (GET_SIZE(HDRP(bp)) != 0) {
    if (!GET_ALLOC(HDRP(bp))
        && (GET_SIZE(HDRP(bp)) >= new_size)) {
      set_allocated(bp, new_size);
      return bp;
    }
    bp = NEXT_BLKP(bp);
  }
  return;
}

// removes block from memory
void mm_free(void *bp) {
  //printf("%s\n", "mm_free called");
  PUT(HDRP(bp), PACK(GET_SIZE(HDRP(bp)), 0));
  coalesce(bp);
}