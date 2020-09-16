
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

.global EXPORT_SYMBOL(XXX);

/*----------------------------------------------------------------*/
GLOBAL_FUNC (InitAudio_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		mov    %eax,%edx
		mov    $data_15ed40,%eax
		call   ac_getenv
		test   %eax,%eax
		je     jump_eb7dd
		movb   $0x1,DebugAudio
	jump_eb7dd:
		push   $data_15ed4f
		push   $FullDIG_INIPath
		call   ac_sprintf
		add    $0x8,%esp
		push   $data_15ed5d
		push   $FullMDI_INIPath
		call   ac_sprintf
		add    $0x8,%esp
		mov    $data_15ed6b,%esi
		mov    $SoundAndMusicDataPath,%edi
		push   %edi
	jump_eb80c:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_eb824
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_eb80c
	jump_eb824:
		pop    %edi
		mov    $data_15ed71,%esi
		mov    $SoundDriverPath,%edi
		push   %edi
	jump_eb830:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_eb848
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_eb830
	jump_eb848:
		pop    %edi
		mov    (%edx),%ebx
		test   %ebx,%ebx
		je     jump_eb877
		push   %ebx
		push   $data_15ed77
		push   $FullDIG_INIPath
		call   ac_sprintf
		add    $0xc,%esp
		mov    (%edx),%esi
		push   %esi
		push   $data_15ed82
		push   $FullMDI_INIPath
		call   ac_sprintf
		add    $0xc,%esp
	jump_eb877:
		mov    0x8(%edx),%edi
		test   %edi,%edi
		je     jump_eb89f
		mov    %edi,%esi
		mov    $SoundAndMusicDataPath,%edi
		push   %edi
	jump_eb886:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_eb89e
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_eb886
	jump_eb89e:
		pop    %edi
	jump_eb89f:
		mov    0x4(%edx),%ebp
		test   %ebp,%ebp
		je     jump_eb8c7
		mov    $SoundDriverPath,%edi
		mov    %ebp,%esi
		push   %edi
	jump_eb8ae:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_eb8c6
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_eb8ae
	jump_eb8c6:
		pop    %edi
	jump_eb8c7:
		xor    %eax,%eax
		mov    0x14(%edx),%al
		mov    %eax,MaxNumberOfSamples
		mov    0xc(%edx),%ax
		mov    0x16(%edx),%bl
		mov    %ax,SoundType
		test   %bl,%bl
		jne    jump_eb8e8
		mov    %bl,AutoScanForSoundHardware
	jump_eb8e8:
		mov    0x15(%edx),%cl
		test   %cl,%cl
		jne    jump_eb8f5
		mov    %cl,StereoSound
	jump_eb8f5:
		xor    %eax,%eax
		mov    0x17(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb906
		movb   $0x1,ive_got_an_awe32
	jump_eb906:
		xor    %eax,%eax
		mov    0x18(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb917
		movb   $0x1,DisableLoadSounds
	jump_eb917:
		xor    %eax,%eax
		mov    0x19(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb928
		movb   $0x1,DisableLoadMusic
	jump_eb928:
		xor    %eax,%eax
		mov    0x1a(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb939
		movb   $0x1,UseCurrentAwe32Soundfont
	jump_eb939:
		xor    %eax,%eax
		mov    0x1b(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb94a
		movb   $0x1,UseMultiMediaExtensions
	jump_eb94a:
		xor    %eax,%eax
		mov    MusicAble,%al
		push   %eax
		push   $data_15ed8d
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    SoundAble,%al
		push   %eax
		push   $data_15ed9d
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		testb  $0x1,0xe(%edx)
		jne    jump_eb99f
		xor    %al,%al
		mov    %al,MusicAble
	jump_eb99f:
		testb  $0x2,0xe(%edx)
		jne    jump_eb9ad
		xor    %bl,%bl
		mov    %bl,SoundAble
	jump_eb9ad:
		call   InitSound_
		call   InitMusic_
		call   InitAllBullfrogSoundTimers_
		xor    %eax,%eax
		mov    0x1c(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb9cb
		call   InitStreamedSound_
	jump_eb9cb:
		xor    %eax,%eax
		mov    0x1d(%edx),%al
		cmp    $0x1,%eax
		jne    jump_eb9dc
		call   InitRedbook_
		jmp    jump_eb9e4
	jump_eb9dc:
		xor    %bh,%bh
		mov    %bh,CDAble
	jump_eb9e4:
		cmpb   $0x0,ive_got_an_sb16
		je     jump_eb9f2
		call   prepare_SB16_volumes
	jump_eb9f2:
		xor    %eax,%eax
		mov    MusicAble,%al
		push   %eax
		push   $data_15edad
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    SoundAble,%al
		push   %eax
		push   $data_15edbd
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		push   $awe32_s1
		push   $data_15edcd
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		push   $SoundInstallChoice__Field1
		push   $data_15ede7
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void prepare_SB16_volumes(void)
 */
prepare_SB16_volumes:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		movswl SoundInstallChoice__Field2,%ebx
		cmp    $0x220,%ebx
		jne    jump_ebbb1
		lea    0x4(%ebx),%ecx
		mov    $0x30,%al
		mov    %ecx,%edx
		add    $0x5,%ebx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		shr    $0x3,%eax
		mov    %ecx,%edx
		mov    %ax,current_SB16_left_master_volume
		mov    $0x31,%al
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		shr    $0x3,%eax
		mov    %ecx,%edx
		mov    %ax,current_SB16_right_master_volume
		mov    $0x32,%al
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_left_digi_volume
		xor    %eax,%eax
		mov    current_SB16_left_master_volume,%ax
		lea    0x0(,%eax,8),%eax
		sub    $0x18,%eax
		out    %al,(%dx)
		mov    $0x33,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_right_digi_volume
		xor    %eax,%eax
		mov    current_SB16_right_master_volume,%ax
		lea    0x0(,%eax,8),%eax
		sub    $0x18,%eax
		out    %al,(%dx)
		mov    $0x44,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_left_treble_volume
		mov    $0x60,%al
		out    %al,(%dx)
		mov    $0x45,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_right_treble_volume
		mov    $0x60,%al
		out    %al,(%dx)
		mov    $0x46,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_left_bass_volume
		mov    $0x80,%al
		out    %al,(%dx)
		mov    $0x47,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_right_bass_volume
		mov    $0x80,%al
		out    %al,(%dx)
		mov    $0x34,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_left_midi_volume
		xor    %eax,%eax
		mov    current_SB16_left_master_volume,%ax
		lea    0x0(,%eax,8),%eax
		sub    $0x10,%eax
		out    %al,(%dx)
		mov    $0x35,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    %ebx,%edx
		sub    %eax,%eax
		in     (%dx),%al
		mov    %ax,old_SB16_right_midi_volume
		xor    %eax,%eax
		mov    current_SB16_right_master_volume,%ax
		lea    0x0(,%eax,8),%eax
		sub    $0x10,%eax
		out    %al,(%dx)
		movb   $0x1,sb16_mixer_set
	jump_ebbb1:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void reset_SB16_volumes(void) */
reset_SB16_volumes:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		movswl SoundInstallChoice__Field2,%ecx
		cmp    $0x220,%ecx
		jne    jump_ebc43
		lea    0x4(%ecx),%ebx
		mov    $0x32,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    $0x225,%edx
		mov    old_SB16_left_digi_volume,%al
		out    %al,(%dx)
		mov    $0x33,%al
		mov    %ebx,%edx
		add    $0x5,%ecx
		out    %al,(%dx)
		mov    old_SB16_right_digi_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x34,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_left_midi_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x35,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_right_midi_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x44,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_left_treble_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x45,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_right_treble_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x46,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_left_bass_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
		mov    $0x47,%al
		mov    %ebx,%edx
		out    %al,(%dx)
		mov    old_SB16_right_bass_volume,%al
		mov    %ecx,%edx
		out    %al,(%dx)
	jump_ebc43:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
cbCDCountdown:
/*----------------------------------------------------------------*/
		push   %esi
		cmpl   $0x0,CDCountdown
		jbe    jump_ebc8a
		subl   $0x5,CDCountdown
		cmpl   $0x0,CDCountdown
		jne    jump_ebc8a
		mov    CDCount_handle,%esi
		xor    %edx,%edx
		push   %esi
		mov    %dx,CurrentCDTrack
		call   AIL_release_timer_handle
		add    $0x4,%esp
		xor    %ah,%ah
		mov    %ah,CDTimerActive
	jump_ebc8a:
		pop    %esi
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetCDAble_)
/*----------------------------------------------------------------*/
		mov    CDAble,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (InitRedbook_)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		call   GetSoundAble_
		test   %al,%al
		jne    jump_ebce9
		call   GetMusicAble_
		test   %al,%al
		jne    jump_ebce9
		call   ail_startup
	jump_ebce9:
		call   cd_init_
		test   %al,%al
		jne    jump_ebcfa
		mov    %al,CDAble
		pop    %edx
		pop    %ecx
		ret
	jump_ebcfa:
		call   GetCDVolume_
		mov    %al,data_158b34
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (PlayCDTrack_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    %eax,%esi
		cmpb   $0x0,CDAble
		je     jump_ebe06
		xor    %eax,%eax
		mov    %si,%ax
		call   is_daudio_track_
		test   %al,%al
		je     jump_ebe06
		mov    %esi,%eax
		call   GetCDTrackStartSector_
		mov    %eax,%edi
		mov    %esi,%eax
		call   GetCDTrackLength_
		mov    %eax,%ebx
		xor    %eax,%eax
		mov    CurrentCDTrack,%ax
		test   %eax,%eax
		je     jump_ebd64
		xor    %eax,%eax
		mov    CurrentCDTrack,%ax
		cmp    %esi,%eax
		je     jump_ebe06
	jump_ebd64:
		xor    %eax,%eax
		mov    CDTimerActive,%al
		cmp    $0x1,%eax
		jne    jump_ebd87
		mov    CDCount_handle,%edx
		push   %edx
		call   AIL_release_timer_handle
		add    $0x4,%esp
		xor    %dl,%dl
		mov    %dl,CDTimerActive
	jump_ebd87:
		call   StopCD_
		mov    $0x4b,%ecx
		mov    %ebx,%eax
		xor    %edx,%edx
		div    %ecx
		mov    $0x5,%ecx
		xor    %edx,%edx
		mov    %eax,TrackLength
		div    %ecx
		mov    %eax,%edx
		lea    0x0(,%eax,4),%eax
		add    %edx,%eax
		add    %ecx,%eax
		push   $cbCDCountdown
		mov    %eax,CDCountdown
		call   AIL_register_timer
		add    $0x4,%esp
		push   $0x4c4b40
		push   %eax
		mov    %eax,CDCount_handle
		call   AIL_set_timer_period
		add    $0x8,%esp
		call   GetCDFirst_
		and    $0xffff,%eax
		mov    %edi,%edx
		call   cd_play_
		mov    CDCount_handle,%ebx
		push   %ebx
		call   AIL_start_timer
		mov    $0x1,%dh
		add    $0x4,%esp
		mov    %dh,CDTimerActive
		mov    %si,CurrentCDTrack
	jump_ebe06:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (StopCD_)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		cmpb   $0x0,CDAble
		je     jump_ebfa3
		cmpb   $0x0,CDTimerActive
		je     jump_ebf8b
		xor    %dh,%dh
		mov    %dh,CDTimerActive
		mov    CDCount_handle,%edx
		push   %edx
		call   AIL_release_timer_handle
		add    $0x4,%esp
	jump_ebf8b:
		xor    %edx,%edx
		mov    %dx,CurrentCDTrack
		call   GetCDFirst_
		and    $0xffff,%eax
		call   cd_stop_
	jump_ebfa3:
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetCDVolume_)
/*----------------------------------------------------------------*/
		push   %edx
		cmpb   $0x0,CDAble
		je     jump_ec07d
		movsbl data_158b34,%edx
		cmp    $0xffffffff,%edx
		je     jump_ec07d
		xor    %edx,%edx
		mov    %al,%dl
		cmp    $0x7f,%edx
		jle    jump_ec071
		mov    $0x7f,%al
	jump_ec071:
		add    %al,%al
		and    $0xff,%eax
		call   SetCDAudioVolume_
	jump_ec07d:
		pop    %edx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetCDVolume_)
/*----------------------------------------------------------------*/
		cmpb   $0x0,CDAble
		jne    GetCDAudioVolume_
		mov    $0xff,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (FreeCD_)
/*----------------------------------------------------------------*/
		movsbl data_158b34,%eax
		cmp    $0xffffffff,%eax
		je     FreeCDAudio_
		xor    %eax,%eax
		mov    data_158b34,%al
		call   SetCDVolume_
		jmp    FreeCDAudio_


/*----------------------------------------------------------------*/
GLOBAL_FUNC (StopMusicIfActive_)
/*----------------------------------------------------------------*/
		push   %ebx
		cmpb   $0x0,MusicInstalled
		je     jump_ec269
		cmpb   $0x0,MusicAble
		je     jump_ec269
		cmpb   $0x0,MusicActive
		je     jump_ec269
		xor    %bl,%bl
		call   StopMusic_
		mov    %bl,MusicActive
	jump_ec269:
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetSoundAble_)
/*----------------------------------------------------------------*/
		mov    SoundAble,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetMusicAble_)
/*----------------------------------------------------------------*/
		mov    MusicAble,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetSoundActive_)
/*----------------------------------------------------------------*/
		mov    SoundActive,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetSoundInstalled_)
/*----------------------------------------------------------------*/
		mov    SoundInstalled,%al
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetSoundDriver_)
/*----------------------------------------------------------------*/
		mov    SoundDriver,%eax
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetFirstSampleInfoStructure_)
/*----------------------------------------------------------------*/
		mov    $sample_id,%eax
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (GetLastSampleInfoStructure_)
/*----------------------------------------------------------------*/
		mov    end_sample_id,%eax
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LoadSounds_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		sub    $0x20,%esp
		mov    %al,0x1c(%esp)
		movzbw %al,%di
		xor    %eax,%eax
		mov    0x1c(%esp),%al
		push   %eax
		push   $data_15ee04
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,SoundInstalled
		je     jump_ec8ae
		cmpb   $0x0,SoundAble
		je     jump_ec8ae
		xor    %eax,%eax
		movsbl CurrentSoundBank,%edx
		mov    %di,%ax
		cmp    %eax,%edx
		je     jump_ec8ae
		cmpb   $0x0,DisableLoadSounds
		jne    jump_ec8ae
		call   StopAllSamples_
		mov    $0x2,%edx
		mov    $full_sound_data_path,%eax
		call   FileOpenInclCD
		mov    %eax,%ecx
		mov    %eax,%esi
		cmp    $0xffffffff,%eax
		jne    jump_ec763
		jmp    jump_ec8a7
	jump_ec6f0:
		xor    %edx,%edx
		mov    %edx,0x18(%esp)
		jmp    jump_ec815
	jump_ec6fb:
		movl   $0x1,0x18(%esp)
		jmp    jump_ec815
	jump_ec708:
		movl   $0x2,0x18(%esp)
		jmp    jump_ec815
	jump_ec715:
		movl   $0x3,0x18(%esp)
		jmp    jump_ec815
	jump_ec722:
		movl   $0x4,0x18(%esp)
		jmp    jump_ec815
	jump_ec72f:
		movl   $0x5,0x18(%esp)
		jmp    jump_ec815
	jump_ec73c:
		movl   $0x6,0x18(%esp)
		jmp    jump_ec815
	jump_ec749:
		movl   $0x7,0x18(%esp)
		jmp    jump_ec815
	jump_ec756:
		movl   $0x8,0x18(%esp)
		jmp    jump_ec815
	jump_ec763:
		mov    $full_sound_data_path,%eax
		call   ac_LbFileLength
		lea    -0x4(%eax),%edx
		xor    %ebx,%ebx
		mov    %ecx,%eax
		call   LbFileSeek_
		mov    $0x4,%ebx
		lea    0x14(%esp),%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    0x14(%esp),%edx
		mov    %ecx,%eax
		xor    %ebx,%ebx
		call   LbFileSeek_
		mov    $0x12,%ebx
		mov    %esp,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    SoundType,%ax
		inc    %edi
		cmp    $0x64b,%ax
		jb     jump_ec7e7
		jbe    jump_ec722
		cmp    $0x656,%ax
		jb     jump_ec7db
		jbe    jump_ec708
		cmp    $0x668,%ax
		jb     jump_ec815
		jbe    jump_ec6fb
		cmp    $0x66c,%ax
		je     jump_ec6f0
		jmp    jump_ec815
	jump_ec7db:
		cmp    $0x654,%ax
		je     jump_ec715
		jmp    jump_ec815
	jump_ec7e7:
		cmp    $0x32b,%ax
		jb     jump_ec80b
		jbe    jump_ec749
		cmp    $0x336,%ax
		jb     jump_ec815
		jbe    jump_ec73c
		cmp    $0x64a,%ax
		je     jump_ec72f
		jmp    jump_ec815
	jump_ec80b:
		cmp    $0x320,%ax
		je     jump_ec756
	jump_ec815:
		mov    0x18(%esp),%eax
		cmp    (%esp,%eax,2),%di
		jbe    jump_ec830
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15ee1f
		jmp    jump_ec8e3
	jump_ec830:
		dec    %edi
		xor    %ecx,%ecx
		mov    %di,%cx
		lea    0x0(,%ecx,8),%edx
		mov    $0x1,%ebx
		add    %ecx,%edx
		mov    %esi,%eax
		shl    $0x4,%edx
		call   LbFileSeek_
		xor    %edx,%edx
		mov    %esi,%eax
		mov    0x18(%esp),%dl
		call   load_sound_bank
		test   %al,%al
		jne    jump_ec870
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15ee51
		jmp    jump_ec8e3
	jump_ec870:
		mov    0x1c(%esp),%al
		mov    %al,CurrentSoundBank
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15ee84
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		add    $0x20,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ec8a7:
		push   $data_15eea5
		jmp    jump_ec8e3
	jump_ec8ae:
		cmpb   $0x0,SoundInstalled
		jne    jump_ec8be
		push   $data_15eed5
		jmp    jump_ec8e3
	jump_ec8be:
		cmpb   $0x0,SoundAble
		jne    jump_ec8ce
		push   $data_15ef0c
		jmp    jump_ec8e3
	jump_ec8ce:
		cmpb   $0x0,DisableLoadSounds
		je     jump_ec8de
		push   $data_15ef3d
		jmp    jump_ec8e3
	jump_ec8de:
		push   $data_15ef74
	jump_ec8e3:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		mov    $0x1,%eax
		add    $0x20,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void format_sounds(void)
 */
