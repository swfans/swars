
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

.global EXPORT_SYMBOL(lbDisplay__WScreen);
.global EXPORT_SYMBOL(lbDisplay__DrawFlags);
.global EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowX);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowY);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight);
.global EXPORT_SYMBOL(lbDisplay__GlassMap);
.global EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr);


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbSpriteDraw_)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x88,%esp
		sub    $0x82,%ebp
		mov    %eax,-0x6(%ebp)
		mov    %edx,-0x2(%ebp)
		mov    %ebx,0x2(%ebp)
		mov    0x2(%ebp),%eax
		cmpb   $0x0,0x4(%eax)
		je     jump_e5b2d
		mov    0x2(%ebp),%eax
		cmpb   $0x0,0x5(%eax)
		jne    jump_e5b37
	jump_e5b2d:
		mov    $0x1,%eax
		jmp    jump_e66de
	jump_e5b37:
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth)
		je     jump_e5b49
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight)
		jne    jump_e5b59
	jump_e5b49:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5b59:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		add    %eax,-0x6(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		add    %eax,-0x2(%ebp)
		mov    0x2(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,0x36(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		mov    -0x6(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,0x16(%ebp)
		cmpl   $0x0,0x16(%ebp)
		jle    jump_e5ba9
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    0x16(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e5ba1
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5ba1:
		mov    0x16(%ebp),%al
		mov    %al,0x5a(%ebp)
		jmp    jump_e5bae
	jump_e5ba9:
		xor    %bl,%bl
		mov    %bl,0x5a(%ebp)
	jump_e5bae:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%ebx
		xor    %ecx,%ecx
		add    %ebx,%eax
		mov    0x36(%ebp),%cl
		add    -0x6(%ebp),%ecx
		sub    %eax,%ecx
		mov    %ecx,0x16(%ebp)
		cmpl   $0x0,0x16(%ebp)
		jle    jump_e5bf4
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    0x16(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e5bea
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5bea:
		mov    0x36(%ebp),%al
		mov    0x16(%ebp),%bh
		sub    %bh,%al
		jmp    jump_e5bf7
	jump_e5bf4:
		mov    0x36(%ebp),%al
	jump_e5bf7:
		mov    %al,0x66(%ebp)
		mov    0x2(%ebp),%eax
		mov    0x5(%eax),%al
		mov    %al,0x32(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		mov    -0x2(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x16(%ebp)
		cmpl   $0x0,0x16(%ebp)
		jle    jump_e5c3a
		xor    %eax,%eax
		mov    0x32(%ebp),%al
		mov    0x16(%ebp),%esi
		cmp    %esi,%eax
		jg     jump_e5c32
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5c32:
		mov    0x16(%ebp),%al
		mov    %al,0x3a(%ebp)
		jmp    jump_e5c3f
	jump_e5c3a:
		xor    %cl,%cl
		mov    %cl,0x3a(%ebp)
	jump_e5c3f:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		xor    %ecx,%ecx
		add    %edi,%eax
		mov    0x32(%ebp),%cl
		add    -0x2(%ebp),%ecx
		sub    %eax,%ecx
		mov    %ecx,0x16(%ebp)
		cmpl   $0x0,0x16(%ebp)
		jle    jump_e5c85
		xor    %eax,%eax
		mov    0x32(%ebp),%al
		mov    0x16(%ebp),%ecx
		cmp    %ecx,%eax
		jg     jump_e5c7b
	jump_e5c6b:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5c7b:
		mov    0x32(%ebp),%al
		mov    0x16(%ebp),%ch
		sub    %ch,%al
		jmp    jump_e5c88
	jump_e5c85:
		mov    0x32(%ebp),%al
	jump_e5c88:
		mov    %al,0x62(%ebp)
		testb  $0x2,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e5ce1
		xor    %eax,%eax
		mov    0x62(%ebp),%al
		add    -0x2(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		dec    %eax
		imul   %edi,%eax
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%ecx
		add    %ecx,%eax
		mov    -0x6(%ebp),%ecx
		add    %eax,%ecx
		xor    %eax,%eax
		mov    0x5a(%ebp),%al
		add    %eax,%ecx
		mov    %ecx,0x6(%ebp)
		mov    %edi,0x1e(%ebp)
		negl   0x1e(%ebp)
		mov    0x62(%ebp),%al
		mov    %al,0x4a(%ebp)
		mov    0x32(%ebp),%al
		mov    0x3a(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x62(%ebp)
		mov    0x32(%ebp),%al
		mov    0x4a(%ebp),%dl
		sub    %dl,%al
		mov    %al,0x3a(%ebp)
		jmp    jump_e5d12
	jump_e5ce1:
		xor    %eax,%eax
		mov    0x3a(%ebp),%al
		mov    -0x2(%ebp),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ecx
		add    %edx,%eax
		imul   %eax,%ecx
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%eax
		add    %ecx,%eax
		mov    -0x6(%ebp),%ecx
		add    %eax,%ecx
		xor    %eax,%eax
		mov    0x5a(%ebp),%al
		add    %eax,%ecx
		mov    %ecx,0x6(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		mov    %eax,0x1e(%ebp)
	jump_e5d12:
		mov    0x62(%ebp),%al
		mov    0x3a(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x32(%ebp)
		mov    0x66(%ebp),%al
		mov    0x5a(%ebp),%bl
		sub    %bl,%al
		mov    %al,0x36(%ebp)
		mov    0x2(%ebp),%eax
		mov    (%eax),%eax
		mov    %eax,0xe(%ebp)
		cmpb   $0x0,0x3a(%ebp)
		je     jump_e5d65
		xor    %eax,%eax
		mov    0x3a(%ebp),%al
		mov    %eax,0x12(%ebp)
	jump_e5d3e:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e5d4e
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e5d3e
	jump_e5d4e:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e5d3e
		decl   0x12(%ebp)
		mov    0x12(%ebp),%edi
		test   %edi,%edi
		jne    jump_e5d3e
	jump_e5d65:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0xd,%cl
		je     jump_e64bd
		test   $0x1,%cl
		je     jump_e5dad
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		dec    %eax
		add    %eax,0x6(%ebp)
		movb   $0x1,0x76(%ebp)
		mov    0x2(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,0x7a(%ebp)
		mov    0x66(%ebp),%al
		mov    %al,0x52(%ebp)
		mov    0x7a(%ebp),%al
		mov    0x5a(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x66(%ebp)
		mov    0x7a(%ebp),%al
		mov    0x52(%ebp),%dl
		sub    %dl,%al
		mov    %al,0x5a(%ebp)
		jmp    jump_e5db2
	jump_e5dad:
		xor    %dh,%dh
		mov    %dh,0x76(%ebp)
	jump_e5db2:
		mov    0x6(%ebp),%eax
		mov    0x1e(%ebp),%ebx
		add    %ebx,%eax
		mov    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
		xor    %eax,%eax
		mov    0x32(%ebp),%al
		mov    %eax,0x12(%ebp)
		testb  $0xc,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e628a
		mov    0x5a(%ebp),%bh
		test   %bh,%bh
		jne    jump_e6020
	jump_e5de5:
		mov    0x1a(%ebp),%ecx
		test   %ecx,%ecx
		jle    jump_e5fd5
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e5f7e
		mov    0x1a(%ebp),%esi
		cmp    %esi,%eax
		jl     jump_e5e08
		mov    0x1a(%ebp),%eax
	jump_e5e08:
		mov    %al,0x56(%ebp)
		mov    0xe(%ebp),%eax
		inc    %eax
		mov    %eax,0x22(%ebp)
		cmpb   $0x0,0x76(%ebp)
		je     jump_e5ec3
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ch
		test   $0x4,%ch
		je     jump_e5e75
		xor    %al,%al
		mov    %al,0x7e(%ebp)
		jmp    jump_e5e40
	jump_e5e2e:
		mov    0x7e(%ebp),%al
		incb   0x7e(%ebp)
		mov    0x22(%ebp),%eax
		incl   0x22(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e5e40:
		mov    0x7e(%ebp),%al
		cmp    0x56(%ebp),%al
		jae    jump_e5f66
		mov    0x22(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%al
		mov    0x6(%ebp),%ecx
		mov    %al,(%ecx)
		jmp    jump_e5e2e
	jump_e5e75:
		xor    %dh,%dh
		mov    %dh,0x7e(%ebp)
		jmp    jump_e5e8e
	jump_e5e7c:
		mov    0x7e(%ebp),%al
		incb   0x7e(%ebp)
		mov    0x22(%ebp),%eax
		incl   0x22(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e5e8e:
		mov    0x7e(%ebp),%al
		cmp    0x56(%ebp),%al
		jae    jump_e5f66
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x22(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%al
		mov    0x6(%ebp),%ecx
		mov    %al,(%ecx)
		jmp    jump_e5e7c
	jump_e5ec3:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0x4,%cl
		je     jump_e5f1c
		xor    %ch,%ch
		mov    %ch,0x7e(%ebp)
		jmp    jump_e5ee7
	jump_e5ed5:
		mov    0x7e(%ebp),%al
		incb   0x7e(%ebp)
		mov    0x22(%ebp),%eax
		incl   0x22(%ebp)
		mov    0x6(%ebp),%eax
		incl   0x6(%ebp)
	jump_e5ee7:
		mov    0x7e(%ebp),%al
		cmp    0x56(%ebp),%al
		jae    jump_e5f66
		mov    0x22(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%al
		mov    0x6(%ebp),%ecx
		mov    %al,(%ecx)
		jmp    jump_e5ed5
	jump_e5f1c:
		xor    %dl,%dl
		mov    %dl,0x7e(%ebp)
		jmp    jump_e5f35
	jump_e5f23:
		mov    0x7e(%ebp),%al
		incb   0x7e(%ebp)
		mov    0x22(%ebp),%eax
		incl   0x22(%ebp)
		mov    0x6(%ebp),%eax
		incl   0x6(%ebp)
	jump_e5f35:
		mov    0x7e(%ebp),%al
		cmp    0x56(%ebp),%al
		jae    jump_e5f66
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x22(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%cl
		mov    0x6(%ebp),%eax
		mov    %cl,(%eax)
		jmp    jump_e5f23
	jump_e5f66:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e5de5
	jump_e5f7e:
		test   %eax,%eax
		jge    jump_e5fab
		add    %eax,0x1a(%ebp)
		mov    0x76(%ebp),%bh
		test   %bh,%bh
		je     jump_e5f97
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,0x6(%ebp)
		jmp    jump_e5fa0
	jump_e5f97:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x6(%ebp)
	jump_e5fa0:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e5de5
	jump_e5fab:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%ebx
		test   %ebx,%ebx
		je     jump_e5c6b
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e6020
	jump_e5fd5:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%eax
		test   %eax,%eax
		jne    jump_e5fef
	jump_e5fdf:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e5fef:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e5fff
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e5fef
	jump_e5fff:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e5fef
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
	jump_e6020:
		cmpb   $0x0,0x5a(%ebp)
		je     jump_e5de5
		mov    0x5a(%ebp),%al
		mov    %al,0x72(%ebp)
	jump_e6030:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6205
		xor    %ecx,%ecx
		mov    0x72(%ebp),%cl
		cmp    %ecx,%eax
		jle    jump_e61e3
		xor    %eax,%eax
		mov    0x72(%ebp),%al
		mov    0xe(%ebp),%ecx
		inc    %ecx
		add    %eax,%ecx
		mov    %ecx,0x26(%ebp)
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		mov    0x72(%ebp),%ch
		sub    %ch,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%ah
		cmp    %ah,%al
		jbe    jump_e6076
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e6076:
		cmpb   $0x0,0x76(%ebp)
		je     jump_e6129
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dh
		test   $0x4,%dh
		je     jump_e60d9
		xor    %bl,%bl
		mov    %bl,0x4e(%ebp)
		jmp    jump_e60a4
	jump_e6092:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0x26(%ebp),%eax
		incl   0x26(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e60a4:
		mov    0x4e(%ebp),%al
		cmp    0x42(%ebp),%al
		jae    jump_e61cc
		mov    0x26(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%cl
		mov    0x6(%ebp),%eax
		mov    %cl,(%eax)
		jmp    jump_e6092
	jump_e60d9:
		xor    %ch,%ch
		mov    %ch,0x4e(%ebp)
		jmp    jump_e60f2
	jump_e60e0:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0x26(%ebp),%eax
		incl   0x26(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e60f2:
		mov    0x4e(%ebp),%al
		cmp    0x42(%ebp),%al
		jae    jump_e61cc
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		mov    %eax,%ecx
		and    $0xff,%ecx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%eax
		shl    $0x8,%ecx
		add    %eax,%ecx
		mov    0x26(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%cl
		mov    0x6(%ebp),%eax
		mov    %cl,(%eax)
		jmp    jump_e60e0
	jump_e6129:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dl
		test   $0x4,%dl
		je     jump_e6182
		xor    %dh,%dh
		mov    %dh,0x4e(%ebp)
		jmp    jump_e614d
	jump_e613b:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0x26(%ebp),%eax
		incl   0x26(%ebp)
		mov    0x6(%ebp),%eax
		incl   0x6(%ebp)
	jump_e614d:
		mov    0x4e(%ebp),%al
		cmp    0x42(%ebp),%al
		jae    jump_e61cc
		mov    0x26(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%al
		mov    0x6(%ebp),%ecx
		mov    %al,(%ecx)
		jmp    jump_e613b
	jump_e6182:
		xor    %cl,%cl
		mov    %cl,0x4e(%ebp)
		jmp    jump_e619b
	jump_e6189:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0x26(%ebp),%eax
		incl   0x26(%ebp)
		mov    0x6(%ebp),%eax
		incl   0x6(%ebp)
	jump_e619b:
		mov    0x4e(%ebp),%al
		cmp    0x42(%ebp),%al
		jae    jump_e61cc
		mov    0x6(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ecx
		shl    $0x8,%eax
		add    %eax,%ecx
		mov    0x26(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%ecx,%eax,1),%cl
		mov    0x6(%ebp),%eax
		mov    %cl,(%eax)
		jmp    jump_e6189
	jump_e61cc:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e5de5
	jump_e61e3:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x72(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		mov    0x72(%ebp),%ch
		test   %ch,%ch
		jne    jump_e6030
		jmp    jump_e5de5
	jump_e6205:
		test   %eax,%eax
		jne    jump_e6211
		mov    %eax,0x1a(%ebp)
		jmp    jump_e5de5
	jump_e6211:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%ecx
		xor    %eax,%eax
		mov    0x72(%ebp),%al
		neg    %eax
		cmp    %eax,%ecx
		jge    jump_e626c
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x72(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e6241
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e6241:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		cmpb   $0x0,0x76(%ebp)
		je     jump_e6257
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x6(%ebp)
		jmp    jump_e625f
	jump_e6257:
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		add    %eax,0x6(%ebp)
	jump_e625f:
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e5de5
	jump_e626c:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x72(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    0x72(%ebp),%ch
		test   %ch,%ch
		jne    jump_e6030
		jmp    jump_e5de5
	jump_e628a:
		mov    0x5a(%ebp),%al
		test   %al,%al
		jne    jump_e6390
	jump_e6295:
		mov    0x1a(%ebp),%edi
		test   %edi,%edi
		jle    jump_e6345
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6300
		mov    0x1a(%ebp),%edx
		cmp    %edx,%eax
		jl     jump_e62b4
		mov    0x1a(%ebp),%eax
	jump_e62b4:
		mov    %al,0x6e(%ebp)
		mov    0xe(%ebp),%eax
		inc    %eax
		mov    %eax,0x2a(%ebp)
		xor    %ah,%ah
		mov    %ah,0x46(%ebp)
		jmp    jump_e62d7
	jump_e62c5:
		mov    0x46(%ebp),%al
		incb   0x46(%ebp)
		mov    0x2a(%ebp),%eax
		incl   0x2a(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e62d7:
		mov    0x46(%ebp),%al
		cmp    0x6e(%ebp),%al
		jae    jump_e62eb
		mov    0x2a(%ebp),%eax
		mov    (%eax),%al
		mov    0x6(%ebp),%ecx
		mov    %al,(%ecx)
		jmp    jump_e62c5
	jump_e62eb:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e6295
	jump_e6300:
		test   %eax,%eax
		jge    jump_e631b
		add    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,0x6(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e6295
	jump_e631b:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%edi
		test   %edi,%edi
		je     jump_e5fdf
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e6390
	jump_e6345:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%esi
		test   %esi,%esi
		jne    jump_e635f
	jump_e634f:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e635f:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e636f
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e635f
	jump_e636f:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e635f
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
	jump_e6390:
		cmpb   $0x0,0x5a(%ebp)
		je     jump_e6295
		mov    0x5a(%ebp),%al
		mov    %al,0x3e(%ebp)
	jump_e63a0:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6448
		xor    %ecx,%ecx
		mov    0x3e(%ebp),%cl
		cmp    %ecx,%eax
		jle    jump_e6426
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		mov    0x3e(%ebp),%bh
		sub    %bh,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%cl
		cmp    %cl,%al
		jbe    jump_e63d4
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e63d4:
		xor    %ecx,%ecx
		mov    0x3e(%ebp),%cl
		mov    0xe(%ebp),%eax
		inc    %eax
		add    %ecx,%eax
		mov    %eax,0x2e(%ebp)
		xor    %ch,%ch
		mov    %ch,0x5e(%ebp)
		jmp    jump_e63fb
	jump_e63e9:
		mov    0x5e(%ebp),%al
		incb   0x5e(%ebp)
		mov    0x2e(%ebp),%eax
		incl   0x2e(%ebp)
		mov    0x6(%ebp),%eax
		decl   0x6(%ebp)
	jump_e63fb:
		mov    0x5e(%ebp),%al
		cmp    0x42(%ebp),%al
		jae    jump_e640f
		mov    0x2e(%ebp),%eax
		mov    (%eax),%cl
		mov    0x6(%ebp),%eax
		mov    %cl,(%eax)
		jmp    jump_e63e9
	jump_e640f:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e6295
	jump_e6426:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x3e(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		mov    0x3e(%ebp),%cl
		test   %cl,%cl
		jne    jump_e63a0
		jmp    jump_e6295
	jump_e6448:
		test   %eax,%eax
		jne    jump_e6454
		mov    %eax,0x1a(%ebp)
		jmp    jump_e6295
	jump_e6454:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%ecx
		xor    %eax,%eax
		mov    0x3e(%ebp),%al
		neg    %eax
		cmp    %eax,%ecx
		jge    jump_e649f
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x3e(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e6484
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e6484:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x6(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e6295
	jump_e649f:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x3e(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    0x3e(%ebp),%cl
		test   %cl,%cl
		jne    jump_e63a0
		jmp    jump_e6295
	jump_e64bd:
		mov    0x6(%ebp),%eax
		mov    0x1e(%ebp),%ecx
		add    %ecx,%eax
		mov    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
		xor    %eax,%eax
		mov    0x32(%ebp),%al
		mov    %eax,0x12(%ebp)
		mov    0x5a(%ebp),%ch
		test   %ch,%ch
		jne    jump_e65c2
	jump_e64e3:
		mov    0x1a(%ebp),%esi
		test   %esi,%esi
		jle    jump_e6577
		mov    0xe(%ebp),%ecx
		movsbl (%ecx),%ecx
		test   %ecx,%ecx
		jle    jump_e6535
		cmp    0x1a(%ebp),%ecx
		jl     jump_e6500
		mov    0x1a(%ebp),%ecx
	jump_e6500:
		mov    0xe(%ebp),%esi
		inc    %esi
		mov    0x6(%ebp),%edi
		push   %edi
		mov    %ecx,%eax
		shr    $0x2,%ecx
		repnz movsl %ds:(%esi),%es:(%edi)
		mov    %al,%cl
		and    $0x3,%cl
		repnz movsb %ds:(%esi),%es:(%edi)
		pop    %edi
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,0x6(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e64e3
	jump_e6535:
		test   %ecx,%ecx
		jge    jump_e654d
		add    %ecx,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x6(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e64e3
	jump_e654d:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%ebx
		test   %ebx,%ebx
		je     jump_e634f
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		jmp    jump_e65c2
	jump_e6577:
		decl   0x12(%ebp)
		mov    0x12(%ebp),%eax
		test   %eax,%eax
		jne    jump_e6591
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_e6591:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e65a1
		inc    %eax
		add    %eax,0xe(%ebp)
		jmp    jump_e6591
	jump_e65a1:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e6591
		mov    0xa(%ebp),%eax
		mov    %eax,0x6(%ebp)
		mov    0x1e(%ebp),%eax
		add    %eax,0xa(%ebp)
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,0x1a(%ebp)
	jump_e65c2:
		cmpb   $0x0,0x5a(%ebp)
		je     jump_e64e3
		mov    0x5a(%ebp),%al
		mov    %al,0x6a(%ebp)
	jump_e65d2:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6669
		xor    %ecx,%ecx
		mov    0x6a(%ebp),%cl
		cmp    %ecx,%eax
		jle    jump_e6647
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		mov    0x6a(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%dl
		cmp    %dl,%al
		jbe    jump_e6606
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e6606:
		xor    %ecx,%ecx
		mov    0x42(%ebp),%cl
		movzbl 0x6a(%ebp),%esi
		mov    0xe(%ebp),%eax
		inc    %eax
		add    %eax,%esi
		mov    0x6(%ebp),%edi
		push   %edi
		mov    %ecx,%eax
		shr    $0x2,%ecx
		repnz movsl %ds:(%esi),%es:(%edi)
		mov    %al,%cl
		and    $0x3,%cl
		repnz movsb %ds:(%esi),%es:(%edi)
		pop    %edi
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		add    %eax,0x6(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e64e3
	jump_e6647:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x6a(%ebp)
		mov    0xe(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,0xe(%ebp)
		mov    0x6a(%ebp),%ch
		test   %ch,%ch
		jne    jump_e65d2
		jmp    jump_e64e3
	jump_e6669:
		test   %eax,%eax
		jne    jump_e6675
		mov    %eax,0x1a(%ebp)
		jmp    jump_e64e3
	jump_e6675:
		mov    0xe(%ebp),%eax
		movsbl (%eax),%ecx
		xor    %eax,%eax
		mov    0x6a(%ebp),%al
		neg    %eax
		cmp    %eax,%ecx
		jge    jump_e66c0
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x6a(%ebp),%bl
		sub    %bl,%al
		mov    %al,0x42(%ebp)
		mov    0x42(%ebp),%al
		mov    0x36(%ebp),%bh
		cmp    %bh,%al
		jbe    jump_e66a5
		mov    0x36(%ebp),%al
		mov    %al,0x42(%ebp)
	jump_e66a5:
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		add    %eax,0x6(%ebp)
		xor    %eax,%eax
		mov    0x42(%ebp),%al
		sub    %eax,0x1a(%ebp)
		jmp    jump_e64e3
	jump_e66c0:
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x6a(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    0x6a(%ebp),%ch
		test   %ch,%ch
		jne    jump_e65d2
		jmp    jump_e64e3
	jump_e66de:
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (func_e66f0)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0xa4,%esp
		sub    $0x82,%ebp
		mov    %eax,-0x22(%ebp)
		mov    %edx,-0x1e(%ebp)
		mov    %ebx,-0x1a(%ebp)
		mov    %ecx,-0x16(%ebp)
		mov    -0x1a(%ebp),%eax
		cmpb   $0x0,0x4(%eax)
		je     jump_e671f
		mov    -0x1a(%ebp),%eax
		cmpb   $0x0,0x5(%eax)
		jne    jump_e6729
	jump_e671f:
		mov    $0x1,%eax
		jmp    jump_e7388
	jump_e6729:
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth)
		je     jump_e673b
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight)
		jne    jump_e674a
	jump_e673b:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e674a:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		add    %eax,-0x22(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		add    %eax,-0x1e(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,0x66(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		mov    -0x22(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,-0x2(%ebp)
		cmpl   $0x0,-0x2(%ebp)
		jle    jump_e6799
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    -0x2(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e6791
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e6791:
		mov    -0x2(%ebp),%al
		mov    %al,0x3e(%ebp)
		jmp    jump_e679e
	jump_e6799:
		xor    %bl,%bl
		mov    %bl,0x3e(%ebp)
	jump_e679e:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%ebx
		xor    %edx,%edx
		add    %ebx,%eax
		mov    0x66(%ebp),%dl
		add    -0x22(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2(%ebp)
		cmpl   $0x0,-0x2(%ebp)
		jle    jump_e67e3
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    -0x2(%ebp),%edi
		cmp    %edi,%eax
		jg     jump_e67d9
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e67d9:
		mov    0x66(%ebp),%al
		mov    -0x2(%ebp),%bh
		sub    %bh,%al
		jmp    jump_e67e6
	jump_e67e3:
		mov    0x66(%ebp),%al
	jump_e67e6:
		mov    %al,0x42(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    0x5(%eax),%al
		mov    %al,0x6a(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		mov    -0x1e(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,-0x2(%ebp)
		cmpl   $0x0,-0x2(%ebp)
		jle    jump_e6828
		xor    %eax,%eax
		mov    0x6a(%ebp),%al
		mov    -0x2(%ebp),%ecx
		cmp    %ecx,%eax
		jg     jump_e6820
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e6820:
		mov    -0x2(%ebp),%al
		mov    %al,0x36(%ebp)
		jmp    jump_e682d
	jump_e6828:
		xor    %cl,%cl
		mov    %cl,0x36(%ebp)
	jump_e682d:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%esi
		xor    %edx,%edx
		add    %esi,%eax
		mov    0x6a(%ebp),%dl
		add    -0x1e(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2(%ebp)
		cmpl   $0x0,-0x2(%ebp)
		jle    jump_e6872
		xor    %eax,%eax
		mov    0x6a(%ebp),%al
		mov    -0x2(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e6868
	jump_e6859:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e6868:
		mov    0x6a(%ebp),%al
		mov    -0x2(%ebp),%ch
		sub    %ch,%al
		jmp    jump_e6875
	jump_e6872:
		mov    0x6a(%ebp),%al
	jump_e6875:
		mov    %al,0x5e(%ebp)
		testb  $0x2,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e68d2
		xor    %eax,%eax
		mov    0x5e(%ebp),%al
		add    -0x1e(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edx
		dec    %eax
		imul   %eax,%edx
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%eax
		add    %edx,%eax
		mov    -0x22(%ebp),%edx
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x3e(%ebp),%al
		add    %eax,%edx
		mov    %edx,-0x12(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		mov    %eax,0x6(%ebp)
		negl   0x6(%ebp)
		mov    0x5e(%ebp),%al
		mov    %al,0x6e(%ebp)
		mov    0x6a(%ebp),%al
		mov    0x36(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x5e(%ebp)
		mov    0x6a(%ebp),%al
		mov    0x6e(%ebp),%dl
		sub    %dl,%al
		mov    %al,0x36(%ebp)
		jmp    jump_e68fd
	jump_e68d2:
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    -0x1e(%ebp),%esi
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		add    %esi,%eax
		imul   %edi,%eax
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%edx
		add    %eax,%edx
		add    -0x22(%ebp),%edx
		xor    %eax,%eax
		mov    0x3e(%ebp),%al
		add    %eax,%edx
		mov    %edx,-0x12(%ebp)
		mov    %edi,0x6(%ebp)
	jump_e68fd:
		mov    0x5e(%ebp),%al
		mov    0x36(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x6a(%ebp)
		mov    0x42(%ebp),%al
		mov    0x3e(%ebp),%bl
		sub    %bl,%al
		mov    %al,0x66(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    (%eax),%eax
		mov    %eax,-0xa(%ebp)
		cmpb   $0x0,0x36(%ebp)
		je     jump_e6950
		xor    %eax,%eax
		mov    0x36(%ebp),%al
		mov    %eax,-0x6(%ebp)
	jump_e6929:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6939
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e6929
	jump_e6939:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e6929
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%esi
		test   %esi,%esi
		jne    jump_e6929
	jump_e6950:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0xd,%cl
		je     jump_e710a
		test   $0x1,%cl
		je     jump_e6998
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		dec    %eax
		add    %eax,-0x12(%ebp)
		movb   $0x1,0x32(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,0x56(%ebp)
		mov    0x42(%ebp),%al
		mov    %al,0x52(%ebp)
		mov    0x56(%ebp),%al
		mov    0x3e(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x42(%ebp)
		mov    0x56(%ebp),%al
		mov    0x52(%ebp),%dl
		sub    %dl,%al
		mov    %al,0x3e(%ebp)
		jmp    jump_e699d
	jump_e6998:
		xor    %dh,%dh
		mov    %dh,0x32(%ebp)
	jump_e699d:
		mov    -0x12(%ebp),%eax
		mov    0x6(%ebp),%edx
		add    %edx,%eax
		mov    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
		xor    %eax,%eax
		mov    0x6a(%ebp),%al
		mov    %eax,-0x6(%ebp)
		testb  $0xc,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e6ec0
		mov    0x3e(%ebp),%bh
		test   %bh,%bh
		jne    jump_e6c2e
	jump_e69d0:
		mov    0x2(%ebp),%ebx
		test   %ebx,%ebx
		jle    jump_e6be4
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6b8d
		mov    0x2(%ebp),%ecx
		cmp    %ecx,%eax
		jl     jump_e69f3
		mov    0x2(%ebp),%eax
	jump_e69f3:
		mov    %al,0x7a(%ebp)
		mov    -0xa(%ebp),%eax
		inc    %eax
		mov    %eax,0xa(%ebp)
		cmpb   $0x0,0x32(%ebp)
		je     jump_e6ac2
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ch
		test   $0x4,%ch
		je     jump_e6a6a
		xor    %al,%al
		mov    %al,0x5a(%ebp)
		jmp    jump_e6a2b
	jump_e6a19:
		mov    0x5a(%ebp),%al
		incb   0x5a(%ebp)
		mov    0xa(%ebp),%eax
		incl   0xa(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e6a2b:
		mov    0x5a(%ebp),%al
		cmp    0x7a(%ebp),%al
		jae    jump_e6b75
		mov    0xa(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%al
		mov    -0x12(%ebp),%edx
		mov    %al,(%edx)
		jmp    jump_e6a19
	jump_e6a6a:
		xor    %dh,%dh
		mov    %dh,0x5a(%ebp)
		jmp    jump_e6a83
	jump_e6a71:
		mov    0x5a(%ebp),%al
		incb   0x5a(%ebp)
		mov    0xa(%ebp),%eax
		incl   0xa(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e6a83:
		mov    0x5a(%ebp),%al
		cmp    0x7a(%ebp),%al
		jae    jump_e6b75
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    0xa(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%al
		mov    -0x12(%ebp),%edx
		mov    %al,(%edx)
		jmp    jump_e6a71
	jump_e6ac2:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0x4,%cl
		je     jump_e6b21
		xor    %ch,%ch
		mov    %ch,0x5a(%ebp)
		jmp    jump_e6ae6
	jump_e6ad4:
		mov    0x5a(%ebp),%al
		incb   0x5a(%ebp)
		mov    0xa(%ebp),%eax
		incl   0xa(%ebp)
		mov    -0x12(%ebp),%eax
		incl   -0x12(%ebp)
	jump_e6ae6:
		mov    0x5a(%ebp),%al
		cmp    0x7a(%ebp),%al
		jae    jump_e6b75
		mov    0xa(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		xor    %edx,%edx
		mov    (%eax),%dl
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%eax
		shl    $0x8,%edx
		add    %eax,%edx
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6ad4
	jump_e6b21:
		xor    %dl,%dl
		mov    %dl,0x5a(%ebp)
		jmp    jump_e6b3a
	jump_e6b28:
		mov    0x5a(%ebp),%al
		incb   0x5a(%ebp)
		mov    0xa(%ebp),%eax
		incl   0xa(%ebp)
		mov    -0x12(%ebp),%eax
		incl   -0x12(%ebp)
	jump_e6b3a:
		mov    0x5a(%ebp),%al
		cmp    0x7a(%ebp),%al
		jae    jump_e6b75
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    0xa(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6b28
	jump_e6b75:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e69d0
	jump_e6b8d:
		test   %eax,%eax
		jge    jump_e6bba
		add    %eax,0x2(%ebp)
		mov    0x32(%ebp),%bh
		test   %bh,%bh
		je     jump_e6ba6
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x12(%ebp)
		jmp    jump_e6baf
	jump_e6ba6:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x12(%ebp)
	jump_e6baf:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e69d0
	jump_e6bba:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%edx
		test   %edx,%edx
		je     jump_e6859
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e6c2e
	jump_e6be4:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%edi
		test   %edi,%edi
		jne    jump_e6bfd
	jump_e6bee:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e6bfd:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6c0d
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e6bfd
	jump_e6c0d:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e6bfd
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
	jump_e6c2e:
		cmpb   $0x0,0x3e(%ebp)
		je     jump_e69d0
		mov    0x3e(%ebp),%al
		mov    %al,0x76(%ebp)
	jump_e6c3e:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6e3b
		xor    %edx,%edx
		mov    0x76(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e6e19
		xor    %eax,%eax
		mov    0x76(%ebp),%al
		mov    -0xa(%ebp),%edx
		inc    %edx
		add    %eax,%edx
		mov    %edx,0xe(%ebp)
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		mov    0x76(%ebp),%ch
		sub    %ch,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%ah
		cmp    %ah,%al
		jbe    jump_e6c84
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e6c84:
		cmpb   $0x0,0x32(%ebp)
		je     jump_e6d4b
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dh
		test   $0x4,%dh
		je     jump_e6cf1
		xor    %bl,%bl
		mov    %bl,0x4e(%ebp)
		jmp    jump_e6cb2
	jump_e6ca0:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e6cb2:
		mov    0x4e(%ebp),%al
		cmp    0x46(%ebp),%al
		jae    jump_e6e02
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6ca0
	jump_e6cf1:
		xor    %ch,%ch
		mov    %ch,0x4e(%ebp)
		jmp    jump_e6d0a
	jump_e6cf8:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e6d0a:
		mov    0x4e(%ebp),%al
		cmp    0x46(%ebp),%al
		jae    jump_e6e02
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		mov    %eax,%edx
		and    $0xff,%edx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%eax
		shl    $0x8,%edx
		add    %eax,%edx
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6cf8
	jump_e6d4b:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dl
		test   $0x4,%dl
		je     jump_e6dae
		xor    %dh,%dh
		mov    %dh,0x4e(%ebp)
		jmp    jump_e6d6f
	jump_e6d5d:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    -0x12(%ebp),%eax
		incl   -0x12(%ebp)
	jump_e6d6f:
		mov    0x4e(%ebp),%al
		cmp    0x46(%ebp),%al
		jae    jump_e6e02
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6d5d
	jump_e6dae:
		xor    %cl,%cl
		mov    %cl,0x4e(%ebp)
		jmp    jump_e6dc7
	jump_e6db5:
		mov    0x4e(%ebp),%al
		incb   0x4e(%ebp)
		mov    0xe(%ebp),%eax
		incl   0xe(%ebp)
		mov    -0x12(%ebp),%eax
		incl   -0x12(%ebp)
	jump_e6dc7:
		mov    0x4e(%ebp),%al
		cmp    0x46(%ebp),%al
		jae    jump_e6e02
		mov    -0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    0xe(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6db5
	jump_e6e02:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e69d0
	jump_e6e19:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x76(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		mov    0x76(%ebp),%ch
		test   %ch,%ch
		jne    jump_e6c3e
		jmp    jump_e69d0
	jump_e6e3b:
		test   %eax,%eax
		jne    jump_e6e47
		mov    %eax,0x2(%ebp)
		jmp    jump_e69d0
	jump_e6e47:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%edx
		xor    %eax,%eax
		mov    0x76(%ebp),%al
		neg    %eax
		cmp    %eax,%edx
		jge    jump_e6ea2
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x76(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e6e77
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e6e77:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		cmpb   $0x0,0x32(%ebp)
		je     jump_e6e8d
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,-0x12(%ebp)
		jmp    jump_e6e95
	jump_e6e8d:
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		add    %eax,-0x12(%ebp)
	jump_e6e95:
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e69d0
	jump_e6ea2:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x76(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		mov    0x76(%ebp),%ch
		test   %ch,%ch
		jne    jump_e6c3e
		jmp    jump_e69d0
	jump_e6ec0:
		mov    0x3e(%ebp),%al
		test   %al,%al
		jne    jump_e6fcf
	jump_e6ecb:
		mov    0x2(%ebp),%esi
		test   %esi,%esi
		jle    jump_e6f85
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6f40
		mov    0x2(%ebp),%edi
		cmp    %edi,%eax
		jl     jump_e6eea
		mov    0x2(%ebp),%eax
	jump_e6eea:
		mov    %al,0x4a(%ebp)
		mov    -0xa(%ebp),%eax
		inc    %eax
		mov    %eax,0x12(%ebp)
		xor    %ah,%ah
		mov    %ah,0x62(%ebp)
		jmp    jump_e6f0d
	jump_e6efb:
		mov    0x62(%ebp),%al
		incb   0x62(%ebp)
		mov    0x12(%ebp),%eax
		incl   0x12(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e6f0d:
		mov    0x62(%ebp),%al
		cmp    0x4a(%ebp),%al
		jae    jump_e6f2b
		mov    0x12(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e6efb
	jump_e6f2b:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e6ecb
	jump_e6f40:
		test   %eax,%eax
		jge    jump_e6f5b
		add    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x12(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e6ecb
	jump_e6f5b:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%esi
		test   %esi,%esi
		je     jump_e6bee
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e6fcf
	jump_e6f85:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%ebx
		test   %ebx,%ebx
		jne    jump_e6f9e
	jump_e6f8f:
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e6f9e:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e6fae
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e6f9e
	jump_e6fae:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e6f9e
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
	jump_e6fcf:
		cmpb   $0x0,0x3e(%ebp)
		je     jump_e6ecb
		mov    0x3e(%ebp),%al
		mov    %al,0x7e(%ebp)
	jump_e6fdf:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e7095
		xor    %edx,%edx
		mov    0x7e(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e7073
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		mov    0x7e(%ebp),%bh
		sub    %bh,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%cl
		cmp    %cl,%al
		jbe    jump_e7017
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e7017:
		xor    %edx,%edx
		mov    0x7e(%ebp),%dl
		mov    -0xa(%ebp),%eax
		inc    %eax
		add    %edx,%eax
		mov    %eax,0x16(%ebp)
		xor    %ch,%ch
		mov    %ch,0x3a(%ebp)
		jmp    jump_e703e
	jump_e702c:
		mov    0x3a(%ebp),%al
		incb   0x3a(%ebp)
		mov    0x16(%ebp),%eax
		incl   0x16(%ebp)
		mov    -0x12(%ebp),%eax
		decl   -0x12(%ebp)
	jump_e703e:
		mov    0x3a(%ebp),%al
		cmp    0x46(%ebp),%al
		jae    jump_e705c
		mov    0x16(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%dl
		mov    -0x12(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e702c
	jump_e705c:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e6ecb
	jump_e7073:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x7e(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		mov    0x7e(%ebp),%cl
		test   %cl,%cl
		jne    jump_e6fdf
		jmp    jump_e6ecb
	jump_e7095:
		test   %eax,%eax
		jne    jump_e70a1
		mov    %eax,0x2(%ebp)
		jmp    jump_e6ecb
	jump_e70a1:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%edx
		xor    %eax,%eax
		mov    0x7e(%ebp),%al
		neg    %eax
		cmp    %eax,%edx
		jge    jump_e70ec
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x7e(%ebp),%dh
		sub    %dh,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e70d1
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e70d1:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,-0x12(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e6ecb
	jump_e70ec:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x7e(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		mov    0x7e(%ebp),%cl
		test   %cl,%cl
		jne    jump_e6fdf
		jmp    jump_e6ecb
	jump_e710a:
		mov    -0x12(%ebp),%eax
		mov    0x6(%ebp),%edi
		add    %edi,%eax
		mov    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
		xor    %eax,%eax
		mov    0x6a(%ebp),%al
		mov    %eax,-0x6(%ebp)
		mov    0x3e(%ebp),%ch
		test   %ch,%ch
		jne    jump_e723d
	jump_e7130:
		mov    0x2(%ebp),%eax
		test   %eax,%eax
		jle    jump_e71f3
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e71ae
		mov    0x2(%ebp),%edx
		cmp    %edx,%eax
		jl     jump_e714f
		mov    0x2(%ebp),%eax
	jump_e714f:
		mov    -0x12(%ebp),%edx
		add    %eax,%edx
		mov    %edx,0x1a(%ebp)
		mov    -0x12(%ebp),%eax
		mov    %eax,0x1e(%ebp)
		mov    -0xa(%ebp),%eax
		inc    %eax
		mov    %eax,0x22(%ebp)
		jmp    jump_e7172
	jump_e7166:
		mov    0x1e(%ebp),%eax
		incl   0x1e(%ebp)
		mov    0x22(%ebp),%eax
		incl   0x22(%ebp)
	jump_e7172:
		mov    0x1e(%ebp),%eax
		cmp    0x1a(%ebp),%eax
		jae    jump_e7190
		mov    0x22(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%dl
		mov    0x1e(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e7166
	jump_e7190:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x12(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e7130
	jump_e71ae:
		test   %eax,%eax
		jge    jump_e71c9
		add    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x12(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e7130
	jump_e71c9:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%ebx
		test   %ebx,%ebx
		je     jump_e6f8f
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		jmp    jump_e723d
	jump_e71f3:
		decl   -0x6(%ebp)
		mov    -0x6(%ebp),%eax
		test   %eax,%eax
		jne    jump_e720c
		mov    $0x1,%eax
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e720c:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e721c
		inc    %eax
		add    %eax,-0xa(%ebp)
		jmp    jump_e720c
	jump_e721c:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e720c
		mov    -0xe(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    0x6(%ebp),%eax
		add    %eax,-0xe(%ebp)
		xor    %eax,%eax
		mov    0x66(%ebp),%al
		mov    %eax,0x2(%ebp)
	jump_e723d:
		cmpb   $0x0,0x3e(%ebp)
		je     jump_e7130
		mov    0x3e(%ebp),%al
		mov    %al,0x72(%ebp)
	jump_e724d:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e7313
		xor    %edx,%edx
		mov    0x72(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e72f1
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		mov    0x72(%ebp),%ah
		sub    %ah,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%dl
		cmp    %dl,%al
		jbe    jump_e7285
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e7285:
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		mov    -0x12(%ebp),%edx
		add    %eax,%edx
		mov    %edx,0x26(%ebp)
		mov    -0x12(%ebp),%eax
		mov    %eax,0x2a(%ebp)
		xor    %eax,%eax
		mov    0x72(%ebp),%al
		mov    -0xa(%ebp),%edx
		inc    %edx
		add    %eax,%edx
		mov    %edx,0x2e(%ebp)
		jmp    jump_e72b4
	jump_e72a8:
		mov    0x2a(%ebp),%eax
		incl   0x2a(%ebp)
		mov    0x2e(%ebp),%eax
		incl   0x2e(%ebp)
	jump_e72b4:
		mov    0x2a(%ebp),%eax
		cmp    0x26(%ebp),%eax
		jae    jump_e72d2
		mov    0x2e(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		add    -0x16(%ebp),%eax
		mov    (%eax),%dl
		mov    0x2a(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e72a8
	jump_e72d2:
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		add    %eax,-0x12(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e7130
	jump_e72f1:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		sub    %al,0x72(%ebp)
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0xa(%ebp)
		mov    0x72(%ebp),%ch
		test   %ch,%ch
		jne    jump_e724d
		jmp    jump_e7130
	jump_e7313:
		test   %eax,%eax
		jne    jump_e731f
		mov    %eax,0x2(%ebp)
		jmp    jump_e7130
	jump_e731f:
		mov    -0xa(%ebp),%eax
		movsbl (%eax),%edx
		xor    %eax,%eax
		mov    0x72(%ebp),%al
		neg    %eax
		cmp    %eax,%edx
		jge    jump_e736a
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    0x72(%ebp),%bl
		sub    %bl,%al
		mov    %al,0x46(%ebp)
		mov    0x46(%ebp),%al
		mov    0x66(%ebp),%bh
		cmp    %bh,%al
		jbe    jump_e734f
		mov    0x66(%ebp),%al
		mov    %al,0x46(%ebp)
	jump_e734f:
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		add    %eax,-0x12(%ebp)
		xor    %eax,%eax
		mov    0x46(%ebp),%al
		sub    %eax,0x2(%ebp)
		jmp    jump_e7130
	jump_e736a:
		mov    -0xa(%ebp),%eax
		mov    (%eax),%al
		add    %al,0x72(%ebp)
		mov    -0xa(%ebp),%eax
		incl   -0xa(%ebp)
		mov    0x72(%ebp),%ch
		test   %ch,%ch
		jne    jump_e724d
		jmp    jump_e7130
	jump_e7388:
		lea    0x82(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbDrawBox_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		sub    $0x10,%esp
		mov    %eax,-0x10(%ebp)
		mov    %edx,-0xc(%ebp)
		mov    %ebx,-0x8(%ebp)
		mov    %ecx,-0x4(%ebp)
		testb  $0x10,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e74f1
		mov    -0x8(%ebp),%edx
		cmp    $0x1,%edx
		jl     jump_e745d
		cmpl   $0x1,-0x4(%ebp)
		jge    jump_e7467
	jump_e745d:
		mov    $0xffffffff,%eax
		jmp    jump_e750d
	jump_e7467:
		xor    %eax,%eax
		mov    0x8(%ebp),%al
		mov    -0xc(%ebp),%ecx
		mov    -0x10(%ebp),%ebx
		add    -0x8(%ebp),%ebx
		dec    %ebx
		mov    -0xc(%ebp),%edx
		push   %eax
		mov    -0x10(%ebp),%eax
		call   LbDrawHVLine_
		xor    %eax,%eax
		mov    0x8(%ebp),%al
		mov    -0xc(%ebp),%ecx
		add    -0x4(%ebp),%ecx
		dec    %ecx
		mov    -0x10(%ebp),%ebx
		push   %eax
		add    -0x8(%ebp),%ebx
		dec    %ebx
		mov    -0xc(%ebp),%edx
		add    -0x4(%ebp),%edx
		dec    %edx
		mov    -0x10(%ebp),%eax
		call   LbDrawHVLine_
		cmpl   $0x2,-0x4(%ebp)
		jbe    jump_e7508
		xor    %eax,%eax
		mov    0x8(%ebp),%al
		mov    -0xc(%ebp),%ecx
		push   %eax
		add    -0x4(%ebp),%ecx
		sub    $0x2,%ecx
		mov    -0x10(%ebp),%ebx
		mov    -0xc(%ebp),%edx
		inc    %edx
		mov    -0x10(%ebp),%eax
		call   LbDrawHVLine_
		xor    %eax,%eax
		mov    0x8(%ebp),%al
		mov    -0xc(%ebp),%ecx
		add    -0x4(%ebp),%ecx
		sub    $0x2,%ecx
		mov    -0x10(%ebp),%ebx
		add    -0x8(%ebp),%ebx
		dec    %ebx
		mov    -0xc(%ebp),%edx
		inc    %edx
		push   %eax
		mov    -0x10(%ebp),%eax
		add    -0x8(%ebp),%eax
		dec    %eax
		call   LbDrawHVLine_
		jmp    jump_e7508
	jump_e74f1:
		xor    %eax,%eax
		mov    0x8(%ebp),%al
		mov    -0x4(%ebp),%ecx
		mov    -0x8(%ebp),%ebx
		mov    -0xc(%ebp),%edx
		push   %eax
		mov    -0x10(%ebp),%eax
		call   func_fadd0
	jump_e7508:
		mov    $0x1,%eax
	jump_e750d:
		mov    %ebp,%esp
		pop    %ebp
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbDrawLine_)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0xa4,%esp
		sub    $0x7e,%ebp
		mov    %eax,-0x26(%ebp)
		mov    %edx,-0x22(%ebp)
		mov    %ebx,-0x1e(%ebp)
		mov    %ecx,-0x1a(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%ebx
		dec    %ebx
		cmpl   $0x0,-0x26(%ebp)
		jge    jump_e7663
		mov    -0x1e(%ebp),%ecx
		test   %ecx,%ecx
		jge    jump_e75c9
		mov    $0x1,%al
		jmp    jump_e79cd
	jump_e75c9:
		mov    -0x26(%ebp),%eax
		mov    -0x1e(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,-0x16(%ebp)
		mov    -0x26(%ebp),%eax
		mov    %eax,-0x12(%ebp)
		mov    -0x22(%ebp),%eax
		mov    -0x1a(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,-0xe(%ebp)
		mov    -0xe(%ebp),%edx
		mov    -0x12(%ebp),%eax
		imul   %eax,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    -0x16(%ebp),%ecx
		idiv   %ecx
		mov    %eax,-0xe(%ebp)
		xor    %esi,%esi
		mov    %esi,-0x26(%ebp)
		mov    -0xe(%ebp),%eax
		sub    %eax,-0x22(%ebp)
		movb   $0x1,0x7a(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%edx
		cmp    %edx,%eax
		jl     jump_e77ab
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%ecx
		sub    %ecx,%eax
		mov    %eax,-0xa(%ebp)
		mov    -0x1e(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,-0x6(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,-0x2(%ebp)
		mov    -0x2(%ebp),%edx
		mov    -0x6(%ebp),%edi
		imul   %edi,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    -0xa(%ebp),%ebx
		idiv   %ebx
		mov    %eax,-0x2(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%eax
		dec    %eax
		mov    %eax,-0x1e(%ebp)
		mov    -0x2(%ebp),%eax
		sub    %eax,-0x1a(%ebp)
		jmp    jump_e77ab
	jump_e7663:
		mov    -0x26(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%esi
		cmp    %esi,%eax
		jl     jump_e771a
		mov    -0x1e(%ebp),%eax
		cmp    %esi,%eax
		jl     jump_e7686
		mov    $0x1,%al
		lea    0x7e(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x4
	jump_e7686:
		mov    -0x26(%ebp),%eax
		mov    -0x1e(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0x2(%ebp)
		mov    -0x26(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,0x6(%ebp)
		mov    -0x22(%ebp),%eax
		mov    -0x1a(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0xa(%ebp)
		mov    0xa(%ebp),%edx
		mov    0x6(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x2(%ebp),%esi
		idiv   %esi
		mov    %eax,0xa(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%eax
		dec    %eax
		mov    %eax,-0x26(%ebp)
		mov    0xa(%ebp),%eax
		sub    %eax,-0x22(%ebp)
		movb   $0x1,0x7a(%ebp)
		mov    -0x1e(%ebp),%eax
		test   %eax,%eax
		jge    jump_e77ab
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0xe(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    %eax,0x12(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x16(%ebp)
		mov    0x16(%ebp),%edx
		mov    0x12(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0xe(%ebp),%esi
		idiv   %esi
		mov    %eax,0x16(%ebp)
		xor    %edi,%edi
		mov    %edi,-0x1e(%ebp)
		mov    0x16(%ebp),%eax
		sub    %eax,-0x1a(%ebp)
		jmp    jump_e77ab
	jump_e771a:
		mov    -0x1e(%ebp),%ecx
		test   %ecx,%ecx
		jge    jump_e775d
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,0x1a(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    %eax,0x1e(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,0x22(%ebp)
		mov    0x22(%ebp),%edx
		mov    0x1e(%ebp),%eax
		imul   %eax,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x1a(%ebp),%ebx
		idiv   %ebx
		mov    %eax,0x22(%ebp)
		xor    %ecx,%ecx
		mov    %ecx,-0x1e(%ebp)
		mov    0x22(%ebp),%eax
		jmp    jump_e77a4
	jump_e775d:
		mov    -0x1e(%ebp),%eax
		cmp    %esi,%eax
		jl     jump_e77ab
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0x26(%ebp)
		mov    -0x1e(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,0x2a(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x2e(%ebp)
		mov    0x2e(%ebp),%edx
		mov    0x2a(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x26(%ebp),%esi
		idiv   %esi
		mov    %eax,0x2e(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%eax
		dec    %eax
		mov    %eax,-0x1e(%ebp)
		mov    0x2e(%ebp),%eax
	jump_e77a4:
		sub    %eax,-0x1a(%ebp)
		movb   $0x1,0x7a(%ebp)
	jump_e77ab:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ebx
		dec    %ebx
		cmpl   $0x0,-0x22(%ebp)
		jge    jump_e7868
		mov    -0x1a(%ebp),%edx
		test   %edx,%edx
		jge    jump_e77ce
		mov    $0x1,%al
		lea    0x7e(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x4
	jump_e77ce:
		mov    -0x22(%ebp),%eax
		mov    -0x1a(%ebp),%ecx
		sub    %ecx,%eax
		mov    %eax,0x32(%ebp)
		mov    -0x22(%ebp),%eax
		mov    %eax,0x36(%ebp)
		mov    -0x26(%ebp),%eax
		mov    -0x1e(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,0x3a(%ebp)
		mov    0x3a(%ebp),%edx
		mov    0x36(%ebp),%edi
		imul   %edi,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x32(%ebp),%ecx
		idiv   %ecx
		mov    %eax,0x3a(%ebp)
		xor    %esi,%esi
		mov    %esi,-0x22(%ebp)
		mov    0x3a(%ebp),%eax
		sub    %eax,-0x26(%ebp)
		movb   $0x1,0x7a(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edx
		cmp    %edx,%eax
		jl     jump_e79b0
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%ecx
		sub    %ecx,%eax
		mov    %eax,0x3e(%ebp)
		mov    -0x1a(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,0x42(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,0x46(%ebp)
		mov    0x46(%ebp),%edx
		mov    0x42(%ebp),%edi
		imul   %edi,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x3e(%ebp),%ebx
		idiv   %ebx
		mov    %eax,0x46(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%eax
		dec    %eax
		mov    %eax,-0x1a(%ebp)
		mov    0x46(%ebp),%eax
		sub    %eax,-0x1e(%ebp)
		jmp    jump_e79b0
	jump_e7868:
		mov    -0x22(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%esi
		cmp    %esi,%eax
		jl     jump_e791f
		mov    -0x1a(%ebp),%eax
		cmp    %esi,%eax
		jl     jump_e788b
		mov    $0x1,%al
		lea    0x7e(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x4
	jump_e788b:
		mov    -0x22(%ebp),%eax
		mov    -0x1a(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0x4a(%ebp)
		mov    -0x22(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,0x4e(%ebp)
		mov    -0x26(%ebp),%eax
		mov    -0x1e(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x52(%ebp)
		mov    0x52(%ebp),%edx
		mov    0x4e(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x4a(%ebp),%esi
		idiv   %esi
		mov    %eax,0x52(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%eax
		dec    %eax
		mov    %eax,-0x22(%ebp)
		mov    0x52(%ebp),%eax
		sub    %eax,-0x26(%ebp)
		movb   $0x1,0x7a(%ebp)
		mov    -0x1a(%ebp),%eax
		test   %eax,%eax
		jge    jump_e79b0
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0x56(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    %eax,0x5a(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x5e(%ebp)
		mov    0x5e(%ebp),%edx
		mov    0x5a(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x56(%ebp),%esi
		idiv   %esi
		mov    %eax,0x5e(%ebp)
		xor    %edi,%edi
		mov    %edi,-0x1a(%ebp)
		mov    0x5e(%ebp),%eax
		sub    %eax,-0x1e(%ebp)
		jmp    jump_e79b0
	jump_e791f:
		mov    -0x1a(%ebp),%ecx
		test   %ecx,%ecx
		jge    jump_e7962
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%esi
		sub    %esi,%eax
		mov    %eax,0x62(%ebp)
		mov    -0x1a(%ebp),%eax
		mov    %eax,0x66(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,0x6a(%ebp)
		mov    0x6a(%ebp),%edx
		mov    0x66(%ebp),%eax
		imul   %eax,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x62(%ebp),%ebx
		idiv   %ebx
		mov    %eax,0x6a(%ebp)
		xor    %ecx,%ecx
		mov    %ecx,-0x1a(%ebp)
		mov    0x6a(%ebp),%eax
		jmp    jump_e79a9
	jump_e7962:
		mov    -0x1a(%ebp),%eax
		cmp    %esi,%eax
		jl     jump_e79b0
		mov    -0x1a(%ebp),%eax
		mov    -0x22(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,0x6e(%ebp)
		mov    -0x1a(%ebp),%eax
		sub    %ebx,%eax
		mov    %eax,0x72(%ebp)
		mov    -0x1e(%ebp),%eax
		mov    -0x26(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,0x76(%ebp)
		mov    0x76(%ebp),%edx
		mov    0x72(%ebp),%ecx
		imul   %ecx,%edx
		mov    %edx,%eax
		sar    $0x1f,%edx
		mov    0x6e(%ebp),%esi
		idiv   %esi
		mov    %eax,0x76(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%eax
		dec    %eax
		mov    %eax,-0x1a(%ebp)
		mov    0x76(%ebp),%eax
	jump_e79a9:
		sub    %eax,-0x1e(%ebp)
		movb   $0x1,0x7a(%ebp)
	jump_e79b0:
		xor    %eax,%eax
		mov    0x8e(%ebp),%al
		mov    -0x1a(%ebp),%ecx
		mov    -0x1e(%ebp),%ebx
		mov    -0x22(%ebp),%edx
		push   %eax
		mov    -0x26(%ebp),%eax
		call   func_e79d8
		mov    0x7a(%ebp),%al
	jump_e79cd:
		lea    0x7e(%ebp),%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x4


/*----------------------------------------------------------------*/
func_e79d8:
/*----------------------------------------------------------------*/
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x3c,%esp
		mov    %eax,-0x3c(%ebp)
		mov    %edx,-0x38(%ebp)
		mov    %ebx,-0x34(%ebp)
		mov    %ecx,-0x30(%ebp)
		mov    -0x34(%ebp),%eax
		mov    -0x3c(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,-0x10(%ebp)
		mov    -0x10(%ebp),%eax
		cltd
		xor    %edx,%eax
		sub    %edx,%eax
		add    %eax,%eax
		mov    %eax,-0x20(%ebp)
		cmpl   $0x0,-0x10(%ebp)
		jle    jump_e7a10
		mov    $0x1,%eax
		jmp    jump_e7a15
	jump_e7a10:
		mov    $0xffffffff,%eax
	jump_e7a15:
		mov    %eax,-0x18(%ebp)
		mov    -0x30(%ebp),%eax
		mov    -0x38(%ebp),%ecx
		sub    %ecx,%eax
		mov    %eax,-0xc(%ebp)
		mov    -0xc(%ebp),%eax
		cltd
		xor    %edx,%eax
		sub    %edx,%eax
		add    %eax,%eax
		mov    %eax,-0x1c(%ebp)
		cmpl   $0x0,-0xc(%ebp)
		jle    jump_e7a3d
		mov    $0x1,%eax
		jmp    jump_e7a42
	jump_e7a3d:
		mov    $0xffffffff,%eax
	jump_e7a42:
		mov    %eax,-0x14(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		mov    -0x14(%ebp),%edi
		imul   %edi,%eax
		mov    %eax,-0x8(%ebp)
		mov    -0x38(%ebp),%eax
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		mov    -0x3c(%ebp),%edx
		add    %eax,%edx
		mov    %edx,-0x4(%ebp)
		mov    -0x3c(%ebp),%eax
		mov    %eax,-0x28(%ebp)
		mov    -0x38(%ebp),%eax
		mov    %eax,-0x24(%ebp)
		testb  $0x4,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e7b5a
		mov    -0x20(%ebp),%eax
		mov    -0x1c(%ebp),%ebx
		cmp    %ebx,%eax
		jle    jump_e7af1
		mov    -0x20(%ebp),%eax
		sar    %eax
		mov    -0x1c(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7a95:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%eax
		mov    -0x4(%ebp),%edx
		xor    %ebx,%ebx
		mov    (%edx,%eax,1),%bl
		xor    %edx,%edx
		mov    0xc(%ebp),%dl
		shl    $0x8,%edx
		add    %ebx,%edx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
		add    %ebx,%edx
		mov    -0x4(%ebp),%ebx
		add    %eax,%ebx
		mov    (%edx),%al
		mov    %al,(%ebx)
		mov    -0x28(%ebp),%eax
		mov    -0x34(%ebp),%ecx
		cmp    %ecx,%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7add
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7add:
		mov    -0x18(%ebp),%eax
		add    %eax,-0x28(%ebp)
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7a95
	jump_e7af1:
		mov    -0x1c(%ebp),%eax
		sar    %eax
		mov    -0x20(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7afe:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%eax
		mov    -0x4(%ebp),%edx
		xor    %ebx,%ebx
		mov    (%edx,%eax,1),%bl
		xor    %edx,%edx
		mov    0xc(%ebp),%dl
		shl    $0x8,%edx
		add    %ebx,%edx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
		add    %ebx,%edx
		mov    -0x4(%ebp),%ebx
		add    %eax,%ebx
		mov    (%edx),%al
		mov    %al,(%ebx)
		mov    -0x24(%ebp),%eax
		mov    -0x30(%ebp),%edi
		cmp    %edi,%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7b46
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7b46:
		mov    -0x14(%ebp),%eax
		add    %eax,-0x24(%ebp)
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7afe
	jump_e7b5a:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dl
		test   $0x8,%dl
		je     jump_e7c4b
		mov    -0x20(%ebp),%eax
		mov    -0x1c(%ebp),%edx
		cmp    %edx,%eax
		jle    jump_e7be0
		mov    -0x20(%ebp),%eax
		sar    %eax
		mov    -0x1c(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7b80:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%eax
		mov    -0x4(%ebp),%edx
		mov    (%edx,%eax,1),%dl
		mov    %edx,%ebx
		and    $0xff,%ebx
		xor    %edx,%edx
		shl    $0x8,%ebx
		mov    0xc(%ebp),%dl
		add    %ebx,%edx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
		add    %ebx,%edx
		mov    -0x4(%ebp),%ebx
		add    %eax,%ebx
		mov    (%edx),%al
		mov    %al,(%ebx)
		mov    -0x28(%ebp),%eax
		cmp    -0x34(%ebp),%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7bcc
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7bcc:
		mov    -0x18(%ebp),%eax
		add    %eax,-0x28(%ebp)
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7b80
	jump_e7be0:
		mov    -0x1c(%ebp),%eax
		sar    %eax
		mov    -0x20(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7bed:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%eax
		mov    -0x4(%ebp),%edx
		mov    (%edx,%eax,1),%dl
		and    $0xff,%edx
		xor    %ebx,%ebx
		shl    $0x8,%edx
		mov    0xc(%ebp),%bl
		add    %ebx,%edx
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
		add    %ebx,%edx
		mov    -0x4(%ebp),%ebx
		add    %eax,%ebx
		mov    (%edx),%al
		mov    %al,(%ebx)
		mov    -0x24(%ebp),%eax
		cmp    -0x30(%ebp),%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7c37
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7c37:
		mov    -0x14(%ebp),%eax
		add    %eax,-0x24(%ebp)
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7bed
	jump_e7c4b:
		mov    -0x20(%ebp),%eax
		mov    -0x1c(%ebp),%edx
		cmp    %edx,%eax
		jle    jump_e7ca5
		mov    -0x20(%ebp),%eax
		sar    %eax
		mov    -0x1c(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7c62:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%eax
		mov    -0x4(%ebp),%edx
		add    %eax,%edx
		mov    0xc(%ebp),%al
		mov    %al,(%edx)
		mov    -0x28(%ebp),%eax
		mov    -0x34(%ebp),%ebx
		cmp    %ebx,%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7c91
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7c91:
		mov    -0x18(%ebp),%eax
		add    %eax,-0x28(%ebp)
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7c62
	jump_e7ca5:
		mov    -0x1c(%ebp),%eax
		sar    %eax
		mov    -0x20(%ebp),%edx
		sub    %eax,%edx
		mov    %edx,-0x2c(%ebp)
	jump_e7cb2:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%edx
		add    -0x4(%ebp),%edx
		mov    0xc(%ebp),%al
		mov    %al,(%edx)
		mov    -0x24(%ebp),%eax
		mov    -0x30(%ebp),%edi
		cmp    %edi,%eax
		je     jump_e7cf0
		cmpl   $0x0,-0x2c(%ebp)
		jl     jump_e7cdc
		mov    -0x18(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x1c(%ebp),%eax
		sub    %eax,-0x2c(%ebp)
	jump_e7cdc:
		mov    -0x14(%ebp),%eax
		add    %eax,-0x24(%ebp)
		mov    -0x8(%ebp),%eax
		add    %eax,-0x4(%ebp)
		mov    -0x20(%ebp),%eax
		add    %eax,-0x2c(%ebp)
		jmp    jump_e7cb2
	jump_e7cf0:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbSpriteDrawOneColour_)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x7c,%esp
		mov    %eax,-0x7c(%ebp)
		mov    %edx,-0x78(%ebp)
		mov    %ebx,-0x74(%ebp)
		mov    %cl,-0x28(%ebp)
		mov    -0x74(%ebp),%eax
		cmpb   $0x0,0x4(%eax)
		je     jump_e7d26
		mov    -0x74(%ebp),%eax
		cmpb   $0x0,0x5(%eax)
		jne    jump_e7d30
	jump_e7d26:
		mov    $0x1,%eax
		jmp    jump_e8825
	jump_e7d30:
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth)
		je     jump_e7d42
		cmpl   $0x0,EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight)
		jne    jump_e7d4d
	jump_e7d42:
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e7d4d:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		add    %eax,-0x7c(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		add    %eax,-0x78(%ebp)
		mov    -0x74(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,-0x38(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		mov    -0x7c(%ebp),%edi
		sub    %edi,%eax
		mov    %eax,-0x60(%ebp)
		cmpl   $0x0,-0x60(%ebp)
		jle    jump_e7d98
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    -0x60(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e7d90
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e7d90:
		mov    -0x60(%ebp),%al
		mov    %al,-0x2c(%ebp)
		jmp    jump_e7d9d
	jump_e7d98:
		xor    %bl,%bl
		mov    %bl,-0x2c(%ebp)
	jump_e7d9d:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%ebx
		xor    %eax,%eax
		add    %ebx,%edx
		mov    -0x38(%ebp),%al
		add    -0x7c(%ebp),%eax
		sub    %edx,%eax
		mov    %eax,-0x60(%ebp)
		cmpl   $0x0,-0x60(%ebp)
		jle    jump_e7ddf
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    -0x60(%ebp),%edi
		cmp    %edi,%eax
		jg     jump_e7dd5
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e7dd5:
		mov    -0x38(%ebp),%al
		mov    -0x60(%ebp),%bh
		sub    %bh,%al
		jmp    jump_e7de2
	jump_e7ddf:
		mov    -0x38(%ebp),%al
	jump_e7de2:
		mov    %al,-0x48(%ebp)
		mov    -0x74(%ebp),%eax
		mov    0x5(%eax),%al
		mov    %al,-0x14(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		mov    -0x78(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,-0x60(%ebp)
		cmpl   $0x0,-0x60(%ebp)
		jle    jump_e7e20
		xor    %eax,%eax
		mov    -0x14(%ebp),%al
		mov    -0x60(%ebp),%ecx
		cmp    %ecx,%eax
		jg     jump_e7e18
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e7e18:
		mov    -0x60(%ebp),%al
		mov    %al,-0x34(%ebp)
		jmp    jump_e7e25
	jump_e7e20:
		xor    %cl,%cl
		mov    %cl,-0x34(%ebp)
	jump_e7e25:
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%esi
		xor    %eax,%eax
		add    %esi,%edx
		mov    -0x14(%ebp),%al
		add    -0x78(%ebp),%eax
		sub    %edx,%eax
		mov    %eax,-0x60(%ebp)
		cmpl   $0x0,-0x60(%ebp)
		jle    jump_e7e67
		xor    %eax,%eax
		mov    -0x14(%ebp),%al
		mov    -0x60(%ebp),%edx
		cmp    %edx,%eax
		jg     jump_e7e5d
	jump_e7e52:
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e7e5d:
		mov    -0x14(%ebp),%al
		mov    -0x60(%ebp),%ch
		sub    %ch,%al
		jmp    jump_e7e6a
	jump_e7e67:
		mov    -0x14(%ebp),%al
	jump_e7e6a:
		mov    %al,-0x4c(%ebp)
		testb  $0x2,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e7ec1
		xor    %eax,%eax
		mov    -0x4c(%ebp),%al
		add    -0x78(%ebp),%eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ecx
		dec    %eax
		imul   %ecx,%eax
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%edx
		add    %edx,%eax
		add    -0x7c(%ebp),%eax
		xor    %edx,%edx
		mov    -0x2c(%ebp),%dl
		add    %edx,%eax
		mov    %eax,-0x70(%ebp)
		mov    %ecx,-0x58(%ebp)
		negl   -0x58(%ebp)
		mov    -0x4c(%ebp),%al
		mov    %al,-0x30(%ebp)
		mov    -0x14(%ebp),%al
		mov    -0x34(%ebp),%ah
		sub    %ah,%al
		mov    %al,-0x4c(%ebp)
		mov    -0x14(%ebp),%al
		mov    -0x30(%ebp),%dl
		sub    %dl,%al
		mov    %al,-0x34(%ebp)
		jmp    jump_e7eec
	jump_e7ec1:
		xor    %eax,%eax
		mov    -0x34(%ebp),%al
		mov    -0x78(%ebp),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		add    %edx,%eax
		imul   %ebx,%eax
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%edx
		add    %eax,%edx
		add    -0x7c(%ebp),%edx
		xor    %eax,%eax
		mov    -0x2c(%ebp),%al
		add    %eax,%edx
		mov    %edx,-0x70(%ebp)
		mov    %ebx,-0x58(%ebp)
	jump_e7eec:
		mov    -0x4c(%ebp),%al
		mov    -0x34(%ebp),%dh
		sub    %dh,%al
		mov    %al,-0x14(%ebp)
		mov    -0x48(%ebp),%al
		mov    -0x2c(%ebp),%bl
		sub    %bl,%al
		mov    %al,-0x38(%ebp)
		mov    -0x74(%ebp),%eax
		mov    (%eax),%eax
		mov    %eax,-0x68(%ebp)
		cmpb   $0x0,-0x34(%ebp)
		je     jump_e7f3f
		xor    %eax,%eax
		mov    -0x34(%ebp),%al
		mov    %eax,-0x64(%ebp)
	jump_e7f18:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e7f28
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e7f18
	jump_e7f28:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e7f18
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%edx
		test   %edx,%edx
		jne    jump_e7f18
	jump_e7f3f:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0xd,%cl
		je     jump_e8631
		test   $0x1,%cl
		je     jump_e7f87
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		dec    %eax
		add    %eax,-0x70(%ebp)
		movb   $0x1,-0x3c(%ebp)
		mov    -0x74(%ebp),%eax
		mov    0x4(%eax),%al
		mov    %al,-0x4(%ebp)
		mov    -0x48(%ebp),%al
		mov    %al,-0x20(%ebp)
		mov    -0x4(%ebp),%al
		mov    -0x2c(%ebp),%ah
		sub    %ah,%al
		mov    %al,-0x48(%ebp)
		mov    -0x4(%ebp),%al
		mov    -0x20(%ebp),%dl
		sub    %dl,%al
		mov    %al,-0x2c(%ebp)
		jmp    jump_e7f8c
	jump_e7f87:
		xor    %dh,%dh
		mov    %dh,-0x3c(%ebp)
	jump_e7f8c:
		mov    -0x70(%ebp),%eax
		mov    -0x58(%ebp),%ecx
		add    %ecx,%eax
		mov    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
		xor    %eax,%eax
		mov    -0x14(%ebp),%al
		mov    %eax,-0x64(%ebp)
		testb  $0xc,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_e842d
		mov    -0x2c(%ebp),%bh
		test   %bh,%bh
		jne    jump_e81dd
	jump_e7fbf:
		mov    -0x5c(%ebp),%esi
		test   %esi,%esi
		jle    jump_e8197
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e8140
		mov    -0x5c(%ebp),%edi
		cmp    %edi,%eax
		jl     jump_e7fe2
		mov    -0x5c(%ebp),%eax
	jump_e7fe2:
		mov    %al,-0x44(%ebp)
		mov    -0x68(%ebp),%eax
		inc    %eax
		mov    %eax,-0x54(%ebp)
		cmpb   $0x0,-0x3c(%ebp)
		je     jump_e8093
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%ch
		test   $0x4,%ch
		je     jump_e804a
		xor    %al,%al
		mov    %al,-0x1c(%ebp)
		jmp    jump_e801a
	jump_e8008:
		mov    -0x1c(%ebp),%al
		incb   -0x1c(%ebp)
		mov    -0x54(%ebp),%eax
		incl   -0x54(%ebp)
		mov    -0x70(%ebp),%eax
		decl   -0x70(%ebp)
	jump_e801a:
		mov    -0x1c(%ebp),%al
		cmp    -0x44(%ebp),%al
		jae    jump_e8128
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e8008
	jump_e804a:
		xor    %dh,%dh
		mov    %dh,-0x1c(%ebp)
		jmp    jump_e8063
	jump_e8051:
		mov    -0x1c(%ebp),%al
		incb   -0x1c(%ebp)
		mov    -0x54(%ebp),%eax
		incl   -0x54(%ebp)
		mov    -0x70(%ebp),%eax
		decl   -0x70(%ebp)
	jump_e8063:
		mov    -0x1c(%ebp),%al
		cmp    -0x44(%ebp),%al
		jae    jump_e8128
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e8051
	jump_e8093:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%cl
		test   $0x4,%cl
		je     jump_e80e3
		xor    %ch,%ch
		mov    %ch,-0x1c(%ebp)
		jmp    jump_e80b7
	jump_e80a5:
		mov    -0x1c(%ebp),%al
		incb   -0x1c(%ebp)
		mov    -0x54(%ebp),%eax
		incl   -0x54(%ebp)
		mov    -0x70(%ebp),%eax
		incl   -0x70(%ebp)
	jump_e80b7:
		mov    -0x1c(%ebp),%al
		cmp    -0x44(%ebp),%al
		jae    jump_e8128
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e80a5
	jump_e80e3:
		xor    %dl,%dl
		mov    %dl,-0x1c(%ebp)
		jmp    jump_e80fc
	jump_e80ea:
		mov    -0x1c(%ebp),%al
		incb   -0x1c(%ebp)
		mov    -0x54(%ebp),%eax
		incl   -0x54(%ebp)
		mov    -0x70(%ebp),%eax
		incl   -0x70(%ebp)
	jump_e80fc:
		mov    -0x1c(%ebp),%al
		cmp    -0x44(%ebp),%al
		jae    jump_e8128
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e80ea
	jump_e8128:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e7fbf
	jump_e8140:
		test   %eax,%eax
		jge    jump_e816d
		add    %eax,-0x5c(%ebp)
		mov    -0x3c(%ebp),%bh
		test   %bh,%bh
		je     jump_e8159
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x70(%ebp)
		jmp    jump_e8162
	jump_e8159:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x70(%ebp)
	jump_e8162:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e7fbf
	jump_e816d:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%esi
		test   %esi,%esi
		je     jump_e7e52
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e81dd
	jump_e8197:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%ebx
		test   %ebx,%ebx
		jne    jump_e81ac
	jump_e81a1:
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e81ac:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e81bc
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e81ac
	jump_e81bc:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e81ac
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
	jump_e81dd:
		cmpb   $0x0,-0x2c(%ebp)
		je     jump_e7fbf
		mov    -0x2c(%ebp),%al
		mov    %al,-0xc(%ebp)
	jump_e81ed:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e83a8
		xor    %edx,%edx
		mov    -0xc(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e8386
		xor    %eax,%eax
		mov    -0xc(%ebp),%al
		mov    -0x68(%ebp),%edx
		inc    %edx
		add    %eax,%edx
		mov    %edx,-0x50(%ebp)
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		mov    -0xc(%ebp),%ch
		sub    %ch,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%ah
		cmp    %ah,%al
		jbe    jump_e8233
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e8233:
		cmpb   $0x0,-0x3c(%ebp)
		je     jump_e82da
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dh
		test   $0x4,%dh
		je     jump_e8291
		xor    %bl,%bl
		mov    %bl,-0x8(%ebp)
		jmp    jump_e8261
	jump_e824f:
		mov    -0x8(%ebp),%al
		incb   -0x8(%ebp)
		mov    -0x50(%ebp),%eax
		incl   -0x50(%ebp)
		mov    -0x70(%ebp),%eax
		decl   -0x70(%ebp)
	jump_e8261:
		mov    -0x8(%ebp),%al
		cmp    -0x40(%ebp),%al
		jae    jump_e836f
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e824f
	jump_e8291:
		xor    %ch,%ch
		mov    %ch,-0x8(%ebp)
		jmp    jump_e82aa
	jump_e8298:
		mov    -0x8(%ebp),%al
		incb   -0x8(%ebp)
		mov    -0x50(%ebp),%eax
		incl   -0x50(%ebp)
		mov    -0x70(%ebp),%eax
		decl   -0x70(%ebp)
	jump_e82aa:
		mov    -0x8(%ebp),%al
		cmp    -0x40(%ebp),%al
		jae    jump_e836f
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e8298
	jump_e82da:
		mov    EXPORT_SYMBOL(lbDisplay__DrawFlags),%dl
		test   $0x4,%dl
		je     jump_e832a
		xor    %dh,%dh
		mov    %dh,-0x8(%ebp)
		jmp    jump_e82fe
	jump_e82ec:
		mov    -0x8(%ebp),%al
		incb   -0x8(%ebp)
		mov    -0x50(%ebp),%eax
		incl   -0x50(%ebp)
		mov    -0x70(%ebp),%eax
		incl   -0x70(%ebp)
	jump_e82fe:
		mov    -0x8(%ebp),%al
		cmp    -0x40(%ebp),%al
		jae    jump_e836f
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e82ec
	jump_e832a:
		xor    %cl,%cl
		mov    %cl,-0x8(%ebp)
		jmp    jump_e8343
	jump_e8331:
		mov    -0x8(%ebp),%al
		incb   -0x8(%ebp)
		mov    -0x50(%ebp),%eax
		incl   -0x50(%ebp)
		mov    -0x70(%ebp),%eax
		incl   -0x70(%ebp)
	jump_e8343:
		mov    -0x8(%ebp),%al
		cmp    -0x40(%ebp),%al
		jae    jump_e836f
		mov    -0x70(%ebp),%eax
		mov    (%eax),%al
		and    $0xff,%eax
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%edx
		shl    $0x8,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    -0x28(%ebp),%al
		mov    (%edx,%eax,1),%dl
		mov    -0x70(%ebp),%eax
		mov    %dl,(%eax)
		jmp    jump_e8331
	jump_e836f:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e7fbf
	jump_e8386:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		sub    %al,-0xc(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		mov    -0xc(%ebp),%ch
		test   %ch,%ch
		jne    jump_e81ed
		jmp    jump_e7fbf
	jump_e83a8:
		test   %eax,%eax
		jne    jump_e83b4
		mov    %eax,-0x5c(%ebp)
		jmp    jump_e7fbf
	jump_e83b4:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%edx
		xor    %eax,%eax
		mov    -0xc(%ebp),%al
		neg    %eax
		cmp    %eax,%edx
		jge    jump_e840f
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    -0xc(%ebp),%dh
		sub    %dh,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e83e4
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e83e4:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		cmpb   $0x0,-0x3c(%ebp)
		je     jump_e83fa
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x70(%ebp)
		jmp    jump_e8402
	jump_e83fa:
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		add    %eax,-0x70(%ebp)
	jump_e8402:
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e7fbf
	jump_e840f:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		add    %al,-0xc(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		mov    -0xc(%ebp),%ch
		test   %ch,%ch
		jne    jump_e81ed
		jmp    jump_e7fbf
	jump_e842d:
		mov    -0x2c(%ebp),%al
		test   %al,%al
		jne    jump_e851b
	jump_e8438:
		mov    -0x5c(%ebp),%edi
		test   %edi,%edi
		jle    jump_e84d5
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e8493
		mov    -0x5c(%ebp),%edx
		cmp    %edx,%eax
		jl     jump_e8457
		mov    -0x5c(%ebp),%eax
	jump_e8457:
		mov    %al,-0x24(%ebp)
		xor    %ebx,%ebx
		mov    -0x24(%ebp),%bl
		xor    %edx,%edx
		mov    -0x28(%ebp),%dl
		xor    %eax,%eax
		mov    -0x24(%ebp),%al
		mov    -0x70(%ebp),%ecx
		sub    %eax,%ecx
		lea    0x1(%ecx),%eax
		call   ac_memset
		xor    %eax,%eax
		mov    -0x24(%ebp),%al
		sub    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e8438
	jump_e8493:
		test   %eax,%eax
		jge    jump_e84ab
		add    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e8438
	jump_e84ab:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%esi
		test   %esi,%esi
		je     jump_e81a1
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e851b
	jump_e84d5:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%ebx
		test   %ebx,%ebx
		jne    jump_e84ea
	jump_e84df:
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e84ea:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e84fa
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e84ea
	jump_e84fa:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e84ea
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
	jump_e851b:
		cmpb   $0x0,-0x2c(%ebp)
		je     jump_e8438
		mov    -0x2c(%ebp),%al
		mov    %al,-0x18(%ebp)
	jump_e852b:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e85bc
		xor    %edx,%edx
		mov    -0x18(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e859a
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		mov    -0x18(%ebp),%dl
		sub    %dl,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%dh
		cmp    %dh,%al
		jbe    jump_e855f
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e855f:
		xor    %ebx,%ebx
		mov    -0x40(%ebp),%bl
		xor    %edx,%edx
		mov    -0x28(%ebp),%dl
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		mov    -0x70(%ebp),%ecx
		sub    %eax,%ecx
		lea    0x1(%ecx),%eax
		call   ac_memset
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e8438
	jump_e859a:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		sub    %al,-0x18(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		mov    -0x18(%ebp),%al
		test   %al,%al
		jne    jump_e852b
		jmp    jump_e8438
	jump_e85bc:
		test   %eax,%eax
		jne    jump_e85c8
		mov    %eax,-0x5c(%ebp)
		jmp    jump_e8438
	jump_e85c8:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%edx
		xor    %eax,%eax
		mov    -0x18(%ebp),%al
		neg    %eax
		cmp    %eax,%edx
		jge    jump_e8613
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    -0x18(%ebp),%bh
		sub    %bh,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%cl
		cmp    %cl,%al
		jbe    jump_e85f8
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e85f8:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x70(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e8438
	jump_e8613:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		add    %al,-0x18(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		mov    -0x18(%ebp),%al
		test   %al,%al
		jne    jump_e852b
		jmp    jump_e8438
	jump_e8631:
		mov    -0x70(%ebp),%eax
		mov    -0x58(%ebp),%esi
		add    %esi,%eax
		mov    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
		xor    %eax,%eax
		mov    -0x14(%ebp),%al
		mov    %eax,-0x64(%ebp)
		mov    -0x2c(%ebp),%ah
		test   %ah,%ah
		jne    jump_e872b
	jump_e8657:
		mov    -0x5c(%ebp),%edi
		test   %edi,%edi
		jle    jump_e86e5
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e86a3
		cmp    -0x5c(%ebp),%eax
		jl     jump_e8674
		mov    -0x5c(%ebp),%eax
	jump_e8674:
		xor    %edx,%edx
		mov    -0x28(%ebp),%dl
		mov    -0x70(%ebp),%ecx
		mov    %eax,%ebx
		mov    %ecx,%eax
		call   ac_memset
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		add    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e8657
	jump_e86a3:
		test   %eax,%eax
		jge    jump_e86bb
		add    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		sub    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e8657
	jump_e86bb:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%esi
		test   %esi,%esi
		je     jump_e84df
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		jmp    jump_e872b
	jump_e86e5:
		decl   -0x64(%ebp)
		mov    -0x64(%ebp),%ebx
		test   %ebx,%ebx
		jne    jump_e86fa
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret
	jump_e86fa:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e870a
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e86fa
	jump_e870a:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		movsbl (%eax),%eax
		test   %eax,%eax
		jne    jump_e86fa
		mov    -0x6c(%ebp),%eax
		mov    %eax,-0x70(%ebp)
		mov    -0x58(%ebp),%eax
		add    %eax,-0x6c(%ebp)
		xor    %eax,%eax
		mov    -0x38(%ebp),%al
		mov    %eax,-0x5c(%ebp)
	jump_e872b:
		cmpb   $0x0,-0x2c(%ebp)
		je     jump_e8657
		mov    -0x2c(%ebp),%al
		mov    %al,-0x10(%ebp)
	jump_e873b:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		test   %eax,%eax
		jle    jump_e87b0
		xor    %edx,%edx
		mov    -0x10(%ebp),%dl
		cmp    %edx,%eax
		jle    jump_e879c
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		mov    -0x10(%ebp),%dh
		sub    %dh,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%bl
		cmp    %bl,%al
		jbe    jump_e876b
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e876b:
		xor    %ebx,%ebx
		mov    -0x40(%ebp),%bl
		xor    %edx,%edx
		mov    -0x28(%ebp),%dl
		mov    -0x70(%ebp),%eax
		call   ac_memset
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		add    %eax,-0x70(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e8657
	jump_e879c:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		sub    %al,-0x10(%ebp)
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		inc    %eax
		add    %eax,-0x68(%ebp)
		jmp    jump_e8815
	jump_e87b0:
		test   %eax,%eax
		jne    jump_e87bc
		mov    %eax,-0x5c(%ebp)
		jmp    jump_e8657
	jump_e87bc:
		mov    -0x68(%ebp),%eax
		movsbl (%eax),%eax
		xor    %edx,%edx
		mov    -0x10(%ebp),%dl
		neg    %edx
		cmp    %edx,%eax
		jge    jump_e8807
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		neg    %al
		mov    -0x10(%ebp),%cl
		sub    %cl,%al
		mov    %al,-0x40(%ebp)
		mov    -0x40(%ebp),%al
		mov    -0x38(%ebp),%ch
		cmp    %ch,%al
		jbe    jump_e87ec
		mov    -0x38(%ebp),%al
		mov    %al,-0x40(%ebp)
	jump_e87ec:
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		add    %eax,-0x70(%ebp)
		xor    %eax,%eax
		mov    -0x40(%ebp),%al
		sub    %eax,-0x5c(%ebp)
		jmp    jump_e8657
	jump_e8807:
		mov    -0x68(%ebp),%eax
		mov    (%eax),%al
		add    %al,-0x10(%ebp)
		mov    -0x68(%ebp),%eax
		incl   -0x68(%ebp)
	jump_e8815:
		mov    -0x10(%ebp),%dl
		test   %dl,%dl
		jne    jump_e873b
		jmp    jump_e8657
	jump_e8825:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbDrawHVLine_)	/* 0xf0834 */
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		add    $0xfffffff0,%esp
		push   %eax
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		push   %esi
		mov    %eax,-0x4(%ebp)
		mov    %edx,-0x8(%ebp)
		mov    %ebx,-0xc(%ebp)
		mov    %ecx,-0x10(%ebp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%eax
		dec    %eax
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ebx
		dec    %ebx
		mov    -0x4(%ebp),%edi
		mov    -0xc(%ebp),%esi
		cmp    %esi,%edi
		jg     jump_f0895
		cmp    $0x0,%esi
		jge    jump_f0872
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f0872:
		cmp    %eax,%edi
		jle    jump_f0880
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f0880:
		cmp    $0x0,%edi
		jge    jump_f088c
		movl   $0x0,-0x4(%ebp)
	jump_f088c:
		cmp    %eax,%esi
		jle    jump_f08c8
		mov    %eax,-0xc(%ebp)
		jmp    jump_f08c8
	jump_f0895:
		cmp    $0x0,%edi
		jge    jump_f08a4
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f08a4:
		cmp    %eax,%esi
		jle    jump_f08b2
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f08b2:
		cmp    $0x0,%esi
		jge    jump_f08bc
		mov    $0x0,%esi
	jump_f08bc:
		cmp    %eax,%edi
		jle    jump_f08c2
		mov    %eax,%edi
	jump_f08c2:
		mov    %esi,-0x4(%ebp)
		mov    %edi,-0xc(%ebp)
	jump_f08c8:
		mov    -0x8(%ebp),%edi
		mov    -0x10(%ebp),%esi
		cmp    %esi,%edi
		jg     jump_f0904
		cmp    $0x0,%esi
		jge    jump_f08e1
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f08e1:
		cmp    %ebx,%edi
		jle    jump_f08ef
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f08ef:
		cmp    $0x0,%edi
		jge    jump_f08fb
		movl   $0x0,-0x8(%ebp)
	jump_f08fb:
		cmp    %ebx,%esi
		jle    jump_f0937
		mov    %ebx,-0x10(%ebp)
		jmp    jump_f0937
	jump_f0904:
		cmp    $0x0,%edi
		jge    jump_f0913
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f0913:
		cmp    %ebx,%esi
		jle    jump_f0921
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4
	jump_f0921:
		cmp    $0x0,%esi
		jge    jump_f092b
		mov    $0x0,%esi
	jump_f092b:
		cmp    %ebx,%edi
		jle    jump_f0931
		mov    %ebx,%edi
	jump_f0931:
		mov    %esi,-0x8(%ebp)
		mov    %edi,-0x10(%ebp)
	jump_f0937:
		mov    -0x8(%ebp),%edi
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		add    -0x4(%ebp),%edi
		add    EXPORT_SYMBOL(lbDisplay__GraphicsWindowPtr),%edi
		mov    -0xc(%ebp),%ecx
		sub    -0x4(%ebp),%ecx
		jne    jump_f09bb
		mov    -0x10(%ebp),%ecx
		sub    -0x8(%ebp),%ecx
		inc    %ecx
		testw  $0x4,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_f0985
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
		shl    $0x8,%eax
	jump_f0971:
		mov    (%edi),%al
		mov    (%eax,%esi,1),%bl
		mov    %bl,(%edi)
		add    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		loop   jump_f0971
		jmp    jump_f0a0b
	jump_f0985:
		testw  $0x8,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_f09ab
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
	jump_f099a:
		mov    (%edi),%ah
		mov    (%eax,%esi,1),%bl
		mov    %bl,(%edi)
		add    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		loop   jump_f099a
		jmp    jump_f0a0b
	jump_f09ab:
		movzbl 0x8(%ebp),%eax
	jump_f09af:
		mov    %al,(%edi)
		add    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		loop   jump_f09af
		jmp    jump_f0a0b
	jump_f09bb:
		inc    %ecx
		testw  $0x4,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_f09e2
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
		shl    $0x8,%eax
	jump_f09d4:
		mov    (%edi),%al
		mov    (%eax,%esi,1),%bl
		mov    %bl,(%edi)
		add    $0x1,%edi
		loop   jump_f09d4
		jmp    jump_f0a0b
	jump_f09e2:
		testw  $0x8,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_f0a05
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
	jump_f09f7:
		mov    (%edi),%ah
		mov    (%eax,%esi,1),%bl
		mov    %bl,(%edi)
		add    $0x1,%edi
		loop   jump_f09f7
		jmp    jump_f0a0b
	jump_f0a05:
		movzbl 0x8(%ebp),%eax
		rep stos %al,%es:(%edi)
	jump_f0a0b:
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4


/*----------------------------------------------------------------*/
func_fadd0:
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		add    $0xfffffff0,%esp
		push   %eax
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		push   %esi
		mov    %eax,-0x4(%ebp)
		mov    %edx,-0x8(%ebp)
		mov    %ebx,-0xc(%ebp)
		mov    %ecx,-0x10(%ebp)
		mov    -0x8(%ebp),%edi
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		jge    jump_faee2
		cmp    $0x0,%edi
		jge    jump_fae01
		add    %edi,-0x10(%ebp)
		xor    %edi,%edi
	jump_fae01:
		mov    %edi,%edx
		add    -0x10(%ebp),%edx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edx
		jle    jump_fae17
		sub    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edx
		sub    %edx,-0x10(%ebp)
	jump_fae17:
		cmpl   $0x0,-0x10(%ebp)
		jle    jump_faee2
		add    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%edi
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edi
		mov    -0x4(%ebp),%eax
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%eax
		jge    jump_faee2
		cmp    $0x0,%eax
		jge    jump_fae47
		add    %eax,-0xc(%ebp)
		xor    %eax,%eax
	jump_fae47:
		mov    %eax,%edx
		add    -0xc(%ebp),%edx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%edx
		jle    jump_fae5d
		sub    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%edx
		sub    %edx,-0xc(%ebp)
	jump_fae5d:
		cmpl   $0x0,-0xc(%ebp)
		jle    jump_faee2
		add    %eax,%edi
		add    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%edi
		add    EXPORT_SYMBOL(lbDisplay__WScreen),%edi
		mov    -0x10(%ebp),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		sub    -0xc(%ebp),%ebx
		testw  $0x4,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_faea8
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
		shl    $0x8,%eax
	jump_fae95:
		mov    -0xc(%ebp),%ecx
	jump_fae98:
		mov    (%edi),%al
		mov    (%eax,%esi,1),%al
		stos   %al,%es:(%edi)
		dec    %ecx
		jne    jump_fae98
		add    %ebx,%edi
		dec    %edx
		jne    jump_fae95
		jmp    jump_faee2
	jump_faea8:
		testw  $0x8,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_faed3
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%esi
		movzbl 0x8(%ebp),%eax
	jump_faebd:
		mov    -0xc(%ebp),%ecx
	jump_faec0:
		mov    0x8(%ebp),%al
		mov    (%edi),%ah
		mov    (%eax,%esi,1),%al
		stos   %al,%es:(%edi)
		dec    %ecx
		jne    jump_faec0
		add    %ebx,%edi
		dec    %edx
		jne    jump_faebd
		jmp    jump_faee2
	jump_faed3:
		mov    0x8(%ebp),%al
	jump_faed6:
		mov    -0xc(%ebp),%ecx
	jump_faed9:
		stos   %al,%es:(%edi)
		dec    %ecx
		jne    jump_faed9
		add    %ebx,%edi
		dec    %edx
		jne    jump_faed6
	jump_faee2:
		pop    %esi
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0x4


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbDrawTriangle_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		sub    $0x10,%esp
		mov    %eax,-0x10(%ebp)
		mov    %edx,-0xc(%ebp)
		mov    %ebx,-0x8(%ebp)
		mov    %ecx,-0x4(%ebp)
		testb  $0x10,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_ef4c2
		xor    %eax,%eax
		mov    0x10(%ebp),%al
		mov    -0x4(%ebp),%ecx
		mov    -0x8(%ebp),%ebx
		mov    -0xc(%ebp),%edx
		push   %eax
		mov    -0x10(%ebp),%eax
		call   LbDrawLine_
		xor    %eax,%eax
		mov    0x10(%ebp),%al
		mov    0xc(%ebp),%ecx
		mov    0x8(%ebp),%ebx
		mov    -0x4(%ebp),%edx
		push   %eax
		mov    -0x8(%ebp),%eax
		call   LbDrawLine_
		xor    %eax,%eax
		mov    0x10(%ebp),%al
		mov    -0xc(%ebp),%ecx
		mov    -0x10(%ebp),%ebx
		mov    0xc(%ebp),%edx
		push   %eax
		mov    0x8(%ebp),%eax
		call   LbDrawLine_
		jmp    jump_ef4e1
	jump_ef4c2:
		xor    %eax,%eax
		mov    0x10(%ebp),%al
		push   %eax
		mov    0xc(%ebp),%eax
		push   %eax
		mov    0x8(%ebp),%eax
		mov    -0x4(%ebp),%ecx
		mov    -0x8(%ebp),%ebx
		mov    -0xc(%ebp),%edx
		push   %eax
		mov    -0x10(%ebp),%eax
		call   LbDrawTriangleFilled_
	jump_ef4e1:
		mov    %ebp,%esp
		pop    %ebp
		ret    $0xc


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbSpriteSetScalingData)
/*----------------------------------------------------------------*/
		pusha
		mov    0x24(%esp),%esi
		mov    0x28(%esp),%edi
		sub    $0x20,%esp
		mov    $0x1,%ebp
		cmp    %ebx,%esi
		jg     jump_136a23
		cmp    %ecx,%edi
		jg     jump_136a23
		xor    %ebp,%ebp
	jump_136a23:
		mov    %ebp,data_1f0350
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%ebp
		mov    %ebp,0x18(%esp)
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ebp
		mov    %ebp,0x1c(%esp)
		mov    %eax,(%esp)
		mov    %edx,0x4(%esp)
		mov    %ebx,0x8(%esp)
		mov    %ecx,0xc(%esp)
		mov    %esi,0x10(%esp)
		mov    %edi,0x14(%esp)
		or     %eax,%eax
		js     jump_136b52
		mov    %eax,%ebp
		add    %esi,%ebp
		cmp    0x18(%esp),%ebp
		jge    jump_136b52
		mov    (%esp),%esi
		shl    $0x10,%esi
		mov    0x10(%esp),%eax
		mov    0x8(%esp),%ebx
		shl    $0x10,%eax
		xor    %edx,%edx
		idiv   %ebx
		lea    data_1f0354,%edx
		mov    0x8(%esp),%ecx
		mov    %eax,%ebx
		sar    %ebx
		add    %ebx,%esi
		mov    %esi,%edi
		sar    $0x10,%edi
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
	jump_136aa0:
		mov    %edi,(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x4(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x8(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0xc(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x10(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x14(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x18(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x1c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x20(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x24(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x28(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x2c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x30(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x34(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		mov    %edi,0x38(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x3c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136b50
		add    $0x40,%edx
		jmp    jump_136aa0
	jump_136b50:
		jmp    jump_136bb5
	jump_136b52:
		mov    (%esp),%esi
		shl    $0x10,%esi
		mov    0x10(%esp),%eax
		mov    0x8(%esp),%ebx
		shl    $0x10,%eax
		xor    %edx,%edx
		idiv   %ebx
		lea    data_1f0354,%edx
		mov    0x8(%esp),%ecx
		mov    %eax,%ebx
		sar    %ebx
		add    %ebx,%esi
		mov    %esi,%edi
		sar    $0x10,%edi
		jns    jump_136b83
		mov    $0x0,%edi
	jump_136b83:
		cmp    0x18(%esp),%edi
		jl     jump_136b8d
		mov    0x18(%esp),%edi
	jump_136b8d:
		mov    %edi,(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		jns    jump_136b9c
		xor    %ebx,%ebx
		mov    %eax,%eax
	jump_136b9c:
		cmp    0x18(%esp),%ebx
		jl     jump_136ba8
		mov    0x18(%esp),%ebx
		mov    %eax,%eax
	jump_136ba8:
		sub    %edi,%ebx
		mov    %ebx,0x4(%edx)
		add    %ebx,%edi
		add    $0x8,%edx
		dec    %ecx
		jne    jump_136b8d
	jump_136bb5:
		or     %edx,%edx
		js     jump_136cb2
		mov    %edx,%ebx
		add    %edi,%ebx
		cmp    0x1c(%esp),%ebx
		jge    jump_136cb2
		mov    0x4(%esp),%esi
		shl    $0x10,%esi
		mov    0x14(%esp),%eax
		mov    0xc(%esp),%ebx
		shl    $0x10,%eax
		xor    %edx,%edx
		idiv   %ebx
		lea    data_1f0754,%edx
		mov    0xc(%esp),%ecx
		mov    %eax,%ebx
		sar    %ebx
		add    %ebx,%esi
		mov    %esi,%edi
		sar    $0x10,%edi
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		cs
		mov    %eax,%eax
		nop
	jump_136c00:
		mov    %edi,(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x4(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x8(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0xc(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x10(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x14(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x18(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x1c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x20(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x24(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x28(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x2c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x30(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x34(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		mov    %edi,0x38(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		sub    %edi,%ebx
		mov    %ebx,0x3c(%edx)
		add    %ebx,%edi
		dec    %ecx
		je     jump_136cb0
		add    $0x40,%edx
		jmp    jump_136c00
	jump_136cb0:
		jmp    jump_136d15
	jump_136cb2:
		mov    0x4(%esp),%esi
		shl    $0x10,%esi
		mov    0x14(%esp),%eax
		mov    0xc(%esp),%ebx
		shl    $0x10,%eax
		xor    %edx,%edx
		idiv   %ebx
		lea    data_1f0754,%edx
		mov    0xc(%esp),%ecx
		mov    %eax,%ebx
		sar    %ebx
		add    %ebx,%esi
		mov    %esi,%edi
		sar    $0x10,%edi
		jns    jump_136ce4
		mov    $0x0,%edi
	jump_136ce4:
		cmp    0x1c(%esp),%edi
		jl     jump_136cee
		mov    0x1c(%esp),%edi
	jump_136cee:
		mov    %edi,(%edx)
		add    %eax,%esi
		mov    %esi,%ebx
		sar    $0x10,%ebx
		jns    jump_136cfc
		xor    %ebx,%ebx
		nop
	jump_136cfc:
		cmp    0x1c(%esp),%ebx
		jl     jump_136d08
		mov    0x1c(%esp),%ebx
		mov    %eax,%eax
	jump_136d08:
		sub    %edi,%ebx
		mov    %ebx,0x4(%edx)
		add    %ebx,%edi
		add    $0x8,%edx
		dec    %ecx
		jne    jump_136cee
	jump_136d15:
		add    $0x20,%esp
		popa
		ret    $0x8


/*----------------------------------------------------------------*/
LbDrawTriangleFilled_:
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		add    $0xffffffb4,%esp
		push   %eax
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,-0x4(%ebp)
		mov    %edx,-0x8(%ebp)
		mov    %ebx,-0xc(%ebp)
		mov    %ecx,-0x10(%ebp)
		lea    -0x14(%ebp),%esi
		lea    -0x18(%ebp),%edi
		lea    -0x1c(%ebp),%ecx
		mov    -0x4(%ebp),%eax
		mov    %ax,(%esi)
		mov    -0xc(%ebp),%eax
		mov    %ax,(%edi)
		mov    0x8(%ebp),%eax
		mov    %ax,(%ecx)
		mov    -0x8(%ebp),%eax
		mov    %ax,0x2(%esi)
		mov    -0x10(%ebp),%ebx
		mov    %bx,0x2(%edi)
		mov    0xc(%ebp),%edx
		mov    %dx,0x2(%ecx)
		mov    0x2(%esi),%ax
		mov    0x2(%edi),%bx
		mov    0x2(%ecx),%dx
		cmp    %bx,%ax
		je     jump_138959
		jg     jump_13891b
		cmp    %dx,%ax
		je     jump_138944
		js     jump_13899b
		xchg   %esi,%ecx
		xchg   %edi,%ecx
		jmp    jump_1389a6
	jump_13891b:
		cmp    %dx,%ax
		je     jump_138d51
		js     jump_138b79
		cmp    %dx,%bx
		je     jump_138e56
		js     jump_13893e
		xchg   %esi,%ecx
		xchg   %edi,%ecx
		jmp    jump_138b7d
	jump_13893e:
		xchg   %esi,%edi
		xchg   %edi,%ecx
		jmp    jump_1389a6
	jump_138944:
		mov    (%esi),%ax
		cmp    (%ecx),%ax
		jle    jump_139010
		xchg   %esi,%ecx
		xchg   %edi,%ecx
		jmp    jump_138e66
	jump_138959:
		cmp    %dx,%ax
		je     jump_139010
		js     jump_138979
		mov    (%esi),%ax
		cmp    (%edi),%ax
		jle    jump_139010
		xchg   %esi,%ecx
		xchg   %edi,%ecx
		jmp    jump_138d61
	jump_138979:
		mov    (%edi),%ax
		cmp    (%esi),%ax
		jle    jump_139010
		jmp    jump_138e66
	jump_13898a:
		mov    (%edi),%ax
		cmp    (%ecx),%ax
		jle    jump_139010
		jmp    jump_138d61
	jump_13899b:
		cmp    %dx,%bx
		je     jump_13898a
		jg     jump_138b7d
	jump_1389a6:
		mov    0x2(%esi),%ax
		mov    %ax,-0x46(%ebp)
		or     %ax,%ax
		jns    jump_1389c2
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x1,-0x47(%ebp)
		jmp    jump_1389e6
	jump_1389c2:
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ax
		jge    jump_139010
		movzwl %ax,%ebx
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		add    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x0,-0x47(%ebp)
	jump_1389e6:
		movzwl 0x2(%ecx),%ebx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x49(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x30(%ebp)
		mov    %ebx,-0x40(%ebp)
		mov    0x2(%edi),%bx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x48(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x34(%ebp)
		mov    (%ecx),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idivl  -0x30(%ebp)
		mov    %eax,-0x24(%ebp)
		mov    (%edi),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idivl  -0x34(%ebp)
		cmp    -0x24(%ebp),%eax
		jle    jump_139010
		mov    %eax,-0x28(%ebp)
		movzwl 0x2(%ecx),%ebx
		sub    0x2(%edi),%bx
		mov    (%ecx),%ax
		sub    (%edi),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x2c(%ebp)
		mov    %ebx,-0x38(%ebp)
		mov    (%edi),%ax
		shl    $0x10,%eax
		mov    %eax,-0x3c(%ebp)
		mov    (%esi),%ax
		shl    $0x10,%eax
		mov    %eax,%ebx
		cmpb   $0x0,-0x47(%ebp)
		je     jump_138b0d
		movswl -0x46(%ebp),%edi
		neg    %edi
		sub    %edi,-0x40(%ebp)
		jle    jump_139010
		mov    %edi,-0x44(%ebp)
		cmp    -0x34(%ebp),%di
		js     jump_138ace
		mov    -0x24(%ebp),%edi
		imul   -0x34(%ebp),%edi
		add    %edi,%eax
		mov    -0x3c(%ebp),%ebx
		mov    -0x44(%ebp),%edi
		sub    -0x34(%ebp),%edi
		sub    %di,-0x38(%ebp)
		mov    %edi,-0x44(%ebp)
		imul   -0x24(%ebp),%edi
		add    %edi,%eax
		mov    -0x44(%ebp),%edi
		imul   -0x2c(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138ac3
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %edi,-0x38(%ebp)
		mov    %edi,-0x40(%ebp)
	jump_138ac3:
		lea    data_1e957c,%edi
		jmp    jump_138b55
	jump_138ace:
		mov    -0x44(%ebp),%edi
		sub    %di,-0x34(%ebp)
		imul   -0x24(%ebp),%edi
		add    %edi,%eax
		mov    -0x28(%ebp),%edi
		imul   -0x44(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138b38
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %edi,-0x40(%ebp)
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138aff
		mov    %di,-0x34(%ebp)
		jmp    jump_138b0b
	jump_138aff:
		sub    -0x34(%ebp),%di
		setle  -0x48(%ebp)
		mov    %di,-0x38(%ebp)
	jump_138b0b:
		jmp    jump_138b38
	jump_138b0d:
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138b38
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		sub    -0x46(%ebp),%di
		mov    %edi,-0x40(%ebp)
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138b2c
		mov    %di,-0x34(%ebp)
		jmp    jump_138b38
	jump_138b2c:
		sub    -0x34(%ebp),%di
		setle  -0x48(%ebp)
		mov    %di,-0x38(%ebp)
	jump_138b38:
		lea    data_1e957c,%edi
	jump_138b3e:
		mov    %eax,(%edi)
		add    -0x24(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x28(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x34(%ebp)
		jne    jump_138b3e
		mov    -0x3c(%ebp),%ebx
	jump_138b55:
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138b60
		jmp    jump_138f5a
	jump_138b60:
		mov    %eax,(%edi)
		add    -0x24(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x2c(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x38(%ebp)
		jne    jump_138b60
		jmp    jump_138f5a
	jump_138b79:
		xchg   %esi,%edi
		xchg   %edi,%ecx
	jump_138b7d:
		mov    0x2(%esi),%ax
		mov    %ax,-0x46(%ebp)
		or     %ax,%ax
		jns    jump_138b99
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x1,-0x47(%ebp)
		jmp    jump_138bbd
	jump_138b99:
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ax
		jge    jump_139010
		movzwl %ax,%ebx
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		add    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x0,-0x47(%ebp)
	jump_138bbd:
		movzwl 0x2(%ecx),%ebx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x48(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x30(%ebp)
		mov    0x2(%edi),%bx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x49(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x34(%ebp)
		mov    %ebx,-0x40(%ebp)
		mov    (%ecx),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idivl  -0x30(%ebp)
		mov    %eax,-0x24(%ebp)
		mov    (%edi),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idivl  -0x34(%ebp)
		cmp    -0x24(%ebp),%eax
		jle    jump_139010
		mov    %eax,-0x28(%ebp)
		movzwl 0x2(%edi),%ebx
		sub    0x2(%ecx),%bx
		mov    (%edi),%ax
		sub    (%ecx),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x2c(%ebp)
		mov    %ebx,-0x38(%ebp)
		mov    (%ecx),%ax
		shl    $0x10,%eax
		mov    %eax,-0x3c(%ebp)
		mov    (%esi),%ax
		shl    $0x10,%eax
		mov    %eax,%ebx
		cmpb   $0x0,-0x47(%ebp)
		je     jump_138ce5
		movswl -0x46(%ebp),%edi
		neg    %edi
		sub    %edi,-0x40(%ebp)
		jle    jump_139010
		mov    %edi,-0x44(%ebp)
		cmp    -0x30(%ebp),%di
		js     jump_138ca6
		mov    -0x28(%ebp),%edi
		imul   -0x30(%ebp),%edi
		add    %edi,%ebx
		mov    -0x3c(%ebp),%eax
		mov    -0x44(%ebp),%edi
		sub    -0x30(%ebp),%edi
		mov    %edi,-0x44(%ebp)
		sub    %di,-0x38(%ebp)
		imul   -0x2c(%ebp),%edi
		add    %edi,%eax
		mov    -0x28(%ebp),%edi
		imul   -0x44(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138c9b
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %di,-0x38(%ebp)
		mov    %edi,-0x40(%ebp)
	jump_138c9b:
		lea    data_1e957c,%edi
		jmp    jump_138d2d
	jump_138ca6:
		mov    -0x44(%ebp),%edi
		sub    %di,-0x30(%ebp)
		imul   -0x24(%ebp),%edi
		add    %edi,%eax
		mov    -0x28(%ebp),%edi
		imul   -0x44(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138d10
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %edi,-0x40(%ebp)
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138cd7
		mov    %di,-0x30(%ebp)
		jmp    jump_138ce3
	jump_138cd7:
		sub    -0x30(%ebp),%di
		setle  -0x48(%ebp)
		mov    %di,-0x38(%ebp)
	jump_138ce3:
		jmp    jump_138d10
	jump_138ce5:
		cmpb   $0x0,-0x49(%ebp)
		je     jump_138d10
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		sub    -0x46(%ebp),%di
		mov    %edi,-0x40(%ebp)
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138d04
		mov    %di,-0x30(%ebp)
		jmp    jump_138d10
	jump_138d04:
		sub    -0x30(%ebp),%di
		setle  -0x48(%ebp)
		mov    %di,-0x38(%ebp)
	jump_138d10:
		lea    data_1e957c,%edi
	jump_138d16:
		mov    %eax,(%edi)
		add    -0x24(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x28(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x30(%ebp)
		jne    jump_138d16
		mov    -0x3c(%ebp),%eax
	jump_138d2d:
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138d38
		jmp    jump_138f5a
	jump_138d38:
		mov    %eax,(%edi)
		add    -0x2c(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x28(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x38(%ebp)
		jne    jump_138d38
		jmp    jump_138f5a
	jump_138d51:
		mov    (%ecx),%ax
		cmp    (%esi),%ax
		jle    jump_139010
		xchg   %esi,%edi
		xchg   %edi,%ecx
	jump_138d61:
		mov    0x2(%esi),%ax
		mov    %ax,-0x46(%ebp)
		or     %ax,%ax
		jns    jump_138d7d
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x1,-0x47(%ebp)
		jmp    jump_138da1
	jump_138d7d:
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ax
		jge    jump_139010
		movzwl %ax,%ebx
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		add    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x0,-0x47(%ebp)
	jump_138da1:
		movzwl 0x2(%ecx),%ebx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x48(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x30(%ebp)
		mov    %ebx,-0x40(%ebp)
		mov    (%ecx),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x24(%ebp)
		mov    (%edi),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x28(%ebp)
		mov    (%esi),%ax
		shl    $0x10,%eax
		mov    %eax,%ebx
		cmpb   $0x0,-0x47(%ebp)
		je     jump_138e20
		movswl -0x46(%ebp),%edi
		neg    %edi
		sub    %di,-0x30(%ebp)
		sub    %di,-0x40(%ebp)
		jle    jump_139010
		mov    %edi,-0x44(%ebp)
		imul   -0x24(%ebp),%edi
		add    %edi,%eax
		mov    -0x28(%ebp),%edi
		imul   -0x44(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138e37
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %edi,-0x40(%ebp)
		mov    %di,-0x30(%ebp)
		jmp    jump_138e37
	jump_138e20:
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138e37
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		sub    -0x46(%ebp),%di
		mov    %edi,-0x40(%ebp)
		mov    %di,-0x30(%ebp)
	jump_138e37:
		lea    data_1e957c,%edi
	jump_138e3d:
		mov    %eax,(%edi)
		add    -0x24(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x28(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x30(%ebp)
		jne    jump_138e3d
		jmp    jump_138f5a
	jump_138e56:
		mov    (%ecx),%ax
		cmp    (%edi),%ax
		jle    jump_139010
		xchg   %esi,%edi
		xchg   %edi,%ecx
	jump_138e66:
		mov    0x2(%esi),%ax
		mov    %ax,-0x46(%ebp)
		or     %ax,%ax
		jns    jump_138e82
		mov    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x1,-0x47(%ebp)
		jmp    jump_138ea6
	jump_138e82:
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%ax
		jge    jump_139010
		movzwl %ax,%ebx
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%ebx
		add    EXPORT_SYMBOL(lbDisplay__WScreen),%ebx
		mov    %ebx,-0x20(%ebp)
		movb   $0x0,-0x47(%ebp)
	jump_138ea6:
		movzwl 0x2(%ecx),%ebx
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%bx
		setg   -0x48(%ebp)
		sub    %ax,%bx
		mov    %ebx,-0x30(%ebp)
		mov    %ebx,-0x40(%ebp)
		mov    (%ecx),%ax
		sub    (%esi),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x24(%ebp)
		mov    (%ecx),%ax
		sub    (%edi),%ax
		shl    $0x10,%eax
		cltd
		idiv   %ebx
		mov    %eax,-0x28(%ebp)
		mov    (%esi),%ax
		shl    $0x10,%eax
		mov    (%edi),%bx
		shl    $0x10,%ebx
		cmpb   $0x0,-0x47(%ebp)
		je     jump_138f29
		movswl -0x46(%ebp),%edi
		neg    %edi
		sub    %di,-0x30(%ebp)
		sub    %di,-0x40(%ebp)
		jle    jump_139010
		mov    %edi,-0x44(%ebp)
		imul   -0x24(%ebp),%edi
		add    %edi,%eax
		mov    -0x28(%ebp),%edi
		imul   -0x44(%ebp),%edi
		add    %edi,%ebx
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138f40
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		mov    %edi,-0x40(%ebp)
		mov    %di,-0x30(%ebp)
		jmp    jump_138f40
	jump_138f29:
		cmpb   $0x0,-0x48(%ebp)
		je     jump_138f40
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowHeight),%edi
		sub    -0x46(%ebp),%di
		mov    %edi,-0x40(%ebp)
		mov    %di,-0x30(%ebp)
	jump_138f40:
		lea    data_1e957c,%edi
	jump_138f46:
		mov    %eax,(%edi)
		add    -0x24(%ebp),%eax
		mov    %ebx,0x4(%edi)
		add    -0x28(%ebp),%ebx
		add    $0x8,%edi
		decw   -0x30(%ebp)
		jne    jump_138f46
	jump_138f5a:
		lea    data_1e957c,%esi
		mov    -0x20(%ebp),%edx
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowY),%eax
		dec    %eax
		imul   EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%eax
		add    EXPORT_SYMBOL(lbDisplay__GraphicsWindowX),%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x10(%ebp),%al
		xor    %ecx,%ecx
	jump_138f7f:
		xor    %ebx,%ebx
		mov    0x2(%esi),%bx
		mov    0x6(%esi),%cx
		add    EXPORT_SYMBOL(lbDisplay__GraphicsScreenWidth),%edx
		or     %bx,%bx
		jns    jump_138fb3
		or     %cx,%cx
		jle    jump_139004
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%cx
		jle    jump_138fa9
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%cx
	jump_138fa9:
		mov    %edx,%edi
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
		jmp    jump_138fd1
	jump_138fb3:
		cmp    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%cx
		jle    jump_138fc3
		mov    EXPORT_SYMBOL(lbDisplay__GraphicsWindowWidth),%cx
	jump_138fc3:
		sub    %bx,%cx
		jle    jump_139004
		lea    (%ebx,%edx,1),%edi
		mov    EXPORT_SYMBOL(lbDisplay__GlassMap),%ebx
	jump_138fd1:
		testw  $0x4,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_138fea
		mov    0x10(%ebp),%ah
	jump_138fdf:
		mov    (%edi),%al
		mov    (%eax,%ebx,1),%al
		stos   %al,%es:(%edi)
		dec    %ecx
		jne    jump_138fdf
		jmp    jump_139004
	jump_138fea:
		testw  $0x8,EXPORT_SYMBOL(lbDisplay__DrawFlags)
		je     jump_139002
	jump_138ff5:
		mov    (%edi),%ah
		mov    (%eax,%ebx,1),%ah
		mov    %ah,(%edi)
		inc    %edi
		dec    %ecx
		jne    jump_138ff5
		jmp    jump_139004
	jump_139002:
		rep stos %al,%es:(%edi)
	jump_139004:
		add    $0x8,%esi
		decl   -0x40(%ebp)
		jne    jump_138f7f
	jump_139010:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		pop    %eax
		leave
		ret    $0xc


.data

data_1e957c:
		.fill   0xf00

GLOBAL (data_1f0350)
		.long	0x0
GLOBAL (data_1f0354)
		.fill   0x400
GLOBAL (data_1f0754)
		.fill   0x400
data_1f0b54:
		.long	0x0
data_1f0b58:
		.fill   0x800
data_1f1358:
		.fill   0x800
