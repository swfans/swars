
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
/* long LbScreenSetupAnyMode(char mode, unsigned int width, unsigned int height, unsigned char *palette)
 */
GLOBAL_FUNC (ASM_LbScreenSetupAnyMode_)	/* 0xED4F0 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x38,%esp
		call   LbExeReferenceNumber_
		movsbl 0x18(%ebp),%eax
		call   LbScreenIsModeAvailable_
		test   %eax,%eax
		jne    jump_ed516
		mov    $0xffffffff,%eax
		jmp    jump_ed6e3
	jump_ed516:
		call   LbMouseSuspend_
		cmpw   $0x0,lbDisplay__OldVideoMode
		jne    jump_ed547
		movl   $0xf00,-0x38(%ebp)
		lea    -0x1c(%ebp),%ebx
		lea    -0x38(%ebp),%edx
		mov    $0x10,%eax
		call   ac_dos_int386
		xor    %ah,%ah
		mov    -0x1c(%ebp),%al
		mov    %ax,lbDisplay__OldVideoMode
	jump_ed547:
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		cmpl   $0x100,lbScreenModeInfo0_VideoFlags(,%eax,2)
		jl     jump_ed590
		movb   $0x1,lbDisplay__VesaIsSetUp
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_Field8(,%eax,2),%eax
		sar    $0x10,%eax
		call   LbVesaSetMode_
		jmp    jump_ed5c4
	jump_ed590:
		xor    %dl,%dl
		mov    %dl,lbDisplay__VesaIsSetUp
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_VideoFlags(,%eax,2),%eax
		mov    %eax,-0x38(%ebp)
		lea    -0x1c(%ebp),%ebx
		lea    -0x38(%ebp),%edx
		mov    $0x10,%eax
		call   ac_dos_int386
	jump_ed5c4:
		movl   $0xa0000,lbDisplay__PhysicalScreen
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_Width(,%eax,2),%ax
		and    $0xffff,%eax
		mov    %eax,lbDisplay__PhysicalScreenWidth
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_Height(,%eax,2),%ax
		and    $0xffff,%eax
		mov    %eax,lbDisplay__PhysicalScreenHeight
		mov    0x1c(%ebp),%eax
		mov    %eax,lbDisplay__GraphicsScreenWidth
		mov    0x20(%ebp),%eax
		mov    %eax,lbDisplay__GraphicsScreenHeight
		movsbw 0x18(%ebp),%ax
		mov    %ax,lbDisplay__ScreenMode
		xor    %ebx,%ebx
		xor    %dh,%dh
		mov    %bx,lbDisplay__DrawFlags
		mov    %dh,lbDisplay__DrawColour
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		xor    %ecx,%ecx
		mov    lbScreenModeInfo0_Height(,%eax,2),%cx
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		xor    %ebx,%ebx
		mov    lbScreenModeInfo0_Width(,%eax,2),%bx
		xor    %edx,%edx
		xor    %eax,%eax
		call   LbScreenSetGraphicsWindow_
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		xor    %ecx,%ecx
		mov    lbScreenModeInfo0_Height(,%eax,2),%cx
		movsbl 0x18(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		xor    %ebx,%ebx
		mov    lbScreenModeInfo0_Width(,%eax,2),%bx
		xor    %edx,%edx
		xor    %eax,%eax
		call   LbTextSetWindow_
		cmpl   $0x0,0x24(%ebp)
		je     jump_ed6d6
		mov    0x24(%ebp),%eax
		push   %eax
		call   ac_LbPaletteSet
		add    $0x4,%esp
	jump_ed6d6:
		mov    $0x1,%edi
		mov    %edi,screen_initialised
		mov    %edi,%eax
	jump_ed6e3:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbScreenReset_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x38,%esp
		cmpl   $0x0,screen_initialised
		jne    jump_ed77c
		mov    $0xffffffff,%eax
		jmp    jump_ed7a9
	jump_ed77c:
		call   LbMouseSuspend_
#if 0
		xor    %eax,%eax
		mov    lbDisplay__OldVideoMode,%ax
		mov    %eax,-0x1c(%ebp)
		lea    -0x38(%ebp),%ebx
		lea    -0x1c(%ebp),%edx
		mov    $0x10,%eax
		call   ac_dos_int386
#endif
		xor    %ebx,%ebx
		mov    $0x1,%eax
		mov    %ebx,screen_initialised
	jump_ed7a9:
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* long LbScreenLock(void)
 */
