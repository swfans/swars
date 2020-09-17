
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

.global EXPORT_SYMBOL(game_things);
.global EXPORT_SYMBOL(data_1a09e0);


/* The debug function which uses this switch was not disassembled.
 * That's a shame. We should re-introduce it.
 */
format_debug_msg_switch_b8d10:
		.long   format_debug_msg_case_b9064
		.long   format_debug_msg_case_b91e9
		.long   format_debug_msg_case_b925e
		.long   format_debug_msg_case_b9299
		.long   format_debug_msg_case_b92db
		.long   format_debug_msg_case_b9444
		.long   format_debug_msg_case_b949d
		.long   format_debug_msg_case_b947f
		.long   format_debug_msg_case_b93ce
		.long   format_debug_msg_case_b9409
		.long   format_debug_msg_case_b92f9
		.long   format_debug_msg_case_b9352
		.long   format_debug_msg_case_b938d
		.long   format_debug_msg_case_b951b
		.long   format_debug_msg_case_b9621
		.long   format_debug_msg_case_b9679
		.long   format_debug_msg_case_b969e
		.long   format_debug_msg_case_b94c2
		.long   format_debug_msg_case_b94fd
		.long   format_debug_msg_case_b9533
		.long   format_debug_msg_case_b903d
		.long   format_debug_msg_case_b96f7
		.long   format_debug_msg_case_b9732
		.long   format_debug_msg_case_b9551
		.long   format_debug_msg_case_b95ab
		.long   format_debug_msg_case_b95c9
		.long   format_debug_msg_case_b96d2
		.long   format_debug_msg_case_b956f
		.long   format_debug_msg_case_b958d
		.long   format_debug_msg_case_b91a2
		.long   format_debug_msg_case_b90d4
		.long   format_debug_msg_case_b91d1
		.long   format_debug_msg_case_b9334
		.long   format_debug_msg_case_b90ae
		.long   format_debug_msg_case_b9100
		.long   format_debug_msg_case_b9173
		.long   format_debug_msg_case_b915b
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_b974a
		.long   format_debug_msg_case_b9885
		.long   format_debug_msg_case_b9919
		.long   format_debug_msg_case_b9785
		.long   format_debug_msg_case_b98aa
		.long   format_debug_msg_case_b9937
		.long   format_debug_msg_case_b97fa
		.long   format_debug_msg_case_b98cf
		.long   format_debug_msg_case_b9955
		.long   format_debug_msg_case_b984a
		.long   format_debug_msg_case_b98f4
		.long   format_debug_msg_case_b9973
		.long   format_debug_msg_case_b99af
		.long   format_debug_msg_case_b99cd
		.long   format_debug_msg_case_b9991
		.long   format_debug_msg_case_b99eb
		.long   format_debug_msg_case_b9a1e
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_b9a54
		.long   format_debug_msg_case_b9b55
		.long   format_debug_msg_case_b9be9
		.long   format_debug_msg_case_b9a8f
		.long   format_debug_msg_case_b9b7a
		.long   format_debug_msg_case_b9c07
		.long   format_debug_msg_case_b9aca
		.long   format_debug_msg_case_b9b9f
		.long   format_debug_msg_case_b9c25
		.long   format_debug_msg_case_b9b1a
		.long   format_debug_msg_case_b9bc4
		.long   format_debug_msg_case_b9c43
		.long   format_debug_msg_case_b9c96
		.long   format_debug_msg_case_b9cb4
		.long   format_debug_msg_case_b9c78
		.long   format_debug_msg_case_b9cd2
		.long   format_debug_msg_case_b9d05
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_ba004
		.long   format_debug_msg_case_b9046
		.long   format_debug_msg_case_b9d42
		.long   format_debug_msg_case_b9e40
		.long   format_debug_msg_case_b9ed1
		.long   format_debug_msg_case_b9d7d
		.long   format_debug_msg_case_b9e65
		.long   format_debug_msg_case_b9eef
		.long   format_debug_msg_case_b9db7
		.long   format_debug_msg_case_b9e89
		.long   format_debug_msg_case_b9f0c
		.long   format_debug_msg_case_b9e06
		.long   format_debug_msg_case_b9ead
		.long   format_debug_msg_case_b9f29
		.long   format_debug_msg_case_b9f7a
		.long   format_debug_msg_case_b9f97
		.long   format_debug_msg_case_b9f5d
		.long   format_debug_msg_case_b9fb4
		.long   format_debug_msg_case_b9fe6
		.long   format_debug_msg_case_b9a3d
		.long   format_debug_msg_case_b9c61
		.long   format_debug_msg_case_b9f46
		.long   format_debug_msg_case_b9118
		.long   format_debug_msg_case_b9144
		.long   format_debug_msg_case_b95e7
		.long   format_debug_msg_case_b9604
		.long   format_debug_msg_case_b9655
		.long   format_debug_msg_case_b918b
		.long   format_debug_msg_case_b9d24
		.long   format_debug_msg_case_b91ba
		.long   format_debug_msg_case_b97c0
		.long   format_debug_msg_case_b9224
		.long   format_debug_msg_case_b92be
		.long   format_debug_msg_case_b9091
		.long   format_debug_msg_case_b902f


