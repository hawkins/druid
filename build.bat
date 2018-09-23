@echo off

::
:: [ ! MACHINE INDEPENDENT SETTING ! ]
::
cd /d D:\CODE\druid > nul
cd D: > nul

::
:: [ SYSTEM DEPENDENT SRC/LIB/INCLUDE LOCATIONS ]
::

@where cl >nul 2>nul
:: If cl was not found in path, initialize for x86
IF %ERRORLEVEL% NEQ 0 call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall" x86 >nul

::
:: [ COMPILER OPTIONS ]
::
:: /Z7	   Full symbolic debug info. No pdb. (See /Zi, /Zl).
:: /GS	   Detect buffer overruns.
:: /MD	   Multi-thread specific, DLL-specific runtime lib. (See /MDd, /MT, /MTd, /LD, /LDd).
:: /GL	   Whole program optimization.
:: /EHsc   No exception handling (Unwind semantics requrie vstudio env). (See /W1).
:: /I<arg> Specify include directory.

::
:: [ LINKER OPTIONS ]
::
:: /link                Invoke microsoft linker options.
:: /NXCOMPAT            Comply with Windows Data Execution Prevention.
:: /MACHINE:<arg>       Declare machine arch (should match vcvarsall env setting).
:: /NODEFAULTLIB:<arg>  Ignore a library.
:: /LIBPATH:<arg>       Specify library directory/directories

:: "%~1" prefix the first command line arg with the string "..\..\"
:: and remove quotations before seinding it as an argument to cl.
cl "lexer.c" -Z7 /GS /MD /EHsc /nologo ^
/link /NXCOMPAT /MACHINE:x86 /NODEFAULTLIB:MSVCRTD
::/LIBPATH: user32.lib
::gdi32.lib ^
::shell32.lib ^
::odbccp32.lib

:: exit
cmd /k
