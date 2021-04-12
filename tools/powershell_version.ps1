$files = Get-ChildItem "inputs\" -Filter *.txt
foreach ($file in $files){
	Write-Output $file.FullName
    $outfile = "seqTest\" + $file.BaseName + "_times" + $file.Extension
    for ($i = 0;$i -lt 3; $i++) {
        $file_p = "inputs\"+$file
        $result = Measure-Command { Invoke-Expression ".\m_threads.exe $file_p" } | findstr -i TotalMilliseconds
        Write-Output $result >> $outfile
    }
}