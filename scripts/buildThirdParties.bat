::Path to set in your PATH
::C:\Program Files (x86)\MSBuild\14.0\Bin
::C:\Program Files\Git\bin
::C:\Program Files (x86)\CMake\bin


call cmakeThirdParties.bat

cd ../

::set compiler="C:\Program Files (x86)\MSBuild\14.0\Bin"

::%compiler%\msbuild "ThirdParties/SFML/build/SFML.sln" /p:Configuration=Release /p:Platform="x64"
::%compiler%\msbuild "ThirdParties/SFML/build/SFML.sln" /p:Configuration=Debug /p:Platform="x64"

	:: BUILDING QT
	:: note : if install fails because it "can't find unicode/unical.h" files, or stuff like that, you need to install the windows sdk
:: Set up \Microsoft Visual Studio 2015
cd ThirdParties/qt

REM if not defined DevEnvDir (
    REM CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
REM )

::CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

::SET _CD=%CD%
::SET _PATH=%PATH%
::SET QT_SOURCE=%_CD%
::SET QT_BUILD=%_CD%/build
::SET QT_DIST=%_CD%/build/mscv2015_32

::SET PATH=%QT_SOURCE%/qtbase/bin;%QT_SOURCE%/gnuwin32/bin;%PATH%
::SET PATH=%QT_SOURCE%/qtrepotools/bin;%PATH%
::SET QMAKESPEC=win32-msvc2015


::set PATH=%cd%\bin;%PATH%

REM Set up \Microsoft Visual Studio 2013, where <arch> is \c amd64, \c x86, etc.
CALL "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86


SET _ROOT=%CD%
SET PATH=%_ROOT%\qtbase\bin;%_ROOT%\gnuwin32\bin;%PATH%
REM Uncomment the below line when using a git checkout of the source repository
REM SET PATH=%_ROOT%\qtrepotools\bin;%PATH%
SET QMAKESPEC=win32-msvc2013
SET _ROOT=

::call %QT_SOURCE%/configure -no-icu -mp -opensource -nomake examples -nomake tests -confirm-license -prefix %QT_DIST% -debug-and-release -skip qtwebkit -skip qtwebchannel -opengl desktop
call configure -no-icu -mp -opensource -nomake examples -nomake tests -confirm-license -debug-and-release -skip qtwebkit -skip qtwebchannel -opengl desktop

nmake & nmake install

::xcopy /e /i /y "ThirdParties/qt/qtbase/lib/*.dll" "ThirdParties/qt/qtbase/bin/"

::TODO copy qt dlls from qt/qtbase/lib into qt/qtbase/bin

CD /D %_CD%

cd ../../

cd scripts
