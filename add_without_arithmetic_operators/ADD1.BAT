@rem subroutine of ADDDIG
set D2=%2
goto B%1
:B9
shift
shift
:B8
shift
shift
:B7
shift
shift
:B6
shift
shift
:B5
shift
shift
:B4
shift
shift
:B3
shift
shift
:B2
shift
shift
:B1
shift
shift
:B0
shift
shift
if "%D2%"=="0" goto NCARRY
shift
shift
:NCARRY
set D1=%1
set D2=%2
