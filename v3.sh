#!/bin/bash

cd `dirname $0`

echo '#!/bin/bash 2>nul || goto :windows' > v3.bat
cat v3.sh >> v3.bat

# linux + osx -----------------------------------------------------------------

if [ "$1" = "tidy" ]; then
    rm -rf *.dSYM 2> /dev/null
    rm a.out *.osx *.linux *.o 2> /dev/null
    rm v3unity.? v3.rc v3.res .*.zip 2> /dev/null
    rm *.o *.obj *.pdb *.ilk *.exp *.lib *.exe *.res *.dll 2> /dev/null
    exit
fi

if [ "$1" = "unity" ]; then
    echo '#include "v3.h"' > v3unity.c
    cc v3unity.c -E -I. > v3unity.h
    rm v3unity.c
    exit
fi

if [ "$1" = "sync" ]; then
    git reset --hard HEAD^1 && git pull
    sh v3.bat tidy
    exit
fi

############ cc -march=native -O3 

if [ "$(uname)" = "Linux" ]; then

    # setup (ArchLinux) ----------------------------------------------------------
    [ ! -f ".setup" ] && [ -x "$(command -v pacman)"  ] && sudo pacman -Sy && sudo pacman -Sy --noconfirm gcc && echo>.setup
    # setup (Debian, Ubuntu, etc) ------------------------------------------------
    [ ! -f ".setup" ] && [ -x "$(command -v apt-get)" ] && sudo apt-get -y update && sudo apt-get -y install gcc libx11-dev libgl1-mesa-dev mesa-common-dev libxrandr-dev libxcursor-dev libxi-dev libasound2-dev libxinerama-dev && echo>.setup

    # compile --------------------------------------------------------------------
    # -O1 -DNDEBUG=1 -Wno-unused-result
    echo cc v3.c -o v3 -lm -ldl -lasound -lpthread -g -I. $*
    time cc v3.c -o v3 -lm -ldl -lasound -lpthread -g -I. $* || exit

elif [ "$(uname)" = "Darwin" ]; then

    # compile (OSX) --------------------------------------------------------------
    # -O1 -DNDEBUG=1 
    export SDKROOT=$(xcrun --show-sdk-path)
    echo cc -ObjC v3.c -o v3 -framework cocoa -framework iokit -framework audiotoolbox -g -I. $*
    time cc -ObjC v3.c -o v3 -framework cocoa -framework iokit -framework audiotoolbox -g -I. $* || exit

else # mingw, git for windows, etc.

    cmd //c v3.bat $*

fi

exit

:windows
@echo off
cd /d "%~dp0"

for /f "tokens=1,* delims= " %%a in ("%*") do set ALL_FROM_2ND=%%b

if "%1"=="sync" (
    git reset --hard HEAD~1 && git pull
    exit /b
)

if "%1"=="ext" (
    rem invoke ext/ installer
    pushd ext
    call EXT.bat %ALL_FROM_2ND%
    popd
    exit /b
)

if "%1"=="asan" (
    %0 %ALL_FROM_2ND% -fsanitize=address
)

if "%1"=="tidy" (
    del v3.rc v3.res v3unity.? .*.zip *.o *.obj *.pdb *.ilk *.exp *.lib *.exe *.dll *.res >nul 2>nul
    for /R %%i in (.*) do del "%%i" >nul 2>nul
    exit /b
)

rem scan for files in any `embed/` subfolder and generate each xincbin entry
rem this block generates 2 files:
rem - the .rc file will be included by v3/v3_res.h, and
rem - the .res file will be linked in the binary.
if "%1"=="embed" if not exist "ext/ext-embed" exit /b
if "%1"=="embed" (
    setlocal enableDelayedExpansion

    echo #include "ext/ext-embed/3rd/xincbin.h"> v3.rc

    REM locate resources in embed folders. embed\subfolders\ allowed. slow.
    REM for /R . %%j in (*) do (
    REM     set "str1=%%j"
    REM     if not "x!str1:\embed\=!"=="x!str1!" (

    REM locate resources in embed folders. embed\subfolders\ NOT allowed. fast.
    for /R . %%j in (embed\*) do (
        rem normalized absolute path. left of embed/ folder is also discarded
        set "p=%%~fj"
        set "p=!p:*ext-embed\=!"
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

        echo XINCBIN^(!id!, "!p!"^) >> v3.rc
    )

    endlocal

    rem compile .rc to .res ; gcc does not need this
    where /q rc.exe && (rc /nologo v3.rc || goto error)

    exit /b
)

rem detect compiler
if "%CC%"=="" (
where /q cl.exe || call "%ProgramFiles%/microsoft visual studio/2022/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS170COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%ProgramFiles(x86)%/microsoft visual studio/2019/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS160COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%ProgramFiles(x86)%/microsoft visual studio/2017/community/VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS150COMNTOOLS%/../../VC/Auxiliary/Build/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS140COMNTOOLS%/../../VC/bin/x86_amd64/vcvarsx86_amd64.bat" >nul 2>nul
where /q cl.exe || call "%VS120COMNTOOLS%/../../VC/bin/x86_amd64/vcvarsx86_amd64.bat" >nul 2>nul
rem detect gcc: some users like this one, albeit it is ultra slow
rem where /q gcc || set "CC=gcc" >nul 2>nul
rem detect clang-cl: unified compiler across all major desktops + less WindowsDefender false positives
rem where /q clang-cl || set "CC=clang-cl -Wno-deprecated-non-prototype" >nul 2>nul
rem enforce cl: faster compilation times
set "CC=cl /nologo /MP"
)

rem generate ext/ include paths and resources
rem echo Generating ext/ includes...
call %0 ext gen
rem echo Generating resources...
call %0 embed
rem echo Generating docs/index file...
(echo #pragma once > docs\index ) & for %%i in (docs/*.c) do echo #include "../docs/%%i" >> docs\index

setlocal enableDelayedExpansion

if "%1"=="unity" (
    echo echo #include "v3.h" ^> v3unity.c
         echo #include "v3.h"  > v3unity.c
    echo !CC! v3unity.c -I. -E ^> v3unity.h
         !CC! v3unity.c -I. -E  > v3unity.h || goto error
    del v3unity.c
    goto ok
)

rem cl and clang-cl
(
    set ASAN=/fsanitize=address
    set SYM=/Zi
    set OPT=/DNDEBUG=3 /Ox /Oy /GL /GF
    set REL=/MT
    set RES=v3.res
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

rem scan options: files and flags are separated
set "list="
set "flag="
:loop
if "%1"=="" goto :end
set "arg=%1"
REM Check if the argument is an existing file
if exist "!arg!" (
    if defined list (
        set "list=!list! !arg!"
    ) else (
        set "list=!arg!"
    )
) else (
    if defined flag (
        set "flag=!flag! !arg!"
    ) else (
        set "flag=!arg!"
    )
)
shift
goto :loop
:end

if "!list!"=="" set list=v3.c

set old=%time%

echo !CC! !ARCH! !SYM! !REL! !RES! -I. !list! !flag! !LIBS!
     !CC! !ARCH! !SYM! !REL! !RES! -I. !list! !flag! !LIBS! || goto error

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

:ok
endlocal
exit /b 0

:error
endlocal
@copy , ,, >nul
