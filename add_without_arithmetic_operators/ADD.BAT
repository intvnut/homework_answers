@echo off
@rem Adds two numbers that are passed as follows
@rem ADD (number 1) : (number 2) :
@rem where each number is expressed as a series of digits
@rem a space between each digit.  Answer is in SUM.
@rem first reverse operands, as we're doing a sequential add
SET A1=
SET A2=
SET SUM=
:L1
if "%1"==":" goto EL1
if "%1"=="" goto ERR
set A1=%1 %A1%
shift 
goto L1
:EL1
shift
:L2
if "%1"==":" goto EL2
if "%1"=="" goto EL2
set A2=%1 %A2%
shift
goto L2
:EL2
set A2=%A1% : %A2% :
set A1=
ADD2 %A2%
goto END
:ERR
echo You goofed.
:END
