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


//---
2025-02-20

### 1. Ensure you are on `main`
```bash
git checkout main
git pull origin main
```
- **git checkout main**: Switches your local repo to the `main` branch.
- **git pull origin main**: Fetches the latest changes from the remote `origin` and merges them into your local `main`. (Recommended to keep your `main` branch up-to-date before creating a new branch.)

---

### 2. Create a new branch `branch_01` from `main` and switch to it
```bash
git checkout -b branch_01
```
- The `-b` flag does two things at once:
  - Creates a new branch called `branch_01`.
  - Checks out that new branch so you are now working on it.

*(Alternatively, you can use `git switch -c branch_01` on newer Git versions, which does the same thing.)*

---

### 3. Switch back to `main`
```bash
git checkout main
```
- This will take you out of `branch_01` and back to `main`.

