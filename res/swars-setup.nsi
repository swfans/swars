; Syndicate Wars Port install script for NSIS-2
; Use it to generate Syndicate Wars Port installer
; Requires:
; - Simple CD-DA Audio Track ripper by j00ru//vx (rip.exe+akrip32.dll)
; - OGG Encoder from Vorbis-tools (oggenc.exe+accompanying DLLs)
; - SW Port executable and shared libs (swars.exe+SDL.dll,openal32.dll,libpng3.dll,libogg-0.dll,libvorbis-0.dll)
; If you don't have any of these, you may extract them using 7Zip on any existing SW Port Installer.


SetCompressor zlib

OutFile swars-setup.exe
!include WinMessages.nsh
!include InstallOptions.nsh
!include FileFunc.nsh
!insertmacro GetDrives

ShowInstDetails show


LicenseLangString LIC_ENG 1033 license-eng.txt

Name "Syndicate Wars"
BrandingText "SW port, by fans for fans"

LangString STR_CHOOSE_DRIVE 1033 "Choose the CD-ROM drive"
LangString STR_CHOOSE_LANG 1033 "Choose the language"

; --------------------
; VARIABLES: 8

Var selected_lang_text
Var selected_lang_abbr
Var inst_src_lang_dir
Var selected_menu_shortcut
Var selected_desk_shortcut
Var selected_music

Var inst_src_root_dir


InstallDir "$PROGRAMFILES\Syndicate Wars\"


; --------------------
; PAGES: 6

Page license
  LicenseData $(LIC_ENG)

Page custom LangNCdromPage LangNCdromLeave

Page components

Page directory

Page instfiles

/*
; Page 5
Page COMPLETED
*/


Function WriteToFile
  Exch $0 ; file to write to
  Exch
  Exch $1 ; text to write
  FileOpen $0 $0 w
  FileSeek $0 0 END
  FileWrite $0 $1
  FileClose $0
  Pop $1
  Pop $0
FunctionEnd


Section "Syndicate Wars Game" ; Section_0
  ; AddSize 4176
  SetOutPath $INSTDIR
  Call CopyGameFilesFromCD
  IfErrors inst_game_fail
  Call RemoveSound
  IfErrors inst_game_fail
  Call InstallConfFile
  IfErrors inst_game_fail
  Call InstallRegistry
  IfErrors inst_game_fail
  File swars.exe
  File openal32.dll
  File wrap_oal.dll
  File libpng3.dll
  File zlib1.dll
  File SDL.dll
  WriteUninstaller $INSTDIR\unistall.exe
  StrCmp $selected_menu_shortcut 1 0 inst_game_menu_end
  CreateDirectory "$SMPROGRAMS\Vexillium"
  CreateShortCut "$SMPROGRAMS\Vexillium\Syndicate Wars.lnk" "$INSTDIR\swars.exe" "" "$INSTDIR\swars.exe" "" SW_SHOWNORMAL
  CreateShortCut "$SMPROGRAMS\Vexillium\Uninstall.lnk" "$INSTDIR\unistall.exe" "" "$INSTDIR\unistall.exe" "" SW_SHOWNORMAL
inst_game_menu_end:
  StrCmp $selected_desk_shortcut 1 0 inst_game_done
  CreateShortCut "$DESKTOP\Syndicate Wars.lnk" $INSTDIR\swars.exe "" $INSTDIR\swars.exe "" SW_SHOWNORMAL
inst_game_done:
  Return

inst_game_fail:
  DetailPrint "An unexpected error has occured during the installation process"
  RMDir /r $INSTDIR
  IntOp $selected_music 1 | ""
SectionEnd


Section "Game music" ; Section_1
  ; AddSize 1904
  StrCmp $selected_music 1 0 inst_music_go
  Goto inst_music_done
inst_music_go:
  File $PLUGINSDIR\rip.exe
  File $PLUGINSDIR\akrip32.dll
  File $PLUGINSDIR\oggenc.exe
  File $PLUGINSDIR\libogg.dll
  File $PLUGINSDIR\libvorbis.dll
  File $PLUGINSDIR\libvorbisfile.dll
  File $PLUGINSDIR\libFLAC_dynamic.dll
  File $PLUGINSDIR\oggenc_LICENSE
  CreateDirectory $INSTDIR\music
  ExecWait "$PLUGINSDIR\rip.exe $inst_src_root_dir $\"$INSTDIR\music$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_1.ogg$\" $\"$INSTDIR\music\track1.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_2.ogg$\" $\"$INSTDIR\music\track2.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_3.ogg$\" $\"$INSTDIR\music\track3.wav$\""
  Delete $INSTDIR\music\*.wav
  IfErrors inst_music_fail
  Goto inst_music_done
inst_music_fail:
  DetailPrint "An unexpected error has occured during the installation process"
  RMDir /r $INSTDIR
inst_music_done:
SectionEnd

Function .onInit
  !insertmacro INSTALLOPTIONS_EXTRACT "lang_n_cdrom.ini"
  !insertmacro INSTALLOPTIONS_WRITE "lang_n_cdrom.ini" "Field 3" State "$(STR_CHOOSE_DRIVE)"
FunctionEnd

