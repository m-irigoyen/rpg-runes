::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin

::you'll need to have perl installed : http://strawberryperl.com

cd ../

set PATH=%PATH%;C:\Program Files\Git\bin;C:\Program Files (x86)\CMake\bin

git clone https://github.com/SFML/SFML ThirdParties/SFML

::pugi xml
git clone https://github.com/zeux/pugixml ThirdParties/pugi
:: moving include directory
xcopy /e /i /y "ThirdParties/pugi/src" "src/pugi"

:: qt

git clone https://code.qt.io/qt/qt5.git ThirdParties/qt
cd ThirdParties/qt
git checkout 5.5

perl init-repository --no-webkit

cd ../../
cd scripts