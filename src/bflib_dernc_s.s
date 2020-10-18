
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

.global EXPORT_SYMBOL(X);


/*----------------------------------------------------------------*/
/* long LbFileLengthRnc(char *fname)
 */
GLOBAL_FUNC (LbFileLengthRnc_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x1c,%esp
		mov    %eax,-0x4(%ebp)
		movl   $0xffffffff,-0x8(%ebp)
		movb   $0x52,-0x14(%ebp)
		movb   $0x4e,-0x13(%ebp)
		movb   $0x43,-0x12(%ebp)
		movb   $0x1,-0x11(%ebp)
		xor    %bh,%bh
		mov    %bh,-0x10(%ebp)
		mov    -0x4(%ebp),%eax
		mov    $0x2,%edx
		call   ac_LbFileOpen
		mov    %eax,-0xc(%ebp)
		mov    -0xc(%ebp),%ebx
		cmp    $0xffffffff,%ebx
		jne    jump_ea486
		mov    %ebx,%eax
		jmp    jump_ea504
	jump_ea486:
		mov    -0xc(%ebp),%eax
		mov    $0x8,%ebx
		lea    -0x1c(%ebp),%edx
		call   ac_LbFileRead
		cmp    $0xffffffff,%eax
		jne    jump_ea4ae
		mov    -0xc(%ebp),%eax
		call   ac_LbFileClose
		mov    $0xffffffff,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ebx
		ret
	jump_ea4ae:
		mov    -0xc(%ebp),%eax
		call   ac_LbFileClose
		mov    $0x4,%ebx
		lea    -0x14(%ebp),%edx
		lea    -0x1c(%ebp),%eax
		call   ac_strncmp
		test   %eax,%eax
		jne    jump_ea4f6
		mov    -0x18(%ebp),%al
		mov    %eax,-0x8(%ebp)
		shll   $0x8,-0x8(%ebp)
		xor    %eax,%eax
		mov    -0x17(%ebp),%al
		add    %eax,-0x8(%ebp)
		shll   $0x8,-0x8(%ebp)
		xor    %eax,%eax
		mov    -0x16(%ebp),%al
		add    %eax,-0x8(%ebp)
		shll   $0x8,-0x8(%ebp)
		xor    %eax,%eax
		mov    -0x15(%ebp),%al
		add    %eax,-0x8(%ebp)
		jmp    jump_ea501
	jump_ea4f6:
		mov    -0x4(%ebp),%eax
		call   ac_LbFileLength
		mov    %eax,-0x8(%ebp)
	jump_ea501:
		mov    -0x8(%ebp),%eax
	jump_ea504:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* long LbFileLoadAt(const char *fname, void *buffer)
 */
GLOBAL_FUNC (LbFileLoadAt_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x14,%esp
		mov    %eax,-0x14(%ebp)
		mov    %edx,-0x10(%ebp)
		mov    -0x14(%ebp),%eax
		call   LbFileLengthRnc_
		mov    %eax,-0x4(%ebp)
		cmpl   $0xffffffff,-0x4(%ebp)
		je     jump_ea59f
		mov    -0x14(%ebp),%eax
		mov    $0x2,%edx
		call   ac_LbFileOpen
		mov    %eax,-0xc(%ebp)
		mov    -0xc(%ebp),%ebx
		cmp    $0xffffffff,%ebx
		je     jump_ea59f
		mov    -0x4(%ebp),%ebx
		mov    -0x10(%ebp),%edx
		mov    -0xc(%ebp),%eax
		call   ac_LbFileRead
		cmp    $0xffffffff,%eax
		je     jump_ea59f
		mov    -0xc(%ebp),%eax
		call   ac_LbFileClose
		mov    -0x10(%ebp),%edx
		mov    -0x10(%ebp),%eax
		call   UnpackM1_
		mov    %eax,-0x8(%ebp)
		mov    -0x8(%ebp),%ecx
		test   %ecx,%ecx
		jge    jump_ea58f
		mov    -0x14(%ebp),%eax
		push   %eax
		push   $data_15ed24
		call   LbErrorLog_
		mov    $0xffffffff,%eax
		add    $0x8,%esp
		jmp    jump_ea5a4
	jump_ea58f:
		cmpl   $0x0,-0x8(%ebp)
		jne    jump_ea59a
		mov    -0x4(%ebp),%eax
		jmp    jump_ea5a4
	jump_ea59a:
		mov    -0x8(%ebp),%eax
		jmp    jump_ea5a4
	jump_ea59f:
		mov    $0xffffffff,%eax
	jump_ea5a4:
		mov    %ebp,%esp
		pop    %ebp
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (UnpackM1_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		add    $0xfffffff8,%esp
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ds
		push   %es
		mov    %eax,-0x4(%ebp)
		mov    %edx,-0x8(%ebp)
		cld
		push   %ds
		pop    %es
		mov    -0x4(%ebp),%esi
		lods   %ds:(%esi),%ax
		cmp    $0x4e52,%ax
		jne    jump_fb77c
		lods   %ds:(%esi),%ax
		cmp    $0x143,%ax
		jne    jump_fb77c
		call   read_long
		mov    %eax,unpck_orig_size
		call   read_long
		mov    %eax,unpck_cmpr_size
		mov    0x5(%esi),%bl
		mov    %bl,unpck_hdrubt
		add    $0x6,%esi
		add    $0x12,%eax
		mov    -0x4(%ebp),%edx
		mov    -0x8(%ebp),%ebx
		add    %eax,%edx
		cmp    %ebx,%edx
		jbe    jump_fb6a3
		mov    -0x4(%ebp),%edi
		xor    %eax,%eax
		mov    0x10(%edi),%al
		add    unpck_orig_size,%eax
		add    %eax,%ebx
		cmp    %edx,%ebx
		jbe    jump_fb6a3
		mov    %edx,%esi
		mov    %ebx,%edi
		sub    $0x4,%esi
		sub    $0x4,%edi
		mov    unpck_cmpr_size,%ecx
		shr    $0x2,%ecx
		std
		rep movsl %ds:(%esi),%es:(%edi)
		add    $0x4,%esi
		add    $0x4,%edi
		mov    unpck_cmpr_size,%cx
		and    $0x3,%cx
		jcxz   jump_fb6a0
	jump_fb6a0:
		cld
		mov    %edi,%esi
	jump_fb6a3:
		mov    -0x8(%ebp),%edi
		movb   $0x0,unpck_byte_1592c9
		mov    (%esi),%ax
		mov    %ax,unpck_word_1592c4
		mov    $0x2,%al
		call   input_bits
	jump_fb6bd:
		mov    $unpck_byte_159138,%edx
		call   make_huftable
		mov    $unpck_byte_1591b8,%edx
		call   make_huftable
		mov    $unpck_byte_159238,%edx
		call   make_huftable
		mov    $0x10,%al
		call   input_bits
		mov    %ax,unpck_word_1592c2
		jmp    jump_fb714
	jump_fb6ea:
		mov    $unpck_byte_1591b8,%edx
		call   input_value
		push   %cx
		mov    $unpck_byte_159238,%edx
		call   input_value
		add    $0x2,%cx
		xor    %eax,%eax
		pop    %ax
		inc    %ax
		mov    %esi,%edx
		mov    %edi,%esi
		sub    %eax,%esi
		rep movsb %ds:(%esi),%es:(%edi)
		mov    %edx,%esi
	jump_fb714:
		mov    $unpck_byte_159138,%edx
		call   input_value
		jcxz   jump_fb75f
	jump_fb75f:
		decw   unpck_word_1592c2
		jne    jump_fb6ea
		decb   unpck_hdrubt
		jne    jump_fb6bd
		mov    unpck_orig_size,%eax
		clc
		jmp    jump_fb790
	jump_fb77c:
		mov    $0x0,%eax
		jmp    jump_fb78f
	jump_fb78f:
		stc
	jump_fb790:
		pop    %es
		pop    %ds
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		leave
		ret

/*----------------------------------------------------------------*/
read_long:
/*----------------------------------------------------------------*/
		lods   %ds:(%esi),%eax
		xchg   %ah,%al
		rol    $0x10,%eax
		xchg   %ah,%al
		ret


/*----------------------------------------------------------------*/
input_value:
/*----------------------------------------------------------------*/
		xchg   %edx,%esi
		mov    unpck_word_1592c4,%cx
	jump_fb7ab:
		lods   %ds:(%esi),%ax
		mov    %ax,%bx
		and    %cx,%bx
		lods   %ds:(%esi),%ax
		cmp    %bx,%ax
		jne    jump_fb7ab
		mov    0x3c(%esi),%cx
		xchg   %edx,%esi
		mov    %ch,%al
		call   input_bits
		xor    %ch,%ch
		cmp    $0x2,%cl
		jb     jump_fb7e4
		dec    %cl
		mov    %cl,%al
		call   input_bits
		mov    $0x1,%bx
		shl    %cl,%bx
		or     %bx,%ax
		mov    %ax,%cx
	jump_fb7e4:
		ret


/*----------------------------------------------------------------*/
input_bits:
/*----------------------------------------------------------------*/
		push   %cx
		mov    %al,%cl
		mov    data_1592c6,%ax
		mov    unpck_word_1592c4,%bx
		mov    unpck_byte_1592c9,%ch
		mov    $0x1,%dx
		shl    %cl,%dx
		dec    %dx
		and    %bx,%dx
		push   %dx
		sub    %cl,%ch
		jae    jump_fb838
		add    %cl,%ch
		xchg   %cl,%ch
		mov    $0x1,%dx
		shl    %cl,%dx
		dec    %dx
		and    %ax,%dx
		ror    %cl,%dx
		shr    %cl,%ax
		shr    %cl,%bx
		or     %dx,%bx
		add    $0x2,%esi
		mov    (%esi),%ax
		xchg   %cl,%ch
		sub    %ch,%cl
		mov    $0x10,%ch
		sub    %cl,%ch
	jump_fb838:
		mov    $0x1,%dx
		shl    %cl,%dx
		dec    %dx
		and    %ax,%dx
		ror    %cl,%dx
		shr    %cl,%ax
		shr    %cl,%bx
		or     %dx,%bx
		mov    %ax,data_1592c6
		mov    %bx,unpck_word_1592c4
		mov    %ch,unpck_byte_1592c9
		pop    %ax
		pop    %cx
		ret


/*----------------------------------------------------------------*/
make_huftable:
/*----------------------------------------------------------------*/
		push   %edi
		push   %edx
		sub    $0x10,%esp
		mov    %esp,%edi
		mov    $0x5,%al
		call   input_bits
		xor    %ecx,%ecx
		mov    %ax,%cx
		jcxz   jump_fb8fb
	jump_fb8fb:
		add    $0x10,%esp
		pop    %edx
		pop    %edi
		ret


.section .rodata

data_15ed24:
		.string "ERROR decompressing %s\n"

.data

unpck_byte_159138:
		.fill   0x80
unpck_byte_1591b8:
		.fill   0x80
unpck_byte_159238:
		.fill   0x80
unpck_orig_size:
		.long	0x0
unpck_cmpr_size:
		.fill   0x6
unpck_word_1592c2:
		.short  0x0
unpck_word_1592c4:
		.short  0x0
data_1592c6:
		.short  0x0
unpck_hdrubt:
		.byte	0x0
unpck_byte_1592c9:
		.byte	0x0
		.align	4

