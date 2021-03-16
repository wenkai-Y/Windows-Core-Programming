@ECHO OFF
:main
echo.--------------------------------------
echo.即将删除Visual Studio所产生的多余数据！
echo.--------------------------------------
echo.--------------------------------------
echo. 1.Number 1 to go on.
echo. 2.Number 2 to exit.
echo.--------------------------------------
set /p var=Please Enter The Number:
echo.
if %var%==1 goto clean
goto end

:clean
FOR /R . %%d IN (.) DO rd /s /q "%%d\x64" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\AutoPCH" 2>nul
del /s /a *.txt *.exe *.suo *.ncb *.user *.db *.dll *.pdb *.netmodule *.aps *.ipch *.ilk 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Debug" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Release" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Bin" 2>nul
FOR /R . %%d IN (.) DO rd /s /q "%%d\Obj" 2>nul
echo The data has been deleted
goto end

:end
echo Please press any key to exit
@Pause>nul