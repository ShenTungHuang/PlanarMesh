@echo off
rem Define CASROOT here explicitly if needed
set "CASROOT=C:\OpenCASCADE6.5.1\ros"
call "%CASROOT%\env.bat"
if not exist "%VS80COMNTOOLS%\vsvars32.bat" goto end
call "%VS80COMNTOOLS%\vsvars32.bat" > nul
start devenv.exe STH_OCC.sln
