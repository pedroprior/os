#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long int size_t;

struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
};

#define META_SIZE sizeof(struct block_meta)

void *first_block = NULL;

struct block_meta *find_free_block(struct block_meta **last, size_t size) {
  struct block_meta *current = first_block;

  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  return current;
};

#define MINREQ 0x2000
void *memory = NULL;
size_t endpos = 0;

void init_memory() {
  size_t required = PTRDIFF_MAX;

  while (memory == NULL) {
    if (required < MINREQ) {
      if (memory) {
        free(memory);
      }
      printf("Cannot allocate enough memory\n");
      exit(ENOMEM);
    }

    required >>= 1;
  }
}

void free_memory() { free(memory); }

struct block_meta *request_block(size_t size) {
  struct block_meta *last = NULL;
  struct block_meta *block = find_free_block(&last, size);

  if (block) {
    block->free = 0;
    return block;
  }

  block = memory + endpos;
  endpos += META_SIZE + size;

  if (last) {
    last->next = block;
  } else {
    first_block = block;
  }

  block->free = 0;
  block->next = NULL;
  block->size = size;

  return block;
}

void *malloc_impl(size_t size) {
  struct block_meta *block = request_block(size);

  return block + 1;
}

void free_impl(void *ptr) {
  struct block_meta *block = ptr - META_SIZE;
  block->free = 1;
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    return malloc_impl(size);
  }

  struct block_meta *block = ptr - META_SIZE;
  if (block->size >= size)
    return block + 1;

  uint8_t *newptr = malloc(size);

  size_t i;

  for (i = 0; i < (block->size < size ? block->size : size); i++) {
    newptr[i] = ((uint8_t *)ptr)[i];
  }
  block->free = 1;
  return newptr;
}