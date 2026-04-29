# FlashCard Studio

Desktop application for creating and learning flashcards with spaced repetition.

## Build Windows Installer (NSIS)

This project uses CMake + CPack to generate a Windows installer that includes Qt runtime dependencies.

### Prerequisites

- Windows 10/11 x64
- CMake 3.21+
- Qt 6 (MinGW kit, Widgets)
- MinGW toolchain (for example `mingw1310_64`)
- NSIS (or run from CI where NSIS is preinstalled)

### Commands

From repository root:

```powershell
$env:Path = "C:\Qt\Tools\mingw1310_64\bin;$env:Path"
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
cmake --build build-mingw --parallel
cmake --install build-mingw --prefix build-mingw/install
cpack -G NSIS --config build-mingw/CPackConfig.cmake
```

### Installer Output

Generated installer is placed in the `build-mingw/` directory and has `.exe` extension.

### What the installer does

- Installs `FlashCardStudio.exe`
- Deploys required Qt runtime libraries/plugins via Qt deploy script
- Installs required runtime libraries detected by CMake/Qt deploy
- Creates Start Menu and Desktop shortcuts
- Registers `.flcr` file association to open projects with FlashCard Studio
- Adds uninstaller

## Release Workflow

GitHub Actions workflow: `.github/workflows/release-installer.yml`

- Runs on manual trigger (`workflow_dispatch`) and git tags (`v*`)
- Builds Release package on `windows-latest`
- Produces NSIS installer and uploads it as workflow artifact

## Post-install Smoke Check

After installing on a clean machine:

1. Launch FlashCard Studio from Start Menu.
2. Create a project and save it as `.flcr`.
3. Close the app and open that `.flcr` by double-clicking it in Explorer.
4. Verify deck list, edit view, and learn view open correctly.
5. Uninstall from Apps & Features and verify app binaries are removed.

