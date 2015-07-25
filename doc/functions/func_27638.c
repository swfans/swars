func_27638 (uint16_t ax)
{
  esi = 1a09d0_full_lights;
  edi = 1a09cc_quick_lights;
  esp_80 = ax;
  esp_7c = 1a09e8_object_faces + ax * 32;
  data_152e24 = *(esp_7c + 0xa);
  data_152e25 = *(esp_7c + 0x9);

  if ((int16_t) *(esp_7c + 0x6) != 0)
    {
      eax = 1a09d8_face_textures + (int16_t) *(esp_7c + 0x6) * 16;
      data_150108 = *(data_1aa278 + (int8_t) *(eax + 0x6) * 4);
      bl = *(esp_7c + 0x8);
      if (bl != 0)
	{
	  if ((bl & 0x2) != 0)
	    data_150108 = data_19cb54;

	  if (((int8_t) *(esp_7c + 0x8) & 0x40) != 0)
	    {
	      edx = data_1a62dc + (int16_t) *(esp_7c + 0xc);
	      if ((edx & 0x1ff) > 0x100)
		{
		  edx &= 0x3f;
		  if ((int8_t) *(data_153014 + edx) == 0)
		    data_152e25 = 0x5; /* 8bit */
		}
	    }
	}

      xor    %edx,%edx
      mov    (%eax),%dl
      shl    $0x10,%edx
      mov    %edx,0x30(%esp)
      xor    %edx,%edx
      mov    0x1(%eax),%dl
      shl    $0x10,%edx
      mov    %edx,0x34(%esp)
      xor    %edx,%edx
      mov    0x4(%eax),%dl
      shl    $0x10,%edx
      mov    %edx,0x8(%esp)
      xor    %edx,%edx
      mov    0x5(%eax),%dl
      shl    $0x10,%edx
      mov    %edx,0xc(%esp)
      xor    %edx,%edx
      mov    0x2(%eax),%dl
      shl    $0x10,%edx
      mov    %edx,0x58(%esp)
      mov    0x3(%eax),%al
      and    $0xff,%eax
      shl    $0x10,%eax
      mov    %eax,0x5c(%esp)
    }

jump_2775a:
  mov    0x7c(%esp),%eax
  xor    %edx,%edx
  mov    (%eax),%dx
  lea    0x0(,%edx,4),%eax
  mov    1a09e4_object_points,%ecx
  add    %edx,%eax
  mov    (%ecx,%eax,2),%ax
  mov    data_19f540,%edx
  and    $0xffff,%eax
  mov    data_176d00,%ebx
  movswl (%edx,%eax,8),%eax
  add    %eax,%ebx
  mov    0x7c(%esp),%eax
  mov    %ebx,0x28(%esp)
  xor    %ebx,%ebx
  mov    (%eax),%bx
  lea    0x0(,%ebx,4),%eax
  add    %ebx,%eax
  mov    (%ecx,%eax,2),%ax
  and    $0xffff,%eax
  mov    (%edx,%eax,8),%eax
  mov    data_176d04,%edx
  sar    $0x10,%eax
  add    %eax,%edx
  xor    %eax,%eax
  mov    data_152e25,%al
  mov    %edx,0x2c(%esp)
  cmp    $0x2,%eax
  je     jump_27838
  test   %eax,%eax
  je     jump_27838
  mov    0x7c(%esp),%eax
  xor    %ebx,%ebx
  mov    0xe(%eax),%bx
  xor    %edx,%edx
  shl    $0x7,%ebx
  mov    0x14(%eax),%ax
  add    $0x100,%ebx
  test   %ax,%ax
  je     jump_27822

jump_277f0:
  mov    %edx,%ecx
  inc    %edx
  cmp    $0x64,%ecx
  jge    jump_27822
  and    $0xffff,%eax
  imul   $0x6,%eax,%ecx
  lea    (%edi,%ecx,1),%eax
  xor    %ecx,%ecx
  mov    0x2(%eax),%cx
  shl    $0x5,%ecx
  add    %esi,%ecx
  movzwl (%eax),%ebp
  movswl (%ecx),%ecx
  imul   %ebp,%ecx
  mov    0x4(%eax),%ax
  add    %ecx,%ebx
  test   %ax,%ax
  jne    jump_277f0

jump_27822:
  cmp    $0x7e00,%ebx
  jle    jump_2782f
  mov    $0x7f00,%ebx

jump_2782f:
  shl    $0x7,%ebx
  mov    %ebx,0x38(%esp)
  jmp    jump_27840

jump_27838:
  movl   $0x200000,0x38(%esp)

jump_27840:
  mov    0x7c(%esp),%eax
  xor    %edx,%edx
  mov    0x4(%eax),%dx
  lea    0x0(,%edx,4),%eax
  mov    1a09e4_object_points,%ebx
  add    %edx,%eax
  mov    (%ebx,%eax,2),%ax
  mov    data_19f540,%edx
  and    $0xffff,%eax
  mov    data_176d00,%ecx
  movswl (%edx,%eax,8),%eax
  add    %eax,%ecx
  mov    0x7c(%esp),%eax
  mov    %ecx,(%esp)
  xor    %ecx,%ecx
  mov    0x4(%eax),%cx
  lea    0x0(,%ecx,4),%eax
  add    %ecx,%eax
  add    %eax,%eax
  add    %eax,%ebx
  xor    %eax,%eax
  mov    (%ebx),%ax
  mov    (%edx,%eax,8),%eax
  mov    data_176d04,%edx
  sar    $0x10,%eax
  add    %eax,%edx
  xor    %eax,%eax
  mov    data_19ec71,%al
  mov    %edx,0x4(%esp)
  cmp    $0x7,%eax
  jne    jump_278ca
  mov    0x38(%esp),%eax
  mov    $0x7,%dh
  sar    $0x10,%eax
  mov    %dh,data_152e25
  mov    %al,data_152e24
  jmp    jump_279dd

jump_278ca:
  xor    %eax,%eax
  mov    data_152e25,%al
  cmp    $0x2,%eax
  je     jump_27952
  test   %eax,%eax
  je     jump_27952
  mov    0x7c(%esp),%eax
  xor    %ebx,%ebx
  mov    0x12(%eax),%bx
  xor    %edx,%edx
  shl    $0x7,%ebx
  mov    0x18(%eax),%ax
  add    $0x100,%ebx
  test   %ax,%ax
  je     jump_2793c

jump_27900:
  mov    %edx,%ecx
  inc    %edx
  cmp    $0x64,%ecx
  jge    jump_2793c
  xor    %ecx,%ecx
  mov    %ax,%cx
  imul   $0x6,%ecx,%ecx
  lea    (%edi,%ecx,1),%eax
  xor    %ecx,%ecx
  mov    0x2(%eax),%cx
  shl    $0x5,%ecx
  add    %esi,%ecx
  movswl (%ecx),%ecx
  mov    %ecx,0x78(%esp)
  xor    %ecx,%ecx
  mov    0x78(%esp),%ebp
  mov    (%eax),%cx
  imul   %ebp,%ecx
  mov    0x4(%eax),%ax
  add    %ecx,%ebx
  test   %ax,%ax
  jne    jump_27900

jump_2793c:
  cmp    $0x7e00,%ebx
  jle    jump_27949
  mov    $0x7f00,%ebx

jump_27949:
  shl    $0x7,%ebx
  mov    %ebx,0x10(%esp)
  jmp    jump_2795a

jump_27952:
  movl   $0x200000,0x10(%esp)

jump_2795a:
  xor    %eax,%eax
  mov    data_152e25,%al
  cmp    $0x2,%eax
  je     jump_279d5
  test   %eax,%eax
  je     jump_279d5
  mov    0x7c(%esp),%eax
  xor    %ebx,%ebx
  mov    0x10(%eax),%bx
  xor    %edx,%edx
  shl    $0x7,%ebx
  mov    0x16(%eax),%ax
  add    $0x100,%ebx
  test   %ax,%ax
  je     jump_279bf

jump_2798c:
  mov    %edx,%ecx
  inc    %edx
  cmp    $0x64,%ecx
  jge    jump_279bf
  and    $0xffff,%eax
  imul   $0x6,%eax,%eax
  mov    %edi,%ecx
  add    %edi,%eax
  xor    %edi,%ecx
  mov    0x2(%eax),%cx
  shl    $0x5,%ecx
  add    %esi,%ecx
  movzwl (%eax),%ebp
  movswl (%ecx),%ecx
  imul   %ebp,%ecx
  mov    0x4(%eax),%ax
  add    %ecx,%ebx
  test   %ax,%ax
  jne    jump_2798c

jump_279bf:
  cmp    $0x7e00,%ebx
  jle    jump_279cc
  mov    $0x7f00,%ebx

jump_279cc:
  shl    $0x7,%ebx
  mov    %ebx,0x60(%esp)
  jmp    jump_279dd

jump_279d5:
  movl   $0x200000,0x60(%esp)

jump_279dd:
  mov    0x7c(%esp),%eax
  xor    %edx,%edx
  mov    0x2(%eax),%dx
  lea    0x0(,%edx,4),%eax
  add    %edx,%eax
  mov    1a09e4_object_points,%edx
  mov    (%edx,%eax,2),%ax
  mov    data_19f540,%ebx
  and    $0xffff,%eax
  mov    data_176d00,%ecx
  movswl (%ebx,%eax,8),%eax
  add    %eax,%ecx
  mov    0x7c(%esp),%eax
  mov    %ecx,0x50(%esp)
  xor    %ecx,%ecx
  mov    0x2(%eax),%cx
  lea    0x0(,%ecx,4),%eax
  add    %ecx,%eax
  mov    (%edx,%eax,2),%ax
  and    $0xffff,%eax
  mov    (%ebx,%eax,8),%eax
  mov    data_176d04,%edx
  sar    $0x10,%eax
  add    %eax,%edx
  mov    data_19ec6f,%bl
  mov    %edx,0x54(%esp)
  test   %bl,%bl
  jne    jump_27a5e
  mov    $0x200000,%eax
  mov    %eax,0x38(%esp)
  mov    %eax,0x10(%esp)
  mov    %eax,0x60(%esp)

jump_27a5e:
  mov    data_176d4c,%ecx
  mov    %esi,1a09d0_full_lights
  mov    %edi,1a09cc_quick_lights
  xor    %eax,%eax
  inc    %ecx
  mov    data_19ec71,%al
  mov    %ecx,data_176d4c
  cmp    $0x3,%eax
  jne    jump_27af2
  xor    %al,%al
  mov    0x60(%esp),%edx
  mov    %al,data_152e25
  xor    %eax,%eax
  sar    $0x10,%edx
  mov    data_1a71ba,%al
  shl    $0x8,%edx
  lea    0x50(%esp),%ebx
  mov    data_162280(%edx,%eax,1),%al
  mov    %esp,%edx
  mov    %al,data_152e24
  lea    0x28(%esp),%eax
  call   func_128380_wrapper
  mov    data_1a71bb,%al
  mov    %al,data_152e24
  cmpb   $0x0,data_1e302c
  jne    jump_27b46
  lea    0x50(%esp),%edx
  lea    0x28(%esp),%eax
  call   func_127daf
  lea    0x50(%esp),%edx
  mov    %esp,%eax
  call   func_127daf
  mov    %esp,%edx
  lea    0x28(%esp),%eax
  call   func_127daf
  jmp    jump_27b46

jump_27af2:
  xor    %eax,%eax
  mov    data_152e25,%al
  cmp    $0x2,%eax
  jne    jump_27b05
  movb   $0x1b,data_152e25

jump_27b05:
  lea    0x50(%esp),%ebx
  mov    %esp,%edx
  lea    0x28(%esp),%eax
  call   func_128380_wrapper
  mov    0x7c(%esp),%eax
  testb  $0x1,0x8(%eax)
  je     jump_27b46
  xor    %eax,%eax
  mov    data_152e25,%al
  cmp    $0x2,%eax
  jne    jump_27b31
  movb   $0x1b,data_152e25

jump_27b31:
  mov    %esp,%ebx
  lea    0x50(%esp),%edx
  lea    0x28(%esp),%eax
  call   func_128380_wrapper
  incl   data_176d4c

jump_27b46:
  mov    0x7c(%esp),%eax
  testb  $0x4,0x8(%eax)
  je     jump_27b9a
  xor    %edx,%edx
  mov    data_1a71b0,%dl
  lea    0x0(,%edx,8),%eax
  add    %edx,%eax
  shl    $0x3,%eax
  sub    %edx,%eax
  add    %eax,%eax
  mov    %eax,%edx
  shl    $0x2,%eax
  sub    %edx,%eax
  mov    data_1a63e2(%eax),%eax
  sar    $0x10,%eax
  cmp    $0x3,%eax
  jge    jump_27b9a
  push   $0x1
  mov    0x82(%esp),%ecx
  lea    0x54(%esp),%ebx
  lea    0x4(%esp),%edx
  lea    0x2c(%esp),%eax
  sar    $0x10,%ecx
  call   func_24a2c

jump_27b9a:
  cmpb   $0x0,data_1c844e
  je     jump_27ba8
  call   func_bbc50

jump_27ba8:
  mov    1a09d0_full_lights,%esi
  add    $0x84,%esp
  pop    %ebp
  pop    %edi
  pop    %esi
  pop    %edx
  pop    %ecx
  pop    %ebx
  ret
}
