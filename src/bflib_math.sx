
#ifdef NEED_UNDERSCORE
# define TRANSFORM_SYMBOLS
# define EXPORT_SYMBOL(sym) _ ## sym
#else
# define EXPORT_SYMBOL(sym) sym
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

/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbSqrL)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		add    $0xfffffffc,%esp
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,-0x4(%ebp)
		mov    -0x4(%ebp),%ecx
		or     %ecx,%ecx
		je     jump_e73d9
		bsr    %ecx,%eax
		movzwl data_e73e2(,%eax,2),%ebx
	jump_e73c0:
		mov    %ecx,%eax
		xor    %edx,%edx
		div    %ebx
		cmp    %ebx,%eax
		jge    jump_e73d0
		add    %eax,%ebx
		shr    %ebx
		jmp    jump_e73c0
	jump_e73d0:
		mov    %ebx,%eax
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		leave
		ret
	jump_e73d9:
		xor    %eax,%eax
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		leave
		ret
.section .rodata
data_e73e2:
		.ascii  "\x01\x00\x02\x00\x02\x00\x04\x00"
		.ascii  "\x05\x00\x08\x00\x0b\x00\x10\x00"
		.ascii  "\x16\x00\x20\x00\x2d\x00\x40\x00"
		.ascii  "\x5a\x00\x80\x00\xb5\x00\x00\x01"
		.ascii  "\x6a\x01\x00\x02\xd4\x02\x00\x04"
		.ascii  "\xa8\x05\x00\x08\x50\x0b\x00\x10"
		.ascii  "\xa0\x16\x00\x20\x41\x2d\x00\x40"
		.ascii  "\x82\x5a\x00\x80\x04\xb5\xff\xff"
		.fill   0xe
