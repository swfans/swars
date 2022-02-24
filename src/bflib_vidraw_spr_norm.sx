
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

.global EXPORT_SYMBOL(lbDisplay__DrawFlags);
.global EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr);
.global EXPORT_SYMBOL(data_1f0354);
.global EXPORT_SYMBOL(data_1f0754);
.global EXPORT_SYMBOL(data_1f0350);
.global EXPORT_SYMBOL(_ghost_table);

/*----------------------------------------------------------------*/
/* void LbSpriteDrawUsingScalingData(int32_t posx, int32_t posy, TbSprite *sprite)
 */
GLOBAL_FUNC (LbSpriteDrawUsingScalingData_)
/*----------------------------------------------------------------*/
		pusha
		sub    $0x280,%esp
		mov    $0x64,%ecx
		sub    $0x17,%ecx
		and    %edx,%ecx
		xor    %ecx,%ecx
		mov    0x4(%ebx),%cl
		mov    %ecx,0x24(%esp)
		mov    0x5(%ebx),%cl
		mov    %ecx,0x28(%esp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%esi
		mov    $0x8,%edi
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cx
		and    $0x1,%cx
		je     jump_1351ce
		add    0x24(%esp),%eax
		dec    %eax
	jump_1351ce:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cx
		and    $0x2,%cx
		je     jump_1351e4
		add    0x28(%esp),%edx
		dec    %edx
		neg    %esi
		neg    %edi
	jump_1351e4:
		mov    %esi,0x2c(%esp)
		mov    %edi,0x30(%esp)
		lea    EXPORT_SYMBOL(data_1f0354)(,%eax,8),%ebp
		mov    %ebp,0x4(%esp)
		lea    EXPORT_SYMBOL(data_1f0754)(,%edx,8),%edi
		mov    EXPORT_SYMBOL(data_1e2ecc),%eax
		mov    %eax,0x20(%esp)
		mov    0x28(%esp),%ecx
		mov    (%ebx),%edx
		mov    (%edi),%esi
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x2,%ax
		je     jump_13521f
		add    0x4(%edi),%esi
		dec    %esi
	jump_13521f:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		imul   %eax,%esi
		add    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%esi
		add    0x0(%ebp),%esi
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		je     jump_135240
		add    0x4(%ebp),%esi
		dec    %esi
	jump_135240:
		mov    EXPORT_SYMBOL(data_1f0350),%eax
		or     %eax,%eax
		jne    jump_135cd8
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x8,%ax
		jne    jump_135968
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x4,%ax
		jne    jump_135598
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_135408
		jmp    jump_1352cb
	jump_135280:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135284:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1352c0
		js     jump_135284
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1352c0
		js     jump_135284
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1352c0
		js     jump_135284
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1352c0
		js     jump_135284
		add    %eax,%edx
		jmp    jump_135284
	jump_1352c0:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135400
	jump_1352cb:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_135280
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
	jump_1352e2:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_1353e0
		js     jump_1353c0
		jmp    jump_135307
	jump_135300:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_1352e2
	jump_135307:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135300
		mov    (%edx),%al
		inc    %edx
		add    $0x8,%ebp
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1352e2
		jmp    jump_135307
	jump_1353c0:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_1352e2
	jump_1353e0:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_1352c0
	jump_135400:
		add    $0x280,%esp
		popa
		ret
	jump_135408:
		jmp    jump_13545b
	jump_135410:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135414:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135450
		js     jump_135414
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135450
		js     jump_135414
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135450
		js     jump_135414
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135450
		js     jump_135414
		add    %eax,%edx
		jmp    jump_135414
	jump_135450:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135590
	jump_13545b:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_135410
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
	jump_135472:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135570
		js     jump_135550
		jmp    jump_135497
	jump_135490:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_135472
	jump_135497:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135490
		mov    (%edx),%al
		inc    %edx
		sub    $0x8,%ebp
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135472
		jmp    jump_135497
	jump_135550:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_135472
	jump_135570:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_135450
	jump_135590:
		add    $0x280,%esp
		popa
		ret
	jump_135598:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_135788
		jmp    jump_1355fb
	jump_1355b0:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_1355b4:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1355f0
		js     jump_1355b4
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1355f0
		js     jump_1355b4
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1355f0
		js     jump_1355b4
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1355f0
		js     jump_1355b4
		add    %eax,%edx
		jmp    jump_1355b4
	jump_1355f0:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135780
	jump_1355fb:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_1355b0
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
	jump_135612:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135760
		js     jump_135740
		jmp    jump_135637
	jump_135630:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_135612
	jump_135637:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135630
		mov    (%esi),%al
		mov    (%edx),%ah
		add    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135612
		jmp    jump_135637
	jump_135740:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_135612
	jump_135760:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_1355f0
	jump_135780:
		add    $0x280,%esp
		popa
		ret
	jump_135788:
		jmp    jump_1357db
	jump_135790:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135794:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1357d0
		js     jump_135794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1357d0
		js     jump_135794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1357d0
		js     jump_135794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1357d0
		js     jump_135794
		add    %eax,%edx
		jmp    jump_135794
	jump_1357d0:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135960
	jump_1357db:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_135790
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
	jump_1357f2:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135940
		js     jump_135920
		jmp    jump_135817
	jump_135810:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_1357f2
	jump_135817:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135810
		mov    (%esi),%al
		mov    (%edx),%ah
		sub    $0x8,%ebp
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_1357f2
		jmp    jump_135817
	jump_135920:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_1357f2
	jump_135940:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_1357d0
	jump_135960:
		add    $0x280,%esp
		popa
		ret
	jump_135968:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_135b28
		jmp    jump_1359cb
	jump_135980:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135984:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1359c0
		js     jump_135984
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1359c0
		js     jump_135984
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1359c0
		js     jump_135984
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1359c0
		js     jump_135984
		add    %eax,%edx
		jmp    jump_135984
	jump_1359c0:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135b20
	jump_1359cb:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_135980
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		mov    0x20(%esp),%edi
	jump_1359e6:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135b00
		js     jump_135ae0
		jmp    jump_135a07
	jump_135a00:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_1359e6
	jump_135a07:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135a00
		mov    (%edx),%al
		add    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_1359e6
		jmp    jump_135a07
	jump_135ae0:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_1359e6
	jump_135b00:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_1359c0
	jump_135b20:
		add    $0x280,%esp
		popa
		ret
	jump_135b28:
		jmp    jump_135b7b
	jump_135b30:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135b34:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135b70
		js     jump_135b34
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135b70
		js     jump_135b34
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135b70
		js     jump_135b34
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135b70
		js     jump_135b34
		add    %eax,%edx
		jmp    jump_135b34
	jump_135b70:
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135cd0
	jump_135b7b:
		mov    0x4(%edi),%eax
		or     %eax,%eax
		je     jump_135b30
		mov    %esi,0xc(%esp)
		mov    %ecx,0x10(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		mov    0x20(%esp),%edi
	jump_135b96:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135cb0
		js     jump_135c90
		jmp    jump_135bb7
	jump_135bb0:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_135b96
	jump_135bb7:
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		mov    0x4(%ebp),%eax
		or     %eax,%eax
		je     jump_135bb0
		mov    (%edx),%al
		sub    $0x8,%ebp
		mov    (%edi,%eax,1),%al
		inc    %edx
		mov    %al,(%esi)
		dec    %esi
		dec    %ecx
		je     jump_135b96
		jmp    jump_135bb7
	jump_135c90:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_135b96
	jump_135cb0:
		mov    0xc(%esp),%esi
		mov    0x10(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		jmp    jump_135b70
	jump_135cd0:
		add    $0x280,%esp
		popa
		ret
	jump_135cd8:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x8,%ax
		jne    jump_1364f8
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x4,%ax
		jne    jump_136208
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_135f88
		jmp    jump_135d5f
	jump_135d10:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135d14:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135d50
		js     jump_135d14
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135d50
		js     jump_135d14
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135d50
		js     jump_135d14
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135d50
		js     jump_135d14
		add    %eax,%edx
		jmp    jump_135d14
	jump_135d50:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_135f80
	jump_135d5f:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		mov    %ecx,0x3c(%esp)
		or     %ecx,%ecx
		je     jump_135d10
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		jmp    jump_135eac
	jump_135d84:
		mov    0x3c(%esp),%ebx
		dec    %ebx
		je     jump_135eac
		js     jump_135eac
		mov    0x34(%esp),%edi
		mov    %esi,0x38(%esp)
		mov    %esi,%ecx
		sub    %edi,%ecx
		mov    %edi,%esi
		add    0x2c(%esp),%esi
		mov    %ecx,0x40(%esp)
		nop
	jump_135dac:
		mov    %esi,0x44(%esp)
		or     %ecx,%ecx
		je     jump_135e8f
		test   $0x3,%esi
		je     jump_135df8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135e8f
		test   $0x3,%esi
		je     jump_135df8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135e8f
		test   $0x3,%esi
		je     jump_135df8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_135e8f
		nop
	jump_135df8:
		mov    %ecx,0x48(%esp)
		sar    $0x2,%ecx
		je     jump_135e70
	jump_135e01:
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_135e70
		jmp    jump_135e01
	jump_135e70:
		mov    0x48(%esp),%ecx
		and    $0x3,%ecx
		je     jump_135e8f
		mov    (%edi),%al
		mov    %al,(%esi)
		dec    %ecx
		je     jump_135e8f
		mov    0x1(%edi),%al
		mov    %al,0x1(%esi)
		dec    %ecx
		je     jump_135e8f
		mov    0x2(%edi),%al
		mov    %al,0x2(%esi)
	jump_135e8f:
		mov    0x34(%esp),%edi
		mov    0x40(%esp),%ecx
		mov    0x44(%esp),%esi
		add    0x2c(%esp),%esi
		dec    %ebx
		jne    jump_135dac
		mov    0x38(%esp),%esi
		mov    %eax,%eax
	jump_135eac:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_135f50
		js     jump_135f30
		mov    %esi,0x34(%esp)
		jmp    jump_135ede
	jump_135ed0:
		add    0x4(%ebp),%esi
	jump_135ed3:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_135d84
	jump_135ede:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_135ed3
		mov    (%edx),%al
		mov    %al,(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x1(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x2(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x3(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x4(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x5(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x6(%esi)
		dec    %ebx
		je     jump_135ed0
		mov    %al,0x7(%esi)
		dec    %ebx
		je     jump_135ed0
		add    $0x8,%esi
		cs
		mov    %eax,%eax
	jump_135f1c:
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		jne    jump_135f1c
		jmp    jump_135ed3
	jump_135f30:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_135eac
	jump_135f50:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		je     jump_135d50
	jump_135f67:
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_135f67
		jmp    jump_135d50
	jump_135f80:
		add    $0x280,%esp
		popa
		ret
	jump_135f88:
		jmp    jump_135fdf
	jump_135f90:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_135f94:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135fd0
		js     jump_135f94
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135fd0
		js     jump_135f94
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135fd0
		js     jump_135f94
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_135fd0
		js     jump_135f94
		add    %eax,%edx
		jmp    jump_135f94
	jump_135fd0:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_136200
	jump_135fdf:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		mov    %ecx,0x3c(%esp)
		or     %ecx,%ecx
		je     jump_135f90
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		jmp    jump_13612c
	jump_136004:
		mov    0x3c(%esp),%ebx
		dec    %ebx
		je     jump_13612c
		js     jump_13612c
		mov    0x34(%esp),%edi
		mov    %esi,0x38(%esp)
		mov    %edi,%ecx
		sub    %esi,%ecx
		mov    %esi,%edi
		mov    %edi,0x34(%esp)
		inc    %ecx
		add    0x2c(%esp),%esi
		mov    %ecx,0x40(%esp)
	jump_136030:
		mov    %esi,0x44(%esp)
		or     %ecx,%ecx
		je     jump_13610f
		test   $0x3,%esi
		je     jump_13607c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13610f
		test   $0x3,%esi
		je     jump_13607c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13610f
		test   $0x3,%esi
		je     jump_13607c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13610f
		nop
	jump_13607c:
		mov    %ecx,0x48(%esp)
		sar    $0x2,%ecx
		je     jump_1360f0
	jump_136085:
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1360f0
		jmp    jump_136085
	jump_1360f0:
		mov    0x48(%esp),%ecx
		and    $0x3,%ecx
		je     jump_13610f
		mov    (%edi),%al
		mov    %al,(%esi)
		dec    %ecx
		je     jump_13610f
		mov    0x1(%edi),%al
		mov    %al,0x1(%esi)
		dec    %ecx
		je     jump_13610f
		mov    0x2(%edi),%al
		mov    %al,0x2(%esi)
	jump_13610f:
		mov    0x34(%esp),%edi
		mov    0x40(%esp),%ecx
		mov    0x44(%esp),%esi
		add    0x2c(%esp),%esi
		dec    %ebx
		jne    jump_136030
		mov    0x38(%esp),%esi
		mov    %eax,%eax
	jump_13612c:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_1361d0
		js     jump_1361b0
		mov    %esi,0x34(%esp)
		jmp    jump_13615e
	jump_136150:
		sub    0x4(%ebp),%esi
	jump_136153:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_136004
	jump_13615e:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_136153
		mov    (%edx),%al
		mov    %al,(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x1(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x2(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x3(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x4(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x5(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x6(%esi)
		dec    %ebx
		je     jump_136150
		mov    %al,-0x7(%esi)
		dec    %ebx
		je     jump_136150
		sub    $0x8,%esi
		cs
		mov    %eax,%eax
	jump_13619c:
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		jne    jump_13619c
		jmp    jump_136153
	jump_1361b0:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_13612c
	jump_1361d0:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		je     jump_135fd0
	jump_1361e7:
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_1361e7
		jmp    jump_135fd0
	jump_136200:
		add    $0x280,%esp
		popa
		ret
	jump_136208:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_136388
		jmp    jump_13626f
	jump_136220:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_136224:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136260
		js     jump_136224
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136260
		js     jump_136224
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136260
		js     jump_136224
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136260
		js     jump_136224
		add    %eax,%edx
		jmp    jump_136224
	jump_136260:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_136380
	jump_13626f:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		or     %ecx,%ecx
		mov    %edx,0x18(%esp)
		je     jump_136220
	jump_13627e:
		mov    0x18(%esp),%edx
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		mov    %eax,%eax
	jump_136294:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_136360
		js     jump_136340
		jmp    jump_1362b7
	jump_1362b0:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_136294
	jump_1362b7:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_1362b0
		mov    (%edx),%ah
	jump_1362c0:
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		je     jump_1362b0
		jmp    jump_1362c0
	jump_136340:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_136294
	jump_136360:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_13627e
		jmp    jump_136260
	jump_136380:
		add    $0x280,%esp
		popa
		ret
	jump_136388:
		jmp    jump_1363df
	jump_136390:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_136394:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1363d0
		js     jump_136394
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1363d0
		js     jump_136394
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1363d0
		js     jump_136394
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1363d0
		js     jump_136394
		add    %eax,%edx
		jmp    jump_136394
	jump_1363d0:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_1364f0
	jump_1363df:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		or     %ecx,%ecx
		mov    %edx,0x18(%esp)
		je     jump_136390
	jump_1363ee:
		mov    0x18(%esp),%edx
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		mov    %eax,%eax
	jump_136404:
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_1364d0
		js     jump_1364b0
		jmp    jump_136427
	jump_136420:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_136404
	jump_136427:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_136420
		mov    (%edx),%ah
	jump_136430:
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		mov    (%esi),%al
		mov    EXPORT_SYMBOL(_ghost_table)(%eax),%al
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		je     jump_136420
		jmp    jump_136430
	jump_1364b0:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_136404
	jump_1364d0:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_1363ee
		jmp    jump_1363d0
	jump_1364f0:
		add    $0x280,%esp
		popa
		ret
	jump_1364f8:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ax
		and    $0x1,%ax
		jne    jump_136788
		jmp    jump_13655f
	jump_136510:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_136514:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136550
		js     jump_136514
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136550
		js     jump_136514
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136550
		js     jump_136514
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_136550
		js     jump_136514
		add    %eax,%edx
		jmp    jump_136514
	jump_136550:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_136780
	jump_13655f:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		mov    %ecx,0x3c(%esp)
		or     %ecx,%ecx
		je     jump_136510
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		jmp    jump_1366ac
	jump_136584:
		mov    0x3c(%esp),%ebx
		dec    %ebx
		je     jump_1366ac
		js     jump_1366ac
		mov    0x34(%esp),%edi
		mov    %esi,0x38(%esp)
		mov    %esi,%ecx
		sub    %edi,%ecx
		mov    %edi,%esi
		add    0x2c(%esp),%esi
		mov    %ecx,0x40(%esp)
		nop
	jump_1365ac:
		mov    %esi,0x44(%esp)
		or     %ecx,%ecx
		je     jump_13668f
		test   $0x3,%esi
		je     jump_1365f8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13668f
		test   $0x3,%esi
		je     jump_1365f8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13668f
		test   $0x3,%esi
		je     jump_1365f8
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13668f
		nop
	jump_1365f8:
		mov    %ecx,0x48(%esp)
		sar    $0x2,%ecx
		je     jump_136670
	jump_136601:
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_136670
		jmp    jump_136601
	jump_136670:
		mov    0x48(%esp),%ecx
		and    $0x3,%ecx
		je     jump_13668f
		mov    (%edi),%al
		mov    %al,(%esi)
		dec    %ecx
		je     jump_13668f
		mov    0x1(%edi),%al
		mov    %al,0x1(%esi)
		dec    %ecx
		je     jump_13668f
		mov    0x2(%edi),%al
		mov    %al,0x2(%esi)
	jump_13668f:
		mov    0x34(%esp),%edi
		mov    0x40(%esp),%ecx
		mov    0x44(%esp),%esi
		add    0x2c(%esp),%esi
		dec    %ebx
		jne    jump_1365ac
		mov    0x38(%esp),%esi
		mov    %eax,%eax
	jump_1366ac:
		mov    0x20(%esp),%edi
		xor    %eax,%eax
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_136750
		js     jump_136730
		mov    %esi,0x34(%esp)
		jmp    jump_1366de
	jump_1366d0:
		add    0x4(%ebp),%esi
	jump_1366d3:
		inc    %edx
		add    $0x8,%ebp
		dec    %ecx
		je     jump_136584
	jump_1366de:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_1366d3
		mov    (%edx),%al
		mov    (%edi,%eax,1),%al
		mov    %al,(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x1(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x2(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x3(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x4(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x5(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x6(%esi)
		dec    %ebx
		je     jump_1366d0
		mov    %al,0x7(%esi)
		dec    %ebx
		je     jump_1366d0
		add    $0x8,%esi
	jump_13671c:
		mov    %al,(%esi)
		inc    %esi
		dec    %ebx
		jne    jump_13671c
		jmp    jump_1366d3
	jump_136730:
		neg    %cl
		sub    0x0(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x0(%ebp),%esi
		jmp    jump_1366ac
	jump_136750:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		je     jump_136550
	jump_136767:
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_136767
		jmp    jump_136550
	jump_136780:
		add    $0x280,%esp
		popa
		ret
	jump_136788:
		jmp    jump_1367df
	jump_136790:
		xor    %eax,%eax
		mov    %eax,%eax
	jump_136794:
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1367d0
		js     jump_136794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1367d0
		js     jump_136794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1367d0
		js     jump_136794
		add    %eax,%edx
		mov    (%edx),%al
		inc    %edx
		or     %al,%al
		je     jump_1367d0
		js     jump_136794
		add    %eax,%edx
		jmp    jump_136794
	jump_1367d0:
		mov    0x10(%esp),%ecx
		add    0x30(%esp),%edi
		dec    %ecx
		je     jump_136a00
	jump_1367df:
		mov    %ecx,0x10(%esp)
		mov    0x4(%edi),%ecx
		mov    %ecx,0x3c(%esp)
		or     %ecx,%ecx
		je     jump_136790
		mov    %esi,0xc(%esp)
		mov    %ecx,0x1c(%esp)
		mov    %edi,0x8(%esp)
		mov    0x4(%esp),%ebp
		jmp    jump_13692c
	jump_136804:
		mov    0x3c(%esp),%ebx
		dec    %ebx
		je     jump_13692c
		js     jump_13692c
		mov    0x34(%esp),%edi
		mov    %esi,0x38(%esp)
		mov    %edi,%ecx
		sub    %esi,%ecx
		mov    %esi,%edi
		mov    %edi,0x34(%esp)
		inc    %ecx
		add    0x2c(%esp),%esi
		mov    %ecx,0x40(%esp)
	jump_136830:
		mov    %esi,0x44(%esp)
		or     %ecx,%ecx
		je     jump_13690f
		test   $0x3,%esi
		je     jump_13687c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13690f
		test   $0x3,%esi
		je     jump_13687c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13690f
		test   $0x3,%esi
		je     jump_13687c
		mov    (%edi),%al
		inc    %edi
		mov    %al,(%esi)
		inc    %esi
		dec    %ecx
		je     jump_13690f
		nop
	jump_13687c:
		mov    %ecx,0x48(%esp)
		sar    $0x2,%ecx
		je     jump_1368f0
	jump_136885:
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		mov    (%edi),%eax
		add    $0x4,%edi
		mov    %eax,(%esi)
		add    $0x4,%esi
		dec    %ecx
		je     jump_1368f0
		jmp    jump_136885
	jump_1368f0:
		mov    0x48(%esp),%ecx
		and    $0x3,%ecx
		je     jump_13690f
		mov    (%edi),%al
		mov    %al,(%esi)
		dec    %ecx
		je     jump_13690f
		mov    0x1(%edi),%al
		mov    %al,0x1(%esi)
		dec    %ecx
		je     jump_13690f
		mov    0x2(%edi),%al
		mov    %al,0x2(%esi)
	jump_13690f:
		mov    0x34(%esp),%edi
		mov    0x40(%esp),%ecx
		mov    0x44(%esp),%esi
		add    0x2c(%esp),%esi
		dec    %ebx
		jne    jump_136830
		mov    0x38(%esp),%esi
		mov    %eax,%eax
	jump_13692c:
		mov    0x20(%esp),%edi
		xor    %eax,%eax
		xor    %ecx,%ecx
		mov    (%edx),%cl
		inc    %edx
		or     %cl,%cl
		je     jump_1369d0
		js     jump_1369b0
		mov    %esi,0x34(%esp)
		jmp    jump_13695e
	jump_136950:
		sub    0x4(%ebp),%esi
	jump_136953:
		inc    %edx
		sub    $0x8,%ebp
		dec    %ecx
		je     jump_136804
	jump_13695e:
		mov    0x4(%ebp),%ebx
		or     %ebx,%ebx
		je     jump_136953
		mov    (%edx),%al
		mov    (%edi,%eax,1),%al
		mov    %al,(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x1(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x2(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x3(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x4(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x5(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x6(%esi)
		dec    %ebx
		je     jump_136950
		mov    %al,-0x7(%esi)
		dec    %ebx
		je     jump_136950
		sub    $0x8,%esi
	jump_13699c:
		mov    %al,(%esi)
		dec    %esi
		dec    %ebx
		jne    jump_13699c
		jmp    jump_136953
	jump_1369b0:
		neg    %cl
		sub    0x0(%ebp),%esi
		neg    %ecx
		sub    0x4(%ebp),%esi
		lea    0x0(%ebp,%ecx,8),%ebp
		add    0x4(%ebp),%esi
		neg    %ecx
		add    0x0(%ebp),%esi
		jmp    jump_13692c
	jump_1369d0:
		mov    0xc(%esp),%esi
		mov    0x1c(%esp),%ecx
		mov    0x8(%esp),%edi
		add    0x2c(%esp),%esi
		dec    %ecx
		je     jump_1367d0
	jump_1369e7:
		add    0x2c(%esp),%esi
		dec    %ecx
		jne    jump_1369e7
		jmp    jump_1367d0
	jump_136a00:
		add    $0x280,%esp
		popa
		ret

.section .rodata

.data

