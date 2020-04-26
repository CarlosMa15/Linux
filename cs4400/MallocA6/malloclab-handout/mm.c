  #include "mm.h"

  //rounds up to a multiple of the alignment for payloads and for block sizes
  #define ALIGNMENT 16
  #define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

  //excess space in block not used for payload
  //#define OVERHEAD sizeof(block_header)
  #define OVERHEAD (sizeof(block_header)+sizeof(block_footer))          // ADDED with block_footer

  //returns a pointer to the header to the given payload's block
  //bp = "Block Payload pointer"
  #define HDRP(bp) ((char *)(bp) - sizeof(block_header))

  // returns a pointer to the foot header to the given payload's block
  #define FTRP(bp) ((char *)(bp)+GET_SIZE(HDRP(bp))-OVERHEAD)           // ADDED with block_footer

  //returns the size of the given block
  #define GET_SIZE(p)  ((block_header *)(p))->size

  //returns whether the block is allocated or not
  #define GET_ALLOC(p) ((block_header *)(p))->allocated

  //returns a payload pointer for the next block, given a payload pointer
  #define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

  // return a payload pointer for the previous block, given a payload pointer
  #define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE((char *)(bp)-OVERHEAD))  // ADDED with block_footer

  #define GET(p) (*(size_t *)(p))                                       // ADDED

  // Naive Chunked sbrk Allocator
  #define CHUNK_SIZE (1 << 14)                                          // ADDED
  #define CHUNK_ALIGN(size) (((size)+(CHUNK_SIZE-1)) & ~(CHUNK_SIZE-1)) // ADDED

  //representation of header and is a multiple of the alignment
  typedef struct {
    size_t size;
    char   allocated;
  } block_header;

  //representation of block footer and is a multiple of alignment
  typedef struct {
    size_t size;
    int filler;
  } block_footer;

  /*****************************************************************
  static void set_allocated(void *bp)
  {
    GET_ALLOC(HDRP(bp)) = 1;
  }
  *****************************************************************/

  //pointer to the payload of the first block in our heap
  void *first_bp;
  void *last_bp;

  void extend(size_t new_size) {
    //size_t chunk_size = CHUNK_ALIGN(new_size);
    //void *bp = last_bp + chunk_size; //sbrk(chunk_size);
    void *bp = last_bp;

    GET_SIZE(HDRP(bp)) = new_size;
    GET_SIZE(FTRP(bp)) = new_size;
    GET_ALLOC(HDRP(bp)) = 0;

    GET_SIZE(HDRP(NEXT_BLKP(bp))) = 0;
    GET_ALLOC(HDRP(NEXT_BLKP(bp))) = 1;

    last_bp = NEXT_BLKP(bp);
  }

  void set_allocated(void *bp, size_t size) {
    size_t extra_size = GET_SIZE(HDRP(bp)) - size;
    if (extra_size > ALIGN(1 + OVERHEAD)) {
      GET_SIZE(HDRP(bp)) = size;
      GET_SIZE(FTRP(bp)) = size;
      GET_SIZE(HDRP(NEXT_BLKP(bp))) = extra_size;
      GET_SIZE(FTRP(NEXT_BLKP(bp))) = extra_size;
      GET_ALLOC(HDRP(NEXT_BLKP(bp))) = 0;
    }
    GET_ALLOC(HDRP(bp)) = 1;
  }

  void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {         //  Case 1
      /* nothing to do */
    }
    else if (prev_alloc && !next_alloc) {   // Case 2
       size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
       GET_SIZE(HDRP(bp)) = size;
       GET_SIZE(FTRP(bp)) = size;
    }
    else if (!prev_alloc && next_alloc) {   // Case 3 
      size += GET_SIZE(HDRP(PREV_BLKP(bp)));
      GET_SIZE(FTRP(bp)) = size;
      GET_SIZE(HDRP(PREV_BLKP(bp))) = size;
      bp = PREV_BLKP(bp);
  }
  else {                                    // Case 4 
      size += (GET_SIZE(HDRP(PREV_BLKP(bp)))
               + GET_SIZE(HDRP(NEXT_BLKP(bp))));
      GET_SIZE(HDRP(PREV_BLKP(bp))) = size;
      GET_SIZE(FTRP(NEXT_BLKP(bp))) = size;
      bp = PREV_BLKP(bp);
  }

    //....

    return bp;
  }

  // GETS THE BLOCK SET WITH BLOCK HEADER AND FOOT HEADER
  /*****************************************************************
  void mm_init(void *heap, size_t heap_size)
  {
    printf("\nSize : %ld\n", heap_size);
    void *bp;
    
    bp = heap + sizeof(block_header);

    GET_SIZE(HDRP(bp)) = heap_size;
    GET_ALLOC(HDRP(bp)) = 0;
    GET_SIZE(FTRP(bp)) = heap_size;

    first_bp = bp;
    printf("Block Allocated: %d\n", GET_ALLOC(HDRP(first_bp)));
    printf("Size Available: %ld\n", GET_SIZE(HDRP(first_bp)));
    printf("Foot Size: %ld\n", GET_SIZE(FTRP(bp)));
  }
  ****************************************************************/

  void mm_init(void *heap, size_t heap_size) {
    char malloc_size_str[11];
    sprintf(&malloc_size_str, "%d", heap_size);
    printf(malloc_size_str);  
    first_bp = heap + sizeof(block_header);// sbrk(0);
    last_bp = heap + sizeof(block_header);
    GET_SIZE(HDRP(first_bp)) = 0;
    GET_ALLOC(HDRP(first_bp)) = 1;
  }

  /****************************************************************
  void *mm_malloc(size_t size)
  {
    void *bp = first_bp;

    if (GET_ALLOC(HDRP(bp))) {
      // Our only block is already allocated
      return NULL;
    } else {
      set_allocated(bp);
      return bp;
    }
  }
  ****************************************************************/

  void *mm_malloc(size_t size) {
    int new_size = ALIGN(size + OVERHEAD);
    void *bp = first_bp;
    while (GET_SIZE(HDRP(bp)) != 0) {
      if (!GET_ALLOC(HDRP(bp))
          && (GET_SIZE(HDRP(bp)) >= new_size)) {
        set_allocated(bp, new_size);
        return bp;
      }
      bp = NEXT_BLKP(bp);
    }
    extend(new_size);
    set_allocated(bp, new_size);
    return bp;
  }

  /****************************************************************
  void mm_free(void *bp)
  {
    GET_ALLOC(HDRP(bp)) = 0;
  }
  ****************************************************************/

  void mm_free(void *bp) {
    GET_ALLOC(HDRP(bp)) = 0;
    coalesce(bp);
  }