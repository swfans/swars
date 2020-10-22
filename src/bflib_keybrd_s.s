
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
GLOBAL_FUNC (ASM_LbKeyboardClose_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		mov    $0x1,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (ASM_LbKeyboard_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		call   ASM_LbDosKeyboard
		xor    %eax,%eax
		mov    lbInkey,%al
		cmp    $0x80,%eax
		jge    jump_ec412
		xor    %eax,%eax
		mov    lbInkey,%al
		mov    EXPORT_SYMBOL(lbInkeyToAscii)(%eax),%al
		and    $0xff,%eax
		pop    %ebp
		ret
	jump_ec412:
		xor    %eax,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (ASM_LbDosKeyboard)
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

GLOBAL (ASM_lbInkeyToAscii)
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

GLOBAL (lbKeyNames)
		.long	0x0
		.long   data_15d154
		.long   data_15d15b
		.long   data_15d15d
		.long   data_15d15f
		.long   data_15d161
		.long   data_15d163
		.long   data_15d165
		.long   data_15d167
		.long   data_15d169
		.long   data_15d16b
		.long   data_15d16d
		.long   data_15d16f
		.long   data_15d171
		.long   data_15d173
		.long   data_15d17d
		.long   data_15d181
		.long   data_15d183
		.long   data_15d185
		.long   data_15d187
		.long   data_15d189
		.long   data_15d18b
		.long   data_15d18d
		.long   data_15d18f
		.long   data_15d191
		.long   data_15d193
		.long   data_15d195
		.long   data_15d197
		.long   data_15d199
		.long   data_15d1a0
		.long   data_15d1aa
		.long   data_15d1ac
		.long   data_15d1ae
		.long   data_15d1b0
		.long   data_15d1b2
		.long   data_15d1b4
		.long   data_15d1b6
		.long   data_15d1b8
		.long   data_15d1ba
		.long   data_15d1bc
		.long   data_15d1be
		.long   data_15d1c0
		.long   data_15d1c2
		.long   data_15d1cd
		.long   data_15d1cf
		.long   data_15d1d1
		.long   data_15d1d3
		.long   data_15d1d5
		.long   data_15d1d7
		.long   data_15d1d9
		.long   data_15d1db
		.long   data_15d1dd
		.long   data_15d1df
		.long   data_15d1e1
		.long   data_15d1e3
		.long   data_15d1ef
		.long   data_15d1f4
		.long   data_15d1fd
		.long   data_15d203
		.long   data_15d20c
		.long   data_15d20f
		.long   data_15d212
		.long   data_15d215
		.long   data_15d218
		.long   data_15d21b
		.long   data_15d21e
		.long   data_15d221
		.long   data_15d224
		.long   data_15d227
		.long   data_15d22b
		.long   data_15d233
		.long   data_15d23b
		.long   data_15d240
		.long   data_15d245
		.long   data_15d24a
		.long   data_15d24f
		.long   data_15d254
		.long   data_15d259
		.long   data_15d25e
		.long   data_15d263
		.long   data_15d268
		.long   data_15d26d
		.long   data_15d272
		.long   data_15d277
		.fill   0x8
		.long   data_15d27c
		.long   data_15d27e
		.long   data_15d282
		.fill   0x10c
		.long   data_15d286
		.long   data_15d28c
		.fill   0x5c
		.long   data_15d297
		.long	0x0
		.long   data_15d29c
		.long   data_15d2a6
		.fill   0x38
		.long   data_15d2b0
		.long   data_15d2b5
		.long   data_15d2b8
		.long	0x0
		.long   data_15d2bd
		.long	0x0
		.long   data_15d2c2
		.long	0x0
		.long   data_15d2c8
		.long   data_15d2cc
		.long   data_15d2d1
		.long   data_15d2d6
		.long   data_15d2dd

data_15d154:
		.string "ESCAPE"
data_15d15b:
		.ascii  "\x31\x00"
data_15d15d:
		.ascii  "\x32\x00"
data_15d15f:
		.ascii  "\x33\x00"
data_15d161:
		.ascii  "\x34\x00"
data_15d163:
		.ascii  "\x35\x00"
data_15d165:
		.ascii  "\x36\x00"
data_15d167:
		.ascii  "\x37\x00"
data_15d169:
		.ascii  "\x38\x00"
data_15d16b:
		.ascii  "\x39\x00"
data_15d16d:
		.ascii  "\x30\x00"
data_15d16f:
		.ascii  "\x2d\x00"
data_15d171:
		.ascii  "\x3d\x00"
data_15d173:
		.string "BACKSPACE"
data_15d17d:
		.ascii  "\x54\x41\x42\x00"
data_15d181:
		.ascii  "\x51\x00"
data_15d183:
		.ascii  "\x57\x00"
data_15d185:
		.ascii  "\x45\x00"
data_15d187:
		.ascii  "\x52\x00"
data_15d189:
		.ascii  "\x54\x00"
data_15d18b:
		.ascii  "\x59\x00"
data_15d18d:
		.ascii  "\x55\x00"
data_15d18f:
		.ascii  "\x49\x00"
data_15d191:
		.ascii  "\x4f\x00"
data_15d193:
		.ascii  "\x50\x00"
data_15d195:
		.ascii  "\x5b\x00"
data_15d197:
		.ascii  "\x5d\x00"
data_15d199:
		.string "RETURN"
data_15d1a0:
		.string "LEFT CTRL"
data_15d1aa:
		.ascii  "\x41\x00"
data_15d1ac:
		.ascii  "\x53\x00"
data_15d1ae:
		.ascii  "\x44\x00"
data_15d1b0:
		.ascii  "\x46\x00"
data_15d1b2:
		.ascii  "\x47\x00"
data_15d1b4:
		.ascii  "\x48\x00"
data_15d1b6:
		.ascii  "\x4a\x00"
data_15d1b8:
		.ascii  "\x4b\x00"
data_15d1ba:
		.ascii  "\x4c\x00"
data_15d1bc:
		.ascii  "\x3b\x00"
data_15d1be:
		.ascii  "\x27\x00"
data_15d1c0:
		.ascii  "\x60\x00"
data_15d1c2:
		.string "LEFT SHIFT"
data_15d1cd:
		.ascii  "\x23\x00"
data_15d1cf:
		.ascii  "\x5a\x00"
data_15d1d1:
		.ascii  "\x58\x00"
data_15d1d3:
		.ascii  "\x43\x00"
data_15d1d5:
		.ascii  "\x56\x00"
data_15d1d7:
		.ascii  "\x42\x00"
data_15d1d9:
		.ascii  "\x4e\x00"
data_15d1db:
		.ascii  "\x4d\x00"
data_15d1dd:
		.ascii  "\x2c\x00"
data_15d1df:
		.ascii  "\x2e\x00"
data_15d1e1:
		.ascii  "\x2f\x00"
data_15d1e3:
		.string "RIGHT SHIFT"
data_15d1ef:
		.string "KP *"
data_15d1f4:
		.string "LEFT ALT"
data_15d1fd:
		.string "SPACE"
data_15d203:
		.string "CAPSLOCK"
data_15d20c:
		.ascii  "\x46\x31\x00"
data_15d20f:
		.ascii  "\x46\x32\x00"
data_15d212:
		.ascii  "\x46\x33\x00"
data_15d215:
		.ascii  "\x46\x34\x00"
data_15d218:
		.ascii  "\x46\x35\x00"
data_15d21b:
		.ascii  "\x46\x36\x00"
data_15d21e:
		.ascii  "\x46\x37\x00"
data_15d221:
		.ascii  "\x46\x38\x00"
data_15d224:
		.ascii  "\x46\x39\x00"
data_15d227:
		.ascii  "\x46\x31\x30\x00"
data_15d22b:
		.string "NUMLOCK"
data_15d233:
		.string "SCRLOCK"
data_15d23b:
		.string "KP 7"
data_15d240:
		.string "KP 8"
data_15d245:
		.string "KP 9"
data_15d24a:
		.string "KP -"
data_15d24f:
		.string "KP 4"
data_15d254:
		.string "KP 5"
data_15d259:
		.string "KP 6"
data_15d25e:
		.string "KP +"
data_15d263:
		.string "KP 1"
data_15d268:
		.string "KP 2"
data_15d26d:
		.string "KP 3"
data_15d272:
		.string "KP 0"
data_15d277:
		.string "KP ."
data_15d27c:
		.ascii  "\x5c\x00"
data_15d27e:
		.ascii  "\x46\x31\x31\x00"
data_15d282:
		.ascii  "\x46\x31\x32\x00"
data_15d286:
		.string "ENTER"
data_15d28c:
		.string "RIGHT CTRL"
data_15d297:
		.string "KP /"
data_15d29c:
		.string "PRINT SCR"
data_15d2a6:
		.string "RIGHT ALT"
data_15d2b0:
		.string "HOME"
data_15d2b5:
		.ascii  "\x55\x50\x00"
data_15d2b8:
		.string "PGUP"
data_15d2bd:
		.string "LEFT"
data_15d2c2:
		.string "RIGHT"
data_15d2c8:
		.ascii  "\x45\x4e\x44\x00"
data_15d2cc:
		.string "DOWN"
data_15d2d1:
		.string "PGDN"
data_15d2d6:
		.string "INSERT"
data_15d2dd:
		.string "DELETE"

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

GLOBAL (lbInkey)	/* 9x1E3074 */
		.byte	0x0

lbInkeyFlags:
		.byte	0x0
lbIInkey:	/* 0x1E3076 */
		.byte	0x0
lbIInkeyFlags:
		.byte	0x0
GLOBAL (lbShift)	/* 0x1E3078 */
		.short  0x0

data_1e5ca0:
		.long	0x0
