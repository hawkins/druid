@echo off

pushd
@where cl >nul 2>nul
:: If cl was not found in path, initialize for x86
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" x86 >nul

::
:: [ COMPILER OPTIONS ]
::
:: /Za     Emits an error for language constructs that are not compatible with ANSI C89 or ISO C++11.
:: /Z7	   Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	   Detect buffer overruns.
:: /MD	   Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /GL	   Whole program optimization.
:: /EHsc   No exception handling (Unwind semantics requrie vstudio env). (See /W1).
:: /I<arg> Specify include directory.
:: /Wall   Displays all warnings, including those that have been turned off by default.
:: /W3     Displays all "production quality" warnings.
:: /WX     Treats all compiler warnings as errors.

::
:: [ LINKER OPTIONS ]
::
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories

popd

:: "%~1" prefix the first command line arg with the string "..\..\"
:: and remove quotations before seinding it as an argument to cl.
cl %~1 /W3 /WX /Za -Z7 /GS /MD /EHsc /nologo /link /NXCOMPAT /MACHINE:x86 /NODEFAULTLIB:MSVCRTD
::/LIBPATH: user32.lib
::gdi32.lib ^
::shell32.lib ^
::odbccp32.lib

:: exit
cmd /k
