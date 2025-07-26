#!/bin/bash 2>nul || goto :windows
# run `sh MAKE.bat` to compile

# linux + osx -----------------------------------------------------------------
cd `dirname $0`

if [ "$1" = "tidy" ]; then
    rm *.o 2> /dev/null
    rm a.out 2> /dev/null
    rm *.osx 2> /dev/null
    rm .*.zip 2> /dev/null
    rm *.linux 2> /dev/null
    exit
fi

if [ "$1" = "sync" ]; then
    git reset --hard HEAD^1 && git pull
    sh MAKE.bat tidy
    exit
fi

if [ "$(uname)" != "Darwin" ]; then

# setup (ArchLinux) ----------------------------------------------------------
[ ! -f ".setup" ] && [ -x "$(command -v pacman)"  ] && sudo pacman -Sy && sudo pacman -Sy --noconfirm gcc && echo>.setup
# setup (Debian, Ubuntu, etc) ------------------------------------------------
[ ! -f ".setup" ] && [ -x "$(command -v apt-get)" ] && sudo apt-get -y update && sudo apt-get -y install gcc libx11-dev libgl1-mesa-dev mesa-common-dev libxrandr-dev libxcursor-dev libxi-dev && echo>.setup

# compile --------------------------------------------------------------------
echo gcc hello.c -o hello.linux -O1 -DNDEBUG=1 -lm -ldl -lXrandr -g -Wno-unused-result -Wno-multichar $* || exit
     gcc hello.c -o hello.linux -O1 -DNDEBUG=1 -lm -ldl -lXrandr -g -Wno-unused-result -Wno-multichar $* || exit

fi

if [ "$(uname)" = "Darwin" ]; then

# compile --------------------------------------------------------------------
export SDKROOT=$(xcrun --show-sdk-path)
gcc -ObjC hello.c -o hello.osx -O1 -DNDEBUG=1 -framework cocoa -framework iokit -framework CoreFoundation -framework CoreAudio -framework OpenGL -ldl -lm -g -Wno-unused-result -Wno-multichar $* || exit

fi

exit

:windows
@echo off
cd /d "%~dp0"

for /f "tokens=1,* delims= " %%a in ("%*") do set ALL_FROM_2ND=%%b

if "%1"=="" (
    make hello.c
)

if "%1"=="sync" (
    git reset --hard HEAD~1 && git pull
    exit /b
)

if "%1"=="plug" (
    pushd v3\ext
    call PLUG.bat %ALL_FROM_2ND%
    popd
    exit /b
)

if "%1"=="asan" (
    call MAKE.bat %ALL_FROM_2ND% -fsanitize=address
    exit /b
)

if "%1"=="tidy" (
    del v3\v3.rc >nul 2>nul
    del v3\v3.res >nul 2>nul
    del *.res >nul 2>nul
    del *.obj >nul 2>nul
    del *.pdb >nul 2>nul
    del *.ilk >nul 2>nul
    del *.exp >nul 2>nul
    del *.lib >nul 2>nul
    del *.exe >nul 2>nul
    del .*.zip >nul 2>nul
    for /R %%i in (.*) do del "%%i" >nul 2>nul
    exit /b
)

rem detect compiler
if "%CC%"=="" (
where /q cl.exe || call "%VS170COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS160COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS150COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS140COMNTOOLS%/../../VC/bin/x86_amd64/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS120COMNTOOLS%/../../VC/bin/x86_amd64/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%ProgramFiles%/microsoft visual studio/2022/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%ProgramFiles(x86)%/microsoft visual studio/2019/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%ProgramFiles(x86)%/microsoft visual studio/2017/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
rem detect gcc: some users like this one, albeit it is ultra slow
rem where /q gcc || set "CC=gcc" >nul 2>nul
rem detect clang-cl: unified compiler across all major desktops + less WindowsDefender false positives
rem where /q clang-cl || set "CC=clang-cl -Wno-deprecated-non-prototype" >nul 2>nul
rem enforce cl: faster compilation times
set "CC=cl /nologo"
)

rem regenerate ext\ include paths
pushd .
call v3\ext\MAKE.bat
popd

setlocal enableDelayedExpansion

rem scan for files in any `embed/` subfolder and generate each xincbin entry
rem this block generates 2 files:
rem - the .rc file will be included by v3/v3_res.h, and
rem - the .res file will be linked in the binary.
if not "%1"=="no-embed" (
    echo #include "ext/ext-embed/xincbin.h"> v3/v3.rc

    for /R . %%j in (*) do (
        set "str1=%%j"
        if not "x!str1:\embed\=!"=="x!str1!" (
            rem normalized absolute path. left of embed/ folder is also discarded
            set "p=%%~fj"
            set "p=!p:*embed\=!"

            rem normalize id to a valid C identifier, by replacing period, slashes, whitespaces, etc.
            set "id=!p!"
            set "id=!id: =_!"
            set "id=!id:;=_!"
            set "id=!id:-=_!"
            set "id=!id:/=_!"
            set "id=!id:\=_!"
            set "id=!id:.=_!"

            rem prevent unescaped and malformed "\a\b\c" backslashes that will be interpreted as C strings
            set "p=%%~fj"
            set "p=!p:\=/!"

            echo XINCBIN^(!id!, "!p!"^) >> v3/v3.rc
        )
    )

    rem compile .rc to .res ; gcc does not need this
    where /q rc.exe && rc /nologo v3/v3.rc
)

rem cl and clang-cl
(
    set ASAN=/fsanitize=address
    set SYM=/Zi
    set OPT=/DNDEBUG=3 /Ox /Oy /GL /GF
    set REL=/MT
    set RES=v3\v3.res
    set LIBS=
    rem X86 use /arch:SSE2 (instead of /arch:SSE) to maximize performance
    rem X64 use /arch:AVX (instead of /arch:AVX2) to maximize compatibility
    if "%__DOTNET_PREFERRED_BITNESS%"=="32" (set ARCH=/arch:SSE2) else (set ARCH=/arch:AVX)
    rem link .res file only if present
    if not exist !RES! set RES=
)

rem gcc family
if "%CC%"=="gcc" (
    set ASAN=
    set SYM=-g
    set OPT=-DNDEBUG=2 -O2
    set REL=
    set RES=
    set ARCH=
    set LIBS=-lgdi32
)

set old=%time%

echo !CC! !ARCH! !SYM! !REL! !RES! -I. %* !LIBS!
     !CC! !ARCH! !SYM! !REL! !RES! -I. %* !LIBS! || goto error

set now=%time%
set "old=!old::0=: !"
set "old=!old:,0=: !"
set "old=!old:.0=: !"
set "now=!now::0=: !"
set "now=!now:,0=: !"
set "now=!now:.0=: !"
set cs0=!old:~0,2!*3600*100+!old:~3,2!*60*100+!old:~6,2!*100+!old:~9,2!
set cs1=!now:~0,2!*3600*100+!now:~3,2!*60*100+!now:~6,2!*100+!now:~9,2!
set /A diff=!cs1!-^(!cs0!^)
set diff=!diff:~0,-2!.!diff:~-2!
echo !diff!s

endlocal
exit /b 0

:error
endlocal
@copy , ,, >nul
