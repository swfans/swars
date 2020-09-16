
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

/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbKeyboardOpen_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		mov    $0x1,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbKeyboardClose_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		mov    $0x1,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbKeyboard_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		call   LbDosKeyboard_
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x80,%eax
		jge    jump_ec412
		xor    %eax,%eax
		mov    lbInkey,%al
		mov    lbInkeyToAscii(%eax),%al
		and    $0xff,%eax
		pop    %ebp
		ret
	jump_ec412:
		xor    %eax,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
LbDosKeyboard_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x1c,%esp
		movb   $0x12,-0x1b(%ebp)
		lea    -0x1c(%ebp),%ebx
		lea    -0x1c(%ebp),%edx
		mov    $0x16,%eax
		call   ac_dos_int386
		xor    %dl,%dl
		mov    %dl,lbShift
		testb  $0x3,-0x1c(%ebp)
		je     jump_ec4b9
		movb   $0x1,lbShift
	jump_ec4b9:
		testb  $0x4,-0x1c(%ebp)
		je     jump_ec4c6
		orb    $0x2,lbShift
	jump_ec4c6:
		testb  $0x8,-0x1c(%ebp)
		je     jump_ec4d3
		orb    $0x4,lbShift
	jump_ec4d3:
		xor    %ah,%ah
		mov    %ah,lbInkey
		mov    $0x11,%ah
		int    $0x16
		je     jump_ec4eb
		mov    $0x10,%ah
		int    $0x16
		mov    %ah,lbInkey
	jump_ec4eb:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void KInt_unused(void);
 * Unused alternative keyboard handler?
 */
KInt_unused:
/*----------------------------------------------------------------*/
		pusha
		push   %ds
		push   %es
		push   %fs
		push   %gs
		mov    %esp,%ebp
		sub    $0x8,%esp
		cld
		call   ____GETDS
		mov    $0x60,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %al,lbInkey
		xor    %eax,%eax
		mov    data_1e5ca0,%al
		cmp    $0xe0,%eax
		jne    jump_ec55b
		movb   $0x1,lbInkey_prefixed
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x2a,%eax
		je     jump_ec542
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0xaa,%eax
		jne    jump_ec552
	jump_ec542:
		movb   $0x80,lbInkey
		movl   $0x80,-0x8(%ebp)
		jmp    jump_ec588
	jump_ec552:
		mov    lbInkey,%al
		or     $0x80,%al
		jmp    jump_ec56a
	jump_ec55b:
		xor    %dh,%dh
		mov    %dh,lbInkey_prefixed
		mov    lbInkey,%al
		and    $0x7f,%al
	jump_ec56a:
		and    $0xff,%eax
		mov    %eax,-0x8(%ebp)
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x7f,%eax
		setle  %dl
		mov    -0x8(%ebp),%eax
		mov    %dl,lbKeyOn(%eax)
	jump_ec588:
		mov    lbInkey,%al
		mov    %al,data_1e5ca0
		xor    %bl,%bl
		mov    %bl,lbInkeyFlags
		cmpb   $0x0,lbKeyOn+42
		jne    jump_ec5ac
		cmpb   $0x0,lbKeyOn+54
		je     jump_ec5b3
	jump_ec5ac:
		orb    $0x10,lbInkeyFlags
	jump_ec5b3:
		cmpb   $0x0,lbKeyOn+29
		jne    jump_ec5c5
		cmpb   $0x0,lbKeyOn+157
		je     jump_ec5cc
	jump_ec5c5:
		orb    $0x20,lbInkeyFlags
	jump_ec5cc:
		cmpb   $0x0,lbKeyOn+56
		jne    jump_ec5de
		cmpb   $0x0,lbKeyOn+184
		je     jump_ec5e5
	jump_ec5de:
		orb    $0x40,lbInkeyFlags
	jump_ec5e5:
		mov    -0x8(%ebp),%eax
		cmpb   $0x0,lbKeyOn(%eax)
		je     jump_ec600
		mov    lbInkeyFlags,%dl
		mov    -0x8(%ebp),%eax
		or     %dl,lbKeyOn(%eax)
	jump_ec600:
		mov    $0x61,%edx
		sub    %eax,%eax
		in     (%dx),%al
		cwtl
		mov    %eax,-0x8(%ebp)
		mov    -0x8(%ebp),%eax
		mov    %eax,-0x4(%ebp)
		mov    -0x8(%ebp),%ebx
		or     $0x80,%bl
		mov    %ebx,-0x8(%ebp)
		mov    -0x8(%ebp),%eax
		out    %al,(%dx)
		mov    -0x4(%ebp),%eax
		out    %al,(%dx)
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x80,%eax
		jge    jump_ec64e
		cmpb   $0x0,lbIInkey
		jne    jump_ec64e
		mov    lbInkey,%al
		mov    %al,lbIInkey
		mov    lbInkeyFlags,%al
		mov    %al,lbIInkeyFlags
	jump_ec64e:
		mov    $0x20,%al
		mov    $0x20,%edx
		out    %al,(%dx)
		mov    %ebp,%esp
		pop    %gs
		pop    %fs
		pop    %es
		pop    %ds
		popa
		iret