/*----------------------------------------------------------------*/
format_debug_msg_case_b902f:
/*----------------------------------------------------------------*/
		push   $data_15dd53
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp


/*----------------------------------------------------------------*/
format_debug_msg_case_b903d:
/*----------------------------------------------------------------*/
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9046:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dd62,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9064:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dd6e,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x14,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9091:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15dd86
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b90ae:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dd95,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b90d4:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15ddaa,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x14,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9100:
/*----------------------------------------------------------------*/
		mov    $data_15ddc4,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9118:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15ddd2
		push   %esi
		call   ac_sprintf
		add    $0x14,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9144:
/*----------------------------------------------------------------*/
		push   $data_15ddea
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b915b:
/*----------------------------------------------------------------*/
		mov    $data_15ddf5,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9173:
/*----------------------------------------------------------------*/
		mov    $data_15de09,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b918b:
/*----------------------------------------------------------------*/
		push   $data_15de13
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b91a2:
/*----------------------------------------------------------------*/
		mov    $data_15de21,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b91ba:
/*----------------------------------------------------------------*/
		push   $data_15de2c
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b91d1:
/*----------------------------------------------------------------*/
		mov    $data_15de3f,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b91e9:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15de4d,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9224:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		push   $data_15de5d
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b925e:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15de6d,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9299:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15de7c,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b92be:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15de91
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b92db:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dea2,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b92f9:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15deb4,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9334:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dec5,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9352:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15ded6,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b938d:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15dee8,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b93ce:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15df01,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9409:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15df11,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9444:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15df21,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b947f:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15df34,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b949d:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15df46,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b94c2:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15df5b,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b94fd:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15df6b,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b951b:
/*----------------------------------------------------------------*/
		mov    $data_15df7f,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9533:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15df84,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9551:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15df98,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b956f:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dfa7,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b958d:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dfb6,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b95ab:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dfc4,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b95c9:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15dfd1,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b95e7:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15dfdf
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9604:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15dfed
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9621:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		mov    %eax,%edx
		shl    $0x4,%edx
		sub    %eax,%edx
		mov    EXPORT_SYMBOL(data_1a09e0),%eax
		mov    0x3a(%eax,%edx,4),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15dffd,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9655:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		mov    weapon_names(,%eax,4),%edx
		push   %edx
		push   $data_15e009
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9679:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		mov    weapon_names(,%eax,4),%ecx
		push   %ecx
		mov    $data_15e01b,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b969e:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		mov    %eax,%edx
		shl    $0x4,%edx
		sub    %eax,%edx
		mov    EXPORT_SYMBOL(data_1a09e0),%eax
		mov    0x3a(%eax,%edx,4),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e029,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b96d2:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		mov    weapon_names(,%eax,4),%ebx
		push   %ebx
		mov    $data_15e03b,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b96f7:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e04a,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9732:
