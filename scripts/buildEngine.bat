::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin

call cmakeEngine.bat

cd ../

set compiler=C:\Program Files (x86)\MSBuild\14.0\Bin

"%compiler%\msbuild" "build/Runes.sln" /p:Configuration=Debug /p:Platform="x64"
"%compiler%\msbuild" "build/Runes.sln" /p:Configuration=Release /p:Platform="x64"


cd scripts