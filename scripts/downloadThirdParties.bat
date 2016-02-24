::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin

cd ../

set PATH=%PATH%;C:\Program Files\Git\bin;C:\Program Files (x86)\CMake\bin

git clone https://github.com/SFML/SFML ThirdParties/SFML

::pugi xml
git clone https://github.com/zeux/pugixml ThirdParties/pugi
:: moving include directory
xcopy /e /i /y "ThirdParties/pugi/src" "src/pugi"

cd scripts