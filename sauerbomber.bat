IF /I "%PROCESSOR_ARCHITECTURE:~-2%"=="64" start bin64\sauerbomber.exe -glog.txt -r %* 2>stderr.txt >stdout.txt < NUL
IF /I "%PROCESSOR_ARCHITECTURE:~-2%"=="86" start bin\sauerbomber.exe -glog.txt -r %* 2>stderr.txt >stdout.txt < NUL
