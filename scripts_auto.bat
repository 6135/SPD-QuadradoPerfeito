@echo OFF
set list=31 51 101 1001 2001 3001 5000
FOR %%y IN (%list%) DO (
    FOR /L %%x IN (1, 1, 31) DO (
    perfectSquare.exe inputs/i%%y.txt >> logTimesi%%y.txt
    )
)
FOR %%y IN (%list%) DO (
    FOR /L %%x IN (1, 1, 31) DO (
    perfectSquare.exe inputs/p%%y.txt >> logTimesp%%y.txt
    )
)
FOR %%y IN (%list%) DO (
    FOR /L %%x IN (1, 1, 31) DO (
    perfectSquare.exe inputs/n%%y.txt >> logTimesn%%y.txt
    )
)