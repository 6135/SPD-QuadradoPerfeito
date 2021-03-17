
set list=5001
FOR %%y IN (%list%) DO (
    FOR /L %%x IN (1, 1, 31) DO (
        set start = %time%
        perfectSquareSequential.exe inputs/i%%y.txt > logs.txt
        set end = %time%
        set interval = %end_ms% - %start_ms%
        echo %interval% >> seqTest/logTimesi%%y.txt
    )
)
