# ex618_guile_004_01
I learn guile.

add to PATH
C:\Qt\5.15.2\msvc2019_64

QT_QPA_PLATFORM_PLUGIN_PATH 
C:\Qt\5.15.2\msvc2019_64\plugins\platforms

CMake GUI
H:/users/prj_app/dev/ex618_guile_004_01
H:/users/prj_app/build/ex618_guile_004_01

$env:PATH += ";U:\3rdparty\qt64_5.15.12\bin"

CMake gui
	Ungrouped Entries PATH

	Qt5_DIR				U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5
	Qt5Core_DIR			U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Core
	Qt5Gui_DIR			U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Gui
	Qt5Script_DIR		U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Script
	Qt5Widgets_DIR		U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Widgets

	Qt5Quick_DIR		U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Quick	
	Qt5Qml_DIR			U:/3rdparty/qt64_5.15.12/lib/cmake/Qt5Qml
	
H:\users\prj_app\build\ex618_guile_005\Debug	

//---
	Qt5_DIR				C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5

//---
Get-ChildItem Env: | Format-Table -AutoSize
...
QML2_IMPORT_PATH            C:\Qt\5.15.2\msvc2019_64\qml
QT_QPA_PLATFORM_PLUGIN_PATH C:\Qt\5.15.2\msvc2019_64\plugins\platforms
QTDIR                       C:\Qt\5.15.2\msvc2019_64

//---
$env:PATH += ";C:\Qt\5.15.2\msvc2019_64\bin"

($Env:PATH -split ';') | ForEach-Object { $_ }



