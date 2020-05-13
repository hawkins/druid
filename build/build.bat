@echo off

@SET APP_NAME="druid"
@SET APP_ARCH=x64

:: Build directory setup
@SET BUILD_DIR=%cd%

:: Code directory setup
@SET CODE_DIR=%BUILD_DIR%\..\code
echo %CODE_DIR%
IF NOT EXIST %CODE_DIR%\NUL GOTO :ERROR_CODE_DIR

:: Output directory setup
@SET OUTPUT_DIR=%BUILD_DIR%\..
echo %OUTPUT_DIR%
IF NOT EXIST %OUTPUT_DIR%\NUL GOTO :ERROR_OUTPUT_DIR


::
:: Initialize cl.exe for correct environment.
::
@where cl >nul 2>nul
:: VS 2017 Community Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2017 Professional Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2017 Enterprise Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall" %APP_ARCH% >nul

:: VS 2019 Community Edition
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul

:: VS 2019 Enterprise Edition
rem IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %APP_ARCH% >nul


::
:: Compile & Link
::
:: /TC                  Compile as C code.
:: /TP                  Compile as C++ code.
:: /Oi                  Enable intrinsic functions.
:: /Od 	                Disables optimization.
:: /Qpar                Enable parallel code generation.
:: /Ot                  Favor fast code (over small code).
:: /Ob2                 Enable full inline expansion. [ cfarvin::NOTE ] Debugging impact.
:: /Z7	                Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	                Detect buffer overruns.
:: /MD	                Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /GL	                Whole program optimization.
:: /EHsc                No exception handling (Unwind semantics requrie vstudio env). (See /W1).
:: /WX                  Treat linker errors as warnings; no output file produced.
:: /I<arg>              Specify include directory.
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories.
IF %ERRORLEVEL% NEQ 0 GOTO :exit
mkdir msvc_landfill >nul 2>nul
pushd msvc_landfill >nul

cl %CODE_DIR%\\%APP_NAME%.c /WX /Oi /Qpar /Ot /W4 /WX /Ob2 /O2 /GS /GL /MD /EHsc /nologo ^
/I%CODE_DIR%\ ^
/I%CODE_DIR%\compiler_tools ^
/I%CODE_DIR%\data_structures ^
/link /SUBSYSTEM:CONSOLE /NXCOMPAT /MACHINE:x64 /NODEFAULTLIB:MSVCRTD ^
user32.lib ^
gdi32.lib ^
shell32.lib ^
odbccp32.lib

IF %ERRORLEVEL% NEQ 0 GOTO :exit
xcopy /y %APP_NAME%.exe %OUTPUT_DIR% >null
popd >null
GOTO :exit

::
:: ERROR_CODE_DIR
::
:ERROR_CODE_DIR
echo The parent directory didn't have a 'code' subdirectory. Run this script from the 'build' directory.
GOTO :exit

::
:: ERROR_OUTPUT_DIR
::
:ERROR_OUTPUT_DIR
echo There doesn't seem to be a parent directory. Run this script from the 'build' directory.
GOTO :exit


:exit
