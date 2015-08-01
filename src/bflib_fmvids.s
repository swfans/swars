
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
func_139100:
/*----------------------------------------------------------------*/
		mov    $0x80000000,%eax
		push   $0xffffffff
		push   $0xfffffffe
	jump_139109:
		decb   data_1598c0
		jne    jump_13911d
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13911d:
		shr    $0x1,%ebp
		jae    jump_139137
		pop    %ecx
		cmp    $0xfffffffe,%ecx
		je     jump_13912f
		push   %ecx
		push   %edi
		add    $0x4,%edi
		jmp    jump_139109
	jump_13912f:
		push   $0xfffffffe
		push   %edi
		add    $0x4,%edi
		jmp    jump_139109
	jump_139137:
		pop    %ecx
		cmp    $0xffffffff,%ecx
		je     jump_139193
		push   %ecx
		cmpb   $0x9,data_1598c0
		jb     jump_139157
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13917d
	jump_139157:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13917d:
		pop    %ecx
		mov    %eax,(%edi)
		add    $0x4,%edi
		cmp    $0xfffffffe,%ecx
		je     jump_139109
		mov    %edi,%edx
		sub    %ecx,%edx
		mov    %edx,(%ecx)
		jmp    jump_139109
	jump_139193:
		ret


/*----------------------------------------------------------------*/
func_1391a0:
/*----------------------------------------------------------------*/
		cmpb   $0x9,data_1598c0
		jb     jump_1391b9
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_1391df
	jump_1391b9:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_1391df:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_1391fa
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_139220
	jump_1391fa:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_139220:
		xchg   %ah,%al
		mov    %ax,data_1599e0
		cmpb   $0x9,data_1598c0
		jb     jump_139241
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_139267
	jump_139241:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_139267:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_139282
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_1392a8
	jump_139282:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_1392a8:
		xchg   %ah,%al
		mov    %ax,data_1599e2
		cmpb   $0x9,data_1598c0
		jb     jump_1392c9
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_1392ef
	jump_1392c9:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_1392ef:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_13930a
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_139330
	jump_13930a:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_139330:
		xchg   %ah,%al
		mov    %ax,data_1599e4
		movl   $0x0,(%edx)
		movl   $0x0,0x4(%edx)
		movl   $0x0,0x8(%edx)
		movl   $0x0,0xc(%edx)
		push   $0xffffffff
		push   $0xfffffffe
	jump_139357:
		decb   data_1598c0
		jne    jump_13936b
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13936b:
		shr    $0x1,%ebp
		jae    jump_139385
		pop    %ecx
		cmp    $0xfffffffe,%ecx
		je     jump_13937d
		push   %ecx
		push   %edi
		add    $0x4,%edi
		jmp    jump_139357
	jump_13937d:
		push   $0xfffffffe
		push   %edi
		add    $0x4,%edi
		jmp    jump_139357
	jump_139385:
		pop    %ecx
		cmp    $0xffffffff,%ecx
		je     jump_139462
		push   %edx
		push   %ecx
		mov    data_15988c,%edx
		mov    (%edx),%ecx
		test   $0x80000000,%ecx
		jne    jump_1393cb
	jump_1393a1:
		decb   data_1598c0
		jne    jump_1393b5
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_1393b5:
		shr    $0x1,%ebp
		jb     jump_1393bf
		mov    $0x4,%ecx
	jump_1393bf:
		add    %ecx,%edx
		mov    (%edx),%ecx
		test   $0x80000000,%ecx
		je     jump_1393a1
	jump_1393cb:
		mov    %cx,%ax
		shl    $0x18,%eax
		mov    data_159890,%edx
		mov    (%edx),%ecx
		test   $0x80000000,%ecx
		jne    jump_13940b
	jump_1393e1:
		decb   data_1598c0
		jne    jump_1393f5
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_1393f5:
		shr    $0x1,%ebp
		jb     jump_1393ff
		mov    $0x4,%ecx
	jump_1393ff:
		add    %ecx,%edx
		mov    (%edx),%ecx
		test   $0x80000000,%ecx
		je     jump_1393e1
	jump_13940b:
		mov    %cx,%ax
		pop    %ecx
		pop    %edx
		rol    $0x8,%eax
		cmp    %ax,data_1599e0
		jne    jump_139423
		xor    %eax,%eax
		mov    %edi,0x4(%edx)
		jmp    jump_139441
	jump_139423:
		cmp    %ax,data_1599e2
		jne    jump_139433
		xor    %eax,%eax
		mov    %edi,0x8(%edx)
		jmp    jump_139441
	jump_139433:
		cmp    %ax,data_1599e4
		jne    jump_139441
		xor    %eax,%eax
		mov    %edi,0xc(%edx)
	jump_139441:
		shl    $0x10,%eax
		mov    $0x1,%al
		mov    %eax,(%edi)
		add    $0x4,%edi
		cmp    $0xfffffffe,%ecx
		je     jump_139357
		mov    %edi,%eax
		sub    %ecx,%eax
		shl    $0xd,%eax
		mov    %eax,(%ecx)
		jmp    jump_139357
	jump_139462:
		cmpl   $0x0,0x4(%edx)
		jne    jump_13946b
		mov    %edi,0x4(%edx)
	jump_13946b:
		cmpl   $0x0,0x8(%edx)
		jne    jump_139477
		add    $0x4,%edi
		mov    %edi,0x8(%edx)
	jump_139477:
		cmpl   $0x0,0xc(%edx)
		jne    jump_139483
		add    $0x4,%edi
		mov    %edi,0xc(%edx)
	jump_139483:
		ret


/*----------------------------------------------------------------*/
func_139490:
/*----------------------------------------------------------------*/
		mov    %edx,data_159878
		push   %ebp
		mov    data_1598b4,%ebp
	jump_13949d:
		push   %ebp
		mov    %bp,%ax
		dec    %ax
		mov    data_1598b0,%cl
		mov    data_159878,%edx
		mov    (%edx),%ebp
		test   $0xffff,%ebp
		jne    jump_1394df
	jump_1394b9:
		shr    $0xd,%ebp
		and    $0xffff8,%ebp
		test   $0xff,%cl
		je     jump_1394df
		dec    %cl
		shr    %ax
		jb     jump_1394d3
		mov    $0x4,%ebp
	jump_1394d3:
		add    %ebp,%edx
		mov    (%edx),%ebp
		test   $0xffff,%ebp
		je     jump_1394b9
	jump_1394df:
		pop    %ebp
		sub    data_1598a4,%edx
		shl    $0x8,%edx
		sub    data_1598b0,%cl
		neg    %cl
		mov    %cl,%dl
		mov    %edx,-0x4(%edi,%ebp,4)
		dec    %ebp
		jne    jump_13949d
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
func_139500:
/*----------------------------------------------------------------*/
		decb   data_1598c0
		jne    jump_139514
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_139514:
		shr    $0x1,%ebp
		jb     jump_13952a
		movl   $0x0,(%edx)
		mov    %eax,0x4(%edx)
		mov    %eax,0x8(%edx)
		mov    %eax,0xc(%edx)
		jmp    jump_139584
	jump_13952a:
		push   %edi
		push   %eax
		push   %edx
		push   %eax
		decb   data_1598c0
		jne    jump_139542
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_139542:
		shr    $0x1,%ebp
		jae    jump_139552
		mov    data_15988c,%edi
		call   func_139100
	jump_139552:
		decb   data_1598c0
		jne    jump_139566
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_139566:
		shr    $0x1,%ebp
		jae    jump_139576
		mov    data_159890,%edi
		call   func_139100
	jump_139576:
		pop    %edi
		pop    %edx
		call   func_1391a0
		pop    %edx
		pop    %edi
		call   func_139490
	jump_139584:
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (func_139590)
/*----------------------------------------------------------------*/
		mov    $0x7468,%eax
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (func_1395a0)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		push   %es
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		push   %esi
		mov    %ds,%ax
		mov    %eax,%es
		cld
		mov    0x8(%ebp),%eax
		mov    %eax,data_159e00
		mov    0xc(%ebp),%eax
		mov    %eax,data_159e04
		mov    0x10(%ebp),%eax
		mov    %eax,data_159e08
		mov    0x14(%ebp),%eax
		mov    %eax,data_159e0c
		mov    0x18(%ebp),%eax
		mov    %eax,data_159e10
		mov    0x1c(%ebp),%eax
		mov    %eax,data_159e14
		mov    data_159e00,%esi
		mov    %esi,data_15988c
		add    $0x800,%esi
		mov    %esi,data_159890
		add    $0x800,%esi
		xor    %ebp,%ebp
		movb   $0x1,data_1598c0
		movl   $0xb,data_1598b0
		movl   $0x800,data_1598b4
		mov    $data_159810,%edx
		mov    data_159e04,%edi
		lea    0x7468(%edi),%eax
		mov    %edi,data_1598a4
		add    $0x10,%edi
		call   func_139500
		movl   $0xa,data_1598b0
		movl   $0x400,data_1598b4
		mov    $data_159820,%edx
		mov    data_159e04,%edi
		lea    0x7468(%edi),%eax
		add    $0x2010,%edi
		add    data_159e08,%eax
		call   func_139500
		movl   $0xc,data_1598b0
		movl   $0x1000,data_1598b4
		mov    $data_159830,%edx
		mov    data_159e04,%edi
		lea    0x7468(%edi),%eax
		add    $0x3010,%edi
		add    data_159e08,%eax
		add    data_159e0c,%eax
		call   func_139500
		movl   $0x8,data_1598b0
		movl   $0x100,data_1598b4
		mov    $data_159840,%edx
		mov    data_159e04,%edi
		lea    0x7468(%edi),%eax
		add    $0x7010,%edi
		add    data_159e08,%eax
		add    data_159e0c,%eax
		add    data_159e10,%eax
		call   func_139500
		mov    data_159e04,%edi
		mov    data_159e08,%eax
		mov    %eax,(%edi)
		mov    data_159e0c,%eax
		mov    %eax,0x4(%edi)
		mov    data_159e10,%eax
		mov    %eax,0x8(%edi)
		mov    data_159e14,%eax
		mov    %eax,0xc(%edi)
		add    $0x7414,%edi
		mov    $data_159810,%esi
		mov    $0x10,%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		mov    data_159e00,%edi
		xor    %eax,%eax
		mov    $0x800,%ecx
		rep stos %eax,%es:(%edi)
		mov    data_159e00,%edi
		mov    data_159e04,%esi
		mov    (%esi),%eax
		call   func_1398e0
		mov    data_159e04,%esi
		mov    0x4(%esi),%eax
		add    (%esi),%esi
		call   func_1398e0
		mov    data_159e04,%esi
		mov    0x4(%esi),%ecx
		mov    0x8(%esi),%eax
		add    (%esi),%esi
		add    %ecx,%esi
		call   func_1398e0
		mov    data_159e04,%esi
		mov    0x4(%esi),%edx
		mov    0x8(%esi),%ecx
		mov    0xc(%esi),%eax
		add    (%esi),%esi
		add    %edx,%esi
		add    %ecx,%esi
		call   func_1398e0
		mov    $0x800,%ecx
		mov    $0xffffffff,%eax
		repz scas %es:(%edi),%eax
		sub    $0x4,%edi
		mov    (%edi),%eax
		sub    data_159e00,%edi
		xor    $0xffffffff,%eax
		shl    $0x3,%edi
		bsf    %eax,%edx
		add    %edx,%edi
		mov    %di,data_1598d0
		mov    data_159e04,%esi
		add    $0x7410,%esi
		mov    %di,(%esi)
		mov    data_159e04,%esi
		mov    (%esi),%eax
		call   func_139910
		mov    data_159e04,%esi
		mov    0x4(%esi),%eax
		add    (%esi),%esi
		call   func_139910
		mov    data_159e04,%esi
		mov    0x4(%esi),%ecx
		mov    0x8(%esi),%eax
		add    (%esi),%esi
		add    %ecx,%esi
		call   func_139910
		mov    data_159e04,%esi
		mov    0x4(%esi),%edx
		mov    0x8(%esi),%ecx
		mov    0xc(%esi),%eax
		add    (%esi),%esi
		add    %edx,%esi
		add    %ecx,%esi
		call   func_139910
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %es
		pop    %ebp
		ret

