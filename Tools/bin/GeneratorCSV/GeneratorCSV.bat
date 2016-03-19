cd %~dp0
python %~dp0GeneratorCSV.py -i .\xls_config -o ..\..\..\bin\vs14.0\x64\DLL_Debug_x64\csv_config -c ..\..\..\vsproject\TestLibCore\CSVConfigs
pause