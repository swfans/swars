
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

.global EXPORT_SYMBOL(vec_window_width);
.global EXPORT_SYMBOL(vec_window_height);
.global EXPORT_SYMBOL(vec_mode);
.global EXPORT_SYMBOL(vec_screen);
.global EXPORT_SYMBOL(vec_map);
.global EXPORT_SYMBOL(sprites);


/*----------------------------------------------------------------*/
GLOBAL_FUNC (draw_gpoly_)
/*----------------------------------------------------------------*/
		pusha
		sub    $0x190,%esp
		xor    %ecx,%ecx
		or     EXPORT_SYMBOL(vec_mode),%cl
		jmp    *vtable_12b16c(,%ecx,4)


/*----------------------------------------------------------------*/
func_128396:
/*----------------------------------------------------------------*/
		/*
		 * XXX: if this is commented out then the gray shield bars are
		 * not drawn (instead only white health bars appear)
		 */
		call   EXPORT_SYMBOL(trig_)
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_1283a3:
/*----------------------------------------------------------------*/
		mov    (%eax),%ebp
		or     (%edx),%ebp
		or     (%ebx),%ebp
		js     jump_1283e0
		mov    EXPORT_SYMBOL(vec_window_width),%ecx
		mov    (%eax),%esi
		cmp    %ecx,%esi
		jg     jump_1283e0
		mov    (%edx),%esi
		cmp    %ecx,%esi
		jg     jump_1283e0
		mov    (%ebx),%esi
		cmp    %ecx,%esi
		jg     jump_1283e0
		mov    $0x0,%ebp
		mov    %ebp,0x170(%esp)
		jmp    jump_1283f0
	jump_1283e0:
		mov    $0x1,%ebp
		mov    %ebp,0x170(%esp)
		cs
		mov    %eax,%eax
		nop
	jump_1283f0:
		mov    0x4(%eax),%ecx
		cmp    0x4(%edx),%ecx
		jle    jump_1283fc
		mov    0x4(%edx),%ecx
		xchg   %eax,%edx
	jump_1283fc:
		cmp    0x4(%ebx),%ecx
		jle    jump_128405
		mov    0x4(%ebx),%ecx
		xchg   %eax,%ebx
	jump_128405:
		mov    0x4(%edx),%ecx
		cmp    0x4(%ebx),%ecx
		jle    jump_12840f
		xchg   %edx,%ebx
	jump_12840f:
		mov    0x4(%eax),%ecx
		cmp    0x4(%ebx),%ecx
		je     jump_12d820
		mov    (%eax),%ecx
		mov    0x4(%eax),%ebp
		mov    %ecx,0x38(%esp)
		mov    %ebp,0x34(%esp)
		shl    $0x10,%ecx
		mov    %ecx,0x3c(%esp)
		mov    (%edx),%ecx
		mov    0x4(%edx),%ebp
		mov    %ecx,0x50(%esp)
		mov    %ebp,0x4c(%esp)
		shl    $0x10,%ecx
		mov    %ecx,0x54(%esp)
		mov    (%ebx),%ecx
		mov    0x4(%ebx),%ebp
		mov    %ecx,0x68(%esp)
		mov    %ebp,0x64(%esp)
		shl    $0x10,%ecx
		mov    %ecx,0x6c(%esp)
		mov    0x10(%eax),%edi
		mov    0x10(%edx),%ebp
		mov    0x10(%ebx),%ecx
		shr    $0x10,%edi
		shr    $0x10,%ebp
		shr    $0x10,%ecx
		mov    %edi,0x40(%esp)
		mov    %ebp,0x58(%esp)
		mov    %ecx,0x70(%esp)
		mov    0x8(%eax),%edi
		mov    0xc(%eax),%ebp
		mov    0x8(%edx),%ecx
		mov    0xc(%edx),%esi
		shr    $0x10,%edi
		shr    $0x10,%ebp
		shr    $0x10,%ecx
		shr    $0x10,%esi
		mov    %edi,0x44(%esp)
		mov    %ebp,0x48(%esp)
		mov    %ecx,0x5c(%esp)
		mov    %esi,0x60(%esp)
		mov    0x8(%ebx),%ecx
		mov    0xc(%ebx),%esi
		shr    $0x10,%ecx
		shr    $0x10,%esi
		mov    %ecx,0x74(%esp)
		mov    %esi,0x78(%esp)
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		je     jump_12859c
		mov    0x68(%esp),%eax
		sub    0x38(%esp),%eax
		test   $0xffffffe0,%ecx
		jne    jump_128570
		cmp    $0xffffffe0,%eax
		jl     jump_128570
		cmp    $0x1f,%eax
		jg     jump_128570
		mov    %ecx,%ebx
		shl    $0x8,%ebx
		mov    data_12b8a0(%ebx,%eax,4),%eax
		mov    %eax,(%esp)
	jump_1284f4:
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		je     jump_12852c
		mov    0x50(%esp),%eax
		sub    0x38(%esp),%eax
		test   $0xffffffe0,%ecx
		jne    jump_128580
		cmp    $0xffffffe0,%eax
		jl     jump_128580
		cmp    $0x1f,%eax
		jg     jump_128580
		mov    %ecx,%ebx
		shl    $0x8,%ebx
		mov    data_12b8a0(%ebx,%eax,4),%eax
		mov    %eax,0x4(%esp)
	jump_12852c:
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		je     jump_12859c
		mov    0x68(%esp),%eax
		sub    0x50(%esp),%eax
		test   $0xffffffe0,%ecx
		jne    jump_128590
		cmp    $0xffffffe0,%eax
		jl     jump_128590
		cmp    $0x1f,%eax
		jg     jump_128590
		mov    %ecx,%ebx
		shl    $0x8,%ebx
		mov    data_12b8a0(%ebx,%eax,4),%eax
		mov    %eax,0x8(%esp)
		jmp    jump_12859c
	jump_128570:
		shl    $0x10,%eax
		cltd
		idiv   %ecx
		mov    %eax,(%esp)
		jmp    jump_1284f4
	jump_128580:
		shl    $0x10,%eax
		cltd
		idiv   %ecx
		mov    %eax,0x4(%esp)
		jmp    jump_12852c
	jump_128590:
		shl    $0x10,%eax
		cltd
		idiv   %ecx
		mov    %eax,0x8(%esp)
		mov    %eax,%eax
	jump_12859c:
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		mov    (%esp),%ebp
		mov    0x3c(%esp),%esi
		imul   %ecx,%ebp
		add    %ebp,%esi
		sub    0x54(%esp),%esi
		mov    %esi,0x7c(%esp)
		xor    %ecx,%ecx
		mov    EXPORT_SYMBOL(vec_mode),%cl
		jmp    *vtable_12b220(,%ecx,4)