/*----------------------------------------------------------------*/
	jump_139820:
		push   %esi
		push   %ecx
		push   %edx
		mov    %edi,%edx
		mov    0x2(%edx),%esi
		mov    0x6(%edx),%edi
		movzwl 0xa(%edx),%ebx
		cmp    $0x0,%ebx
		je     jump_139845
	jump_139834:
		mov    data_159874,%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		add    data_159880,%edi
		dec    %ebx
		jne    jump_139834
	jump_139845:
		movzwl 0xc(%edx),%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		mov    0xe(%edx),%ax
		call   *data_15989c
		sub    $0x10000,%edi
		movzwl 0x10(%edx),%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		add    data_159880,%edi
		movzwl 0x12(%edx),%ebx
		cmp    $0x0,%ebx
		je     jump_139881
	jump_139870:
		mov    data_159874,%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		add    data_159880,%edi
		dec    %ebx
		jne    jump_139870
	jump_139881:
		xchg   %edx,%edi
		add    $0x14,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl (%edi),%eax
		add    $0x2,%edi
		mov    %eax,data_159870
		mov    %edi,data_1598a0
		mov    %edx,%edi
		pop    %edx
		pop    %ecx
		pop    %esi
		ret



/*----------------------------------------------------------------*/
func_1398e0:
/*----------------------------------------------------------------*/
		add    $0x7468,%esi
		shr    $0x2,%eax
	jump_1398e9:
		testb  $0x1,(%esi)
		jne    jump_1398f6
		add    $0x4,%esi
		dec    %eax
		jne    jump_1398e9
		jmp    jump_13990f
	jump_1398f6:
		movzwl 0x2(%esi),%edx
		mov    $0x1,%ch
		mov    %dl,%cl
		shr    $0x3,%edx
		and    $0x7,%cl
		shl    %cl,%ch
		or     %ch,(%edi,%edx,1)
		add    $0x4,%esi
		dec    %eax
		jne    jump_1398e9
	jump_13990f:
		ret


/*----------------------------------------------------------------*/
func_139910:
/*----------------------------------------------------------------*/
		add    $0x7468,%esi
		shr    $0x2,%eax
		push   %ebp
		mov    data_1598d0,%bp
	jump_139921:
		testb  $0x1,(%esi)
		jne    jump_139931
		mov    %bp,(%esi)
		add    $0x4,%esi
		dec    %eax
		jne    jump_139921
		jmp    jump_13993e
	jump_139931:
		mov    0x2(%esi),%dx
		mov    %dx,(%esi)
		add    $0x4,%esi
		dec    %eax
		jne    jump_139921
	jump_13993e:
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
func_139940:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_139990
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_139981
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_139973:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_139973
		sub    data_1599f8,%ecx
	jump_139981:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_139990:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xa,%al
		ja     jump_1399e0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159804,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0x3ff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139a2c
		jmp    jump_139a05
	jump_1399e0:
		mov    %ebp,%edx
		mov    data_159804,%ecx
		and    $0x3ff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139a2c
	jump_139a05:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139a13
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139a13:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_139a23
		mov    $0x4,%edx
	jump_139a23:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139a05
	jump_139a2c:
		mov    data_159824,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_139a5d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159828,%ebx
		mov    %edx,(%ecx)
		mov    data_15982c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_139a5d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xb,%al
		ja     jump_139ab0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159800,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0x7ff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139afc
		jmp    jump_139ad5
	jump_139ab0:
		mov    %ebp,%edx
		mov    data_159800,%ecx
		and    $0x7ff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139afc
	jump_139ad5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139ae3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139ae3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_139af3
		mov    $0x4,%edx
	jump_139af3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139ad5
	jump_139afc:
		mov    data_159814,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_139b34
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159818,%ebx
		mov    %edx,(%ecx)
		mov    data_15981c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_139b34:
		mov    %eax,%edx
		mov    %eax,%ecx
		ror    $0x10,%eax
		mov    %ax,%dx
		mov    %ecx,%eax
		and    $0xff,%eax
		call   *data_159a00(,%eax,4)
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %ch,%al
		mov    %ebx,(%edi)
		and    $0xff,%eax
		add    data_159860,%edi
		mov    %ebx,(%edi)
		add    data_159860,%edi
		call   *data_159a00(,%eax,4)
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %ebx,(%edi)
		add    data_159860,%edi
		mov    %ebx,(%edi)
		sub    data_15986c,%edi
		decl   data_159868
		je     jump_139bc0
		decl   data_159864
		jne    jump_139990
		jmp    jump_13c4c0
	jump_139bc0:
		decl   data_159870
		je     jump_139bf0
		add    data_159888,%edi
	jump_139bce:
		mov    data_159874,%eax
		mov    %eax,data_159868
		decl   data_159864
		jne    jump_139990
		jmp    jump_13c4c0
	jump_139bf0:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_139c01:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_139c30
		cmp    $0xfffe,%ax
		je     jump_139c36
		ja     jump_13c5c0
		push   $func_139c83
		jmp    jump_139820
	jump_139c30:
		call   *data_15989c
	jump_139c36:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_139c83:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_139c01
		pop    %eax
		jmp    jump_139bce


