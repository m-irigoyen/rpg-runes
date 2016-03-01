CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(EXE_NAME "runes")

PROJECT(${EXE_NAME})

SET(EXECUTABLE_OUTPUT bin / ${CMAKE_BUILD_TYPE})

# Find qt
set(CMAKE_PREFIX_PATH c:/Qt/5.5/msvc2013_64/)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

#Source grouping
include_directories(${runes_SOURCE_DIR}/src)
include_directories(../ThirdParties/pugi/include/pugi)

FILE(GLOB_RECURSE SRC_MAIN ${runes_SOURCE_DIR}/src/main.cpp )
 FILE(GLOB_RECURSE SRC_ENGINE
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Engine.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Engine.h
	 )
 FILE(GLOB_RECURSE SRC_AUDIO
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Audio/*.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Audio/*.h
	 ${runes_SOURCE_DIR}/src/Engine/Audio/*.hpp
	 )
 FILE(GLOB_RECURSE SRC_GRAPHICS
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Graphics/*.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Graphics/*.h
	 ${runes_SOURCE_DIR}/src/Engine/Graphics/*.hpp
	 )
 FILE(GLOB_RECURSE SRC_NETWORK
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Network/*.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Network/*.h
	 ${runes_SOURCE_DIR}/src/Engine/Network/*.hpp
	 )
FILE(GLOB_RECURSE SRC_RUNES
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Runes/*.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Runes/*.h
	 ${runes_SOURCE_DIR}/src/Engine/Runes/*.hpp
	 )
FILE(GLOB_RECURSE SRC_USERPROFILES
	 ${runes_SOURCE_DIR}
	 ${runes_SOURCE_DIR}/src/Engine/Users/*.cpp 
	 ${runes_SOURCE_DIR}/src/Engine/Users/*.h
	 ${runes_SOURCE_DIR}/src/Engine/Users/*.hpp
	 )
FILE(GLOB_RECURSE SRC_UTILITIES 
	${runes_SOURCE_DIR}/src/Engine/Utilities/*.h	
	${runes_SOURCE_DIR}/src/Engine/Utilities/*.cpp	
	${runes_SOURCE_DIR}/src/Engine/Utilities/*.hpp
	)
FILE(GLOB_RECURSE SRC_PUGI
	 ${runes_SOURCE_DIR}/src/pugi*.cpp 
	 ${runes_SOURCE_DIR}/src/pugi*.hpp 
	 )
	 
	
SET(SOURCE_GROUP_DELIMITER "/")
source_group(Engine FILES ${SRC_ENGINE})
source_group(Engine/Audio FILES ${SRC_AUDIO})
source_group(Engine/Graphics FILES ${SRC_GRAPHICS})
source_group(Engine/Network FILES ${SRC_NETWORK})
source_group(Engine/Runes FILES ${SRC_RUNES})
source_group(Engine/Users FILES ${SRC_USERPROFILES})
source_group(Engine/Utilities FILES ${SRC_UTILITIES})
source_group(Pugi FILES ${SRC_PUGI})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
 
ADD_EXECUTABLE( ${EXE_NAME} ${SRC_MAIN} ${SRC_ENGINE} ${SRC_AUDIO} ${SRC_GRAPHICS} ${SRC_NETWORK} ${SRC_RUNES} ${SRC_UTILITIES} ${SRC_PUGI} ${SRC_USERPROFILES})

target_link_libraries(${EXE_NAME} Qt5::Widgets)
target_link_libraries(${EXE_NAME} Qt5::Core)