GLOBAL_FUNC (LbScreenLock_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		mov    $0x1,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
/* long LbScreenUnlock(void)
 */
GLOBAL_FUNC (LbScreenUnlock_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		mov    $0x1,%eax
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
LbScreenFindVideoModes_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x4,%esp
		mov    $0x1,%edx
		call   LbVesaGetInfo_
		mov    %edx,-0x4(%ebp)
		jmp    jump_ed827
	jump_ed821:
		mov    -0x4(%ebp),%eax
		incl   -0x4(%ebp)
	jump_ed827:
		mov    -0x4(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_Width(,%eax,2),%ax
		and    $0xffff,%eax
		je     jump_ed8a9
		mov    -0x4(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_VideoFlags(,%eax,2),%ecx
		cmp    $0x100,%ecx
		jl     jump_ed821
		mov    -0x4(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_VideoFlags(,%eax,2),%ax
		and    $0xffff,%eax
		call   LbVesaIsModeAvailable_
		mov    %eax,%ebx
		mov    -0x4(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    %ebx,lbScreenModeInfo0_Field6(,%eax,2)
		jmp    jump_ed821
	jump_ed8a9:
		mov    $0x1,%esi
		mov    %esi,lbScreenModeInfo1_Field6
		mov    %esi,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (ASM_LbScreenSwap)	/* 0xED8C0 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x10,%esp
		call   LbMousePlace_
		cmpb   $0x0,lbDisplay__VesaIsSetUp
		jne    jump_ed8f9
		mov    lbDisplay__GraphicsScreenWidth,%ecx
		imul   lbDisplay__GraphicsScreenHeight,%ecx
		mov    lbDisplay__WScreen,%esi
		mov    lbDisplay__PhysicalScreen,%edi
		sar    $0x2,%ecx
		rep movsl %ds:(%esi),%es:(%edi)
		jmp    jump_ed95f
	jump_ed8f9:
		mov    lbDisplay__GraphicsScreenWidth,%eax
		mov    lbDisplay__GraphicsScreenHeight,%ebx
		imul   %ebx,%eax
		mov    %eax,-0x10(%ebp)
		mov    lbDisplay__WScreen,%eax
		mov    %eax,-0x8(%ebp)
		xor    %edx,%edx
		mov    %edx,-0x4(%ebp)
		jmp    jump_ed94b
	jump_ed919:
		mov    $0x10000,%eax
	jump_ed91e:
		mov    %eax,-0xc(%ebp)
		mov    -0xc(%ebp),%eax
		sub    %eax,-0x10(%ebp)
		mov    -0x4(%ebp),%eax
		incl   -0x4(%ebp)
		cwtl
		call   LbVesaSetPage_
		mov    -0xc(%ebp),%ecx
		sar    $0x2,%ecx
		mov    -0x8(%ebp),%esi
		mov    lbDisplay__PhysicalScreen,%edi
		rep movsl %ds:(%esi),%es:(%edi)
		addl   $0x10000,-0x8(%ebp)
	jump_ed94b:
		cmpl   $0x0,-0x10(%ebp)
		je     jump_ed95f
		cmpl   $0x10000,-0x10(%ebp)
		jge    jump_ed919
		mov    -0x10(%ebp),%eax
		jmp    jump_ed91e
	jump_ed95f:
		call   LbMouseRemove_
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* long LbScreenSwapClear(unsigned char colour)
 */
GLOBAL_FUNC (ASM_LbScreenSwapClear)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x10,%esp
		call   LbMousePlace_
		cmpb   $0x0,lbDisplay__VesaIsSetUp
		jne    jump_edba6
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		shl    $0x8,%eax
		xor    %edx,%edx
		mov    0x18(%ebp),%dl
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		shl    $0x10,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		mov    lbDisplay__GraphicsScreenWidth,%ecx
		imul   lbDisplay__GraphicsScreenHeight,%ecx
		mov    lbDisplay__WScreen,%esi
		shl    $0x18,%eax
		mov    lbDisplay__PhysicalScreen,%edi
		add    %eax,%edx
		sar    $0x2,%ecx
	jump_edb9b:
		movsl  %ds:(%esi),%es:(%edi)
		mov    %edx,-0x4(%esi)
		loop   jump_edb9b
		jmp    jump_edc33
	jump_edba6:
		mov    lbDisplay__GraphicsScreenWidth,%eax
		mov    lbDisplay__GraphicsScreenHeight,%ecx
		imul   %ecx,%eax
		mov    %eax,-0x10(%ebp)
		mov    lbDisplay__WScreen,%eax
		mov    %eax,-0x8(%ebp)
		xor    %edx,%edx
		mov    %edx,-0x4(%ebp)
		jmp    jump_edc1f
	jump_edbc6:
		mov    $0x10000,%eax
	jump_edbcb:
		mov    %eax,-0xc(%ebp)
		mov    -0x4(%ebp),%eax
		incl   -0x4(%ebp)
		cwtl
		call   LbVesaSetPage_
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		shl    $0x8,%eax
		xor    %edx,%edx
		mov    0x18(%ebp),%dl
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		shl    $0x10,%eax
		add    %eax,%edx
		xor    %eax,%eax
		mov    0x18(%ebp),%al
		shl    $0x18,%eax
		add    %eax,%edx
		mov    -0xc(%ebp),%ecx
		sar    $0x2,%ecx
		mov    -0x8(%ebp),%esi
		mov    lbDisplay__PhysicalScreen,%edi
	jump_edc0c:
		movsl  %ds:(%esi),%es:(%edi)
		mov    %edx,-0x4(%esi)
		loop   jump_edc0c
		mov    -0xc(%ebp),%eax
		sub    %eax,-0x10(%ebp)
		addl   $0x10000,-0x8(%ebp)
	jump_edc1f:
		cmpl   $0x0,-0x10(%ebp)
		je     jump_edc33
		cmpl   $0x10000,-0x10(%ebp)
		jge    jump_edbc6
		mov    -0x10(%ebp),%eax
		jmp    jump_edbcb
	jump_edc33:
		call   LbMouseRemove_
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
/* long LbScreenWaitVbi(void)
 */
LbScreenWaitVbi_:
/*----------------------------------------------------------------*/
		push   %edx
		push   %ebp
		mov    %esp,%ebp
	jump_ee1c4:
		mov    $0x3da,%edx
		sub    %eax,%eax
		in     (%dx),%al
		test   $0x8,%al
		je     jump_ee1c4
		mov    $0x1,%eax
		pop    %ebp
		pop    %edx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbScreenSetup_)
/*----------------------------------------------------------------*/
		push   %ebp
		mov    %esp,%ebp
		sub    $0x10,%esp
		mov    %al,-0x4(%ebp)
		mov    %edx,-0x10(%ebp)
		mov    %ebx,-0xc(%ebp)
		mov    %ecx,-0x8(%ebp)
		mov    -0x8(%ebp),%eax
		push   %eax
		mov    -0xc(%ebp),%eax
		push   %eax
		mov    -0x10(%ebp),%eax
		push   %eax
		movsbl -0x4(%ebp),%eax
		push   %eax
		call   ac_LbScreenSetupAnyMode
		add    $0x10,%esp
		mov    %ebp,%esp
		pop    %ebp
		ret


/*----------------------------------------------------------------*/
LbScreenIsModeAvailable_:
/*----------------------------------------------------------------*/
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x4,%esp
		mov    %al,-0x4(%ebp)
		cmpl   $0x0,data_1e2e50
		jne    jump_e0ce2
		movl   $0x1,data_1e2e50
		call   LbScreenFindVideoModes_
	jump_e0ce2:
		movsbl -0x4(%ebp),%edx
		lea    0x0(,%edx,4),%eax
		add    %edx,%eax
		shl    $0x2,%eax
		sub    %edx,%eax
		mov    lbScreenModeInfo0_Field6(,%eax,2),%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbScreenSetGraphicsWindow_)	/* e0d00 */
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x20,%esp
		mov    %eax,-0x20(%ebp)
		mov    %edx,-0x1c(%ebp)
		mov    %ebx,-0x18(%ebp)
		mov    %ecx,-0x14(%ebp)
		mov    -0x20(%ebp),%eax
		mov    %eax,-0x10(%ebp)
		mov    -0x1c(%ebp),%eax
		mov    %eax,-0xc(%ebp)
		mov    -0x20(%ebp),%eax
		mov    -0x18(%ebp),%edx
		add    %edx,%eax
		mov    %eax,-0x8(%ebp)
		mov    -0x1c(%ebp),%eax
		mov    -0x14(%ebp),%ebx
		add    %ebx,%eax
		mov    %eax,-0x4(%ebp)
		mov    -0x8(%ebp),%eax
		cmp    -0x10(%ebp),%eax
		jge    jump_e0d50
		mov    -0x10(%ebp),%eax
		xor    %eax,-0x8(%ebp)
		mov    -0x8(%ebp),%eax
		xor    %eax,-0x10(%ebp)
		mov    -0x10(%ebp),%eax
		xor    %eax,-0x8(%ebp)
	jump_e0d50:
		mov    -0x4(%ebp),%eax
		cmp    -0xc(%ebp),%eax
		jge    jump_e0d6a
		mov    -0xc(%ebp),%eax
		xor    %eax,-0x4(%ebp)
		mov    -0x4(%ebp),%eax
		xor    %eax,-0xc(%ebp)
		mov    -0xc(%ebp),%eax
		xor    %eax,-0x4(%ebp)
	jump_e0d6a:
		cmpl   $0x0,-0x10(%ebp)
		jge    jump_e0d75
		xor    %edx,%edx
		mov    %edx,-0x10(%ebp)
	jump_e0d75:
		cmpl   $0x0,-0x8(%ebp)
		jge    jump_e0d80
		xor    %ecx,%ecx
		mov    %ecx,-0x8(%ebp)
	jump_e0d80:
		cmpl   $0x0,-0xc(%ebp)
		jge    jump_e0d8b
		xor    %edi,%edi
		mov    %edi,-0xc(%ebp)
	jump_e0d8b:
		cmpl   $0x0,-0x4(%ebp)
		jge    jump_e0d96
		xor    %edx,%edx
		mov    %edx,-0x4(%ebp)
	jump_e0d96:
		mov    -0x10(%ebp),%eax
		mov    lbDisplay__GraphicsScreenWidth,%ebx
		cmp    %ebx,%eax
		jle    jump_e0da6
		mov    %ebx,-0x10(%ebp)
	jump_e0da6:
		mov    -0x8(%ebp),%eax
		mov    lbDisplay__GraphicsScreenWidth,%ecx
		cmp    %ecx,%eax
		jle    jump_e0db6
		mov    %ecx,-0x8(%ebp)
	jump_e0db6:
		mov    -0xc(%ebp),%eax
		mov    lbDisplay__GraphicsScreenHeight,%esi
		cmp    %esi,%eax
		jle    jump_e0dc6
		mov    %esi,-0xc(%ebp)
	jump_e0dc6:
		mov    -0x4(%ebp),%eax
		mov    lbDisplay__GraphicsScreenHeight,%edi
		cmp    %edi,%eax
		jle    jump_e0dd6
		mov    %edi,-0x4(%ebp)
	jump_e0dd6:
		mov    -0x10(%ebp),%eax
		mov    %eax,lbDisplay__GraphicsWindowX
		mov    -0xc(%ebp),%eax
		mov    %eax,lbDisplay__GraphicsWindowY
		mov    -0x8(%ebp),%eax
		mov    -0x10(%ebp),%edx
		sub    %edx,%eax
		mov    %eax,lbDisplay__GraphicsWindowWidth
		mov    -0x4(%ebp),%eax
		mov    -0xc(%ebp),%ebx
		sub    %ebx,%eax
		mov    %eax,lbDisplay__GraphicsWindowHeight
		mov    lbDisplay__GraphicsScreenWidth,%eax
		imul   -0x1c(%ebp),%eax
		mov    lbDisplay__WScreen,%edx
		add    %edx,%eax
		mov    -0x20(%ebp),%edx
		add    %eax,%edx
		mov    %edx,lbDisplay__GraphicsWindowPtr
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
LbVesaGetGran_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x7c,%esp
		mov    %eax,-0x4(%ebp)
		mov    $0x32,%ebx
		lea    -0x7c(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    _lbVesaData,%eax
		shr    $0x4,%eax
		mov    %ax,-0x5a(%ebp)
		mov    %ax,-0x58(%ebp)
		xor    %edx,%edx
		mov    %edx,-0x7c(%ebp)
		movl   $0x4f01,-0x60(%ebp)
		movswl -0x4(%ebp),%eax
		mov    %eax,-0x64(%ebp)
		mov    $0x1c,%ebx
		lea    -0x2c(%ebp),%eax
		call   ac_memset
		mov    $0x1c,%ebx
		lea    -0x48(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0xc,%ebx
		lea    -0x10(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		lea    -0x10(%ebp),%eax
		mov    $0x300,%ecx
		call   _segread
		mov    %ecx,-0x2c(%ebp)
		movl   $0x10,-0x28(%ebp)
		lea    -0x7c(%ebp),%eax
		mov    %eax,-0x18(%ebp)
		lea    -0x10(%ebp),%ecx
		lea    -0x48(%ebp),%ebx
		lea    -0x2c(%ebp),%edx
		mov    $0x31,%eax
		call   ac_dos_int386x
		mov    _lbVesaData,%eax
		xor    %edx,%edx
		mov    0x5(%eax),%dl
		mov    %edx,%ebx
		xor    %edx,%edx
		shl    $0x8,%ebx
		mov    0x4(%eax),%dl
		add    %edx,%ebx
		mov    %bx,_lbVesaGran
		xor    %edx,%edx
		mov    0x11(%eax),%dl
		mov    %edx,%ebx
		xor    %edx,%edx
		shl    $0x8,%ebx
		mov    0x10(%eax),%dl
		add    %edx,%ebx
		mov    %bx,_lbVesaBytesPerLine
		xor    %edx,%edx
		mov    0x13(%eax),%dl
		mov    %edx,%ebx
		xor    %edx,%edx
		shl    $0x8,%ebx
		mov    0x12(%eax),%dl
		add    %edx,%ebx
		mov    %bx,_lbVesaHRes
		xor    %edx,%edx
		mov    0x15(%eax),%dl
		shl    $0x8,%edx
		mov    0x14(%eax),%al
		xor    %ah,%ah
		add    %eax,%edx
		mov    %dx,_lbVesaVRes
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LbVesaSetMode_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x3c,%esp
		mov    %eax,-0x4(%ebp)
		mov    $0x1c,%ebx
		lea    -0x3c(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0x1c,%ebx
		lea    -0x20(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		movb   $0x4f,-0x3b(%ebp)
		movb   $0x2,-0x3c(%ebp)
		mov    -0x4(%ebp),%eax
		mov    %ax,-0x38(%ebp)
		lea    -0x20(%ebp),%ebx
		lea    -0x3c(%ebp),%edx
		mov    $0x10,%eax
		call   ac_dos_int386
		movswl -0x4(%ebp),%eax
		call   LbVesaGetGran_
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* long LbVesaSetPage(long dxp)
 */
GLOBAL_FUNC (LbVesaSetPage_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0x4,%esp
		mov    %eax,-0x4(%ebp)
		mov    _lbVesaPage,%ax
		cmp    -0x4(%ebp),%ax
		je     jump_eee78
		movswl -0x4(%ebp),%edx
		mov    _lbVesaPage,%ebx
		shl    $0x6,%edx
		sar    $0x10,%ebx
		mov    %edx,%eax
		sar    $0x1f,%edx
		idiv   %ebx
		xor    %ecx,%ecx
		mov    %eax,%edx
		xor    %ebx,%ebx
		mov    $0x4f05,%eax
		push   %bp
		int    $0x10
		pop    %bp
		movswl -0x4(%ebp),%edx
		mov    _lbVesaPage,%ebx
		shl    $0x6,%edx
		sar    $0x10,%ebx
		mov    %edx,%eax
		sar    $0x1f,%edx
		idiv   %ebx
		xor    %ecx,%ecx
		mov    $0x1,%ebx
		mov    %eax,%edx
		mov    $0x4f05,%eax
		push   %bp
		int    $0x10
		pop    %bp
		mov    -0x4(%ebp),%eax
		mov    %ax,_lbVesaPage
	jump_eee78:
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LbVesaGetInfo_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x78,%esp
		mov    $0x32,%ebx
		lea    -0x78(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    _lbVesaData,%eax
		shr    $0x4,%eax
		mov    %ax,-0x56(%ebp)
		mov    %ax,-0x54(%ebp)
		xor    %edx,%edx
		mov    %edx,-0x78(%ebp)
		movl   $0x4f00,-0x5c(%ebp)
		movl   $0x101,-0x68(%ebp)
		mov    $0x1c,%ebx
		lea    -0x28(%ebp),%eax
		call   ac_memset
		mov    $0x1c,%ebx
		lea    -0x44(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0xc,%ebx
		lea    -0xc(%ebp),%eax
		xor    %edx,%edx
		call   ac_memset
		lea    -0xc(%ebp),%eax
		mov    $0x300,%esi
		call   _segread
		mov    %esi,-0x28(%ebp)
		movl   $0x10,-0x24(%ebp)
		lea    -0x78(%ebp),%eax
		mov    %eax,-0x14(%ebp)
		lea    -0xc(%ebp),%ecx
		lea    -0x44(%ebp),%ebx
		lea    -0x28(%ebp),%edx
		mov    $0x31,%eax
		call   ac_dos_int386x
		mov    $0x4,%ebx
		mov    $data_15f1a8,%edx
		mov    _lbVesaData,%eax
		call   ac_strncmp
		test   %eax,%eax
		jne    jump_eef3a
		mov    $0x1,%eax
		jmp    jump_eef3f
	jump_eef3a:
		mov    $0xffffffff,%eax
	jump_eef3f:
		mov    %ebp,%esp
		pop    %ebp
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LbVesaIsModeAvailable_:
/*----------------------------------------------------------------*/
		push   %edx
		push   %ebp
		mov    %esp,%ebp
		sub    $0xc,%esp
		mov    %eax,-0x4(%ebp)
		call   LbVesaGetInfo_
		cmp    $0x1,%eax
		jne    jump_eefc3
		mov    _lbVesaData,%eax
		add    $0xe,%eax
		mov    %eax,-0xc(%ebp)
		xor    %eax,%eax
		mov    -0xc(%ebp),%edx
		mov    (%edx),%ax
		mov    %eax,-0x8(%ebp)
		mov    -0xc(%ebp),%eax
		addl   $0x2,-0xc(%ebp)
		mov    -0xc(%ebp),%eax
		mov    (%eax),%ax
		and    $0xffff,%eax
		shl    $0x4,%eax
		add    %eax,-0x8(%ebp)
		mov    -0x8(%ebp),%eax
		mov    %eax,-0xc(%ebp)
		jmp    jump_eef99
	jump_eef92:
		mov    -0xc(%ebp),%eax
		addl   $0x2,-0xc(%ebp)
	jump_eef99:
		mov    -0xc(%ebp),%eax
		mov    (%eax),%ax
		and    $0xffff,%eax
		cmp    $0xffff,%eax
		je     jump_eefc3
		mov    -0xc(%ebp),%eax
		mov    (%eax),%ax
		mov    -0x4(%ebp),%edx
		cmp    %dx,%ax
		jne    jump_eef92
		mov    $0x1,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		ret
	jump_eefc3:
		xor    %eax,%eax
		mov    %ebp,%esp
		pop    %ebp
		pop    %edx
		ret


.section .rodata

/*
 * Internal graphic mode list
 */
GLOBAL (lbScreenModeInfo)
lbScreenModeInfo0_Width:
		.short  0x0
lbScreenModeInfo0_Height:
		.short  0x0
lbScreenModeInfo0_BitsPerPixel:
		.short  0x0
lbScreenModeInfo0_Field6:
		.short  0x0
lbScreenModeInfo0_Field8:
		.short  0x0
lbScreenModeInfo0_VideoFlags:
		.long	0x0
lbScreenModeInfo0_Desc:
		.string "MODE_INVALID     "
		.fill   0x6

lbScreenModeInfo1_Width:
		.short  0x140
lbScreenModeInfo1_Height:
		.short  0x0c8
lbScreenModeInfo1_BitsPerPixel:
		.short  0x08
lbScreenModeInfo1_Field6:
		.short  0x0
		.short  0x0
lbScreenModeInfo1_VideoFlags:
		.ascii  "\x13\x00\x00\x00"
lbScreenModeInfo1_Desc:
		.string "MODE_320_200_8\t  "
		.fill   0x6

lbScreenModeInfo2_Width:
		.short  0x140
lbScreenModeInfo2_Height:
		.short  0x0c8
lbScreenModeInfo2_BitsPerPixel:
		.short  0x10
		.fill   0x4
lbScreenModeInfo2_VideoFlags:
		.long	0x0
lbScreenModeInfo2_Desc:
		.string "MODE_320_200_16  "
		.fill   0x6

lbScreenModeInfo3_Width:
		.short  0x140
lbScreenModeInfo3_Height:
		.short  0x0c8
lbScreenModeInfo3_BitsPerPixel:
		.short  0x18
		.fill   0x8
lbScreenModeInfo3_Desc:
		.string "MODE_320_200_24  "
		.fill   0x6

lbScreenModeInfo4_Width:
		.short  0x140
		.ascii  "\xf0\x00\x08"
		.fill   0x9
lbScreenModeInfo4_Desc:
		.string "MODE_320_240_8\t  "
		.fill   0x6

lbScreenModeInfo5_Width:
		.short  0x140
		.ascii  "\xf0\x00\x10"
		.fill   0x9
lbScreenModeInfo5_Desc:
		.string "MODE_320_240_16  "
		.fill   0x6

lbScreenModeInfo6_Width:
		.short  0x140
		.ascii  "\xf0\x00\x18"
		.fill   0x9
lbScreenModeInfo6_Desc:
		.string "MODE_320_240_24  "
		.fill   0x6

lbScreenModeInfo7_Width:
		.short  0x200
		.ascii  "\x80\x01\x08"
		.fill   0x9
lbScreenModeInfo7_Desc:
		.string "MODE_512_384_8\t  "
		.fill   0x6

lbScreenModeInfo8_Width:
		.short  0x200
		.ascii  "\x80\x01\x10"
		.fill   0x9
lbScreenModeInfo8_Desc:
		.string "MODE_512_384_16  "
		.fill   0x7
		.ascii  "\x02\x80\x01\x18"
		.fill   0x9
lbScreenModeInfo9_Desc:
		.string "MODE_512_384_24  "
		.fill   0x6
		.ascii  "\x80\x02\x90\x01\x08"
		.fill   0x6
		.ascii  "\x01\x00\x00"
lbScreenModeInfo10_Desc:
		.string "MODE_640_400_8\t  "
		.fill   0x6
		.ascii  "\x80\x02\x90\x01\x10"
		.fill   0x9
lbScreenModeInfo11_Desc:
		.string "MODE_640_400_16  "
		.fill   0x6
		.ascii  "\x80\x02\x90\x01\x18"
		.fill   0x9
lbScreenModeInfo12_Desc:
		.string "MODE_640_400_24  "
		.fill   0x6
		.ascii  "\x80\x02\xe0\x01\x08"
		.fill   0x5
		.ascii  "\x01\x01\x00\x00"
lbScreenModeInfo13_Desc:
		.string "MODE_640_480_8\t  "
		.fill   0x6
		.ascii  "\x80\x02\xe0\x01\x10"
		.fill   0x9
lbScreenModeInfo14_Desc:
		.string "MODE_640_480_16  "
		.fill   0x6
		.ascii  "\x80\x02\xe0\x01\x18"
		.fill   0x9
lbScreenModeInfo15_Desc:
		.string "MODE_640_480_24  "
		.fill   0x6
		.ascii  "\x20\x03\x58\x02\x08"
		.fill   0x5
		.ascii  "\x03\x01\x00\x00"
lbScreenModeInfo16_Desc:
		.string "MODE_800_600_8\t  "
		.fill   0x6
		.ascii  "\x20\x03\x58\x02\x10"
		.fill   0x9
lbScreenModeInfo17_Desc:
		.string "MODE_800_600_16  "
		.fill   0x6
		.ascii  "\x20\x03\x58\x02\x18"
		.fill   0x9
lbScreenModeInfo18_Desc:
		.string "MODE_800_600_24  "
		.fill   0x7
		.ascii  "\x04\x00\x03\x08"
		.fill   0x5
		.ascii  "\x05\x01\x00\x00"
lbScreenModeInfo19_Desc:
		.string "MODE_1024_768_8  "
		.fill   0x7
		.ascii  "\x04\x00\x03\x10"
		.fill   0x9
lbScreenModeInfo20_Desc:
		.string "MODE_1024_768_16 "
		.fill   0x7
		.ascii  "\x04\x00\x03\x18"
		.fill   0x9
lbScreenModeInfo21_Desc:
		.string "MODE_1024_768_24 "
		.fill   0x6
		.ascii  "\xb0\x04\x00\x04\x08"
		.fill   0x5
		.ascii  "\x07\x01\x00\x00"
lbScreenModeInfo22_Desc:
		.string "MODE_1280_1024_8 "
		.fill   0x6
		.ascii  "\xb0\x04\x00\x04\x10"
		.fill   0x9
lbScreenModeInfo23_Desc:
		.string "MODE_1280_1024_16"
		.fill   0x6
		.ascii  "\xb0\x04\x00\x04\x18"
		.fill   0x9
lbScreenModeInfo24_Desc:
		.string "MODE_1280_1024_24"
		.fill   0x6
		.ascii  "\x40\x06\xb0\x04\x08"
		.fill   0x9
lbScreenModeInfo25_Desc:
		.string "MODE_1600_1200_8 "
		.fill   0x6
		.ascii  "\x40\x06\xb0\x04\x10"
		.fill   0x9
lbScreenModeInfo26_Desc:
		.string "MODE_1600_1200_16"
		.fill   0x6
		.ascii  "\x40\x06\xb0\x04\x18"
		.fill   0x9
lbScreenModeInfo27_Desc:
		.string "MODE_1600_1200_24"
		.fill   0x14
lbScreenModeInfo28_Desc:
		.string "MODE_INVALID     "
		.fill   0x6
		.align	4

data_15f1a8:
		.string "VESA"
		.align	4

.data

/*
 * current graphic mode information set in LbScreenSetupAnyMode
 */
GLOBAL (lbDisplay)
/* pointer to the frame buffer */
GLOBAL (lbDisplay__PhysicalScreen)
		.long	0x0
GLOBAL (lbDisplay__WScreen)			/* 1e2e58 */
		.long	0x0
GLOBAL (lbDisplay__GlassMap)
		.long	0x0
lbDisplay__FadeTable:
		.long	0x0
GLOBAL (lbDisplay__GraphicsWindowPtr)
		.long	0x0
GLOBAL (lbDisplay__MouseSprite)
		.long	0x0
GLOBAL (lbDisplay__PhysicalScreenWidth)		/* 1e2e6c */
		.long	0x0
GLOBAL (lbDisplay__PhysicalScreenHeight)		/* 1e2e70 */
		.long	0x0
GLOBAL (lbDisplay__GraphicsScreenWidth)			/* 1e2e74 */
		.long	0x0
GLOBAL (lbDisplay__GraphicsScreenHeight)			/* 1e2e78 */
		.long	0x0
GLOBAL (lbDisplay__GraphicsWindowX)
		.long	0x0
GLOBAL (lbDisplay__GraphicsWindowY)
		.long	0x0
GLOBAL (lbDisplay__GraphicsWindowWidth)
		.long	0x0
GLOBAL (lbDisplay__GraphicsWindowHeight)
		.long	0x0
GLOBAL (lbDisplay__MouseWindowX)			/* 1e2e8c */
		.long	0x0
GLOBAL (lbDisplay__MouseWindowY)			/* 1e2e90 */
		.long	0x0
GLOBAL (lbDisplay__MouseWindowWidth)		/* 1e2e94 */
		.long	0x0
GLOBAL (lbDisplay__MouseWindowHeight)		/* 1e2e98 */
		.long	0x0
GLOBAL (lbDisplay__MouseX)			/* 1e2e9c */
		.long	0x0
GLOBAL (lbDisplay__MouseY)			/* 1e2ea0 */
		.long	0x0
GLOBAL (lbDisplay__MMouseX)			/* 1e2ea4 */
		.long	0x0
GLOBAL (lbDisplay__MMouseY)			/* 1e2ea8 */
		.long	0x0
GLOBAL (lbDisplay__RMouseX)		/* 1e2eac */
		.long	0x0
GLOBAL (lbDisplay__RMouseY)		/* 1e2eb0 */
		.long	0x0
GLOBAL (lbDisplay__DrawFlags) /* no idea, but it's a word, even if here it says otherwise */
		.byte	0x0
GLOBAL (lbDisplay__DrawFlagsB1)
		.byte	0x0
/* graphic mode at the start of the app (at exit the app goes back to this mode) */
lbDisplay__OldVideoMode:		/* 1e2eb6 */
		.short	0x0
/* index in the mode array */
GLOBAL (lbDisplay__ScreenMode)			/* 1e2eb8 */
		.short	0x0
/* bool */
GLOBAL (lbDisplay__VesaIsSetUp)		/* 1e2eba */
		.byte	0x0
GLOBAL (lbDisplay__LeftButton)	/* 1e2ebb */
		.byte	0x0
GLOBAL (lbDisplay__RightButton)	/* 1e2ebc */
		.byte	0x0
GLOBAL (lbDisplay__MiddleButton)	/* 1e2ebd */
		.byte	0x0
GLOBAL (lbDisplay__MLeftButton)		/* 1e2ebe */
		.byte	0x0
GLOBAL (lbDisplay__MRightButton)		/* 1e2ebf */
		.byte	0x0
GLOBAL (lbDisplay__MMiddleButton)		/* 1e2ec0 */
		.byte	0x0
GLOBAL (lbDisplay__RLeftButton)	/* 1e2ec1 */
		.byte	0x0
GLOBAL (lbDisplay__RMiddleButton)	/* 1e2ec2 */
		.byte	0x0
GLOBAL (lbDisplay__RRightButton)	/* 1e2ec3 */
		.byte	0x0
lbDisplay__FadeStep:
		.byte	0x0
GLOBAL (lbDisplay__DrawColour)
		.byte	0x0
lbDisplay__Palette:
		.long	0x0
		.fill   0x2

data_1e2e50:
		.long	0x0

GLOBAL (screen_initialised)		/* 1e5e98 */
		.long	0x0
_lbVesaBytesPerLine:
		.short  0x0
_lbVesaHRes:
		.short  0x0
_lbVesaVRes:
		.long	0x0
_lbVesaData:
		.long	0x0
GLOBAL (_lbVesaPage)
		.short  0x0
_lbVesaGran:
		.short  0x0

GLOBAL (lbScreenDirectAccessActive)
		.long	0x0