/*----------------------------------------------------------------*/
func_139ca0:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_139cf0
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_139ce1
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_139cd3:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_139cd3
		sub    data_1599f8,%ecx
	jump_139ce1:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_139cf0:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_139d40
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139d8c
		jmp    jump_139d65
	jump_139d40:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139d8c
	jump_139d65:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139d73
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139d73:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_139d83
		mov    $0x4,%edx
	jump_139d83:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139d65
	jump_139d8c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_139dbd
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_139dbd:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_139e10
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139e5c
		jmp    jump_139e35
	jump_139e10:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139e5c
	jump_139e35:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139e43
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139e43:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_139e53
		mov    $0x4,%edx
	jump_139e53:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139e35
	jump_139e5c:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_139e94
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_139e94:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_139ef0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139f3c
		jmp    jump_139f15
	jump_139ef0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_139f3c
	jump_139f15:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139f23
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139f23:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_139f33
		mov    $0x4,%edx
	jump_139f33:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139f15
	jump_139f3c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_139f6d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_139f6d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_139fc0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a00c
		jmp    jump_139fe5
	jump_139fc0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a00c
	jump_139fe5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_139ff3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_139ff3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13a003
		mov    $0x4,%edx
	jump_13a003:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_139fe5
	jump_13a00c:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13a044
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13a044:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13a0a0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a0ec
		jmp    jump_13a0c5
	jump_13a0a0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a0ec
	jump_13a0c5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13a0d3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13a0d3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13a0e3
		mov    $0x4,%edx
	jump_13a0e3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13a0c5
	jump_13a0ec:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13a11d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13a11d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13a170
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a1bc
		jmp    jump_13a195
	jump_13a170:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a1bc
	jump_13a195:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13a1a3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13a1a3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13a1b3
		mov    $0x4,%edx
	jump_13a1b3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13a195
	jump_13a1bc:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13a1f4
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13a1f4:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13a250
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a29c
		jmp    jump_13a275
	jump_13a250:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a29c
	jump_13a275:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13a283
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13a283:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13a293
		mov    $0x4,%edx
	jump_13a293:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13a275
	jump_13a29c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13a2cd
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13a2cd:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13a320
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a36c
		jmp    jump_13a345
	jump_13a320:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13a36c
	jump_13a345:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13a353
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13a353:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13a363
		mov    $0x4,%edx
	jump_13a363:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13a345
	jump_13a36c:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13a3a4
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13a3a4:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %eax,(%edi)
		sub    data_15986c,%edi
		decl   data_159868
		je     jump_13a3d0
		decl   data_159864
		jne    jump_139cf0
		jmp    jump_13c4c0
	jump_13a3d0:
		decl   data_159870
		je     jump_13a400
		add    data_159888,%edi
	jump_13a3de:
		mov    data_159874,%eax
		mov    %eax,data_159868
		decl   data_159864
		jne    jump_139cf0
		jmp    jump_13c4c0
	jump_13a400:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13a411:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13a440
		cmp    $0xfffe,%ax
		je     jump_13a446
		ja     jump_13c5c0
		push   $func_13a493
		jmp    jump_139820
	jump_13a440:
		call   *data_15989c
	jump_13a446:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13a493:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13a411
		pop    %eax
		jmp    jump_13a3de


/*----------------------------------------------------------------*/
func_13a4b0:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_13a500
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_13a4f1
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_13a4e3:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_13a4e3
		sub    data_1599f8,%ecx
	jump_13a4f1:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_13a500:
		mov    %ah,%al
		mov    %eax,%ecx
		shl    $0x10,%eax
		mov    %cx,%ax
	jump_13a50a:
		mov    data_159868,%ecx
		cmp    %ecx,data_159864
		jae    jump_13a51e
		mov    data_159864,%ecx
	jump_13a51e:
		mov    data_159860,%ebx
		lea    0x0(,%ecx,4),%edx
		sub    %edx,%ebx
		mov    %ecx,%edx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		sub    data_159884,%edi
		sub    %edx,data_159868
		je     jump_13a580
		sub    %edx,data_159864
		jne    jump_13a50a
		jmp    jump_13c4c0
	jump_13a580:
		decl   data_159870
		je     jump_13a5b0
		add    data_159888,%edi
	jump_13a58e:
		sub    %edx,data_159864
		mov    data_159874,%ecx
		mov    %ecx,data_159868
		jne    jump_13a50a
		jmp    jump_13c4c0
	jump_13a5b0:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13a5c1:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13a5f0
		cmp    $0xfffe,%ax
		je     jump_13a5f6
		ja     jump_13c5c0
		push   $func_13a643
		jmp    jump_139820
	jump_13a5f0:
		call   *data_15989c
	jump_13a5f6:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13a643:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13a5c1
		pop    %eax
		jmp    jump_13a58e


/*----------------------------------------------------------------*/
func_13a660:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a66c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13a678:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a684:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13a690:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%ebx
		mov    %dl,%bh
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13a6a0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a6b0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%eax
		ror    $0x18,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13a6c0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13a6d0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a6dc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13a6e8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x10,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a6f4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x18,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13a700:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%ebx
		xchg   %al,%bl
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13a710:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a720:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%eax
		ror    $0x10,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13a730:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13a740:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13a74c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a758:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a768:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13a774:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a784:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x8,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a794:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7a4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x18,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7b4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7c0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7cc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7d8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a7e4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x18,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13a7f4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a804:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a814:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x18,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13a824:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13a830:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a840:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a84c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13a858:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a868:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a878:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a888:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x18,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a898:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8a4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8b0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8bc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8c8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x10,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13a8d8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8e8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a8f8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x10,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13a908:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13a914:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13a920:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13a92c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a938:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		mov    %dl,%al
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a948:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a958:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %dx,%ax
		mov    %eax,%ebx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a968:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		mov    %dh,%ah
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a978:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%ebx
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13a984:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a990:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		mov    %dl,%bh
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a99c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%bh
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a9a8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		ror    $0x18,%eax
		mov    %eax,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13a9b8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a9c8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%bh
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a9d8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		xchg   %ax,%bx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a9e8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13a9f8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aa08:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aa18:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aa28:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		ror    $0x8,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aa34:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa40:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%ax
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa50:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa5c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aa68:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%ah
		ror    $0x18,%ebx
		xchg   %al,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa74:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa80:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		xchg   %al,%bl
		ret


/*----------------------------------------------------------------*/
func_13aa8c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x8,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13aa9c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aaac:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aabc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%ebx
		ror    $0x8,%eax
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13aacc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x10,%eax
		mov    %dl,%ah
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aadc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x18,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aaec:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aafc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%ax
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13ab0c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13ab18:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ab20:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%ax
		ret


/*----------------------------------------------------------------*/
func_13ab2c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13ab38:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13ab44:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%al
		ror    $0x8,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13ab50:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ab58:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13ab64:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%ax
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13ab74:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ab84:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13ab94:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %dh,%ah
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aba4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		ror    $0x8,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13abb4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dx,%bx
		mov    %dl,%ah
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13abc4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dx,%bx
		mov    %dl,%ah
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13abd4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %dx,%ax
		mov    %eax,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13abe4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%bx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13abf4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%bx
		ret


/*----------------------------------------------------------------*/
func_13ac00:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %dx,%ax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ac0c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%bx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13ac1c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%ebx
		mov    %ax,%bx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13ac2c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		xchg   %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13ac38:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13ac44:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%ebx
		mov    %dx,%bx
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13ac54:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x18,%eax
		mov    %dx,%ax
		mov    %eax,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13ac64:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%ebx
		mov    %dx,%bx
		mov    %dh,%al
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13ac74:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dx,%bx
		mov    %dh,%al
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13ac84:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		ror    $0x18,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13ac94:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x8,%ebx
		ror    $0x10,%eax
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13aca4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x18,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13acb4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13acc4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x18,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13acd4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13ace0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13acec:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dx,%ax
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13acfc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x18,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ad08:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13ad14:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%al
		ror    $0x18,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13ad20:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13ad2c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ad38:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ad48:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13ad58:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x10,%eax
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ad68:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ad78:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13ad84:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13ad90:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13ad9c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13ada8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13adb4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13adc0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %dx,%ax
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13add0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13addc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13ade4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13adf0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13adf8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13ae00:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%al
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13ae0c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13ae18:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %al,%bh
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13ae24:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13ae30:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x10,%eax
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13ae3c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13ae4c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%bl
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13ae58:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13ae64:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13ae74:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13ae80:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		xchg   %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13ae8c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13ae9c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13aea8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %dh,%al
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13aeb4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13aec0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13aecc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13aed8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		ret


/*----------------------------------------------------------------*/
func_13aee8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%bh
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13aef4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13af00:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13af0c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13af18:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13af24:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x10,%eax
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13af30:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %edx,%ebx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13af3c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13af44:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %dx,%ax
		ret


/*----------------------------------------------------------------*/
func_13af50:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %edx,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13af5c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13af64:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%ah
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13af70:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ret


/*----------------------------------------------------------------*/
func_13af78:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13af80:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x10,%eax
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13af8c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13af98:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13afa4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13afb0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x10,%eax
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13afbc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13afc8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13afd4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13afe0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		xchg   %al,%bl
		ret


/*----------------------------------------------------------------*/
func_13afec:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13aff8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %dx,%ax
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13b008:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b014:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13b01c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13b028:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13b030:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b038:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13b044:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13b050:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13b05c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%eax
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13b068:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x18,%eax
		xchg   %ax,%bx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b078:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b088:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%bl
		ror    $0x10,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b098:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13b0a8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		mov    %dl,%al
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b0b8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x18,%eax
		mov    %ax,%bx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b0c8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		ror    $0x8,%eax
		mov    %dx,%ax
		mov    %eax,%ebx
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b0d8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		ror    $0x8,%eax
		mov    %ax,%bx
		mov    %dh,%ah
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b0e8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dl,%ah
		mov    %eax,%ebx
		mov    %dh,%bl
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b0f4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		mov    %eax,%ebx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b100:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dl,%bh
		mov    %dh,%bl
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b10c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13b118:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %dl,%ah
		ror    $0x8,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b124:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dl,%bl
		ret


