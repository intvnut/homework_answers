@REM Adds digits %1 and %2 (and possible carry %3), putting results in 
@REM variables D1 and D2
if not "%3"=="" goto CARRY
set D2=0
goto A%1
:CARRY
set D2=%3
goto A%1
:A0
CALL ADD1 %2 %D2% 0 0 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 1 0
goto end
:A1
CALL ADD1 %2 %D2% 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 1 0 1 1
goto end
:A2
CALL ADD1 %2 %D2% 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 1 0 1 1 1 2
goto end
:A3
CALL ADD1 %2 %D2% 0 3 0 4 0 5 0 6 0 7 0 8 0 9 1 0 1 1 1 2 1 3
goto end
:A4
CALL ADD1 %2 %D2% 0 4 0 5 0 6 0 7 0 8 0 9 1 0 1 1 1 2 1 3 1 4
goto end
:A5
CALL ADD1 %2 %D2% 0 5 0 6 0 7 0 8 0 9 1 0 1 1 1 2 1 3 1 4 1 5
goto end
:A6
CALL ADD1 %2 %D2% 0 6 0 7 0 8 0 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6
goto end
:A7
CALL ADD1 %2 %D2% 0 7 0 8 0 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7
goto end
:A8
CALL ADD1 %2 %D2% 0 8 0 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8
goto end
:A9
CALL ADD1 %2 %D2% 0 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1 7 1 8 1 9
goto end
:A%1
echo Error in arguments.  usage "CALL ADD digit digit"
echo Result is in D1 and D2
:end
