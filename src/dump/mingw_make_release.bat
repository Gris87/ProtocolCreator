if not "%~n3"=="" (

if not exist "%~dp2\symbols" (
mkdir "%~dp2\symbols"
)

echo "Creating symbol file for %~f2\%3"
"%~dp1\objcopy.exe" --only-keep-debug "%~f2\%3" "%~dp2\symbols\%~n3.sym"

echo "Stripping %~f2\%3"
"%~dp1\strip.exe" --strip-debug --strip-unneeded "%~f2\%3"

echo "Attaching symbol file to %~f2\%3"
"%~dp1\objcopy.exe" --add-gnu-debuglink="%~dp2\symbols\%~n3.sym" "%~f2\%3"
)