/*----------------------------------------------------------------*/
func_13b130:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dl,%bh
		ret


/*----------------------------------------------------------------*/
func_13b13c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		xchg   %ah,%bl
		ret


/*----------------------------------------------------------------*/
func_13b148:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x8,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13b158:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x18,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b168:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b178:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x8,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b188:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x8,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b198:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1a8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1b8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%eax
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1c8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1d4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1e0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1ec:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b1f8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ret


/*----------------------------------------------------------------*/
func_13b204:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b210:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		ror    $0x10,%eax
		ret


/*----------------------------------------------------------------*/
func_13b220:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%ah
		ret


/*----------------------------------------------------------------*/
func_13b22c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x10,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13b23c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x18,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b24c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b25c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x10,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b26c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x8,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b27c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%eax
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b28c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x18,%eax
		mov    %eax,%ebx
		mov    %dl,%al
		ror    $0x18,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b29c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x10,%ebx
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b2ac:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b2b8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		mov    %dl,%ah
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b2c4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b2d0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x10,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b2dc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dl,%ah
		ret


/*----------------------------------------------------------------*/
func_13b2e8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x10,%ebx
		ror    $0x8,%eax
		ret


/*----------------------------------------------------------------*/
func_13b2f8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x10,%eax
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b304:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dh,%al
		ret


/*----------------------------------------------------------------*/
func_13b310:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x8,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13b320:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x18,%eax
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b330:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x10,%eax
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b340:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		xchg   %ah,%bh
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b350:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%eax
		ror    $0x8,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13b360:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%eax
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b370:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x18,%eax
		ror    $0x8,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13b380:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		mov    %dh,%bh
		ror    $0x18,%eax
		ret


/*----------------------------------------------------------------*/
func_13b390:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		xchg   %ah,%bl
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b39c:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		xchg   %ah,%bh
		ret


/*----------------------------------------------------------------*/
func_13b3a8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %edx,%ebx
		mov    %dh,%bl
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b3b4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		mov    %eax,%ebx
		ror    $0x8,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b3c0:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		xchg   %al,%bh
		ret


/*----------------------------------------------------------------*/
func_13b3cc:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %eax,%ebx
		mov    %dh,%bh
		ret


/*----------------------------------------------------------------*/
func_13b3d8:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x10,%eax
		mov    %eax,%ebx
		mov    %dh,%bl
		ret


/*----------------------------------------------------------------*/
func_13b3e4:
/*----------------------------------------------------------------*/
		mov    %edx,%eax
		mov    %dh,%al
		ror    $0x8,%eax
		mov    %dh,%ah
		mov    %eax,%ebx
		ret


/*----------------------------------------------------------------*/
func_13b3f0:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_13b440
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_13b431
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_13b423:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_13b423
		sub    data_1599f8,%ecx
	jump_13b431:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_13b440:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xa,%al
		ja     jump_13b490
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159804,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0x3ff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b4dc
		jmp    jump_13b4b5
	jump_13b490:
		mov    %ebp,%edx
		mov    data_159804,%ecx
		and    $0x3ff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b4dc
	jump_13b4b5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13b4c3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13b4c3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13b4d3
		mov    $0x4,%edx
	jump_13b4d3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13b4b5
	jump_13b4dc:
		mov    data_159824,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13b50d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159828,%ebx
		mov    %edx,(%ecx)
		mov    data_15982c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13b50d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xb,%al
		ja     jump_13b560
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159800,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0x7ff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b5ac
		jmp    jump_13b585
	jump_13b560:
		mov    %ebp,%edx
		mov    data_159800,%ecx
		and    $0x7ff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b5ac
	jump_13b585:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13b593
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13b593:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13b5a3
		mov    $0x4,%edx
	jump_13b5a3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13b585
	jump_13b5ac:
		mov    data_159814,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13b5e4
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159818,%ebx
		mov    %edx,(%ecx)
		mov    data_15981c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13b5e4:
		mov    %eax,%edx
		mov    %eax,%ecx
		ror    $0x10,%eax
		mov    %ax,%dx
		mov    %ecx,%eax
		and    $0xff,%eax
		call   *data_159a00(,%eax,4)
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %ch,%al
		mov    %ebx,(%edi)
		and    $0xff,%eax
		add    data_159860,%edi
		call   *data_159a00(,%eax,4)
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    %ebx,(%edi)
		sub    data_15986c,%edi
		decl   data_159868
		je     jump_13b650
		decl   data_159864
		jne    jump_13b440
		jmp    jump_13c4c0
	jump_13b650:
		decl   data_159870
		je     jump_13b680
		add    data_159888,%edi
	jump_13b65e:
		mov    data_159874,%eax
		mov    %eax,data_159868
		decl   data_159864
		jne    jump_13b440
		jmp    jump_13c4c0
	jump_13b680:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13b691:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13b6c0
		cmp    $0xfffe,%ax
		je     jump_13b6c6
		ja     jump_13c5c0
		push   $func_13b713
		jmp    jump_139820
	jump_13b6c0:
		call   *data_15989c
	jump_13b6c6:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13b713:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13b691
		pop    %eax
		jmp    jump_13b65e


/*----------------------------------------------------------------*/
func_13b730:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_13b780
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_13b771
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_13b763:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_13b763
		sub    data_1599f8,%ecx
	jump_13b771:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_13b780:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13b7d0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b81c
		jmp    jump_13b7f5
	jump_13b7d0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b81c
	jump_13b7f5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13b803
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13b803:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13b813
		mov    $0x4,%edx
	jump_13b813:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13b7f5
	jump_13b81c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13b84d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13b84d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13b8a0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b8ec
		jmp    jump_13b8c5
	jump_13b8a0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b8ec
	jump_13b8c5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13b8d3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13b8d3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13b8e3
		mov    $0x4,%edx
	jump_13b8e3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13b8c5
	jump_13b8ec:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13b924
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13b924:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13b980
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b9cc
		jmp    jump_13b9a5
	jump_13b980:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13b9cc
	jump_13b9a5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13b9b3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13b9b3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13b9c3
		mov    $0x4,%edx
	jump_13b9c3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13b9a5
	jump_13b9cc:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13b9fd
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13b9fd:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13ba50
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13ba9c
		jmp    jump_13ba75
	jump_13ba50:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13ba9c
	jump_13ba75:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13ba83
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13ba83:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13ba93
		mov    $0x4,%edx
	jump_13ba93:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13ba75
	jump_13ba9c:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13bad4
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13bad4:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13bb30
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bb7c
		jmp    jump_13bb55
	jump_13bb30:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bb7c
	jump_13bb55:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13bb63
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13bb63:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13bb73
		mov    $0x4,%edx
	jump_13bb73:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13bb55
	jump_13bb7c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13bbad
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13bbad:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13bc00
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bc4c
		jmp    jump_13bc25
	jump_13bc00:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bc4c
	jump_13bc25:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13bc33
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13bc33:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13bc43
		mov    $0x4,%edx
	jump_13bc43:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13bc25
	jump_13bc4c:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13bc84
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13bc84:
		mov    %eax,(%edi)
		add    data_159860,%edi
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13bce0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bd2c
		jmp    jump_13bd05
	jump_13bce0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bd2c
	jump_13bd05:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13bd13
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13bd13:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13bd23
		mov    $0x4,%edx
	jump_13bd23:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13bd05
	jump_13bd2c:
		mov    data_159834,%ecx
		mov    %al,data_1598c0
		mov    %edx,%eax
		cmp    %edx,(%ecx)
		je     jump_13bd5d
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13bd5d:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0xc,%al
		ja     jump_13bdb0
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_159808,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xfff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bdfc
		jmp    jump_13bdd5
	jump_13bdb0:
		mov    %ebp,%edx
		mov    data_159808,%ecx
		and    $0xfff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13bdfc
	jump_13bdd5:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13bde3
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13bde3:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13bdf3
		mov    $0x4,%edx
	jump_13bdf3:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13bdd5
	jump_13bdfc:
		mov    data_159834,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13be34
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159838,%ebx
		mov    %edx,(%ecx)
		mov    data_15983c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13be34:
		mov    %eax,(%edi)
		sub    data_15986c,%edi
		decl   data_159868
		je     jump_13be60
		decl   data_159864
		jne    jump_13b780
		jmp    jump_13c4c0
	jump_13be60:
		decl   data_159870
		je     jump_13be90
		add    data_159888,%edi
	jump_13be6e:
		mov    data_159874,%eax
		mov    %eax,data_159868
		decl   data_159864
		jne    jump_13b780
		jmp    jump_13c4c0
	jump_13be90:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13bea1:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13bed0
		cmp    $0xfffe,%ax
		je     jump_13bed6
		ja     jump_13c5c0
		push   $func_13bf23
		jmp    jump_139820
	jump_13bed0:
		call   *data_15989c
	jump_13bed6:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13bf23:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13bea1
		pop    %eax
		jmp    jump_13be6e


