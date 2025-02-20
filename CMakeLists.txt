# set minimum cmake version
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

set(CMAKE_GENERATOR_PLATFORM x64)

# project name and language
project (ex618_guile_004_01_prj  LANGUAGES CXX)


IF (MSVC)
	message(STATUS " MSVC version '${MSVC_VERSION}'")
	IF(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
		set(CMAKE_CXX_STANDARD 20)
	ELSEIF(MSVC_VERSION GREATER_EQUAL 1930)
		set(CMAKE_CXX_STANDARD 23)
	ENDIF()

	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
ELSE()
	set(CMAKE_CXX_STANDARD 23)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
ENDIF()


IF (MSVC)
	# Activate C++ exception handling
	IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
	ENDIF ()
ELSE ()
	if(CMAKE_BUILD_TYPE STREQUAL "")
		# Build type is not set e.g., command was "cmake .."
		message(STATUS " Build type unspecified, set to Release")
		set(CMAKE_BUILD_TYPE Release)
	else()
		message(STATUS " Build type specified as '${CMAKE_BUILD_TYPE}'")
	endif()

	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wnull-dereference")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		SET(GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS "-fconcepts")
		SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_CONCEPTS_COMPILE_FLAGS}")
	endif()

	SET(GCC_STD_CPP_PERMISIV_COMPILE_FLAGS "-fpermissive")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_STD_CPP_PERMISIV_COMPILE_FLAGS}")
ENDIF ()

set(INCLUDE_LOCAL_DIRS "test/inc" "lib/inc" "inc")


IF (MSVC)
	IF(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
		SET(INCLUDE_DIRS "C:/Qt/5.15.2/msvc2019_64/include/QtWidgets")
		SET(QT_LIB_DIR "C:/Qt/5.15.2/msvc2019_64/lib")
	ELSEIF(MSVC_VERSION GREATER_EQUAL 1930)
		SET(INCLUDE_DIR "C:/Qt/5.15.2/msvc2019_64/include/QtWidgets")
#		SET(INCLUDE_DIR_03 "C:/Qt/5.15.2/msvc2019_64/include/QtScript")
#		SET(QT_LIB_DIR "C:/Qt/5.15.2/msvc2019_64/lib")
	ENDIF()
ELSE ()

ENDIF ()

include_directories(${INCLUDE_DIRS})
link_directories(${QT_LIB_DIR})

find_package(Qt5 COMPONENTS Script Core Widgets Qml REQUIRED)

# Enable the AUTOMOC feature
set(CMAKE_AUTOMOC ON)

#------------------------------------------------------------------------------
# add app
#------------------------------------------------------------------------------
SET(GUILE_APP_NAME "guile_app_04_01")
add_executable(${GUILE_APP_NAME}

	inc/widget_factory.h
	src/widget_factory.cpp

	inc/dialog_dynamic.h
	src/dialog_dynamic.cpp
	
	main/main_01.cpp
)
target_link_libraries(${GUILE_APP_NAME} Qt5::Core Qt5::Widgets Qt5::Qml)
target_include_directories(${GUILE_APP_NAME} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_LOCAL_DIRS})


#------------------------------------------------------------------------------
# add app 2
#------------------------------------------------------------------------------
SET(GUILE_APP_NAME_02 "guile_app_04_02")
add_executable(${GUILE_APP_NAME_02}

	inc/widget_factory_02.h
	src/widget_factory_02.cpp

	inc/dialog_dynamic_02.h
	src/dialog_dynamic_02.cpp
	
	main/main_02.cpp
)
target_link_libraries(${GUILE_APP_NAME_02} Qt5::Core Qt5::Widgets Qt5::Qml)
target_include_directories(${GUILE_APP_NAME_02} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_LOCAL_DIRS})


#------------------------------------------------------------------------------
# add app 3
#------------------------------------------------------------------------------
SET(GUILE_APP_NAME_03 "guile_app_04_03")
add_executable(${GUILE_APP_NAME_03}

	inc/widget_factory_03.h
	src/widget_factory_03.cpp

	inc/dialog_dynamic_03.h
	src/dialog_dynamic_03.cpp
	
	main/main_03.cpp
)
target_link_libraries(${GUILE_APP_NAME_03} Qt5::Script Qt5::Core Qt5::Widgets)
target_include_directories(${GUILE_APP_NAME_03} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_DIRS_03} ${INCLUDE_LOCAL_DIRS})


#------------------------------------------------------------------------------
# add app 5
#------------------------------------------------------------------------------
SET(GUILE_APP_NAME_05 "guile_app_04_05")
add_executable(${GUILE_APP_NAME_05}

	inc/widget_factory_05.h
	src/widget_factory_05.cpp

	inc/dialog_dynamic_05.h
	src/dialog_dynamic_05.cpp

	main/main_05.cpp
)
target_link_libraries(${GUILE_APP_NAME_05} Qt5::Script Qt5::Core Qt5::Widgets)
target_include_directories(${GUILE_APP_NAME_05} PUBLIC ${INCLUDE_DIRS} ${INCLUDE_DIRS_03} ${INCLUDE_LOCAL_DIRS})

