
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

.global EXPORT_SYMBOL(__Extender);
.global EXPORT_SYMBOL(__osmajor);
.global EXPORT_SYMBOL(data_159581);
.global EXPORT_SYMBOL(data_1595e5);
.global EXPORT_SYMBOL(data_159590);
.global EXPORT_SYMBOL(data_1595e4);
.global EXPORT_SYMBOL(__STACKLOW);
.global EXPORT_SYMBOL(data_1e9560);
.global EXPORT_SYMBOL(data_1ea484);
.global EXPORT_SYMBOL(data_1ea480);

/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileExists_)
/*----------------------------------------------------------------*/
        push   %edx
        push   %ebp
        mov    %esp,%ebp
        sub    $0x4,%esp
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%eax
        xor    %edx,%edx
        call   ac_dos_access
        test   %eax,%eax
        sete   %al
        and    $0xff,%eax
        mov    %ebp,%esp
        pop    %ebp
        pop    %edx
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFilePosition_)
/*----------------------------------------------------------------*/
        push   %edx
        push   %ebp
        mov    %esp,%ebp
        sub    $0x8,%esp
        mov    %eax,-0x8(%ebp)
        mov    -0x8(%ebp),%eax
        call   _tell
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%edx
        cmp    $0xffffffff,%edx
        jne    jump_e8938
        mov    %edx,%eax
        mov    %ebp,%esp
        pop    %ebp
        pop    %edx
        ret
    jump_e8938:
        mov    -0x4(%ebp),%eax
        mov    %ebp,%esp
        pop    %ebp
        pop    %edx
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileClose_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0x4,%esp
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%eax
        call   ac_close
        test   %eax,%eax
        jne    jump_e8a3a
        mov    $0x1,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8a3a:
        mov    $0xffffffff,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileSeek_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0x10,%esp
        mov    %eax,-0x10(%ebp)
        mov    %edx,-0xc(%ebp)
        mov    %bl,-0x4(%ebp)
        movl   $0xffffffff,-0x8(%ebp)
        mov    -0x4(%ebp),%al
        cmp    $0x1,%al
        jb     jump_e8aa5
        jbe    jump_e8a7b
        cmp    $0x2,%al
        je     jump_e8a90
        jmp    jump_e8ab0
    jump_e8a69:
        mov    -0xc(%ebp),%edx
        mov    -0x10(%ebp),%eax
        xor    %ebx,%ebx
        call   ac_dos_lseek
        mov    %eax,-0x8(%ebp)
        jmp    jump_e8ab0
    jump_e8a7b:
        mov    -0xc(%ebp),%edx
        mov    -0x10(%ebp),%eax
        mov    $0x1,%ebx
        call   ac_dos_lseek
        mov    %eax,-0x8(%ebp)
        jmp    jump_e8ab0
    jump_e8a90:
        mov    -0xc(%ebp),%edx
        mov    -0x10(%ebp),%eax
        mov    $0x2,%ebx
        call   ac_dos_lseek
        mov    %eax,-0x8(%ebp)
        jmp    jump_e8ab0
    jump_e8aa5:
        test   %al,%al
        je     jump_e8a69
        mov    -0x8(%ebp),%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8ab0:
        mov    -0x8(%ebp),%eax
        mov    %ebp,%esp
        pop    %ebp
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileRead_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0x10,%esp
        mov    %eax,-0x10(%ebp)
        mov    %edx,-0xc(%ebp)
        mov    %ebx,-0x8(%ebp)
        mov    -0x8(%ebp),%ebx
        mov    -0xc(%ebp),%edx
        mov    -0x10(%ebp),%eax
        call   ac_read
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%edx
        cmp    $0xffffffff,%edx
        jne    jump_e8ae6
        mov    %edx,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8ae6:
        mov    -0x4(%ebp),%eax
        mov    %ebp,%esp
        pop    %ebp
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileWrite_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0x10,%esp
        mov    %eax,-0x10(%ebp)
        mov    %edx,-0xc(%ebp)
        mov    %ebx,-0x8(%ebp)
        mov    -0x8(%ebp),%ebx
        mov    -0xc(%ebp),%edx
        mov    -0x10(%ebp),%eax
        call   ac_write
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%edx
        cmp    $0xffffffff,%edx
        jne    jump_e8b1e
        mov    %edx,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8b1e:
        mov    -0x4(%ebp),%eax
        mov    %ebp,%esp
        pop    %ebp
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileLength_)
/*----------------------------------------------------------------*/
        push   %ebx
        push   %edx
        push   %ebp
        mov    %esp,%ebp
        sub    $0xc,%esp
        mov    %eax,-0xc(%ebp)
        mov    -0xc(%ebp),%eax
        mov    $0x2,%edx
        call   FileOpenInclCD
        mov    %eax,-0x8(%ebp)
        mov    -0x8(%ebp),%edx
        cmp    $0xffffffff,%edx
        jne    jump_e8b4f
        mov    %edx,%eax
        jmp    jump_e8b75
    jump_e8b4f:
        mov    -0x8(%ebp),%eax
        call   _filelength
        mov    %eax,-0x4(%ebp)
        mov    -0x8(%ebp),%eax
        call   LbFileClose_
        mov    -0x4(%ebp),%ebx
        cmp    $0xffffffff,%ebx
        jne    jump_e8b72
        mov    %ebx,%eax
        mov    %ebp,%esp
        pop    %ebp
        pop    %edx
        pop    %ebx
        ret
    jump_e8b72:
        mov    -0x4(%ebp),%eax
    jump_e8b75:
        mov    %ebp,%esp
        pop    %ebp
        pop    %edx
        pop    %ebx
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileFindFirst_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0xc,%esp
        mov    %eax,-0xc(%ebp)
        mov    %edx,-0x8(%ebp)
        mov    %ebx,-0x4(%ebp)
        mov    -0x8(%ebp),%ebx
        add    $0xb8,%ebx
        mov    -0x4(%ebp),%edx
        mov    -0xc(%ebp),%eax
        call   ___dos_findfirst_
        test   %eax,%eax
        je     jump_e8bac
        mov    $0xffffffff,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8bac:
        mov    -0x8(%ebp),%eax
        call   func_e8c60
        mov    $0x1,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LbFileFindNext_)