format_sounds:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    NumberOfSamples,%bx
		mov    EndSfxs,%ecx
		mov    SfxData,%esi
		mov    Sfx,%edx
		test   %edx,%edx
		je     jump_ec94f
		test   %esi,%esi
		je     jump_ec94f
		lea    0x20(%edx),%eax
		xor    %ebx,%ebx
		cmp    %ecx,%eax
		jae    jump_ec94f
	jump_ec93f:
		mov    0x12(%eax),%edi
		add    $0x20,%eax
		add    %esi,%edi
		inc    %ebx
		mov    %edi,-0xe(%eax)
		cmp    %ecx,%eax
		jb     jump_ec93f
	jump_ec94f:
		mov    %esi,SfxData
		mov    %ecx,EndSfxs
		mov    %bx,NumberOfSamples
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* unsigned char load_sound_bank(long, unsigned char)
 */
load_sound_bank:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x94,%esp
		mov    %eax,%ecx
		mov    %dl,0x90(%esp)
		call   LbFilePosition_
		mov    $0x90,%ebx
		mov    %esp,%edx
		movzbl 0x90(%esp),%ebp
		mov    %ecx,%eax
		shl    $0x4,%ebp
		call   LbFileRead_
		cmpl   $0xffffffff,0x4(%esp,%ebp,1)
		je     jump_ecb73
		xor    %ah,%ah
		mov    SfxData,%ebx
		mov    %ah,SoundAble
		test   %ebx,%ebx
		je     jump_ecb73
		cmpl   $0x0,Sfx
		je     jump_ecb73
		mov    largest_dat_size,%ebx
		mov    SfxData,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    largest_tab_size,%ebx
		mov    Sfx,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    Sfx,%esi
		mov    0x8(%esp,%ebp,1),%eax
		lea    (%esi,%eax,1),%edx
		xor    %ebx,%ebx
		mov    %edx,EndSfxs
		mov    %ecx,%eax
		mov    0x4(%esp,%ebp,1),%edx
		mov    SfxData,%edi
		call   LbFileSeek_
		mov    $0x8,%ebx
		mov    SfxData,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		xor    %eax,%eax
		mov    (%edi),%al
		cmp    $0x52,%eax
		jne    jump_eca8a
		xor    %eax,%eax
		mov    0x1(%edi),%al
		cmp    $0x4e,%eax
		jne    jump_eca8a
		xor    %eax,%eax
		mov    0x2(%edi),%al
		cmp    $0x43,%eax
		jne    jump_eca8a
		xor    %eax,%eax
		xor    %edx,%edx
		mov    0x4(%edi),%al
		mov    0x5(%edi),%dl
		shl    $0x8,%eax
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x6(%edi),%dl
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x7(%edi),%dl
		add    %edx,%eax
		mov    SfxData,%edx
		lea    -0x8(%eax),%ebx
		add    $0x8,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    %edi,%edx
		mov    %edi,%eax
		call   UnpackM1_
		jmp    jump_ecaad
	jump_eca8a:
		xor    %ebx,%ebx
		mov    0x90(%esp),%bl
		mov    SfxData,%edx
		shl    $0x4,%ebx
		mov    %ecx,%eax
		mov    0xc(%esp,%ebx,1),%ebx
		add    $0x8,%edx
		sub    $0x8,%ebx
		call   LbFileRead_
	jump_ecaad:
		xor    %eax,%eax
		mov    0x90(%esp),%al
		shl    $0x4,%eax
		xor    %ebx,%ebx
		mov    (%esp,%eax,1),%edx
		mov    %ecx,%eax
		call   LbFileSeek_
		mov    $0x8,%ebx
		mov    Sfx,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		xor    %eax,%eax
		mov    (%esi),%al
		cmp    $0x52,%eax
		jne    jump_ecb35
		xor    %eax,%eax
		mov    0x1(%esi),%al
		cmp    $0x4e,%eax
		jne    jump_ecb35
		xor    %eax,%eax
		mov    0x2(%esi),%al
		cmp    $0x43,%eax
		jne    jump_ecb35
		xor    %eax,%eax
		xor    %edx,%edx
		mov    0x4(%esi),%al
		mov    0x5(%esi),%dl
		shl    $0x8,%eax
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x6(%esi),%dl
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x7(%esi),%dl
		add    %edx,%eax
		mov    Sfx,%edx
		lea    -0x8(%eax),%ebx
		add    $0x8,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    %esi,%edx
		mov    %esi,%eax
		call   UnpackM1_
		jmp    jump_ecb58
	jump_ecb35:
		xor    %eax,%eax
		mov    0x90(%esp),%al
		shl    $0x4,%eax
		mov    Sfx,%edx
		mov    0x8(%esp,%eax,1),%ebx
		add    $0x8,%edx
		sub    $0x8,%ebx
		mov    %ecx,%eax
		call   LbFileRead_
	jump_ecb58:
		mov    $0x1,%dl
		call   format_sounds
		mov    %dl,SoundAble
		mov    %dl,%al
		add    $0x94,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret
	jump_ecb73:
		xor    %al,%al
		add    $0x94,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
