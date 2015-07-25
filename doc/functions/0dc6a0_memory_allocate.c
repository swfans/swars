void *
func_dc6a0 (size_t size)
{
  memory_preallocate ();

  blockp = &memory_block_nodes[0];
  max_size = 0xffffffff;
  rounded_size = (size + 3) & ~4;
  res_block = NULL;

  for (blockp = &memory_block_nodes[0]; blockp != NULL; blockp = blockp->next)
    {
      if (blockp->size >= rounded_size && blockp->size < max_size
	  && blockp->unknown == 0
	  && memory_blocks[blockp->block_index].selector == 0)
	{
	  res_block = blockp;
	  max_size = blockp->size;
	}
    }

  if (res_block != NULL && func_dcde8 (res_block, rounded_size) != 0)
    {
      func_dc7ac ();
      memset (res_block->address, 0, rounded_size);

      return res_block->address;
    }

  return func_dc728 (rounded_size);
}
