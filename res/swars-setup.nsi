; NSIS script NSIS-2
; Install

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

LangString LSTR_2 1033 "Syndicate Wars"
LangString LSTR_20 1033 "Execute: "
LangString LSTR_21 1033 "Extract: "
LangString LSTR_22 1033 "Extract: error writing to file "
LangString LSTR_23 1033 "Installer corrupted: invalid opcode"
LangString LSTR_24 1033 "No OLE for: "
LangString LSTR_25 1033 "Output folder: "
LangString LSTR_26 1033 "Remove folder: "
LangString LSTR_27 1033 "Rename on reboot: "
LangString LSTR_28 1033 "Rename: "
LangString LSTR_29 1033 "Skipped: "
LangString LSTR_30 1033 "Copy Details To Clipboard"
LangString LSTR_59 1033 "Show &details"
LangString LSTR_60 1033 "Completed"
LangString LSTR_61 1033 ": Completed"
LangString LSTR_62 1033 "&Close"
LangString LSTR_63 1033 "Syndicate Wars"


; --------------------
; VARIABLES: 8

Var _0_
Var _1_
Var _2_
Var _3_
Var _4_
Var _5_
Var _6_
Var _7_


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


; --------------------
; SECTIONS: 2
; COMMANDS: 294

Function func_0
  Exch $0
    ; Push $0
    ; Exch
    ; Pop $0
  Exch
  Exch $1
    ; Push $1
    ; Exch
    ; Pop $1
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
  Call func_245
  IfErrors label_37
  Call RemoveSound
  IfErrors label_37
  Call InstallConfFile
  IfErrors label_37
  Call InstallRegistry
  IfErrors label_37
  File swars.exe
  File openal32.dll
  File wrap_oal.dll
  File libpng3.dll
  File zlib1.dll
  File SDL.dll
  WriteUninstaller $INSTDIR\unistall.exe ;  $INSTDIR\$INSTDIR\unistall.exe
  StrCmp $_3_ 1 0 label_34
  CreateDirectory "$SMPROGRAMS\Vexillium"
  CreateShortCut "$SMPROGRAMS\Vexillium\Syndicate Wars.lnk" "$INSTDIR\swars.exe" "" "$INSTDIR\swars.exe" "" SW_SHOWNORMAL
  CreateShortCut "$SMPROGRAMS\Vexillium\Uninstall.lnk" "$INSTDIR\unistall.exe" "" "$INSTDIR\unistall.exe" "" SW_SHOWNORMAL
label_34:
  StrCmp $_4_ 1 0 label_36
  CreateShortCut "$DESKTOP\Syndicate Wars.lnk" $INSTDIR\swars.exe "" $INSTDIR\swars.exe "" SW_SHOWNORMAL
label_36:
  Return

label_37:
  DetailPrint "An unexpected error has occured during the installation process"
  RMDir /r $INSTDIR
  IntOp $_5_ 1 | ""
SectionEnd


Section "Game music" ; Section_1
  ; AddSize 1904
  StrCmp $_5_ 1 0 label_43
  Goto label_61
label_43:
  File $PLUGINSDIR\rip.exe
  File $PLUGINSDIR\akrip32.dll
  File $PLUGINSDIR\oggenc.exe
  File $PLUGINSDIR\libogg.dll
  File $PLUGINSDIR\libvorbis.dll
  File $PLUGINSDIR\libvorbisfile.dll
  File $PLUGINSDIR\libFLAC_dynamic.dll
  File $PLUGINSDIR\oggenc_LICENSE
  CreateDirectory $INSTDIR\music
  ExecWait "$PLUGINSDIR\rip.exe $_7_ $\"$INSTDIR\music$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_1.ogg$\" $\"$INSTDIR\music\track1.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_2.ogg$\" $\"$INSTDIR\music\track2.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_3.ogg$\" $\"$INSTDIR\music\track3.wav$\""
  Delete $INSTDIR\music\*.wav
  IfErrors label_59
  Goto label_61
label_59:
  DetailPrint "An unexpected error has occured during the installation process"
  RMDir /r $INSTDIR
label_61:
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
  !insertmacro INSTALLOPTIONS_READ $_0_ "lang_n_cdrom.ini" "Field 2" State
  StrCmp $_0_ "$(STR_CHOOSE_LANG)" label_237
  !insertmacro INSTALLOPTIONS_READ $_7_ "lang_n_cdrom.ini" "Field 3" State
  StrCmp $_7_ "$(STR_CHOOSE_DRIVE)" label_239
  Goto label_229
label_218:
  StrCpy $_1_ eng
  Goto label_234
label_220:
  StrCpy $_1_ fre
  Goto label_234