/*----------------------------------------------------------------*/
func_8c4c0_keyboard_clear_buffer:
/*----------------------------------------------------------------*/
		push   %edx
		xor    %edx,%edx
		mov    %edx,keyboard_buffer_read_index
		mov    keyboard_buffer_read_index,%eax
		mov    %eax,keyboard_buffer_write_index
		pop    %edx
		ret


/*----------------------------------------------------------------*/
func_8c4d8_keyboard_read_key:
/*----------------------------------------------------------------*/
		push   %edx
		mov    keyboard_buffer_write_index,%eax
		mov    keyboard_buffer_read_index,%edx
		cmp    %edx,%eax
		jne    jump_8c4ec
		xor    %edx,%eax
		pop    %edx
		ret
	jump_8c4ec:
		mov    keyboard_buffer_read_index,%eax
		mov    keyboard_buffer(,%eax,4),%eax
		mov    keyboard_buffer_read_index,%edx
		incl   keyboard_buffer_read_index
		andl   $0xf,keyboard_buffer_read_index
		pop    %edx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbIKeyboardOpen)	/* 8c510 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    $0x100,%ebx
		mov    $lbKeyOn,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    %dx,data_1c47c4

		/* XXX: early return */
#if 0
		mov    %cs,%ecx
		mov    $0x9,%eax
		mov    $KInt,%ebx
		call   ac_dos_getvect
		mov    %eax,data_1c47c0
		mov    $0x9,%eax
		call   ac_dos_setvect
