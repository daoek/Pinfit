@echo off
setlocal
rem Launcher installed next to pinfit.jar. Runs it on the Java runtime bundled next to it
rem (runtime\, made by jlink at release time) - never on a system Java, so nothing else needs to
rem be installed and a JAVA_HOME or PATH change can't break it.

if not exist "%~dp0runtime\bin\java.exe" goto missing
if not exist "%~dp0pinfit.jar" goto missing

"%~dp0runtime\bin\java.exe" -jar "%~dp0pinfit.jar" %*
exit /b %ERRORLEVEL%

:missing
echo Pinfit's bundled runtime is missing or incomplete in %~dp0 - reinstall Pinfit: 1>&2
echo   irm https://raw.githubusercontent.com/daoek/Pinfit/main/scripts/install.ps1 ^| iex 1>&2
exit /b 1