Function LangNCdromPage
  StrCpy $R2 0
  StrCpy $R0 "$(STR_CHOOSE_DRIVE)|"
  ${GetDrives} "CDROM" GetDrivesCallBack
  !insertmacro INSTALLOPTIONS_WRITE "lang_n_cdrom.ini" "Field 3" ListItems $R0
  !insertmacro INSTALLOPTIONS_DISPLAY "lang_n_cdrom.ini"
FunctionEnd


Function LangNCdromLeave
  !insertmacro INSTALLOPTIONS_READ $selected_lang_text "lang_n_cdrom.ini" "Field 2" State
  StrCmp $selected_lang_text "$(STR_CHOOSE_LANG)" langncd_no_lang
  !insertmacro INSTALLOPTIONS_READ $inst_src_root_dir "lang_n_cdrom.ini" "Field 3" State
  StrCmp $inst_src_root_dir "$(STR_CHOOSE_DRIVE)" langncd_no_drive
  Goto langcd_lngswitch
langcd_lng_eng:
  StrCpy $selected_lang_abbr eng
  Goto langncd_store
langcd_lng_fre:
  StrCpy $selected_lang_abbr fre
  Goto langncd_store
langcd_lng_ita:
  StrCpy $selected_lang_abbr ita
  Goto langncd_store
langcd_lng_spa:
  StrCpy $selected_lang_abbr spa
  Goto langncd_store
langcd_lng_swe:
  StrCpy $selected_lang_abbr swe
  Goto langncd_store
  Goto langncd_store
langcd_lngswitch:
  StrCmp $selected_lang_text "English" langcd_lng_eng
  StrCmp $selected_lang_text "French" langcd_lng_fre
  StrCmp $selected_lang_text "Italian" langcd_lng_ita
  StrCmp $selected_lang_text "Spanish" langcd_lng_spa
  StrCmp $selected_lang_text "Swedish" langcd_lng_swe
langncd_store:
  !insertmacro INSTALLOPTIONS_READ $selected_menu_shortcut "lang_n_cdrom.ini" "Field 4" State
  !insertmacro INSTALLOPTIONS_READ $selected_desk_shortcut "lang_n_cdrom.ini" "Field 5" State
  Return

langncd_no_lang:
  MessageBox MB_OK|MB_ICONEXCLAMATION "Please select a valid installation language!"
  Abort
langncd_no_drive:
  MessageBox MB_OK|MB_ICONEXCLAMATION "Please select a valid CD-ROM drive!"
  Abort
FunctionEnd


Function GetDrivesCallBack
     StrCpy $R0 '$R0$9|'
	 Push $0
FunctionEnd

Function CopyGameFilesFromCD
  DetailPrint "Copying the game files..."
  CreateDirectory $INSTDIR\data
  CopyFiles /SILENT $inst_src_root_dir\GAME\data\* $INSTDIR\data
  CreateDirectory $INSTDIR\qdata
  CopyFiles /SILENT $inst_src_root_dir\GAME\qdata\* $INSTDIR\qdata
  CreateDirectory $INSTDIR\intro
  CreateDirectory $INSTDIR\sound
  CopyFiles /SILENT $inst_src_root_dir\GAME\sound\*.dat $INSTDIR\sound
  CreateDirectory $INSTDIR\levels
  CopyFiles /SILENT $inst_src_root_dir\GAME\levels\* $INSTDIR\levels
  CreateDirectory $INSTDIR\maps
  CopyFiles /SILENT $inst_src_root_dir\GAME\maps\* $INSTDIR\maps
  IfErrors copy_files_fail
  StrCpy $inst_src_lang_dir $inst_src_root_dir\GAME\language\$selected_lang_abbr
  CopyFiles /SILENT $inst_src_lang_dir\text.dat $INSTDIR\data
  CopyFiles /SILENT $inst_src_lang_dir\syn_ele.smk $INSTDIR\data
  CopyFiles /SILENT $inst_src_lang_dir\chu_ele.smk $INSTDIR\data
  CopyFiles /SILENT $inst_src_lang_dir\alltext.wad $INSTDIR\qdata
  CopyFiles /SILENT $inst_src_lang_dir\alltext.idx $INSTDIR\qdata
  CopyFiles /SILENT $inst_src_lang_dir\intro.smk $INSTDIR\intro
  CopyFiles /SILENT $inst_src_lang_dir\sound.dat $INSTDIR\sound
  Return

copy_files_fail:
  SetErrors
  Return

FunctionEnd


Function RemoveSound
  RMDir /r $INSTDIR\qdata\sound
FunctionEnd


Function InstallConfFile
  DetailPrint "Creating initial configuration file..."
  Push CD=$\"D:\GAME\$\"$\r$\nInstallDrive=$\"C:\$\"$\r$\nLanguage=$\"$selected_lang_text$\"$\r$\nIntro=$\"Max$\"$\r$\nAnims=$\"Max$\"$\r$\nSound=$\"Max$\"$\r$\nLevels=$\"Max$\"$\r$\nData=$\"Max$\"$\r$\nMaps=$\"Max$\"$\r$\nDOS=$\"Max$\"$\r$\n
  Push $INSTDIR\config.ini
  Call WriteToFile
FunctionEnd


Function InstallRegistry
  DetailPrint "Adding the installation path to registry..."
  WriteRegStr HKLM "Software\Syndicate Wars\CurrentVersion" InstallPath $INSTDIR
FunctionEnd


; Uninstall section
Section Uninstall
; TODO
SectionEnd