/*----------------------------------------------------------------*/
func_1285c7:
/*----------------------------------------------------------------*/
		mov    0x50(%esp),%esi
		mov    0x7c(%esp),%edi
		or     %edi,%edi
		sub    $0x0,%esi
		add    $0x0,%esi
		mov    0x38(%esp),%eax
		sub    %eax,%esi
		mov    0x68(%esp),%edi
		sub    %eax,%edi
		mov    0x34(%esp),%eax
		mov    0x4c(%esp),%ebx
		sub    %eax,%ebx
		mov    0x64(%esp),%ecx
		sub    %eax,%ecx
		mov    %ecx,%eax
		imul   %esi,%ecx
		mov    0x7c(%esp),%ebp
		or     %ebp,%ebp
		js     jump_128604
		sub    %eax,%ecx
		sub    %eax,%ecx
	jump_128604:
		add    %eax,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		je     jump_12868a
		xor    %edx,%edx
		mov    $0x7fffffff,%eax
		idiv   %ebx
		mov    %eax,%ebp
		mov    0x34(%esp),%eax
		mov    0x64(%esp),%esi
		sub    %eax,%esi
		mov    0x4c(%esp),%edi
		sub    %eax,%edi
		mov    %ebp,%eax
		mov    0x44(%esp),%edx
		mov    0x74(%esp),%ebx
		sub    %edx,%ebx
		mov    0x5c(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128651
		inc    %eax
	jump_128651:
		mov    %eax,0x100(%esp)
		mov    %ebp,%eax
		mov    0x48(%esp),%edx
		mov    0x78(%esp),%ebx
		sub    %edx,%ebx
		mov    0x60(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128681
		inc    %eax
	jump_128681:
		mov    %eax,0x104(%esp)
		jmp    jump_12869a
	jump_12868a:
		xor    %eax,%eax
		mov    %eax,0x100(%esp)
		mov    %eax,0x104(%esp)
	jump_12869a:
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_1287e7
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_1286bf
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_1286cd
	jump_1286bf:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_1286cd:
		mov    0x5c(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1286e2
		inc    %eax
	jump_1286e2:
		mov    %eax,0x1c(%esp)
		mov    0x60(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1286fb
		inc    %eax
	jump_1286fb:
		mov    %eax,0x28(%esp)
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_128718
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_128726
	jump_128718:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_128726:
		mov    0x74(%esp),%eax
		sub    0x5c(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12873b
		inc    %eax
	jump_12873b:
		mov    %eax,0x18(%esp)
		mov    0x78(%esp),%eax
		sub    0x60(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128754
		inc    %eax
	jump_128754:
		mov    %eax,0x24(%esp)
		mov    0x4(%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x1c(%esp)
		mov    0x4(%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x28(%esp)
		mov    0x4(%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0xc(%esp)
		mov    0x8(%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x18(%esp)
		mov    0x8(%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x24(%esp)
		mov    0x8(%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x10(%esp)
		jmp    jump_128882
	jump_1287e7:
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_128800
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_12880e
	jump_128800:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_12880e:
		mov    0x74(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128823
		inc    %eax
	jump_128823:
		mov    %eax,0x1c(%esp)
		mov    0x78(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12883c
		inc    %eax
	jump_12883c:
		mov    %eax,0x28(%esp)
		mov    (%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x1c(%esp)
		mov    (%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x28(%esp)
		mov    (%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0xc(%esp)
	jump_128882:
		mov    0x44(%esp),%eax
		shl    $0x10,%eax
		add    0x100(%esp),%eax
		mov    %eax,0x15c(%esp)
		mov    0x48(%esp),%eax
		shl    $0x10,%eax
		add    0x104(%esp),%eax
		mov    %eax,0x160(%esp)
		mov    0x5c(%esp),%eax
		shl    $0x10,%eax
		add    0x100(%esp),%eax
		mov    %eax,0x168(%esp)
		mov    0x60(%esp),%eax
		shl    $0x10,%eax
		add    0x104(%esp),%eax
		mov    %eax,0x16c(%esp)
		mov    0x100(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0xf4(%esp)
		mov    %edx,0xf8(%esp)
		mov    0x104(%esp),%eax
		shl    $0x10,%eax
		mov    0x104(%esp),%edx
		sar    $0x10,%edx
		mov    0xf8(%esp),%al
		or     %al,%al
		jns    jump_12891a
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_12891a:
		mov    %eax,0xf8(%esp)
		mov    %edx,0xfc(%esp)
		mov    0x1c(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0x10c(%esp)
		mov    %edx,0x110(%esp)
		mov    0x28(%esp),%eax
		shl    $0x10,%eax
		mov    0x28(%esp),%edx
		sar    $0x10,%edx
		mov    0x110(%esp),%al
		or     %al,%al
		jns    jump_128963
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_128963:
		mov    %eax,0x110(%esp)
		mov    %edx,0x114(%esp)
		mov    0x15c(%esp),%ecx
		mov    0x160(%esp),%edx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %ecx,0x124(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x128(%esp)
		mov    %edx,0x12c(%esp)
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_128a3c
		mov    0x18(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0x118(%esp)
		mov    %edx,0x11c(%esp)
		mov    0x24(%esp),%eax
		shl    $0x10,%eax
		mov    0x24(%esp),%edx
		sar    $0x10,%edx
		mov    0x11c(%esp),%al
		or     %al,%al
		jns    jump_1289f3
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_1289f3:
		mov    %eax,0x11c(%esp)
		mov    %edx,0x120(%esp)
		mov    0x168(%esp),%ecx
		mov    0x16c(%esp),%edx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %ecx,0x130(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x134(%esp)
		mov    %edx,0x138(%esp)
	jump_128a3c:
		jmp    func_12998f


/*----------------------------------------------------------------*/
func_128a41:
/*----------------------------------------------------------------*/
		mov    0x50(%esp),%esi
		mov    0x7c(%esp),%edi
		or     %edi,%edi
		sub    $0x0,%esi
		add    $0x0,%esi
		mov    0x38(%esp),%eax
		sub    %eax,%esi
		mov    0x68(%esp),%edi
		sub    %eax,%edi
		mov    0x34(%esp),%eax
		mov    0x4c(%esp),%ebx
		sub    %eax,%ebx
		mov    0x64(%esp),%ecx
		sub    %eax,%ecx
		mov    %ecx,%eax
		imul   %esi,%ecx
		mov    0x7c(%esp),%ebp
		or     %ebp,%ebp
		js     jump_128a7e
		sub    %eax,%ecx
		sub    %eax,%ecx
	jump_128a7e:
		add    %eax,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		je     jump_128b38
		xor    %edx,%edx
		mov    $0x7fffffff,%eax
		idiv   %ebx
		mov    %eax,%ebp
		mov    0x34(%esp),%eax
		mov    0x64(%esp),%esi
		sub    %eax,%esi
		mov    0x4c(%esp),%edi
		sub    %eax,%edi
		mov    %ebp,%eax
		mov    0x40(%esp),%edx
		mov    0x70(%esp),%ebx
		sub    %edx,%ebx
		mov    0x58(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128acf
		inc    %eax
	jump_128acf:
		mov    %eax,0x108(%esp)
		mov    %ebp,%eax
		mov    0x44(%esp),%edx
		mov    0x74(%esp),%ebx
		sub    %edx,%ebx
		mov    0x5c(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128aff
		inc    %eax
	jump_128aff:
		mov    %eax,0x100(%esp)
		mov    %ebp,%eax
		mov    0x48(%esp),%edx
		mov    0x78(%esp),%ebx
		sub    %edx,%ebx
		mov    0x60(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128b2f
		inc    %eax
	jump_128b2f:
		mov    %eax,0x104(%esp)
		jmp    jump_128b4f
	jump_128b38:
		xor    %eax,%eax
		mov    %eax,0x108(%esp)
		mov    %eax,0x100(%esp)
		mov    %eax,0x104(%esp)
	jump_128b4f:
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_128cce
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_128b74
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_128b82
	jump_128b74:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_128b82:
		mov    0x58(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128b97
		inc    %eax
	jump_128b97:
		mov    %eax,0xc(%esp)
		mov    0x5c(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128bb0
		inc    %eax
	jump_128bb0:
		mov    %eax,0x1c(%esp)
		mov    0x60(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128bc9
		inc    %eax
	jump_128bc9:
		mov    %eax,0x28(%esp)
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_128be6
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_128bf4
	jump_128be6:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_128bf4:
		mov    0x70(%esp),%eax
		sub    0x58(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128c09
		inc    %eax
	jump_128c09:
		mov    %eax,0x10(%esp)
		mov    0x74(%esp),%eax
		sub    0x5c(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128c22
		inc    %eax
	jump_128c22:
		mov    %eax,0x18(%esp)
		mov    0x78(%esp),%eax
		sub    0x60(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128c3b
		inc    %eax
	jump_128c3b:
		mov    %eax,0x24(%esp)
		mov    0x4(%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x1c(%esp)
		mov    0x4(%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x28(%esp)
		mov    0x4(%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0xc(%esp)
		mov    0x8(%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x18(%esp)
		mov    0x8(%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x24(%esp)
		mov    0x8(%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x10(%esp)
		jmp    jump_128d82
	jump_128cce:
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_128ce7
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_128cf5
	jump_128ce7:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_128cf5:
		mov    0x70(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128d0a
		inc    %eax
	jump_128d0a:
		mov    %eax,0xc(%esp)
		mov    0x74(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128d23
		inc    %eax
	jump_128d23:
		mov    %eax,0x1c(%esp)
		mov    0x78(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_128d3c
		inc    %eax
	jump_128d3c:
		mov    %eax,0x28(%esp)
		mov    (%esp),%eax
		mov    0x100(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x1c(%esp)
		mov    (%esp),%eax
		mov    0x104(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0x28(%esp)
		mov    (%esp),%eax
		mov    0x108(%esp),%ebx
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		sub    %eax,0xc(%esp)
	jump_128d82:
		mov    0x40(%esp),%eax
		shl    $0x10,%eax
		add    0x108(%esp),%eax
		mov    %eax,0x158(%esp)
		mov    0x44(%esp),%eax
		shl    $0x10,%eax
		add    0x100(%esp),%eax
		mov    %eax,0x15c(%esp)
		mov    0x48(%esp),%eax
		shl    $0x10,%eax
		add    0x104(%esp),%eax
		mov    %eax,0x160(%esp)
		mov    0x58(%esp),%eax
		shl    $0x10,%eax
		add    0x108(%esp),%eax
		mov    %eax,0x164(%esp)
		mov    0x5c(%esp),%eax
		shl    $0x10,%eax
		add    0x100(%esp),%eax
		mov    %eax,0x168(%esp)
		mov    0x60(%esp),%eax
		shl    $0x10,%eax
		add    0x104(%esp),%eax
		mov    %eax,0x16c(%esp)
		mov    0x100(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0x109(%esp),%ax
		or     %ax,%ax
		jns    jump_128e24
		sub    $0xffff,%eax
		sbb    $0x0,%dl
	jump_128e24:
		mov    %eax,0xf4(%esp)
		mov    %edx,0xf8(%esp)
		mov    0x104(%esp),%eax
		shl    $0x10,%eax
		mov    0x104(%esp),%edx
		sar    $0x10,%edx
		mov    0xf8(%esp),%al
		or     %al,%al
		jns    jump_128e59
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_128e59:
		mov    %eax,0xf8(%esp)
		mov    %edx,0xfc(%esp)
		mov    0x1c(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0xd(%esp),%ax
		or     %ax,%ax
		jns    jump_128e85
		sub    $0x10000,%eax
		sbb    $0x0,%dl
	jump_128e85:
		mov    %eax,0x10c(%esp)
		mov    %edx,0x110(%esp)
		mov    0x28(%esp),%eax
		shl    $0x10,%eax
		mov    0x28(%esp),%edx
		sar    $0x10,%edx
		mov    0x110(%esp),%al
		or     %al,%al
		jns    jump_128eb4
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_128eb4:
		mov    %eax,0x110(%esp)
		mov    %edx,0x114(%esp)
		mov    0x158(%esp),%ebx
		mov    0x15c(%esp),%ecx
		mov    0x160(%esp),%edx
		mov    %bl,0x12c(%esp)
		shr    $0x8,%ebx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %bx,%cx
		mov    %ecx,0x124(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x128(%esp)
		mov    0x12c(%esp),%dl
		mov    %edx,0x12c(%esp)
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_128fd5
		mov    0x18(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0x11(%esp),%ax
		or     %ax,%ax
		jns    jump_128f42
		sub    $0x10000,%eax
		sbb    $0x0,%dl
	jump_128f42:
		mov    %eax,0x118(%esp)
		mov    %edx,0x11c(%esp)
		mov    0x24(%esp),%eax
		shl    $0x10,%eax
		mov    0x24(%esp),%edx
		sar    $0x10,%edx
		mov    0x11c(%esp),%al
		or     %al,%al
		jns    jump_128f71
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_128f71:
		mov    %eax,0x11c(%esp)
		mov    %edx,0x120(%esp)
		mov    0x164(%esp),%ebx
		mov    0x168(%esp),%ecx
		mov    0x16c(%esp),%edx
		mov    %bl,0x138(%esp)
		shr    $0x8,%ebx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %bx,%cx
		mov    %ecx,0x130(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x134(%esp)
		mov    0x138(%esp),%dl
		mov    %edx,0x138(%esp)
	jump_128fd5:
		jmp    func_12998f


/*----------------------------------------------------------------*/
func_128fda:
/*----------------------------------------------------------------*/
		mov    0x50(%esp),%esi
		mov    0x7c(%esp),%edi
		or     %edi,%edi
		sub    $0x0,%esi
		add    $0x0,%esi
		mov    0x38(%esp),%eax
		sub    %eax,%esi
		mov    0x68(%esp),%edi
		sub    %eax,%edi
		mov    0x34(%esp),%eax
		mov    0x4c(%esp),%ebx
		sub    %eax,%ebx
		mov    0x64(%esp),%ecx
		sub    %eax,%ecx
		mov    %ecx,%eax
		imul   %esi,%ecx
		mov    0x7c(%esp),%ebp
		or     %ebp,%ebp
		js     jump_129017
		sub    %eax,%ecx
		sub    %eax,%ecx
	jump_129017:
		add    %eax,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		je     jump_12906d
		xor    %edx,%edx
		mov    $0x7fffffff,%eax
		idiv   %ebx
		mov    %eax,%ebp
		mov    0x34(%esp),%eax
		mov    0x64(%esp),%esi
		sub    %eax,%esi
		mov    0x4c(%esp),%edi
		sub    %eax,%edi
		mov    %ebp,%eax
		mov    0x40(%esp),%edx
		mov    0x70(%esp),%ebx
		sub    %edx,%ebx
		mov    0x58(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129064
		inc    %eax
	jump_129064:
		mov    %eax,0x108(%esp)
		jmp    jump_129076
	jump_12906d:
		xor    %eax,%eax
		mov    %eax,0x108(%esp)
	jump_129076:
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_129104
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_12909b
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_1290a9
	jump_12909b:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_1290a9:
		mov    0x58(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1290be
		inc    %eax
	jump_1290be:
		mov    %eax,0xc(%esp)
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_1290db
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_1290e9
	jump_1290db:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_1290e9:
		mov    0x70(%esp),%eax
		sub    0x58(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1290fe
		inc    %eax
	jump_1290fe:
		mov    %eax,0x10(%esp)
		jmp    jump_129144
	jump_129104:
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_12911d
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_12912b
	jump_12911d:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_12912b:
		mov    0x70(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129140
		inc    %eax
	jump_129140:
		mov    %eax,0xc(%esp)
	jump_129144:
		mov    0x40(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x158(%esp)
		mov    0x58(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x164(%esp)
		jmp    func_12998f


/*----------------------------------------------------------------*/
func_129165:
/*----------------------------------------------------------------*/
		mov    0x50(%esp),%esi
		mov    0x7c(%esp),%edi
		or     %edi,%edi
		sub    $0x0,%esi
		add    $0x0,%esi
		mov    0x38(%esp),%eax
		sub    %eax,%esi
		mov    0x68(%esp),%edi
		sub    %eax,%edi
		mov    0x34(%esp),%eax
		mov    0x4c(%esp),%ebx
		sub    %eax,%ebx
		mov    0x64(%esp),%ecx
		sub    %eax,%ecx
		mov    %ecx,%eax
		imul   %esi,%ecx
		mov    0x7c(%esp),%ebp
		or     %ebp,%ebp
		js     jump_1291a2
		sub    %eax,%ecx
		sub    %eax,%ecx
	jump_1291a2:
		add    %eax,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		je     jump_129228
		xor    %edx,%edx
		mov    $0x7fffffff,%eax
		idiv   %ebx
		mov    %eax,%ebp
		mov    0x34(%esp),%eax
		mov    0x64(%esp),%esi
		sub    %eax,%esi
		mov    0x4c(%esp),%edi
		sub    %eax,%edi
		mov    %ebp,%eax
		mov    0x44(%esp),%edx
		mov    0x74(%esp),%ebx
		sub    %edx,%ebx
		mov    0x5c(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1291ef
		inc    %eax
	jump_1291ef:
		mov    %eax,0x100(%esp)
		mov    %ebp,%eax
		mov    0x48(%esp),%edx
		mov    0x78(%esp),%ebx
		sub    %edx,%ebx
		mov    0x60(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12921f
		inc    %eax
	jump_12921f:
		mov    %eax,0x104(%esp)
		jmp    jump_129238
	jump_129228:
		xor    %eax,%eax
		mov    %eax,0x100(%esp)
		mov    %eax,0x104(%esp)
	jump_129238:
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_1292f8
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_12925d
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_12926b
	jump_12925d:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_12926b:
		mov    0x5c(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129280
		inc    %eax
	jump_129280:
		mov    %eax,0x1c(%esp)
		mov    0x60(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129299
		inc    %eax
	jump_129299:
		mov    %eax,0x28(%esp)
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_1292b6
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_1292c4
	jump_1292b6:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_1292c4:
		mov    0x74(%esp),%eax
		sub    0x5c(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1292d9
		inc    %eax
	jump_1292d9:
		mov    %eax,0x18(%esp)
		mov    0x78(%esp),%eax
		sub    0x60(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1292f2
		inc    %eax
	jump_1292f2:
		mov    %eax,0x24(%esp)
		jmp    jump_129351
	jump_1292f8:
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_129311
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_12931f
	jump_129311:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_12931f:
		mov    0x74(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129334
		inc    %eax
	jump_129334:
		mov    %eax,0x1c(%esp)
		mov    0x78(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12934d
		inc    %eax
	jump_12934d:
		mov    %eax,0x28(%esp)
	jump_129351:
		mov    0x44(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x15c(%esp)
		mov    0x48(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x160(%esp)
		mov    0x5c(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x168(%esp)
		mov    0x60(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x16c(%esp)
		mov    0x100(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0xf4(%esp)
		mov    %edx,0xf8(%esp)
		mov    0x104(%esp),%eax
		shl    $0x10,%eax
		mov    0x104(%esp),%edx
		sar    $0x10,%edx
		mov    0xf8(%esp),%al
		or     %al,%al
		jns    jump_1293cd
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_1293cd:
		mov    %eax,0xf8(%esp)
		mov    %edx,0xfc(%esp)
		mov    0x1c(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0x10c(%esp)
		mov    %edx,0x110(%esp)
		mov    0x28(%esp),%eax
		shl    $0x10,%eax
		mov    0x28(%esp),%edx
		sar    $0x10,%edx
		mov    0x110(%esp),%al
		or     %al,%al
		jns    jump_129416
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_129416:
		mov    %eax,0x110(%esp)
		mov    %edx,0x114(%esp)
		mov    0x15c(%esp),%ecx
		mov    0x160(%esp),%edx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %ecx,0x124(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x128(%esp)
		mov    %edx,0x12c(%esp)
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_1294ef
		mov    0x18(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    %eax,0x118(%esp)
		mov    %edx,0x11c(%esp)
		mov    0x24(%esp),%eax
		shl    $0x10,%eax
		mov    0x24(%esp),%edx
		sar    $0x10,%edx
		mov    0x11c(%esp),%al
		or     %al,%al
		jns    jump_1294a6
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_1294a6:
		mov    %eax,0x11c(%esp)
		mov    %edx,0x120(%esp)
		mov    0x168(%esp),%ecx
		mov    0x16c(%esp),%edx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %ecx,0x130(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x134(%esp)
		mov    %edx,0x138(%esp)
	jump_1294ef:
		jmp    func_12998f


/*----------------------------------------------------------------*/
func_1294f4:
/*----------------------------------------------------------------*/
		mov    0x50(%esp),%esi
		mov    0x7c(%esp),%edi
		or     %edi,%edi
		sub    $0x0,%esi
		add    $0x0,%esi
		mov    0x38(%esp),%eax
		sub    %eax,%esi
		mov    0x68(%esp),%edi
		sub    %eax,%edi
		mov    0x34(%esp),%eax
		mov    0x4c(%esp),%ebx
		sub    %eax,%ebx
		mov    0x64(%esp),%ecx
		sub    %eax,%ecx
		mov    %ecx,%eax
		imul   %esi,%ecx
		mov    0x7c(%esp),%ebp
		or     %ebp,%ebp
		js     jump_129531
		sub    %eax,%ecx
		sub    %eax,%ecx
	jump_129531:
		add    %eax,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		je     jump_1295eb
		xor    %edx,%edx
		mov    $0x7fffffff,%eax
		idiv   %ebx
		mov    %eax,%ebp
		mov    0x34(%esp),%eax
		mov    0x64(%esp),%esi
		sub    %eax,%esi
		mov    0x4c(%esp),%edi
		sub    %eax,%edi
		mov    %ebp,%eax
		mov    0x40(%esp),%edx
		mov    0x70(%esp),%ebx
		sub    %edx,%ebx
		mov    0x58(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129582
		inc    %eax
	jump_129582:
		mov    %eax,0x108(%esp)
		mov    %ebp,%eax
		mov    0x44(%esp),%edx
		mov    0x74(%esp),%ebx
		sub    %edx,%ebx
		mov    0x5c(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1295b2
		inc    %eax
	jump_1295b2:
		mov    %eax,0x100(%esp)
		mov    %ebp,%eax
		mov    0x48(%esp),%edx
		mov    0x78(%esp),%ebx
		sub    %edx,%ebx
		mov    0x60(%esp),%ecx
		sub    %edx,%ecx
		imul   %esi,%ecx
		imul   %edi,%ebx
		sub    %ecx,%ebx
		imul   %ebx
		shl    %eax
		rcl    %edx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1295e2
		inc    %eax
	jump_1295e2:
		mov    %eax,0x104(%esp)
		jmp    jump_129602
	jump_1295eb:
		xor    %eax,%eax
		mov    %eax,0x108(%esp)
		mov    %eax,0x100(%esp)
		mov    %eax,0x104(%esp)
	jump_129602:
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     jump_1296f4
		mov    0x4c(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_129627
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_129635
	jump_129627:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_129635:
		mov    0x58(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12964a
		inc    %eax
	jump_12964a:
		mov    %eax,0xc(%esp)
		mov    0x5c(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129663
		inc    %eax
	jump_129663:
		mov    %eax,0x1c(%esp)
		mov    0x60(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_12967c
		inc    %eax
	jump_12967c:
		mov    %eax,0x28(%esp)
		mov    0x64(%esp),%ecx
		sub    0x4c(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_129699
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_1296a7
	jump_129699:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_1296a7:
		mov    0x70(%esp),%eax
		sub    0x58(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1296bc
		inc    %eax
	jump_1296bc:
		mov    %eax,0x10(%esp)
		mov    0x74(%esp),%eax
		sub    0x5c(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1296d5
		inc    %eax
	jump_1296d5:
		mov    %eax,0x18(%esp)
		mov    0x78(%esp),%eax
		sub    0x60(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_1296ee
		inc    %eax
	jump_1296ee:
		mov    %eax,0x24(%esp)
		jmp    jump_129766
	jump_1296f4:
		mov    0x64(%esp),%ecx
		sub    0x34(%esp),%ecx
		cmp    $0xff,%ecx
		jg     jump_12970d
		mov    data_12b410(,%ecx,4),%ebx
		jmp    jump_12971b
	jump_12970d:
		mov    $0x0,%edx
		mov    $0x7fffffff,%eax
		idiv   %ecx
		mov    %eax,%ebx
	jump_12971b:
		mov    0x70(%esp),%eax
		sub    0x40(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129730
		inc    %eax
	jump_129730:
		mov    %eax,0xc(%esp)
		mov    0x74(%esp),%eax
		sub    0x44(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129749
		inc    %eax
	jump_129749:
		mov    %eax,0x1c(%esp)
		mov    0x78(%esp),%eax
		sub    0x48(%esp),%eax
		shl    %eax
		imul   %ebx
		mov    %dx,%ax
		rol    $0x10,%eax
		jns    jump_129762
		inc    %eax
	jump_129762:
		mov    %eax,0x28(%esp)
	jump_129766:
		mov    0x40(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x158(%esp)
		mov    0x44(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x15c(%esp)
		mov    0x48(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x160(%esp)
		mov    0x58(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x164(%esp)
		mov    0x5c(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x168(%esp)
		mov    0x60(%esp),%eax
		shl    $0x10,%eax
		mov    %eax,0x16c(%esp)
		mov    0x100(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0x109(%esp),%ax
		or     %ax,%ax
		jns    jump_1297de
		sub    $0xffff,%eax
		sbb    $0x0,%dl
	jump_1297de:
		mov    %eax,0xf4(%esp)
		mov    %edx,0xf8(%esp)
		mov    0x104(%esp),%eax
		shl    $0x10,%eax
		mov    0x104(%esp),%edx
		sar    $0x10,%edx
		mov    0xf8(%esp),%al
		or     %al,%al
		jns    jump_129813
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_129813:
		mov    %eax,0xf8(%esp)
		mov    %edx,0xfc(%esp)
		mov    0x1c(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0xd(%esp),%ax
		or     %ax,%ax
		jns    jump_12983f
		sub    $0x10000,%eax
		sbb    $0x0,%dl
	jump_12983f:
		mov    %eax,0x10c(%esp)
		mov    %edx,0x110(%esp)
		mov    0x28(%esp),%eax
		shl    $0x10,%eax
		mov    0x28(%esp),%edx
		sar    $0x10,%edx
		mov    0x110(%esp),%al
		or     %al,%al
		jns    jump_12986e
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_12986e:
		mov    %eax,0x110(%esp)
		mov    %edx,0x114(%esp)
		mov    0x158(%esp),%ebx
		mov    0x15c(%esp),%ecx
		mov    0x160(%esp),%edx
		mov    %bl,0x12c(%esp)
		shr    $0x8,%ebx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %bx,%cx
		mov    %ecx,0x124(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x128(%esp)
		mov    0x12c(%esp),%dl
		mov    %edx,0x12c(%esp)
		mov    0x7c(%esp),%esi
		or     %esi,%esi
		js     func_12998f
		mov    0x18(%esp),%eax
		mov    %eax,%edx
		shl    $0x10,%eax
		sar    $0x10,%edx
		mov    0x11(%esp),%ax
		or     %ax,%ax
		jns    jump_1298fc
		sub    $0x10000,%eax
		sbb    $0x0,%dl
	jump_1298fc:
		mov    %eax,0x118(%esp)
		mov    %edx,0x11c(%esp)
		mov    0x24(%esp),%eax
		shl    $0x10,%eax
		mov    0x24(%esp),%edx
		sar    $0x10,%edx
		mov    0x11c(%esp),%al
		or     %al,%al
		jns    jump_12992b
		sub    $0x100,%eax
		sbb    $0x0,%dl
	jump_12992b:
		mov    %eax,0x11c(%esp)
		mov    %edx,0x120(%esp)
		mov    0x164(%esp),%ebx
		mov    0x168(%esp),%ecx
		mov    0x16c(%esp),%edx
		mov    %bl,0x138(%esp)
		shr    $0x8,%ebx
		mov    %ecx,%eax
		shl    $0x10,%ecx
		shr    $0x10,%eax
		mov    %bx,%cx
		mov    %ecx,0x130(%esp)
		mov    %edx,%ecx
		shl    $0x10,%ecx
		shl    $0x8,%edx
		shr    $0x18,%edx
		shl    $0x8,%edx
		mov    %al,%cl
		mov    %ecx,0x134(%esp)
		mov    0x138(%esp),%dl
		mov    %edx,0x138(%esp)


/*----------------------------------------------------------------*/
func_12998f:
/*----------------------------------------------------------------*/
		xor    %ecx,%ecx
		mov    EXPORT_SYMBOL(vec_mode),%cl
		jmp    *vtable_12b0c0(,%ecx,4)


/*----------------------------------------------------------------*/
func_12999e:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_1299a6:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_1299ae:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_1299b6:
/*----------------------------------------------------------------*/
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		nop
		mov    0x170(%esp),%eax
		or     %eax,%eax
		jne    jump_129f20
		mov    EXPORT_SYMBOL(vec_screen_width),%ecx
		mov    %ecx,0xac(%esp)
		mov    $0x2,%ecx
		mov    %ecx,0x30(%esp)
		mov    0xc(%esp),%eax
		mov    %eax,0x150(%esp)
		mov    0x10c(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x110(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x114(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_129a39
		mov    0x4(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    (%esp),%eax
		mov    %eax,0x88(%esp)
		jmp    jump_129a4e
	jump_129a39:
		mov    (%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x4(%esp),%eax
		mov    %eax,0x88(%esp)
	jump_129a4e:
		mov    0x124(%esp),%ecx
		mov    0x128(%esp),%edx
		mov    0x12c(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x34(%esp),%esi
		mov    EXPORT_SYMBOL(vec_screen_width),%edi
		imul   %esi,%edi
		add    EXPORT_SYMBOL(vec_screen),%edi
		mov    0x34(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jg     jump_129f10
		mov    0x4c(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_129a9d
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_129a9d:
		sub    0x34(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x38(%esp),%esi
		mov    %esi,0x13c(%esp)
		mov    0x3c(%esp),%eax
		mov    %eax,%ebp
		je     jump_129de8
		mov    0x34(%esp),%esi
		or     %esi,%esi
		js     jump_129d60
		mov    0x13c(%esp),%esi
		jmp    jump_129cf6


/*----------------------------------------------------------------*/
func_129ad7:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,(%edi)


/*----------------------------------------------------------------*/
func_129af0:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x1(%edi)


/*----------------------------------------------------------------*/
func_129b0a:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x2(%edi)


/*----------------------------------------------------------------*/
func_129b24:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x3(%edi)


/*----------------------------------------------------------------*/
func_129b3e:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x4(%edi)


/*----------------------------------------------------------------*/
func_129b58:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x5(%edi)


/*----------------------------------------------------------------*/
func_129b72:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x6(%edi)


/*----------------------------------------------------------------*/
func_129b8c:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x7(%edi)


/*----------------------------------------------------------------*/
func_129ba6:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x8(%edi)


/*----------------------------------------------------------------*/
func_129bc0:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x9(%edi)


/*----------------------------------------------------------------*/
func_129bda:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xa(%edi)


/*----------------------------------------------------------------*/
func_129bf4:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xb(%edi)


/*----------------------------------------------------------------*/
func_129c0e:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xc(%edi)


/*----------------------------------------------------------------*/
func_129c28:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xd(%edi)


/*----------------------------------------------------------------*/
func_129c42:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xe(%edi)


/*----------------------------------------------------------------*/
func_129c5c:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xf(%edi)
		add    $0x10,%edi
		subl   $0x10,0xdc(%esp)
		jg     func_129ad7
	jump_129c87:
		mov    0xb4(%esp),%eax
		mov    0xb8(%esp),%ebp
		mov    0xbc(%esp),%edi
		mov    0x13c(%esp),%esi
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    0xd0(%esp),%ecx
		mov    0xcc(%esp),%ebx
		mov    0xd8(%esp),%edx
		add    0x150(%esp),%bl
		adc    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_129de8
	jump_129cf6:
		mov    %eax,0xb4(%esp)
		mov    %ebp,0xb8(%esp)
		mov    %edi,0xbc(%esp)
		sar    $0x10,%eax
		mov    0xbc(%esp),%edi
		mov    %ecx,0xd0(%esp)
		mov    %ebx,0xcc(%esp)
		mov    %edx,0xd8(%esp)
		sar    $0x10,%ebp
		add    %eax,%edi
		sub    %eax,%ebp
		jle    jump_129c87
		mov    %ebp,%eax
		and    $0xf,%eax
		add    data_12b3d0(,%eax,4),%edi
		mov    %ebp,0xdc(%esp)
		mov    0xf4(%esp),%ebp
		mov    0xf8(%esp),%esi
		jmp    *vtable_12b2d0(,%eax,4)
	jump_129d60:
		add    0x150(%esp),%bl
		adc    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		mov    %eax,0xb4(%esp)
		sar    $0x10,%eax
		sub    %eax,0x13c(%esp)
		mov    0xb4(%esp),%eax
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    %eax,0xb4(%esp)
		sar    $0x10,%eax
		add    %eax,0x13c(%esp)
		mov    0xb4(%esp),%eax
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_129de0
		inc    %esi
		js     jump_129d60
		mov    0x13c(%esp),%esi
		jmp    jump_129cf6
	jump_129de0:
		mov    0x13c(%esp),%esi
		nop
	jump_129de8:
		decl   0x30(%esp)
		je     jump_129f10
		mov    %eax,0xb4(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_129eb0
		mov    0x8(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x10(%esp),%eax
		mov    %eax,0x150(%esp)
		mov    0x118(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x11c(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x120(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x130(%esp),%ecx
		mov    0x134(%esp),%edx
		mov    0x138(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_129e71
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_129e71:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x50(%esp),%eax
		mov    %eax,0x13c(%esp)
		mov    0x54(%esp),%eax
		jle    jump_129f10
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_129d60
		mov    0x50(%esp),%esi
		jmp    jump_129cf6
	jump_129eb0:
		mov    0x8(%esp),%ebp
		mov    %ebp,0x88(%esp)
		mov    0x54(%esp),%ebp
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_129ed0
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_129ed0:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0xb4(%esp),%eax
		jle    jump_129f10
		mov    %esi,0x13c(%esp)
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_129d60
		mov    0x13c(%esp),%esi
		jmp    jump_129cf6
	jump_129f10:
		add    $0x190,%esp
		popa
		ret
	jump_129f20:
		mov    EXPORT_SYMBOL(vec_screen_width),%ecx
		mov    %ecx,0xac(%esp)
		mov    $0x2,%ecx
		mov    %ecx,0x30(%esp)
		mov    0xc(%esp),%eax
		mov    %eax,0x150(%esp)
		mov    0x10c(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x110(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x114(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_129f8a
		mov    0x4(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    (%esp),%eax
		mov    %eax,0x88(%esp)
		jmp    jump_129f9f
	jump_129f8a:
		mov    (%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x4(%esp),%eax
		mov    %eax,0x88(%esp)
	jump_129f9f:
		mov    0x124(%esp),%ecx
		mov    0x128(%esp),%edx
		mov    0x12c(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x34(%esp),%esi
		mov    EXPORT_SYMBOL(vec_screen_width),%edi
		imul   %esi,%edi
		add    EXPORT_SYMBOL(vec_screen),%edi
		mov    0x34(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jg     jump_12a590
		mov    0x4c(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_129fee
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_129fee:
		sub    0x34(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x38(%esp),%esi
		mov    %esi,0x13c(%esp)
		mov    0x3c(%esp),%eax
		mov    %eax,%ebp
		je     jump_12a468
		mov    0x34(%esp),%esi
		or     %esi,%esi
		js     jump_12a3e0
		mov    0x13c(%esp),%esi
		jmp    jump_12a266


/*----------------------------------------------------------------*/
func_12a028:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,(%edi)


/*----------------------------------------------------------------*/
func_12a041:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x1(%edi)


/*----------------------------------------------------------------*/
func_12a05b:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x2(%edi)


/*----------------------------------------------------------------*/
func_12a075:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x3(%edi)


/*----------------------------------------------------------------*/
func_12a08f:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x4(%edi)


/*----------------------------------------------------------------*/
func_12a0a9:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x5(%edi)


/*----------------------------------------------------------------*/
func_12a0c3:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x6(%edi)


/*----------------------------------------------------------------*/
func_12a0dd:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x7(%edi)


/*----------------------------------------------------------------*/
func_12a0f7:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x8(%edi)


/*----------------------------------------------------------------*/
func_12a111:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0x9(%edi)


/*----------------------------------------------------------------*/
func_12a12b:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xa(%edi)


/*----------------------------------------------------------------*/
func_12a145:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xb(%edi)


/*----------------------------------------------------------------*/
func_12a15f:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xc(%edi)


/*----------------------------------------------------------------*/
func_12a179:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xd(%edi)


/*----------------------------------------------------------------*/
func_12a193:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xe(%edi)


/*----------------------------------------------------------------*/
func_12a1ad:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		mov    %ch,%ah
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		mov    EXPORT_SYMBOL(sprites)(%eax),%al
		adc    0xfc(%esp),%bh
		mov    %al,0xf(%edi)
		add    $0x10,%edi
		subl   $0x10,0xdc(%esp)
		jg     func_12a028
	jump_12a1d8:
		mov    0xb4(%esp),%eax
		mov    0xb8(%esp),%ebp
		mov    0xbc(%esp),%edi
		mov    0x13c(%esp),%esi
		sar    $0x10,%eax
		sub    %eax,%esi
		mov    0xb4(%esp),%eax
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    %eax,0xb4(%esp)
		sar    $0x10,%eax
		add    %eax,%esi
		mov    0xb4(%esp),%eax
		mov    0xd0(%esp),%ecx
		mov    0xcc(%esp),%ebx
		mov    0xd8(%esp),%edx
		add    0x150(%esp),%bl
		adc    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12a468
	jump_12a266:
		mov    %eax,0xb4(%esp)
		mov    %ebp,0xb8(%esp)
		mov    %edi,0xbc(%esp)
		sar    $0x10,%eax
		js     jump_12a360
		cmp    %esi,%eax
		jg     jump_12a300
		jl     jump_12a330
	jump_12a28e:
		mov    %esi,0x13c(%esp)
		mov    0xbc(%esp),%edi
		mov    %ecx,0xd0(%esp)
		mov    %ebx,0xcc(%esp)
		mov    %edx,0xd8(%esp)
		sar    $0x10,%ebp
		cmp    EXPORT_SYMBOL(vec_window_width),%ebp
		jg     jump_12a3d0
	jump_12a2c0:
		add    %esi,%edi
		sub    %esi,%ebp
		jle    jump_12a1d8
		mov    %ebp,%eax
		and    $0xf,%eax
		add    data_12b3d0(,%eax,4),%edi
		mov    %ebp,0xdc(%esp)
		mov    0xf4(%esp),%ebp
		mov    0xf8(%esp),%esi
		jmp    *vtable_12b310(,%eax,4)
	jump_12a300:
		add    0x108(%esp),%bl
		adc    0xf4(%esp),%ecx
		adc    0xf8(%esp),%edx
		adc    0xfc(%esp),%bh
		inc    %esi
		cmp    %esi,%eax
		jle    jump_12a28e
		jmp    jump_12a300
	jump_12a330:
		sub    0x108(%esp),%bl
		sbb    0xf4(%esp),%ecx
		sbb    0xf8(%esp),%edx
		sbb    0xfc(%esp),%bh
		dec    %esi
		cmp    %esi,%eax
		jge    jump_12a28e
		jmp    jump_12a330
	jump_12a360:
		or     %esi,%esi
		je     jump_12a28e
		js     jump_12a370
		jmp    jump_12a3a0
	jump_12a370:
		add    0x108(%esp),%bl
		adc    0xf4(%esp),%ecx
		adc    0xf8(%esp),%edx
		adc    0xfc(%esp),%bh
		inc    %esi
		je     jump_12a28e
		jmp    jump_12a370
	jump_12a3a0:
		sub    0x108(%esp),%bl
		sbb    0xf4(%esp),%ecx
		sbb    0xf8(%esp),%edx
		sbb    0xfc(%esp),%bh
		dec    %esi
		je     jump_12a28e
		jmp    jump_12a3a0
	jump_12a3d0:
		mov    EXPORT_SYMBOL(vec_window_width),%ebp
		jmp    jump_12a2c0
	jump_12a3e0:
		add    0x150(%esp),%bl
		adc    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		mov    %eax,0xb4(%esp)
		sar    $0x10,%eax
		sub    %eax,0x13c(%esp)
		mov    0xb4(%esp),%eax
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    %eax,0xb4(%esp)
		sar    $0x10,%eax
		add    %eax,0x13c(%esp)
		mov    0xb4(%esp),%eax
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12a460
		inc    %esi
		js     jump_12a3e0
		mov    0x13c(%esp),%esi
		jmp    jump_12a266
	jump_12a460:
		mov    0x13c(%esp),%esi
		nop
	jump_12a468:
		decl   0x30(%esp)
		je     jump_12a590
		mov    %eax,0xb4(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_12a530
		mov    0x8(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x10(%esp),%eax
		mov    %eax,0x150(%esp)
		mov    0x118(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x11c(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x120(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x130(%esp),%ecx
		mov    0x134(%esp),%edx
		mov    0x138(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12a4f1
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12a4f1:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x50(%esp),%eax
		mov    %eax,0x13c(%esp)
		mov    0x54(%esp),%eax
		jle    jump_12a590
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12a3e0
		mov    0x50(%esp),%esi
		jmp    jump_12a266
	jump_12a530:
		mov    0x8(%esp),%ebp
		mov    %ebp,0x88(%esp)
		mov    0x54(%esp),%ebp
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12a550
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12a550:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0xb4(%esp),%eax
		jle    jump_12a590
		mov    %esi,0x13c(%esp)
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12a3e0
		mov    0x13c(%esp),%esi
		jmp    jump_12a266
	jump_12a590:
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a598:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5a0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5a8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5b0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5b8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5c0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5c8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5d0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5d8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5e0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5e8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5f0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a5f8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a600:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a608:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a610:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12a618:
/*----------------------------------------------------------------*/
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		mov    %eax,%eax
		mov    0x170(%esp),%eax
		or     %eax,%eax
		jne    jump_12ab00
		mov    EXPORT_SYMBOL(vec_screen_width),%ecx
		mov    %ecx,0xac(%esp)
		mov    $0x2,%ecx
		mov    %ecx,0x30(%esp)
		mov    0x10c(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x110(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x114(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_12a68e
		mov    0x4(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    (%esp),%eax
		mov    %eax,0x88(%esp)
		jmp    jump_12a6a3
	jump_12a68e:
		mov    (%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x4(%esp),%eax
		mov    %eax,0x88(%esp)
	jump_12a6a3:
		mov    0x124(%esp),%ecx
		mov    0x128(%esp),%edx
		mov    0x12c(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x34(%esp),%esi
		mov    EXPORT_SYMBOL(vec_screen_width),%edi
		imul   %esi,%edi
		add    EXPORT_SYMBOL(vec_screen),%edi
		mov    0x34(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jg     jump_12aaf0
		mov    0x4c(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12a6f2
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12a6f2:
		sub    0x34(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x38(%esp),%esi
		mov    %esi,0x13c(%esp)
		mov    0x3c(%esp),%eax
		mov    %eax,%ebp
		je     jump_12a9d8
		mov    0x34(%esp),%esi
		or     %esi,%esi
		js     jump_12a980
		mov    0x13c(%esp),%esi
		jmp    jump_12a8c4


/*----------------------------------------------------------------*/
func_12a72c:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,(%edi)


/*----------------------------------------------------------------*/
func_12a73d:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x1(%edi)


/*----------------------------------------------------------------*/
func_12a74f:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x2(%edi)


/*----------------------------------------------------------------*/
func_12a761:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x3(%edi)


/*----------------------------------------------------------------*/
func_12a773:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x4(%edi)


/*----------------------------------------------------------------*/
func_12a785:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x5(%edi)


/*----------------------------------------------------------------*/
func_12a797:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x6(%edi)


/*----------------------------------------------------------------*/
func_12a7a9:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x7(%edi)


/*----------------------------------------------------------------*/
func_12a7bb:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x8(%edi)


/*----------------------------------------------------------------*/
func_12a7cd:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x9(%edi)


/*----------------------------------------------------------------*/
func_12a7df:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xa(%edi)


/*----------------------------------------------------------------*/
func_12a7f1:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xb(%edi)


/*----------------------------------------------------------------*/
func_12a803:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xc(%edi)


/*----------------------------------------------------------------*/
func_12a815:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xd(%edi)


/*----------------------------------------------------------------*/
func_12a827:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xe(%edi)


/*----------------------------------------------------------------*/
func_12a839:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xf(%edi)
		add    $0x10,%edi
		subl   $0x10,0xdc(%esp)
		jg     func_12a72c
	jump_12a85c:
		mov    0xb4(%esp),%eax
		mov    0xb8(%esp),%ebp
		mov    0xbc(%esp),%edi
		mov    0x13c(%esp),%esi
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    0xd0(%esp),%ecx
		mov    0xcc(%esp),%ebx
		mov    0xd8(%esp),%edx
		add    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12a9d8
	jump_12a8c4:
		mov    %eax,0xb4(%esp)
		mov    %ebp,0xb8(%esp)
		mov    %edi,0xbc(%esp)
		sar    $0x10,%eax
		cmp    %esi,%eax
		jg     jump_12a940
		jl     jump_12a960
	jump_12a8e6:
		mov    %esi,0x13c(%esp)
		mov    0xbc(%esp),%edi
		mov    %ecx,0xd0(%esp)
		mov    %ebx,0xcc(%esp)
		mov    %edx,0xd8(%esp)
		sar    $0x10,%ebp
		add    %esi,%edi
		sub    %esi,%ebp
		jle    jump_12a85c
		mov    %ebp,%eax
		and    $0xf,%eax
		add    data_12b3d0(,%eax,4),%edi
		mov    %ebp,0xdc(%esp)
		mov    0xf4(%esp),%ebp
		mov    0xf8(%esp),%esi
		jmp    *vtable_12b350(,%eax,4)
	jump_12a940:
		add    0xf4(%esp),%ecx
		adc    0xf8(%esp),%edx
		adc    0xfc(%esp),%bh
		inc    %esi
		cmp    %esi,%eax
		jle    jump_12a8e6
		jmp    jump_12a940
	jump_12a960:
		sub    0xf4(%esp),%ecx
		sbb    0xf8(%esp),%edx
		sbb    0xfc(%esp),%bh
		dec    %esi
		cmp    %esi,%eax
		jge    jump_12a8e6
		jmp    jump_12a960
	jump_12a980:
		add    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12a9d0
		inc    %esi
		js     jump_12a980
		mov    0x13c(%esp),%esi
		jmp    jump_12a8c4
	jump_12a9d0:
		mov    0x13c(%esp),%esi
		nop
	jump_12a9d8:
		decl   0x30(%esp)
		je     jump_12aaf0
		mov    %eax,0xb4(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_12aa90
		mov    0x8(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x118(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x11c(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x120(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x130(%esp),%ecx
		mov    0x134(%esp),%edx
		mov    0x138(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12aa56
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12aa56:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x50(%esp),%eax
		mov    %eax,0x13c(%esp)
		mov    0x54(%esp),%eax
		jle    jump_12aaf0
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12a980
		mov    0x50(%esp),%esi
		jmp    jump_12a8c4
	jump_12aa90:
		mov    0x8(%esp),%ebp
		mov    %ebp,0x88(%esp)
		mov    0x54(%esp),%ebp
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12aab0
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12aab0:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0xb4(%esp),%eax
		jle    jump_12aaf0
		mov    %esi,0x13c(%esp)
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12a980
		mov    0x13c(%esp),%esi
		jmp    jump_12a8c4
	jump_12aaf0:
		add    $0x190,%esp
		popa
		ret
	jump_12ab00:
		mov    EXPORT_SYMBOL(vec_screen_width),%ecx
		mov    %ecx,0xac(%esp)
		mov    $0x2,%ecx
		mov    %ecx,0x30(%esp)
		mov    0x10c(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x110(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x114(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_12ab5f
		mov    0x4(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    (%esp),%eax
		mov    %eax,0x88(%esp)
		jmp    jump_12ab74
	jump_12ab5f:
		mov    (%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x4(%esp),%eax
		mov    %eax,0x88(%esp)
	jump_12ab74:
		mov    0x124(%esp),%ecx
		mov    0x128(%esp),%edx
		mov    0x12c(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x34(%esp),%esi
		mov    EXPORT_SYMBOL(vec_screen_width),%edi
		imul   %esi,%edi
		add    EXPORT_SYMBOL(vec_screen),%edi
		mov    0x34(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jg     jump_12b040
		mov    0x4c(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12abc3
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12abc3:
		sub    0x34(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x38(%esp),%esi
		mov    %esi,0x13c(%esp)
		mov    0x3c(%esp),%eax
		mov    %eax,%ebp
		je     jump_12af28
		mov    0x34(%esp),%esi
		or     %esi,%esi
		js     jump_12aed0
		mov    0x13c(%esp),%esi
		jmp    jump_12ad95


/*----------------------------------------------------------------*/
func_12abfd:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,(%edi)


/*----------------------------------------------------------------*/
func_12ac0e:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x1(%edi)


/*----------------------------------------------------------------*/
func_12ac20:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x2(%edi)


/*----------------------------------------------------------------*/
func_12ac32:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x3(%edi)


/*----------------------------------------------------------------*/
func_12ac44:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x4(%edi)


/*----------------------------------------------------------------*/
func_12ac56:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x5(%edi)


/*----------------------------------------------------------------*/
func_12ac68:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x6(%edi)


/*----------------------------------------------------------------*/
func_12ac7a:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x7(%edi)


/*----------------------------------------------------------------*/
func_12ac8c:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x8(%edi)


/*----------------------------------------------------------------*/
func_12ac9e:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0x9(%edi)


/*----------------------------------------------------------------*/
func_12acb0:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xa(%edi)


/*----------------------------------------------------------------*/
func_12acc2:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xb(%edi)


/*----------------------------------------------------------------*/
func_12acd4:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xc(%edi)


/*----------------------------------------------------------------*/
func_12ace6:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xd(%edi)


/*----------------------------------------------------------------*/
func_12acf8:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xe(%edi)


/*----------------------------------------------------------------*/
func_12ad0a:
/*----------------------------------------------------------------*/
		mov    %dl,%bl
		add    %ebp,%ecx
		mov    (%ebx),%al
		adc    %esi,%edx
		adc    0xfc(%esp),%bh
		mov    %al,0xf(%edi)
		add    $0x10,%edi
		subl   $0x10,0xdc(%esp)
		jg     func_12abfd
	jump_12ad2d:
		mov    0xb4(%esp),%eax
		mov    0xb8(%esp),%ebp
		mov    0xbc(%esp),%edi
		mov    0x13c(%esp),%esi
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		mov    0xd0(%esp),%ecx
		mov    0xcc(%esp),%ebx
		mov    0xd8(%esp),%edx
		add    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12af28
	jump_12ad95:
		mov    %eax,0xb4(%esp)
		mov    %ebp,0xb8(%esp)
		mov    %edi,0xbc(%esp)
		sar    $0x10,%eax
		js     jump_12ae70
		cmp    %esi,%eax
		jg     jump_12ae30
		jl     jump_12ae50
	jump_12adbd:
		mov    %esi,0x13c(%esp)
		mov    0xbc(%esp),%edi
		mov    %ecx,0xd0(%esp)
		mov    %ebx,0xcc(%esp)
		mov    %edx,0xd8(%esp)
		sar    $0x10,%ebp
		cmp    EXPORT_SYMBOL(vec_window_width),%ebp
		jg     jump_12aec0
	jump_12adef:
		add    %esi,%edi
		sub    %esi,%ebp
		jle    jump_12ad2d
		mov    %ebp,%eax
		and    $0xf,%eax
		add    data_12b3d0(,%eax,4),%edi
		mov    %ebp,0xdc(%esp)
		mov    0xf4(%esp),%ebp
		mov    0xf8(%esp),%esi
		jmp    *vtable_12b390(,%eax,4)
	jump_12ae30:
		add    0xf4(%esp),%ecx
		adc    0xf8(%esp),%edx
		adc    0xfc(%esp),%bh
		inc    %esi
		cmp    %esi,%eax
		jle    jump_12adbd
		jmp    jump_12ae30
	jump_12ae50:
		sub    0xf4(%esp),%ecx
		sbb    0xf8(%esp),%edx
		sbb    0xfc(%esp),%bh
		dec    %esi
		cmp    %esi,%eax
		jge    jump_12adbd
		jmp    jump_12ae50
	jump_12ae70:
		or     %esi,%esi
		je     jump_12adbd
		js     jump_12ae80
		jmp    jump_12aea0
	jump_12ae80:
		add    0xf4(%esp),%ecx
		adc    0xf8(%esp),%edx
		adc    0xfc(%esp),%bh
		inc    %esi
		je     jump_12adbd
		jmp    jump_12ae80
	jump_12aea0:
		sub    0xf4(%esp),%ecx
		sbb    0xf8(%esp),%edx
		sbb    0xfc(%esp),%bh
		dec    %esi
		je     jump_12adbd
		jmp    jump_12aea0
	jump_12aec0:
		mov    EXPORT_SYMBOL(vec_window_width),%ebp
		jmp    jump_12adef
	jump_12aed0:
		add    0xe4(%esp),%ecx
		adc    0xec(%esp),%edx
		adc    0xe8(%esp),%bh
		add    0x84(%esp),%eax
		add    0x88(%esp),%ebp
		add    0xac(%esp),%edi
		decl   0xf0(%esp)
		je     jump_12af20
		inc    %esi
		js     jump_12aed0
		mov    0x13c(%esp),%esi
		jmp    jump_12ad95
	jump_12af20:
		mov    0x13c(%esp),%esi
		nop
	jump_12af28:
		decl   0x30(%esp)
		je     jump_12b040
		mov    %eax,0xb4(%esp)
		mov    0x7c(%esp),%eax
		or     %eax,%eax
		js     jump_12afe0
		mov    0x8(%esp),%eax
		mov    %eax,0x84(%esp)
		mov    0x118(%esp),%eax
		mov    %eax,0xe4(%esp)
		mov    0x11c(%esp),%eax
		mov    %eax,0xec(%esp)
		mov    0x120(%esp),%eax
		mov    %eax,0xe8(%esp)
		mov    0x130(%esp),%ecx
		mov    0x134(%esp),%edx
		mov    0x138(%esp),%ebx
		add    EXPORT_SYMBOL(vec_map),%ebx
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12afa6
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12afa6:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0x50(%esp),%eax
		mov    %eax,0x13c(%esp)
		mov    0x54(%esp),%eax
		jle    jump_12b040
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12aed0
		mov    0x50(%esp),%esi
		jmp    jump_12ad95
	jump_12afe0:
		mov    0x8(%esp),%ebp
		mov    %ebp,0x88(%esp)
		mov    0x54(%esp),%ebp
		mov    0x64(%esp),%eax
		cmp    EXPORT_SYMBOL(vec_window_height),%eax
		jle    jump_12b000
		mov    EXPORT_SYMBOL(vec_window_height),%eax
	jump_12b000:
		sub    0x4c(%esp),%eax
		mov    %eax,0xf0(%esp)
		mov    0xb4(%esp),%eax
		jle    jump_12b040
		mov    %esi,0x13c(%esp)
		mov    0x4c(%esp),%esi
		or     %esi,%esi
		js     jump_12aed0
		mov    0x13c(%esp),%esi
		jmp    jump_12ad95
	jump_12b040:
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b048:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b050:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b058:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b060:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b068:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b070:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b078:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b080:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b088:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b090:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b098:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b0a0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b0a8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b0b0:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret


/*----------------------------------------------------------------*/
func_12b0b8:
/*----------------------------------------------------------------*/
		add    $0x190,%esp
		popa
		ret

vtable_12b0c0:
		.long   func_12999e
		.long   func_12b0b8
		.long   func_1299ae
		.long   func_12a598
		.long   func_1299a6
		.long   func_1299b6
		.long   func_12a5a0
		.long   func_12b0b8
		.long   func_12b0b8
		.long   func_12b0b8
		.long   func_12b0b8
		.long   func_12b0b8
		.long   func_12a5a8
		.long   func_12a5b0
		.long   func_12a5b8
		.long   func_12a5c0
		.long   func_12a5c8
		.long   func_12a5d0
		.long   func_12a5d8
		.long   func_12a5e0
		.long   func_12a5e8
		.long   func_12a5f0
		.long   func_12a5f8
		.long   func_12a600
		.long   func_12a608
		.long   func_12a610
		.long   func_12b0b8
		.long   func_12a618
		.long   func_12b048
		.long   func_12b050
		.long   func_12b058
		.long   func_12b060
		.long   func_12b068
		.long   func_12b070
		.long   func_12b078
		.long   func_12b080
		.long   func_12b088
		.long   func_12b090
		.long   func_12b098
		.long   func_12b0a0
		.long   func_12b0a8
		.long   func_12b0b8
		.long   func_12b0b0

vtable_12b16c:
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_1283a3
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_1283a3
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396
		.long   func_128396

vtable_12b220:
		.long   func_12998f
		.long   func_12998f
		.long   func_129165
		.long   func_129165
		.long   func_128fda
		.long   func_1294f4
		.long   func_1294f4
		.long   func_12998f
		.long   func_12998f
		.long   func_12998f
		.long   func_12998f
		.long   func_12998f
		.long   func_129165
		.long   func_129165
		.long   func_12998f
		.long   func_12998f
		.long   func_128fda
		.long   func_128fda
		.long   func_129165
		.long   func_129165
		.long   func_1294f4
		.long   func_1294f4
		.long   func_129165
		.long   func_129165
		.long   func_1294f4
		.long   func_1294f4
		.long   func_12998f
		.long   func_1285c7
		.long   func_128a41
		.long   func_1285c7
		.long   func_128a41
		.long   func_1285c7
		.long   func_1285c7
		.long   func_1285c7
		.long   func_1285c7
		.long   func_128a41
		.long   func_128a41
		.long   func_1285c7
		.long   func_1285c7
		.long   func_128a41
		.long   func_128a41
		.long   func_12998f
		.long   func_12998f

vtable_12b2d0:
		.long   func_129ad7
		.long   func_129c5c
		.long   func_129c42
		.long   func_129c28
		.long   func_129c0e
		.long   func_129bf4
		.long   func_129bda
		.long   func_129bc0
		.long   func_129ba6
		.long   func_129b8c
		.long   func_129b72
		.long   func_129b58
		.long   func_129b3e
		.long   func_129b24
		.long   func_129b0a
		.long   func_129af0

vtable_12b310:
		.long   func_12a028
		.long   func_12a1ad
		.long   func_12a193
		.long   func_12a179
		.long   func_12a15f
		.long   func_12a145
		.long   func_12a12b
		.long   func_12a111
		.long   func_12a0f7
		.long   func_12a0dd
		.long   func_12a0c3
		.long   func_12a0a9
		.long   func_12a08f
		.long   func_12a075
		.long   func_12a05b
		.long   func_12a041

vtable_12b350:
		.long   func_12a72c
		.long   func_12a839
		.long   func_12a827
		.long   func_12a815
		.long   func_12a803
		.long   func_12a7f1
		.long   func_12a7df
		.long   func_12a7cd
		.long   func_12a7bb
		.long   func_12a7a9
		.long   func_12a797
		.long   func_12a785
		.long   func_12a773
		.long   func_12a761
		.long   func_12a74f
		.long   func_12a73d

vtable_12b390:
		.long   func_12abfd
		.long   func_12ad0a
		.long   func_12acf8
		.long   func_12ace6
		.long   func_12acd4
		.long   func_12acc2
		.long   func_12acb0
		.long   func_12ac9e
		.long   func_12ac8c
		.long   func_12ac7a
		.long   func_12ac68
		.long   func_12ac56
		.long   func_12ac44
		.long   func_12ac32
		.long   func_12ac20
		.long   func_12ac0e

.section .rodata
data_12b3d0:
		.fill   0x4
		.ascii  "\xf1\xff\xff\xff\xf2\xff\xff\xff"
		.ascii  "\xf3\xff\xff\xff\xf4\xff\xff\xff"
		.ascii  "\xf5\xff\xff\xff\xf6\xff\xff\xff"
		.ascii  "\xf7\xff\xff\xff\xf8\xff\xff\xff"
		.ascii  "\xf9\xff\xff\xff\xfa\xff\xff\xff"
		.ascii  "\xfb\xff\xff\xff\xfc\xff\xff\xff"
		.ascii  "\xfd\xff\xff\xff\xfe\xff\xff\xff"
		.ascii  "\xff\xff\xff\xff"
data_12b410:
		.fill   0x4
		.ascii  "\xff\xff\xff\x7f\xff\xff\xff\x3f"
		.ascii  "\xaa\xaa\xaa\x2a\xff\xff\xff\x1f"
		.ascii  "\x99\x99\x99\x19\x55\x55\x55\x15"
		.ascii  "\x92\x24\x49\x12\xff\xff\xff\x0f"
		.ascii  "\x8e\xe3\x38\x0e\xcc\xcc\xcc\x0c"
		.ascii  "\xba\xe8\xa2\x0b\xaa\xaa\xaa\x0a"
		.ascii  "\x89\x9d\xd8\x09\x49\x92\x24\x09"
		.ascii  "\x88\x88\x88\x08\xff\xff\xff\x07"
		.ascii  "\x87\x87\x87\x07\xc7\x71\x1c\x07"
		.ascii  "\xaf\xa1\xbc\x06\x66\x66\x66\x06"
		.ascii  "\x86\x61\x18\x06\x5d\x74\xd1\x05"
		.ascii  "\x16\xb2\x90\x05\x55\x55\x55\x05"
		.ascii  "\x51\xb8\x1e\x05\xc4\x4e\xec\x04"
		.ascii  "\x2f\xa1\xbd\x04\x24\x49\x92\x04"
		.ascii  "\x58\xee\x69\x04\x44\x44\x44\x04"
		.ascii  "\x42\x08\x21\x04\xff\xff\xff\x03"
		.ascii  "\x3e\xf8\xe0\x03\xc3\xc3\xc3\x03"
		.ascii  "\x83\x3a\xa8\x03\xe3\x38\x8e\x03"
		.ascii  "\x22\x9f\x75\x03\xd7\x50\x5e\x03"
		.ascii  "\x83\x34\x48\x03\x33\x33\x33\x03"
		.ascii  "\x31\x38\x1f\x03\xc3\x30\x0c\x03"
		.ascii  "\xe8\x0b\xfa\x02\x2e\xba\xe8\x02"
		.ascii  "\x82\x2d\xd8\x02\x0b\x59\xc8\x02"
		.ascii  "\x05\x31\xb9\x02\xaa\xaa\xaa\x02"
		.ascii  "\x14\xbc\x9c\x02\x28\x5c\x8f\x02"
		.ascii  "\x82\x82\x82\x02\x62\x27\x76\x02"
		.ascii  "\x9f\x43\x6a\x02\x97\xd0\x5e\x02"
		.ascii  "\x25\xc8\x53\x02\x92\x24\x49\x02"
		.ascii  "\x8f\xe0\x3e\x02\x2c\xf7\x34\x02"
		.ascii  "\xcb\x63\x2b\x02\x22\x22\x22\x02"
		.ascii  "\x29\x2e\x19\x02\x21\x84\x10\x02"
		.ascii  "\x82\x20\x08\x02\xff\xff\xff\x01"
		.ascii  "\x81\x1f\xf8\x01\x1f\x7c\xf0\x01"
		.ascii  "\x1a\x13\xe9\x01\xe1\xe1\xe1\x01"
		.ascii  "\x07\xe6\xda\x01\x41\x1d\xd4\x01"
		.ascii  "\x68\x85\xcd\x01\x71\x1c\xc7\x01"
		.ascii  "\x70\xe0\xc0\x01\x91\xcf\xba\x01"
		.ascii  "\x1b\xe8\xb4\x01\x6b\x28\xaf\x01"
		.ascii  "\xf6\x8e\xa9\x01\x41\x1a\xa4\x01"
		.ascii  "\xe9\xc8\x9e\x01\x99\x99\x99\x01"
		.ascii  "\x0f\x8b\x94\x01\x18\x9c\x8f\x01"
		.ascii  "\x90\xcb\x8a\x01\x61\x18\x86\x01"
		.ascii  "\x81\x81\x81\x01\xf4\x05\x7d\x01"
		.ascii  "\xc8\xa4\x78\x01\x17\x5d\x74\x01"
		.ascii  "\x05\x2e\x70\x01\xc1\x16\x6c\x01"
		.ascii  "\x81\x16\x68\x01\x85\x2c\x64\x01"
		.ascii  "\x16\x58\x60\x01\x82\x98\x5c\x01"
		.ascii  "\x23\xed\x58\x01\x55\x55\x55\x01"
		.ascii  "\x7e\xd0\x51\x01\x0a\x5e\x4e\x01"
		.ascii  "\x6a\xfd\x4a\x01\x14\xae\x47\x01"
		.ascii  "\x86\x6f\x44\x01\x41\x41\x41\x01"
		.ascii  "\xcb\x22\x3e\x01\xb1\x13\x3b\x01"
		.ascii  "\x81\x13\x38\x01\xcf\x21\x35\x01"
		.ascii  "\x34\x3e\x32\x01\x4b\x68\x2f\x01"
		.ascii  "\xb4\x9f\x2c\x01\x12\xe4\x29\x01"
		.ascii  "\x0b\x35\x27\x01\x49\x92\x24\x01"
		.ascii  "\x78\xfb\x21\x01\x47\x70\x1f\x01"
		.ascii  "\x6a\xf0\x1c\x01\x96\x7b\x1a\x01"
		.ascii  "\x81\x11\x18\x01\xe5\xb1\x15\x01"
		.ascii  "\x81\x5c\x13\x01\x11\x11\x11\x01"
		.ascii  "\x56\xcf\x0e\x01\x14\x97\x0c\x01"
		.ascii  "\x10\x68\x0a\x01\x10\x42\x08\x01"
		.ascii  "\xdd\x24\x06\x01\x41\x10\x04\x01"
		.ascii  "\x08\x04\x02\x01\xff\xff\xff\x00"
		.ascii  "\xf8\x03\xfe\x00\xc0\x0f\xfc\x00"
		.ascii  "\x2c\x23\xfa\x00\x0f\x3e\xf8\x00"
		.ascii  "\x3d\x60\xf6\x00\x8d\x89\xf4\x00"
		.ascii  "\xd6\xb9\xf2\x00\xf0\xf0\xf0\x00"
		.ascii  "\xb7\x2e\xef\x00\x03\x73\xed\x00"
		.ascii  "\xb2\xbd\xeb\x00\xa0\x0e\xea\x00"
		.ascii  "\xac\x65\xe8\x00\xb4\xc2\xe6\x00"
		.ascii  "\x98\x25\xe5\x00\x38\x8e\xe3\x00"
		.ascii  "\x78\xfc\xe1\x00\x38\x70\xe0\x00"
		.ascii  "\x5c\xe9\xde\x00\xc8\x67\xdd\x00"
		.ascii  "\x61\xeb\xdb\x00\x0d\x74\xda\x00"
		.ascii  "\xb2\x01\xd9\x00\x35\x94\xd7\x00"
		.ascii  "\x80\x2b\xd6\x00\x7b\xc7\xd4\x00"
		.ascii  "\x0d\x68\xd3\x00\x20\x0d\xd2\x00"
		.ascii  "\x9f\xb6\xd0\x00\x74\x64\xcf\x00"
		.ascii  "\x8a\x16\xce\x00\xcc\xcc\xcc\x00"
		.ascii  "\x27\x87\xcb\x00\x87\x45\xca\x00"
		.ascii  "\xda\x07\xc9\x00\x0c\xce\xc7\x00"
		.ascii  "\x0c\x98\xc6\x00\xc8\x65\xc5\x00"
		.ascii  "\x2f\x37\xc4\x00\x30\x0c\xc3\x00"
		.ascii  "\xbb\xe4\xc1\x00\xc0\xc0\xc0\x00"
		.ascii  "\x2f\xa0\xbf\x00\xfa\x82\xbe\x00"
		.ascii  "\x10\x69\xbd\x00\x64\x52\xbc\x00"
		.ascii  "\xe7\x3e\xbb\x00\x8b\x2e\xba\x00"
		.ascii  "\x43\x21\xb9\x00\x02\x17\xb8\x00"
		.ascii  "\xbb\x0f\xb7\x00\x60\x0b\xb6\x00"
		.ascii  "\xe6\x09\xb5\x00\x40\x0b\xb4\x00"
		.ascii  "\x63\x0f\xb3\x00\x42\x16\xb2\x00"
		.ascii  "\xd3\x1f\xb1\x00\x0b\x2c\xb0\x00"
		.ascii  "\xdd\x3a\xaf\x00\x41\x4c\xae\x00"
		.ascii  "\x2b\x60\xad\x00\x91\x76\xac\x00"
		.ascii  "\x69\x8f\xab\x00\xaa\xaa\xaa\x00"
		.ascii  "\x4a\xc8\xa9\x00\x3f\xe8\xa8\x00"
		.ascii  "\x80\x0a\xa8\x00\x05\x2f\xa7\x00"
		.ascii  "\xc4\x55\xa6\x00\xb5\x7e\xa5\x00"
		.ascii  "\xcf\xa9\xa4\x00\x0a\xd7\xa3\x00"
		.ascii  "\x5e\x06\xa3\x00\xc3\x37\xa2\x00"
		.ascii  "\x31\x6b\xa1\x00\xa0\xa0\xa0\x00"
		.ascii  "\x09\xd8\x9f\x00\x65\x11\x9f\x00"
		.ascii  "\xad\x4c\x9e\x00\xd8\x89\x9d\x00"
		.ascii  "\xe1\xc8\x9c\x00\xc0\x09\x9c\x00"
		.ascii  "\x6f\x4c\x9b\x00\xe7\x90\x9a\x00"
		.ascii  "\x22\xd7\x99\x00\x1a\x1f\x99\x00"
		.ascii  "\xc8\x68\x98\x00\x25\xb4\x97\x00"
		.ascii  "\x2e\x01\x97\x00\xda\x4f\x96\x00"
		.ascii  "\x25\xa0\x95\x00\x09\xf2\x94\x00"
		.ascii  "\x80\x45\x94\x00\x85\x9a\x93\x00"
		.ascii  "\x13\xf1\x92\x00\x24\x49\x92\x00"
		.ascii  "\xb3\xa2\x91\x00\xbc\xfd\x90\x00"
		.ascii  "\x38\x5a\x90\x00\x23\xb8\x8f\x00"
		.ascii  "\x79\x17\x8f\x00\x35\x78\x8e\x00"
		.ascii  "\x52\xda\x8d\x00\xcb\x3d\x8d\x00"
		.ascii  "\x9c\xa2\x8c\x00\xc0\x08\x8c\x00"
		.ascii  "\x34\x70\x8b\x00\xf2\xd8\x8a\x00"
		.ascii  "\xf8\x42\x8a\x00\x40\xae\x89\x00"
		.ascii  "\xc7\x1a\x89\x00\x88\x88\x88\x00"
		.ascii  "\x80\xf7\x87\x00\xab\x67\x87\x00"
		.ascii  "\x05\xd9\x86\x00\x8a\x4b\x86\x00"
		.ascii  "\x37\xbf\x85\x00\x08\x34\x85\x00"
		.ascii  "\xf9\xa9\x84\x00\x08\x21\x84\x00"
		.ascii  "\x30\x99\x83\x00\x6e\x12\x83\x00"
		.ascii  "\xbf\x8c\x82\x00\x20\x08\x82\x00"
		.ascii  "\x8d\x84\x81\x00\x04\x02\x81\x00"
		.ascii  "\x80\x80\x80"
		.fill   0x9
		.ascii  "\x2e\x8b\xc0\x2e\x8b\xc0\x8b\xc0"
		.fill   0x80
data_12b8a0:
		.fill   0x82
		.ascii  "\xe0\xff\x00\x00\xe1\xff\x00\x00"
		.ascii  "\xe2\xff\x00\x00\xe3\xff\x00\x00"
		.ascii  "\xe4\xff\x00\x00\xe5\xff\x00\x00"
		.ascii  "\xe6\xff\x00\x00\xe7\xff\x00\x00"
		.ascii  "\xe8\xff\x00\x00\xe9\xff\x00\x00"
		.ascii  "\xea\xff\x00\x00\xeb\xff\x00\x00"
		.ascii  "\xec\xff\x00\x00\xed\xff\x00\x00"
		.ascii  "\xee\xff\x00\x00\xef\xff\x00\x00"
		.ascii  "\xf0\xff\x00\x00\xf1\xff\x00\x00"
		.ascii  "\xf2\xff\x00\x00\xf3\xff\x00\x00"
		.ascii  "\xf4\xff\x00\x00\xf5\xff\x00\x00"
		.ascii  "\xf6\xff\x00\x00\xf7\xff\x00\x00"
		.ascii  "\xf8\xff\x00\x00\xf9\xff\x00\x00"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\x00\x00"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x00\x00\xff\xff"
		.fill   0x6
		.ascii  "\x01\x00\x00\x00\x02\x00\x00\x00"
		.ascii  "\x03\x00\x00\x00\x04\x00\x00\x00"
		.ascii  "\x05\x00\x00\x00\x06\x00\x00\x00"
		.ascii  "\x07\x00\x00\x00\x08\x00\x00\x00"
		.ascii  "\x09\x00\x00\x00\x0a\x00\x00\x00"
		.ascii  "\x0b\x00\x00\x00\x0c\x00\x00\x00"
		.ascii  "\x0d\x00\x00\x00\x0e\x00\x00\x00"
		.ascii  "\x0f\x00\x00\x00\x10\x00\x00\x00"
		.ascii  "\x11\x00\x00\x00\x12\x00\x00\x00"
		.ascii  "\x13\x00\x00\x00\x14\x00\x00\x00"
		.ascii  "\x15\x00\x00\x00\x16\x00\x00\x00"
		.ascii  "\x17\x00\x00\x00\x18\x00\x00\x00"
		.ascii  "\x19\x00\x00\x00\x1a\x00\x00\x00"
		.ascii  "\x1b\x00\x00\x00\x1c\x00\x00\x00"
		.ascii  "\x1d\x00\x00\x00\x1e\x00\x00\x00"
		.ascii  "\x1f\x00\x00\x00\xf0\xff\x00\x80"
		.ascii  "\xf0\xff\x00\x00\xf1\xff\x00\x80"
		.ascii  "\xf1\xff\x00\x00\xf2\xff\x00\x80"
		.ascii  "\xf2\xff\x00\x00\xf3\xff\x00\x80"
		.ascii  "\xf3\xff\x00\x00\xf4\xff\x00\x80"
		.ascii  "\xf4\xff\x00\x00\xf5\xff\x00\x80"
		.ascii  "\xf5\xff\x00\x00\xf6\xff\x00\x80"
		.ascii  "\xf6\xff\x00\x00\xf7\xff\x00\x80"
		.ascii  "\xf7\xff\x00\x00\xf8\xff\x00\x80"
		.ascii  "\xf8\xff\x00\x00\xf9\xff\x00\x80"
		.ascii  "\xf9\xff\x00\x00\xfa\xff\x00\x80"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\x00\x80"
		.ascii  "\xfb\xff\x00\x00\xfc\xff\x00\x80"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x00\x80"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x00\x80"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x00\x80"
		.ascii  "\xff\xff"
		.fill   0x5
		.ascii  "\x80"
		.fill   0x4
		.ascii  "\x01\x00\x00\x80\x01\x00\x00\x00"
		.ascii  "\x02\x00\x00\x80\x02\x00\x00\x00"
		.ascii  "\x03\x00\x00\x80\x03\x00\x00\x00"
		.ascii  "\x04\x00\x00\x80\x04\x00\x00\x00"
		.ascii  "\x05\x00\x00\x80\x05\x00\x00\x00"
		.ascii  "\x06\x00\x00\x80\x06\x00\x00\x00"
		.ascii  "\x07\x00\x00\x80\x07\x00\x00\x00"
		.ascii  "\x08\x00\x00\x80\x08\x00\x00\x00"
		.ascii  "\x09\x00\x00\x80\x09\x00\x00\x00"
		.ascii  "\x0a\x00\x00\x80\x0a\x00\x00\x00"
		.ascii  "\x0b\x00\x00\x80\x0b\x00\x00\x00"
		.ascii  "\x0c\x00\x00\x80\x0c\x00\x00\x00"
		.ascii  "\x0d\x00\x00\x80\x0d\x00\x00\x00"
		.ascii  "\x0e\x00\x00\x80\x0e\x00\x00\x00"
		.ascii  "\x0f\x00\x00\x80\x0f\x00\x56\x55"
		.ascii  "\xf5\xff\xab\xaa\xf5\xff\x00\x00"
		.ascii  "\xf6\xff\x56\x55\xf6\xff\xab\xaa"
		.ascii  "\xf6\xff\x00\x00\xf7\xff\x56\x55"
		.ascii  "\xf7\xff\xab\xaa\xf7\xff\x00\x00"
		.ascii  "\xf8\xff\x56\x55\xf8\xff\xab\xaa"
		.ascii  "\xf8\xff\x00\x00\xf9\xff\x56\x55"
		.ascii  "\xf9\xff\xab\xaa\xf9\xff\x00\x00"
		.ascii  "\xfa\xff\x56\x55\xfa\xff\xab\xaa"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\x56\x55"
		.ascii  "\xfb\xff\xab\xaa\xfb\xff\x00\x00"
		.ascii  "\xfc\xff\x56\x55\xfc\xff\xab\xaa"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x56\x55"
		.ascii  "\xfd\xff\xab\xaa\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\xab\xaa"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x56\x55"
		.ascii  "\xff\xff\xab\xaa\xff\xff"
		.fill   0x4
		.ascii  "\x55\x55\x00\x00\xaa\xaa"
		.fill   0x4
		.ascii  "\x01\x00\x55\x55\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\x00\x00\x02\x00\x55\x55"
		.ascii  "\x02\x00\xaa\xaa\x02\x00\x00\x00"
		.ascii  "\x03\x00\x55\x55\x03\x00\xaa\xaa"
		.ascii  "\x03\x00\x00\x00\x04\x00\x55\x55"
		.ascii  "\x04\x00\xaa\xaa\x04\x00\x00\x00"
		.ascii  "\x05\x00\x55\x55\x05\x00\xaa\xaa"
		.ascii  "\x05\x00\x00\x00\x06\x00\x55\x55"
		.ascii  "\x06\x00\xaa\xaa\x06\x00\x00\x00"
		.ascii  "\x07\x00\x55\x55\x07\x00\xaa\xaa"
		.ascii  "\x07\x00\x00\x00\x08\x00\x55\x55"
		.ascii  "\x08\x00\xaa\xaa\x08\x00\x00\x00"
		.ascii  "\x09\x00\x55\x55\x09\x00\xaa\xaa"
		.ascii  "\x09\x00\x00\x00\x0a\x00\x55\x55"
		.ascii  "\x0a\x00\x00\x00\xf8\xff\x00\x40"
		.ascii  "\xf8\xff\x00\x80\xf8\xff\x00\xc0"
		.ascii  "\xf8\xff\x00\x00\xf9\xff\x00\x40"
		.ascii  "\xf9\xff\x00\x80\xf9\xff\x00\xc0"
		.ascii  "\xf9\xff\x00\x00\xfa\xff\x00\x40"
		.ascii  "\xfa\xff\x00\x80\xfa\xff\x00\xc0"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\x00\x40"
		.ascii  "\xfb\xff\x00\x80\xfb\xff\x00\xc0"
		.ascii  "\xfb\xff\x00\x00\xfc\xff\x00\x40"
		.ascii  "\xfc\xff\x00\x80\xfc\xff\x00\xc0"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x00\x40"
		.ascii  "\xfd\xff\x00\x80\xfd\xff\x00\xc0"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x00\x40"
		.ascii  "\xfe\xff\x00\x80\xfe\xff\x00\xc0"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x00\x40"
		.ascii  "\xff\xff\x00\x80\xff\xff\x00\xc0"
		.ascii  "\xff\xff"
		.fill   0x5
		.ascii  "\x40\x00\x00\x00\x80\x00\x00\x00"
		.ascii  "\xc0"
		.fill   0x4
		.ascii  "\x01\x00\x00\x40\x01\x00\x00\x80"
		.ascii  "\x01\x00\x00\xc0\x01\x00\x00\x00"
		.ascii  "\x02\x00\x00\x40\x02\x00\x00\x80"
		.ascii  "\x02\x00\x00\xc0\x02\x00\x00\x00"
		.ascii  "\x03\x00\x00\x40\x03\x00\x00\x80"
		.ascii  "\x03\x00\x00\xc0\x03\x00\x00\x00"
		.ascii  "\x04\x00\x00\x40\x04\x00\x00\x80"
		.ascii  "\x04\x00\x00\xc0\x04\x00\x00\x00"
		.ascii  "\x05\x00\x00\x40\x05\x00\x00\x80"
		.ascii  "\x05\x00\x00\xc0\x05\x00\x00\x00"
		.ascii  "\x06\x00\x00\x40\x06\x00\x00\x80"
		.ascii  "\x06\x00\x00\xc0\x06\x00\x00\x00"
		.ascii  "\x07\x00\x00\x40\x07\x00\x00\x80"
		.ascii  "\x07\x00\x00\xc0\x07\x00\x9a\x99"
		.ascii  "\xf9\xff\xcd\xcc\xf9\xff\x00\x00"
		.ascii  "\xfa\xff\x34\x33\xfa\xff\x67\x66"
		.ascii  "\xfa\xff\x9a\x99\xfa\xff\xcd\xcc"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\x34\x33"
		.ascii  "\xfb\xff\x67\x66\xfb\xff\x9a\x99"
		.ascii  "\xfb\xff\xcd\xcc\xfb\xff\x00\x00"
		.ascii  "\xfc\xff\x34\x33\xfc\xff\x67\x66"
		.ascii  "\xfc\xff\x9a\x99\xfc\xff\xcd\xcc"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x34\x33"
		.ascii  "\xfd\xff\x67\x66\xfd\xff\x9a\x99"
		.ascii  "\xfd\xff\xcd\xcc\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x34\x33\xfe\xff\x67\x66"
		.ascii  "\xfe\xff\x9a\x99\xfe\xff\xcd\xcc"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x34\x33"
		.ascii  "\xff\xff\x67\x66\xff\xff\x9a\x99"
		.ascii  "\xff\xff\xcd\xcc\xff\xff"
		.fill   0x4
		.ascii  "\x33\x33\x00\x00\x66\x66\x00\x00"
		.ascii  "\x99\x99\x00\x00\xcc\xcc"
		.fill   0x4
		.ascii  "\x01\x00\x33\x33\x01\x00\x66\x66"
		.ascii  "\x01\x00\x99\x99\x01\x00\xcc\xcc"
		.ascii  "\x01\x00\x00\x00\x02\x00\x33\x33"
		.ascii  "\x02\x00\x66\x66\x02\x00\x99\x99"
		.ascii  "\x02\x00\xcc\xcc\x02\x00\x00\x00"
		.ascii  "\x03\x00\x33\x33\x03\x00\x66\x66"
		.ascii  "\x03\x00\x99\x99\x03\x00\xcc\xcc"
		.ascii  "\x03\x00\x00\x00\x04\x00\x33\x33"
		.ascii  "\x04\x00\x66\x66\x04\x00\x99\x99"
		.ascii  "\x04\x00\xcc\xcc\x04\x00\x00\x00"
		.ascii  "\x05\x00\x33\x33\x05\x00\x66\x66"
		.ascii  "\x05\x00\x99\x99\x05\x00\xcc\xcc"
		.ascii  "\x05\x00\x00\x00\x06\x00\x33\x33"
		.ascii  "\x06\x00\xab\xaa\xfa\xff\x56\xd5"
		.ascii  "\xfa\xff\x00\x00\xfb\xff\xab\x2a"
		.ascii  "\xfb\xff\x56\x55\xfb\xff\x00\x80"
		.ascii  "\xfb\xff\xab\xaa\xfb\xff\x56\xd5"
		.ascii  "\xfb\xff\x00\x00\xfc\xff\xab\x2a"
		.ascii  "\xfc\xff\x56\x55\xfc\xff\x00\x80"
		.ascii  "\xfc\xff\xab\xaa\xfc\xff\x56\xd5"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\xab\x2a"
		.ascii  "\xfd\xff\x56\x55\xfd\xff\x00\x80"
		.ascii  "\xfd\xff\xab\xaa\xfd\xff\x56\xd5"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\xab\x2a"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\x00\x80"
		.ascii  "\xfe\xff\xab\xaa\xfe\xff\x56\xd5"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xab\x2a"
		.ascii  "\xff\xff\x56\x55\xff\xff\x00\x80"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x56\xd5"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\xaa\x2a\x00\x00\x55\x55\x00\x00"
		.ascii  "\x00\x80\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x55\xd5"
		.fill   0x4
		.ascii  "\x01\x00\xaa\x2a\x01\x00\x55\x55"
		.ascii  "\x01\x00\x00\x80\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\x55\xd5\x01\x00\x00\x00"
		.ascii  "\x02\x00\xaa\x2a\x02\x00\x55\x55"
		.ascii  "\x02\x00\x00\x80\x02\x00\xaa\xaa"
		.ascii  "\x02\x00\x55\xd5\x02\x00\x00\x00"
		.ascii  "\x03\x00\xaa\x2a\x03\x00\x55\x55"
		.ascii  "\x03\x00\x00\x80\x03\x00\xaa\xaa"
		.ascii  "\x03\x00\x55\xd5\x03\x00\x00\x00"
		.ascii  "\x04\x00\xaa\x2a\x04\x00\x55\x55"
		.ascii  "\x04\x00\x00\x80\x04\x00\xaa\xaa"
		.ascii  "\x04\x00\x55\xd5\x04\x00\x00\x00"
		.ascii  "\x05\x00\xaa\x2a\x05\x00\xb7\x6d"
		.ascii  "\xfb\xff\x4a\x92\xfb\xff\xdc\xb6"
		.ascii  "\xfb\xff\x6e\xdb\xfb\xff\x00\x00"
		.ascii  "\xfc\xff\x93\x24\xfc\xff\x25\x49"
		.ascii  "\xfc\xff\xb7\x6d\xfc\xff\x4a\x92"
		.ascii  "\xfc\xff\xdc\xb6\xfc\xff\x6e\xdb"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x93\x24"
		.ascii  "\xfd\xff\x25\x49\xfd\xff\xb7\x6d"
		.ascii  "\xfd\xff\x4a\x92\xfd\xff\xdc\xb6"
		.ascii  "\xfd\xff\x6e\xdb\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x93\x24\xfe\xff\x25\x49"
		.ascii  "\xfe\xff\xb7\x6d\xfe\xff\x4a\x92"
		.ascii  "\xfe\xff\xdc\xb6\xfe\xff\x6e\xdb"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x93\x24"
		.ascii  "\xff\xff\x25\x49\xff\xff\xb7\x6d"
		.ascii  "\xff\xff\x4a\x92\xff\xff\xdc\xb6"
		.ascii  "\xff\xff\x6e\xdb\xff\xff"
		.fill   0x4
		.ascii  "\x92\x24\x00\x00\x24\x49\x00\x00"
		.ascii  "\xb6\x6d\x00\x00\x49\x92\x00\x00"
		.ascii  "\xdb\xb6\x00\x00\x6d\xdb"
		.fill   0x4
		.ascii  "\x01\x00\x92\x24\x01\x00\x24\x49"
		.ascii  "\x01\x00\xb6\x6d\x01\x00\x49\x92"
		.ascii  "\x01\x00\xdb\xb6\x01\x00\x6d\xdb"
		.ascii  "\x01\x00\x00\x00\x02\x00\x92\x24"
		.ascii  "\x02\x00\x24\x49\x02\x00\xb6\x6d"
		.ascii  "\x02\x00\x49\x92\x02\x00\xdb\xb6"
		.ascii  "\x02\x00\x6d\xdb\x02\x00\x00\x00"
		.ascii  "\x03\x00\x92\x24\x03\x00\x24\x49"
		.ascii  "\x03\x00\xb6\x6d\x03\x00\x49\x92"
		.ascii  "\x03\x00\xdb\xb6\x03\x00\x6d\xdb"
		.ascii  "\x03\x00\x00\x00\x04\x00\x92\x24"
		.ascii  "\x04\x00\x24\x49\x04\x00\xb6\x6d"
		.ascii  "\x04\x00\x00\x00\xfc\xff\x00\x20"
		.ascii  "\xfc\xff\x00\x40\xfc\xff\x00\x60"
		.ascii  "\xfc\xff\x00\x80\xfc\xff\x00\xa0"
		.ascii  "\xfc\xff\x00\xc0\xfc\xff\x00\xe0"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x00\x20"
		.ascii  "\xfd\xff\x00\x40\xfd\xff\x00\x60"
		.ascii  "\xfd\xff\x00\x80\xfd\xff\x00\xa0"
		.ascii  "\xfd\xff\x00\xc0\xfd\xff\x00\xe0"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x00\x20"
		.ascii  "\xfe\xff\x00\x40\xfe\xff\x00\x60"
		.ascii  "\xfe\xff\x00\x80\xfe\xff\x00\xa0"
		.ascii  "\xfe\xff\x00\xc0\xfe\xff\x00\xe0"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x00\x20"
		.ascii  "\xff\xff\x00\x40\xff\xff\x00\x60"
		.ascii  "\xff\xff\x00\x80\xff\xff\x00\xa0"
		.ascii  "\xff\xff\x00\xc0\xff\xff\x00\xe0"
		.ascii  "\xff\xff"
		.fill   0x5
		.ascii  "\x20\x00\x00\x00\x40\x00\x00\x00"
		.ascii  "\x60\x00\x00\x00\x80\x00\x00\x00"
		.ascii  "\xa0\x00\x00\x00\xc0\x00\x00\x00"
		.ascii  "\xe0"
		.fill   0x4
		.ascii  "\x01\x00\x00\x20\x01\x00\x00\x40"
		.ascii  "\x01\x00\x00\x60\x01\x00\x00\x80"
		.ascii  "\x01\x00\x00\xa0\x01\x00\x00\xc0"
		.ascii  "\x01\x00\x00\xe0\x01\x00\x00\x00"
		.ascii  "\x02\x00\x00\x20\x02\x00\x00\x40"
		.ascii  "\x02\x00\x00\x60\x02\x00\x00\x80"
		.ascii  "\x02\x00\x00\xa0\x02\x00\x00\xc0"
		.ascii  "\x02\x00\x00\xe0\x02\x00\x00\x00"
		.ascii  "\x03\x00\x00\x20\x03\x00\x00\x40"
		.ascii  "\x03\x00\x00\x60\x03\x00\x00\x80"
		.ascii  "\x03\x00\x00\xa0\x03\x00\x00\xc0"
		.ascii  "\x03\x00\x00\xe0\x03\x00\xc8\x71"
		.ascii  "\xfc\xff\x39\x8e\xfc\xff\xab\xaa"
		.ascii  "\xfc\xff\x1d\xc7\xfc\xff\x8f\xe3"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x72\x1c"
		.ascii  "\xfd\xff\xe4\x38\xfd\xff\x56\x55"
		.ascii  "\xfd\xff\xc8\x71\xfd\xff\x39\x8e"
		.ascii  "\xfd\xff\xab\xaa\xfd\xff\x1d\xc7"
		.ascii  "\xfd\xff\x8f\xe3\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x72\x1c\xfe\xff\xe4\x38"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\xc8\x71"
		.ascii  "\xfe\xff\x39\x8e\xfe\xff\xab\xaa"
		.ascii  "\xfe\xff\x1d\xc7\xfe\xff\x8f\xe3"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x72\x1c"
		.ascii  "\xff\xff\xe4\x38\xff\xff\x56\x55"
		.ascii  "\xff\xff\xc8\x71\xff\xff\x39\x8e"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x1d\xc7"
		.ascii  "\xff\xff\x8f\xe3\xff\xff"
		.fill   0x4
		.ascii  "\x71\x1c\x00\x00\xe3\x38\x00\x00"
		.ascii  "\x55\x55\x00\x00\xc7\x71\x00\x00"
		.ascii  "\x38\x8e\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x1c\xc7\x00\x00\x8e\xe3"
		.fill   0x4
		.ascii  "\x01\x00\x71\x1c\x01\x00\xe3\x38"
		.ascii  "\x01\x00\x55\x55\x01\x00\xc7\x71"
		.ascii  "\x01\x00\x38\x8e\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\x1c\xc7\x01\x00\x8e\xe3"
		.ascii  "\x01\x00\x00\x00\x02\x00\x71\x1c"
		.ascii  "\x02\x00\xe3\x38\x02\x00\x55\x55"
		.ascii  "\x02\x00\xc7\x71\x02\x00\x38\x8e"
		.ascii  "\x02\x00\xaa\xaa\x02\x00\x1c\xc7"
		.ascii  "\x02\x00\x8e\xe3\x02\x00\x00\x00"
		.ascii  "\x03\x00\x71\x1c\x03\x00\xe3\x38"
		.ascii  "\x03\x00\x55\x55\x03\x00\xc7\x71"
		.ascii  "\x03\x00\xcd\xcc\xfc\xff\x67\xe6"
		.ascii  "\xfc\xff\x00\x00\xfd\xff\x9a\x19"
		.ascii  "\xfd\xff\x34\x33\xfd\xff\xcd\x4c"
		.ascii  "\xfd\xff\x67\x66\xfd\xff\x00\x80"
		.ascii  "\xfd\xff\x9a\x99\xfd\xff\x34\xb3"
		.ascii  "\xfd\xff\xcd\xcc\xfd\xff\x67\xe6"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x9a\x19"
		.ascii  "\xfe\xff\x34\x33\xfe\xff\xcd\x4c"
		.ascii  "\xfe\xff\x67\x66\xfe\xff\x00\x80"
		.ascii  "\xfe\xff\x9a\x99\xfe\xff\x34\xb3"
		.ascii  "\xfe\xff\xcd\xcc\xfe\xff\x67\xe6"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x9a\x19"
		.ascii  "\xff\xff\x34\x33\xff\xff\xcd\x4c"
		.ascii  "\xff\xff\x67\x66\xff\xff\x00\x80"
		.ascii  "\xff\xff\x9a\x99\xff\xff\x34\xb3"
		.ascii  "\xff\xff\xcd\xcc\xff\xff\x67\xe6"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x99\x19\x00\x00\x33\x33\x00\x00"
		.ascii  "\xcc\x4c\x00\x00\x66\x66\x00\x00"
		.ascii  "\x00\x80\x00\x00\x99\x99\x00\x00"
		.ascii  "\x33\xb3\x00\x00\xcc\xcc\x00\x00"
		.ascii  "\x66\xe6"
		.fill   0x4
		.ascii  "\x01\x00\x99\x19\x01\x00\x33\x33"
		.ascii  "\x01\x00\xcc\x4c\x01\x00\x66\x66"
		.ascii  "\x01\x00\x00\x80\x01\x00\x99\x99"
		.ascii  "\x01\x00\x33\xb3\x01\x00\xcc\xcc"
		.ascii  "\x01\x00\x66\xe6\x01\x00\x00\x00"
		.ascii  "\x02\x00\x99\x19\x02\x00\x33\x33"
		.ascii  "\x02\x00\xcc\x4c\x02\x00\x66\x66"
		.ascii  "\x02\x00\x00\x80\x02\x00\x99\x99"
		.ascii  "\x02\x00\x33\xb3\x02\x00\xcc\xcc"
		.ascii  "\x02\x00\x66\xe6\x02\x00\x00\x00"
		.ascii  "\x03\x00\x99\x19\x03\x00\x46\x17"
		.ascii  "\xfd\xff\x8c\x2e\xfd\xff\xd2\x45"
		.ascii  "\xfd\xff\x18\x5d\xfd\xff\x5e\x74"
		.ascii  "\xfd\xff\xa3\x8b\xfd\xff\xe9\xa2"
		.ascii  "\xfd\xff\x2f\xba\xfd\xff\x75\xd1"
		.ascii  "\xfd\xff\xbb\xe8\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x46\x17\xfe\xff\x8c\x2e"
		.ascii  "\xfe\xff\xd2\x45\xfe\xff\x18\x5d"
		.ascii  "\xfe\xff\x5e\x74\xfe\xff\xa3\x8b"
		.ascii  "\xfe\xff\xe9\xa2\xfe\xff\x2f\xba"
		.ascii  "\xfe\xff\x75\xd1\xfe\xff\xbb\xe8"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x46\x17"
		.ascii  "\xff\xff\x8c\x2e\xff\xff\xd2\x45"
		.ascii  "\xff\xff\x18\x5d\xff\xff\x5e\x74"
		.ascii  "\xff\xff\xa3\x8b\xff\xff\xe9\xa2"
		.ascii  "\xff\xff\x2f\xba\xff\xff\x75\xd1"
		.ascii  "\xff\xff\xbb\xe8\xff\xff"
		.fill   0x4
		.ascii  "\x45\x17\x00\x00\x8b\x2e\x00\x00"
		.ascii  "\xd1\x45\x00\x00\x17\x5d\x00\x00"
		.ascii  "\x5d\x74\x00\x00\xa2\x8b\x00\x00"
		.ascii  "\xe8\xa2\x00\x00\x2e\xba\x00\x00"
		.ascii  "\x74\xd1\x00\x00\xba\xe8"
		.fill   0x4
		.ascii  "\x01\x00\x45\x17\x01\x00\x8b\x2e"
		.ascii  "\x01\x00\xd1\x45\x01\x00\x17\x5d"
		.ascii  "\x01\x00\x5d\x74\x01\x00\xa2\x8b"
		.ascii  "\x01\x00\xe8\xa2\x01\x00\x2e\xba"
		.ascii  "\x01\x00\x74\xd1\x01\x00\xba\xe8"
		.ascii  "\x01\x00\x00\x00\x02\x00\x45\x17"
		.ascii  "\x02\x00\x8b\x2e\x02\x00\xd1\x45"
		.ascii  "\x02\x00\x17\x5d\x02\x00\x5d\x74"
		.ascii  "\x02\x00\xa2\x8b\x02\x00\xe8\xa2"
		.ascii  "\x02\x00\x2e\xba\x02\x00\x74\xd1"
		.ascii  "\x02\x00\x56\x55\xfd\xff\xab\x6a"
		.ascii  "\xfd\xff\x00\x80\xfd\xff\x56\x95"
		.ascii  "\xfd\xff\xab\xaa\xfd\xff\x00\xc0"
		.ascii  "\xfd\xff\x56\xd5\xfd\xff\xab\xea"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x56\x15"
		.ascii  "\xfe\xff\xab\x2a\xfe\xff\x00\x40"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\xab\x6a"
		.ascii  "\xfe\xff\x00\x80\xfe\xff\x56\x95"
		.ascii  "\xfe\xff\xab\xaa\xfe\xff\x00\xc0"
		.ascii  "\xfe\xff\x56\xd5\xfe\xff\xab\xea"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x56\x15"
		.ascii  "\xff\xff\xab\x2a\xff\xff\x00\x40"
		.ascii  "\xff\xff\x56\x55\xff\xff\xab\x6a"
		.ascii  "\xff\xff\x00\x80\xff\xff\x56\x95"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x00\xc0"
		.ascii  "\xff\xff\x56\xd5\xff\xff\xab\xea"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x55\x15\x00\x00\xaa\x2a\x00\x00"
		.ascii  "\x00\x40\x00\x00\x55\x55\x00\x00"
		.ascii  "\xaa\x6a\x00\x00\x00\x80\x00\x00"
		.ascii  "\x55\x95\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x00\xc0\x00\x00\x55\xd5\x00\x00"
		.ascii  "\xaa\xea"
		.fill   0x4
		.ascii  "\x01\x00\x55\x15\x01\x00\xaa\x2a"
		.ascii  "\x01\x00\x00\x40\x01\x00\x55\x55"
		.ascii  "\x01\x00\xaa\x6a\x01\x00\x00\x80"
		.ascii  "\x01\x00\x55\x95\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\x00\xc0\x01\x00\x55\xd5"
		.ascii  "\x01\x00\xaa\xea\x01\x00\x00\x00"
		.ascii  "\x02\x00\x55\x15\x02\x00\xaa\x2a"
		.ascii  "\x02\x00\x00\x40\x02\x00\x55\x55"
		.ascii  "\x02\x00\xaa\x6a\x02\x00\x00\x80"
		.ascii  "\x02\x00\x55\x95\x02\x00\xd9\x89"
		.ascii  "\xfd\xff\x8a\x9d\xfd\xff\x3c\xb1"
		.ascii  "\xfd\xff\xed\xc4\xfd\xff\x9e\xd8"
		.ascii  "\xfd\xff\x4f\xec\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\xb2\x13\xfe\xff\x63\x27"
		.ascii  "\xfe\xff\x14\x3b\xfe\xff\xc5\x4e"
		.ascii  "\xfe\xff\x77\x62\xfe\xff\x28\x76"
		.ascii  "\xfe\xff\xd9\x89\xfe\xff\x8a\x9d"
		.ascii  "\xfe\xff\x3c\xb1\xfe\xff\xed\xc4"
		.ascii  "\xfe\xff\x9e\xd8\xfe\xff\x4f\xec"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xb2\x13"
		.ascii  "\xff\xff\x63\x27\xff\xff\x14\x3b"
		.ascii  "\xff\xff\xc5\x4e\xff\xff\x77\x62"
		.ascii  "\xff\xff\x28\x76\xff\xff\xd9\x89"
		.ascii  "\xff\xff\x8a\x9d\xff\xff\x3c\xb1"
		.ascii  "\xff\xff\xed\xc4\xff\xff\x9e\xd8"
		.ascii  "\xff\xff\x4f\xec\xff\xff"
		.fill   0x4
		.ascii  "\xb1\x13\x00\x00\x62\x27\x00\x00"
		.ascii  "\x13\x3b\x00\x00\xc4\x4e\x00\x00"
		.ascii  "\x76\x62\x00\x00\x27\x76\x00\x00"
		.ascii  "\xd8\x89\x00\x00\x89\x9d\x00\x00"
		.ascii  "\x3b\xb1\x00\x00\xec\xc4\x00\x00"
		.ascii  "\x9d\xd8\x00\x00\x4e\xec"
		.fill   0x4
		.ascii  "\x01\x00\xb1\x13\x01\x00\x62\x27"
		.ascii  "\x01\x00\x13\x3b\x01\x00\xc4\x4e"
		.ascii  "\x01\x00\x76\x62\x01\x00\x27\x76"
		.ascii  "\x01\x00\xd8\x89\x01\x00\x89\x9d"
		.ascii  "\x01\x00\x3b\xb1\x01\x00\xec\xc4"
		.ascii  "\x01\x00\x9d\xd8\x01\x00\x4e\xec"
		.ascii  "\x01\x00\x00\x00\x02\x00\xb1\x13"
		.ascii  "\x02\x00\x62\x27\x02\x00\x13\x3b"
		.ascii  "\x02\x00\xc4\x4e\x02\x00\x76\x62"
		.ascii  "\x02\x00\xdc\xb6\xfd\xff\x25\xc9"
		.ascii  "\xfd\xff\x6e\xdb\xfd\xff\xb7\xed"
		.ascii  "\xfd\xff\x00\x00\xfe\xff\x4a\x12"
		.ascii  "\xfe\xff\x93\x24\xfe\xff\xdc\x36"
		.ascii  "\xfe\xff\x25\x49\xfe\xff\x6e\x5b"
		.ascii  "\xfe\xff\xb7\x6d\xfe\xff\x00\x80"
		.ascii  "\xfe\xff\x4a\x92\xfe\xff\x93\xa4"
		.ascii  "\xfe\xff\xdc\xb6\xfe\xff\x25\xc9"
		.ascii  "\xfe\xff\x6e\xdb\xfe\xff\xb7\xed"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x4a\x12"
		.ascii  "\xff\xff\x93\x24\xff\xff\xdc\x36"
		.ascii  "\xff\xff\x25\x49\xff\xff\x6e\x5b"
		.ascii  "\xff\xff\xb7\x6d\xff\xff\x00\x80"
		.ascii  "\xff\xff\x4a\x92\xff\xff\x93\xa4"
		.ascii  "\xff\xff\xdc\xb6\xff\xff\x25\xc9"
		.ascii  "\xff\xff\x6e\xdb\xff\xff\xb7\xed"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x49\x12\x00\x00\x92\x24\x00\x00"
		.ascii  "\xdb\x36\x00\x00\x24\x49\x00\x00"
		.ascii  "\x6d\x5b\x00\x00\xb6\x6d\x00\x00"
		.ascii  "\x00\x80\x00\x00\x49\x92\x00\x00"
		.ascii  "\x92\xa4\x00\x00\xdb\xb6\x00\x00"
		.ascii  "\x24\xc9\x00\x00\x6d\xdb\x00\x00"
		.ascii  "\xb6\xed"
		.fill   0x4
		.ascii  "\x01\x00\x49\x12\x01\x00\x92\x24"
		.ascii  "\x01\x00\xdb\x36\x01\x00\x24\x49"
		.ascii  "\x01\x00\x6d\x5b\x01\x00\xb6\x6d"
		.ascii  "\x01\x00\x00\x80\x01\x00\x49\x92"
		.ascii  "\x01\x00\x92\xa4\x01\x00\xdb\xb6"
		.ascii  "\x01\x00\x24\xc9\x01\x00\x6d\xdb"
		.ascii  "\x01\x00\xb6\xed\x01\x00\x00\x00"
		.ascii  "\x02\x00\x49\x12\x02\x00\x92\x24"
		.ascii  "\x02\x00\xdb\x36\x02\x00\xde\xdd"
		.ascii  "\xfd\xff\xef\xee\xfd\xff\x00\x00"
		.ascii  "\xfe\xff\x12\x11\xfe\xff\x23\x22"
		.ascii  "\xfe\xff\x34\x33\xfe\xff\x45\x44"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\x67\x66"
		.ascii  "\xfe\xff\x78\x77\xfe\xff\x89\x88"
		.ascii  "\xfe\xff\x9a\x99\xfe\xff\xab\xaa"
		.ascii  "\xfe\xff\xbc\xbb\xfe\xff\xcd\xcc"
		.ascii  "\xfe\xff\xde\xdd\xfe\xff\xef\xee"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x12\x11"
		.ascii  "\xff\xff\x23\x22\xff\xff\x34\x33"
		.ascii  "\xff\xff\x45\x44\xff\xff\x56\x55"
		.ascii  "\xff\xff\x67\x66\xff\xff\x78\x77"
		.ascii  "\xff\xff\x89\x88\xff\xff\x9a\x99"
		.ascii  "\xff\xff\xab\xaa\xff\xff\xbc\xbb"
		.ascii  "\xff\xff\xcd\xcc\xff\xff\xde\xdd"
		.ascii  "\xff\xff\xef\xee\xff\xff"
		.fill   0x4
		.ascii  "\x11\x11\x00\x00\x22\x22\x00\x00"
		.ascii  "\x33\x33\x00\x00\x44\x44\x00\x00"
		.ascii  "\x55\x55\x00\x00\x66\x66\x00\x00"
		.ascii  "\x77\x77\x00\x00\x88\x88\x00\x00"
		.ascii  "\x99\x99\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\xbb\xbb\x00\x00\xcc\xcc\x00\x00"
		.ascii  "\xdd\xdd\x00\x00\xee\xee"
		.fill   0x4
		.ascii  "\x01\x00\x11\x11\x01\x00\x22\x22"
		.ascii  "\x01\x00\x33\x33\x01\x00\x44\x44"
		.ascii  "\x01\x00\x55\x55\x01\x00\x66\x66"
		.ascii  "\x01\x00\x77\x77\x01\x00\x88\x88"
		.ascii  "\x01\x00\x99\x99\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\xbb\xbb\x01\x00\xcc\xcc"
		.ascii  "\x01\x00\xdd\xdd\x01\x00\xee\xee"
		.ascii  "\x01\x00\x00\x00\x02\x00\x11\x11"
		.ascii  "\x02\x00\x00\x00\xfe\xff\x00\x10"
		.ascii  "\xfe\xff\x00\x20\xfe\xff\x00\x30"
		.ascii  "\xfe\xff\x00\x40\xfe\xff\x00\x50"
		.ascii  "\xfe\xff\x00\x60\xfe\xff\x00\x70"
		.ascii  "\xfe\xff\x00\x80\xfe\xff\x00\x90"
		.ascii  "\xfe\xff\x00\xa0\xfe\xff\x00\xb0"
		.ascii  "\xfe\xff\x00\xc0\xfe\xff\x00\xd0"
		.ascii  "\xfe\xff\x00\xe0\xfe\xff\x00\xf0"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x00\x10"
		.ascii  "\xff\xff\x00\x20\xff\xff\x00\x30"
		.ascii  "\xff\xff\x00\x40\xff\xff\x00\x50"
		.ascii  "\xff\xff\x00\x60\xff\xff\x00\x70"
		.ascii  "\xff\xff\x00\x80\xff\xff\x00\x90"
		.ascii  "\xff\xff\x00\xa0\xff\xff\x00\xb0"
		.ascii  "\xff\xff\x00\xc0\xff\xff\x00\xd0"
		.ascii  "\xff\xff\x00\xe0\xff\xff\x00\xf0"
		.ascii  "\xff\xff"
		.fill   0x5
		.ascii  "\x10\x00\x00\x00\x20\x00\x00\x00"
		.ascii  "\x30\x00\x00\x00\x40\x00\x00\x00"
		.ascii  "\x50\x00\x00\x00\x60\x00\x00\x00"
		.ascii  "\x70\x00\x00\x00\x80\x00\x00\x00"
		.ascii  "\x90\x00\x00\x00\xa0\x00\x00\x00"
		.ascii  "\xb0\x00\x00\x00\xc0\x00\x00\x00"
		.ascii  "\xd0\x00\x00\x00\xe0\x00\x00\x00"
		.ascii  "\xf0"
		.fill   0x4
		.ascii  "\x01\x00\x00\x10\x01\x00\x00\x20"
		.ascii  "\x01\x00\x00\x30\x01\x00\x00\x40"
		.ascii  "\x01\x00\x00\x50\x01\x00\x00\x60"
		.ascii  "\x01\x00\x00\x70\x01\x00\x00\x80"
		.ascii  "\x01\x00\x00\x90\x01\x00\x00\xa0"
		.ascii  "\x01\x00\x00\xb0\x01\x00\x00\xc0"
		.ascii  "\x01\x00\x00\xd0\x01\x00\x00\xe0"
		.ascii  "\x01\x00\x00\xf0\x01\x00\x1f\x1e"
		.ascii  "\xfe\xff\x2e\x2d\xfe\xff\x3d\x3c"
		.ascii  "\xfe\xff\x4c\x4b\xfe\xff\x5b\x5a"
		.ascii  "\xfe\xff\x6a\x69\xfe\xff\x79\x78"
		.ascii  "\xfe\xff\x88\x87\xfe\xff\x97\x96"
		.ascii  "\xfe\xff\xa6\xa5\xfe\xff\xb5\xb4"
		.ascii  "\xfe\xff\xc4\xc3\xfe\xff\xd3\xd2"
		.ascii  "\xfe\xff\xe2\xe1\xfe\xff\xf1\xf0"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x10\x0f"
		.ascii  "\xff\xff\x1f\x1e\xff\xff\x2e\x2d"
		.ascii  "\xff\xff\x3d\x3c\xff\xff\x4c\x4b"
		.ascii  "\xff\xff\x5b\x5a\xff\xff\x6a\x69"
		.ascii  "\xff\xff\x79\x78\xff\xff\x88\x87"
		.ascii  "\xff\xff\x97\x96\xff\xff\xa6\xa5"
		.ascii  "\xff\xff\xb5\xb4\xff\xff\xc4\xc3"
		.ascii  "\xff\xff\xd3\xd2\xff\xff\xe2\xe1"
		.ascii  "\xff\xff\xf1\xf0\xff\xff"
		.fill   0x4
		.ascii  "\x0f\x0f\x00\x00\x1e\x1e\x00\x00"
		.ascii  "\x2d\x2d\x00\x00\x3c\x3c\x00\x00"
		.ascii  "\x4b\x4b\x00\x00\x5a\x5a\x00\x00"
		.ascii  "\x69\x69\x00\x00\x78\x78\x00\x00"
		.ascii  "\x87\x87\x00\x00\x96\x96\x00\x00"
		.ascii  "\xa5\xa5\x00\x00\xb4\xb4\x00\x00"
		.ascii  "\xc3\xc3\x00\x00\xd2\xd2\x00\x00"
		.ascii  "\xe1\xe1\x00\x00\xf0\xf0"
		.fill   0x4
		.ascii  "\x01\x00\x0f\x0f\x01\x00\x1e\x1e"
		.ascii  "\x01\x00\x2d\x2d\x01\x00\x3c\x3c"
		.ascii  "\x01\x00\x4b\x4b\x01\x00\x5a\x5a"
		.ascii  "\x01\x00\x69\x69\x01\x00\x78\x78"
		.ascii  "\x01\x00\x87\x87\x01\x00\x96\x96"
		.ascii  "\x01\x00\xa5\xa5\x01\x00\xb4\xb4"
		.ascii  "\x01\x00\xc3\xc3\x01\x00\xd2\xd2"
		.ascii  "\x01\x00\xe4\x38\xfe\xff\x1d\x47"
		.ascii  "\xfe\xff\x56\x55\xfe\xff\x8f\x63"
		.ascii  "\xfe\xff\xc8\x71\xfe\xff\x00\x80"
		.ascii  "\xfe\xff\x39\x8e\xfe\xff\x72\x9c"
		.ascii  "\xfe\xff\xab\xaa\xfe\xff\xe4\xb8"
		.ascii  "\xfe\xff\x1d\xc7\xfe\xff\x56\xd5"
		.ascii  "\xfe\xff\x8f\xe3\xfe\xff\xc8\xf1"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x39\x0e"
		.ascii  "\xff\xff\x72\x1c\xff\xff\xab\x2a"
		.ascii  "\xff\xff\xe4\x38\xff\xff\x1d\x47"
		.ascii  "\xff\xff\x56\x55\xff\xff\x8f\x63"
		.ascii  "\xff\xff\xc8\x71\xff\xff\x00\x80"
		.ascii  "\xff\xff\x39\x8e\xff\xff\x72\x9c"
		.ascii  "\xff\xff\xab\xaa\xff\xff\xe4\xb8"
		.ascii  "\xff\xff\x1d\xc7\xff\xff\x56\xd5"
		.ascii  "\xff\xff\x8f\xe3\xff\xff\xc8\xf1"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x38\x0e\x00\x00\x71\x1c\x00\x00"
		.ascii  "\xaa\x2a\x00\x00\xe3\x38\x00\x00"
		.ascii  "\x1c\x47\x00\x00\x55\x55\x00\x00"
		.ascii  "\x8e\x63\x00\x00\xc7\x71\x00\x00"
		.ascii  "\x00\x80\x00\x00\x38\x8e\x00\x00"
		.ascii  "\x71\x9c\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\xe3\xb8\x00\x00\x1c\xc7\x00\x00"
		.ascii  "\x55\xd5\x00\x00\x8e\xe3\x00\x00"
		.ascii  "\xc7\xf1"
		.fill   0x4
		.ascii  "\x01\x00\x38\x0e\x01\x00\x71\x1c"
		.ascii  "\x01\x00\xaa\x2a\x01\x00\xe3\x38"
		.ascii  "\x01\x00\x1c\x47\x01\x00\x55\x55"
		.ascii  "\x01\x00\x8e\x63\x01\x00\xc7\x71"
		.ascii  "\x01\x00\x00\x80\x01\x00\x38\x8e"
		.ascii  "\x01\x00\x71\x9c\x01\x00\xaa\xaa"
		.ascii  "\x01\x00\xe3\xb8\x01\x00\xd8\x50"
		.ascii  "\xfe\xff\x51\x5e\xfe\xff\xcb\x6b"
		.ascii  "\xfe\xff\x44\x79\xfe\xff\xbd\x86"
		.ascii  "\xfe\xff\x36\x94\xfe\xff\xb0\xa1"
		.ascii  "\xfe\xff\x29\xaf\xfe\xff\xa2\xbc"
		.ascii  "\xfe\xff\x1b\xca\xfe\xff\x95\xd7"
		.ascii  "\xfe\xff\x0e\xe5\xfe\xff\x87\xf2"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x7a\x0d"
		.ascii  "\xff\xff\xf3\x1a\xff\xff\x6c\x28"
		.ascii  "\xff\xff\xe6\x35\xff\xff\x5f\x43"
		.ascii  "\xff\xff\xd8\x50\xff\xff\x51\x5e"
		.ascii  "\xff\xff\xcb\x6b\xff\xff\x44\x79"
		.ascii  "\xff\xff\xbd\x86\xff\xff\x36\x94"
		.ascii  "\xff\xff\xb0\xa1\xff\xff\x29\xaf"
		.ascii  "\xff\xff\xa2\xbc\xff\xff\x1b\xca"
		.ascii  "\xff\xff\x95\xd7\xff\xff\x0e\xe5"
		.ascii  "\xff\xff\x87\xf2\xff\xff"
		.fill   0x4
		.ascii  "\x79\x0d\x00\x00\xf2\x1a\x00\x00"
		.ascii  "\x6b\x28\x00\x00\xe5\x35\x00\x00"
		.ascii  "\x5e\x43\x00\x00\xd7\x50\x00\x00"
		.ascii  "\x50\x5e\x00\x00\xca\x6b\x00\x00"
		.ascii  "\x43\x79\x00\x00\xbc\x86\x00\x00"
		.ascii  "\x35\x94\x00\x00\xaf\xa1\x00\x00"
		.ascii  "\x28\xaf\x00\x00\xa1\xbc\x00\x00"
		.ascii  "\x1a\xca\x00\x00\x94\xd7\x00\x00"
		.ascii  "\x0d\xe5\x00\x00\x86\xf2"
		.fill   0x4
		.ascii  "\x01\x00\x79\x0d\x01\x00\xf2\x1a"
		.ascii  "\x01\x00\x6b\x28\x01\x00\xe5\x35"
		.ascii  "\x01\x00\x5e\x43\x01\x00\xd7\x50"
		.ascii  "\x01\x00\x50\x5e\x01\x00\xca\x6b"
		.ascii  "\x01\x00\x43\x79\x01\x00\xbc\x86"
		.ascii  "\x01\x00\x35\x94\x01\x00\xaf\xa1"
		.ascii  "\x01\x00\x67\x66\xfe\xff\x34\x73"
		.ascii  "\xfe\xff\x00\x80\xfe\xff\xcd\x8c"
		.ascii  "\xfe\xff\x9a\x99\xfe\xff\x67\xa6"
		.ascii  "\xfe\xff\x34\xb3\xfe\xff\x00\xc0"
		.ascii  "\xfe\xff\xcd\xcc\xfe\xff\x9a\xd9"
		.ascii  "\xfe\xff\x67\xe6\xfe\xff\x34\xf3"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xcd\x0c"
		.ascii  "\xff\xff\x9a\x19\xff\xff\x67\x26"
		.ascii  "\xff\xff\x34\x33\xff\xff\x00\x40"
		.ascii  "\xff\xff\xcd\x4c\xff\xff\x9a\x59"
		.ascii  "\xff\xff\x67\x66\xff\xff\x34\x73"
		.ascii  "\xff\xff\x00\x80\xff\xff\xcd\x8c"
		.ascii  "\xff\xff\x9a\x99\xff\xff\x67\xa6"
		.ascii  "\xff\xff\x34\xb3\xff\xff\x00\xc0"
		.ascii  "\xff\xff\xcd\xcc\xff\xff\x9a\xd9"
		.ascii  "\xff\xff\x67\xe6\xff\xff\x34\xf3"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\xcc\x0c\x00\x00\x99\x19\x00\x00"
		.ascii  "\x66\x26\x00\x00\x33\x33\x00\x00"
		.ascii  "\x00\x40\x00\x00\xcc\x4c\x00\x00"
		.ascii  "\x99\x59\x00\x00\x66\x66\x00\x00"
		.ascii  "\x33\x73\x00\x00\x00\x80\x00\x00"
		.ascii  "\xcc\x8c\x00\x00\x99\x99\x00\x00"
		.ascii  "\x66\xa6\x00\x00\x33\xb3\x00\x00"
		.ascii  "\x00\xc0\x00\x00\xcc\xcc\x00\x00"
		.ascii  "\x99\xd9\x00\x00\x66\xe6\x00\x00"
		.ascii  "\x33\xf3"
		.fill   0x4
		.ascii  "\x01\x00\xcc\x0c\x01\x00\x99\x19"
		.ascii  "\x01\x00\x66\x26\x01\x00\x33\x33"
		.ascii  "\x01\x00\x00\x40\x01\x00\xcc\x4c"
		.ascii  "\x01\x00\x99\x59\x01\x00\x66\x66"
		.ascii  "\x01\x00\x33\x73\x01\x00\x00\x80"
		.ascii  "\x01\x00\xcc\x8c\x01\x00\xe8\x79"
		.ascii  "\xfe\xff\x19\x86\xfe\xff\x4a\x92"
		.ascii  "\xfe\xff\x7a\x9e\xfe\xff\xab\xaa"
		.ascii  "\xfe\xff\xdc\xb6\xfe\xff\x0d\xc3"
		.ascii  "\xfe\xff\x3d\xcf\xfe\xff\x6e\xdb"
		.ascii  "\xfe\xff\x9f\xe7\xfe\xff\xd0\xf3"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x31\x0c"
		.ascii  "\xff\xff\x62\x18\xff\xff\x93\x24"
		.ascii  "\xff\xff\xc4\x30\xff\xff\xf4\x3c"
		.ascii  "\xff\xff\x25\x49\xff\xff\x56\x55"
		.ascii  "\xff\xff\x87\x61\xff\xff\xb7\x6d"
		.ascii  "\xff\xff\xe8\x79\xff\xff\x19\x86"
		.ascii  "\xff\xff\x4a\x92\xff\xff\x7a\x9e"
		.ascii  "\xff\xff\xab\xaa\xff\xff\xdc\xb6"
		.ascii  "\xff\xff\x0d\xc3\xff\xff\x3d\xcf"
		.ascii  "\xff\xff\x6e\xdb\xff\xff\x9f\xe7"
		.ascii  "\xff\xff\xd0\xf3\xff\xff"
		.fill   0x4
		.ascii  "\x30\x0c\x00\x00\x61\x18\x00\x00"
		.ascii  "\x92\x24\x00\x00\xc3\x30\x00\x00"
		.ascii  "\xf3\x3c\x00\x00\x24\x49\x00\x00"
		.ascii  "\x55\x55\x00\x00\x86\x61\x00\x00"
		.ascii  "\xb6\x6d\x00\x00\xe7\x79\x00\x00"
		.ascii  "\x18\x86\x00\x00\x49\x92\x00\x00"
		.ascii  "\x79\x9e\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\xdb\xb6\x00\x00\x0c\xc3\x00\x00"
		.ascii  "\x3c\xcf\x00\x00\x6d\xdb\x00\x00"
		.ascii  "\x9e\xe7\x00\x00\xcf\xf3"
		.fill   0x4
		.ascii  "\x01\x00\x30\x0c\x01\x00\x61\x18"
		.ascii  "\x01\x00\x92\x24\x01\x00\xc3\x30"
		.ascii  "\x01\x00\xf3\x3c\x01\x00\x24\x49"
		.ascii  "\x01\x00\x55\x55\x01\x00\x86\x61"
		.ascii  "\x01\x00\xb6\x6d\x01\x00\xe7\x79"
		.ascii  "\x01\x00\xa3\x8b\xfe\xff\x46\x97"
		.ascii  "\xfe\xff\xe9\xa2\xfe\xff\x8c\xae"
		.ascii  "\xfe\xff\x2f\xba\xfe\xff\xd2\xc5"
		.ascii  "\xfe\xff\x75\xd1\xfe\xff\x18\xdd"
		.ascii  "\xfe\xff\xbb\xe8\xfe\xff\x5e\xf4"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xa3\x0b"
		.ascii  "\xff\xff\x46\x17\xff\xff\xe9\x22"
		.ascii  "\xff\xff\x8c\x2e\xff\xff\x2f\x3a"
		.ascii  "\xff\xff\xd2\x45\xff\xff\x75\x51"
		.ascii  "\xff\xff\x18\x5d\xff\xff\xbb\x68"
		.ascii  "\xff\xff\x5e\x74\xff\xff\x00\x80"
		.ascii  "\xff\xff\xa3\x8b\xff\xff\x46\x97"
		.ascii  "\xff\xff\xe9\xa2\xff\xff\x8c\xae"
		.ascii  "\xff\xff\x2f\xba\xff\xff\xd2\xc5"
		.ascii  "\xff\xff\x75\xd1\xff\xff\x18\xdd"
		.ascii  "\xff\xff\xbb\xe8\xff\xff\x5e\xf4"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\xa2\x0b\x00\x00\x45\x17\x00\x00"
		.ascii  "\xe8\x22\x00\x00\x8b\x2e\x00\x00"
		.ascii  "\x2e\x3a\x00\x00\xd1\x45\x00\x00"
		.ascii  "\x74\x51\x00\x00\x17\x5d\x00\x00"
		.ascii  "\xba\x68\x00\x00\x5d\x74\x00\x00"
		.ascii  "\x00\x80\x00\x00\xa2\x8b\x00\x00"
		.ascii  "\x45\x97\x00\x00\xe8\xa2\x00\x00"
		.ascii  "\x8b\xae\x00\x00\x2e\xba\x00\x00"
		.ascii  "\xd1\xc5\x00\x00\x74\xd1\x00\x00"
		.ascii  "\x17\xdd\x00\x00\xba\xe8\x00\x00"
		.ascii  "\x5d\xf4"
		.fill   0x4
		.ascii  "\x01\x00\xa2\x0b\x01\x00\x45\x17"
		.ascii  "\x01\x00\xe8\x22\x01\x00\x8b\x2e"
		.ascii  "\x01\x00\x2e\x3a\x01\x00\xd1\x45"
		.ascii  "\x01\x00\x74\x51\x01\x00\x17\x5d"
		.ascii  "\x01\x00\xba\x68\x01\x00\xd4\x9b"
		.ascii  "\xfe\xff\xf5\xa6\xfe\xff\x17\xb2"
		.ascii  "\xfe\xff\x38\xbd\xfe\xff\x5a\xc8"
		.ascii  "\xfe\xff\x7b\xd3\xfe\xff\x9c\xde"
		.ascii  "\xfe\xff\xbe\xe9\xfe\xff\xdf\xf4"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x22\x0b"
		.ascii  "\xff\xff\x43\x16\xff\xff\x65\x21"
		.ascii  "\xff\xff\x86\x2c\xff\xff\xa7\x37"
		.ascii  "\xff\xff\xc9\x42\xff\xff\xea\x4d"
		.ascii  "\xff\xff\x0c\x59\xff\xff\x2d\x64"
		.ascii  "\xff\xff\x4e\x6f\xff\xff\x70\x7a"
		.ascii  "\xff\xff\x91\x85\xff\xff\xb3\x90"
		.ascii  "\xff\xff\xd4\x9b\xff\xff\xf5\xa6"
		.ascii  "\xff\xff\x17\xb2\xff\xff\x38\xbd"
		.ascii  "\xff\xff\x5a\xc8\xff\xff\x7b\xd3"
		.ascii  "\xff\xff\x9c\xde\xff\xff\xbe\xe9"
		.ascii  "\xff\xff\xdf\xf4\xff\xff"
		.fill   0x4
		.ascii  "\x21\x0b\x00\x00\x42\x16\x00\x00"
		.ascii  "\x64\x21\x00\x00\x85\x2c\x00\x00"
		.ascii  "\xa6\x37\x00\x00\xc8\x42\x00\x00"
		.ascii  "\xe9\x4d\x00\x00\x0b\x59\x00\x00"
		.ascii  "\x2c\x64\x00\x00\x4d\x6f\x00\x00"
		.ascii  "\x6f\x7a\x00\x00\x90\x85\x00\x00"
		.ascii  "\xb2\x90\x00\x00\xd3\x9b\x00\x00"
		.ascii  "\xf4\xa6\x00\x00\x16\xb2\x00\x00"
		.ascii  "\x37\xbd\x00\x00\x59\xc8\x00\x00"
		.ascii  "\x7a\xd3\x00\x00\x9b\xde\x00\x00"
		.ascii  "\xbd\xe9\x00\x00\xde\xf4"
		.fill   0x4
		.ascii  "\x01\x00\x21\x0b\x01\x00\x42\x16"
		.ascii  "\x01\x00\x64\x21\x01\x00\x85\x2c"
		.ascii  "\x01\x00\xa6\x37\x01\x00\xc8\x42"
		.ascii  "\x01\x00\xe9\x4d\x01\x00\x0b\x59"
		.ascii  "\x01\x00\xab\xaa\xfe\xff\x56\xb5"
		.ascii  "\xfe\xff\x00\xc0\xfe\xff\xab\xca"
		.ascii  "\xfe\xff\x56\xd5\xfe\xff\x00\xe0"
		.ascii  "\xfe\xff\xab\xea\xfe\xff\x56\xf5"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xab\x0a"
		.ascii  "\xff\xff\x56\x15\xff\xff\x00\x20"
		.ascii  "\xff\xff\xab\x2a\xff\xff\x56\x35"
		.ascii  "\xff\xff\x00\x40\xff\xff\xab\x4a"
		.ascii  "\xff\xff\x56\x55\xff\xff\x00\x60"
		.ascii  "\xff\xff\xab\x6a\xff\xff\x56\x75"
		.ascii  "\xff\xff\x00\x80\xff\xff\xab\x8a"
		.ascii  "\xff\xff\x56\x95\xff\xff\x00\xa0"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x56\xb5"
		.ascii  "\xff\xff\x00\xc0\xff\xff\xab\xca"
		.ascii  "\xff\xff\x56\xd5\xff\xff\x00\xe0"
		.ascii  "\xff\xff\xab\xea\xff\xff\x56\xf5"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\xaa\x0a\x00\x00\x55\x15\x00\x00"
		.ascii  "\x00\x20\x00\x00\xaa\x2a\x00\x00"
		.ascii  "\x55\x35\x00\x00\x00\x40\x00\x00"
		.ascii  "\xaa\x4a\x00\x00\x55\x55\x00\x00"
		.ascii  "\x00\x60\x00\x00\xaa\x6a\x00\x00"
		.ascii  "\x55\x75\x00\x00\x00\x80\x00\x00"
		.ascii  "\xaa\x8a\x00\x00\x55\x95\x00\x00"
		.ascii  "\x00\xa0\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x55\xb5\x00\x00\x00\xc0\x00\x00"
		.ascii  "\xaa\xca\x00\x00\x55\xd5\x00\x00"
		.ascii  "\x00\xe0\x00\x00\xaa\xea\x00\x00"
		.ascii  "\x55\xf5"
		.fill   0x4
		.ascii  "\x01\x00\xaa\x0a\x01\x00\x55\x15"
		.ascii  "\x01\x00\x00\x20\x01\x00\xaa\x2a"
		.ascii  "\x01\x00\x55\x35\x01\x00\x00\x40"
		.ascii  "\x01\x00\xaa\x4a\x01\x00\x52\xb8"
		.ascii  "\xfe\xff\x90\xc2\xfe\xff\xcd\xcc"
		.ascii  "\xfe\xff\x0b\xd7\xfe\xff\x48\xe1"
		.ascii  "\xfe\xff\x86\xeb\xfe\xff\xc3\xf5"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x3e\x0a"
		.ascii  "\xff\xff\x7b\x14\xff\xff\xb9\x1e"
		.ascii  "\xff\xff\xf6\x28\xff\xff\x34\x33"
		.ascii  "\xff\xff\x71\x3d\xff\xff\xaf\x47"
		.ascii  "\xff\xff\xec\x51\xff\xff\x29\x5c"
		.ascii  "\xff\xff\x67\x66\xff\xff\xa4\x70"
		.ascii  "\xff\xff\xe2\x7a\xff\xff\x1f\x85"
		.ascii  "\xff\xff\x5d\x8f\xff\xff\x9a\x99"
		.ascii  "\xff\xff\xd8\xa3\xff\xff\x15\xae"
		.ascii  "\xff\xff\x52\xb8\xff\xff\x90\xc2"
		.ascii  "\xff\xff\xcd\xcc\xff\xff\x0b\xd7"
		.ascii  "\xff\xff\x48\xe1\xff\xff\x86\xeb"
		.ascii  "\xff\xff\xc3\xf5\xff\xff"
		.fill   0x4
		.ascii  "\x3d\x0a\x00\x00\x7a\x14\x00\x00"
		.ascii  "\xb8\x1e\x00\x00\xf5\x28\x00\x00"
		.ascii  "\x33\x33\x00\x00\x70\x3d\x00\x00"
		.ascii  "\xae\x47\x00\x00\xeb\x51\x00\x00"
		.ascii  "\x28\x5c\x00\x00\x66\x66\x00\x00"
		.ascii  "\xa3\x70\x00\x00\xe1\x7a\x00\x00"
		.ascii  "\x1e\x85\x00\x00\x5c\x8f\x00\x00"
		.ascii  "\x99\x99\x00\x00\xd7\xa3\x00\x00"
		.ascii  "\x14\xae\x00\x00\x51\xb8\x00\x00"
		.ascii  "\x8f\xc2\x00\x00\xcc\xcc\x00\x00"
		.ascii  "\x0a\xd7\x00\x00\x47\xe1\x00\x00"
		.ascii  "\x85\xeb\x00\x00\xc2\xf5"
		.fill   0x4
		.ascii  "\x01\x00\x3d\x0a\x01\x00\x7a\x14"
		.ascii  "\x01\x00\xb8\x1e\x01\x00\xf5\x28"
		.ascii  "\x01\x00\x33\x33\x01\x00\x70\x3d"
		.ascii  "\x01\x00\xed\xc4\xfe\xff\xc5\xce"
		.ascii  "\xfe\xff\x9e\xd8\xfe\xff\x77\xe2"
		.ascii  "\xfe\xff\x4f\xec\xfe\xff\x28\xf6"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xd9\x09"
		.ascii  "\xff\xff\xb2\x13\xff\xff\x8a\x1d"
		.ascii  "\xff\xff\x63\x27\xff\xff\x3c\x31"
		.ascii  "\xff\xff\x14\x3b\xff\xff\xed\x44"
		.ascii  "\xff\xff\xc5\x4e\xff\xff\x9e\x58"
		.ascii  "\xff\xff\x77\x62\xff\xff\x4f\x6c"
		.ascii  "\xff\xff\x28\x76\xff\xff\x00\x80"
		.ascii  "\xff\xff\xd9\x89\xff\xff\xb2\x93"
		.ascii  "\xff\xff\x8a\x9d\xff\xff\x63\xa7"
		.ascii  "\xff\xff\x3c\xb1\xff\xff\x14\xbb"
		.ascii  "\xff\xff\xed\xc4\xff\xff\xc5\xce"
		.ascii  "\xff\xff\x9e\xd8\xff\xff\x77\xe2"
		.ascii  "\xff\xff\x4f\xec\xff\xff\x28\xf6"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\xd8\x09\x00\x00\xb1\x13\x00\x00"
		.ascii  "\x89\x1d\x00\x00\x62\x27\x00\x00"
		.ascii  "\x3b\x31\x00\x00\x13\x3b\x00\x00"
		.ascii  "\xec\x44\x00\x00\xc4\x4e\x00\x00"
		.ascii  "\x9d\x58\x00\x00\x76\x62\x00\x00"
		.ascii  "\x4e\x6c\x00\x00\x27\x76\x00\x00"
		.ascii  "\x00\x80\x00\x00\xd8\x89\x00\x00"
		.ascii  "\xb1\x93\x00\x00\x89\x9d\x00\x00"
		.ascii  "\x62\xa7\x00\x00\x3b\xb1\x00\x00"
		.ascii  "\x13\xbb\x00\x00\xec\xc4\x00\x00"
		.ascii  "\xc4\xce\x00\x00\x9d\xd8\x00\x00"
		.ascii  "\x76\xe2\x00\x00\x4e\xec\x00\x00"
		.ascii  "\x27\xf6"
		.fill   0x4
		.ascii  "\x01\x00\xd8\x09\x01\x00\xb1\x13"
		.ascii  "\x01\x00\x89\x1d\x01\x00\x62\x27"
		.ascii  "\x01\x00\x3b\x31\x01\x00\x98\xd0"
		.ascii  "\xfe\xff\x13\xda\xfe\xff\x8f\xe3"
		.ascii  "\xfe\xff\x0a\xed\xfe\xff\x85\xf6"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x7c\x09"
		.ascii  "\xff\xff\xf7\x12\xff\xff\x72\x1c"
		.ascii  "\xff\xff\xee\x25\xff\xff\x69\x2f"
		.ascii  "\xff\xff\xe4\x38\xff\xff\x5f\x42"
		.ascii  "\xff\xff\xdb\x4b\xff\xff\x56\x55"
		.ascii  "\xff\xff\xd1\x5e\xff\xff\x4c\x68"
		.ascii  "\xff\xff\xc8\x71\xff\xff\x43\x7b"
		.ascii  "\xff\xff\xbe\x84\xff\xff\x39\x8e"
		.ascii  "\xff\xff\xb5\x97\xff\xff\x30\xa1"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x26\xb4"
		.ascii  "\xff\xff\xa2\xbd\xff\xff\x1d\xc7"
		.ascii  "\xff\xff\x98\xd0\xff\xff\x13\xda"
		.ascii  "\xff\xff\x8f\xe3\xff\xff\x0a\xed"
		.ascii  "\xff\xff\x85\xf6\xff\xff"
		.fill   0x4
		.ascii  "\x7b\x09\x00\x00\xf6\x12\x00\x00"
		.ascii  "\x71\x1c\x00\x00\xed\x25\x00\x00"
		.ascii  "\x68\x2f\x00\x00\xe3\x38\x00\x00"
		.ascii  "\x5e\x42\x00\x00\xda\x4b\x00\x00"
		.ascii  "\x55\x55\x00\x00\xd0\x5e\x00\x00"
		.ascii  "\x4b\x68\x00\x00\xc7\x71\x00\x00"
		.ascii  "\x42\x7b\x00\x00\xbd\x84\x00\x00"
		.ascii  "\x38\x8e\x00\x00\xb4\x97\x00\x00"
		.ascii  "\x2f\xa1\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x25\xb4\x00\x00\xa1\xbd\x00\x00"
		.ascii  "\x1c\xc7\x00\x00\x97\xd0\x00\x00"
		.ascii  "\x12\xda\x00\x00\x8e\xe3\x00\x00"
		.ascii  "\x09\xed\x00\x00\x84\xf6"
		.fill   0x4
		.ascii  "\x01\x00\x7b\x09\x01\x00\xf6\x12"
		.ascii  "\x01\x00\x71\x1c\x01\x00\xed\x25"
		.ascii  "\x01\x00\x6e\xdb\xfe\xff\x93\xe4"
		.ascii  "\xfe\xff\xb7\xed\xfe\xff\xdc\xf6"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x25\x09"
		.ascii  "\xff\xff\x4a\x12\xff\xff\x6e\x1b"
		.ascii  "\xff\xff\x93\x24\xff\xff\xb7\x2d"
		.ascii  "\xff\xff\xdc\x36\xff\xff\x00\x40"
		.ascii  "\xff\xff\x25\x49\xff\xff\x4a\x52"
		.ascii  "\xff\xff\x6e\x5b\xff\xff\x93\x64"
		.ascii  "\xff\xff\xb7\x6d\xff\xff\xdc\x76"
		.ascii  "\xff\xff\x00\x80\xff\xff\x25\x89"
		.ascii  "\xff\xff\x4a\x92\xff\xff\x6e\x9b"
		.ascii  "\xff\xff\x93\xa4\xff\xff\xb7\xad"
		.ascii  "\xff\xff\xdc\xb6\xff\xff\x00\xc0"
		.ascii  "\xff\xff\x25\xc9\xff\xff\x4a\xd2"
		.ascii  "\xff\xff\x6e\xdb\xff\xff\x93\xe4"
		.ascii  "\xff\xff\xb7\xed\xff\xff\xdc\xf6"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x24\x09\x00\x00\x49\x12\x00\x00"
		.ascii  "\x6d\x1b\x00\x00\x92\x24\x00\x00"
		.ascii  "\xb6\x2d\x00\x00\xdb\x36\x00\x00"
		.ascii  "\x00\x40\x00\x00\x24\x49\x00\x00"
		.ascii  "\x49\x52\x00\x00\x6d\x5b\x00\x00"
		.ascii  "\x92\x64\x00\x00\xb6\x6d\x00\x00"
		.ascii  "\xdb\x76\x00\x00\x00\x80\x00\x00"
		.ascii  "\x24\x89\x00\x00\x49\x92\x00\x00"
		.ascii  "\x6d\x9b\x00\x00\x92\xa4\x00\x00"
		.ascii  "\xb6\xad\x00\x00\xdb\xb6\x00\x00"
		.ascii  "\x00\xc0\x00\x00\x24\xc9\x00\x00"
		.ascii  "\x49\xd2\x00\x00\x6d\xdb\x00\x00"
		.ascii  "\x92\xe4\x00\x00\xb6\xed\x00\x00"
		.ascii  "\xdb\xf6"
		.fill   0x4
		.ascii  "\x01\x00\x24\x09\x01\x00\x49\x12"
		.ascii  "\x01\x00\x6d\x1b\x01\x00\x85\xe5"
		.ascii  "\xfe\xff\x59\xee\xfe\xff\x2d\xf7"
		.ascii  "\xfe\xff\x00\x00\xff\xff\xd4\x08"
		.ascii  "\xff\xff\xa8\x11\xff\xff\x7c\x1a"
		.ascii  "\xff\xff\x50\x23\xff\xff\x24\x2c"
		.ascii  "\xff\xff\xf8\x34\xff\xff\xcc\x3d"
		.ascii  "\xff\xff\x9f\x46\xff\xff\x73\x4f"
		.ascii  "\xff\xff\x47\x58\xff\xff\x1b\x61"
		.ascii  "\xff\xff\xef\x69\xff\xff\xc3\x72"
		.ascii  "\xff\xff\x97\x7b\xff\xff\x6a\x84"
		.ascii  "\xff\xff\x3e\x8d\xff\xff\x12\x96"
		.ascii  "\xff\xff\xe6\x9e\xff\xff\xba\xa7"
		.ascii  "\xff\xff\x8e\xb0\xff\xff\x62\xb9"
		.ascii  "\xff\xff\x35\xc2\xff\xff\x09\xcb"
		.ascii  "\xff\xff\xdd\xd3\xff\xff\xb1\xdc"
		.ascii  "\xff\xff\x85\xe5\xff\xff\x59\xee"
		.ascii  "\xff\xff\x2d\xf7\xff\xff"
		.fill   0x4
		.ascii  "\xd3\x08\x00\x00\xa7\x11\x00\x00"
		.ascii  "\x7b\x1a\x00\x00\x4f\x23\x00\x00"
		.ascii  "\x23\x2c\x00\x00\xf7\x34\x00\x00"
		.ascii  "\xcb\x3d\x00\x00\x9e\x46\x00\x00"
		.ascii  "\x72\x4f\x00\x00\x46\x58\x00\x00"
		.ascii  "\x1a\x61\x00\x00\xee\x69\x00\x00"
		.ascii  "\xc2\x72\x00\x00\x96\x7b\x00\x00"
		.ascii  "\x69\x84\x00\x00\x3d\x8d\x00\x00"
		.ascii  "\x11\x96\x00\x00\xe5\x9e\x00\x00"
		.ascii  "\xb9\xa7\x00\x00\x8d\xb0\x00\x00"
		.ascii  "\x61\xb9\x00\x00\x34\xc2\x00\x00"
		.ascii  "\x08\xcb\x00\x00\xdc\xd3\x00\x00"
		.ascii  "\xb0\xdc\x00\x00\x84\xe5\x00\x00"
		.ascii  "\x58\xee\x00\x00\x2c\xf7"
		.fill   0x4
		.ascii  "\x01\x00\xd3\x08\x01\x00\xa7\x11"
		.ascii  "\x01\x00\xef\xee\xfe\xff\x78\xf7"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x89\x08"
		.ascii  "\xff\xff\x12\x11\xff\xff\x9a\x19"
		.ascii  "\xff\xff\x23\x22\xff\xff\xab\x2a"
		.ascii  "\xff\xff\x34\x33\xff\xff\xbc\x3b"
		.ascii  "\xff\xff\x45\x44\xff\xff\xcd\x4c"
		.ascii  "\xff\xff\x56\x55\xff\xff\xde\x5d"
		.ascii  "\xff\xff\x67\x66\xff\xff\xef\x6e"
		.ascii  "\xff\xff\x78\x77\xff\xff\x00\x80"
		.ascii  "\xff\xff\x89\x88\xff\xff\x12\x91"
		.ascii  "\xff\xff\x9a\x99\xff\xff\x23\xa2"
		.ascii  "\xff\xff\xab\xaa\xff\xff\x34\xb3"
		.ascii  "\xff\xff\xbc\xbb\xff\xff\x45\xc4"
		.ascii  "\xff\xff\xcd\xcc\xff\xff\x56\xd5"
		.ascii  "\xff\xff\xde\xdd\xff\xff\x67\xe6"
		.ascii  "\xff\xff\xef\xee\xff\xff\x78\xf7"
		.ascii  "\xff\xff"
		.fill   0x4
		.ascii  "\x88\x08\x00\x00\x11\x11\x00\x00"
		.ascii  "\x99\x19\x00\x00\x22\x22\x00\x00"
		.ascii  "\xaa\x2a\x00\x00\x33\x33\x00\x00"
		.ascii  "\xbb\x3b\x00\x00\x44\x44\x00\x00"
		.ascii  "\xcc\x4c\x00\x00\x55\x55\x00\x00"
		.ascii  "\xdd\x5d\x00\x00\x66\x66\x00\x00"
		.ascii  "\xee\x6e\x00\x00\x77\x77\x00\x00"
		.ascii  "\x00\x80\x00\x00\x88\x88\x00\x00"
		.ascii  "\x11\x91\x00\x00\x99\x99\x00\x00"
		.ascii  "\x22\xa2\x00\x00\xaa\xaa\x00\x00"
		.ascii  "\x33\xb3\x00\x00\xbb\xbb\x00\x00"
		.ascii  "\x44\xc4\x00\x00\xcc\xcc\x00\x00"
		.ascii  "\x55\xd5\x00\x00\xdd\xdd\x00\x00"
		.ascii  "\x66\xe6\x00\x00\xee\xee\x00\x00"
		.ascii  "\x77\xf7"
		.fill   0x4
		.ascii  "\x01\x00\x88\x08\x01\x00\xbe\xf7"
		.ascii  "\xfe\xff\x00\x00\xff\xff\x43\x08"
		.ascii  "\xff\xff\x85\x10\xff\xff\xc7\x18"
		.ascii  "\xff\xff\x09\x21\xff\xff\x4b\x29"
		.ascii  "\xff\xff\x8d\x31\xff\xff\xcf\x39"
		.ascii  "\xff\xff\x11\x42\xff\xff\x53\x4a"
		.ascii  "\xff\xff\x95\x52\xff\xff\xd7\x5a"
		.ascii  "\xff\xff\x19\x63\xff\xff\x5b\x6b"
		.ascii  "\xff\xff\x9d\x73\xff\xff\xdf\x7b"
		.ascii  "\xff\xff\x22\x84\xff\xff\x64\x8c"
		.ascii  "\xff\xff\xa6\x94\xff\xff\xe8\x9c"
		.ascii  "\xff\xff\x2a\xa5\xff\xff\x6c\xad"
		.ascii  "\xff\xff\xae\xb5\xff\xff\xf0\xbd"
		.ascii  "\xff\xff\x32\xc6\xff\xff\x74\xce"
		.ascii  "\xff\xff\xb6\xd6\xff\xff\xf8\xde"
		.ascii  "\xff\xff\x3a\xe7\xff\xff\x7c\xef"
		.ascii  "\xff\xff\xbe\xf7\xff\xff"
		.fill   0x4
		.ascii  "\x42\x08\x00\x00\x84\x10\x00\x00"
		.ascii  "\xc6\x18\x00\x00\x08\x21\x00\x00"
		.ascii  "\x4a\x29\x00\x00\x8c\x31\x00\x00"
		.ascii  "\xce\x39\x00\x00\x10\x42\x00\x00"
		.ascii  "\x52\x4a\x00\x00\x94\x52\x00\x00"
		.ascii  "\xd6\x5a\x00\x00\x18\x63\x00\x00"
		.ascii  "\x5a\x6b\x00\x00\x9c\x73\x00\x00"
		.ascii  "\xde\x7b\x00\x00\x21\x84\x00\x00"
		.ascii  "\x63\x8c\x00\x00\xa5\x94\x00\x00"
		.ascii  "\xe7\x9c\x00\x00\x29\xa5\x00\x00"
		.ascii  "\x6b\xad\x00\x00\xad\xb5\x00\x00"
		.ascii  "\xef\xbd\x00\x00\x31\xc6\x00\x00"
		.ascii  "\x73\xce\x00\x00\xb5\xd6\x00\x00"
		.ascii  "\xf7\xde\x00\x00\x39\xe7\x00\x00"
		.ascii  "\x7b\xef\x00\x00\xbd\xf7"
		.fill   0x4
		.ascii  "\x01\x00"
.text


	jump_12d820:
		add    $0x190,%esp
		popa
		ret
