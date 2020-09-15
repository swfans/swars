
#ifdef NEED_UNDERSCORE
# define TRANSFORM_SYMBOLS
# define EXPORT_SYMBOL(sym) _ ## sym
#endif

#ifndef TRANSFORM_SYMBOLS

# define GLOBAL(sym) \
  .global sym; \
  sym ##:

# define GLOBAL_FUNC(sym) \
  GLOBAL (sym)

#else

# define GLOBAL(sym) \
  .global EXPORT_SYMBOL (sym); \
  EXPORT_SYMBOL (sym) ##: \
  sym ##:

# define GLOBAL_FUNC(sym) \
  .global sym; \
  GLOBAL (sym)

#endif


.text

.global EXPORT_SYMBOL(XXX);


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbMemoryAlloc_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,%ecx
		call   ac_memory_preallocate
		mov    $memory_arenas,%edx
		mov    $0xffffffff,%ebx
		add    $0x3,%ecx
		xor    %esi,%esi
		and    $0xfc,%cl
		test   %edx,%edx
		jne    jump_dc6cb
		jmp    jump_dc6f5
	jump_dc6c4:
		mov    0x8(%edx),%edx
		test   %edx,%edx
		je     jump_dc6f5
	jump_dc6cb:
		mov    0x4(%edx),%edi
		cmp    %edi,%ecx
		ja     jump_dc6c4
		cmp    %edi,%ebx
		jbe    jump_dc6c4
		xor    %eax,%eax
		mov    0x10(%edx),%al
		test   %eax,%eax
		jne    jump_dc6c4
		mov    0x11(%edx),%al
		imul   $0xc,%eax,%eax
		cmpl   $0x0,data_1e08d8(%eax)
		jne    jump_dc6c4
		mov    %edx,%esi
		mov    0x4(%edx),%ebx
		jmp    jump_dc6c4
	jump_dc6f5:
		test   %esi,%esi
		je     jump_dc71a
		mov    %ecx,%edx
		mov    %esi,%eax
		call   split_arena
		test   %al,%al
		je     jump_dc71a
		call   LbMemoryCheck_
		mov    %ecx,%ebx
		mov    (%esi),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    (%esi),%eax
		jmp    jump_dc721
	jump_dc71a:
		mov    %ecx,%eax
		call   LbMemoryAllocLow_
	jump_dc721:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbMemoryAllocLow_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,%ecx
		call   ac_memory_preallocate
		mov    $memory_arenas,%edx
		mov    $0xffffffff,%ebx
		add    $0xf,%ecx
		xor    %esi,%esi
		and    $0xf0,%cl
		test   %edx,%edx
		jne    jump_dc753
		jmp    jump_dc77d
	jump_dc74c:
		mov    0x8(%edx),%edx
		test   %edx,%edx
		je     jump_dc77d
	jump_dc753:
		mov    0x4(%edx),%edi
		cmp    %edi,%ecx
		ja     jump_dc74c
		cmp    %edi,%ebx
		jbe    jump_dc74c
		xor    %eax,%eax
		mov    0x10(%edx),%al
		test   %eax,%eax
		jne    jump_dc74c
		mov    0x11(%edx),%al
		imul   $0xc,%eax,%eax
		cmpl   $0x0,data_1e08d8(%eax)
		je     jump_dc74c
		mov    %edx,%esi
		mov    0x4(%edx),%ebx
		jmp    jump_dc74c
	jump_dc77d:
		test   %esi,%esi
		je     jump_dc7a2
		mov    %ecx,%edx
		mov    %esi,%eax
		call   split_arena
		test   %al,%al
		je     jump_dc7a2
		call   LbMemoryCheck_
		mov    %ecx,%ebx
		mov    (%esi),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    (%esi),%eax
		jmp    jump_dc7a4
	jump_dc7a2:
		xor    %eax,%eax
	jump_dc7a4:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LbMemoryCheck_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		xor    %edx,%edx
		mov    $0xffffffff,%edi
		mov    %edx,data_1e08c0
		mov    %edx,data_1e08c4
		mov    %edx,data_1e08c8
		mov    %edx,data_1e08cc
		mov    $memory_arenas,%edx
		mov    %edi,data_1e08d0
		test   %edx,%edx
		jne    jump_dc7f0
		jmp    jump_dc85f
	jump_dc7e5:
		mov    0x8(%edx),%edx
		test   %edx,%edx
		je     jump_dc85f
	jump_dc7f0:
		xor    %eax,%eax
		mov    0x10(%edx),%al
		test   %eax,%eax
		jne    jump_dc83e
		mov    data_1e08c4,%ecx
		mov    0x4(%edx),%eax
		add    %eax,%ecx
		mov    data_1e08cc,%esi
		mov    %ecx,data_1e08c4
		cmp    %esi,%eax
		jbe    jump_dc819
		mov    %eax,data_1e08cc
	jump_dc819:
		mov    data_1e08d0,%edi
		mov    0x4(%edx),%eax
		cmp    %edi,%eax
		jae    jump_dc82b
		mov    %eax,data_1e08d0
	jump_dc82b:
		mov    data_1e08c0,%ebp
		mov    0x4(%edx),%eax
		add    %eax,%ebp
		mov    %ebp,data_1e08c0
		jmp    jump_dc7e5
	jump_dc83e:
		mov    data_1e08c8,%ebp
		mov    0x4(%edx),%eax
		mov    data_1e08c0,%ebx
		add    %eax,%ebp
		add    %eax,%ebx
		mov    %ebp,data_1e08c8
		mov    %ebx,data_1e08c0
		jmp    jump_dc7e5
	jump_dc85f:
		mov    data_1e08c0,%ah
		mov    data_1e08c4,%dl
		mov    data_1e08cc,%bl
		and    $0xfc,%ah
		and    $0xfc,%dl
		and    $0xfc,%bl
		mov    %ah,data_1e08c0
		mov    %dl,data_1e08c4
		mov    %bl,data_1e08cc
		mov    $0x1,%eax
		mov    data_1e08c8,%dh
		mov    data_1e08d0,%bh
		and    $0xfc,%dh
		and    $0xfc,%bh
		mov    %dh,data_1e08c8
		mov    %bh,data_1e08d0
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* uint32_t LbMemoryFree(void *mptr);
 */