/*----------------------------------------------------------------*/
func_13bf40:
/*----------------------------------------------------------------*/
		mov    data_1599f4,%ecx
		add    %edx,%ecx
		mov    %ecx,data_1599f4
		mov    data_1599f0,%eax
		cmp    %ecx,%eax
		ja     jump_13bf84
		shr    $0x4,%ecx
		shr    $0x4,%eax
		cmp    %ecx,%eax
		jae    jump_13bf7b
		add    data_1599f8,%eax
		add    data_1599f8,%ecx
	jump_13bf6d:
		movb   $0x0,(%eax)
		inc    %eax
		cmp    %ecx,%eax
		jne    jump_13bf6d
		sub    data_1599f8,%ecx
	jump_13bf7b:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
	jump_13bf84:
		mov    data_159868,%ecx
		cmp    %ecx,%edx
		jae    jump_13bf90
		mov    %edx,%ecx
	jump_13bf90:
		lea    (%edi,%ecx,4),%edi
		sub    %ecx,data_159868
		je     jump_13bfb0
		sub    %ecx,%edx
		jne    jump_13bf84
		jmp    jump_13c4c0
	jump_13bfb0:
		decl   data_159870
		je     jump_13bfe0
		add    data_159888,%edi
	jump_13bfbe:
		mov    data_159874,%eax
		mov    %eax,data_159868
		sub    %ecx,%edx
		jne    jump_13bf84
		jmp    jump_13c4c0
	jump_13bfe0:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13bff1:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13c020
		cmp    $0xfffe,%ax
		je     jump_13c026
		ja     jump_13c5c0
		push   $func_13c073
		jmp    jump_139820
	jump_13c020:
		call   *data_15989c
	jump_13c026:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13c073:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13bff1
		pop    %eax
		jmp    jump_13bfbe


/*----------------------------------------------------------------*/
func_13c090:
/*----------------------------------------------------------------*/
		add    %edx,data_1599f4
		mov    data_1599f4,%ecx
		cmp    %ecx,data_1599f0
		ja     jump_13c0e0
		mov    data_1599f0,%ebx
		add    $0xf,%ecx
		shr    $0x4,%ecx
		shr    $0x4,%ebx
		cmp    %ecx,%ebx
		je     jump_13c0d1
		add    data_1599f8,%ebx
		add    data_1599f8,%ecx
	jump_13c0c3:
		movb   $0x1,(%ebx)
		inc    %ebx
		cmp    %ecx,%ebx
		jne    jump_13c0c3
		sub    data_1599f8,%ecx
	jump_13c0d1:
		shl    $0x4,%ecx
		mov    %ecx,data_1599f0
		xchg   %ebx,%ebx
		xchg   %ecx,%ecx
		xchg   %ebx,%ebx
	jump_13c0e0:
		mov    %ah,%al
		mov    %eax,%ecx
		shl    $0x10,%eax
		mov    %cx,%ax
	jump_13c0ea:
		mov    data_159868,%ecx
		cmp    %ecx,data_159864
		jae    jump_13c0fe
		mov    data_159864,%ecx
	jump_13c0fe:
		mov    data_159860,%ebx
		lea    0x0(,%ecx,4),%edx
		sub    %edx,%ebx
		mov    %ecx,%edx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		add    %ebx,%edi
		mov    %edx,%ecx
		rep stos %eax,%es:(%edi)
		sub    data_159884,%edi
		sub    %edx,data_159868
		je     jump_13c140
		sub    %edx,data_159864
		jne    jump_13c0ea
		jmp    jump_13c4c0
	jump_13c140:
		decl   data_159870
		je     jump_13c170
		add    data_159888,%edi
	jump_13c14e:
		sub    %edx,data_159864
		mov    data_159874,%ecx
		mov    %ecx,data_159868
		jne    jump_13c0ea
		jmp    jump_13c4c0
	jump_13c170:
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13c181:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13c1b0
		cmp    $0xfffe,%ax
		je     jump_13c1b6
		ja     jump_13c5c0
		push   $func_13c203
		jmp    jump_139820
	jump_13c1b0:
		call   *data_15989c
	jump_13c1b6:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi


/*----------------------------------------------------------------*/
func_13c203:
/*----------------------------------------------------------------*/
		cmpl   $0x0,data_159870
		je     jump_13c181
		pop    %eax
		jmp    jump_13c14e


