@echo OFF
ECHO TIMES > timeLog.txt
FOR /L %%x IN (1, 1, 30) DO (
    ECHO %%x
    perfectSquare.exe >> timeLog.txt
)
PAUSE