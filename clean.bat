@echo off
call :SafeRMDIR boost_build
call :SafeRMDIR build
call :SafeRMDIR lib
call :SafeRMDIR include\boost
pause
:SafeRMDIR
IF EXIST "%~1" (
	RMDIR /S /Q "%~1"
)
exit /b