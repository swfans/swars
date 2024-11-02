; Syndicate Wars Port install script for NSIS-2
; Use it to generate Syndicate Wars Port installer
; Requires:
; - Simple CD-DA Audio Track ripper by j00ru//vx (rip.exe+akrip32.dll)
; - OGG Encoder from Vorbis-tools (oggenc.exe+accompanying DLLs)
; - GoG ISO Ripper by Moburma (Gogisoripper.exe)
; - SW Port executable, shared libs, lang files and config files (swars.exe+*.dll+conf/*+lang/*)
; If you don't have any of these, you may extract them using 7Zip on any existing SW Port Installer.


SetCompressor zlib

OutFile swars-setup.exe
!include WinMessages.nsh
!include InstallOptions.nsh
!include FileFunc.nsh
!insertmacro GetDrives
!include "MUI2.nsh"
!include x64.nsh
!include swars-config.nsh

ShowInstDetails show


LicenseLangString LIC_ENG 1033 license-eng.txt

Name "Syndicate Wars"
BrandingText "SW port, by fans for fans"

LangString STR_CHOOSE_DRIVE 1033 "Choose the CD-ROM drive"
LangString STR_CHOOSE_LANG 1033 "Choose the language"

; --------------------
; VARIABLES: 12

Var selected_lang_text
Var selected_lang_abbr
Var inst_src_lang_dir
Var selected_menu_shortcut
Var selected_desk_shortcut
Var selected_music
Var gog_path
Var update_path
Var RadioSelected
Var inst_src_root_dir
Var levels_md5
Var gfx_md5

InstallDir "$PROGRAMFILES\Syndicate Wars\"

; Search for built files where `make install` puts them during automatic builds
!if /FileExists "..\pkg\swars\swars.exe"
  !define BUILDENV_PKG_DIR "..\pkg\swars"
!else if /FileExists "..\pkg\mingw64\swars\swars.exe"
  !define BUILDENV_PKG_DIR "..\pkg\mingw64\swars"
!else if /FileExists "..\pkg\mingw32\swars\swars.exe"
  !define BUILDENV_PKG_DIR "..\pkg\mingw32\swars"
!else
  !define BUILDENV_PKG_DIR ".\swars"
!endif

; Search for additional utils where they are extracted during automatic builds
!if /FileExists "..\util-nsis\oggenc.exe"
  !define BUILDENV_UTIL_DIR "..\util-nsis"
!else
  !define BUILDENV_UTIL_DIR ".\util-nsis"
!endif


!define MUI_ICON "swars.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${BUILDENV_UTIL_DIR}\win.bmp"
!define MUI_WELCOMEPAGE_TITLE "Welcome To The Syndicate Wars Port Setup"
!define MUI_WELCOMEPAGE_TEXT "This fan port requires the original Syndicate Wars game files. Installation is supported from the following versions of Syndicate Wars:$\r$\n$\r$\n * GOG Download version$\r$\n * Original European/USA DOS release CD$\r$\n * German DOS release CD$\r$\n * Korean DOS release CD$\r$\n * Japanese Windows release CD$\r$\n$\r$\nNote: While the Japanese version is supported, only English and French languages from this release are supported, Japanese text is not yet supported.$\r$\n$\r$\nBuild ${PRODUCT_VERSION}$\nLevels ${LEVELS_VERSION}"


; --------------------
; PAGES: 10

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE $(LIC_ENG)
Page custom SelectionPage SelectionPageLeave
Page custom GOGInstall GOGInstallLeave
Page custom GOGInstallAdditional GOGInstallAdditionalLeave
Page custom UpdateInstall UpdateInstallLeave
Page custom LangNCdromPage LangNCdromLeave
!define MUI_PAGE_CUSTOMFUNCTION_Pre SkipComponents
!insertmacro MUI_PAGE_COMPONENTS 
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

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


Section "Syndicate Wars Game" Section_0
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
  File "${BUILDENV_PKG_DIR}\swars.exe"
  File "${BUILDENV_PKG_DIR}\libgcc_s_dw2-1.dll"
  File "${BUILDENV_PKG_DIR}\libstdc++-6.dll"
  File "${BUILDENV_PKG_DIR}\libwinpthread-1.dll"
  File "${BUILDENV_PKG_DIR}\libopenal-1.dll"
  File "${BUILDENV_PKG_DIR}\libpng16-16.dll"
  File "${BUILDENV_PKG_DIR}\zlib1.dll"
  File "${BUILDENV_PKG_DIR}\libogg-0.dll"
  File "${BUILDENV_PKG_DIR}\libvorbis-0.dll"
  File "${BUILDENV_PKG_DIR}\libvorbisfile-3.dll"
  File "${BUILDENV_PKG_DIR}\libWildMidi.dll"
  IfFileExists "${BUILDENV_PKG_DIR}\SDL2.dll" 0 +1
  File /nonfatal "${BUILDENV_PKG_DIR}\SDL2.dll"
  IfFileExists "${BUILDENV_PKG_DIR}\SDL.dll" 0 +1
  File /nonfatal "${BUILDENV_PKG_DIR}\SDL.dll"
  SetOutPath $INSTDIR\conf
  ${If} $RadioSelected == "UpdateInstall"
	File /r /x "rules.ini" "${BUILDENV_PKG_DIR}\conf\"   ; If doing an udpdate install, do not copy over existing rules.ini
  ${Else} 
	File /r "${BUILDENV_PKG_DIR}\conf\"
   ${EndIf}
  SetOutPath $INSTDIR\language
  File /r "${BUILDENV_PKG_DIR}\language\"
  WriteUninstaller $INSTDIR\Uninstall.exe
  StrCmp $selected_menu_shortcut 1 0 inst_game_menu_end
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\Vexillium"
  CreateShortCut "$SMPROGRAMS\Vexillium\Syndicate Wars.lnk" "$INSTDIR\swars.exe" "" "$INSTDIR\swars.exe" "" SW_SHOWNORMAL
  CreateShortCut "$SMPROGRAMS\Vexillium\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" "" SW_SHOWNORMAL
inst_game_menu_end:
  StrCmp $selected_desk_shortcut 1 0 inst_game_done
  CreateShortCut "$DESKTOP\Syndicate Wars.lnk" $INSTDIR\swars.exe "" $INSTDIR\swars.exe "" SW_SHOWNORMAL
  Goto inst_game_done
inst_game_fail:
  DetailPrint "An unexpected error has occured during the installation process"
  RMDir /r $INSTDIR
  IntOp $selected_music 1 | ""
inst_game_done:
  nop 
SectionEnd


Section "Game music"  Section_1
  ; AddSize 1904
  StrCmp $selected_music 1 0 inst_music_go
  Goto inst_music_done
inst_music_go:
  SetOutPath $PLUGINSDIR
  File "${BUILDENV_UTIL_DIR}\rip.exe"
  File "${BUILDENV_UTIL_DIR}\akrip32.dll"
  File "${BUILDENV_UTIL_DIR}\oggenc.exe"
  File "${BUILDENV_UTIL_DIR}\libogg.dll"
  File "${BUILDENV_UTIL_DIR}\libvorbis.dll"
  File "${BUILDENV_UTIL_DIR}\libvorbisfile.dll"
  File "${BUILDENV_UTIL_DIR}\libFLAC_dynamic.dll"
  File "${BUILDENV_UTIL_DIR}\oggenc_LICENSE"
  CreateDirectory "$INSTDIR\music"
  ExecWait "$PLUGINSDIR\rip.exe $inst_src_root_dir $\"$INSTDIR\music$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_1.ogg$\" $\"$INSTDIR\music\track1.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_2.ogg$\" $\"$INSTDIR\music\track2.wav$\""
  ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_3.ogg$\" $\"$INSTDIR\music\track3.wav$\""
  Delete $INSTDIR\music\*.wav
  SetOutPath $INSTDIR
  IfErrors inst_music_fail
  Goto inst_music_done
inst_music_fail:
  DetailPrint "An unexpected error has occured during the music ripping process"
  RMDir /r $INSTDIR
inst_music_done:
SectionEnd

Function .onInit
  !insertmacro INSTALLOPTIONS_EXTRACT "lang_n_cdrom.ini"
  !insertmacro INSTALLOPTIONS_WRITE "lang_n_cdrom.ini" "Field 3" State "$(STR_CHOOSE_DRIVE)"
  StrCpy $RadioSelected "" ; Initialize RadioSelected variable
  SectionSetSize ${section_0} 220000
  SectionSetSize ${section_1} 36000
FunctionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_0} "Install the Syndicate Wars game files"
  !insertmacro MUI_DESCRIPTION_TEXT ${Section_1} "Extract the game's CD audio. Needed to have music in-game.$\r$\nIMPORTANT: do not use this option if installing from an iso image mounted in a virtual drive. You must rip the audio yourself in this case."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function SelectionPage
  !insertmacro MUI_HEADER_TEXT "Choose Game Files Source Type" "If installing from an original CD, insert the disc now"
  nsDialogs::Create 1018
  Pop $0

  ${NSD_CreateRadioButton} 0 0 100% 14u "Install game from original CD"
  Pop $1
  ${NSD_OnClick} $1 RadioClick
  ${NSD_CreateRadioButton} 0 18u 100% 14u "Install from GOG files"
  Pop $2
  ${NSD_OnClick} $2 RadioClick
  ${NSD_CreateRadioButton} 0 36u 100% 14u "Update an existing fan port install"
  Pop $3
  ${NSD_OnClick} $3 RadioClick
  nsDialogs::Show
  
FunctionEnd

Function RadioClick
 pop $4

  ${If} $4 == $1
    StrCpy $RadioSelected "CDInstall"
  ${ElseIf} $4 == $2
    StrCpy $RadioSelected "GOGInstall"
  ${ElseIf} $4 == $3
    StrCpy $RadioSelected "UpdateInstall"
  ${EndIf}
FunctionEnd

Function SelectionPageLeave
${If} $RadioSelected == ""
    MessageBox MB_ICONEXCLAMATION|MB_OK "Please select an install type before proceeding."
    Abort
${EndIf}
FunctionEnd

Function GOGInstall
!insertmacro MUI_HEADER_TEXT "GOG Syndicate Wars Installation Location" "Please indicate where GOG installed the root Syndicate Wars folder on your computer"
${If} $RadioSelected != "GOGInstall"
   Abort
 ${EndIf}

SetRegView 32
; Read the value from the registry key
ReadRegStr $gog_path HKLM "SOFTWARE\WOW6432Node\GOG.com\Games\1207659193\" "path"

    ;Create a custom page
    nsDialogs::Create 1018
    Pop $0

    ${NSD_CreateGroupBox} 5% 36u 90% 34u "Syndicate Wars GOG install folder"
    Pop $0

        ${NSD_CreateDirRequest} 15% 50u 49% 12u $gog_path
        Pop $1

        ${NSD_CreateBrowseButton} 65% 50u 20% 12u "Browse..."
        Pop $0
        ${NSD_OnClick} $0 BrowseButtonClicked

  nsDialogs::Show
FunctionEnd

Function BrowseButtonClicked
       ; Create a directory picker dialog to choose a directory
    
    nsDialogs::SelectFolderDialog /NOUNLOAD "Select Syndicate Wars GOG Directory" "$gog_path" 
    Pop $3
    ${If} $0 != error
        ${NSD_SetText} $1 "$3"
        StrCpy $gog_path "$3"
    ${EndIf}
FunctionEnd

Function GOGInstallLeave
;MessageBox MB_ICONEXCLAMATION|MB_OK "gog path is $gog_path"
${If} $gog_path == ""
        MessageBox MB_ICONEXCLAMATION|MB_OK "Please indicate the GOG Syndicate Wars install path before proceeding."
        Abort
${EndIf}
FunctionEnd

Function GOGInstallAdditional
  !insertmacro MUI_HEADER_TEXT "Additional Game Settings" "Please set your GOG version's language here"
  ${If} $RadioSelected != "GOGInstall"
      Abort
  ${EndIf}
  nsDialogs::Create /NOUNLOAD 1018

  ${NSD_CreateGroupBox} 21u 18u 249u 109u "Additional game settings"
  Pop $0
  
  ${NSD_CreateCheckbox} 37u 55u 134u 15u "Create a shortcut on the desktop"
  Pop $1

  ${NSD_CreateCheckbox} 37u 41u 160u 15u "Create an entry in the Start Menu"
  Pop $2

  ${NSD_SetState} $1 ${BST_CHECKED}
  ${NSD_SetState} $2 ${BST_CHECKED}

  ${NSD_CreateLabel} 37u 75u 118u 12u "Select GOG version language:"
  Pop $6

  ${NSD_CreateComboBox} 37u 90u 125u 21u ""
  Pop $5

  ${NSD_CB_AddString} $5 "English"
  ${NSD_CB_AddString} $5 "French"
  ${NSD_CB_AddString} $5 "German"
  ${NSD_CB_AddString} $5 "Italian"
  ${NSD_CB_AddString} $5 "Spanish"
  ${NSD_CB_AddString} $5 "Swedish"

  ;default to English
  StrCpy $selected_lang_text "English"
  ${NSD_CB_SelectString} $5 "English"

  nsDialogs::Show
FunctionEnd

Function GOGInstallAdditionalLeave
  
  ${NSD_GetState} $1 $3  ; Get state of Checkbox 1
  ${NSD_GetState} $2 $4  ; Get state of Checkbox 2

  ${If} $3 <> ${BST_CHECKED}
    StrCpy $selected_desk_shortcut "0"
  ${EndIf}

  ${If} $3 <> ${BST_UNCHECKED}
    StrCpy $selected_desk_shortcut "1"
  ${EndIf}

  ${If} $4 <> ${BST_CHECKED}
    StrCpy $selected_menu_shortcut "0"
  ${EndIf}
  
  ${If} $4 <> ${BST_UNCHECKED}
    StrCpy $selected_menu_shortcut "1"
  ${EndIf}

   ${NSD_GetText} $5 $selected_lang_text

  ;MessageBox MB_ICONEXCLAMATION|MB_OK "Desktop state is $selected_desk_shortcut"
  ;MessageBox MB_ICONEXCLAMATION|MB_OK "language is $selected_lang_text"

FunctionEnd

Function UpdateInstall
!insertmacro MUI_HEADER_TEXT "Syndicate Wars Port Installation Location" "Please indicate where you previously installed the root Syndicate Wars port folder on your computer"
${If} $RadioSelected != "UpdateInstall"
   Abort
 ${EndIf}

SetRegView 32
; Read the value from the registry key
ReadRegStr $update_path HKLM "Software\Syndicate Wars\CurrentVersion" "InstallPath" 

    ;Create a custom page
    nsDialogs::Create 1018
    Pop $0

    ${NSD_CreateGroupBox} 5% 20u 90% 60% "Syndicate Wars existing install folder"
    Pop $0

        ${NSD_CreateDirRequest} 15% 35u 49% 12u $update_path
        Pop $1

        ${NSD_CreateBrowseButton} 65% 35u 20% 12u "Browse..."
        Pop $0
        ${NSD_OnClick} $0 BrowseButtonClicked
  
		${NSD_CreateLabel} 20u 54u 118u 12u "Additional install options:"
    
		${NSD_CreateCheckbox} 37u 65u 134u 15u "Create a shortcut on the desktop"
		Pop $5

		${NSD_CreateCheckbox} 37u 80u 160u 15u "Create an entry in the Start Menu"
		Pop $6

    
  nsDialogs::Show
FunctionEnd

Function UpdateInstallLeave
;MessageBox MB_ICONEXCLAMATION|MB_OK "install path is $update_path"
${If} $update_path == ""
        MessageBox MB_ICONEXCLAMATION|MB_OK "You must indicate your existing Syndicate Wars install path before proceeding."
        Abort
${EndIf}

  ${NSD_GetState} $5 $3  ; Get state of Checkbox for desktop icon
  ${NSD_GetState} $6 $4  ; Get state of Checkbox for startmenu entry

  ${If} $3 <> ${BST_CHECKED}
    StrCpy $selected_desk_shortcut "0"
  ${EndIf}

  ${If} $3 <> ${BST_UNCHECKED}
    StrCpy $selected_desk_shortcut "1"
  ${EndIf}

  ${If} $4 <> ${BST_CHECKED}
    StrCpy $selected_menu_shortcut "0"
  ${EndIf}
  
  ${If} $4 <> ${BST_UNCHECKED}
    StrCpy $selected_menu_shortcut "1"
  ${EndIf}

StrCpy $selected_music 1 ; Never try and rip music if we're doing an update
StrCpy $INSTDIR $update_path
FunctionEnd

Function SkipComponents
  ${If} $RadioSelected != "CDInstall"
    Abort
  ${EndIf}
FunctionEnd

Function LangNCdromPage
  !insertmacro MUI_HEADER_TEXT "Additional Game Settings" "Please indicate which drive your Syndicate Wars CD is in, and which language you want to install from it"
  ${If} $RadioSelected != "CDInstall"
    Abort
  ${EndIf}
    nsDialogs::Create 1018
    Pop $0
    StrCpy $R2 0
    StrCpy $R0 "$(STR_CHOOSE_DRIVE)|"
    ${GetDrives} "CDROM" GetDrivesCallBack
    !insertmacro INSTALLOPTIONS_WRITE "lang_n_cdrom.ini" "Field 3" ListItems $R0
    !insertmacro INSTALLOPTIONS_DISPLAY "lang_n_cdrom.ini"
    
    Pop $1

    nsDialogs::Show
  
FunctionEnd

Function LangNCdromLeave
${If} $RadioSelected != "CDInstall"
    Abort
${EndIf}
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
langcd_lng_ger:
  StrCpy $selected_lang_abbr ger
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
langcd_lng_jap:
  StrCpy $selected_lang_abbr jap
  Goto langncd_store
langcd_lngswitch:
  StrCmp $selected_lang_text "English" langcd_lng_eng
  StrCmp $selected_lang_text "French" langcd_lng_fre
  StrCmp $selected_lang_text "German" langcd_lng_ger
  StrCmp $selected_lang_text "Italian" langcd_lng_ita
  StrCmp $selected_lang_text "Spanish" langcd_lng_spa
  StrCmp $selected_lang_text "Swedish" langcd_lng_swe
  StrCmp $selected_lang_text "Japanese" langcd_lng_jap
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
  ClearErrors
  ${If} $RadioSelected == "CDInstall"
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
  ${EndIf}
 
  ${If} $RadioSelected == "GOGInstall"
    ;DetailPrint "$PLUGINSDIR"
    CreateDirectory $INSTDIR\data
    CopyFiles /SILENT $gog_path\SWARS\data\* $INSTDIR\data
    CreateDirectory $INSTDIR\qdata
    CopyFiles /SILENT $gog_path\SWARS\qdata\* $INSTDIR\qdata
	Delete $INSTDIR\qdata\INTRO.SMK
    CreateDirectory $INSTDIR\intro
    CopyFiles /SILENT $gog_path\SWARS\qdata\INTRO.SMK $INSTDIR\intro
    CreateDirectory $INSTDIR\sound
    CopyFiles /SILENT $gog_path\SWARS\sound\*.dat $INSTDIR\sound
    CreateDirectory $INSTDIR\levels
    CopyFiles /SILENT $gog_path\SWARS\levels\* $INSTDIR\levels
    CreateDirectory $INSTDIR\maps
    CopyFiles /SILENT $gog_path\SWARS\maps\* $INSTDIR\maps
    IfErrors copy_files_fail
    StrCpy $selected_music "1"
    CreateDirectory $INSTDIR\music
    SetOutPath $PLUGINSDIR
    File "${BUILDENV_UTIL_DIR}\Gogisoripper.exe"
    File "${BUILDENV_UTIL_DIR}\oggenc.exe"
    File "${BUILDENV_UTIL_DIR}\libogg.dll"
    File "${BUILDENV_UTIL_DIR}\libvorbis.dll"
    File "${BUILDENV_UTIL_DIR}\libvorbisfile.dll"
    File "${BUILDENV_UTIL_DIR}\libFLAC_dynamic.dll"
    File "${BUILDENV_UTIL_DIR}\oggenc_LICENSE"
    DetailPrint "Ripping audio tracks from GOG iso"
    nsExec::ExecToLog "Gogisoripper.exe $\"$gog_path\SWARS\game.gog$\" $\"$INSTDIR\music\track_1.wav$\" 456262128 556118288"
    nsExec::ExecToLog "Gogisoripper.exe $\"$gog_path\SWARS\game.gog$\" $\"$INSTDIR\music\track_2.wav$\" 556118288 661238576"
    nsExec::ExecToLog "Gogisoripper.exe $\"$gog_path\SWARS\game.gog$\" $\"$INSTDIR\music\track_3.wav$\" 661238576 768718320"
    DetailPrint "Converting .wav files to .ogg"
    ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_1.ogg$\" $\"$INSTDIR\music\track_1.wav$\""
    ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_2.ogg$\" $\"$INSTDIR\music\track_2.wav$\""
    ExecWait "$PLUGINSDIR\oggenc.exe -b 192 --output=$\"$INSTDIR\music\track_3.ogg$\" $\"$INSTDIR\music\track_3.wav$\""
    DetailPrint "Cleaning up .wav files"
    Delete $INSTDIR\music\*.wav
    SetOutPath $INSTDIR
  ${EndIf}

  ;Update levels and maps from swars-levels repository

  StrCpy $levels_md5 "${LEVELS_PKG_MD5}"

  !if /FileExists "$PLUGINSDIR\${LEVELS_PACKAGE}.zip"
  !else
 	 Call DownloadLevels
  !endif

  ;Check MD5 of downloaded file to see if it's intact

  md5dll::GetMD5File "$PLUGINSDIR\${LEVELS_PACKAGE}.zip"
  Pop $0
  StrCmp $0 $levels_md5 extract_level_files retry_level_download

retry_level_download:
 DetailPrint "Error detected with level files zip, retrying download"
 Call DownloadLevels
 md5dll::GetMD5File "$PLUGINSDIR\${LEVELS_PACKAGE}.zip"
 Pop $0
 StrCmp $0 $levels_md5 extract_level_files 0
 SetErrors
 DetailPrint "Error downloading level files from swars-levels repository, please check the source file."
 DetailPrint "Aborting install"
 Abort

extract_level_files:
 DetailPrint "Extracting updated game levels..."
 nsisunz::Unzip  "$PLUGINSDIR\${LEVELS_PACKAGE}.zip" "$PLUGINSDIR\"
 CopyFiles /SILENT $PLUGINSDIR\SWARS\levels\* $INSTDIR\levels
 CopyFiles /SILENT $PLUGINSDIR\SWARS\maps\* $INSTDIR\maps

  ;Update graphics files from swars-gfx repository

  StrCpy $gfx_md5 "${GFX_PKG_MD5}"

  !if /FileExists "$PLUGINSDIR\${GFX_PACKAGE}.zip"
  !else
 	 Call DownloadGfx
  !endif

  ;Check MD5 of downloaded file to see if it's intact

  md5dll::GetMD5File "$PLUGINSDIR\${GFX_PACKAGE}.zip"
  Pop $0
  StrCmp $0 $gfx_md5 extract_gfx_files retry_gfx_download

retry_gfx_download:
 DetailPrint "Error detected with graphics files zip, retrying download"
 Call DownloadGFX
 md5dll::GetMD5File "$PLUGINSDIR\${GFX_PACKAGE}.zip"
 Pop $0
 StrCmp $0 $gfx_md5 extract_gfx_files 0
 SetErrors
 DetailPrint "Error downloading graphics files from swars-gfx repository, please check the source file."
 DetailPrint "Aborting install"
 Abort

extract_gfx_files:
 DetailPrint "Extracting updated graphics files..."
 nsisunz::Unzip  "$PLUGINSDIR\${GFX_PACKAGE}.zip" "$PLUGINSDIR\"
 CopyFiles /SILENT $PLUGINSDIR\SWARS\data\* $INSTDIR\data

 Return

copy_files_fail:
  SetErrors
  DetailPrint "Error copying files, check this is the correct source disc/path for the game"
  DetailPrint "Aborting Install"
  Abort

FunctionEnd


Function DownloadLevels
  DetailPrint "Downloading latest game levels from Github..."
  inetc::get "https://github.com/swfans/swars-levels/releases/download/${LEVELS_VERSION}/${LEVELS_PACKAGE}.zip" "$PLUGINSDIR\${LEVELS_PACKAGE}.zip"
FunctionEnd


Function DownloadGfx
  DetailPrint "Downloading latest game graphics from Github..."
   inetc::get "https://github.com/swfans/swars-gfx/releases/download/${GFX_VERSION}/${GFX_PACKAGE}.zip" "$PLUGINSDIR\${GFX_PACKAGE}.zip"
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

Delete '$INSTDIR\config.ini'
Delete '$INSTDIR\libgcc_s_dw2-1.dll'
Delete '$INSTDIR\libogg-0.dll'
Delete '$INSTDIR\libopenal-1.dll'
Delete '$INSTDIR\libpng16-16.dll'
Delete '$INSTDIR\libstdc++-6.dll'
Delete '$INSTDIR\libvorbis-0.dll'
Delete '$INSTDIR\libvorbisfile-3.dll'
Delete '$INSTDIR\libWildMidi.dll'
Delete '$INSTDIR\libwinpthread-1.dll'
Delete '$INSTDIR\SDL.dll'
Delete '$INSTDIR\SDL2.dll'
Delete '$INSTDIR\swars.exe'
Delete '$INSTDIR\Uninstall.exe'
Delete '$INSTDIR\zlib1.dll'

Delete '$INSTDIR\conf\cities.ini'
Delete '$INSTDIR\conf\cybmods.ini'
Delete '$INSTDIR\conf\midipats.cfg'
Delete '$INSTDIR\conf\miss000.ini'
Delete '$INSTDIR\conf\miss001.ini'
Delete '$INSTDIR\conf\miss002.ini'
Delete '$INSTDIR\conf\miss003.ini'
Delete '$INSTDIR\conf\miss099.ini'
Delete '$INSTDIR\conf\people.ini'
Delete '$INSTDIR\conf\rules.ini'
Delete '$INSTDIR\conf\weapons.ini'


Delete '$INSTDIR\conf\Tone_000\000_Acoustic_Grand_Piano.pat'
Delete '$INSTDIR\conf\Tone_000\004_Electric_Piano_1_Rhodes.pat'

Delete '$INSTDIR\data\BG.DAT'
Delete '$INSTDIR\data\BGTABLES.DAT'
Delete '$INSTDIR\data\CHU_ELE.SMK'
Delete '$INSTDIR\data\CITIES.DAT'
Delete '$INSTDIR\data\FONT0-0.DAT'
Delete '$INSTDIR\data\FONT0-0.TAB'
Delete '$INSTDIR\data\FONT0-1.DAT'
Delete '$INSTDIR\data\FONT0-1.TAB'
Delete '$INSTDIR\data\FONT0-2.DAT'
Delete '$INSTDIR\data\FONT0-2.TAB'
Delete '$INSTDIR\data\FONT0-3.DAT'
Delete '$INSTDIR\data\FONT0-3.TAB'
Delete '$INSTDIR\data\FONT0-4.DAT'
Delete '$INSTDIR\data\FONT0-4.TAB'
Delete '$INSTDIR\data\FONT0-5.DAT'
Delete '$INSTDIR\data\FONT0-5.TAB'
Delete '$INSTDIR\data\ICONS0-0.DAT'
Delete '$INSTDIR\data\ICONS0-0.TAB'
Delete '$INSTDIR\data\MAPINSID.DAT'
Delete '$INSTDIR\data\MAPOUT00.DAT'
Delete '$INSTDIR\data\MAPOUT01.DAT'
Delete '$INSTDIR\data\MAPOUT02.DAT'
Delete '$INSTDIR\data\MAPOUT03.DAT'
Delete '$INSTDIR\data\MAPOUT04.DAT'
Delete '$INSTDIR\data\MAPOUT05.DAT'
Delete '$INSTDIR\data\MECH.DAT'
Delete '$INSTDIR\data\MELE-0.ANI'
Delete '$INSTDIR\data\MFRA-0.ANI'
Delete '$INSTDIR\data\MLPR-0.DAT'
Delete '$INSTDIR\data\MLPR-0.TAB'
Delete '$INSTDIR\data\MOUSE-0.DAT'
Delete '$INSTDIR\data\MOUSE-0.TAB'
Delete '$INSTDIR\data\MSPR-0.DAT'
Delete '$INSTDIR\data\MSPR-0.INF'
Delete '$INSTDIR\data\MSPR-0.PAL'
Delete '$INSTDIR\data\MSPR-0.TAB'
Delete '$INSTDIR\data\MSPR-1.DAT'
Delete '$INSTDIR\data\MSPR-1.INF'
Delete '$INSTDIR\data\MSPR-1.TAB'
Delete '$INSTDIR\data\MSPR-2.DAT'
Delete '$INSTDIR\data\MSPR-2.INF'
Delete '$INSTDIR\data\MSPR-2.TAB'
Delete '$INSTDIR\data\MSPR-3.DAT'
Delete '$INSTDIR\data\MSPR-3.INF'
Delete '$INSTDIR\data\MSPR-3.TAB'
Delete '$INSTDIR\data\MSPR-4.DAT'
Delete '$INSTDIR\data\MSPR-4.INF'
Delete '$INSTDIR\data\MSPR-4.TAB'
Delete '$INSTDIR\data\MSPR-5.DAT'
Delete '$INSTDIR\data\MSPR-5.INF'
Delete '$INSTDIR\data\MSPR-5.TAB'
Delete '$INSTDIR\data\MSPR0-0.DAT'
Delete '$INSTDIR\data\MSPR0-0.TAB'
Delete '$INSTDIR\data\MSPRPSX0.TAB'
Delete '$INSTDIR\data\MSTA-0.ANI'
Delete '$INSTDIR\data\MUSIC.DAT'
Delete '$INSTDIR\data\NAM-FONT.DAT'
Delete '$INSTDIR\data\NAM-FONT.TAB'
Delete '$INSTDIR\data\NFRA-0.ANI'
Delete '$INSTDIR\data\NSTA-0.ANI'
Delete '$INSTDIR\data\OUTRO-S.RAW'
Delete '$INSTDIR\data\OUTRO-S.SMK'
Delete '$INSTDIR\data\OUTRO-Z.RAW'
Delete '$INSTDIR\data\OUTRO-Z.SMK'
Delete '$INSTDIR\data\PAL.DAT'
Delete '$INSTDIR\data\PAL.PAL'
Delete '$INSTDIR\data\PAL0.DAT'
Delete '$INSTDIR\data\PAL1.DAT'
Delete '$INSTDIR\data\PAL2.DAT'
Delete '$INSTDIR\data\PAL3.DAT'
Delete '$INSTDIR\data\PANEL0-0.DAT'
Delete '$INSTDIR\data\PANEL0-0.TAB'
Delete '$INSTDIR\data\PANEL0-1.DAT'
Delete '$INSTDIR\data\PANEL0-1.TAB'
Delete '$INSTDIR\data\PANEL0-2.DAT'
Delete '$INSTDIR\data\PANEL0-2.TAB'
Delete '$INSTDIR\data\PANEL1-0.DAT'
Delete '$INSTDIR\data\PANEL1-0.TAB'
Delete '$INSTDIR\data\PANEL2-0.DAT'
Delete '$INSTDIR\data\PANEL2-0.TAB'
Delete '$INSTDIR\data\PEOPLE.BAK'
Delete '$INSTDIR\data\PEOPLE.TXT'
Delete '$INSTDIR\data\POINTERS.DAT'
Delete '$INSTDIR\data\POINTERS.TAB'
Delete '$INSTDIR\data\POP0-0.DAT'
Delete '$INSTDIR\data\POP0-0.TAB'
Delete '$INSTDIR\data\POP0-1.DAT'
Delete '$INSTDIR\data\POP0-1.TAB'
Delete '$INSTDIR\data\POP1-0.DAT'
Delete '$INSTDIR\data\POP1-0.TAB'
Delete '$INSTDIR\data\POP1-1.DAT'
Delete '$INSTDIR\data\POP1-1.TAB'
Delete '$INSTDIR\data\POP2-0.DAT'
Delete '$INSTDIR\data\POP2-0.TAB'
Delete '$INSTDIR\data\POP2-1.DAT'
Delete '$INSTDIR\data\POP2-1.TAB'
Delete '$INSTDIR\data\POSDEFS.IDX'
Delete '$INSTDIR\data\POSDEFS.WAD'
Delete '$INSTDIR\data\QOT-FONT.DAT'
Delete '$INSTDIR\data\QOT-FONT.TAB'
Delete '$INSTDIR\data\S-PROJ.PAL'
Delete '$INSTDIR\data\STARTGHO.DAT'
Delete '$INSTDIR\data\SYNGHOST.TAB'
Delete '$INSTDIR\data\SYN_ELE.SMK'
Delete '$INSTDIR\data\TABLES.DAT'
Delete '$INSTDIR\data\TABLES1.DAT'
Delete '$INSTDIR\data\TEX00.DAT'
Delete '$INSTDIR\data\TEX01.DAT'
Delete '$INSTDIR\data\TEX02.DAT'
Delete '$INSTDIR\data\TEX03.DAT'
Delete '$INSTDIR\data\TEX04.DAT'
Delete '$INSTDIR\data\TEX05.DAT'
Delete '$INSTDIR\data\TEXT.DAT'
Delete '$INSTDIR\data\TEXTWALK.DAT'
Delete '$INSTDIR\data\TIT-FONT.DAT'
Delete '$INSTDIR\data\TIT-FONT.TAB'
Delete '$INSTDIR\data\TITLE.DAT'
Delete '$INSTDIR\data\TITLE.PAL'
Delete '$INSTDIR\data\TITLEHI.PAL'
Delete '$INSTDIR\data\TMAP.ANI'
Delete '$INSTDIR\data\W-ICONS.DAT'
Delete '$INSTDIR\data\W-ICONS.TAB'

Delete '$INSTDIR\data\equip\MOD-00.FLI'
Delete '$INSTDIR\data\equip\MOD-00Z.FLI'
Delete '$INSTDIR\data\equip\MOD-01.FLI'
Delete '$INSTDIR\data\equip\MOD-01Z.FLI'
Delete '$INSTDIR\data\equip\MOD-02.FLI'
Delete '$INSTDIR\data\equip\MOD-02Z.FLI'
Delete '$INSTDIR\data\equip\MOD-03.FLI'
Delete '$INSTDIR\data\equip\MOD-03Z.FLI'
Delete '$INSTDIR\data\equip\MOD-04.FLI'
Delete '$INSTDIR\data\equip\MOD-04Z.FLI'
Delete '$INSTDIR\data\equip\MOD-05.FLI'
Delete '$INSTDIR\data\equip\MOD-05Z.FLI'
Delete '$INSTDIR\data\equip\MOD-06.FLI'
Delete '$INSTDIR\data\equip\MOD-06Z.FLI'
Delete '$INSTDIR\data\equip\MOD-07.FLI'
Delete '$INSTDIR\data\equip\MOD-07Z.FLI'
Delete '$INSTDIR\data\equip\MOD-08.FLI'
Delete '$INSTDIR\data\equip\MOD-08Z.FLI'
Delete '$INSTDIR\data\equip\MOD-09.FLI'
Delete '$INSTDIR\data\equip\MOD-09Z.FLI'
Delete '$INSTDIR\data\equip\MOD-10.FLI'
Delete '$INSTDIR\data\equip\MOD-10Z.FLI'
Delete '$INSTDIR\data\equip\MOD-11.FLI'
Delete '$INSTDIR\data\equip\MOD-11Z.FLI'
Delete '$INSTDIR\data\equip\NET01.FLI'
Delete '$INSTDIR\data\equip\NET02.FLI'
Delete '$INSTDIR\data\equip\NET03.FLI'
Delete '$INSTDIR\data\equip\NET04.FLI'
Delete '$INSTDIR\data\equip\NET05.FLI'
Delete '$INSTDIR\data\equip\NET06.FLI'
Delete '$INSTDIR\data\equip\NET07.FLI'
Delete '$INSTDIR\data\equip\NET08.FLI'
Delete '$INSTDIR\data\equip\NET09.FLI'
Delete '$INSTDIR\data\equip\NET10.FLI'
Delete '$INSTDIR\data\equip\NET11.FLI'
Delete '$INSTDIR\data\equip\NET12.FLI'
Delete '$INSTDIR\data\equip\NET13.FLI'
Delete '$INSTDIR\data\equip\NET14.FLI'
Delete '$INSTDIR\data\equip\NET15.FLI'
Delete '$INSTDIR\data\equip\NET16.FLI'
Delete '$INSTDIR\data\equip\NET17.FLI'
Delete '$INSTDIR\data\equip\NET18.FLI'
Delete '$INSTDIR\data\equip\WEP-00.FLI'
Delete '$INSTDIR\data\equip\WEP-00Z.FLI'
Delete '$INSTDIR\data\equip\WEP-01.FLI'
Delete '$INSTDIR\data\equip\WEP-01Z.FLI'
Delete '$INSTDIR\data\equip\WEP-02.FLI'
Delete '$INSTDIR\data\equip\WEP-02Z.FLI'
Delete '$INSTDIR\data\equip\WEP-03.FLI'
Delete '$INSTDIR\data\equip\WEP-03Z.FLI'
Delete '$INSTDIR\data\equip\WEP-04.FLI'
Delete '$INSTDIR\data\equip\WEP-04Z.FLI'
Delete '$INSTDIR\data\equip\WEP-05.FLI'
Delete '$INSTDIR\data\equip\WEP-05Z.FLI'
Delete '$INSTDIR\data\equip\WEP-06.FLI'
Delete '$INSTDIR\data\equip\WEP-06Z.FLI'
Delete '$INSTDIR\data\equip\WEP-07.FLI'
Delete '$INSTDIR\data\equip\WEP-07Z.FLI'
Delete '$INSTDIR\data\equip\WEP-08.FLI'
Delete '$INSTDIR\data\equip\WEP-08Z.FLI'
Delete '$INSTDIR\data\equip\WEP-09.FLI'
Delete '$INSTDIR\data\equip\WEP-09Z.FLI'
Delete '$INSTDIR\data\equip\WEP-10.FLI'
Delete '$INSTDIR\data\equip\WEP-10Z.FLI'
Delete '$INSTDIR\data\equip\WEP-11.FLI'
Delete '$INSTDIR\data\equip\WEP-11Z.FLI'
Delete '$INSTDIR\data\equip\WEP-12.FLI'
Delete '$INSTDIR\data\equip\WEP-12Z.FLI'
Delete '$INSTDIR\data\equip\WEP-13.FLI'
Delete '$INSTDIR\data\equip\WEP-13Z.FLI'
Delete '$INSTDIR\data\equip\WEP-14.FLI'
Delete '$INSTDIR\data\equip\WEP-14Z.FLI'
Delete '$INSTDIR\data\equip\WEP-15.FLI'
Delete '$INSTDIR\data\equip\WEP-15Z.FLI'
Delete '$INSTDIR\data\equip\WEP-16.FLI'
Delete '$INSTDIR\data\equip\WEP-16Z.FLI'
Delete '$INSTDIR\data\equip\WEP-17.FLI'
Delete '$INSTDIR\data\equip\WEP-17Z.FLI'
Delete '$INSTDIR\data\equip\WEP-18.FLI'
Delete '$INSTDIR\data\equip\WEP-18Z.FLI'
Delete '$INSTDIR\data\equip\WEP-19.FLI'
Delete '$INSTDIR\data\equip\WEP-19Z.FLI'
Delete '$INSTDIR\data\equip\WEP-20.FLI'
Delete '$INSTDIR\data\equip\WEP-20Z.FLI'
Delete '$INSTDIR\data\equip\WEP-21.FLI'
Delete '$INSTDIR\data\equip\WEP-21Z.FLI'
Delete '$INSTDIR\data\equip\WEP-22.FLI'
Delete '$INSTDIR\data\equip\WEP-22Z.FLI'
Delete '$INSTDIR\data\equip\WEP-23.FLI'
Delete '$INSTDIR\data\equip\WEP-23Z.FLI'
Delete '$INSTDIR\data\equip\WEP-24.FLI'
Delete '$INSTDIR\data\equip\WEP-24Z.FLI'
Delete '$INSTDIR\data\equip\WEP-25.FLI'
Delete '$INSTDIR\data\equip\WEP-25Z.FLI'
Delete '$INSTDIR\data\equip\WEP-26.FLI'
Delete '$INSTDIR\data\equip\WEP-26Z.FLI'
Delete '$INSTDIR\data\equip\WEP-27.FLI'
Delete '$INSTDIR\data\equip\WEP-27Z.FLI'
Delete '$INSTDIR\data\equip\WEP-28.FLI'
Delete '$INSTDIR\data\equip\WEP-28Z.FLI'
Delete '$INSTDIR\data\equip\WEP-29.FLI'
Delete '$INSTDIR\data\equip\WEP-29Z.FLI'
Delete '$INSTDIR\data\equip\WEP-30.FLI'
Delete '$INSTDIR\data\equip\WEP-30Z.FLI'

Delete $INSTDIR\levels\C*.DAT
Delete $INSTDIR\levels\C*.D1
Delete $INSTDIR\levels\C*.D2
Delete $INSTDIR\levels\C*.D3

Delete '$INSTDIR\maps\MAP000.SCN'
Delete '$INSTDIR\maps\MAP001.MAD'
Delete '$INSTDIR\maps\MAP001.SCN'
Delete '$INSTDIR\maps\MAP002.MAD'
Delete '$INSTDIR\maps\MAP002.SCN'
Delete '$INSTDIR\maps\MAP003.MAD'
Delete '$INSTDIR\maps\MAP003.SCN'
Delete '$INSTDIR\maps\MAP004.MAD'
Delete '$INSTDIR\maps\MAP004.SCN'
Delete '$INSTDIR\maps\MAP005.MAD'
Delete '$INSTDIR\maps\MAP005.SCN'
Delete '$INSTDIR\maps\MAP006.MAD'
Delete '$INSTDIR\maps\MAP006.SCN'
Delete '$INSTDIR\maps\MAP007.MAD'
Delete '$INSTDIR\maps\MAP007.SCN'
Delete '$INSTDIR\maps\MAP008.MAD'
Delete '$INSTDIR\maps\MAP008.SCN'
Delete '$INSTDIR\maps\MAP009.MAD'
Delete '$INSTDIR\maps\MAP009.SCN'
Delete '$INSTDIR\maps\MAP010.MAD'
Delete '$INSTDIR\maps\MAP010.SCN'
Delete '$INSTDIR\maps\MAP011.MAD'
Delete '$INSTDIR\maps\MAP011.SCN'
Delete '$INSTDIR\maps\MAP012.MAD'
Delete '$INSTDIR\maps\MAP020.MAD'
Delete '$INSTDIR\maps\MAP020.SCN'
Delete '$INSTDIR\maps\MAP021.MAD'
Delete '$INSTDIR\maps\MAP021.SCN'
Delete '$INSTDIR\maps\MAP022.MAD'
Delete '$INSTDIR\maps\MAP022.SCN'
Delete '$INSTDIR\maps\MAP025.MAD'
Delete '$INSTDIR\maps\MAP025.SCN'
Delete '$INSTDIR\maps\MAP026.MAD'
Delete '$INSTDIR\maps\MAP026.SCN'
Delete '$INSTDIR\maps\MAP027.MAD'
Delete '$INSTDIR\maps\MAP027.SCN'
Delete '$INSTDIR\maps\MAP028.MAD'
Delete '$INSTDIR\maps\MAP028.SCN'
Delete '$INSTDIR\maps\MAP030.B&B'
Delete '$INSTDIR\maps\MAP030.MAD'
Delete '$INSTDIR\maps\MAP030.SCN'
Delete '$INSTDIR\maps\MAP032.MAD'
Delete '$INSTDIR\maps\MAP032.SCN'
Delete '$INSTDIR\maps\MAP035.MAD'
Delete '$INSTDIR\maps\MAP035.SCN'
Delete '$INSTDIR\maps\MAP036.MAD'
Delete '$INSTDIR\maps\MAP036.SCN'
Delete '$INSTDIR\maps\MAP040.MAD'
Delete '$INSTDIR\maps\MAP040.SCN'
Delete '$INSTDIR\maps\MAP041.MAD'
Delete '$INSTDIR\maps\MAP041.SCN'
Delete '$INSTDIR\maps\MAP044.MAD'
Delete '$INSTDIR\maps\MAP044.SCN'
Delete '$INSTDIR\maps\MAP045.MAD'
Delete '$INSTDIR\maps\MAP045.SCN'
Delete '$INSTDIR\maps\MAP046.MAD'
Delete '$INSTDIR\maps\MAP046.SCN'
Delete '$INSTDIR\maps\MAP047.MAD'
Delete '$INSTDIR\maps\MAP047.SCN'
Delete '$INSTDIR\maps\MAP050.MAD'
Delete '$INSTDIR\maps\MAP050.SCN'
Delete '$INSTDIR\maps\MAP051.MAD'
Delete '$INSTDIR\maps\MAP052.MAD'
Delete '$INSTDIR\maps\MAP060.MAD'
Delete '$INSTDIR\maps\MAP065.MAD'
Delete '$INSTDIR\maps\MAP070.MAD'
Delete '$INSTDIR\maps\MAP074.MAD'
Delete '$INSTDIR\maps\MAP079.MAD'
Delete '$INSTDIR\maps\MAP079.SCN'

Delete '$INSTDIR\qdata\ALLTEXT.IDX'
Delete '$INSTDIR\qdata\ALLTEXT.WAD'
Delete '$INSTDIR\qdata\DEMO-10.FLI'
Delete '$INSTDIR\qdata\DEMO-11.FLI'
Delete '$INSTDIR\qdata\DEMO-12.FLI'
Delete '$INSTDIR\qdata\DEMO-13.FLI'
Delete '$INSTDIR\qdata\FLYBY.FLI'
Delete '$INSTDIR\qdata\INTRO.SMK'
Delete '$INSTDIR\qdata\OUTRO-S.SMK'
Delete '$INSTDIR\qdata\PAL.DAT'
Delete '$INSTDIR\qdata\PAL.PAL'
Delete '$INSTDIR\qdata\PAL0.DAT'
Delete '$INSTDIR\qdata\PAL1.DAT'
Delete '$INSTDIR\qdata\PAL2.DAT'
Delete '$INSTDIR\qdata\PAL3.DAT'
Delete '$INSTDIR\qdata\POSDEFS.IDX'
Delete '$INSTDIR\qdata\POSDEFS.WAD'
Delete '$INSTDIR\qdata\PRIMVEH.OBJ'
Delete '$INSTDIR\qdata\S-PROJ.DAT'
Delete '$INSTDIR\qdata\SYN_FAIL.SMK'
Delete '$INSTDIR\qdata\SYN_GO.SMK'
Delete '$INSTDIR\qdata\SYN_MC.SMK'
Delete '$INSTDIR\qdata\Z-PROJ.DAT'
Delete '$INSTDIR\qdata\ZEL-GO.SMK'
Delete '$INSTDIR\qdata\ZEL-MC.SMK'

Delete '$INSTDIR\qdata\EQUIP\BGMAN.DAT'
Delete '$INSTDIR\qdata\EQUIP\BGMANZ.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1B.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M1L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M1L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A3.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2A3.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2A3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2B.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L3.DAT'
Delete '$INSTDIR\qdata\EQUIP\M2L3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M2L3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A3.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3A3.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3A3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3B.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L3.DAT'
Delete '$INSTDIR\qdata\EQUIP\M3L3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\M3L3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1B.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z1L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z1L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A3.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2A3.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2A3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2B.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L3.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z2L3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z2L3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3A1.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3A2.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A3.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3A3.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3A3O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3B.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3BB.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3BBI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3BBO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3BI.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3BO.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L1.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3L1I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L1O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L2.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3L2I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L2O.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L3.DAT'
Delete '$INSTDIR\qdata\EQUIP\Z3L3I.FLI'
Delete '$INSTDIR\qdata\EQUIP\Z3L3O.FLI'

Delete '$INSTDIR\sound\MUSIC.DAT'
Delete '$INSTDIR\sound\SOUND.DAT'
Delete '$INSTDIR\sound\SYNCREDS.DAT'

Delete '$INSTDIR\language\eng\alltext-p.idx'
Delete '$INSTDIR\language\eng\alltext-p.wad'
Delete '$INSTDIR\language\eng\alltext-s.idx'
Delete '$INSTDIR\language\eng\alltext-s.wad'
Delete '$INSTDIR\language\eng\alltext-z.idx'
Delete '$INSTDIR\language\eng\alltext-z.wad'
Delete '$INSTDIR\language\fre\alltext-p.idx'
Delete '$INSTDIR\language\fre\alltext-p.wad'
Delete '$INSTDIR\language\fre\alltext-s.idx'
Delete '$INSTDIR\language\fre\alltext-s.wad'
Delete '$INSTDIR\language\fre\alltext-z.idx'
Delete '$INSTDIR\language\fre\alltext-z.wad'
Delete '$INSTDIR\language\ger\alltext-p.idx'
Delete '$INSTDIR\language\ger\alltext-p.wad'
Delete '$INSTDIR\language\ger\alltext-s.idx'
Delete '$INSTDIR\language\ger\alltext-s.wad'
Delete '$INSTDIR\language\ger\alltext-z.idx'
Delete '$INSTDIR\language\ger\alltext-z.wad'
Delete '$INSTDIR\language\ita\alltext-p.idx'
Delete '$INSTDIR\language\ita\alltext-p.wad'
Delete '$INSTDIR\language\ita\alltext-s.idx'
Delete '$INSTDIR\language\ita\alltext-s.wad'
Delete '$INSTDIR\language\ita\alltext-z.idx'
Delete '$INSTDIR\language\ita\alltext-z.wad'
Delete '$INSTDIR\language\spa\alltext-p.idx'
Delete '$INSTDIR\language\spa\alltext-p.wad'
Delete '$INSTDIR\language\spa\alltext-s.idx'
Delete '$INSTDIR\language\spa\alltext-s.wad'
Delete '$INSTDIR\language\spa\alltext-z.idx'
Delete '$INSTDIR\language\spa\alltext-z.wad'
Delete '$INSTDIR\language\swe\alltext-p.idx'
Delete '$INSTDIR\language\swe\alltext-p.wad'
Delete '$INSTDIR\language\swe\alltext-s.idx'
Delete '$INSTDIR\language\swe\alltext-s.wad'
Delete '$INSTDIR\language\swe\alltext-z.idx'
Delete '$INSTDIR\language\swe\alltext-z.wad'

Delete '$INSTDIR\intro\INTRO.SMK'

Delete '$INSTDIR\music\track_1.ogg'
Delete '$INSTDIR\music\track_2.ogg'
Delete '$INSTDIR\music\track_3.ogg'

RMDir "$INSTDIR\conf\Tone_000"
RMDir "$INSTDIR\conf"
RMDir "$INSTDIR\data\EQUIP"
RMDir "$INSTDIR\data"
RMDir "$INSTDIR\intro"
RMDir "$INSTDIR\levels"
RMDir "$INSTDIR\maps"
RMDir "$INSTDIR\music"
RMDir "$INSTDIR\qdata\EQUIP"
RMDir "$INSTDIR\qdata\SAVEGAME"
RMDir "$INSTDIR\qdata"
RMDir "$INSTDIR\sound"
RMDir "$INSTDIR\language\eng"
RMDir "$INSTDIR\language\fre"
RMDir "$INSTDIR\language\ger"
RMDir "$INSTDIR\language\spa"
RMDir "$INSTDIR\language\ita"
RMDir "$INSTDIR\language\swe"
RMDir "$INSTDIR\language"

RMDir "$INSTDIR"
Delete "$SMPROGRAMS\Vexillium\Syndicate Wars.lnk"
Delete "$DESKTOP\Syndicate Wars.lnk" 
Delete "$SMPROGRAMS\Vexillium\Uninstall.lnk"
RMDir "$SMPROGRAMS\Vexillium"

DetailPrint "Removing Registry Key"
DeleteRegKey HKLM "Software\Syndicate Wars"

SetAutoClose false

SectionEnd