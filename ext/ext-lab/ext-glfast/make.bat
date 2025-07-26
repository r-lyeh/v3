if "%1"=="tidy" del *.exe *.exp *.lib *.obj *.ilk *.pdb & (for /R %%i in (.*) do @del /q %%i) & exit /b

cl demo01InstancingMTR.c /Zi && demo01InstancingMTR