; Script generated by the HM NIS Edit Script Wizard.

!include "NsisStudy.nsh"
!include "WinMessages.nsh"
!include "LogicLib.nsh"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "NsisStudy"
!define PRODUCT_VERSION "1.0.0.0"
!define PRODUCT_PUBLISHER "fishjam"
!define PRODUCT_WEB_SITE "http://www.fishjam.com"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

;Control variable and macro
!define MUI_HEADERIMAGE          ;���� Header �����Ƿ���ʾ����ͼƬ
;!define MUI_HEADERIMAGE_RIGHT    ;����ͼƬ��ʾ���Ҳ�, TODO:ȱʡ��Ч�����ã���û�ͼ

SetCompressor lzma

!include "FUtils.nsh"

;Control Functions
Function LicensePageLeave
  ;License ҳ����ʾʱ����г�ʼ��(TODO: �ƺ��Ҳ��� 1034 �ؼ� -- Accept �� RadioButton )
  ;MessageBox MB_OK "In LicensePagePre"
  Push 1
  Call GetControlInfo
  /*
  GetDlgItem $0 $HWNDPARENT 1034
  ${IF} $0 == 0
    MessageBox MB_OK "$0 is 0"
  ${Else}
    ;MessageBox MB_OK "$0"
    EnableWindow $0 0
    ShowWindow $0 ${SW_HIDE}
    SendMessage $0 ${BM_SETCHECK} ${BST_CHECKED} 0
  ${EndIF}
  */
FunctionEnd


; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!define MUI_WELCOMEPAGE_TITLE "MUI_WELCOMEPAGE_TITLE"
!define MUI_WELCOMEPAGE_TEXT "MUI_WELCOMEPAGE_TEXT \r\n\r\n$_CLICK"
!insertmacro MUI_PAGE_WELCOME

; License page
LicenseForceSelection radiobuttons "accept_text" "decline_text"
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE LicensePageLeave
;!define MUI_LICENSEPAGE_CHECKBOX  ;MUI_LICENSEPAGE_RADIOBUTTONS
;!define MUI_PAGE_HEADER_TEXT "MUI_PAGE_HEADER_TEXT"
;!define MUI_PAGE_HEADER_SUBTEXT "MUI_PAGE_HEADER_SUBTEXT"
!insertmacro MUI_PAGE_LICENSE "License.txt"

; Components page
;!define MUI_PAGE_HEADER_TEXT "MUI_PAGE_HEADER_TEXT"
;!define MUI_PAGE_HEADER_SUBTEXT "MUI_PAGE_HEADER_SUBTEXT"
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!define MUI_DIRECTORYPAGE_TEXT_TOP "MUI_DIRECTORYPAGE_TEXT_TOP"
!define MUI_DIRECTORYPAGE_TEXT_DESTINATION "MUI_DIRECTORYPAGE_TEXT_DESTINATION"
!insertmacro MUI_PAGE_DIRECTORY

;variable
Var hmci     ;background music
   
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "NsisStudy"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_TITLE   "MUI_FINISHPAGE_TITLE"
!define MUI_FINISHPAGE_TEXT    "MUI_FINISHPAGE_TEXT"
!define MUI_FINISHPAGE_BUTTON  "MUI_FINISHPAGE_BUTTON"
!define MUI_FINISHPAGE_TEXT_REBOOT "MUI_FINISHPAGE_TEXT_REBOOT"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\ReadMe.txt"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "MUI_FINISHPAGE_SHOWREADME_TEXT"
!define MUI_FINISHPAGE_RUN "Calc"
!define MUI_FINISHPAGE_RUN_TEXT "MUI_FINISHPAGE_RUN_TEXT"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "TradChinese"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "NsisStudySetup.exe"
InstallDir "$PROGRAMFILES\NsisStudy"
ShowInstDetails show
ShowUnInstDetails show
BrandingText "  BrandingText"

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY

;;Splash Ч��
/*
   ;��ͨЧ��
   SetOutPath $TEMP
   File /oname=spltmp.bmp "splash.bmp"
   splash::show 2000 $TEMP\spltmp
   Pop $0
;*/

/*
   ;��������Ч��
   InitPluginsDir
   File /oname=$PLUGINSDIR\splash.bmp "splash.bmp"
   advsplash::show 1500 800 600 -1 $PLUGINSDIR\splash
;*/

/*
   ;;��������(�ᴩ��װ����) -- ��δʵ��, �˴��ǳ�ʼ�����ֵ� hmci ������ Ȼ���� Section �� SendMessage $hmci 0x0010 0 0
   InitPluginsDir
   SetOutPath $PLUGINSDIR
   File "music.mp3"             [˵����ԴMP3�Ĵ��·��]
   System::Call msvfw32.dll::MCIWndCreate(i 0, i 0,i 0x0070, t "$PLUGINSDIR\music.mp3") i .r0
   SendMessage $0 0x0465 0 "STR:play repeat"
   System::Call user32.dll::ShowWindow(i r0, i 0)
   StrCpy $hmci $0
*/

FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "NsisStudy.nsh"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\NsisStudy.lnk" "$INSTDIR\NsisStudy.nsh"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Misc" SEC02
  File "License.txt"
  File "ReadMe.txt"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Choose Main Section Files"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Choose Misc Files"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow

  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\ReadMe.txt"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\NsisStudy.nsh"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Website.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\NsisStudy.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose true
SectionEnd