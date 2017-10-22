@ECHO ON

set "CASROOT=%~dp0"

SET "BINDIR=%CASROOT%\src\bin"
set "CSF_UnitsLexicon=%CASROOT%\src\UnitsAPI\Lexi_Expr.dat"
set "CSF_UnitsDefinition=%CASROOT%\src\UnitsAPI\Units.dat"
set "CSF_StandardDefaults=%CASROOT%\src\StdResource"
set "PATH=%BINDIR%;%PATH%"

start "" "%~dp0STH_OCC.exe