/*----------------------------------------------------------------*/
GLOBAL_FUNC (Unsmack)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		push   %es
		push   %ebx
		push   %edi
		push   %esi
		push   %edx
		mov    %ds,%ax
		mov    %eax,%es
		cld
		mov    0xc(%ebp),%esi
		mov    %esi,data_1598a4
		add    $0x7410,%esi
		mov    (%esi),%eax
		add    $0x4,%esi
		mov    %ax,data_1598d0
		mov    $data_159810,%edi
		mov    $0x4,%ecx
	jump_13c252:
		lods   %ds:(%esi),%eax
		stos   %eax,%es:(%edi)
		mov    %eax,%edx
		lods   %ds:(%esi),%eax
		stos   %eax,%es:(%edi)
		mov    %edx,(%eax)
		lods   %ds:(%esi),%eax
		stos   %eax,%es:(%edi)
		mov    %edx,(%eax)
		lods   %ds:(%esi),%eax
		stos   %eax,%es:(%edi)
		mov    %edx,(%eax)
		dec    %ecx
		jne    jump_13c252
		mov    0xc(%ebp),%eax
		add    $0x10,%eax
		mov    %eax,data_159800
		add    $0x2000,%eax
		mov    %eax,data_159804
		add    $0x1000,%eax
		mov    %eax,data_159808
		add    $0x4000,%eax
		mov    %eax,data_15980c
		mov    0x10(%ebp),%edi
		mov    0x8(%ebp),%esi
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x21,data_1598c0
		mov    (%edi),%al
		and    $0x1,%eax
		xor    $0x1,%eax
		mov    %eax,data_1598ac
		testb  $0x1,0x1(%edi)
		jne    jump_13c31d
		testl  $0x1,data_1598ac
		je     jump_13c2e9
		movl   $jump_13b440,data_159850
		movl   $jump_13b780,data_159854
		movl   $jump_13bf84,data_159858
		movl   $jump_13c0e0,data_15985c
		jmp    jump_13c311
	jump_13c2e9:
		movl   $func_13b3f0,data_159850
		movl   $func_13b730,data_159854
		movl   $func_13bf40,data_159858
		movl   $func_13c090,data_15985c
	jump_13c311:
		movl   $0x0,data_1598a8
		jmp    jump_13c385
	jump_13c31d:
		testl  $0x1,data_1598ac
		je     jump_13c353
		movl   $jump_139990,data_159850
		movl   $jump_139cf0,data_159854
		movl   $jump_13bf84,data_159858
		movl   $jump_13a500,data_15985c
		jmp    jump_13c37b
	jump_13c353:
		movl   $func_139940,data_159850
		movl   $func_139ca0,data_159854
		movl   $func_13bf40,data_159858
		movl   $func_13a4b0,data_15985c
	jump_13c37b:
		movl   $0x1,data_1598a8
	jump_13c385:
		testl  $0x1,data_1598ac
		je     jump_13c453
		mov    0x4(%edi),%eax
		mov    %eax,data_15989c
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159880
		add    $0xa,%edi
		mov    %edi,data_1598a0
		nop
		testl  $0x1,data_1598ac
		je     jump_13c5c0
		push   %eax
	jump_13c3c1:
		mov    data_1598a0,%edi
		mov    (%edi),%ax
		cmp    $0xfffd,%ax
		jb     jump_13c3f0
		cmp    $0xfffe,%ax
		je     jump_13c3f6
		ja     jump_13c5c0
		push   $jump_13c443
		jmp    jump_139820
	jump_13c3f0:
		call   *data_15989c
	jump_13c3f6:
		add    $0x2,%edi
		movzwl (%edi),%eax
		mov    %eax,data_159860
		movzwl 0x2(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		movzwl 0x4(%edi),%eax
		mov    %eax,data_15986c
		movzwl 0x6(%edi),%eax
		mov    %eax,data_159884
		movzwl 0x8(%edi),%eax
		mov    %eax,data_159888
		add    $0xa,%edi
		movzwl 0x4(%edi),%eax
		mov    %eax,data_159870
		mov    (%edi),%eax
		add    $0x6,%edi
		mov    %edi,data_1598a0
		mov    %eax,%edi
	jump_13c443:
		cmpl   $0x0,data_159870
		je     jump_13c3c1
		pop    %eax
		jmp    jump_13c4b9
	jump_13c453:
		mov    0x8(%edi),%eax
		mov    %eax,data_159874
		mov    %eax,data_159868
		mov    0xc(%edi),%eax
		mov    %eax,data_159870
		mov    0x10(%edi),%eax
		mov    %eax,data_15986c
		mov    0x14(%edi),%eax
		mov    %eax,data_159884
		mov    0x18(%edi),%eax
		mov    %eax,data_159860
		mov    0x1c(%edi),%eax
		mov    %eax,data_159888
		mov    0x4(%edi),%eax
		movl   $0x0,0x20(%edi)
		movl   $0x0,0x2c(%edi)
		movl   $0x0,0x30(%edi)
		add    $0x38,%edi
		mov    %edi,data_1599f8
		xor    %edi,%edi
		mov    %edi,data_1599f4
		mov    %edi,data_1599f0
		mov    %eax,%edi
	jump_13c4b9:
		jmp    jump_13c4c0

	jump_13c4c0:
		mov    data_1598c0,%al
		mov    data_1598d0,%ebx
		cmp    $0x8,%al
		ja     jump_13c510
		mov    %al,%cl
		mov    (%esi),%edx
		dec    %cl
		add    $0x2,%esi
		shl    %cl,%edx
		mov    data_15980c,%ecx
		or     %ebp,%edx
		mov    %edx,%ebp
		and    $0xff,%edx
		add    $0x10,%al
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13c55c
		jmp    jump_13c535
	jump_13c510:
		mov    %ebp,%edx
		mov    data_15980c,%ecx
		and    $0xff,%edx
		mov    (%ecx,%edx,4),%ecx
		shr    %cl,%ebp
		sub    %cl,%al
		shr    $0x8,%ecx
		add    data_1598a4,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		jne    jump_13c55c
	jump_13c535:
		shr    $0xd,%edx
		dec    %al
		jne    jump_13c543
		mov    (%esi),%ebp
		add    $0x4,%esi
		mov    $0x20,%al
	jump_13c543:
		and    $0xffff8,%edx
		shr    $0x1,%ebp
		jb     jump_13c553
		mov    $0x4,%edx
	jump_13c553:
		add    %edx,%ecx
		mov    (%ecx),%edx
		cmp    %dx,%bx
		je     jump_13c535
	jump_13c55c:
		mov    data_159844,%ecx
		ror    $0x10,%edx
		mov    %al,data_1598c0
		mov    %dx,%ax
		ror    $0x10,%edx
		cmp    %edx,(%ecx)
		je     jump_13c594
		mov    %eax,data_159878
		mov    (%ecx),%eax
		mov    data_159848,%ebx
		mov    %edx,(%ecx)
		mov    data_15984c,%ecx
		mov    (%ebx),%edx
		mov    %eax,(%ebx)
		mov    data_159878,%eax
		mov    %edx,(%ecx)
	jump_13c594:
		mov    %eax,%edx
		mov    %eax,%ecx
		and    $0xfc,%edx
		and    $0x3,%ecx
		mov    data_1598e0(%edx),%edx
		mov    %edx,data_159864
		jmp    *data_159850(,%ecx,4)
	jump_13c5c0:
		testl  $0x1,data_1598ac
		je     jump_13c5cf
		pop    %eax
		jmp    jump_13c5e0
	jump_13c5cf:
		mov    data_1599f0,%eax
		shr    $0x4,%eax
		add    data_1599f8,%eax
		movb   $0x0,(%eax)
	jump_13c5e0:
		pop    %edx
		pop    %esi
		pop    %edi
		pop    %ebx
		pop    %es
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SmackDoPCM)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		push   %ebx
		push   %esi
		push   %edi
		push   %es
		cld
		mov    %ds,%ax
		mov    %eax,%es
		mov    0xc(%ebp),%eax
		mov    %eax,data_159e20
		mov    0x10(%ebp),%eax
		mov    %eax,data_159e24
		mov    0x14(%ebp),%eax
		mov    %eax,data_159e28
		mov    0x18(%ebp),%eax
		mov    %eax,data_159e2c
		mov    0x1c(%ebp),%eax
		mov    %eax,data_159e30
		movb   $0x1,data_1598c0
		mov    0x8(%ebp),%esi
		xor    %ebp,%ebp
		decb   data_1598c0
		jne    jump_13c645
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c645:
		shr    $0x1,%ebp
		jae    jump_13ce12
		decb   data_1598c0
		jne    jump_13c662
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c662:
		shr    $0x1,%ebp
		jb     jump_13c935
		decb   data_1598c0
		jne    jump_13c67f
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c67f:
		shr    $0x1,%ebp
		jb     jump_13c780
		decb   data_1598c0
		jne    jump_13c69c
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c69c:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		mov    %edi,data_15988c
		call   func_139100
		mov    data_159e20,%edx
		cmp    $0x0,%edx
		je     jump_13ce12
		mov    data_159e30,%edi
		add    %edi,%edx
		cmp    %edx,data_159e2c
		jae    jump_13c6db
		sub    data_159e2c,%edx
		add    data_159e28,%edx
	jump_13c6db:
		mov    %edx,data_1599fc
		cmpb   $0x9,data_1598c0
		jb     jump_13c6fa
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13c720
	jump_13c6fa:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13c720:
		mov    %al,%cl
		jmp    jump_13c761
	jump_13c724:
		mov    data_15988c,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13c75c
	jump_13c733:
		decb   data_1598c0
		jne    jump_13c747
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c747:
		shr    $0x1,%ebp
		jb     jump_13c751
		mov    $0x4,%eax
	jump_13c751:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13c733
	jump_13c75c:
		mov    %ax,%ax
		add    %al,%cl
	jump_13c761:
		mov    %cl,(%edi)
		inc    %edi
		cmp    %edi,data_1599fc
		je     jump_13ce12
		cmp    %edi,data_159e2c
		jne    jump_13c724
		mov    data_159e28,%edi
		jmp    jump_13c724
	jump_13c780:
		decb   data_1598c0
		jne    jump_13c794
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c794:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		mov    %edi,data_15988c
		call   func_139100
		decb   data_1598c0
		jne    jump_13c7bc
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c7bc:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		add    $0x800,%edi
		mov    %edi,data_159890
		call   func_139100
		mov    data_159e20,%edx
		cmp    $0x0,%edx
		je     jump_13ce12
		mov    data_159e30,%edi
		add    %edi,%edx
		cmp    %edx,data_159e2c
		jae    jump_13c801
		sub    data_159e2c,%edx
		add    data_159e28,%edx
	jump_13c801:
		mov    %edx,data_1599fc
		cmpb   $0x9,data_1598c0
		jb     jump_13c820
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13c846
	jump_13c820:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13c846:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_13c861
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13c887
	jump_13c861:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13c887:
		mov    %ax,%cx
		jmp    jump_13c90c
	jump_13c88f:
		mov    data_15988c,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13c8c7
	jump_13c89e:
		decb   data_1598c0
		jne    jump_13c8b2
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c8b2:
		shr    $0x1,%ebp
		jb     jump_13c8bc
		mov    $0x4,%eax
	jump_13c8bc:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13c89e
	jump_13c8c7:
		mov    %ax,%ax
		add    %al,%cl
		adc    $0x0,%ch
		mov    data_159890,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13c907
	jump_13c8de:
		decb   data_1598c0
		jne    jump_13c8f2
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c8f2:
		shr    $0x1,%ebp
		jb     jump_13c8fc
		mov    $0x4,%eax
	jump_13c8fc:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13c8de
	jump_13c907:
		mov    %ax,%ax
		add    %al,%ch
	jump_13c90c:
		mov    %cx,(%edi)
		add    $0x2,%edi
		cmp    %edi,data_1599fc
		je     jump_13ce12
		cmp    %edi,data_159e2c
		jne    jump_13c88f
		mov    data_159e28,%edi
		jmp    jump_13c88f
	jump_13c935:
		decb   data_1598c0
		jne    jump_13c949
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c949:
		shr    $0x1,%ebp
		jb     jump_13cb01
		decb   data_1598c0
		jne    jump_13c966
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c966:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		mov    %edi,data_15988c
		call   func_139100
		decb   data_1598c0
		jne    jump_13c98e
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13c98e:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		add    $0x800,%edi
		mov    %edi,data_159890
		call   func_139100
		mov    data_159e20,%edx
		cmp    $0x0,%edx
		je     jump_13ce12
		mov    data_159e30,%edi
		add    %edi,%edx
		cmp    %edx,data_159e2c
		jae    jump_13c9d3
		sub    data_159e2c,%edx
		add    data_159e28,%edx
	jump_13c9d3:
		mov    %edx,data_1599fc
		cmpb   $0x9,data_1598c0
		jb     jump_13c9f2
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13ca18
	jump_13c9f2:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13ca18:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_13ca33
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13ca59
	jump_13ca33:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13ca59:
		mov    %ax,%cx
		jmp    jump_13cad8
	jump_13ca5e:
		mov    data_15988c,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13ca96
	jump_13ca6d:
		decb   data_1598c0
		jne    jump_13ca81
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13ca81:
		shr    $0x1,%ebp
		jb     jump_13ca8b
		mov    $0x4,%eax
	jump_13ca8b:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13ca6d
	jump_13ca96:
		mov    %ax,%ax
		add    %al,%cl
		mov    data_159890,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13cad3
	jump_13caaa:
		decb   data_1598c0
		jne    jump_13cabe
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cabe:
		shr    $0x1,%ebp
		jb     jump_13cac8
		mov    $0x4,%eax
	jump_13cac8:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13caaa
	jump_13cad3:
		mov    %ax,%ax
		add    %al,%ch
	jump_13cad8:
		mov    %cx,(%edi)
		add    $0x2,%edi
		cmp    %edi,data_1599fc
		je     jump_13ce12
		cmp    %edi,data_159e2c
		jne    jump_13ca5e
		mov    data_159e28,%edi
		jmp    jump_13ca5e
	jump_13cb01:
		decb   data_1598c0
		jne    jump_13cb15
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cb15:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		mov    %edi,data_15988c
		call   func_139100
		decb   data_1598c0
		jne    jump_13cb3d
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cb3d:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		add    $0x800,%edi
		mov    %edi,data_159890
		call   func_139100
		decb   data_1598c0
		jne    jump_13cb6b
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cb6b:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		add    $0x1000,%edi
		mov    %edi,data_159894
		call   func_139100
		decb   data_1598c0
		jne    jump_13cb99
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cb99:
		shr    $0x1,%ebp
		mov    data_159e24,%edi
		add    $0x1800,%edi
		mov    %edi,data_159898
		call   func_139100
		mov    data_159e20,%edx
		cmp    $0x0,%edx
		je     jump_13ce12
		mov    data_159e30,%edi
		add    %edi,%edx
		cmp    %edx,data_159e2c
		jae    jump_13cbde
		sub    data_159e2c,%edx
		add    data_159e28,%edx
	jump_13cbde:
		mov    %edx,data_1599fc
		cmpb   $0x9,data_1598c0
		jb     jump_13cbfd
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13cc23
	jump_13cbfd:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13cc23:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_13cc3e
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13cc64
	jump_13cc3e:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13cc64:
		shl    $0x10,%eax
		cmpb   $0x9,data_1598c0
		jb     jump_13cc80
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13cca6
	jump_13cc80:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13cca6:
		mov    %al,%ah
		cmpb   $0x9,data_1598c0
		jb     jump_13ccc1
		mov    %ebp,%ebx
		subb   $0x8,data_1598c0
		shr    $0x8,%ebp
		mov    %bl,%al
		jmp    jump_13cce7
	jump_13ccc1:
		mov    data_1598c0,%cl
		mov    %ebp,%ebx
		addb   $0x18,data_1598c0
		mov    %bl,%al
		mov    (%esi),%ebx
		dec    %cl
		mov    %ebx,%ebp
		shl    %cl,%ebx
		sub    $0x9,%cl
		or     %bl,%al
		xor    $0xff,%cl
		add    $0x4,%esi
		shr    %cl,%ebp
	jump_13cce7:
		mov    %eax,%ecx
		jmp    jump_13cdee
	jump_13ccee:
		mov    data_15988c,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13cd26
	jump_13ccfd:
		decb   data_1598c0
		jne    jump_13cd11
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cd11:
		shr    $0x1,%ebp
		jb     jump_13cd1b
		mov    $0x4,%eax
	jump_13cd1b:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13ccfd
	jump_13cd26:
		mov    %ax,%ax
		add    %al,%cl
		adc    $0x0,%ch
		mov    data_159890,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13cd66
	jump_13cd3d:
		decb   data_1598c0
		jne    jump_13cd51
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cd51:
		shr    $0x1,%ebp
		jb     jump_13cd5b
		mov    $0x4,%eax
	jump_13cd5b:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13cd3d
	jump_13cd66:
		mov    %ax,%ax
		add    %al,%ch
		ror    $0x10,%ecx
		mov    data_159894,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13cda6
	jump_13cd7d:
		decb   data_1598c0
		jne    jump_13cd91
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cd91:
		shr    $0x1,%ebp
		jb     jump_13cd9b
		mov    $0x4,%eax
	jump_13cd9b:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13cd7d
	jump_13cda6:
		mov    %ax,%ax
		add    %al,%cl
		adc    $0x0,%ch
		mov    data_159898,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		jne    jump_13cde6
	jump_13cdbd:
		decb   data_1598c0
		jne    jump_13cdd1
		mov    (%esi),%ebp
		add    $0x4,%esi
		movb   $0x20,data_1598c0
	jump_13cdd1:
		shr    $0x1,%ebp
		jb     jump_13cddb
		mov    $0x4,%eax
	jump_13cddb:
		add    %eax,%edx
		mov    (%edx),%eax
		test   $0x80000000,%eax
		je     jump_13cdbd
	jump_13cde6:
		mov    %ax,%ax
		add    %al,%ch
		ror    $0x10,%ecx
	jump_13cdee:
		mov    %ecx,(%edi)
		add    $0x4,%edi
		cmp    %edi,data_1599fc
		je     jump_13ce12
		cmp    %edi,data_159e2c
		jne    jump_13ccee
		mov    data_159e28,%edi
		jmp    jump_13ccee
	jump_13ce12:
		pop    %es
		pop    %edi
		pop    %esi
		pop    %ebx
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SmackGetSizeDeltas)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		push   %edx
		push   %ecx
		mov    0x8(%ebp),%eax
		mov    0xc(%ebp),%ecx
		mul    %ecx
		shr    $0x8,%eax
		add    $0x20,%eax
		pop    %ecx
		pop    %edx
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SmackRemapTables)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		push   %edi
		push   %esi
		push   %ebx
		push   %edx
		push   %es
		cld
		mov    0xc(%ebp),%edi
		xor    %ebx,%ebx
		mov    0x8(%ebp),%esi
		lea    0x7410(%esi),%eax
		mov    (%eax),%cx
		add    $0x4,%eax
		mov    %cx,data_1598d0
		mov    (%edi),%cl
		mov    %cl,%ch
		mov    %cx,0x12(%eax)
		mov    %cx,0x22(%eax)
		mov    0x4(%esi),%ecx
		add    (%esi),%esi
		add    $0x7468,%esi
		shr    $0x2,%ecx
		jecxz  jump_13cfbb
	jump_13cfa0:
		lods   %ds:(%esi),%eax
		cmp    %ax,data_1598d0
		je     jump_13cfb8
		mov    %al,%bl
		mov    (%edi,%ebx,1),%al
		mov    %ah,%bl
		mov    (%edi,%ebx,1),%ah
		mov    %ax,-0x2(%esi)
	jump_13cfb8:
		dec    %ecx
		jne    jump_13cfa0
	jump_13cfbb:
		mov    0x8(%ebp),%esi
		mov    (%esi),%eax
		add    0x4(%esi),%eax
		mov    0x8(%esi),%ecx
		add    %eax,%esi
		add    $0x7468,%esi
		shr    $0x2,%ecx
		jecxz  jump_13cfee
	jump_13cfd3:
		lods   %ds:(%esi),%eax
		cmp    %ax,data_1598d0
		je     jump_13cfeb
		mov    %al,%bl
		mov    (%edi,%ebx,1),%al
		mov    %ah,%bl
		mov    (%edi,%ebx,1),%ah
		mov    %ax,-0x2(%esi)
	jump_13cfeb:
		dec    %ecx
		jne    jump_13cfd3
	jump_13cfee:
		mov    0x8(%ebp),%esi
		mov    (%esi),%eax
		add    0x4(%esi),%eax
		add    0x8(%esi),%eax
		mov    0xc(%esi),%ecx
		add    %eax,%esi
		add    $0x7468,%esi
		shr    $0x2,%ecx
		jecxz  jump_13d028
	jump_13d009:
		lods   %ds:(%esi),%eax
		mov    %eax,%edx
		cmp    %ax,data_1598d0
		je     jump_13d025
		and    $0x3,%edx
		cmp    $0x3,%edx
		jne    jump_13d025
		mov    %ah,%bl
		mov    (%edi,%ebx,1),%ah
		mov    %ah,-0x1(%esi)
	jump_13d025:
		dec    %ecx
		jne    jump_13d009
	jump_13d028:
		pop    %es
		pop    %edx
		pop    %ebx
		pop    %esi
		pop    %edi
		pop    %ebp
		ret


