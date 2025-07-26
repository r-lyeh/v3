fart -- src\*.c "#include <spine" "//#include <spine"

cd inc\spine
for %%i in (*.h) do echo fart -- *.h "#include <spine/%%i>" "#include \"../spine/%%i\""
cd ..\..