/*----------------------------------------------------------------*/
		mov    $data_15e059,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b974a:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e066,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9785:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e077,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b97c0:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		push   $data_15e08a
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b97fa:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e096,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b984a:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e0b8,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9885:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e0cc,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b98aa:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e0e2,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b98cf:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e0f9,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b98f4:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e111,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9919:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e12c,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9937:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e13f,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9955:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e153,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9973:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e168,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9991:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e180,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b99af:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e196,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b99cd:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e1ab,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b99eb:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e1c0,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9a1e:
/*----------------------------------------------------------------*/
		mov    0x10(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e1e5,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9a3d:
/*----------------------------------------------------------------*/
		push   $data_15e1f2
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9a54:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e1fd,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9a8f:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e20e,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9aca:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e221,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9b1a:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e243,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9b55:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e257,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9b7a:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e26e,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9b9f:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e285,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9bc4:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e29d,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9be9:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e2b8,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c07:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e2cb,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c25:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e2df,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c43:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e2f4,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c61:
/*----------------------------------------------------------------*/
		push   $data_15e30c
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c78:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e317,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9c96:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e32d,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9cb4:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e342,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9cd2:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e357,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9d05:
/*----------------------------------------------------------------*/
		mov    0x10(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e37c,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9d24:
/*----------------------------------------------------------------*/
		mov    0x10(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e389
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9d42:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		mov    $data_15e39d,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9d7d:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		push   $data_15e3af
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9db7:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		push   $data_15e3c3
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9e06:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		lea    0x0(,%eax,4),%edx
		add    %eax,%edx
		shl    $0x2,%edx
		add    %eax,%edx
		mov    EXPORT_SYMBOL(game_things),%eax
		mov    0x4e(%eax,%edx,8),%ax
		and    $0xffff,%eax
		push   %eax
		push   $data_15e3e6
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9e40:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e3fb,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9e65:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e412
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9e89:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e42a
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9ead:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0xe(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e443
		push   %esi
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9ed1:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    $data_15e45f,%eax
		push   %eax
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9eef:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e473
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f0c:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e488
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f29:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e49e
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f46:
/*----------------------------------------------------------------*/
		push   $data_15e4b7
		push   %esi
		call   ac_sprintf
		add    $0x8,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f5d:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e4c3
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f7a:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e4da
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9f97:
/*----------------------------------------------------------------*/
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e4f0
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9fb4:
/*----------------------------------------------------------------*/
		mov    0x6(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x4(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    0x2(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		mov    (%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e506
		push   %esi
		call   ac_sprintf
		add    $0x18,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_b9fe6:
/*----------------------------------------------------------------*/
		mov    0x10(%edx),%eax
		sar    $0x10,%eax
		push   %eax
		push   $data_15e52c
		push   %esi
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x1,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
format_debug_msg_case_ba004:
/*----------------------------------------------------------------*/
		xor    %al,%al
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret

.section .rodata

weapon_names:
		.long   data_15e691
		.long   data_15e692
		.long   data_15e697
		.long   data_15e6a0
		.long   data_15e6a7
		.long   data_15e6b7
		.long   data_15e6bc
		.long   data_15e6cd
		.long   data_15e6dc
		.long   data_15e6e4
		.long   data_15e6f5
		.long   data_15e700
		.long   data_15e70e
		.long   data_15e71d
		.long   data_15e72d
		.long   data_15e73d
		.long   data_15e749
		.long   data_15e755
		.long   data_15e75b
		.long   data_15e767
		.long   data_15e774
		.long   data_15e782
		.long   data_15e792
		.long   data_15e7a0
		.long   data_15e7af
		.long   data_15e7b9
		.long   data_15e7c3
		.long   data_15e7d0
		.long   data_15e7d8
		.long   data_15e7e0
		.long   data_15e7ee
		.long	0x0
		.long   data_15e7fc
		.long   data_15e7fd
		.long   data_15e7fe
		.long   data_15e7ff
		.long   data_15e800
		.long   data_15e801
		.long   data_15e802


data_15dd2c:
		.string "TH %d ID %d"
data_15dd38:
		.string "TH %d ID %d"
data_15dd44:
		.string "%d "
data_15dd48:
		.string "U "
data_15dd4b:
		.string "IU "
data_15dd4f:
		.string "!!!"
data_15dd53:
		.string "IGNORE ENEMIES"
data_15dd62:
		.string "LOOP COM %d"
data_15dd6e:
		.string "GO TO POINT X%d Y%d Z%d"
data_15dd86:
		.string "PLAY SAMPLE %d"
data_15dd95:
		.string "RUN TO POINT X%d Z%d"
data_15ddaa:
		.string "GOTOPOINT FACE %d X%d Z%d"
data_15ddc4:
		.string "KILL EVERYONE"
data_15ddd2:
		.string "WITHIN AREA X%d Y%d Z%d"
data_15ddea:
		.string "WITHIN OFF"
data_15ddf5:
		.string "Execute Coms (plyr)"
data_15de09:
		.string "GUARD OFF"
data_15de13:
		.string "HARD AS AGENT"
data_15de21:
		.string "PING EXIST"
data_15de2c:
		.string "start danger music"
data_15de3f:
		.string "SELF_DESTRUCT"
data_15de4d:
		.string "GO TO PERSON %d"
data_15de5d:
		.string "CAMERA TRACK %d"
data_15de6d:
		.string "KILL PERSON %d"
data_15de7c:
		.string "KILL %d MEM GROUP %d"
data_15de91:
		.string "UNTRUCE GROUP %d"
data_15dea2:
		.string "KILL ALL GROUP %d"
data_15deb4:
		.string "FOLLOW PERSON %d"
data_15dec5:
		.string "PROTECT MEM G %d"
data_15ded6:
		.string "SUPPORT PERSON %d"
data_15dee8:
		.string "PROTECT PERSON %d idx %d"
data_15df01:
		.string "BLOCK PERSON %d"
data_15df11:
		.string "SCARE PERSON %d"
data_15df21:
		.string "PERSUADE PERSON %d"
data_15df34:
		.string "PERSUADE All G %d"
data_15df46:
		.string "PERSUADE %d Mem G %d"
data_15df5b:
		.string "AVOID PERSON %d"
data_15df6b:
		.string "WAND AVOID GROUP %d"
data_15df7f:
		.string "HIDE"
data_15df84:
		.string "DESTROY BUILDING %d"
data_15df98:
		.string "CATCH TRAIN %d"
data_15dfa7:
		.string "CATCH FERRY %d"
data_15dfb6:
		.string "EXIT FERRY %d"
data_15dfc4:
		.string "OPEN DOME %d"
data_15dfd1:
		.string "CLOSE DOME %d"
data_15dfdf:
		.string "LOCK BUILD %d"
data_15dfed:
		.string "UNLOCK BUILD %d"
data_15dffd:
		.string "GET ITEM %d"
data_15e009:
		.string "SELECT WEAPON %s "
data_15e01b:
		.string "USE WEAPON %s"
data_15e029:
		.string "DROP SPEC ITEM %d"
data_15e03b:
		.string "DROP WEAPON %s"
data_15e04a:
		.string "USE VEHICLE %d"
data_15e059:
		.string "EXIT VEHICLE"
data_15e066:
		.string "WAIT P/V DEAD %d"
data_15e077:
		.string "WAIT P/V/I NEAR %d"
data_15e08a:
		.string "PING P/V %d"
data_15e096:
		.string "WAIT P/V/I ARRIVES %d X%d Y%d Z%d"
data_15e0b8:
		.string "WAIT P PERSUADED %d"
data_15e0cc:
		.string "WAIT %d MEM G DEAD %d"
data_15e0e2:
		.string "WAIT %d MEM G %d NEAR "
data_15e0f9:
		.string "WAIT %d MEM G ARRIVE %d"
data_15e111:
		.string "WAIT %d MEM G PERSUADED %d"
data_15e12c:
		.string "WAIT ALL G DEAD %d"
data_15e13f:
		.string "WAIT ALL G %d NEAR "
data_15e153:
		.string "WAIT ALL G ARRIVE %d"
data_15e168:
		.string "WAIT ALL G PERSUADED %d"
data_15e180:
		.string "WAIT MISSION START %d"
data_15e196:
		.string "WAIT MISSION SUCC %d"
data_15e1ab:
		.string "WAIT MISSION FAIL %d"
data_15e1c0:
		.string "WAIT OBJECT DESTROYED %d X%d Y%d Z%d"
data_15e1e5:
		.string "WAIT TIME %d"
data_15e1f2:
		.string "WAIT OBJ:-"
data_15e1fd:
		.string "WAND P/V DEAD %d"
data_15e20e:
		.string "WAND P/V/I NEAR %d"
data_15e221:
		.string "WAND P/V/I ARRIVES %d X%d Y%d Z%d"
data_15e243:
		.string "WAND P PERSUADED %d"
data_15e257:
		.string "WAND %d MEM G %d DEAD "
data_15e26e:
		.string "WAND %d MEM G %d NEAR "
data_15e285:
		.string "WAND %d MEM G ARRIVE %d"
data_15e29d:
		.string "WAND %d MEM G PERSUADED %d"
data_15e2b8:
		.string "WAND ALL G DEAD %d"
data_15e2cb:
		.string "WAND ALL G %d NEAR "
data_15e2df:
		.string "WAND ALL G ARRIVE %d"
data_15e2f4:
		.string "WAND ALL G PERSUADED %d"
data_15e30c:
		.string "WAND OBJ:-"
data_15e317:
		.string "WAND MISSION START %d"
data_15e32d:
		.string "WAND MISSION SUCC %d"
data_15e342:
		.string "WAND MISSION FAIL %d"
data_15e357:
		.string "WAND OBJECT DESTROYED %d X%d Y%d Z%d"
data_15e37c:
		.string "WAND TIME %d"
data_15e389:
		.string "UNTIL G %d NOT SEEN"
data_15e39d:
		.string "UNTIL P/V DEAD %d"
data_15e3af:
		.string "UNTIL P/V/I NEAR %d"
data_15e3c3:
		.string "UNTIL P/V/I ARRIVES %d X%d Y%d Z%d"
data_15e3e6:
		.string "UNTIL P PERSUADED %d"
data_15e3fb:
		.string "UNTIL %d MEM G DEAD %d"
data_15e412:
		.string "UNTIL %d MEM G %d NEAR "
data_15e42a:
		.string "UNTIL %d MEM G ARRIVE %d"
data_15e443:
		.string "UNTIL %d MEM G PERSUADED %d"
data_15e45f:
		.string "UNTIL ALL G DEAD %d"
data_15e473:
		.string "UNTIL ALL G %d NEAR "
data_15e488:
		.string "UNTIL ALL G ARRIVE %d"
data_15e49e:
		.string "UNTIL ALL G PERSUADED %d"
data_15e4b7:
		.string "UNTIL OBJ:-"
data_15e4c3:
		.string "UNTIL MISSION START %d"
data_15e4da:
		.string "UNTIL MISSION SUCC %d"
data_15e4f0:
		.string "UNTIL MISSION FAIL %d"
data_15e506:
		.string "UNTIL OBJECT DESTROYED %d X%d Y%d Z%d"
data_15e52c:
		.string "UNTIL TIME %d"

data_15e691:
		.byte	0x0
data_15e692:
		.string " Uzi"
data_15e697:
		.string " Minigun"
data_15e6a0:
		.string " Laser"
data_15e6a7:
		.string " Electric Laser"
data_15e6b7:
		.string " RAP"
data_15e6bc:
		.string " Nuclear Grenade"
data_15e6cd:
		.string " Persuadertron"
data_15e6dc:
		.string " Flamer"
data_15e6e4:
		.string " Hand2Hand Taser"
data_15e6f5:
		.string " Crazy Gas"
data_15e700:
		.string " Knockout Gas"
data_15e70e:
		.string " Electric Mine"
data_15e71d:
		.string " Explosive Mine"
data_15e72d:
		.string " ***Napalm Mine"
data_15e73d:
		.string " Long Range"
data_15e749:
		.string " Air Strike"
data_15e755:
		.string " Beam"
data_15e75b:
		.string " Razor Wire"
data_15e767:
		.string " Sonic Blast"
data_15e774:
		.string " Q DEVASTATOR"
data_15e782:
		.string " PERSUADERTRON2"
data_15e792:
		.string " Stasis Field"
data_15e7a0:
		.string " Energy Shield"
data_15e7af:
		.string " Soul Gun"
data_15e7b9:
		.string " Time Gun"
data_15e7c3:
		.string " Cerebus IFF"
data_15e7d0:
		.string " medi 1"
data_15e7d8:
		.string " medi 2"
data_15e7e0:
		.string " EXPLODE Wire"
data_15e7ee:
		.string " Clone Shield"
data_15e7fc:
		.byte	0x0
data_15e7fd:
		.byte	0x0
data_15e7fe:
		.byte	0x0
data_15e7ff:
		.byte	0x0
data_15e800:
		.byte	0x0
data_15e801:
		.byte	0x0
data_15e802:
		.short  0x0

/* Person state text.
 * Used by function at 0xBAC50, which was not disassembled.
 */
person_st_text:	/* 0x154aac */
		.long   data_15cdc0
		.long   data_15cdc1
		.long   data_15cdcf
		.long   data_15cde0
		.long   data_15cdea
		.long   data_15cdfc
		.long   data_15ce04
		.long   data_15ce17
		.long   data_15ce26
		.long   data_15ce33
		.long   data_15ce4d
		.long   data_15ce5e
		.long   data_15ce65
		.long   data_15ce6f
		.long   data_15ce77
		.long   data_15ce8b
		.long   data_15ce9a
		.long   data_15cea9
		.long   data_15ceb8
		.long   data_15cecb
		.long   data_15cede
		.long   data_15ceed
		.long   data_15cf00
		.long   data_15cf12
		.long   data_15cf28
		.long   data_15cf35
		.long   data_15cf45
		.long   data_15cf4d
		.long   data_15cf5c
		.long   data_15cf6c
		.long   data_15cf7c
		.long   data_15cf89
		.long   data_15cf9c
		.long   data_15cfaa
		.long   data_15cfb6
		.long   data_15cfc2
		.long   data_15cfd4
		.long   data_15cfe6
		.long   data_15cff4
		.long   data_15d007
		.long   data_15d01b
		.long   data_15d02e
		.long   data_15d03f
		.long   data_15d051
		.long   data_15d063
		.long   data_15d06f
		.long   data_15d07e
		.long   data_15d096
		.long   data_15d0a6
		.long   data_15d0b6
		.long   data_15d0c5
		.long   data_15d0d3
		.long   data_15d0e7
		.long   data_15d0f7
		.long   data_15d108
		.long   data_15d120
		.long   data_15d12f
		.long   data_15d13d
		.long   data_15d14c
		.long   data_15d14d
		.long   data_15d14e
		.long   data_15d14f
		.long   data_15d150
		.long   data_15d151
		.long   data_15d152
		.long   data_15d153

data_15cdc0:
		.string ""
data_15cdc1:
		.string "ST_GOTO_POINT"
data_15cdcf:
		.string "ST_BOMB_BUILDING"
data_15cde0:
		.string "ST_WANDER"
data_15cdea:
		.string "ST_EXPLODE_EFFECT"
data_15cdfc:
		.string "ST_WAIT"
data_15ce04:
		.string "ST_AGENT_DEFENSIVE"
data_15ce17:
		.string "ST_PICKUP_ITEM"
data_15ce26:
		.string "ST_DROP_ITEM"
data_15ce33:
		.string "ST_SHOOT_PERSON_WITH_BGUN"
data_15ce4d:
		.string "ST_SHOOT_BGUN_AT"
data_15ce5e:
		.string "ST_DIE"
data_15ce65:
		.string "ST_DIEING"
data_15ce6f:
		.string "ST_DEAD"
data_15ce77:
		.string "ST_INIT_SAVE_VICTIM"
data_15ce8b:
		.string "ST_SAVE_VICTIM"
data_15ce9a:
		.string "ST_GOTO_PERSON"
data_15cea9:
		.string "ST_WAIT_P_DEAD"
data_15ceb8:
		.string "ST_WAIT_ALL_G_DEAD"
data_15cecb:
		.string "ST_WAIT_MEM_G_DEAD"
data_15cede:
		.string "ST_WAIT_P_NEAR"
data_15ceed:
		.string "ST_WAIT_MEM_G_NEAR"
data_15cf00:
		.string "ST_WAIT_P_ARRIVES"
data_15cf12:
		.string "ST_WAIT_MEM_G_ARRIVES"
data_15cf28:
		.string "ST_WAIT_TIME"
data_15cf35:
		.string "ST_DEFINE_ROUTE"
data_15cf45:
		.string " UNUSED"
data_15cf4d:
		.string "ST_KILL_PERSON"
data_15cf5c:
		.string "ST_BLOCK_PERSON"
data_15cf6c:
		.string "ST_SCARE_PERSON"
data_15cf7c:
		.string "ST_USE_ITEM "
data_15cf89:
		.string "ST_STATIONARY_SHOT"
data_15cf9c:
		.string "ST_RECOIL_HIT"
data_15cfaa:
		.string "ST_V_PARKED"
data_15cfb6:
		.string "ST_V_DRIVEN"
data_15cfc2:
		.string "ST_GOTO_POINT_REL"
data_15cfd4:
		.string "ST_PERSON_BURNING"
data_15cfe6:
		.string "ST_IN_VEHICLE"
data_15cff4:
		.string "ST_DRIVING_VEHICLE"
data_15d007:
		.string "ST_V_GOTO_POINT_FLY"
data_15d01b:
		.string "ST_PERSUADE_PERSON"
data_15d02e:
		.string "ST_FOLLOW_PERSON"
data_15d03f:
		.string "ST_SUPPORT_PERSON"
data_15d051:
		.string "ST_PROTECT_PERSON"
data_15d063:
		.string "ST_GET_ITEM"
data_15d06f:
		.string "ST_USE_VEHICLE"
data_15d07e:
		.string "ST_USE_VEHICLE_TO_POINT"
data_15d096:
		.string "ST_WAIT_VEHICLE"
data_15d0a6:
		.string "ST_GOTO_VEHICLE"
data_15d0b6:
		.string "ST_CATCH_TRAIN"
data_15d0c5:
		.string "ST_WAIT_TRAIN"
data_15d0d3:
		.string "ST_DESTROY_BUILDING"
data_15d0e7:
		.string "ST_WANDER_DRIVE"
data_15d0f7:
		.string "ST_GO_PLANT_MINE"
data_15d108:
		.string "ST_WAIT_TO_EXIT_VEHICLE"
data_15d120:
		.string "ST_CATCH_FERRY"
data_15d12f:
		.string "ST_EXIT_FERRY"
data_15d13d:
		.string "ST_AVOID_GROUP"
data_15d14c:
		.string ""
data_15d14d:
		.string ""
data_15d14e:
		.string ""
data_15d14f:
		.string ""
data_15d150:
		.string ""
data_15d151:
		.string ""
data_15d152:
		.string ""
data_15d153:
		.string ""