.data


data_159800:
		.long	0x0
data_159804:
		.long	0x0
data_159808:
		.long	0x0
data_15980c:
		.long	0x0
data_159810:
		.long	0x0
data_159814:
		.long	0x0
data_159818:
		.long	0x0
data_15981c:
		.long	0x0
data_159820:
		.long	0x0
data_159824:
		.long	0x0
data_159828:
		.long	0x0
data_15982c:
		.long	0x0
data_159830:
		.long	0x0
data_159834:
		.long	0x0
data_159838:
		.long	0x0
data_15983c:
		.long	0x0
data_159840:
		.long	0x0
data_159844:
		.long	0x0
data_159848:
		.long	0x0
data_15984c:
		.long	0x0
data_159850:
		.long	0x0
data_159854:
		.long	0x0
data_159858:
		.long	0x0
data_15985c:
		.long	0x0
data_159860:
		.long	0x0
data_159864:
		.long	0x0
data_159868:
		.long	0x0
data_15986c:
		.long	0x0
data_159870:
		.long	0x0
data_159874:
		.long	0x0
data_159878:
		.fill   0x8
data_159880:
		.long	0x0
data_159884:
		.long	0x0
data_159888:
		.long	0x0
data_15988c:
		.long	0x0
data_159890:
		.long	0x0
data_159894:
		.long	0x0
data_159898:
		.long	0x0
data_15989c:
		.long	0x0
data_1598a0:
		.long	0x0
data_1598a4:
		.long	0x0
data_1598a8:
		.long	0x0
data_1598ac:
		.long	0x0
data_1598b0:
		.long	0x0
data_1598b4:
		.fill   0xc