label_222:
  StrCpy $_1_ ita
  Goto label_234
label_224:
  StrCpy $_1_ spa
  Goto label_234
label_226:
  StrCpy $_1_ swe
  Goto label_234
  Goto label_234
label_229:
  StrCmp $_0_ English label_218
  StrCmp $_0_ French label_220
  StrCmp $_0_ Italian label_222
  StrCmp $_0_ Spanish label_224
  StrCmp $_0_ Swedish label_226
label_234:
  !insertmacro INSTALLOPTIONS_READ $_3_ "lang_n_cdrom.ini" "Field 4" State
  !insertmacro INSTALLOPTIONS_READ $_4_ "lang_n_cdrom.ini" "Field 5" State
  Return

label_237:
  MessageBox MB_OK|MB_ICONEXCLAMATION "Please select a valid installation language!"
  Abort
label_239:
  MessageBox MB_OK|MB_ICONEXCLAMATION "Please select a valid CD-ROM drive!"
  Abort
  Return

  StrCpy $_6_ $_6_|$9
  Push $0
FunctionEnd


Function GetDrivesCallBack
     StrCpy $R0 '$R0$9|'
	 Push $0
FunctionEnd

Function func_245
  DetailPrint "Copying the game files..."
  CreateDirectory $INSTDIR\data
  CopyFiles /SILENT $_7_\GAME\data\* $INSTDIR\data    ; $(LSTR_7)$INSTDIR\data    ;  "Copy to "
  CreateDirectory $INSTDIR\qdata
  CopyFiles /SILENT $_7_\GAME\qdata\* $INSTDIR\qdata    ; $(LSTR_7)$INSTDIR\qdata    ;  "Copy to "
  CreateDirectory $INSTDIR\intro
  CreateDirectory $INSTDIR\sound
  CopyFiles /SILENT $_7_\GAME\sound\*.dat $INSTDIR\sound    ; $(LSTR_7)$INSTDIR\sound    ;  "Copy to "
  CreateDirectory $INSTDIR\levels
  CopyFiles /SILENT $_7_\GAME\levels\* $INSTDIR\levels    ; $(LSTR_7)$INSTDIR\levels    ;  "Copy to "
  CreateDirectory $INSTDIR\maps
  CopyFiles /SILENT $_7_\GAME\maps\* $INSTDIR\maps    ; $(LSTR_7)$INSTDIR\maps    ;  "Copy to "
  IfErrors label_267
  StrCpy $_2_ $_7_\GAME\language\$_1_
  CopyFiles /SILENT $_2_\text.dat $INSTDIR\data    ; $(LSTR_7)$INSTDIR\data    ;  "Copy to "
  CopyFiles /SILENT $_2_\syn_ele.smk $INSTDIR\data    ; $(LSTR_7)$INSTDIR\data    ;  "Copy to "
  CopyFiles /SILENT $_2_\chu_ele.smk $INSTDIR\data    ; $(LSTR_7)$INSTDIR\data    ;  "Copy to "
  CopyFiles /SILENT $_2_\alltext.wad $INSTDIR\qdata    ; $(LSTR_7)$INSTDIR\qdata    ;  "Copy to "
  CopyFiles /SILENT $_2_\alltext.idx $INSTDIR\qdata    ; $(LSTR_7)$INSTDIR\qdata    ;  "Copy to "
  CopyFiles /SILENT $_2_\intro.smk $INSTDIR\intro    ; $(LSTR_7)$INSTDIR\intro    ;  "Copy to "
  CopyFiles /SILENT $_2_\sound.dat $INSTDIR\sound    ; $(LSTR_7)$INSTDIR\sound    ;  "Copy to "
  Return

label_267:
  SetErrors
  Return

FunctionEnd


Function RemoveSound
  RMDir /r $INSTDIR\qdata\sound
FunctionEnd


Function InstallConfFile
  DetailPrint "Creating initial configuration file..."
  Push CD=$\"D:\GAME\$\"$\r$\nInstallDrive=$\"C:\$\"$\r$\nLanguage=$\"$_0_$\"$\r$\nIntro=$\"Max$\"$\r$\nAnims=$\"Max$\"$\r$\nSound=$\"Max$\"$\r$\nLevels=$\"Max$\"$\r$\nData=$\"Max$\"$\r$\nMaps=$\"Max$\"$\r$\nDOS=$\"Max$\"$\r$\n
  Push $INSTDIR\config.ini
  Call func_0
FunctionEnd


Function InstallRegistry
  DetailPrint "Adding the installation path to registry..."
  WriteRegStr HKLM "Software\Syndicate Wars\CurrentVersion" InstallPath $INSTDIR
FunctionEnd


; Uninstall section
Section Uninstall
SectionEnd