AllocateSoundBankMemory_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x4,%esp
		mov    %eax,%ecx
		push   $SoundAndMusicDataPath
		push   $data_15efa6
		push   $full_sound_data_path
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x2,%edx
		mov    $full_sound_data_path,%eax
		call   FileOpenInclCD
		mov    %eax,%ebp
		cmp    $0xffffffff,%eax
		jne    jump_ecc6f
		add    $0x4,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ecbd6:
		mov    sound_bank_size_info_Bnk0Fld0,%esi
		mov    sound_bank_size_info_Bnk0Fld1,%edi
		jmp    jump_ecd39
	jump_ecbe7:
		mov    sound_bank_size_info_Bnk1Fld0,%esi
		mov    sound_bank_size_info_Bnk1Fld1,%edi
		jmp    jump_ecd39
	jump_ecbf8:
		mov    sound_bank_size_info_Bnk2Fld0,%esi
		mov    sound_bank_size_info_Bnk2Fld1,%edi
		jmp    jump_ecd39
	jump_ecc09:
		mov    sound_bank_size_info_Bnk3Fld0,%esi
		mov    sound_bank_size_info_Bnk3Fld1,%edi
		jmp    jump_ecd39
	jump_ecc1a:
		mov    sound_bank_size_info_Bnk4Fld0,%esi
		mov    sound_bank_size_info_Bnk4Fld1,%edi
		jmp    jump_ecd39
	jump_ecc2b:
		mov    sound_bank_size_info_Bnk5Fld0,%esi
		mov    sound_bank_size_info_Bnk5Fld1,%edi
		jmp    jump_ecd39
	jump_ecc3c:
		mov    sound_bank_size_info_Bnk6Fld0,%esi
		mov    sound_bank_size_info_Bnk6Fld1,%edi
		jmp    jump_ecd39
	jump_ecc4d:
		mov    sound_bank_size_info_Bnk7Fld0,%esi
		mov    sound_bank_size_info_Bnk7Fld1,%edi
		jmp    jump_ecd39
	jump_ecc5e:
		mov    sound_bank_size_info_Bnk8Fld0,%esi
		mov    sound_bank_size_info_Bnk8Fld1,%edi
		jmp    jump_ecd39
	jump_ecc6f:
		mov    $full_sound_data_path,%eax
		mov    $0x48,%ebx
		call   ac_LbFileLength
		xor    %edx,%edx
		mov    %eax,(%esp)
		mov    $sound_bank_size_info_Bnk0Fld0,%eax
		call   ac_memset
		mov    $0x2,%ebx
		mov    %ebp,%eax
		xor    %edx,%edx
		call   LbFileSeek_
		mov    (%esp),%edx
		mov    %ebp,%eax
		xor    %ebx,%ebx
		sub    $0x4c,%edx
		call   LbFileSeek_
		mov    $0x48,%ebx
		mov    $sound_bank_size_info_Bnk0Fld0,%edx
		mov    %ebp,%eax
		xor    %edi,%edi
		call   LbFileRead_
		mov    %ebp,%eax
		xor    %esi,%esi
		call   LbFileClose_
		cmp    $0x64b,%cx
		jb     jump_ecd07
		jbe    jump_ecc1a
		cmp    $0x656,%cx
		jb     jump_eccfa
		jbe    jump_ecbf8
		cmp    $0x668,%cx
		jb     jump_ecd39
		jbe    jump_ecbe7
		cmp    $0x66c,%cx
		je     jump_ecbd6
		jmp    jump_ecd39
	jump_eccfa:
		cmp    $0x654,%cx
		je     jump_ecc09
		jmp    jump_ecd39
	jump_ecd07:
		cmp    $0x32b,%cx
		jb     jump_ecd2e
		jbe    jump_ecc4d
		cmp    $0x336,%cx
		jb     jump_ecd39
		jbe    jump_ecc3c
		cmp    $0x64a,%cx
		je     jump_ecc2b
		jmp    jump_ecd39
	jump_ecd2e:
		cmp    $0x320,%cx
		je     jump_ecc5e
	jump_ecd39:
		test   %esi,%esi
		je     jump_ecdee
		test   %edi,%edi
		je     jump_ecdee
		lea    0x100(%esi),%ebp
		mov    %ebp,%eax
		call   LbMemoryAlloc_
		push   %ebp
		push   $data_15efb3
		push   $SoundProgressMessage
		mov    %eax,SfxData
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		lea    0x100(%edi),%eax
		call   LbMemoryAlloc_
		mov    SfxData,%edx
		mov    %eax,Sfx
		test   %edx,%edx
		je     jump_ecd96
		test   %eax,%eax
		jne    jump_ecdb6
	jump_ecd96:
		mov    SfxData,%eax
		call   LbMemoryFree_
		mov    Sfx,%eax
		call   LbMemoryFree_
		xor    %eax,%eax
		add    $0x4,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ecdb6:
		mov    %esi,largest_dat_size
		mov    %edx,%eax
		mov    %esi,%ebx
		xor    %edx,%edx
		mov    %edi,largest_tab_size
		call   ac_memset
		mov    largest_tab_size,%ebx
		mov    Sfx,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0x1,%eax
		add    $0x4,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ecdee:
		mov    $0xfffffffe,%eax
		add    $0x4,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (LoadMusic_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		sub    $0x10,%esp
		mov    %eax,%edi
		xor    %eax,%eax
		mov    %di,%ax
		push   %eax
		push   $data_15efcc
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,MusicInstalled
		je     jump_ecfef
		cmpb   $0x0,MusicAble
		je     jump_ecfef
		xor    %eax,%eax
		mov    DisableLoadMusic,%al
		test   %eax,%eax
		jne    jump_ecfef
		call   StopMusic_
		mov    $0x2,%edx
		mov    $full_music_data_path,%eax
		call   FileOpenInclCD
		mov    %eax,%ecx
		mov    %eax,%esi
		cmp    $0xffffffff,%eax
		jne    jump_eced9
		jmp    jump_ecfe8
	jump_ecea7:
		xor    %ecx,%ecx
		mov    %ecx,0xc(%esp)
		jmp    jump_ecf72
	jump_eceb2:
		movl   $0x1,0xc(%esp)
		jmp    jump_ecf72
	jump_ecebf:
		movl   $0x2,0xc(%esp)
		jmp    jump_ecf72
	jump_ececc:
		movl   $0x3,0xc(%esp)
		jmp    jump_ecf72
	jump_eced9:
		mov    $0x2,%ebx
		xor    %edx,%edx
		call   LbFileSeek_
		mov    %ecx,%eax
		call   LbFilePosition_
		lea    -0x4(%eax),%edx
		xor    %ebx,%ebx
		mov    %ecx,%eax
		call   LbFileSeek_
		mov    $0x4,%ebx
		lea    0x8(%esp),%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    0x8(%esp),%edx
		mov    %ecx,%eax
		xor    %ebx,%ebx
		call   LbFileSeek_
		mov    $0x8,%ebx
		mov    %esp,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    MusicType,%cl
		inc    %edi
		cmp    $0x57,%cl
		jb     jump_ecf53
		jbe    jump_ececc
		cmp    $0x67,%cl
		jb     jump_ecf4e
		jbe    jump_ecea7
		cmp    $0x72,%cl
		jb     jump_ecf72
		jbe    jump_eceb2
		cmp    $0x77,%cl
		je     jump_ececc
		jmp    jump_ecf72
	jump_ecf4e:
		cmp    $0x66,%cl
		jmp    jump_ecf6c
	jump_ecf53:
		cmp    $0x47,%cl
		jb     jump_ecf69
		jbe    jump_ecea7
		cmp    $0x52,%cl
		je     jump_eceb2
		jmp    jump_ecf72
	jump_ecf69:
		cmp    $0x46,%cl
	jump_ecf6c:
		je     jump_ecebf
	jump_ecf72:
		mov    0xc(%esp),%eax
		cmp    (%esp,%eax,2),%di
		jbe    jump_ecf8d
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15efe7
		jmp    jump_ed01d
	jump_ecf8d:
		dec    %edi
		xor    %edx,%edx
		mov    $0x1,%ebx
		mov    %di,%dx
		mov    %esi,%eax
		shl    $0x6,%edx
		call   LbFileSeek_
		xor    %edx,%edx
		mov    %esi,%eax
		mov    0xc(%esp),%dl
		call   load_music_bank
		test   %al,%al
		jne    jump_ecfc1
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15f019
		jmp    jump_ed01d
	jump_ecfc1:
		mov    %esi,%eax
		call   LbFileClose_
		push   $data_15f04c
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		jmp    jump_ed039
	jump_ecfe8:
		push   $data_15f06d
		jmp    jump_ed01d
	jump_ecfef:
		cmpb   $0x0,MusicInstalled
		jne    jump_ecfff
		push   $data_15f09d
		jmp    jump_ed01d
	jump_ecfff:
		cmpb   $0x0,MusicAble
		jne    jump_ed00f
		push   $data_15f0d4
		jmp    jump_ed01d
	jump_ed00f:
		cmpb   $0x0,DisableLoadMusic
		je     jump_ed034
		push   $data_15f105
	jump_ed01d:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_ed034:
		mov    $0x1,%eax
	jump_ed039:
		add    $0x10,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void format_music(void)
 */
format_music:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		mov    NumberOfSongs,%bx
		mov    EndMusic,%ecx
		mov    MusicData,%esi
		mov    Music,%edx
		test   %edx,%edx
		je     jump_ed08f
		test   %esi,%esi
		je     jump_ed08f
		lea    0x20(%edx),%eax
		xor    %ebx,%ebx
		cmp    %ecx,%eax
		jae    jump_ed08f
	jump_ed07f:
		mov    0x12(%eax),%edi
		add    $0x20,%eax
		add    %esi,%edi
		inc    %ebx
		mov    %edi,-0xe(%eax)
		cmp    %ecx,%eax
		jb     jump_ed07f
	jump_ed08f:
		mov    %esi,MusicData
		mov    %ecx,EndMusic
		mov    %bx,NumberOfSongs
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* uint8_t load_music_bank(long a1, uint8_t a2)
 */
load_music_bank:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x44,%esp
		mov    %eax,%ecx
		mov    %dl,0x40(%esp)
		call   LbFilePosition_
		mov    $0x40,%ebx
		mov    %esp,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		xor    %eax,%eax
		mov    0x40(%esp),%al
		shl    $0x4,%eax
		cmpl   $0xffffffff,0x4(%esp,%eax,1)
		je     jump_ed299
		mov    MusicData,%ebx
		test   %ebx,%ebx
		je     jump_ed299
		mov    Music,%esi
		test   %esi,%esi
		je     jump_ed299
		mov    0x8(%esp,%eax,1),%edx
		mov    %ebx,%edi
		lea    (%esi,%edx,1),%ebx
		mov    %ebx,EndMusic
		mov    0x4(%esp,%eax,1),%edx
		mov    %ecx,%eax
		xor    %ebx,%ebx
		call   LbFileSeek_
		mov    $0x8,%ebx
		mov    %edi,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		xor    %eax,%eax
		mov    (%edi),%al
		cmp    $0x52,%eax
		jne    jump_ed189
		xor    %eax,%eax
		mov    0x1(%edi),%al
		cmp    $0x4e,%eax
		jne    jump_ed189
		xor    %eax,%eax
		mov    0x2(%edi),%al
		cmp    $0x43,%eax
		jne    jump_ed189
		xor    %eax,%eax
		xor    %edx,%edx
		mov    0x4(%edi),%al
		mov    0x5(%edi),%dl
		shl    $0x8,%eax
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x6(%edi),%dl
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x7(%edi),%dl
		add    %edx,%eax
		mov    MusicData,%edx
		lea    -0x8(%eax),%ebx
		add    $0x8,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    %edi,%edx
		mov    %edi,%eax
		call   UnpackM1_
		jmp    jump_ed1a9
	jump_ed189:
		xor    %eax,%eax
		mov    0x40(%esp),%al
		shl    $0x4,%eax
		mov    MusicData,%edx
		mov    0xc(%esp,%eax,1),%ebx
		add    $0x8,%edx
		sub    $0x8,%ebx
		mov    %ecx,%eax
		call   LbFileRead_
	jump_ed1a9:
		xor    %edx,%edx
		mov    0x40(%esp),%dl
		mov    %ecx,%eax
		shl    $0x4,%edx
		xor    %ebx,%ebx
		mov    (%esp,%edx,1),%edx
		call   LbFileSeek_
		mov    $0x8,%ebx
		mov    %esi,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		xor    %eax,%eax
		mov    (%esi),%al
		cmp    $0x52,%eax
		jne    jump_ed22a
		xor    %eax,%eax
		mov    0x1(%esi),%al
		cmp    $0x4e,%eax
		jne    jump_ed22a
		xor    %eax,%eax
		mov    0x2(%esi),%al
		cmp    $0x43,%eax
		jne    jump_ed22a
		xor    %eax,%eax
		xor    %edx,%edx
		mov    0x4(%esi),%al
		mov    0x5(%esi),%dl
		shl    $0x8,%eax
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x6(%esi),%dl
		add    %edx,%eax
		xor    %edx,%edx
		shl    $0x8,%eax
		mov    0x7(%esi),%dl
		add    %edx,%eax
		mov    Music,%edx
		lea    -0x8(%eax),%ebx
		add    $0x8,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    %esi,%edx
		mov    %esi,%eax
		call   UnpackM1_
		jmp    jump_ed24a
	jump_ed22a:
		xor    %ebx,%ebx
		mov    0x40(%esp),%bl
		mov    Music,%edx
		shl    $0x4,%ebx
		mov    %ecx,%eax
		mov    0x8(%esp,%ebx,1),%ebx
		add    $0x8,%edx
		sub    $0x8,%ebx
		call   LbFileRead_
	jump_ed24a:
		call   format_music
		mov    $0x1,%bl
		jmp    jump_ed275
	jump_ed253:
		mov    %eax,%edx
		mov    Music,%eax
		shl    $0x5,%edx
		push   $0x0
		mov    0x12(%edx,%eax,1),%edi
		push   %edi
		mov    SongHandle,%ebp
		push   %ebp
		inc    %bl
		call   AIL_init_sequence
		add    $0xc,%esp
	jump_ed275:
		xor    %edx,%edx
		xor    %eax,%eax
		mov    NumberOfSongs,%dx
		mov    %bl,%al
		cmp    %edx,%eax
		jle    jump_ed253
		mov    $0x1,%ah
		mov    %ah,MusicAble
		mov    %ah,%al
		add    $0x44,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret
	jump_ed299:
		xor    %al,%al
		add    $0x44,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
AllocateMusicBankMemory_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		push   $SoundAndMusicDataPath
		push   $data_15f13f
		push   $full_music_data_path
		call   ac_sprintf
		add    $0xc,%esp
		mov    $0x2,%edx
		mov    $full_music_data_path,%eax
		call   FileOpenInclCD
		mov    %eax,%ecx
		cmp    $0xffffffff,%eax
		jne    jump_ed32c
		jmp    jump_ed45d
	jump_ed2e8:
		mov    music_bank_size_info__Field4,%esi
		mov    music_bank_size_info__Field5,%edi
		jmp    jump_ed3dc
	jump_ed2f9:
		mov    music_bank_size_info__Field2,%esi
		mov    music_bank_size_info__Field3,%edi
		jmp    jump_ed3dc
	jump_ed30a:
		mov    music_bank_size_info__Field0,%esi
		mov    music_bank_size_info__Field1,%edi
		jmp    jump_ed3dc
	jump_ed31b:
		mov    music_bank_size_info__Field6,%esi
		mov    music_bank_size_info__Field7,%edi
		jmp    jump_ed3dc
	jump_ed32c:
		mov    $0x20,%ebx
		mov    $music_bank_size_info__Field0,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0x2,%ebx
		mov    %ecx,%eax
		xor    %edx,%edx
		call   LbFileSeek_
		mov    %ecx,%eax
		mov    $0x2,%ebx
		call   LbFilePosition_
		xor    %edx,%edx
		mov    %eax,%ebp
		mov    %ecx,%eax
		call   LbFileSeek_
		lea    -0x24(%ebp),%edx
		mov    %ecx,%eax
		xor    %ebx,%ebx
		call   LbFileSeek_
		mov    $0x20,%ebx
		mov    $music_bank_size_info__Field0,%edx
		mov    %ecx,%eax
		call   LbFileRead_
		mov    %ecx,%eax
		xor    %edi,%edi
		call   LbFileClose_
		mov    MusicType,%cl
		xor    %esi,%esi
		cmp    $0x57,%cl
		jb     jump_ed3bd
		jbe    jump_ed31b
		cmp    $0x67,%cl
		jb     jump_ed3b8
		jbe    jump_ed2e8
		cmp    $0x72,%cl
		jb     jump_ed3dc
		jbe    jump_ed2f9
		cmp    $0x77,%cl
		je     jump_ed31b
		jmp    jump_ed3dc
	jump_ed3b8:
		cmp    $0x66,%cl
		jmp    jump_ed3d6
	jump_ed3bd:
		cmp    $0x47,%cl
		jb     jump_ed3d3
		jbe    jump_ed2e8
		cmp    $0x52,%cl
		je     jump_ed2f9
		jmp    jump_ed3dc
	jump_ed3d3:
		cmp    $0x46,%cl
	jump_ed3d6:
		je     jump_ed30a
	jump_ed3dc:
		test   %esi,%esi
		je     jump_ed458
		test   %edi,%edi
		je     jump_ed458
		lea    0x100(%esi),%eax
		call   LbMemoryAlloc_
		mov    %eax,MusicData
		lea    0x100(%edi),%eax
		call   LbMemoryAlloc_
		mov    MusicData,%edx
		mov    %eax,Music
		test   %edx,%edx
		je     jump_ed416
		test   %eax,%eax
		jne    jump_ed433
	jump_ed416:
		mov    MusicData,%eax
		call   LbMemoryFree_
		mov    Music,%eax
		call   LbMemoryFree_
		xor    %eax,%eax
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ed433:
		mov    %edx,%eax
		mov    %esi,%ebx
		xor    %edx,%edx
		call   ac_memset
		mov    Music,%eax
		mov    %edi,%ebx
		xor    %edx,%edx
		call   ac_memset
		mov    $0x1,%eax
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ed458:
		mov    $0xfffffffe,%eax
	jump_ed45d:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* void FreeAudio(void)
 */
GLOBAL_FUNC (FreeAudio_)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		cmpb   $0x0,CDAble
		je     jump_ed4c7
		call   FreeCD_
		call   GetSoundAble_
		test   %al,%al
		jne    jump_ed4c7
		call   GetMusicAble_
		test   %al,%al
		jne    jump_ed4c7
		call   ail_shutdown
	jump_ed4c7:
		cmpb   $0x0,StreamedSoundAble
		je     jump_ed4d5
		call   FreeStreamedSound_
	jump_ed4d5:
		call   FreeMusic_
		call   FreeSound_
		cmpb   $0x0,sb16_mixer_set
		je     jump_ed4ed
		call   reset_SB16_volumes
	jump_ed4ed:
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetMusicMasterVolume_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    %eax,%ebx
		mov    MusicInstalled,%ah
		test   %ah,%ah
		je     jump_ee6f3
		cmpb   $0x0,MusicAble
		je     jump_ee6f3
		test   %ah,%ah
		je     jump_ee6f3
		cmp    CurrentMusicMasterVolume,%ebx
		je     jump_ee6f3
		cmp    $0x7f,%ebx
		jg     jump_ee6f3
		test   %ebx,%ebx
		jl     jump_ee6f3
		push   %ebx
		mov    MusicDriver,%ecx
		push   %ecx
		call   AIL_set_XMIDI_master_volume
		add    $0x8,%esp
		mov    %ebx,CurrentMusicMasterVolume
	jump_ee6f3:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSoundMasterVolume_)	/* 0xEE700 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    %eax,%ebx
		cmpb   $0x0,SoundAble
		je     jump_ee73e
		cmpb   $0x0,SoundInstalled
		je     jump_ee73e
		cmp    CurrentSoundMasterVolume,%ebx
		je     jump_ee73e
		cmp    $0x7f,%ebx
		jg     jump_ee73e
		test   %ebx,%ebx
		jl     jump_ee73e
		push   %ebx
		mov    SoundDriver,%ecx
		push   %ecx
		call   ail_set_digital_master_volume
		add    $0x8,%esp
		mov    %ebx,CurrentSoundMasterVolume
	jump_ee73e:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (IsSamplePlaying_)	/* 0xee750 */
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		mov    %eax,%esi
		mov    %edx,%edi
		cmpb   $0x0,SoundInstalled
		je     jump_ee828
		cmpb   $0x0,SoundAble
		je     jump_ee828
		cmpb   $0x0,SoundActive
		je     jump_ee828
		movswl %di,%eax
		test   %eax,%eax
		jne    jump_ee7a3
		test   %esi,%esi
		jne    jump_ee7a3
		push   %ebx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_ee828
		mov    $0x1,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_ee7a3:
		movswl %di,%eax
		test   %eax,%eax
		jne    jump_ee7e5
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_ee7c9
		jmp    jump_ee828
	jump_ee7bc:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_ee828
	jump_ee7c9:
		cmp    0xe(%ebx),%esi
		jne    jump_ee7bc
		mov    (%ebx),%ebp
		push   %ebp
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_ee7bc
		mov    $0x1,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_ee7e5:
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_ee806
		xor    %al,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_ee7f9:
		mov    end_sample_id,%ebp
		add    $0x19,%ebx
		cmp    %ebp,%ebx
		ja     jump_ee828
	jump_ee806:
		cmp    0xe(%ebx),%esi
		jne    jump_ee7f9
		cmp    0x12(%ebx),%di
		jne    jump_ee7f9
		mov    (%ebx),%edx
		push   %edx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_ee7f9
		mov    $0x1,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_ee828:
		xor    %al,%al
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (DangerMusicFadeTick)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %esi
		push   %edi
		cmpb   $0x0,MusicInstalled
		je     jump_ef11b
		cmpb   $0x0,MusicAble
		je     jump_ef11b
		cmpb   $0x0,MusicActive
		je     jump_ef11b
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_ef11b
		mov    SongHandle,%edx
		push   %edx
		call   AIL_sequence_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_ef11b
		cmpb   $0x0,data_1e5eac
		je     jump_ef06c
		mov    DangerMusicFadeHandle,%ecx
		push   %ecx
		call   AIL_release_timer_handle
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ebx
		ret
	jump_ef06c:
		xor    %eax,%eax
		mov    DangerMusicVolume,%al
		cmp    $0x7f,%eax
		jne    jump_ef097
		xor    %eax,%eax
		mov    CurrentDangerMusicFadeDirection,%al
		cmp    $0x2,%eax
		jne    jump_ef097
		movb   $0x1,data_1e5eac
		xor    %cl,%cl
		mov    %cl,DangerMusicFadeActive
		pop    %edi
		pop    %esi
		pop    %ebx
		ret
	jump_ef097:
		xor    %eax,%eax
		mov    DangerMusicVolume,%al
		test   %eax,%eax
		jne    jump_ef0bf
		mov    CurrentDangerMusicFadeDirection,%al
		cmp    $0x1,%eax
		jne    jump_ef0bf
		movb   $0x1,data_1e5eac
		xor    %bl,%bl
		mov    %bl,DangerMusicFadeActive
		pop    %edi
		pop    %esi
		pop    %ebx
		ret
	jump_ef0bf:
		cmpb   $0x0,DangerMusicFadeActive
		je     jump_ef146
		mov    DangerMusicVolumeChange,%al
		add    %al,DangerMusicVolume
		xor    %bl,%bl
		jmp    jump_ef0dd
	jump_ef0db:
		inc    %bl
	jump_ef0dd:
		xor    %eax,%eax
		mov    %bl,%al
		cmp    $0x10,%eax
		jge    jump_ef146
		cmpb   $0x0,data_1e5edc(%eax)
		je     jump_ef0db
		xor    %eax,%eax
		mov    DangerMusicVolume,%al
		push   %eax
		mov    %bl,%al
		push   $0xb
		or     $0xb0,%al
		and    $0xff,%eax
		push   %eax
		mov    SongHandle,%esi
		push   %esi
		mov    MusicDriver,%edi
		push   %edi
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		jmp    jump_ef0db
	jump_ef11b:
		mov    DangerMusicFadeHandle,%ebx
		push   %ebx
		call   AIL_release_timer_handle
		add    $0x4,%esp
		xor    %bl,%bl
		mov    %bl,DangerMusicFadeActive
		movb   $0x1,CurrentDangerMusicFadeDirection
		mov    %bl,DangerMusicVolume
		movb   $0xff,DangerMusicVolumeChange
	jump_ef146:
		pop    %edi
		pop    %esi
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (DangerMusicFadeSwitch)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		mov    %al,%bl
		mov    %dl,%bh
		cmpb   $0x0,ive_got_an_awe32
		jne    jump_ef27c
		cmpb   $0x0,DangerMusicAble
		je     jump_ef27c
		cmpb   $0x0,MusicInstalled
		je     jump_ef27c
		cmpb   $0x0,MusicAble
		je     jump_ef27c
		cmpb   $0x0,MusicActive
		je     jump_ef27c
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_ef27c
		mov    SongHandle,%edx
		push   %edx
		call   AIL_sequence_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_ef27c
		xor    %edx,%edx
		mov    CurrentDangerMusicFadeDirection,%dl
		xor    %eax,%eax
		mov    %bl,%al
		cmp    %eax,%edx
		je     jump_ef27c
		cmpb   $0x0,DangerMusicFadeActive
		je     jump_ef1eb
		mov    DangerMusicFadeHandle,%ecx
		push   %ecx
		call   AIL_release_timer_handle
		add    $0x4,%esp
	jump_ef1eb:
		mov    %bl,CurrentDangerMusicFadeDirection
		negb   DangerMusicVolumeChange
		movb   $0x1,DangerMusicFadeActive
		mov    %bh,%bl
		and    $0xff,%ebx
		cmp    $0x4,%ebx
		jg     jump_ef248
		cmp    $0x1,%ebx
		jl     jump_ef248
		xor    %cl,%cl
		push   $DangerMusicFadeTick
		mov    %cl,data_1e5eac
		call   AIL_register_timer
		mov    %eax,%edx
		mov    %eax,DangerMusicFadeHandle
		mov    %ebx,%eax
		shl    $0x4,%eax
		sub    %ebx,%eax
		add    $0x4,%esp
		add    %eax,%eax
		push   %eax
		push   %edx
		call   AIL_set_timer_frequency
		add    $0x8,%esp
		mov    DangerMusicFadeHandle,%edi
		push   %edi
		jmp    jump_ef274
	jump_ef248:
		xor    %dh,%dh
		push   $DangerMusicFadeTick
		mov    %dh,data_1e5eac
		call   AIL_register_timer
		add    $0x4,%esp
		push   $0x1e
		push   %eax
		mov    %eax,DangerMusicFadeHandle
		call   AIL_set_timer_frequency
		add    $0x8,%esp
		mov    DangerMusicFadeHandle,%esi
		push   %esi
	jump_ef274:
		call   AIL_start_timer
		add    $0x4,%esp
	jump_ef27c:
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSampleTempo)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		mov    %eax,%ebx
		cmpb   $0x0,MusicInstalled
		je     jump_ef2dd
		cmpb   $0x0,MusicAble
		je     jump_ef2dd
		cmpb   $0x0,MusicActive
		je     jump_ef2dd
		xor    %eax,%eax
		mov    CurrentTempo,%ax
		cmp    %ebx,%eax
		je     jump_ef2dd
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_ef2dd
		push   %edx
		push   %ebx
		mov    SongHandle,%edx
		push   %edx
		call   AIL_set_sequence_tempo
		add    $0xc,%esp
		mov    %bx,CurrentTempo
	jump_ef2dd:
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSampleTempoNormal)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		cmpb   $0x0,MusicInstalled
		je     jump_ef334
		cmpb   $0x0,MusicAble
		je     jump_ef334
		cmpb   $0x0,MusicActive
		je     jump_ef334
		xor    %eax,%eax
		mov    CurrentTempo,%ax
		cmp    $0x64,%eax
		je     jump_ef334
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_ef334
		push   $0x0
		push   $0x64
		mov    SongHandle,%edx
		push   %edx
		mov    $0x64,%ebx
		call   AIL_set_sequence_tempo
		add    $0xc,%esp
		mov    %bx,CurrentTempo
	jump_ef334:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSampleRepeat)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		mov    %eax,%esi
		mov    %edx,%edi
		cmpb   $0x0,SoundInstalled
		je     jump_ef3aa
		cmpb   $0x0,SoundAble
		je     jump_ef3aa
		cmpb   $0x0,SoundActive
		je     jump_ef3aa
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_ef382
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret
	jump_ef375:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_ef3aa
	jump_ef382:
		cmp    0xe(%ebx),%esi
		jne    jump_ef375
		cmp    0x12(%ebx),%di
		jne    jump_ef375
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_ef375
		push   $0x1
		mov    (%ebx),%edx
		push   %edx
		call   ail_set_sample_loop_count
		add    $0x8,%esp
	jump_ef3aa:
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSamplePitch_)
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		mov    %eax,%ebp
		mov    %edx,%edi
		mov    %ebx,%esi
		cmpb   $0x0,SoundInstalled
		je     jump_ef451
		cmpb   $0x0,SoundAble
		je     jump_ef451
		cmpb   $0x0,SoundActive
		je     jump_ef451
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_ef400
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_ef3f3:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_ef451
	jump_ef400:
		cmp    0xe(%ebx),%ebp
		jne    jump_ef3f3
		cmp    0x12(%ebx),%di
		jne    jump_ef3f3
		mov    (%ebx),%edx
		push   %edx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_ef3f3
		movswl %si,%eax
		test   %eax,%eax
		jle    jump_ef3f3
		xor    %edx,%edx
		mov    0x8(%ebx),%dx
		cmp    %eax,%edx
		je     jump_ef3f3
		mov    SampleRate,%edx
		imul   %eax,%edx
		mov    $0x64,%ebp
		mov    %edx,%eax
		sar    $0x1f,%edx
		idiv   %ebp
		push   %eax
		mov    (%ebx),%ecx
		push   %ecx
		call   AIL_set_sample_playback_rate
		add    $0x8,%esp
		mov    %si,0x8(%ebx)
	jump_ef451:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (StopAllSamples_)	/* 0xEE910 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		cmpb   $0x0,SoundInstalled
		je     jump_ee970
		cmpb   $0x0,SoundAble
		je     jump_ee970
		cmpb   $0x0,SoundActive
		je     jump_ee970
		call   StopAllSampleFadeTimers_
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		ja     jump_ee96b
	jump_ee940:
		mov    (%ebx),%ecx
		push   %ecx
		add    $0x19,%ebx
		call   AIL_end_sample
		movw   $0x0,-0x7(%ebx)
		add    $0x4,%esp
		movl   $0x0,-0xb(%ebx)
		movb   $0x0,-0x5(%ebx)
		movb   $0x0,-0x4(%ebx)
		cmp    end_sample_id,%ebx
		jbe    jump_ee940
	jump_ee96b:
		call   StopSampleQueueList_
	jump_ee970:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetVolume_func_ee980) /* 0xee980 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		mov    %eax,%ebx
		cmpb   $0x0,MusicInstalled
		je     jump_ee9dc
		cmpb   $0x0,MusicAble
		je     jump_ee9dc
		cmpb   $0x0,MusicActive
		je     jump_ee9dc
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_ee9dc
		movzbl %dl,%esi
		cmp    $0x7f,%esi
		jg     jump_ee9dc
		mov    SongHandle,%edx
		push   %edx
		call   AIL_sequence_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_ee9dc
		xor    %eax,%eax
		mov    %bx,%ax
		push   %eax
		push   %esi
		mov    SongHandle,%ebx
		push   %ebx
		call   AIL_set_sequence_volume
		add    $0xc,%esp
	jump_ee9dc:
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
cb_get_trigger_info: /* 0xF0340 */
/*----------------------------------------------------------------*/
		push   %ebx
		push   %esi
		push   %edi
		push   %ebp
		mov    0x18(%esp),%ebx
		cmpl   $0x0,0x1c(%esp)
		jne    jump_f03ac
		movb   $0x1,DangerMusicAble
		mov    %ebx,%eax
		mov    ive_got_an_awe32,%dl
		or     $0xb0,%al
		test   %dl,%dl
		je     jump_f0389
		push   $0x7f
		push   $0x6b
		push   %eax
		mov    SongHandle,%edi
		push   %edi
		mov    MusicDriver,%ebp
		push   %ebp
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		xor    %cl,%cl
		mov    %cl,DangerMusicAble
		jmp    jump_f03ac
	jump_f0389:
		push   $0x0
		push   $0xb
		push   %eax
		mov    SongHandle,%ecx
		push   %ecx
		mov    MusicDriver,%esi
		push   %esi
		call   AIL_send_channel_voice_message
		mov    $0x1,%dh
		add    $0x14,%esp
		mov    %dh,data_1e5edc(%ebx)
	jump_f03ac:
		cmpl   $0x1,0x1c(%esp)
		jne    jump_f03ef
		mov    SongHandle,%edx
		push   %edx
		call   AIL_stop_sequence
		add    $0x4,%esp
		push   $0x1
		push   $0x0
		or     $0xb0,%bl
		push   %ebx
		mov    SongHandle,%ebx
		push   %ebx
		mov    MusicDriver,%ecx
		push   %ecx
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		mov    SongHandle,%esi
		push   %esi
		call   AIL_resume_sequence
		add    $0x4,%esp
	jump_f03ef:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (StartMusic_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x8,%esp
		mov    %eax,%esi
		mov    %dl,(%esp)
		cmpb   $0x0,MusicInstalled
		je     jump_f0656
		cmpb   $0x0,MusicAble
		je     jump_f0656
		cmpb   $0x0,MusicActive
		je     jump_f0656
		xor    %ebx,%ebx
		movswl %si,%eax
		mov    NumberOfSongs,%bx
		cmp    %ebx,%eax
		jg     jump_f0656
		xor    %ebx,%ebx
		mov    SongCurrentlyPlaying,%bx
		cmp    %eax,%ebx
		je     jump_f0656
		cmpb   $0x0,DangerMusicFadeActive
		je     jump_f0471
		mov    DangerMusicFadeHandle,%edx
		push   %edx
		call   AIL_release_timer_handle
		add    $0x4,%esp
	jump_f0471:
		xor    %bh,%bh
		mov    %bh,DangerMusicAble
		mov    %bh,DangerMusicFadeActive
		movb   $0x1,CurrentDangerMusicFadeDirection
		mov    %bh,DangerMusicVolume
		movb   $0xff,DangerMusicVolumeChange
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_f04d8
		mov    SongHandle,%ebx
		push   %ebx
		call   AIL_sequence_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_f04cf
		mov    SongHandle,%ecx
		push   %ecx
		call   AIL_stop_sequence
		add    $0x4,%esp
		mov    SongHandle,%edi
		push   %edi
		call   AIL_end_sequence
		add    $0x4,%esp
	jump_f04cf:
		xor    %ebx,%ebx
		mov    %bx,SongCurrentlyPlaying
	jump_f04d8:
		movswl %si,%ebx
		mov    Music,%eax
		shl    $0x5,%ebx
		push   $0x0
		mov    0x12(%ebx,%eax,1),%ebp
		push   %ebp
		mov    SongHandle,%eax
		push   %eax
		call   AIL_init_sequence
		add    $0xc,%esp
		push   $cb_get_trigger_info
		mov    SongHandle,%edx
		push   %edx
		call   AIL_register_trigger_callback
		add    $0x8,%esp
		xor    %dl,%dl
		mov    %dl,0x4(%esp)
		jmp    jump_f0609
	jump_f0517:
		xor    %dh,%dh
		push   $0x0
		mov    %dh,data_1e5edc(%eax)
		or     $0xb0,%al
		xor    %ebx,%ebx
		push   $0x0
		mov    %al,%bl
		push   %ebx
		mov    SongHandle,%edi
		push   %edi
		mov    MusicDriver,%ebp
		push   %ebp
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x0
		push   $0x7
		push   %ebx
		mov    SongHandle,%eax
		push   %eax
		mov    MusicDriver,%edx
		push   %edx
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x40
		mov    0x8(%esp),%al
		push   $0x0
		or     $0xe0,%al
		and    $0xff,%eax
		push   %eax
		mov    SongHandle,%ecx
		push   %ecx
		mov    MusicDriver,%edi
		push   %edi
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x7f
		push   $0xb
		push   %ebx
		mov    SongHandle,%ebp
		push   %ebp
		mov    MusicDriver,%eax
		push   %eax
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x0
		push   $0x1
		push   %ebx
		mov    SongHandle,%edx
		push   %edx
		mov    MusicDriver,%ecx
		push   %ecx
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x0
		push   $0x5b
		push   %ebx
		mov    SongHandle,%edi
		push   %edi
		mov    MusicDriver,%ebp
		push   %ebp
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x0
		push   $0x5d
		push   %ebx
		mov    SongHandle,%eax
		push   %eax
		mov    MusicDriver,%edx
		push   %edx
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		push   $0x0
		push   $0x6b
		push   %ebx
		mov    SongHandle,%ebx
		push   %ebx
		mov    MusicDriver,%ecx
		push   %ecx
		call   AIL_send_channel_voice_message
		add    $0x14,%esp
		incb   0x4(%esp)
	jump_f0609:
		xor    %eax,%eax
		mov    0x4(%esp),%al
		cmp    $0x10,%eax
		jl     jump_f0517
		xor    %eax,%eax
		mov    (%esp),%al
		cmp    $0x7f,%eax
		jge    jump_f0634
		push   $0x0
		push   %eax
		mov    SongHandle,%ebx
		push   %ebx
		call   AIL_set_sequence_volume
		add    $0xc,%esp
	jump_f0634:
		mov    SongHandle,%ecx
		push   %ecx
		call   AIL_start_sequence
		mov    $0x64,%ecx
		add    $0x4,%esp
		mov    %si,SongCurrentlyPlaying
		mov    %cx,CurrentTempo
	jump_f0656:
		add    $0x8,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC(SoundProgressLog_)
/*----------------------------------------------------------------*/
		cmpb   $0x0,DebugAudio
		je     jump_ff3e2
		push   %eax
		call   ac_printf
		add    $0x4,%esp
	jump_ff3e2:
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC(InitSound_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x28,%esp
		mov    SampleRate,%esi
		xor    %bl,%bl
		mov    SoundAble,%ah
		mov    $0x5,%bh
		test   %ah,%ah
		je     jump_ffa7c
		push   $data_15f804
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    SoundType,%ax
		push   %eax
		push   $data_15f817
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    SoundType,%ax
		test   %eax,%eax
		jne    jump_ff4c0
		cmpb   $0x0,MusicAble
		jne    jump_ff491
		cmpb   $0x0,AILStartupAlreadyInitiated
		je     jump_ff491
		call   ail_shutdown
		push   $data_15f838
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_ff491:
		push   $data_15f864
		xor    %ah,%ah
		push   $SoundProgressMessage
		mov    %ah,SoundAble
		mov    %ah,SoundActive
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		jmp    jump_ffa7c
	jump_ff4c0:
		cmpb   $0x0,AILStartupAlreadyInitiated
		jne    jump_ff4ef
		push   $LbMemoryAlloc_
		call   __unknown_libname_1
		add    $0x4,%esp
		push   $LbMemoryFree_
		call   __unknown_libname_2
		add    $0x4,%esp
		call   ail_startup
		movb   $0x1,AILStartupAlreadyInitiated
	jump_ff4ef:
		push   $0x0
		push   $0x9
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0xc8
		push   $0x0
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x1
		push   $0x8
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x5622
		push   $0x1
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x64
		push   $0x3
		call   AIL_set_preference
		xor    %eax,%eax
		mov    MaxNumberOfSamples,%edx
		mov    %bh,%al
		add    $0x8,%esp
		add    %edx,%eax
		push   %eax
		push   $0x4
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x7f
		push   $0x5
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x28f
		push   $0x6
		call   AIL_set_preference
		mov    StereoSound,%bh
		add    $0x8,%esp
		test   %bh,%bh
		je     jump_ff571
		push   $0x1
		jmp    jump_ff573
	jump_ff571:
		push   $0x0
	jump_ff573:
		push   $0x7
		call   AIL_set_preference
		add    $0x8,%esp
		cmpb   $0x0,AutoScanForSoundHardware
		jne    jump_ff592
		push   $0x0
		push   $0x11
		call   AIL_set_preference
		add    $0x8,%esp
	jump_ff592:
		mov    $data_15f883,%eax
		call   ac_getenv
		mov    %eax,%edx
		test   %eax,%eax
		je     jump_ff6cd
		push   $data_15f88b
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		lea    0x6(%esp),%eax
		push   %eax
		lea    0x8(%esp),%eax
		push   %eax
		lea    0xa(%esp),%eax
		push   %eax
		lea    0xc(%esp),%eax
		push   %eax
		lea    0x28(%esp),%eax
		push   %eax
		push   $data_15f8ae
		push   %edx
		call   ac_sscanf
		add    $0x1c,%esp
		mov    %esp,%eax
		push   %eax
		lea    0x1c(%esp),%eax
		push   %eax
		call   AIL_install_DIG_driver_file
		add    $0x8,%esp
		mov    %eax,SoundDriver
		test   %eax,%eax
		jne    jump_ff65c
		push   $data_15f8bd
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,MusicAble
		jne    jump_ff62c
		call   ail_shutdown
	jump_ff62c:
		push   $data_15f8f6
		push   $SoundInstallChoice__Field1
		xor    %bh,%bh
		call   ac_sprintf
		add    $0x8,%esp
		mov    %bh,SoundAble
		mov    %bh,SoundActive
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ff65c:
		lea    0x18(%esp),%eax
		push   %eax
		push   $data_15f8fb
		push   $SoundInstallChoice__Field1
		call   ac_sprintf
		add    $0xc,%esp
		push   $data_15f8fe
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		mov    (%esp),%eax
		mov    %ax,SoundInstallChoice__Field2
	jump_ff698:
		push   $data_15fa47
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    DisableLoadSounds,%al
		test   %eax,%eax
		jne    jump_ff81f
		call   DetermineSoundType_
		jmp    jump_ff83b
	jump_ff6cd:
#if 0
		push   $FullDIG_INIPath
		push   $SoundInstallChoice__Field0
		call   AIL_read_INI
		add    $0x8,%esp
		test   %eax,%eax
		je     jump_ff7a3
#endif
		push   $data_15f937
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		mov    $data_15f95e,%edx
		call   SoundProgressLog_
		mov    $SoundInstallChoice__Field1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_ff75e
		push   $data_15f963
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,MusicAble
		jne    jump_ff740
		call   ail_shutdown
	jump_ff740:
		xor    %dh,%dh
		mov    %dh,SoundAble
		mov    %dh,SoundActive
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ff75e:
		push   $SoundDriver
		call   AIL_install_DIG_INI
		add    $0x4,%esp
		test   %eax,%eax
		jne    jump_ff78a
		push   $data_15f991
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		mov    $0x1,%bl
		jmp    jump_ff7cc
	jump_ff78a:
		push   $data_15f9be
		push   $SoundInstallChoice__Field1
		call   ac_sprintf
		add    $0x8,%esp
		push   $data_15f9c3
		jmp    jump_ff7ba
	jump_ff7a3:
		push   $data_15f9ea
		push   $SoundInstallChoice__Field1
		call   ac_sprintf
		add    $0x8,%esp
		push   $data_15f9ef
	jump_ff7ba:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
	jump_ff7cc:
		call   SoundProgressLog_
		test   %bl,%bl
		jne    jump_ff698
		push   $data_15fa13
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,MusicAble
		jne    jump_ff803
		call   ail_shutdown
	jump_ff803:
		xor    %al,%al
		mov    %al,SoundAble
		mov    %al,SoundActive
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ff81f:
		push   $data_15fa71
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_ff83b:
		xor    %eax,%eax
		mov    SoundType,%ax
		test   %eax,%eax
		jne    jump_ff974
		push   $data_15fa8d
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		push   $data_15fab9
		push   $SoundInstallChoice__Field1
		call   ac_sprintf
		mov    MusicAble,%ah
		add    $0x8,%esp
		test   %ah,%ah
		jne    jump_ff888
		call   ail_shutdown
	jump_ff888:
		mov    SoundDriver,%edi
		xor    %dl,%dl
		push   %edi
		mov    %dl,SoundAble
		mov    %dl,SoundActive
		call   AIL_uninstall_DIG_driver
		add    $0x4,%esp
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ff8b5:
		mov    $0x1,%bh
		mov    $0xac44,%esi
		mov    %bh,SixteenBit
		jmp    jump_ff9e5
	jump_ff8c7:
		mov    $0x1,%cl
		mov    $0xac44,%esi
		mov    %cl,SixteenBit
		jmp    jump_ff9e5
	jump_ff8d9:
		mov    $0x1,%ch
		mov    $0x5622,%esi
		mov    %ch,SixteenBit
		jmp    jump_ff9e5
	jump_ff8eb:
		mov    $0x1,%al
		mov    $0x5622,%esi
		mov    %al,SixteenBit
		jmp    jump_ff9e5
	jump_ff8fc:
		mov    $0x1,%ah
		mov    $0x2b11,%esi
		mov    %ah,SixteenBit
		jmp    jump_ff9e5
	jump_ff90e:
		mov    $0x1,%dl
		mov    $0x2b11,%esi
		mov    %dl,SixteenBit
		jmp    jump_ff9e5
	jump_ff920:
		xor    %dh,%dh
		mov    $0x5622,%esi
		mov    %dh,SixteenBit
		jmp    jump_ff9e5
	jump_ff932:
		xor    %bl,%bl
		mov    $0x2b11,%esi
		mov    %bl,SixteenBit
		jmp    jump_ff9e5
	jump_ff944:
		xor    %bh,%bh
		mov    $0x2b11,%esi
		mov    %bh,SixteenBit
		jmp    jump_ff9e5
	jump_ff956:
		xor    %ch,%ch
		mov    %ch,SoundAble
		mov    %ch,SoundActive
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_ff974:
		mov    SampleRate,%esi
		cmp    $0x64a,%ax
		jb     jump_ff9bc
		jbe    jump_ff90e
		cmp    $0x656,%ax
		jb     jump_ff9a6
		jbe    jump_ff8d9
		cmp    $0x668,%ax
		jb     jump_ff9e5
		jbe    jump_ff8c7
		cmp    $0x66c,%ax
		je     jump_ff8b5
		jmp    jump_ff9e5
	jump_ff9a6:
		cmp    $0x64b,%ax
		jbe    jump_ff8fc
		cmp    $0x654,%ax
		je     jump_ff8eb
		jmp    jump_ff9e5
	jump_ff9bc:
		cmp    $0x320,%ax
		jb     jump_ff9dc
		jbe    jump_ff944
		cmp    $0x32b,%ax
		jb     jump_ff9e5
		jbe    jump_ff932
		cmp    $0x336,%ax
		je     jump_ff920
		jmp    jump_ff9e5
	jump_ff9dc:
		test   %ax,%ax
		je     jump_ff956
	jump_ff9e5:
		mov    SoundAble,%cl
		mov    %esi,SampleRate
		test   %cl,%cl
		je     jump_ffa7c
		mov    MaxNumberOfSamples,%ebx
		dec    %ebx
		lea    0x0(,%ebx,4),%eax
		sub    %ebx,%eax
		lea    0x0(,%eax,8),%eax
		mov    $sample_id,%edx
		add    %ebx,%eax
		add    %eax,%edx
		mov    $sample_id,%ebx
		mov    %edx,end_sample_id
		cmp    %ebx,%edx
		jb     jump_ffa4e
	jump_ffa28:
		mov    SoundDriver,%ebp
		push   %ebp
		add    $0x19,%ebx
		call   AIL_allocate_sample_handle
		add    $0x4,%esp
		mov    %eax,-0x19(%ebx)
		movb   $0x0,-0x5(%ebx)
		mov    end_sample_id,%eax
		movb   $0x0,-0x2(%ebx)
		cmp    %eax,%ebx
		jbe    jump_ffa28
	jump_ffa4e:
		push   $data_15fabe
		mov    $0x1,%dh
		push   $SoundProgressMessage
		mov    %dh,SoundInstalled
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		mov    CurrentSoundMasterVolume,%eax
		call   SetSoundMasterVolume_
	jump_ffa7c:
		mov    SampleRate,%esi
		add    $0x28,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
DetermineSoundType_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		mov    $SoundProgressMessage,%ebx
	jump_ffa97:
		xor    %eax,%eax
		mov    SoundType,%ax
		push   %eax
		push   $data_15fadb
		push   %ebx
		call   ac_sprintf
		add    $0xc,%esp
		mov    %ebx,%eax
		call   SoundProgressLog_
		xor    %eax,%eax
		mov    SoundType,%ax
		call   AllocateSoundBankMemory_
		mov    %eax,%edx
		test   %eax,%eax
		jg     jump_ffc40
		cmp    $0xffffffff,%eax
		jne    jump_ffaf9
		push   $data_15fae5
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		xor    %ebx,%ebx
		call   SoundProgressLog_
		mov    %bx,SoundType
		pop    %edx
		pop    %ebx
		ret
	jump_ffaf9:
		test   %eax,%eax
		jne    jump_ffb17
		push   $data_15faf6
		push   %ebx
		call   ac_sprintf
		add    $0x8,%esp
		mov    %ebx,%eax
		call   SoundProgressLog_
		jmp    jump_ffb35
	jump_ffb17:
		cmp    $0xfffffffe,%eax
		jne    jump_ffb35
		push   $data_15fb0a
		push   %ebx
		call   ac_sprintf
		add    $0x8,%esp
		mov    %ebx,%eax
		call   SoundProgressLog_
	jump_ffb35:
		mov    SoundType,%ax
		cmp    $0x64b,%ax
		jb     jump_ffc0e
		jbe    jump_ffba1
		cmp    $0x656,%ax
		jb     jump_ffbfd
		jbe    jump_ffb85
		cmp    $0x668,%ax
		jb     jump_ffc55
		jbe    jump_ffb77
		cmp    $0x66c,%ax
		jne    jump_ffc55
		movw   $0x668,SoundType
		jmp    jump_ffc55
	jump_ffb77:
		movw   $0x656,SoundType
		jmp    jump_ffc55
	jump_ffb85:
		movw   $0x654,SoundType
		jmp    jump_ffc55
	jump_ffb93:
		movw   $0x64b,SoundType
		jmp    jump_ffc55
	jump_ffba1:
		movw   $0x64a,SoundType
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffbb5:
		movw   $0x336,SoundType
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffbc9:
		movw   $0x32b,SoundType
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffbdd:
		movw   $0x320,SoundType
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffbf1:
		xor    %edx,%edx
		mov    %dx,SoundType
		pop    %edx
		pop    %ebx
		ret
	jump_ffbfd:
		cmp    $0x654,%ax
		je     jump_ffb93
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffc0e:
		cmp    $0x32b,%ax
		jb     jump_ffc2f
		jbe    jump_ffbdd
		cmp    $0x336,%ax
		jb     jump_ffc55
		jbe    jump_ffbc9
		cmp    $0x64a,%ax
		je     jump_ffbb5
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffc2f:
		cmp    $0x320,%ax
		je     jump_ffbf1
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret
	jump_ffc40:
		push   $data_15fb27
		push   %ebx
		call   ac_sprintf
		add    $0x8,%esp
		mov    %ebx,%eax
		call   SoundProgressLog_
	jump_ffc55:
		test   %edx,%edx
		jle    jump_ffa97
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC(InitMusic_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		sub    $0x148,%esp
		mov    MusicAble,%ah
		xor    %bl,%bl
		test   %ah,%ah
		je     jump_100489
		push   $data_15fb44
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,AILStartupAlreadyInitiated
		jne    jump_ffcc6
		push   $LbMemoryAlloc_
		call   __unknown_libname_1
		add    $0x4,%esp
		push   $LbMemoryFree_
		call   __unknown_libname_2
		add    $0x4,%esp
		call   ail_startup
		movb   $0x1,AILStartupAlreadyInitiated
	jump_ffcc6:
		push   $0x78
		push   $0xb
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x1
		push   $0xc
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x7f
		push   $0xd
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x1
		push   $0xe
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0x0
		push   $0xf
		call   AIL_set_preference
		add    $0x8,%esp
		push   $0xc
		push   $0x10
		call   AIL_set_preference
		mov    AutoScanForSoundHardware,%bh
		add    $0x8,%esp
		test   %bh,%bh
		jne    jump_ffd24
		push   $0x0
		push   $0x11
		call   AIL_set_preference
		add    $0x8,%esp
	jump_ffd24:
		push   $SoundAndMusicDataPath
		push   $data_15fb57
		lea    0x8(%esp),%eax
		push   %eax
		call   ac_sprintf
		add    $0xc,%esp
		mov    %esp,%eax
		push   %eax
		call   AIL_set_GTL_filename_prefix
		add    $0x4,%esp
		mov    %esp,%eax
		push   %eax
		push   $data_15fb61
		lea    0x98(%esp),%eax
		push   %eax
		call   ac_sprintf
		add    $0xc,%esp
		lea    0x90(%esp),%eax
		call   fm_instrument_file_exists
		test   %eax,%eax
		jne    jump_ffd8a
		push   $data_15fb67
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_ffd8a:
		mov    %esp,%eax
		push   %eax
		push   $data_15fb83
		lea    0x98(%esp),%eax
		push   %eax
		call   ac_sprintf
		add    $0xc,%esp
		lea    0x90(%esp),%eax
		call   fm_instrument_file_exists
		test   %eax,%eax
		jne    jump_ffdce
		push   $data_15fb8a
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_ffdce:
		mov    $data_15fba7,%eax
		call   ac_getenv
		mov    %eax,%edx
		test   %eax,%eax
		je     jump_fff04
		push   $data_15fbaf
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		lea    0x126(%esp),%eax
		push   %eax
		lea    0x128(%esp),%eax
		push   %eax
		lea    0x12a(%esp),%eax
		push   %eax
		lea    0x12c(%esp),%eax
		push   %eax
		lea    0x148(%esp),%eax
		push   %eax
		push   $data_15fbd2
		push   %edx
		call   ac_sscanf
		add    $0x1c,%esp
		lea    0x120(%esp),%eax
		push   %eax
		lea    0x13c(%esp),%eax
		push   %eax
		call   ail_install_mdi_driver_file
		add    $0x8,%esp
		mov    %eax,MusicDriver
		test   %eax,%eax
		jne    jump_ffe92
		push   $data_15fbe1
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,SoundAble
		jne    jump_ffe7f
		call   ail_shutdown
	jump_ffe7f:
		xor    %dl,%dl
		mov    %dl,MusicAble
		mov    %dl,MusicActive
		jmp    jump_100489
	jump_ffe92:
		push   $data_15fc1a
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		lea    0x138(%esp),%eax
		push   %eax
		push   $data_15fc53
		push   $awe32_s1
		call   ac_sprintf
		add    $0xc,%esp
	jump_ffec8:
		mov    MusicDriver,%edx
		push   %edx
		call   AIL_allocate_sequence_handle
		mov    $data_15fd5e,%edx
		add    $0x4,%esp
		mov    %eax,SongHandle
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_10002c
		mov    $data_15fd68,%eax
		push   %eax
		mov    $MusicType,%eax
		push   %eax
		jmp    jump_1002e1
	jump_fff04:
		push   $FullMDI_INIPath
		push   $data_1e8c84
		call   AIL_read_INI
		add    $0x8,%esp
		test   %eax,%eax
		je     jump_fffc4
		push   $data_15fc56
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		mov    $data_15fc7a,%edx
		call   SoundProgressLog_
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_fff91
		push   $data_15fc7f
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,SoundAble
		jne    jump_fff77
		call   ail_shutdown
	jump_fff77:
		xor    %dl,%dl
		mov    %dl,MusicAble
		mov    %dl,MusicActive
		add    $0x148,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_fff91:
		push   $MusicDriver
		call   ail_install_mdi_ini
		add    $0x4,%esp
		test   %eax,%eax
		jne    jump_fffbd
		push   $data_15fcaa
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		mov    $0x1,%bl
		jmp    jump_fffdb
	jump_fffbd:
		push   $data_15fcd7
		jmp    jump_fffc9
	jump_fffc4:
		push   $data_15fd04
	jump_fffc9:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
	jump_fffdb:
		call   SoundProgressLog_
		test   %bl,%bl
		jne    jump_ffec8
		push   $data_15fd28
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		cmpb   $0x0,SoundAble
		jne    jump_100012
		call   ail_shutdown
	jump_100012:
		xor    %ch,%ch
		mov    %ch,MusicAble
		mov    %ch,MusicActive
		add    $0x148,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_10002c:
		mov    $data_15fd6a,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100050
		mov    $data_15fd75,%eax
		push   %eax
		mov    $MusicType,%eax
		push   %eax
		jmp    jump_1002e1
	jump_100050:
		mov    $data_15fd77,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100074
		mov    $data_15fd80,%eax
		push   %eax
		mov    $MusicType,%eax
		push   %eax
		jmp    jump_1002e1
	jump_100074:
		mov    $data_15fd82,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100098
		mov    $data_15fd8a,%eax
		push   %eax
		mov    $MusicType,%eax
		push   %eax
		jmp    jump_1002e1
	jump_100098:
		mov    $data_15fd8c,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1000b5
		push   $data_15fd98
		jmp    jump_1002dc
	jump_1000b5:
		mov    $data_15fd9a,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1000d2
		push   $data_15fda7
		jmp    jump_1002dc
	jump_1000d2:
		mov    $data_15fda9,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1000ef
		push   $data_15fdb4
		jmp    jump_1002dc
	jump_1000ef:
		mov    $data_15fdb6,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_10010c
		push   $data_15fdc1
		jmp    jump_1002dc
	jump_10010c:
		mov    $data_15fdc3,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100129
		push   $data_15fdcc
		jmp    jump_1002dc
	jump_100129:
		mov    $data_15fdce,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1001ca
		push   $data_15fdda
		push   $MusicType
		mov    $0x1,%ch
		call   ac_sprintf
		add    $0x8,%esp
		mov    $_CurrentAwe32SoundfontPrefix,%eax
		mov    %ch,MusicInstalled
		call   LoadAwe32Soundfont_
		xor    %al,%al
		mov    %al,MusicInstalled
		xor    %eax,%eax
		mov    Awe32SoundfontLoaded,%al
		test   %eax,%eax
		jne    jump_1001a9
		push   $data_15fddc
		push   $MusicType
		call   ac_sprintf
		add    $0x8,%esp
		push   $data_15fdde
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		jmp    jump_1002e9
	jump_1001a9:
		push   $data_15fe00
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		jmp    jump_1002e9
	jump_1001ca:
		mov    $data_15fe1f,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1001e7
		push   $data_15fe2a
		jmp    jump_1002dc
	jump_1001e7:
		mov    $data_15fe2c,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100204
		push   $data_15fe39
		jmp    jump_1002dc
	jump_100204:
		mov    $data_15fe3b,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100221
		push   $data_15fe45
		jmp    jump_1002dc
	jump_100221:
		mov    $data_15fe47,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_10023e
		push   $data_15fe53
		jmp    jump_1002dc
	jump_10023e:
		mov    MusicDriver,%ebx
		push   %ebx
		call   AIL_MDI_driver_type
		add    $0x4,%esp
		test   %eax,%eax
		jne    jump_10025b
		push   $data_15fe55
		jmp    jump_1002dc
	jump_10025b:
		mov    MusicDriver,%ecx
		push   %ecx
		call   AIL_MDI_driver_type
		add    $0x4,%esp
		cmp    $0x1,%eax
		je     jump_1002d7
		mov    MusicDriver,%esi
		push   %esi
		call   AIL_MDI_driver_type
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_1002d7
		mov    MusicDriver,%edi
		push   %edi
		call   ail_uninstall_mdi_driver
		mov    SoundAble,%bl
		add    $0x4,%esp
		test   %bl,%bl
		jne    jump_1002a1
		call   ail_shutdown
	jump_1002a1:
		push   $data_15fe59
		xor    %bh,%bh
		push   $SoundProgressMessage
		mov    %bh,MusicAble
		mov    %bh,MusicActive
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		add    $0x148,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_1002d7:
		push   $data_15fe57
	jump_1002dc:
		push   $MusicType
	jump_1002e1:
		call   ac_sprintf
		add    $0x8,%esp
	jump_1002e9:
		cmpb   $0x0,DisableLoadMusic
		jne    jump_100440
		push   $MusicType
		push   $data_15fe81
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		mov    $data_15fea0,%edx
		call   SoundProgressLog_
		call   AllocateMusicBankMemory_
		mov    %eax,music_allocated
		mov    $MusicType,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_100396
		cmpl   $0xfffffffe,music_allocated
		jne    jump_100396
		push   $data_15fea2
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		push   $data_15febf
		push   $MusicType
		call   ac_sprintf
		add    $0x8,%esp
		push   $MusicType
		push   $data_15fec1
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0xc,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		call   AllocateMusicBankMemory_
		mov    %eax,music_allocated
	jump_100396:
		mov    music_allocated,%eax
		test   %eax,%eax
		jg     jump_100439
		cmp    $0xffffffff,%eax
		jne    jump_1003af
		push   $data_15fee1
		jmp    jump_1003c4
	jump_1003af:
		test   %eax,%eax
		jne    jump_1003ba
		push   $data_15fef2
		jmp    jump_1003c4
	jump_1003ba:
		cmp    $0xfffffffe,%eax
		jne    jump_1003db
		push   $data_15ff06
	jump_1003c4:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
	jump_1003db:
		xor    %eax,%eax
		mov    Awe32SoundfontLoaded,%al
		cmp    $0x1,%eax
		jne    jump_100401
		movb   $0x1,MusicInstalled
		xor    %dh,%dh
		call   FreeAwe32Soundfont
		mov    %dh,MusicInstalled
		mov    %dh,Awe32SoundfontLoaded
	jump_100401:
		mov    MusicDriver,%esi
		push   %esi
		call   ail_uninstall_mdi_driver
		mov    SoundAble,%bh
		add    $0x4,%esp
		test   %bh,%bh
		jne    jump_10041f
		call   ail_shutdown
	jump_10041f:
		xor    %cl,%cl
		mov    %cl,MusicAble
		mov    %cl,MusicActive
		add    $0x148,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_100439:
		push   $data_15ff23
		jmp    jump_100445
	jump_100440:
		push   $data_15ff3d
	jump_100445:
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		push   $data_15ff58
		push   $SoundProgressMessage
		call   ac_sprintf
		add    $0x8,%esp
		mov    $SoundProgressMessage,%eax
		call   SoundProgressLog_
		movb   $0x1,MusicInstalled
		mov    CurrentMusicMasterVolume,%eax
		call   SetMusicMasterVolume_
	jump_100489:
		add    $0x148,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
/* int fm_instrument_file_exists(char *fname)
 */
fm_instrument_file_exists:
/*----------------------------------------------------------------*/
		push   %edx
		xor    %edx,%edx
		call   ac_dos_access
		test   %eax,%eax
		sete   %al
		and    $0xff,%eax
		pop    %edx
		ret


/*----------------------------------------------------------------*/
InitAllBullfrogSoundTimers_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		cmpb   $0x0,MusicInstalled
		jne    jump_1004d4
		cmpb   $0x0,SoundInstalled
		je     jump_1004e5
	jump_1004d4:
		mov    $0x19,%ebx
		mov    $sound_timer,%eax
		xor    %edx,%edx
		call   ac_memset
	jump_1004e5:
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
InitStreamedSound_:
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		cmpb   $0x0,SoundInstalled
		je     jump_1006ae
		cmpb   $0x0,SoundAble
		je     jump_1006ae
		cmpb   $0x0,StreamedSoundAble
		jne    jump_1006ae
#if 0
		call   _flushall
		mov    $data_159392,%eax
		xor    %edx,%edx
		call   ac_setbuf
#endif
		call   allocate_buffers
		test   %eax,%eax
		jne    jump_10065a
		mov    $data_15ff78,%esi
		mov    $SoundProgressMessage,%edi
		call   free_buffers
		push   %edi
	jump_100635:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_10064d
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_100635
	jump_10064d:
		pop    %edi
		mov    %edi,%eax
		call   SoundProgressLog_
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		ret
	jump_10065a:
		mov    SoundDriver,%edx
		push   %edx
		call   AIL_allocate_sample_handle
		add    $0x4,%esp
		mov    %eax,data_1e8e4c
		test   %eax,%eax
		jne    jump_1006a7
		mov    $data_15ffac,%esi
		mov    $SoundProgressMessage,%edi
		call   free_buffers
		push   %edi
	jump_100682:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_10069a
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_100682
	jump_10069a:
		pop    %edi
		mov    %edi,%eax
		call   SoundProgressLog_
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		ret
	jump_1006a7:
		movb   $0x1,StreamedSoundAble
	jump_1006ae:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
FreeStreamedSound_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		mov    sample_file,%edx
		cmp    $0xffffffff,%edx
		je     jump_1006df
		mov    %edx,%eax
		mov    $0xffffffff,%ebx
		call   LbFileClose_
		mov    %ebx,sample_file
	jump_1006df:
		call   SwitchOffStreamedSound_
		call   free_buffers
		xor    %ah,%ah
		mov    %ah,StreamedSoundAble
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
SwitchOffStreamedSound_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %edi
		cmpb   $0x0,StreamedSoundAble
		je     jump_100d1b
		cmpb   $0x0,active
		je     jump_100d1b
		mov    data_1e8e4c,%edx
		push   %edx
		mov    $0x4000,%ebx
		call   AIL_end_sample
		add    $0x4,%esp
		mov    data_1e8f60,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    $0x4000,%ebx
		mov    data_1e8f64,%eax
		xor    %edx,%edx
		call   ac_memset
		mov    sample_file,%ebx
		cmp    $0xffffffff,%ebx
		je     jump_100cec
		mov    %ebx,%eax
		mov    $0xffffffff,%ecx
		call   LbFileClose_
		mov    %ecx,sample_file
	jump_100cec:
		cmpl   $0x0,adpcm_file_open
		je     jump_100d02
		xor    %edi,%edi
		call   close_adpcm_file_
		mov    %edi,adpcm_file_open
	jump_100d02:
		cmpb   $0x0,mixed_file_open
		je     jump_100d13
		xor    %bl,%bl
		mov    %bl,mixed_file_open
	jump_100d13:
		xor    %bh,%bh
		mov    %bh,active
	jump_100d1b:
		pop    %edi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
allocate_buffers:
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		mov    $0x8000,%eax
		call   LbMemoryAlloc_
		mov    %eax,data_1e8f94
		test   %eax,%eax
		je     jump_100ff9
		mov    $0x800,%eax
		call   LbMemoryAlloc_
		mov    %eax,data_1e8f6c
		test   %eax,%eax
		je     jump_100ff9
		push   $0x4000
		call   MEM_alloc_lock
		add    $0x4,%esp
		mov    %eax,data_1e8f60
		test   %eax,%eax
		je     jump_100ff9
		push   $0x4000
		call   MEM_alloc_lock
		add    $0x4,%esp
		mov    %eax,data_1e8f64
		test   %eax,%eax
		je     jump_100ff9
		mov    $0x1,%eax
	jump_100ff9:
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
free_buffers:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %ebp
		mov    data_1e8f60,%edx
		test   %edx,%edx
		je     jump_101025
		push   $0x4000
		push   %edx
		call   MEM_free_lock
		xor    %ecx,%ecx
		add    $0x8,%esp
		mov    %ecx,data_1e8f60
	jump_101025:
		mov    data_1e8f64,%esi
		test   %esi,%esi
		je     jump_101045
		push   $0x4000
		push   %esi
		xor    %ebp,%ebp
		call   MEM_free_lock
		add    $0x8,%esp
		mov    %ebp,data_1e8f64
	jump_101045:
		mov    data_1e8f6c,%eax
		test   %eax,%eax
		je     jump_10105b
		xor    %edx,%edx
		call   LbMemoryFree_
		mov    %edx,data_1e8f6c
	jump_10105b:
		mov    data_1e8f94,%ebx
		test   %ebx,%ebx
		je     jump_101074
		mov    %ebx,%eax
		xor    %ecx,%ecx
		call   LbMemoryFree_
		mov    %ecx,data_1e8f94
	jump_101074:
		pop    %ebp
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
StopMusic_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		cmpb   $0x0,MusicInstalled
		je     jump_108c5a
		cmpb   $0x0,MusicAble
		je     jump_108c5a
		cmpb   $0x0,MusicActive
		je     jump_108c5a
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_108c5a
		cmpb   $0x0,DangerMusicFadeActive
		je     jump_108bfd
		mov    DangerMusicFadeHandle,%edx
		push   %edx
		call   AIL_release_timer_handle
		add    $0x4,%esp
	jump_108bfd:
		xor    %bh,%bh
		mov    %bh,DangerMusicAble
		mov    %bh,DangerMusicFadeActive
		movb   $0x1,CurrentDangerMusicFadeDirection
		mov    %bh,DangerMusicVolume
		movb   $0xff,DangerMusicVolumeChange
		mov    SongHandle,%ebx
		push   %ebx
		call   AIL_sequence_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_108c51
		mov    SongHandle,%ecx
		push   %ecx
		call   AIL_stop_sequence
		add    $0x4,%esp
		mov    SongHandle,%esi
		push   %esi
		call   AIL_end_sequence
		add    $0x4,%esp
	jump_108c51:
		xor    %ebx,%ebx
		mov    %bx,SongCurrentlyPlaying
	jump_108c5a:
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
FreeMusic_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		cmpb   $0x0,MusicInstalled
		je     jump_108d00
		cmpw   $0x0,SongCurrentlyPlaying
		je     jump_108ca3
		mov    SongHandle,%edx
		push   %edx
		call   AIL_stop_sequence
		add    $0x4,%esp
		mov    SongHandle,%ebx
		push   %ebx
		call   AIL_end_sequence
		xor    %ebx,%ebx
		add    $0x4,%esp
		mov    %bx,SongCurrentlyPlaying
	jump_108ca3:
		xor    %eax,%eax
		mov    Awe32SoundfontLoaded,%al
		cmp    $0x1,%eax
		jne    jump_108cb4
		call   FreeAwe32Soundfont
	jump_108cb4:
		cmpb   $0x0,SoundInstalled
		jne    jump_108cc2
		call   ail_shutdown
	jump_108cc2:
		mov    Music,%ecx
		test   %ecx,%ecx
		je     jump_108cdb
		mov    %ecx,%eax
		xor    %esi,%esi
		call   LbMemoryFree_
		mov    %esi,EndMusic
	jump_108cdb:
		mov    MusicData,%edi
		test   %edi,%edi
		je     jump_108cec
		mov    %edi,%eax
		call   LbMemoryFree_
	jump_108cec:
		xor    %dh,%dh
		mov    %dh,MusicAble
		mov    %dh,MusicActive
		mov    %dh,MusicInstalled
	jump_108d00:
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
FreeSound_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		cmpb   $0x0,SoundInstalled
		je     jump_108d6d
		call   StopAllSamples_
		cmpb   $0x0,MusicInstalled
		jne    jump_108d2f
		call   ail_shutdown
	jump_108d2f:
		mov    Sfx,%edx
		test   %edx,%edx
		je     jump_108d48
		mov    %edx,%eax
		xor    %ebx,%ebx
		call   LbMemoryFree_
		mov    %ebx,EndSfxs
	jump_108d48:
		mov    SfxData,%ecx
		test   %ecx,%ecx
		je     jump_108d59
		mov    %ecx,%eax
		call   LbMemoryFree_
	jump_108d59:
		xor    %dh,%dh
		mov    %dh,SoundAble
		mov    %dh,SoundActive
		mov    %dh,SoundInstalled
	jump_108d6d:
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSampleVolume_)	/* 0xf0a20 */
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		mov    %eax,%ebp
		mov    %edx,%edi
		mov    %ebx,%esi
		cmpb   $0x0,SoundInstalled
		je     jump_f0aaa
		cmpb   $0x0,SoundAble
		je     jump_f0aaa
		cmpb   $0x0,SoundActive
		je     jump_f0aaa
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_f0a6c
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret
	jump_f0a5f:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f0aaa
	jump_f0a6c:
		cmp    0xe(%ebx),%ebp
		jne    jump_f0a5f
		cmp    0x12(%ebx),%di
		jne    jump_f0a5f
		mov    (%ebx),%edx
		push   %edx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_f0a5f
		movswl %si,%eax
		test   %eax,%eax
		jl     jump_f0a5f
		cmp    $0x7f,%eax
		jg     jump_f0a5f
		cmp    0x4(%ebx),%eax
		je     jump_f0a5f
		push   %eax
		mov    (%ebx),%edi
		push   %edi
		call   AIL_set_sample_volume
		add    $0x8,%esp
		movswl %si,%eax
		mov    %eax,0x4(%ebx)
	jump_f0aaa:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (SetSamplePan_)	/* 0xF0AB0 */
/*----------------------------------------------------------------*/
		push   %ecx
		push   %esi
		push   %edi
		sub    $0x4,%esp
		mov    %eax,%edi
		mov    %edx,(%esp)
		mov    %ebx,%esi
		cmpb   $0x0,SoundInstalled
		je     jump_f0b31
		cmpb   $0x0,SoundAble
		je     jump_f0b31
		cmpb   $0x0,SoundActive
		je     jump_f0b31
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_f0af8
		jmp    jump_f0b31
	jump_f0aeb:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f0b31
	jump_f0af8:
		cmp    0xe(%ebx),%edi
		jne    jump_f0aeb
		mov    (%esp),%edx
		cmp    0x12(%ebx),%dx
		jne    jump_f0aeb
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_f0aeb
		cmp    0xa(%ebx),%si
		je     jump_f0aeb
		xor    %eax,%eax
		mov    %si,%ax
		push   %eax
		mov    (%ebx),%edx
		push   %edx
		call   AIL_set_sample_pan
		add    $0x8,%esp
		mov    %si,0xa(%ebx)
	jump_f0b31:
		add    $0x4,%esp
		pop    %edi
		pop    %esi
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (StopSample_)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		mov    %eax,%esi
		mov    %edx,%edi
		cmpb   $0x0,SoundInstalled
		je     jump_f1788
		cmpb   $0x0,SoundAble
		je     jump_f1788
		cmpb   $0x0,SoundActive
		je     jump_f1788
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_f1762
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret
	jump_f1755:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f1788
	jump_f1762:
		cmp    0xe(%ebx),%esi
		jne    jump_f1755
		cmp    0x12(%ebx),%di
		jne    jump_f1755
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_f1755
		mov    (%ebx),%edx
		push   %edx
		call   AIL_end_sample
		add    $0x4,%esp
	jump_f1788:
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
GLOBAL_FUNC (PlaySampleFromAddress_)
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		push   %ebp
		sub    $0xc,%esp
		mov    %eax,%esi
		mov    %edx,%edi
		mov    %ebx,0x4(%esp)
		mov    %ecx,(%esp)
		mov    0x24(%esp),%al
		cmpb   $0x0,SoundInstalled
		je     jump_f17cc
		cmpb   $0x0,SoundAble
		je     jump_f17cc
		cmpb   $0x0,SoundActive
		jne    jump_f1918
	jump_f17cc:
		xor    %eax,%eax
	jump_f17ce:
		add    $0xc,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x10
	jump_f17d7:
		mov    $sample_id,%ebx
		mov    end_sample_id,%eax
		xor    %ebp,%ebp
		cmp    %ebx,%eax
		jae    jump_f17fd
		jmp    jump_f1934
	jump_f17ec:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f1934
	jump_f17fd:
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		jne    jump_f17ec
		xor    %edx,%edx
		mov    %ebx,%ebp
		mov    %edx,0x8(%esp)
		jmp    jump_f1934
	jump_f181a:
		xor    %ebx,%ebx
		mov    end_sample_id,%ecx
		mov    %ebx,0x8(%esp)
		mov    $sample_id,%ebx
		cmp    %ebx,%ecx
		jae    jump_f183e
		jmp    jump_f185d
	jump_f1831:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f185d
	jump_f183e:
		cmp    0xe(%ebx),%esi
		jne    jump_f1831
		cmp    0x12(%ebx),%di
		jne    jump_f1831
		mov    (%ebx),%edx
		push   %edx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		je     jump_f1831
		mov    %ebx,0x8(%esp)
	jump_f185d:
		cmpl   $0x0,0x8(%esp)
		jne    jump_f1934
		mov    $sample_id,%ebx
		mov    end_sample_id,%ecx
		xor    %ebp,%ebp
		cmp    %ebx,%ecx
		jae    jump_f188e
		jmp    jump_f1934
	jump_f187e:
		mov    end_sample_id,%eax
		add    $0x19,%ebx
		cmp    %eax,%ebx
		ja     jump_f1934
	jump_f188e:
		mov    (%ebx),%edx
		push   %edx
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		jne    jump_f187e
		mov    %ebx,%ebp
		jmp    jump_f1934
	jump_f18a5:
		mov    $sample_id,%ebx
		mov    end_sample_id,%edx
		xor    %al,%al
		cmp    %ebx,%edx
		jae    jump_f18c5
		jmp    jump_f18da
	jump_f18b8:
		mov    end_sample_id,%edx
		add    $0x19,%ebx
		cmp    %edx,%ebx
		ja     jump_f18da
	jump_f18c5:
		cmp    0xe(%ebx),%esi
		jne    jump_f18b8
		cmp    0x12(%ebx),%di
		jne    jump_f18b8
		xor    %eax,%eax
		mov    %ebx,%ebp
		mov    %eax,0x8(%esp)
		mov    $0x1,%al
	jump_f18da:
		test   %al,%al
		jne    jump_f1934
		mov    $sample_id,%ebx
		mov    end_sample_id,%edx
		xor    %ebp,%ebp
		cmp    %ebx,%edx
		jae    jump_f18fe
		jmp    jump_f1934
	jump_f18f1:
		mov    end_sample_id,%ecx
		add    $0x19,%ebx
		cmp    %ecx,%ebx
		ja     jump_f1934
	jump_f18fe:
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x2,%eax
		jne    jump_f18f1
		xor    %edx,%edx
		mov    %ebx,%ebp
		mov    %edx,0x8(%esp)
		jmp    jump_f1934
	jump_f1918:
		cmp    $0x2,%al
		jb     jump_f192c
		jbe    jump_f181a
		cmp    $0x3,%al
		je     jump_f18a5
		jmp    jump_f1934
	jump_f192c:
		cmp    $0x1,%al
		je     jump_f17d7
	jump_f1934:
		test   %ebp,%ebp
		je     jump_f17cc
		cmpl   $0x0,0x8(%esp)
		jne    jump_f17cc
		mov    0x0(%ebp),%ecx
		push   %ecx
		call   AIL_init_sample
		add    $0x4,%esp
		mov    0x28(%esp),%eax
		test   %eax,%eax
		je     jump_f17ce
		push   $0x1
		push   %eax
		mov    0x0(%ebp),%ebx
		push   %ebx
		call   AIL_set_sample_file
		add    $0xc,%esp
		mov    0x4(%esp),%ecx
		push   %ecx
		mov    0x0(%ebp),%eax
		push   %eax
		call   AIL_set_sample_volume
		mov    StereoSound,%bl
		add    $0x8,%esp
		test   %bl,%bl
		je     jump_f1999
		mov    (%esp),%edx
		push   %edx
		mov    0x0(%ebp),%ebx
		push   %ebx
		call   AIL_set_sample_pan
		add    $0x8,%esp
	jump_f1999:
		movsbl 0x20(%esp),%eax
		inc    %eax
		push   %eax
		mov    0x0(%ebp),%ecx
		push   %ecx
		call   ail_set_sample_loop_count
		add    $0x8,%esp
		mov    0x0(%ebp),%eax
		push   %eax
		call   AIL_start_sample
		mov    %esi,0xe(%ebp)
		add    $0x4,%esp
		mov    %di,0x12(%ebp)
		mov    0x4(%esp),%eax
		mov    %eax,0x4(%ebp)
		mov    (%esp),%eax
		mov    %ax,0xa(%ebp)
		mov    0x1c(%esp),%eax
		mov    %ax,0x8(%ebp)
		movb   $0x0,0x14(%ebp)
		movb   $0x0,0x15(%ebp)
		mov    %ebp,%eax
		movb   $0x0,0x17(%ebp)
		add    $0xc,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		ret    $0x10


/*----------------------------------------------------------------*/
func_10ffb0:
/*----------------------------------------------------------------*/
		push   %ecx
		push   %edx
		sub    $0xc,%esp
		mov    %esp,%ecx
		push   %ecx
		mov    $0x10,%edx
		push   %ecx
		mov    %dx,0xc(%esp)
		push   $0x506
		mov    (%eax),%edx
		push   %edx
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
		test   %eax,%eax
		jne    jump_10ffec
		movswl 0x8(%esp),%ecx
		movswl 0x6(%esp),%eax
		add    $0x10,%ecx
		shl    %cl,%eax
		jmp    jump_10fff1
	jump_10ffec:
		mov    $0xffffffff,%eax
	jump_10fff1:
		add    $0xc,%esp
		pop    %edx
		pop    %ecx
		ret


/*----------------------------------------------------------------*/
AWEDefMemMap_:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		sub    $0xc,%esp
		mov    %eax,%edi
		mov    %edx,%esi
		cmpl   $0x0,awe32_sf_data
		jne    jump_110042
		cmpw   $0x0,awe32_sf_data_add
		jne    jump_110042
		mov    $0x4,%eax
		call   AllocDOSmem_
		mov    %dx,awe32_sf_data_add
		mov    %eax,awe32_sf_data
		test   %eax,%eax
		jne    jump_110042
		test   %dx,%dx
		jne    jump_110042
		mov    $0xffffffff,%eax
		jmp    jump_1100a7
	jump_110042:
		lgs    awe32_sf_data,%eax
		mov    %ecx,%edx
		mov    %si,%gs:(%eax)
		mov    %ebx,%eax
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    awe32_sf_data_add,%gs
		mov    %eax,%ebx
		mov    awe32_sf_data,%eax
		mov    %bx,%gs:0x2(%eax)
		mov    $0x11,%ebx
		mov    %gs,%edx
		mov    %bx,0x4(%esp)
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    %ax,0x6(%esp)
		mov    %esp,%eax
		push   %eax
		xor    %ecx,%ecx
		push   %eax
		mov    %cx,0x10(%esp)
		push   $0x506
		mov    (%edi),%ebx
		push   %ebx
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
	jump_1100a7:
		add    $0xc,%esp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
AWEGetSFInfo_:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		sub    $0xc,%esp
		mov    %eax,%edi
		mov    %edx,%esi
		cmpl   $0x0,awe32_sf_data
		jne    jump_1100ef
		cmpw   $0x0,awe32_sf_data_add
		jne    jump_1100ef
		mov    $0x4,%eax
		call   AllocDOSmem_
		mov    %dx,awe32_sf_data_add
		mov    %eax,awe32_sf_data
		test   %eax,%eax
		jne    jump_1100ef
		test   %dx,%dx
		je     jump_11016d
	jump_1100ef:
		lgs    awe32_sf_data,%eax
		mov    %ecx,%edx
		mov    %si,%gs:(%eax)
		mov    %ebx,%eax
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    awe32_sf_data_add,%gs
		mov    %eax,%ebx
		mov    awe32_sf_data,%eax
		mov    %bx,%gs:0x2(%eax)
		mov    $0x17,%ebx
		mov    %gs,%edx
		mov    %bx,0x4(%esp)
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    %ax,0x6(%esp)
		mov    %esp,%eax
		push   %eax
		xor    %ecx,%ecx
		push   %eax
		mov    %cx,0x10(%esp)
		push   $0x506
		mov    (%edi),%ebx
		push   %ebx
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
		test   %eax,%eax
		jne    jump_11016d
		movswl 0x6(%esp),%eax
		movswl 0x8(%esp),%ebx
		call   func_10ff20
		mov    %eax,%edx
		mov    %ebx,%eax
		jmp    jump_110171
	jump_11016d:
		xor    %edx,%edx
		xor    %eax,%eax
	jump_110171:
		add    $0xc,%esp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
AWEStreamSample_:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		sub    $0xc,%esp
		mov    %eax,%edi
		mov    %edx,%esi
		cmpl   $0x0,awe32_sf_data
		jne    jump_1101c2
		cmpw   $0x0,awe32_sf_data_add
		jne    jump_1101c2
		mov    $0x4,%eax
		call   AllocDOSmem_
		mov    %dx,awe32_sf_data_add
		mov    %eax,awe32_sf_data
		test   %eax,%eax
		jne    jump_1101c2
		test   %dx,%dx
		jne    jump_1101c2
		mov    $0xffffffff,%eax
		jmp    jump_110227
	jump_1101c2:
		lgs    awe32_sf_data,%eax
		mov    %ecx,%edx
		mov    %si,%gs:(%eax)
		mov    %ebx,%eax
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    awe32_sf_data_add,%gs
		mov    %eax,%ebx
		mov    awe32_sf_data,%eax
		mov    %bx,%gs:0x2(%eax)
		mov    $0x18,%ebx
		mov    %gs,%edx
		mov    %bx,0x4(%esp)
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    %ax,0x6(%esp)
		mov    %esp,%eax
		push   %eax
		xor    %ecx,%ecx
		push   %eax
		mov    %cx,0x10(%esp)
		push   $0x506
		mov    (%edi),%ebx
		push   %ebx
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
	jump_110227:
		add    $0xc,%esp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
func_110230:
/*----------------------------------------------------------------*/
		push   %esi
		push   %edi
		sub    $0xc,%esp
		mov    %eax,%edi
		mov    %edx,%esi
		cmpl   $0x0,awe32_sf_data
		jne    jump_110272
		cmpw   $0x0,awe32_sf_data_add
		jne    jump_110272
		mov    $0x4,%eax
		call   AllocDOSmem_
		mov    %dx,awe32_sf_data_add
		mov    %eax,awe32_sf_data
		test   %eax,%eax
		jne    jump_110272
		test   %dx,%dx
		jne    jump_110272
		mov    $0xffffffff,%eax
		jmp    jump_1102d7
	jump_110272:
		lgs    awe32_sf_data,%eax
		mov    %ecx,%edx
		mov    %si,%gs:(%eax)
		mov    %ebx,%eax
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    awe32_sf_data_add,%gs
		mov    %eax,%ebx
		mov    awe32_sf_data,%eax
		mov    %bx,%gs:0x2(%eax)
		mov    $0x19,%ebx
		mov    %gs,%edx
		mov    %bx,0x4(%esp)
		xor    %eax,%eax
		mov    %dx,%ax
		call   func_10ff60
		mov    %ax,0x6(%esp)
		mov    %esp,%eax
		push   %eax
		xor    %ecx,%ecx
		push   %eax
		mov    %cx,0x10(%esp)
		push   $0x506
		mov    (%edi),%ebx
		push   %ebx
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
	jump_1102d7:
		add    $0xc,%esp
		pop    %edi
		pop    %esi
		ret


/*----------------------------------------------------------------*/
AWEFreeMem_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		sub    $0xc,%esp
		mov    %eax,%ebx
		mov    %edx,%ecx
		mov    awe32_sf_data_add,%dx
		mov    awe32_sf_data,%eax
		call   FreeDOSmem_
		mov    %esp,%eax
		mov    $0x12,%esi
		push   %eax
		mov    %si,0x8(%esp)
		xor    %edx,%edx
		push   %eax
		mov    %cx,0xe(%esp)
		mov    %dx,awe32_sf_data_add
		push   $0x506
		mov    (%ebx),%ecx
		xor    %edx,%edx
		push   %ecx
		mov    %edx,awe32_sf_data
		call   AIL_call_driver
		add    $0x10,%esp
		movswl (%esp),%eax
		add    $0xc,%esp
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
LoadAwe32Soundfont_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		sub    $0x1c,%esp
		mov    %eax,%esi
		cmpb   $0x0,UseCurrentAwe32Soundfont
		je     jump_11035f
		movb   $0x1,Awe32SoundfontLoaded
		jmp    jump_1105d0
	jump_11035f:
		cmpb   $0x0,MusicInstalled
		je     jump_1105d0
		cmpb   $0x0,MusicAble
		je     jump_1105d0
		mov    $data_1617f8,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1105d0
		mov    $data_161804,%edx
		mov    $MusicType,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_1105d0
		mov    Awe32SoundfontLoaded,%al
		cmp    $0x1,%eax
		jne    jump_1103be
		xor    %bl,%bl
		call   FreeAwe32Soundfont
		mov    %bl,Awe32SoundfontLoaded
	jump_1103be:
		mov    $_CurrentAwe32SoundfontPrefix,%edi
		push   %edi
	jump_1103c4:
		mov    (%esi),%al
		mov    %al,(%edi)
		cmp    $0x0,%al
		je     jump_1103dc
		mov    0x1(%esi),%al
		add    $0x2,%esi
		mov    %al,0x1(%edi)
		add    $0x2,%edi
		cmp    $0x0,%al
		jne    jump_1103c4
	jump_1103dc:
		pop    %edi
		push   %edi
		push   $SoundAndMusicDataPath
		push   $data_161806
		lea    0xc(%esp),%eax
		push   %eax
		call   ac_sprintf
		add    $0x10,%esp
		mov    $0x2,%edx
		mov    %esp,%eax
		call   FileOpenInclCD
		mov    %eax,sbkHandle
		cmp    $0xffffffff,%eax
		je     jump_1105d0
		call   _filelength
		mov    %eax,%esi
		mov    $0x200,%eax
		call   AllocDOSmem_
		mov    %dx,awe32_buffer_add
		mov    %eax,awe32_buffer
		test   %eax,%eax
		jne    jump_110439
		test   %dx,%dx
		je     jump_1105d0
	jump_110439:
		mov    MusicDriver,%eax
		call   func_10ffb0
		cmp    $0xffffffff,%eax
		je     jump_1105d0
		lgs    awe32_buffer,%ebx
		mov    $0x2,%edx
		mov    MusicDriver,%eax
		movl   $0x0,%gs:(%ebx)
		mov    %gs,%ecx
		mov    %esi,%gs:0x4(%ebx)
		call   AWEDefMemMap_
		test   %eax,%eax
		jne    jump_1105d0
		lea    0x14(%esp),%eax
		mov    $0x200,%edx
		mov    awe32_buffer_add,%cx
		push   %eax
		mov    awe32_buffer,%ebx
		mov    sbkHandle,%eax
		call   ___dos_read_
		test   %eax,%eax
		jne    jump_1105d0
		mov    $0x1,%edx
		mov    awe32_buffer_add,%cx
		mov    awe32_buffer,%ebx
		mov    MusicDriver,%eax
		call   AWEGetSFInfo_
		mov    %edx,0x18(%esp)
		mov    %eax,%edi
		test   %eax,%eax
		jne    jump_1104cf
		test   %dx,%dx
		je     jump_1105d0
	jump_1104cf:
		mov    %edx,%gs
		xor    %ebx,%ebx
		mov    %gs:0x2(%eax),%edx
		mov    sbkHandle,%eax
		xor    %esi,%esi
		call   ac_dos_lseek
		jmp    jump_1104e6
	jump_1104e5:
		inc    %esi
	jump_1104e6:
		mov    0x18(%esp),%gs
		movswl %gs:(%edi),%eax
		cmp    %eax,%esi
		jge    jump_110544
		lea    0x14(%esp),%eax
		mov    $0x200,%edx
		mov    awe32_buffer_add,%cx
		push   %eax
		mov    awe32_buffer,%ebx
		mov    sbkHandle,%eax
		call   ___dos_read_
		test   %eax,%eax
		jne    jump_1105d0
		mov    $0x1,%edx
		mov    awe32_buffer_add,%cx
		mov    awe32_buffer,%ebx
		mov    MusicDriver,%eax
		call   AWEStreamSample_
		test   %eax,%eax
		je     jump_1104e5
		add    $0x1c,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret
	jump_110544:
		mov    %gs:0xa(%edi),%eax
		call   AllocDOSmem_
		mov    %dx,awe32_preset_add
		mov    %eax,awe32_preset
		test   %eax,%eax
		jne    jump_110562
		test   %dx,%dx
		je     jump_1105d0
	jump_110562:
		mov    0x18(%esp),%gs
		xor    %ebx,%ebx
		mov    sbkHandle,%eax
		mov    %gs:0x6(%edi),%edx
		call   ac_dos_lseek
		lea    0x14(%esp),%eax
		mov    0x18(%esp),%gs
		mov    awe32_preset_add,%cx
		mov    awe32_preset,%ebx
		push   %eax
		mov    %gs:0xa(%edi),%edx
		mov    sbkHandle,%eax
		call   ___dos_read_
		test   %eax,%eax
		jne    jump_1105d0
		mov    $0x1,%edx
		mov    awe32_preset_add,%cx
		mov    awe32_preset,%ebx
		mov    MusicDriver,%eax
		call   func_110230
		test   %eax,%eax
		jne    jump_1105d0
		mov    $0x1,%cl
		mov    sbkHandle,%eax
		mov    %cl,Awe32SoundfontLoaded
		call   LbFileClose_
	jump_1105d0:
		add    $0x1c,%esp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
FreeAwe32Soundfont:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		cmpb   $0x0,UseCurrentAwe32Soundfont
		jne    jump_110660
		cmpb   $0x0,MusicInstalled
		je     jump_110660
		cmpb   $0x0,MusicAble
		je     jump_110660
		mov    $data_161810,%edx
		mov    $awe32_s1,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_110660
		mov    $data_16181c,%edx
		mov    $MusicType,%eax
		call   ac_stricmp
		test   %eax,%eax
		jne    jump_110660
		mov    $0x1,%edx
		mov    MusicDriver,%eax
		call   AWEFreeMem_
		mov    awe32_buffer_add,%dx
		mov    awe32_buffer,%eax
		call   FreeDOSmem_
		mov    awe32_preset_add,%dx
		mov    awe32_preset,%eax
		xor    %bl,%bl
		call   FreeDOSmem_
		mov    %bl,Awe32SoundfontLoaded
	jump_110660:
		pop    %edx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
func_109170:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %esi
		push   %ebp
		sub    $0x4,%esp
		cmpb   $0x0,SoundInstalled
		je     jump_10939c
		cmpb   $0x0,SoundAble
		je     jump_10939c
		cmpb   $0x0,SoundActive
		je     jump_10939c
		mov    data_1e956c,%cl
		test   %cl,%cl
		je     jump_1091bb
		mov    data_1e9568,%esi
		push   %esi
		call   AIL_release_timer_handle
		add    $0x4,%esp
		jmp    jump_1093cb
	jump_1091bb:
		mov    %cl,data_1e956d
		mov    $sample_id,%ebx
		cmp    end_sample_id,%ebx
		jbe    jump_109375
	jump_1091d2:
		xor    %eax,%eax
		mov    data_1e956d,%al
		test   %eax,%eax
		jne    jump_1093cb
		movb   $0x1,data_1e956c
		xor    %ah,%ah
		mov    %ah,data_1e956e
		add    $0x4,%esp
		pop    %ebp
		pop    %esi
		pop    %ebx
		ret
	jump_1091f7:
		movb   $0x1,data_1e956d
		mov    (%ebx),%esi
		push   %esi
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_1092bd
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		cmp    $0x7f,%eax
		jle    jump_109223
		movw   $0x7f,0xc(%ebx)
	jump_109223:
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		cmp    0x4(%ebx),%eax
		jg     jump_109247
		movb   $0x0,0x14(%ebx)
		cmpb   $0x0,0x15(%ebx)
		je     jump_109247
		mov    (%ebx),%ebp
		push   %ebp
		call   AIL_end_sample
		add    $0x4,%esp
		movb   $0x0,0x15(%ebx)
	jump_109247:
		xor    %eax,%eax
		mov    0x14(%ebx),%al
		cmp    $0x1,%eax
		jne    jump_10938a
		movb   $0x1,(%esp)
		mov    (%esp),%al
		cmp    0x16(%ebx),%al
		jbe    jump_109271
		jmp    jump_1092a9
	jump_109263:
		mov    (%esp),%al
		incb   (%esp)
		mov    (%esp),%al
		cmp    0x16(%ebx),%al
		ja     jump_1092a9
	jump_109271:
		incl   0x4(%ebx)
		cmpl   $0x7f,0x4(%ebx)
		jl     jump_109295
		movl   $0x7f,0x4(%ebx)
		mov    0x4(%ebx),%eax
		push   %eax
		mov    (%ebx),%ecx
		push   %ecx
		call   AIL_set_sample_volume
		add    $0x8,%esp
		jmp    jump_10938a
	jump_109295:
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		cmp    0x4(%ebx),%eax
		jg     jump_109263
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		mov    %eax,0x4(%ebx)
	jump_1092a9:
		mov    0x4(%ebx),%eax
		push   %eax
		mov    (%ebx),%ecx
		push   %ecx
		call   AIL_set_sample_volume
		add    $0x8,%esp
		jmp    jump_10938a
	jump_1092bd:
		movb   $0x0,0x14(%ebx)
		movb   $0x0,0x15(%ebx)
		jmp    jump_10938a
	jump_1092ca:
		movb   $0x1,data_1e956d
		mov    (%ebx),%eax
		push   %eax
		call   AIL_sample_status
		add    $0x4,%esp
		cmp    $0x4,%eax
		jne    jump_1092bd
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		cmp    0x4(%ebx),%eax
		jl     jump_109305
		movb   $0x0,0x14(%ebx)
		cmpb   $0x0,0x15(%ebx)
		je     jump_109305
		mov    (%ebx),%ecx
		push   %ecx
		call   AIL_end_sample
		add    $0x4,%esp
		movb   $0x0,0x15(%ebx)
	jump_109305:
		xor    %eax,%eax
		mov    0x14(%ebx),%al
		cmp    $0x2,%eax
		jne    jump_10938a
		movb   $0x1,(%esp)
		mov    (%esp),%al
		cmp    0x16(%ebx),%al
		jbe    jump_10932f
		jmp    jump_109364
	jump_109321:
		mov    (%esp),%al
		incb   (%esp)
		mov    (%esp),%al
		cmp    0x16(%ebx),%al
		ja     jump_109364
	jump_10932f:
		decl   0x4(%ebx)
		cmpl   $0x0,0x4(%ebx)
		jg     jump_109350
		movl   $0x0,0x4(%ebx)
		mov    0x4(%ebx),%eax
		push   %eax
		mov    (%ebx),%esi
		push   %esi
		call   AIL_set_sample_volume
		add    $0x8,%esp
		jmp    jump_10938a
	jump_109350:
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		cmp    0x4(%ebx),%eax
		jl     jump_109321
		xor    %eax,%eax
		mov    0xc(%ebx),%ax
		mov    %eax,0x4(%ebx)
	jump_109364:
		mov    0x4(%ebx),%eax
		push   %eax
		mov    (%ebx),%esi
		push   %esi
		call   AIL_set_sample_volume
		add    $0x8,%esp
		jmp    jump_10938a
	jump_109375:
		mov    0x14(%ebx),%al
		cmp    $0x1,%al
		jb     jump_10938a
		jbe    jump_1091f7
		cmp    $0x2,%al
		je     jump_1092ca
	jump_10938a:
		mov    end_sample_id,%ebp
		add    $0x19,%ebx
		cmp    %ebp,%ebx
		jbe    jump_109375
		jmp    jump_1091d2
	jump_10939c:
		movb   $0x1,data_1e956c
		xor    %bh,%bh
		mov    %bh,data_1e956e
		mov    $sample_id,%eax
		cmp    end_sample_id,%eax
		ja     jump_1093cb
		xor    %bl,%bl
	jump_1093ba:
		add    $0x19,%eax
		mov    %bl,-0x5(%eax)
		mov    %bl,-0x4(%eax)
		cmp    end_sample_id,%eax
		jbe    jump_1093ba
	jump_1093cb:
		add    $0x4,%esp
		pop    %ebp
		pop    %esi
		pop    %ebx
		ret


/* Part of FadeSample_ (0x1093F0) func, which was removed. No real reason to keep this. */
vtable_1093d4:
		.long   func_1094c9
		.long   func_1094cf
		.long   func_1094d5
		.long   func_1094db
		.long   func_1094e1


/*----------------------------------------------------------------*/
func_1094c9:
/*----------------------------------------------------------------*/
		movb   $0x1,0x16(%ebx)
		jmp    jump_1094e5


/*----------------------------------------------------------------*/
func_1094cf:
/*----------------------------------------------------------------*/
		movb   $0x2,0x16(%ebx)
		jmp    jump_1094e5


/*----------------------------------------------------------------*/
func_1094d5:
/*----------------------------------------------------------------*/
		movb   $0x3,0x16(%ebx)
		jmp    jump_1094e5


/*----------------------------------------------------------------*/
func_1094db:
/*----------------------------------------------------------------*/
		movb   $0x4,0x16(%ebx)
		jmp    jump_1094e5


/*----------------------------------------------------------------*/
func_1094e1:
/*----------------------------------------------------------------*/
		movb   $0x5,0x16(%ebx)
	jump_1094e5:
		xor    %eax,%eax
		mov    data_1e956e,%al
		test   %eax,%eax
		jne    jump_10952c
		xor    %bl,%bl
		mov    %bl,data_1e956c
		mov    $0x1,%bh
		push   $func_109170
		mov    %bh,data_1e956e
		call   AIL_register_timer
		add    $0x4,%esp
		push   $0x14
		push   %eax
		mov    %eax,data_1e9568
		call   AIL_set_timer_frequency
		add    $0x8,%esp
		mov    data_1e9568,%esi
		push   %esi
		call   AIL_start_timer
		add    $0x4,%esp
	jump_10952c:
		add    $0x8,%esp
		pop    %edi
		pop    %esi
		ret    $0x4


/*----------------------------------------------------------------*/
StopAllSampleFadeTimers_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		mov    end_sample_id,%ebx
		cmpb   $0x0,data_1e956e
		je     jump_10958e
		movb   $0x1,data_1e956c
		mov    data_1e9568,%edx
		push   %edx
		call   AIL_release_timer_handle
		mov    end_sample_id,%ebx
		add    $0x4,%esp
		xor    %dh,%dh
		mov    %dh,data_1e956e
		mov    $sample_id,%eax
		cmp    %eax,%ebx
		jb     jump_10958e
	jump_10957f:
		add    $0x19,%eax
		movb   $0x0,-0x5(%eax)
		movb   $0x0,-0x4(%eax)
		cmp    %ebx,%eax
		jbe    jump_10957f
	jump_10958e:
		mov    %ebx,end_sample_id
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
func_1095a0:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %esi
		push   %edi
		push   %ebp
		cmpb   $0x0,sample_queue_handle_stopped
		jne    jump_10964f
		mov    data_1e9570,%eax
		push   %eax
		mov    sample_queue_handle,%edx
		push   %edx
		call   ail_func_104d20
		add    $0x8,%esp
		test   %eax,%eax
		je     jump_10964f
		cmpl   $0x8,data_1e9570
		jae    jump_10964f
		mov    sample_queue_handle,%ecx
		push   %ecx
		call   AIL_init_sample
		add    $0x4,%esp
		push   $func_1095a0
		mov    sample_queue_handle,%esi
		push   %esi
		call   AIL_register_EOS_callback
		add    $0x8,%esp
		push   $0x1
		mov    data_1e9570,%eax
		push   %eax
		mov    sample_queue_handle,%edi
		push   %edi
		call   ail_func_104d20
		mov    %eax,%edx
		mov    Sfx,%eax
		shl    $0x5,%edx
		add    $0x8,%esp
		mov    0x12(%edx,%eax,1),%ebp
		push   %ebp
		mov    sample_queue_handle,%eax
		push   %eax
		call   AIL_set_sample_file
		add    $0xc,%esp
		mov    data_1e9570,%eax
		mov    sample_queue_handle,%edx
		mov    data_1e9570,%ebx
		inc    %ebx
		push   %edx
		mov    %ebx,data_1e9570
		call   AIL_start_sample
		add    $0x4,%esp
	jump_10964f:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
StopSampleQueueList_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %edx
		push   %esi
		push   %edi
		push   %ebp
		cmpb   $0x0,sample_queue_handle_initiated
		je     jump_109862
		mov    $0x1,%dl
		push   $0x0
		mov    %dl,sample_queue_handle_stopped
		mov    sample_queue_handle,%edx
		push   %edx
		call   AIL_register_EOS_callback
		add    $0x8,%esp
		mov    sample_queue_handle,%ebx
		push   %ebx
		call   AIL_end_sample
		add    $0x4,%esp
		push   $0x0
		push   $0x0
		mov    sample_queue_handle,%ecx
		push   %ecx
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x1
		mov    sample_queue_handle,%esi
		push   %esi
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x2
		mov    sample_queue_handle,%edi
		push   %edi
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x3
		mov    sample_queue_handle,%ebp
		push   %ebp
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x4
		mov    sample_queue_handle,%eax
		push   %eax
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x5
		mov    sample_queue_handle,%edx
		push   %edx
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x6
		mov    sample_queue_handle,%ebx
		push   %ebx
		call   AIL_set_sample_user_data
		add    $0xc,%esp
		push   $0x0
		push   $0x7
		mov    sample_queue_handle,%ecx
		push   %ecx
		call   AIL_set_sample_user_data
		add    $0xc,%esp
	jump_109862:
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %edx
		pop    %ecx
		pop    %ebx
		ret


/*----------------------------------------------------------------*/
close_adpcm_file_:
/*----------------------------------------------------------------*/
		push   %ebx
		push   %edx
		mov    data_159e54,%edx
		cmp    $0xffffffff,%edx
		je     jump_1109ff
		mov    %edx,%eax
		mov    $0xffffffff,%ebx
		call   LbFileClose_
		mov    %ebx,data_159e54
	jump_1109ff:
		pop    %edx
		pop    %ebx
		ret


.section .rodata

/* unsigned char sample_queue_handle_stopped
 */
sample_queue_handle_stopped:	/* 0x15958c */
		.byte	0x1
		.align	4

data_15ed3c:
		.string "%d"
data_15ed40:
		.string "DEBUG_BF_AUDIO"
data_15ed4f:
		.string "sound/DIG.INI"
data_15ed5d:
		.string "sound/MDI.INI"
data_15ed6b:
		.string "sound"
data_15ed71:
		.string "sound"
data_15ed77:
		.string "%s/DIG.INI"
data_15ed82:
		.string "%s/MDI.INI"
data_15ed8d:
		.string "BF1  - MA   %d\n"
data_15ed9d:
		.string "BF2  - SA   %d\n"
data_15edad:
		.string "BF54 - MA   %d\n"
data_15edbd:
		.string "BF55 - SA   %d\n"
data_15edcd:
		.string "BF56 - music driver = %s\n"
data_15ede7:
		.string "BF57 - sound driver = %s\n"
		.align	4
data_15ee04:
		.string "BF42 - load sound bank %d\n"
data_15ee1f:
		.string "BF43 - load sound bank - failed - bank not found\n"
data_15ee51:
		.string "BF44 - load sound bank - failed - cannot allocate\n"
data_15ee84:
		.string "BF45 - load sound bank - passed\n"
data_15eea5:
		.string "BF46 - load sound bank - failed - no sound.dat\n"
data_15eed5:
		.string "BF47 - load sound bank - failed - Sound not installed\n"
data_15ef0c:
		.string "BF47 - load sound bank - failed - SoundAble = 0\n"
data_15ef3d:
		.string "BF47 - load sound bank - failed - LoadSounds disabled\n"
data_15ef74:
		.string "BF47 - load sound bank - failed - already loaded\n"
data_15efa6:
		.string "%s/SOUND.DAT"
data_15efb3:
		.string "  Sound Data size = %d  "
data_15efcc:
		.string "BF48 - load music bank %d\n"
data_15efe7:
		.string "BF49 - load music bank - failed - bank not found\n"
data_15f019:
		.string "BF50 - load music bank - failed - cannot allocate\n"
data_15f04c:
		.string "BF51 - load music bank - passed\n"
data_15f06d:
		.string "BF52 - load music bank - failed - no music.dat\n"
data_15f09d:
		.string "BF53 - load music bank - failed - music not installed\n"
data_15f0d4:
		.string "BF53 - load music bank - failed - MusicAble = 0\n"
data_15f105:
		.string "BF53 - load music bank - failed - LoadMusic Disabled = 0\n"
data_15f13f:
		.string "%s/MUSIC.DAT"

data_15f804:
		.string "BF3  - Init sound\n"
data_15f817:
		.string "BF4  - Default sound type -  %d\n"
data_15f838:
		.string "BF5  - No samples requested - AIL shutdown\n"
data_15f864:
		.string "BF6  - No samples requested  \n"
data_15f883:
		.string "MDSOUND"
data_15f88b:
		.string "BF8  - MDSOUND environment active\n"
data_15f8ae:
		.string "%s %x %d %d %d"
data_15f8bd:
		.string "BF9  - MDSOUND environment driver installation - failed\n"
data_15f8f6:
		.string "none"
data_15f8fb:
		.string "%s"
data_15f8fe:
		.string "BF10 - MDSOUND environment driver installation - passed\n"
data_15f937:
		.string "BF11 - Search for DIG.INI - passed   \n"
data_15f95e:
		.string "None"
data_15f963:
		.string "BF12 - user requests no sound in SETSOUND   \n"
data_15f991:
		.string "BF13 - DIG.INI driver installation - passed\n"
data_15f9be:
		.string "none"
data_15f9c3:
		.string "BF14 - DIG.INI driver installation - \n"
data_15f9ea:
		.string "none"
data_15f9ef:
		.string "BF15 - Search for DIG.INI - failed\n"
data_15fa13:
		.string "BF16 - all dig driver installation attempts failed\n"
data_15fa47:
		.string "BF18 - determine sound type to be loaded\n"
data_15fa71:
		.string "BF19 - LoadSounds disabled\n"
data_15fa8d:
		.string "BF20 - cannot allocate for digital samples\n"
data_15fab9:
		.string "none"
data_15fabe:
		.string "BF24 - Init sound completed\n"
data_15fadb:
		.string "BF19 - %d"
data_15fae5:
		.string " - no sound.dat\n"
data_15faf6:
		.string " - cannot allocate\n"
data_15fb0a:
		.string " - not present in sound.dat\n"
data_15fb27:
		.string " - allocation successful\n"
		.align	4
data_15fb44:
		.string "BF25 - Init Music\n"
data_15fb57:
		.string "%s/SAMPLE"
data_15fb61:
		.string "%s.ad"
data_15fb67:
		.string "BF26 - sample.ad not found\n"
data_15fb83:
		.string "%s.opl"
data_15fb8a:
		.string "BF27 - sample.opl not found\n"
data_15fba7:
		.string "MDMUSIC"
data_15fbaf:
		.string "BF29 - MDMUSIC environment active\n"
data_15fbd2:
		.string "%s %x %d %d %d"
data_15fbe1:
		.string "BF30 - MDMUSIC environment driver installation - failed\n"
data_15fc1a:
		.string "BF31 - MDMUSIC environment driver installation - passed\n"
data_15fc53:
		.string "%s"
data_15fc56:
		.string "BF32 - Search for MDI.INI - passed\n"
data_15fc7a:
		.string "None"
data_15fc7f:
		.string "BF33 - user requests no music in SETSOUND\n"
data_15fcaa:
		.string "BF34 - MDI.INI driver installation - passed\n"
data_15fcd7:
		.string "BF35 - MDI.INI driver installation - failed\n"
data_15fd04:
		.string "BF36 - Search for MDI.INI - failed\n"
data_15fd28:
		.string "BF37 - all music driver installation attempts failed\n"
data_15fd5e:
		.string "ADLIB.MDI"
data_15fd68:
		.string "f"
data_15fd6a:
		.string "ADLIBG.MDI"
data_15fd75:
		.string "f"
data_15fd77:
		.string "OPL3.MDI"
data_15fd80:
		.string "f"
data_15fd82:
		.string "PAS.MDI"
data_15fd8a:
		.string "f"
data_15fd8c:
		.string "PASPLUS.MDI"
data_15fd98:
		.string "f"
data_15fd9a:
		.string "SBLASTER.MDI"
data_15fda7:
		.string "f"
data_15fda9:
		.string "SBPRO1.MDI"
data_15fdb4:
		.string "f"
data_15fdb6:
		.string "SBPRO2.MDI"
data_15fdc1:
		.string "f"
data_15fdc3:
		.string "ESFM.MDI"
data_15fdcc:
		.string "f"
data_15fdce:
		.string "SBAWE32.MDI"
data_15fdda:
		.string "w"
data_15fddc:
		.string "g"
data_15fdde:
		.string "BF38 - awe32 soundfont not found\n"
data_15fe00:
		.string "BF38 - awe32 soundfont loaded\n"
data_15fe1f:
		.string "MPU401.MDI"
data_15fe2a:
		.string "g"
data_15fe2c:
		.string "SNDSCAPE.MDI"
data_15fe39:
		.string "g"
data_15fe3b:
		.string "ULTRA.MDI"
data_15fe45:
		.string "g"
data_15fe47:
		.string "MT32MPU.MDI"
data_15fe53:
		.string "r"
data_15fe55:
		.string "g"
data_15fe57:
		.string "f"
data_15fe59:
		.string "BF39 - music driver type not supported\n"
data_15fe81:
		.string "BF40 - music%s bank allocation"
data_15fea0:
		.string "w"
data_15fea2:
		.string " - not present in music.dat\n"
data_15febf:
		.string "g"
data_15fec1:
		.string "BF40A - music%s bank allocation"
data_15fee1:
		.string " - no music.dat\n"
data_15fef2:
		.string " - cannot allocate\n"
data_15ff06:
		.string " - not present in music.dat\n"
data_15ff23:
		.string " - allocation successful\n"
data_15ff3d:
		.string "BF40 - LoadMusic Disabled\n"
data_15ff58:
		.string "BF41 - Init music completed\n"
		.align	4
data_15ff78:
		.string "BF100 - Cannot allocate buffers for streamed sound\n"
data_15ffac:
		.string "BF100 - Cannot allocate handle for streamed sound\n"
data_15ffdf:
		.string "BF100 - Unknown streamed sample type %d\n"
data_160008:
		.string "BF101 - cannot open file %s\n"
data_160025:
		.string "BF101 - cannot open file %s\n"
data_160042:
		.string "BF103 - oversized ADPCM chunks (%ld)\n"
data_160068:
		.string "Got adpcm block"

data_1617f8:
		.string "SBAWE32.MDI"
data_161804:
		.ascii  "\x77\x00"
data_161806:
		.string "%s/%s.sbk"
data_161810:
		.string "SBAWE32.MDI"
data_16181c:
		.ascii  "\x77\x00\x00\x00"

.data

data_158b34:
		.byte	0xff
CDAble:
		.ascii  "\x01\x00\x00"

MaxNumberOfSamples: /* 0x159300 */
		.long	0x0a
GLOBAL (SoundAble)	/* 159304 */
		.byte	0x1
SixteenBit:
		.byte	0x1
SoundActive:
		.byte	0x1
AutoScanForSoundHardware:
		.byte	0x1
StereoSound:
		.byte	0x1
CurrentSoundBank:
		.byte	0x0ff
		.ascii  "\x00\x00"
CurrentSoundMasterVolume:
		.long	0x7f
DirectSoundUsingGameSDKWindow:
		.long	0x01
CurrentMusicMasterVolume:
		.long	0x7f
MusicAble:
		.byte	0x1
MusicActive:
		.byte	0x1
CurrentDangerMusicFadeDirection:
		.byte	0x1
DangerMusicVolumeChange:
		.byte	0xff
sample_file:
		.ascii  "\xff\xff\xff\xff"
sample_volume:
		.long	0x7f
active:
		.long	0x01

_CurrentAwe32SoundfontPrefix:
		.string "Bullfrog"
		.align	4
data_159e54:
		.ascii  "\xff\xff\xff\xff"
data_159e58:
		.ascii  "\xe6\x00\x00\x00\xe6\x00\x00\x00"
		.ascii  "\xe6\x00\x00\x00\xe6\x00\x00\x00"
		.ascii  "\x33\x01\x00\x00\x99\x01\x00\x00"
		.ascii  "\x00\x02\x00\x00\x66\x02\x00\x00"
		.ascii  "\x00\x03\x00\x00\x66\x02\x00\x00"
		.ascii  "\x00\x02\x00\x00\x99\x01\x00\x00"
		.ascii  "\x33\x01\x00\x00\xe6\x00\x00\x00"
		.ascii  "\xe6\x00\x00\x00\xe6\x00\x00\x00"

GLOBAL (FullDIG_INIPath)		/* 1e5a30 */
		.fill   0x90
GLOBAL (FullMDI_INIPath)		/* 1e5ac0 */
		.fill   0x90
SoundAndMusicDataPath:
		.fill   0x90
GLOBAL (SoundDriverPath)		/* 1e5be0 */
		.fill   0x90
current_SB16_left_master_volume:
		.short  0x0
current_SB16_right_master_volume:
		.short  0x0
old_SB16_left_midi_volume:
		.short  0x0
old_SB16_right_midi_volume:
		.short  0x0
old_SB16_left_digi_volume:
		.short  0x0
old_SB16_right_digi_volume:
		.short  0x0
old_SB16_left_treble_volume:
		.short  0x0
old_SB16_right_treble_volume:
		.short  0x0
old_SB16_left_bass_volume:
		.short  0x0
old_SB16_right_bass_volume:
		.short  0x0
sb16_mixer_set:
		.byte	0x0
		.fill   0x3
CurrentCDTrack:
		.short  0x0
		.fill   0x2
TrackLength:
		.long	0x0
CDCountdown:
		.long	0x0
CDTimerActive:
		.byte	0x0
		.fill   0x3
CDCount_handle:
		.long	0x0
end_time:
		.long	0x0

data_1e5ca4:
		.long	0x0
data_1e5ca8:
		.long	0x0
/* SoundBankSizes sound_bank_size_info;
 */
sound_bank_size_info_Bnk0Fld0:
		.long	0x0
sound_bank_size_info_Bnk0Fld1:
		.long	0x0
sound_bank_size_info_Bnk1Fld0:
		.long	0x0
sound_bank_size_info_Bnk1Fld1:
		.long	0x0
sound_bank_size_info_Bnk2Fld0:
		.long	0x0
sound_bank_size_info_Bnk2Fld1:
		.long	0x0
sound_bank_size_info_Bnk3Fld0:
		.long	0x0
sound_bank_size_info_Bnk3Fld1:
		.long	0x0
sound_bank_size_info_Bnk4Fld0:
		.long	0x0
sound_bank_size_info_Bnk4Fld1:
		.long	0x0
sound_bank_size_info_Bnk5Fld0:
		.long	0x0
sound_bank_size_info_Bnk5Fld1:
		.long	0x0
sound_bank_size_info_Bnk6Fld0:
		.long	0x0
sound_bank_size_info_Bnk6Fld1:
		.long	0x0
sound_bank_size_info_Bnk7Fld0:
		.long	0x0
sound_bank_size_info_Bnk7Fld1:
		.long	0x0
sound_bank_size_info_Bnk8Fld0:
		.long	0x0
sound_bank_size_info_Bnk8Fld1:
		.long	0x0
/* long largest_dat_size;
 */
largest_dat_size:
		.long	0x0
/* long largest_tab_size;
 */
largest_tab_size:
		.long	0x0
full_sound_data_path:
		.fill   0xe0
/* MusicBankSizes music_bank_size_info;
 */
music_bank_size_info__Field0:
		.long	0x0
music_bank_size_info__Field1:
		.long	0x0
music_bank_size_info__Field2:
		.long	0x0
music_bank_size_info__Field3:
		.long	0x0
music_bank_size_info__Field4:
		.long	0x0
music_bank_size_info__Field5:
		.long	0x0
music_bank_size_info__Field6:
		.long	0x0
music_bank_size_info__Field7:
		.long	0x0
full_music_data_path:
		.fill   0x90

DebugAudio: /* 0x1E86E0 */
		.long	0x0

GLOBAL (SoundProgressMessage) /* 0x1E86E4 */
		.fill   0x100
sample_id:
		.fill   0x320
end_sample_id:
		.long	0x0
SoundInstalled:
		.byte	0x0
DisableLoadSounds:
		.byte	0x0
NumberOfSamples:
		.short  0x0
Sfx:
		.long	0x0
EndSfxs:
		.long	0x0
SfxData:
		.long	0x0
SoundType:
		.short  0x0
		.fill   0x2
SoundDriver:
		.long	0x0
AILStartupAlreadyInitiated:
		.byte   0x0
		.fill   0x3
SampleRate:
		.long	0x0
ive_got_an_sb16:
		.byte   0x0
		.fill   0x1
Selected95WaveDevice:
		.short  0x0
SB16_io:
		.long	0x0
UseMultiMediaExtensions:
		.byte   0x0
		.fill   0x3
SoundInstallChoice__Field0:
		.fill   0x80
SoundInstallChoice__Field1:
		.fill   0x80
SoundInstallChoice__Field2:
		.fill   0x18
music_allocated:
		.long	0x0
UseCurrentAwe32Soundfont:
		.byte	0x0
MusicInstalled:
		.byte	0x0
		.fill   0x2
MusicType:
		.fill   0x6
CurrentTempo:
		.short  0x0
SongCurrentlyPlaying:
		.short  0x0
NumberOfSongs:
		.short  0x0
Music:
		.long	0x0
EndMusic:
		.long	0x0
MusicData:
		.long	0x0

MusicDriver:
		.long	0x0
SongHandle:
		.long	0x0
DangerMusicFadeHandle:
		.long	0x0
Awe32SoundfontLoaded:
		.byte	0x0
DisableLoadMusic:
		.short  0x0
ive_got_an_awe32:
		.byte	0x0
DangerMusicVolume:
		.byte	0x0
DangerMusicFadeActive:
		.byte	0x0
DangerMusicAble:
		.fill   0x6

data_1e8c84:
		.fill   0x80
awe32_s1:	/* 0x1e8d04 */
		.fill   0x98

data_1e5eac:	/* 0x1e5eac */
		.byte	0x0
		.align	4

data_1e5edc:	/* 0x1e5edc */
		.fill   0x10

/* sound_timer_inf sound_timer[];
 */
sound_timer:
		.long	0x0
data_1e8da0: /* 0x1e8da0 */
		.fill   0x15
data_1e8db5:
		.ascii  "\x00\x00\x00"
data_1e8db8:
		.fill   0x90
StreamedSoundAble: /* 0x1e8e48 */
		.byte	0x0
data_1e8e49:
		.ascii  "\x00\x00\x00"
data_1e8e4c:
		.fill   0x104
data_1e8f50:
		.long	0x0
data_1e8f54:
		.long	0x0
data_1e8f58:
		.long	0x0
data_1e8f5c:
		.long	0x0
data_1e8f60:
		.long	0x0
data_1e8f64:
		.fill   0x8
data_1e8f6c:
		.long	0x0
adpcm_file_open:
		.long	0x0
data_1e8f74:
		.long	0x0
data_1e8f78:
		.long	0x0
data_1e8f7c:
		.long	0x0
data_1e8f80:
		.long	0x0
data_1e8f84:
		.long	0x0
data_1e8f88:
		.long	0x0
data_1e8f8c:
		.long	0x0
data_1e8f90:
		.long	0x0
data_1e8f94: /* 0x1e8f94 */
		.long	0x0
mixed_file_open:
		.byte	0x0
		.fill   0x7

data_1e9568:	/* 0x1e9568 */
		.long	0x0
data_1e956c:
		.byte	0x0
data_1e956d:
		.byte	0x0
data_1e956e:
		.short  0x0
data_1e9570:
		.long	0x0
/* unsigned char sample_queue_handle_initiated
 */
sample_queue_handle_initiated:
		.long	0x0
/* _SAMPLE *sample_queue_handle
 */
sample_queue_handle:
		.long	0x0

sbkHandle:	/* 0x1ED5F0 */
		.long	0x0
/* unsigned char *awe32_buffer;
 */
awe32_buffer:
		.long	0x0
awe32_buffer_add:
		.short	0x0
		.align	4
/* unsigned char *awe32_preset;
 */
awe32_preset:
		.long	0x0
awe32_preset_add:
		.short	0x0
		.align	4
/* __38rogeSF_DATA *awe32_sf_data;
 */
awe32_sf_data:
		.long	0x0
awe32_sf_data_add:
		.short	0x0
		.align	4


