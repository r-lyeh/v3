@echo off
setlocal enableDelayedExpansion
cd /d "%~dp0"

rem check first char of 2nd argument. process filename if it starts with '@'
IF NOT "%~2"=="" (
    SET firstChar=%2
    SET firstChar=!firstChar:~0,1!
    if "!firstChar!"=="@" (
        SET filename=%2
        SET filename=!filename:~1!
        for /f "tokens=* delims=" %%a in (!filename!) do (
            echo PLUG %1 %%a
            call PLUG.bat %1 %%a
        )
        exit /b
    )
)

rem show help screen
if "%1"=="" (
    echo plug ^[dir^] && echo plug ^[add^|del^|syn^] * && echo plug ^[add^|del^|syn^] user/repo && echo plug ^[add^|del^|syn^] @filelist && echo plug ^[gen^]
    exit /b
)

rem generate header files
if "%1"=="gen" (
    pushd .

    rem show ms redist dependences in any dll
    rem for /R %%i in (*.exe;*.dll) do dumpbin /dependents %%i |find /i "vcr"
    rem copy dlls/libs to root folder
    rem for /D /R %%i in (*) do if exist %%i\x64\* xcopy /ys %%i\x64\* .. >nul 2>nul

    rem generate ext.h file
    echo // auto-generated file. do not edit  > ext.h
    echo // list of extensions included here >> ext.h
    echo. >> ext.h
    for /D /R %%i in (*) do if exist %%i\api.h (
    set DIR=%%i
    set DIR=!DIR:%CD%\=!
    set DIR=!DIR:\=/!
    echo #if __has_include^("!DIR!/api.h"^) >> ext.h
    echo #         include "!DIR!/api.h" >> ext.h
    echo #endif >> ext.h
    )

    rem generate ext.hh file
    echo // auto-generated file. do not edit  > ext.hh
    echo // list of extensions included here >> ext.hh
    echo. >> ext.hh
    rem echo #ifdef __cplusplus >> ext.hh
    echo #pragma push_macro^("set"^) >> ext.hh
    echo #pragma push_macro^("map"^) >> ext.hh
    echo #pragma push_macro^("array"^) >> ext.hh
    echo #undef set >> ext.hh
    echo #undef map >> ext.hh
    echo #undef array >> ext.hh
    for /D /R %%i in (*) do if exist %%i\api.hh (
    set DIR=%%i
    set DIR=!DIR:%CD%\=!
    set DIR=!DIR:\=/!
    echo #if __has_include^("!DIR!/api.hh"^) >> ext.hh
    echo #         include "!DIR!/api.hh" >> ext.hh
    echo #endif >> ext.hh
    )
    echo #pragma pop_macro^("set"^) >> ext.hh
    echo #pragma pop_macro^("map"^) >> ext.hh
    echo #pragma pop_macro^("array"^) >> ext.hh
    rem echo #endif >> ext.hh

    rem generate ext-demos.c file
    echo // auto-generated file. do not edit  > ext-demos.c
    echo // list of extensions included here >> ext-demos.c
    echo. >> ext-demos.c
    for /D /R %%i in (*) do if exist %%i\demo.c (
    set DIR=%%i
    set DIR=!DIR:%CD%\=!
    set DIR=!DIR:\=/!
    echo #if __has_include^("!DIR!/demo.c"^) >> ext-demos.c
    echo #         include "!DIR!/demo.c" >> ext-demos.c
    echo #endif >> ext-demos.c
    )

    popd

    endlocal

    exit /b
)

rem list plugins and descriptions
rem echo fetching...

:dir_ext
rem curl -s "https://api.github.com/search/repositories?q=topic:ext-v3+&sort=updated&order=desc" > plugs.0
    curl -s "https://api.github.com/search/repositories?q=topic:ext-v3+&sort=name&order=asc" > plugs.0

:process_plugs
findstr /C:"\"description\"" /C:"\"clone_url\"" plugs.0 > plugs.1 && rem /C:"\"name\"" 

rem remove keys, commas and quotes
if exist plugs.x del plugs.x
for /f "tokens=*" %%i in (plugs.1) do (
    set var=%%i
    set var=!var:,=!
    set var=!var:"name": =!
    set var=!var:"clone_url": =!
    set var=!var:"description": =!
    set var=!var:https://github.com/=!
    set var=!var:.git=!
    set var=!var:"=!
    echo !var! >> plugs.x
)

rem determine number of lines
for /f %%i in ('find /c /v "" ^< plugs.x') do set "cnt=%%i"

rem read the file into an array
<plugs.x (
    for /l %%i in (1 1 %cnt%) do (
        set "str.%%i="
        set /p "str.%%i="
    )
)

rem combine odd/even lines
for /l %%i in (1 2 %cnt%) do (
    set /A from=%%i+1
    set /A to=%%i+2
    for /l %%u in (!from! 2 !to!) do (

        set "full=./!str.%%u!"

        rem split owner/prj
        for %%a in ("!full!") do for %%b in ("%%~dpa\.") do set "owner=%%~nxb"
        call :basename !full! prj
        rem echo full=!full! ^| owner=!owner! ^| prj=!prj!

        rem iterate list and perform action
               if "%1"=="add" (
            if "%2" == "!owner!/!prj!" ( call :add !owner! !prj! ) else if "%2" == "*" ( call :add !owner! !prj! )
        ) else if "%1"=="del" (
            if "%2" == "!owner!/!prj!" ( call :era !owner! !prj! ) else if "%2" == "*" ( call :era !owner! !prj! )
        ) else if "%1"=="syn" (
            if "%2" == "!owner!/!prj!" ( call :syn !owner! !prj! ) else if "%2" == "*" ( call :syn !owner! !prj! )
        ) else if "%1"=="dir" (
            if exist "!prj!\.git\@!owner!" (echo [x] %0 add !str.%%u!: !str.%%i!) else (echo [ ] %0 add !str.%%u!: !str.%%i!)
        )
    )
)

rem clean up
del plugs.*

rem generate header files
call %0 gen
GOTO :EOF

:basename
set "%2=%~nx1"
GOTO :EOF

:add
if not exist "%2\.git\@%1" (
    git clone https://github.com/%1/%2 -q --depth=1 --recursive && (echo. > "%2\.git\@%1") && echo [x] added %1/%2
) else ( echo [x] skipped %1/%2 )
GOTO :EOF

:era
if exist "%2\.git\@%1" (
    rd /q /s "%2" && echo [x] deleted %1/%2
) else rem echo [ ] %1/%2 not installed
GOTO :EOF

:syn
if exist "%2\.git\@%1" (
    pushd "%2"
        git fetch
        git rev-list --count HEAD..@{u} > repo.0
        findstr /m "0" "repo.0" >nul
        if !errorlevel!==0 (
            echo [x] synced %1/%2
        ) else (
            git pull -q && echo [x] updated %1/%2
        )
        del repo.0
    popd
) else rem echo [ ] %1/%2 not installed
GOTO :EOF