data_1598c0:
		.fill   0x10
data_1598d0:
		.fill   0x10
data_1598e0:
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
		.ascii  "\x1f\x00\x00\x00\x20\x00\x00\x00"
		.ascii  "\x21\x00\x00\x00\x22\x00\x00\x00"
		.ascii  "\x23\x00\x00\x00\x24\x00\x00\x00"
		.ascii  "\x25\x00\x00\x00\x26\x00\x00\x00"
		.ascii  "\x27\x00\x00\x00\x28\x00\x00\x00"
		.ascii  "\x29\x00\x00\x00\x2a\x00\x00\x00"
		.ascii  "\x2b\x00\x00\x00\x2c\x00\x00\x00"
		.ascii  "\x2d\x00\x00\x00\x2e\x00\x00\x00"
		.ascii  "\x2f\x00\x00\x00\x30\x00\x00\x00"
		.ascii  "\x31\x00\x00\x00\x32\x00\x00\x00"
		.ascii  "\x33\x00\x00\x00\x34\x00\x00\x00"
		.ascii  "\x35\x00\x00\x00\x36\x00\x00\x00"
		.ascii  "\x37\x00\x00\x00\x38\x00\x00\x00"
		.ascii  "\x39\x00\x00\x00\x3a\x00\x00\x00"
		.ascii  "\x3b\x00\x00\x00\x80"
		.long	0x0
		.ascii  "\x01\x00\x00\x00\x02\x00\x00\x00"
		.ascii  "\x04\x00\x00\x00\x08\x00\x00"


data_1599e0:
		.short  0x0
data_1599e2:
		.short  0x0
data_1599e4:
		.fill   0xc

data_1599f0:
		.long	0x0
data_1599f4:
		.long	0x0
data_1599f8:
		.long	0x0
data_1599fc:
		.long	0x0
data_159a00:
		.long   func_13a660
		.long   func_13a66c
		.long   func_13a678
		.long   func_13a684
		.long   func_13a690
		.long   func_13a6a0
		.long   func_13a6b0
		.long   func_13a6c0
		.long   func_13a6d0
		.long   func_13a6dc
		.long   func_13a6e8
		.long   func_13a6f4
		.long   func_13a700
		.long   func_13a710
		.long   func_13a720
		.long   func_13a730
		.long   func_13a740
		.long   func_13a74c
		.long   func_13a758
		.long   func_13a768
		.long   func_13a774
		.long   func_13a784
		.long   func_13a794
		.long   func_13a7a4
		.long   func_13a7b4
		.long   func_13a7c0
		.long   func_13a7cc
		.long   func_13a7d8
		.long   func_13a7e4
		.long   func_13a7f4
		.long   func_13a804
		.long   func_13a814
		.long   func_13a824
		.long   func_13a830
		.long   func_13a840
		.long   func_13a84c
		.long   func_13a858
		.long   func_13a868
		.long   func_13a878
		.long   func_13a888
		.long   func_13a898
		.long   func_13a8a4
		.long   func_13a8b0
		.long   func_13a8bc
		.long   func_13a8c8
		.long   func_13a8d8
		.long   func_13a8e8
		.long   func_13a8f8
		.long   func_13a908
		.long   func_13a914
		.long   func_13a920
		.long   func_13a92c
		.long   func_13a938
		.long   func_13a948
		.long   func_13a958
		.long   func_13a968
		.long   func_13a978
		.long   func_13a984
		.long   func_13a990
		.long   func_13a99c
		.long   func_13a9a8
		.long   func_13a9b8
		.long   func_13a9c8
		.long   func_13a9d8
		.long   func_13a9e8
		.long   func_13a9f8
		.long   func_13aa08
		.long   func_13aa18
		.long   func_13aa28
		.long   func_13aa34
		.long   func_13aa40
		.long   func_13aa50
		.long   func_13aa5c
		.long   func_13aa68
		.long   func_13aa74
		.long   func_13aa80
		.long   func_13aa8c
		.long   func_13aa9c
		.long   func_13aaac
		.long   func_13aabc
		.long   func_13aacc
		.long   func_13aadc
		.long   func_13aaec
		.long   func_13aafc
		.long   func_13ab0c
		.long   func_13ab18
		.long   func_13ab20
		.long   func_13ab2c
		.long   func_13ab38
		.long   func_13ab44
		.long   func_13ab50
		.long   func_13ab58
		.long   func_13ab64
		.long   func_13ab74
		.long   func_13ab84
		.long   func_13ab94
		.long   func_13aba4
		.long   func_13abb4
		.long   func_13abc4
		.long   func_13abd4
		.long   func_13abe4
		.long   func_13abf4
		.long   func_13ac00
		.long   func_13ac0c
		.long   func_13ac1c
		.long   func_13ac2c
		.long   func_13ac38
		.long   func_13ac44
		.long   func_13ac54
		.long   func_13ac64
		.long   func_13ac74
		.long   func_13ac84
		.long   func_13ac94
		.long   func_13aca4
		.long   func_13acb4
		.long   func_13acc4
		.long   func_13acd4
		.long   func_13ace0
		.long   func_13acec
		.long   func_13acfc
		.long   func_13ad08
		.long   func_13ad14
		.long   func_13ad20
		.long   func_13ad2c
		.long   func_13ad38
		.long   func_13ad48
		.long   func_13ad58
		.long   func_13ad68
		.long   func_13ad78
		.long   func_13ad84
		.long   func_13ad90
		.long   func_13ad9c
		.long   func_13ada8
		.long   func_13adb4
		.long   func_13adc0
		.long   func_13add0
		.long   func_13addc
		.long   func_13ade4
		.long   func_13adf0
		.long   func_13adf8
		.long   func_13ae00
		.long   func_13ae0c
		.long   func_13ae18
		.long   func_13ae24
		.long   func_13ae30
		.long   func_13ae3c
		.long   func_13ae4c
		.long   func_13ae58
		.long   func_13ae64
		.long   func_13ae74
		.long   func_13ae80
		.long   func_13ae8c
		.long   func_13ae9c
		.long   func_13aea8
		.long   func_13aeb4
		.long   func_13aec0
		.long   func_13aecc
		.long   func_13aed8
		.long   func_13aee8
		.long   func_13aef4
		.long   func_13af00
		.long   func_13af0c
		.long   func_13af18
		.long   func_13af24
		.long   func_13af30
		.long   func_13af3c
		.long   func_13af44
		.long   func_13af50
		.long   func_13af5c
		.long   func_13af64
		.long   func_13af70
		.long   func_13af78
		.long   func_13af80
		.long   func_13af8c
		.long   func_13af98
		.long   func_13afa4
		.long   func_13afb0
		.long   func_13afbc
		.long   func_13afc8
		.long   func_13afd4
		.long   func_13afe0
		.long   func_13afec
		.long   func_13aff8
		.long   func_13b008
		.long   func_13b014
		.long   func_13b01c
		.long   func_13b028
		.long   func_13b030
		.long   func_13b038
		.long   func_13b044
		.long   func_13b050
		.long   func_13b05c
		.long   func_13b068
		.long   func_13b078
		.long   func_13b088
		.long   func_13b098
		.long   func_13b0a8
		.long   func_13b0b8
		.long   func_13b0c8
		.long   func_13b0d8
		.long   func_13b0e8
		.long   func_13b0f4
		.long   func_13b100
		.long   func_13b10c
		.long   func_13b118
		.long   func_13b124
		.long   func_13b130
		.long   func_13b13c
		.long   func_13b148
		.long   func_13b158
		.long   func_13b168
		.long   func_13b178
		.long   func_13b188
		.long   func_13b198
		.long   func_13b1a8
		.long   func_13b1b8
		.long   func_13b1c8
		.long   func_13b1d4
		.long   func_13b1e0
		.long   func_13b1ec
		.long   func_13b1f8
		.long   func_13b204
		.long   func_13b210
		.long   func_13b220
		.long   func_13b22c
		.long   func_13b23c
		.long   func_13b24c
		.long   func_13b25c
		.long   func_13b26c
		.long   func_13b27c
		.long   func_13b28c
		.long   func_13b29c
		.long   func_13b2ac
		.long   func_13b2b8
		.long   func_13b2c4
		.long   func_13b2d0
		.long   func_13b2dc
		.long   func_13b2e8
		.long   func_13b2f8
		.long   func_13b304
		.long   func_13b310
		.long   func_13b320
		.long   func_13b330
		.long   func_13b340
		.long   func_13b350
		.long   func_13b360
		.long   func_13b370
		.long   func_13b380
		.long   func_13b390
		.long   func_13b39c
		.long   func_13b3a8
		.long   func_13b3b4
		.long   func_13b3c0
		.long   func_13b3cc
		.long   func_13b3d8
		.long   func_13b3e4


data_159e00:
		.long	0x0
data_159e04:
		.long	0x0
data_159e08:
		.long	0x0
data_159e0c:
		.long	0x0
data_159e10:
		.long	0x0
data_159e14:
		.fill   0xc
data_159e20:
		.long	0x0
data_159e24:
		.long	0x0
data_159e28:
		.long	0x0
data_159e2c:
		.long	0x0
data_159e30:
		.fill   0x10