#endif
		mov    $0x1,%eax
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
KInt:	/* 8c578 */
/*----------------------------------------------------------------*/
		pusha
		push   %ds
		push   %es
		push   %fs
		push   %gs
		mov    %esp,%ebp
		cld
		call   ____GETDS
		mov    $0x60,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %al,lbInkey
		xor    %eax,%eax
		mov    data_1c4774,%al
		cmp    $0xe0,%eax
		jne    jump_8c5f6
		movb   $0x1,lbInkey_prefixed
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x2a,%eax
		je     jump_8c5c3
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0xaa,%eax
		jne    jump_8c5d4
	jump_8c5c3:
		movb   $0x80,lbInkey
		mov    $0x80,%eax
		jmp    jump_8c64c
	jump_8c5d4:
		mov    lbInkey,%al
		or     $0x80,%al
		and    $0xff,%eax
		xor    %ebx,%ebx
		mov    lbInkey,%bl
		cmp    $0x7f,%ebx
		setle  %bl
		mov    %bl,lbKeyOn(%eax)
		jmp    jump_8c64c
	jump_8c5f6:
		xor    %ah,%ah
		mov    %ah,lbInkey_prefixed
		mov    lbInkey,%al
		and    $0x7f,%al
		and    $0xff,%eax
		xor    %ebx,%ebx
		mov    lbInkey,%bl
		cmp    $0x7f,%ebx
		setle  %bl
		mov    %bl,lbKeyOn(%eax)
		cmpb   $0x0,lbKeyOn(%eax)
		je     jump_8c64c
		mov    keyboard_buffer_write_index,%ebx
		mov    %eax,keyboard_buffer(,%ebx,4)
		mov    keyboard_buffer_write_index,%ebx
		inc    %ebx
		and    $0xf,%ebx
		cmp    keyboard_buffer_read_index,%ebx
		je     jump_8c64c
		mov    %ebx,keyboard_buffer_write_index
	jump_8c64c:
		mov    lbInkey,%bl
		mov    %bl,data_1c4774
		xor    %bh,%bh
		mov    %bh,lbInkeyFlags
		cmpb   $0x0,lbKeyOn+42
		jne    jump_8c672
		cmpb   $0x0,lbKeyOn+54
		je     jump_8c679
	jump_8c672:
		orb    $0x10,lbInkeyFlags
	jump_8c679:
		cmpb   $0x0,lbKeyOn+29
		jne    jump_8c68b
		cmpb   $0x0,lbKeyOn+157
		je     jump_8c692
	jump_8c68b:
		orb    $0x20,lbInkeyFlags
	jump_8c692:
		cmpb   $0x0,lbKeyOn+56
		jne    jump_8c6a4
		cmpb   $0x0,lbKeyOn+184
		je     jump_8c6ab
	jump_8c6a4:
		orb    $0x40,lbInkeyFlags
	jump_8c6ab:
		cmpb   $0x0,lbKeyOn(%eax)
		je     jump_8c6c0
		mov    lbInkeyFlags,%bl
		or     %bl,lbKeyOn(%eax)
	jump_8c6c0:
		mov    $0x61,%edx
		sub    %eax,%eax
		in     (%dx),%al
		cwtl
		mov    %eax,%ebx
		or     $0x80,%al
		out    %al,(%dx)
		mov    %bl,%al
		out    %al,(%dx)
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x80,%eax
		jge    jump_8c6fc
		cmpb   $0x0,lbIInkey
		jne    jump_8c6fc
		mov    lbInkey,%al
		mov    %al,lbIInkey
		mov    lbInkeyFlags,%al
		mov    %al,lbIInkeyFlags
	jump_8c6fc:
		mov    $0x20,%al
		mov    $0x20,%edx
		out    %al,(%dx)
		pop    %gs
		pop    %fs
		pop    %es
		pop    %ds
		popa
		iret


.section .rodata

GLOBAL (lbInkeyToAscii)
		.ascii  "\x00\x00"
		.ascii  "1234567890-=\x08\t"
		.ascii  "qwertyuiop[]\x00\x00"
		.ascii  "asdfghjkl;'`\x00"
		.ascii  "#zxcvbnm,./\x00\x2a"
		.ascii  "\x00\x20\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x2d\x00\x00\x00\x2b\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x5c\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x00\x00"
		.ascii  "\x2f\x00\x00"
		.ascii  "()/*\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x2e\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x21\x22\x9c"
		.ascii  "$%^&*()_+\x08\t"
		.ascii  "QWERTYUIOP{}\x00\x00"
		.ascii  "ASDFGHJKL:@~\x00"
		.ascii  "~ZXCVBNM<>?\x00\x2a"
		.ascii  "\x00\x20\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x00\x00"
		.ascii  "\x00\x00\x2d\x00\x00\x00\x2b\x00"
		.ascii  "\x00\x00\x00\x00\x00\x00\x7c\x00"
		.fill   0x8
		.ascii  "\x2f\x00\x00"
		.ascii "()/*\x00"
		.fill   0x8
		.ascii  "\x00\x2e\x00\x00\x00\x00\x00\x00"
		.fill   0x8

.data

data_1c4774:
		.long	0x0
GLOBAL (keyboard_buffer)	/* 1c4778 */
		.fill   0x40
GLOBAL (keyboard_buffer_write_index)	/* 1c47b8 */
		.long	0x0
GLOBAL (keyboard_buffer_read_index)	/* 1c47bc */
		.long	0x0
data_1c47c0:
		.long	0x0
data_1c47c4:
		.fill   0x14

GLOBAL (lbInkey_prefixed)	/* 1e2f70 */
		.long	0x0

GLOBAL (lbKeyOn)			/* 1e2f74 */
		.fill   0x100

GLOBAL (lbInkey)	/* 1e3074 */
		.byte	0x0

lbInkeyFlags:
		.byte	0x0
lbIInkey:
		.byte	0x0
lbIInkeyFlags:
		.byte	0x0
GLOBAL (lbShift)
		.short  0x0

data_1e5ca0:
		.long	0x0
