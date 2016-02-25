::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin


call cmakeThirdParties.bat

cd ../

set compiler="C:\Program Files (x86)\MSBuild\14.0\Bin"

%compiler%\msbuild "ThirdParties/SFML/build/SFML.sln" /p:Configuration=Release /p:Platform="x64"
%compiler%\msbuild "ThirdParties/SFML/build/SFML.sln" /p:Configuration=Debug /p:Platform="x64"

	:: BUILDING QT
	:: note : if install fails because it "can't find unicode/unical.h" files, or stuff like that, you need to install the windows sdk
:: Set up \Microsoft Visual Studio 2015
cd ThirdParties/qt

if not defined DevEnvDir (
    CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
)

::CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

SET _CD=%CD%
SET _PATH=%PATH%
SET QT_SOURCE=%_CD%
SET QT_BUILD=%_CD%/build
SET QT_DIST=%_CD%/build/mscv2015_64

SET PATH=%QT_SOURCE%/qtbase/bin;%QT_SOURCE%/gnuwin32/bin;%PATH%
SET PATH=%QT_SOURCE%/qtrepotools/bin;%PATH%
SET QMAKESPEC=win32-msvc2015

call %QT_SOURCE%/configure -no-icu -mp -opensource -nomake examples -nomake tests -confirm-license -prefix %QT_DIST% -debug-and-release -skip qtwebkit -skip qtwebchannel -opengl desktop

RD /s /q %QT_BUILD%
MKDIR %QT_BUILD%
CD /D %QT_BUILD%

::nmake && nmake install
cd ThirdParties/qt
nmake
cd ../../

CD /D %_CD%

cd ../../

cd scripts
