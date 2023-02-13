@rem second phase of 2 number add--interleave addens and pad to same len.
set A2=
if "%1%2"=="::" goto DONE
if "%1"=="" goto ERR
if "%1"==":" goto E1
set A1=%A1% %1
shift
goto EE1
:E1
set A1=%A1% 0
rem if "%1"==":" goto DONE
rem if "%1"=="" goto DONE
:EE1
if "%1"==":" goto EEE1
if "%1"=="" goto ERR
set A2=%A2% %1
shift
goto EE1
:EEE1
shift
set A2=%A2% :
if "%1"==":" goto E2
if "%1"=="" goto E2
set A1=%A1% %1
shift
goto EE2
:E2
set A1=%A1% 0
:EE2
if "%1"==":" goto EEE2
if "%1"=="" goto EEE2
set A2=%A2% %1
shift
goto EE2
:EEE2
set A2=%A2% :
ADD2 %A2% 
goto END
:DONE
ADD3 %A1% 0 :
goto END
:ERR
echo An error occured.
:END
