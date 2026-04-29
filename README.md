<div align="center">
  <img src="icons/app_icon.svg" alt="FlashCard Studio app icon" width="84" height="84" />
  <h1>FlashCard Studio</h1>
  <p><b>Desktop app for creating and learning flashcards with spaced repetition.</b></p>
  <p>
    <img src="icons/add.svg" alt="Add icon" width="16" height="16" />
    &nbsp;Create decks quickly&nbsp;&nbsp;•&nbsp;&nbsp;Review smarter&nbsp;&nbsp;•&nbsp;&nbsp;Ship as Windows installer
  </p>
</div>

---

## Build Windows Installer (NSIS)

This project uses CMake + CPack to generate a Windows installer with Qt runtime dependencies included.

### Prerequisites

- Windows 10/11 x64
- CMake 3.21+
- Qt 6 (MinGW kit, Widgets)
- MinGW toolchain (for example `mingw1310_64`)
- NSIS (or run from CI where NSIS is preinstalled)

### Build Commands

From repository root:

```powershell
$env:Path = "C:\Qt\Tools\mingw1310_64\bin;$env:Path"
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/mingw_64"
cmake --build build-mingw --parallel
cmake --install build-mingw --prefix build-mingw/install
cpack -G NSIS --config build-mingw/CPackConfig.cmake
```

### Installer Output

The generated installer is placed in `build-mingw/` and has the `.exe` extension.

### Installer Includes

- `FlashCardStudio.exe`
- Required Qt runtime libraries and plugins (via Qt deploy script)
- Additional runtime libraries detected by CMake/Qt deploy
- Start Menu and Desktop shortcuts
- `.flcr` file association for FlashCard Studio projects
- Uninstaller registration

## Release Workflow

GitHub Actions workflow: `.github/workflows/release-installer.yml`

- Triggered manually (`workflow_dispatch`) and on tags (`v*`)
- Builds Release package on `windows-latest`
- Produces NSIS installer and uploads it as workflow artifact