GLOBAL_FUNC (LbMemoryFree_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    %eax,%ebx
		mov    $memory_arenas,%edx
		xor    %cl,%cl
		test   %edx,%edx
		jne    jump_dc8d1
		jmp    jump_dc8db
	jump_dc8ca:
		mov    0x8(%edx),%edx
		test   %edx,%edx
		je     jump_dc8db
	jump_dc8d1:
		cmp    (%edx),%ebx
		jne    jump_dc8ca
		mov    $0x1,%cl
		movb   $0x0,0x10(%edx)
	jump_dc8db:
		xor    %eax,%eax
		mov    %cl,%al
		cmp    $0x1,%eax
		jne    jump_dc908
		mov    $memory_arenas,%edx
		test   %edx,%edx
		jne    jump_dc8f6
		jmp    jump_dc911
	jump_dc8ef:
		mov    0x8(%edx),%edx
		test   %edx,%edx
		je     jump_dc911
	jump_dc8f6:
		xor    %eax,%eax
		mov    0x10(%edx),%al
		test   %eax,%eax
		jne    jump_dc8ef
		mov    %edx,%eax
		call   delete_arena
		jmp    jump_dc8ef
	jump_dc908:
		mov    $0xffffffff,%eax
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_dc911:
		call   LbMemoryCheck_
		mov    $0x1,%eax
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbMemoryReset_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		sub    $0x38,%esp
		mov    data_1e08dc,%edx
		mov    $memory_blocks,%ecx
		test   %edx,%edx
		jne    jump_dca04
		jmp    jump_dca5b
	jump_dca04:
		mov    $0x101,%edi
	jump_dca09:
		cmpl   $0x0,0x4(%ecx)
		je     jump_dca36
		mov    0x4(%ecx),%ax
		test   %ax,%ax
		je     jump_dca3d
		mov    %edi,(%esp)
		lea    0x1c(%esp),%ebx
		and    $0xffff,%eax
		mov    %esp,%edx
		mov    %eax,0xc(%esp)
		mov    $0x31,%eax
		call   ac_dos_int386
		jmp    jump_dca3d
	jump_dca36:
		mov    (%ecx),%eax
		call   ac_free
	jump_dca3d:
		movl   $0x0,0x8(%ecx)
		movl   $0x0,(%ecx)
		movl   $0x0,0x4(%ecx)
		mov    0x14(%ecx),%ebx
		add    $0xc,%ecx
		test   %ebx,%ebx
		jne    jump_dca09
	jump_dca5b:
		mov    $0x1,%eax
		add    $0x38,%esp
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LbMemorySetup_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0xb0,%esp
		cmpl   $0x0,data_1e08dc
		jne    jump_dcd51
		mov    $0x100,%ebx
		mov    $0x1000,%esi
		lea    0x54(%esp),%edx
		mov    $0x31,%eax
		mov    %ebx,0x54(%esp)
		lea    0x70(%esp),%ebx
		mov    %esi,0x58(%esp)
		call   ac_dos_int386
		mov    0x88(%esp),%edi
		xor    %ecx,%ecx
		test   %edi,%edi
		jne    jump_dcabf
		mov    0x7c(%esp),%ecx
		mov    0x70(%esp),%ebp
		shl    $0x10,%ecx
		or     %ebp,%ecx
	jump_dcabf:
		mov    $0xc,%edi
		mov    %ecx,0xa8(%esp)
		mov    $memory_blocks,%ecx
		xor    %ebp,%ebp
		lea    0xc00(%ecx),%esi
	jump_dcad8:
		mov    %edi,%ebx
		mov    %ebp,%edx
		mov    %ecx,%eax
		add    $0xc,%ecx
		call   ac_memset
		cmp    %esi,%ecx
		jne    jump_dcad8
		mov    $memory_arenas,%ecx
		mov    $0x12,%edi
		xor    %ebp,%ebp
		lea    0x1200(%ecx),%esi
	jump_dcafc:
		mov    %edi,%ebx
		mov    %ebp,%edx
		mov    %ecx,%eax
		add    $0x12,%ecx
		call   ac_memset
		cmp    %esi,%ecx
		je     jump_dcb10
		jmp    jump_dcafc
	jump_dcb10:
		mov    $0xa0000,%ecx
		xor    %ebp,%ebp
		xor    %esi,%esi
	jump_dcb19:
		xor    %edi,%edi
		cmp    $0xa0000,%ecx
		jae    jump_dcb67
		movl   $0x100,0x38(%esp)
		lea    0xf(%ecx),%eax
		lea    0x8c(%esp),%ebx
		shr    $0x4,%eax
		lea    0x38(%esp),%edx
		mov    %eax,0x3c(%esp)
		mov    $0x31,%eax
		call   ac_dos_int386
		cmpl   $0x0,0xa4(%esp)
		jne    jump_dcb67
		mov    0x98(%esp),%edi
		mov    0x8c(%esp),%ebx
		shl    $0x10,%edi
		or     %ebx,%edi
	jump_dcb67:
		test   %edi,%edi
		je     jump_dcb94
		mov    %edi,%eax
		inc    %ebp
		mov    %ecx,data_1e08dc(%esi)
		add    $0xc,%esi
		and    $0xffff,%eax
		shr    $0x10,%edi
		shl    $0x4,%eax
		mov    %edi,data_1e08cc(%esi)
		add    $0x400,%ecx
		mov    %eax,data_1e08c8(%esi)
	jump_dcb94:
		sub    $0x400,%ecx
		cmp    $0x400,%ecx
		jl     jump_dcba7
		jmp    jump_dcb19
	jump_dcba7:
		lea    0x0(,%ebp,4),%eax
		sub    %ebp,%eax
		shl    $0x2,%eax
		mov    $0x1000000,%ecx
		mov    %eax,0xac(%esp)
	jump_dcbbf:
		mov    %ecx,%eax
		mov    0xac(%esp),%edx
		call   ac_malloc
		mov    %eax,memory_blocks(%edx)
		test   %eax,%eax
		jne    jump_dcbe2
		jmp    jump_dcc23
	jump_dcbd9:
		sub    $0x10,%ebx
		cmp    %edi,%ebx
		jg     jump_dcbec
		jmp    jump_dcc0b
	jump_dcbe2:
		lea    0x1000(%ecx),%ebx
		mov    %edx,%esi
		mov    %ecx,%edi
	jump_dcbec:
		mov    %ebx,%edx
		mov    memory_blocks(%esi),%eax
		call   ac_realloc
		test   %eax,%eax
		je     jump_dcbd9
		xor    %edi,%edi
		mov    %ebx,data_1e08dc(%esi)
		mov    %edi,data_1e08d8(%esi)
	jump_dcc0b:
		mov    0xac(%esp),%eax
		add    $0x1000,%ecx
		add    $0xc,%eax
		inc    %ebp
		mov    %eax,0xac(%esp)
	jump_dcc23:
		sub    $0x1000,%ecx
		cmp    $0x1000,%ecx
		jl     jump_dcc33
		jmp    jump_dcbbf
	jump_dcc33:
		mov    %ebp,%edx
		lea    0x0(,%ebp,4),%ebx
		sub    %ebp,%ebx
		mov    $0x1000,%ecx
		shl    $0x2,%ebx
		xor    %ebp,%edx
	jump_dcc48:
		mov    %ecx,%eax
		call   ac_malloc
		mov    %eax,memory_blocks(%ebx)
		cmp    %eax,%edx
		je     jump_dcc6f
		add    $0xc,%ebx
		mov    %ecx,data_1e08d0(%ebx)
		inc    %ebp
		mov    %edx,data_1e08cc(%ebx)
		add    $0x1000,%ecx
	jump_dcc6f:
		sub    $0x10,%ecx
		cmp    $0x10,%ecx
		jge    jump_dcc48
		mov    $compare,%ecx
		mov    $0xc,%ebx
		mov    $memory_blocks,%eax
		mov    %ebp,%edx
		call   ac_qsort
		xor    %eax,%eax
	jump_dcc8f:
		add    $0x12,%eax
		xor    %edi,%edi
		mov    %edi,data_1e14c6(%eax)
		cmp    $0x1200,%eax
		jne    jump_dcc8f
		xor    %ecx,%ecx
		test   %ebp,%ebp
		jle    jump_dcd0c
		mov    $memory_arenas,%esi
		mov    $memory_arenas,%edi
		xor    %ebx,%ebx
		xor    %eax,%eax
		sub    $0x12,%esi
		add    $0x12,%edi
	jump_dccbb:
		mov    memory_blocks(%ebx),%edx
		mov    %edx,memory_arenas(%eax)
		mov    data_1e08dc(%ebx),%edx
		mov    %edx,data_1e14d8(%eax)
		test   %ecx,%ecx
		jne    jump_dcd04
		mov    %ecx,data_1e14e0(%eax)
	jump_dccdd:
		add    $0xc,%ebx
		add    $0x12,%esi
		mov    %edi,data_1e14dc(%eax)
		xor    %dl,%dl
		mov    %cl,data_1e14e5(%eax)
		add    $0x12,%eax
		add    $0x12,%edi
		inc    %ecx
		mov    %dl,data_1e14d2(%eax)
		cmp    %ebp,%ecx
		jge    jump_dcd0c
		jmp    jump_dccbb
	jump_dcd04:
		mov    %esi,data_1e14e0(%eax)
		jmp    jump_dccdd
	jump_dcd0c:
		lea    -0x1(%ebp),%edx
		lea    0x0(,%edx,8),%eax
		add    %edx,%eax
		xor    %ebx,%ebx
		mov    %ebx,data_1e14dc(,%eax,2)
		mov    0xa8(%esp),%eax
		shr    $0x10,%eax
		test   %ax,%ax
		je     jump_dcd51
		movl   $0x101,0x1c(%esp)
		mov    %esp,%ebx
		and    $0xffff,%eax
		lea    0x1c(%esp),%edx
		mov    %eax,0x28(%esp)
		mov    $0x31,%eax
		call   ac_dos_int386
	jump_dcd51:
		mov    $0x1,%eax
		add    $0xb0,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* uint8_t split_arena(mem_arena *marena, unsigned long arg2)
 */
split_arena:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edi
		mov    %eax,%ebx
		mov    %edx,%ecx
		cmp    0x4(%eax),%ecx
		jne    jump_dcdfe
		movb   $0x1,0x10(%eax)
		mov    $0x1,%al
		pop    %edi
		pop    %ecx
		pop    %ebx
		ret
	jump_dcdfe:
		mov    $memory_arenas,%edx
		cmp    $memory_arenas_end,%edx
		jb     jump_dce18
		jmp    jump_dce1e
	jump_dce0d:
		add    $0x12,%edx
		cmp    $memory_arenas_end,%edx
		jae    jump_dce1e
	jump_dce18:
		cmpl   $0x0,0x4(%edx)
		jne    jump_dce0d
	jump_dce1e:
		cmp    $memory_arenas_end,%edx
		jne    jump_dce2c
		xor    %al,%al
		pop    %edi
		pop    %ecx
		pop    %ebx
		ret
	jump_dce2c:
		mov    0x4(%ebx),%eax
		sub    %ecx,%eax
		mov    %eax,0x4(%edx)
		mov    (%ebx),%eax
		add    %ecx,%eax
		mov    %eax,(%edx)
		movb   $0x1,0x10(%ebx)
		mov    0x11(%ebx),%al
		mov    %ecx,0x4(%ebx)
		mov    %al,0x11(%edx)
		mov    0x8(%ebx),%eax
		mov    %eax,0x8(%edx)
		mov    %ebx,0xc(%edx)
		mov    0x8(%ebx),%edi
		test   %edi,%edi
		je     jump_dce5a
		mov    %edx,0xc(%edi)
	jump_dce5a:
		mov    $0x1,%al
		mov    %edx,0x8(%ebx)
		pop    %edi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void delete_arena(mem_arena *marena);
 */
delete_arena:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		cmpl   $0x0,0xc(%eax)
		je     jump_dcea7
		movb   $0x0,0x10(%eax)
		mov    0xc(%eax),%edx
		mov    0x11(%eax),%bl
		cmp    0x11(%edx),%bl
		jne    jump_dcea7
		xor    %ebx,%ebx
		mov    0x10(%edx),%bl
		test   %ebx,%ebx
		jne    jump_dcea7
		mov    0x8(%eax),%ebx
		test   %ebx,%ebx
		je     jump_dce8e
		mov    %edx,0xc(%ebx)
	jump_dce8e:
		mov    0xc(%eax),%ebx
		mov    0x8(%eax),%edx
		mov    %edx,0x8(%ebx)
		mov    0xc(%eax),%edx
		mov    0x4(%eax),%ebx
		add    %ebx,0x4(%edx)
		movl   $0x0,0x4(%eax)
	jump_dcea7:
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* int compare(mem_block *mblock1, mem_block *mblock2)
 */
compare:
/*----------------------------------------------------------------*/
		mov    0x4(%eax),%eax
		ret


.section .rodata

data_1e08c0:
		.long	0x0
data_1e08c4:
		.long	0x0
data_1e08c8:
		.long	0x0
data_1e08cc:
		.long	0x0
data_1e08d0:
		.long	0x0
/* mem_block memory_blocks[];
 */
GLOBAL (memory_blocks)	/* 1e08d4 */
		.long	0x0
data_1e08d8:
		.long	0x0
data_1e08dc:
		.fill   0xbea
data_1e14c6:
		.fill   0xc
data_1e14d2:
		.short  0x0
/* mem_arena memory_arenas[];
 */
GLOBAL (memory_arenas)	/* 1e14d4 */
		.long	0x0
data_1e14d8:
		.long	0x0
data_1e14dc:
		.long	0x0
data_1e14e0:
		.fill   0x5
data_1e14e5:
		.fill   0x11ef
/* This value is unused, but must be placed just after memory_arenas
 */
memory_arenas_end:
		.long	0x0
