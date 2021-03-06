@echo off
rem %1 is chk or fre 
rem %2 is x86 / ia64 / x64
rem %3 is WIN7 / WLH / WXP / W2K or WNET
rem %4 is copy source directory path
rem %5 is copy source file(with out extersion name)
rem %6 is copy target dir

rem WIN32=100;_X86_=1;WINVER=0x501;DBG=1

@echo on
call set_wdk_env.bat

rem if "%7"=="/a" call wdk_clean %1 %2

pushd.
rem call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 no_oacr
call %WDKPATH%\bin\setenv.bat %WDKPATH% %1 %2 %3 
popd

rem ml64 /c AsmHelperFun.asm

@echo on
build

rem /t http://timestamp.verisign.com/scripts/timestamp.dll
if %_BUILDARCH%==x86 signtool sign /v /ac VeriSignG5.cer /f fishjam.pfx /p wood5566 obj%BUILD_ALT_DIR%\i386\FJDriverDemo32.sys
if %_BUILDARCH%==x86 xcopy obj%BUILD_ALT_DIR%\i386\FJDriverDemo32.sys %4 /Y
if %_BUILDARCH%==x86 xcopy obj%BUILD_ALT_DIR%\i386\FJDriverDemo32.pdb %4 /Y
if %_BUILDARCH%==AMD64 signtool sign /v /ac VeriSignG5.cer /f fishjam.pfx /p wood5566 obj%BUILD_ALT_DIR%\amd64\FJDriverDemo64.sys
if %_BUILDARCH%==AMD64 xcopy obj%BUILD_ALT_DIR%\amd64\FJDriverDemo64.sys %4 /Y
if %_BUILDARCH%==AMD64 xcopy obj%BUILD_ALT_DIR%\amd64\FJDriverDemo64.pdb %4 /Y
