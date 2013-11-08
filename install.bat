@echo off
@echo Adding Directories to PATH Variable...

for /F "tokens=2* delims= " %%f IN ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path ^| findstr /i path') do set OLD_SYSTEM_PATH=%%g
setx.exe PATH "%OLD_SYSTEM_PATH%;%CD%\coreutils\bin;%CD%\openocd-0.7.0\bin;" -m

@echo Complete!
PAUSE