/*----------------------------------------------------------------*/
        push   %ebp
        mov    %esp,%ebp
        sub    $0x4,%esp
        mov    %eax,-0x4(%ebp)
        mov    -0x4(%ebp),%eax
        add    $0xb8,%eax
        call   ___dos_findnext_
        test   %eax,%eax
        je     jump_e8be3
        mov    $0xffffffff,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret
    jump_e8be3:
        mov    -0x4(%ebp),%eax
        call   func_e8c60
        mov    $0x1,%eax
        mov    %ebp,%esp
        pop    %ebp
        ret

/*----------------------------------------------------------------*/
GLOBAL_FUNC (FileOpenInclCD)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		sub    $0x64,%esp
		mov    %dl,%bl
		mov    %esp,%edx
		call   ac_game_transform_path
		test   %bl,%bl
		jne    jump_dfa15
		mov    $0x1c0,%edx
		mov    %esp,%eax
		call   _creat
		mov    $0x200,%edx
		mov    %eax,%ecx
		call   ac_dos_setmode
		mov    %ecx,%eax
		call   ac_close
	jump_dfa15:
		mov    $0xffffffff,%eax
		cmp    $0x1,%bl
		jb     jump_dfa68
		jbe    jump_dfa52
		cmp    $0x2,%bl
		jne    jump_dfa6c
		push   $0x40
		push   $0x200
		lea    0x8(%esp),%eax
		push   %eax
		call   ac_dos_sopen
		add    $0xc,%esp
		jmp    jump_dfa6c
	jump_dfa3c:
		push   $0x40
		push   $0x222
		lea    0x8(%esp),%eax
		push   %eax
		call   ac_dos_sopen
		add    $0xc,%esp
		jmp    jump_dfa6c
	jump_dfa52:
		push   $0x40
		push   $0x202
		lea    0x8(%esp),%eax
		push   %eax
		call   ac_dos_sopen
		add    $0xc,%esp
		jmp    jump_dfa6c
	jump_dfa68:
		test   %bl,%bl
		je     jump_dfa3c
	jump_dfa6c:
		cmp    $0xffffffff,%eax
		add    $0x64,%esp
		pop    %ecx
		pop    %ebx
		ret

