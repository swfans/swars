#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


#define AVAILABLE_MEMORY (16*1024*1024)
#define TABLE_SIZE 256 /* hardcoded, do not change */


struct MemoryBlock
{
  void    *address;
  uint32_t selector;
  uint32_t size;
} __attribute__ ((packed));

typedef struct MemoryBlock MemoryBlock;


struct MemoryBlockNode
{
  void    *address;
  uint32_t size;
  struct MemoryBlockNode *next;
  struct MemoryBlockNode *prev;
  uint8_t  unknown;
  uint8_t  block_index;
} __attribute__ ((packed));

typedef struct MemoryBlockNode MemoryBlockNode;

#pragma pack(1)

extern MemoryBlock     memory_blocks[TABLE_SIZE];
extern MemoryBlockNode memory_arenas[TABLE_SIZE];

#pragma pack()

static size_t
get_block_count (void)
{
  size_t n;

  for (n = 0; n < TABLE_SIZE; n++)
    {
      if (memory_blocks[n].address == NULL)
	return n;
    }

  return TABLE_SIZE;
}

static void
initialise_block_nodes (void)
{
  size_t n;
  size_t count;

  count = get_block_count ();

  for (n = 0; n < count; n++)
    {
      memory_arenas[n].address	= memory_blocks[n].address;
      memory_arenas[n].size	= memory_blocks[n].size;
      memory_arenas[n].block_index = n;

      if (n != 0)
	memory_arenas[n].prev = &memory_arenas[n - 1];
      else
	memory_arenas[n].prev = NULL;

      if (n + 1 != count)
	memory_arenas[n].next = &memory_arenas[n + 1];
      else
	memory_arenas[n].next = NULL;
    }
}

bool
memory_preallocate (void)
{
  static bool allocated = false;

  if (allocated)
    return true;

  memset (&memory_blocks, 0, sizeof (memory_blocks));
  memset (&memory_arenas, 0, sizeof (memory_arenas));

  memory_blocks[0].address = malloc (AVAILABLE_MEMORY);
  memory_blocks[0].size    = AVAILABLE_MEMORY;

  assert (memory_blocks[0].address != NULL);

  initialise_block_nodes ();

  allocated = true;
  return true;
}
