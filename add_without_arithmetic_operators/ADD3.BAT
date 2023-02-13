@REM Final phase of add... takes interleaved addends and adds them.
SET SUM=
SET D1=0
:L1
if "%1"=="" goto ERR
if "%1"==":"  goto DONE
if "%2"=="" goto ERR
if "%2"==":"  goto DONE
call ADDDIG %1 %2 %D1%
SET SUM=%D2% %SUM%
shift 
shift
goto L1
:DONE
if "%D1%"=="0" goto END
set SUM=%D1% %SUM%
goto END
:ERR
echo An error occurred.
:END
echo %SUM%