/*----------------------------------------------------------------*/
func_e8c60:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		mov    %esp,%ebp
		sub    $0x4,%esp
		mov    %eax,-0x4(%ebp)
		mov    -0x4(%ebp),%esi
		add    $0xd6,%esi
		mov    -0x4(%ebp),%edi
		push   %edi
	jump_e8c78:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_e8c90
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_e8c78
	jump_e8c90:
		pop    %edi
		mov    -0x4(%ebp),%esi
		add    $0xd6,%esi
		mov    -0x4(%ebp),%edi
		add    $0x90,%edi
		push   %edi
	jump_e8ca4:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_e8cbc
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_e8ca4
	jump_e8cbc:
		pop    %edi
		mov    -0x4(%ebp),%esi
		mov    0xd2(%esi),%edi
		mov    -0x4(%ebp),%esi
		mov    %edi,0xa2(%esi)
		mov    -0x4(%ebp),%esi
		mov    0xca(%esi),%esi
		sar    $0x18,%esi
		mov    -0x4(%ebp),%eax
		mov    %esi,0x9e(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xa6(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xa7(%eax)
		mov    -0x4(%ebp),%eax
		movw   $0x0,0xa8(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xaa(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xae(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xad(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xac(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xab(%eax)
		mov    -0x4(%ebp),%eax
		mov    0xd0(%eax),%al
		and    $0x1f,%al
		mov    -0x4(%ebp),%esi
		mov    %al,0xaf(%esi)
		mov    -0x4(%ebp),%eax
		xor    %esi,%esi
		mov    0xd0(%eax),%si
		mov    %esi,%eax
		and    $0x1e0,%eax
		shr    $0x5,%eax
		mov    -0x4(%ebp),%esi
		mov    %al,0xb0(%esi)
		mov    -0x4(%ebp),%esi
		xor    %eax,%eax
		mov    0xd0(%esi),%ax
		and    $0xfe00,%eax
		shr    $0x9,%eax
		add    $0x7bc,%eax
		mov    -0x4(%ebp),%esi
		mov    %ax,0xb1(%esi)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xb3(%eax)
		mov    -0x4(%ebp),%eax
		movb   $0x0,0xb7(%eax)
		mov    -0x4(%ebp),%eax
		mov    0xce(%eax),%al
		and    $0x1f,%al
		mov    -0x4(%ebp),%esi
		add    %al,%al
		mov    %al,0xb6(%esi)
		mov    -0x4(%ebp),%esi
		xor    %eax,%eax
		mov    0xce(%esi),%ax
		and    $0x7e0,%eax
		shr    $0x5,%eax
		mov    -0x4(%ebp),%esi
		mov    %al,0xb5(%esi)
		mov    -0x4(%ebp),%esi
		xor    %eax,%eax
		mov    0xce(%esi),%ax
		and    $0xf800,%eax
		shr    $0xb,%eax
		mov    -0x4(%ebp),%esi
		mov    %al,0xb4(%esi)
		mov    %ebp,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
_creat:	/* 0xf13ca */
/*----------------------------------------------------------------*/
		push   %edx
		push   $0x62
		push   %eax
		call   ac_dos_open
		add    $0xc,%esp
		ret


/*----------------------------------------------------------------*/
_tell:  /* 0xfafcf */
/*----------------------------------------------------------------*/
        push   %ebx
        push   %edx
        mov    $0x1,%ebx
        xor    %edx,%edx
        call   ac_dos_lseek
        pop    %edx
        pop    %ebx
        ret


/*----------------------------------------------------------------*/
_filelength:    /* 0xfb2f3 */
/*----------------------------------------------------------------*/
        push   %ebx
        push   %ecx
        push   %edx
        push   %esi
        push   %edi
        mov    %eax,%ecx
        mov    $0x1,%ebx
        xor    %edx,%edx
        call   ac_dos_lseek
        mov    %eax,%esi
        cmp    $0xffffffff,%eax
        je     jump_fb32a
        mov    $0x2,%ebx
        mov    %ecx,%eax
        xor    %edx,%edx
        call   ac_dos_lseek
        mov    %eax,%edi
        mov    %esi,%edx
        xor    %ebx,%ebx
        mov    %ecx,%eax
        call   ac_dos_lseek
        mov    %edi,%eax
    jump_fb32a:
        pop    %edi
        pop    %esi
        pop    %edx
        pop    %ecx
        pop    %ebx
        ret


/*----------------------------------------------------------------*/
___dos_findfirst_:  /* 0xfb330 */
/*----------------------------------------------------------------*/
        push   %ecx
        push   %eax
        push   %edx
        mov    %ebx,%edx
        mov    $0x1a,%ah
        int    $0x21
        pop    %ecx
        pop    %edx
        mov    $0x4e,%ah
        int    $0x21
        call   ____doserror_
        mov    %ebx,%edx
        call   func_fb36c
        pop    %ecx
        ret


/*----------------------------------------------------------------*/
___dos_findnext_:   /* 0xfb34d */
/*----------------------------------------------------------------*/
        push   %edx
        mov    %eax,%edx
        mov    $0x1a,%ah
        int    $0x21
        call   func_fb38f
        mov    $0x4f,%ah
        int    $0x21
        call   ____doserror_
        call   func_fb36c
        pop    %edx
        ret


/*----------------------------------------------------------------*/
func_fb36c:
/*----------------------------------------------------------------*/
		cmpb   $0x9,EXPORT_SYMBOL(__Extender)
		jne    jump_fb38e
		push   %eax
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		mov    $0x2f,%ah
		int    $0x21
		mov    %ebx,%esi
		mov    %edx,%edi
		mov    $0x2b,%ecx
		rep movsb %ds:(%esi),%es:(%edi)
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		pop    %eax
	jump_fb38e:
		ret


/*----------------------------------------------------------------*/
func_fb38f:
/*----------------------------------------------------------------*/
        cmpb   $0x9,EXPORT_SYMBOL(__Extender)
        jne    jump_fb3b1
        push   %eax
        push   %ebx
        push   %ecx
        push   %esi
        push   %edi
        mov    $0x2f,%ah
        int    $0x21
        mov    %ebx,%edi
        mov    %edx,%esi
        mov    $0x2b,%ecx
        rep movsb %ds:(%esi),%es:(%edi)
        pop    %edi
        pop    %esi
        pop    %ecx
        pop    %ebx
        pop    %eax
    jump_fb3b1:
        ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (__j_unlink_)
/*----------------------------------------------------------------*/
        jmp    ac_unlink


/*----------------------------------------------------------------*/
____set_errno_:	/* 0x108dec */
/*----------------------------------------------------------------*/
		push   %edx
		mov    %eax,%edx
		call   func_10af3f
		mov    %edx,(%eax)
		pop    %edx
		ret

/*----------------------------------------------------------------*/
GLOBAL_FUNC (____IOMode_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    %eax,%edx
		cmp    EXPORT_SYMBOL(data_159590),%eax
		jb     jump_10ad1f
		xor    %eax,%eax
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_10ad1f:
		cmp    $0x5,%eax
		jg     jump_10ad55
		mov    %eax,%ebx
		mov    EXPORT_SYMBOL(data_1595e4),%eax
		shl    $0x2,%ebx
		add    %ebx,%eax
		mov    0x1(%eax),%cl
		test   $0x40,%cl
		jne    jump_10ad55
		mov    %cl,%ch
		or     $0x40,%ch
		mov    %ch,0x1(%eax)
		mov    %edx,%eax
		call   ac_isatty
		test   %eax,%eax
		je     jump_10ad55
		mov    EXPORT_SYMBOL(data_1595e4),%eax
		orb    $0x20,0x1(%ebx,%eax,1)
	jump_10ad55:
		mov    EXPORT_SYMBOL(data_1595e4),%eax
		mov    (%eax,%edx,4),%eax
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
___dosret0_:	/* 0x10ad9d */
/*----------------------------------------------------------------*/
		test   %edx,%edx
		jne    ___dosretax_
		xor    %eax,%eax
	jump_10ada3:
		ret


/*----------------------------------------------------------------*/
___dosretax_:	/* 0x10ada4 */
/*----------------------------------------------------------------*/
		test   %edx,%edx
		je     jump_10ada3


/*----------------------------------------------------------------*/
____doserror_:	/* 0x10df92 */
/*----------------------------------------------------------------*/
		jae    jump_10dfa2
		and    $0xffff,%eax
		push   %eax
		call   ____set_errno_dos_
		pop    %eax
		jmp    jump_10dfa4
	jump_10dfa2:
		sub    %eax,%eax
	jump_10dfa4:
		ret


/*----------------------------------------------------------------*/
____set_doserrno_:	/* 0x108e0f */
/*----------------------------------------------------------------*/
		push   %edx
		mov    %eax,%edx
		call   func_10af45
		mov    %edx,(%eax)
		pop    %edx
		ret


/*----------------------------------------------------------------*/
____set_errno_dos_:	/* 0x10ada8 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		mov    %eax,%ebx
		mov    %al,%dl
		xor    %eax,%eax
		mov    %bl,%al
		call   ____set_doserrno_
		cmp    $0x100,%ebx
		jae    jump_10ae02
		xor    %eax,%eax
		mov    EXPORT_SYMBOL(__osmajor),%al
		cmp    $0x3,%eax
		jl     jump_10ade8
		xor    %ebx,%ebx
		mov    %dl,%bl
		cmp    $0x50,%ebx
		jne    jump_10add8
		mov    $0xe,%dl
		jmp    jump_10ade8
	jump_10add8:
		cmp    $0x22,%ebx
		jl     jump_10ade1
		mov    $0x13,%dl
		jmp    jump_10ade8
	jump_10ade1:
		cmp    $0x20,%ebx
		jl     jump_10ade8
		mov    $0x5,%dl
	jump_10ade8:
		xor    %eax,%eax
		mov    %dl,%al
		cmp    $0x13,%eax
		jle    jump_10adf3
		mov    $0x13,%dl
	jump_10adf3:
		xor    %ebx,%ebx
		mov    %dl,%bl
		mov    EXPORT_SYMBOL(data_1595e5)(%ebx),%eax
		sar    $0x18,%eax
		jmp    jump_10ae0c
	jump_10ae02:
		mov    %ebx,%eax
		shr    $0x8,%eax
		and    $0xff,%eax
	jump_10ae0c:
		call   ____set_errno_
		mov    $0xffffffff,%eax
		pop    %edx
		pop    %ebx
		ret

/*----------------------------------------------------------------*/
func_10df1f:
/*----------------------------------------------------------------*/
		mov    %esp,%eax
		sub    EXPORT_SYMBOL(__STACKLOW),%eax
		ret

/*----------------------------------------------------------------*/
func_10af45:
/*----------------------------------------------------------------*/
		mov    $EXPORT_SYMBOL(data_1ea484),%eax
		ret



/*----------------------------------------------------------------*/
func_10af3f:
/*----------------------------------------------------------------*/
		mov    $EXPORT_SYMBOL(data_1ea480),%eax
		ret


/*----------------------------------------------------------------*/
____qwrite_:	/* 0x118624 */
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x4,%esp
		mov    %eax,%esi
		mov    %edx,%ebp
		mov    %ebx,%edi
		call   ____IOMode_
		test   $0x80,%al
		je     jump_118669
		mov    $0x2,%al
		mov    %esi,%ebx
		xor    %edx,%edx
		mov    $0x42,%ah
		mov    %edx,%ecx
		shr    $0x10,%ecx
		int    $0x21
		rcl    %dx
		ror    %dx
		shl    $0x10,%edx
		mov    %ax,%dx
		mov    %edx,(%esp)
		test   %edx,%edx
		jge    jump_118669
	jump_11865c:
		xor    %eax,%eax
		mov    (%esp),%ax
		call   ____set_errno_dos_
		jmp    jump_118690
	jump_118669:
		mov    %edi,%ecx
		mov    %ebp,%edx
		mov    %esi,%ebx
		mov    $0x40,%ah
		int    $0x21
		rcl    %eax
		ror    %eax
		mov    %eax,(%esp)
		mov    %eax,%edx
		test   %eax,%eax
		jl     jump_11865c
		cmp    %edi,%eax
		je     jump_11868e
		mov    $0xc,%eax
		call   ____set_errno_
	jump_11868e:
		mov    %edx,%eax
	jump_118690:
		add    $0x4,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


