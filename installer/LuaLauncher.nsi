!include "MUI2.nsh"

Name "LauLauncher"
OutFile "LauLauncher x64 Installer.EXE"
InstallDir "C:\Program Files\LuaLauncher"

# Use default images from NSIS
!define MUI_HEADERIMAGE

# Header
!define MUI_PAGE_HEADER_TEXT "Cesar Berriot Presents:"
!define MUI_PAGE_HEADER_SUBTEXT "LuaLauncher (c) 2024"

# Welcome Page
!define MUI_WELCOMEPAGE_TITLE "LuaLauncher"
!define MUI_WELCOMEPAGE_TEXT "A Lua script manager, launcher and creator made in C++ with Qt."
!insertmacro MUI_PAGE_WELCOME

# License Page
!define MUI_LICENSEPAGE_TEXT_TOP "This software is distributed under the MIT License :"
!define MUI_LICENSEPAGE_TEXT_BOTTOM "Please read and agree with this license to continue the installation."
!define MUI_LICENSEPAGE_CHECKBOX
!define MUI_LICENSEPAGE_CHECKBOX_TEXT "I agree."
!insertmacro MUI_PAGE_LICENSE "LICENSE"

# Installation Page
!insertmacro MUI_PAGE_INSTFILES


# Finish Page
!define MUI_FINISHPAGE_TITLE "The installation has been completed."
!define MUI_FINISHPAGE_TEXT "You may now use LuaLauncher"
!define MUI_FINISHPAGE_RUN "$INSTDIR\LuaLauncher.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Run LuaLauncher."
!define MUI_FINISHPAGE_LINK "LuaLauncher On Github"
!define MUI_FINISHPAGE_LINK_LOCATION "https://github.com/CesarBerriot/LuaLauncher"
!insertmacro MUI_PAGE_FINISH

# Language
!insertmacro MUI_LANGUAGE "English"

Section
    # Require admin rights
    UserInfo::GetAccountType
    Pop $0
    StrCmp $0 "Admin" +3
    MessageBox MB_OK "This installer requires administrator permissions. Please restart the application as an administrator."
    Quit
	# Copy binaries
	SetOutPath $INSTDIR
	File /r "bin\"
	# Create the scripts directory
	# CreateDirectory "$\"$INSTDIR\scripts$\""
    # Write the uninstaller
    SetOutPath $INSTDIR
    WriteUninstaller "LuaLauncher Uninstaller.exe"
    # Write start menu items
	CreateDirectory "$SMPROGRAMS\LuaLauncher"
    CreateShortcut "$SMPROGRAMS\LuaLauncher\LuaLauncher.lnk" "$INSTDIR\LuaLauncher.exe"
	CreateShortcut "$SMPROGRAMS\LuaLauncher\Uninstall LuaLauncher.lnk" "$INSTDIR\LuaLauncher Uninstaller.exe"
    # Write a control panel program entry in the registry
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher" \
        "DisplayName" "LuaLauncher"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher" \
        "UninstallString" "$\"$INSTDIR\LuaLauncher Uninstaller.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher" \
        "InstallLocation" "$\"$INSTDIR$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher" \
        "Publisher" "Cesar Berriot"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher" \
        "URLInfoAbout" "https://github.com/CesarBerriot/LuaLauncher"
SectionEnd

Section "Uninstall"
    # Delete files
    RMDir /r "$INSTDIR"
    # Delete start menu items
	RMDir /r "$SMPROGRAMS\LuaLauncher"
    # Delete registry keys
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\LuaLauncher"
SectionEnd