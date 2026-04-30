cmake_minimum_required(VERSION 3.21)
project(FlashCardStudio VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/main.cpp

    src/models/Card.cpp
    src/models/Deck.cpp
    src/models/Project.cpp
    src/models/ProjectManager.cpp
    src/models/SpacedRepetitionLogic.cpp
    src/models/CardTableModel.cpp

    src/utils/FileIO.cpp
    src/utils/ThemeManager.cpp

    src/views/DeckItemDelegate.cpp
    src/views/DeckListWidget.cpp
    src/views/EditWidget.cpp
    src/views/LearnWidget.cpp
    src/views/StartScreen.cpp
    src/views/MainWindow.cpp
)

set(HEADERS
    src/models/Card.h
    src/models/Deck.h
    src/models/Project.h
    src/models/ProjectManager.h
    src/models/SpacedRepetitionLogic.h
    src/models/CardTableModel.h

    src/utils/FileIO.h
    src/utils/ThemeManager.h

    src/views/DeckItemDelegate.h
    src/views/DeckListWidget.h
    src/views/EditWidget.h
    src/views/LearnWidget.h
    src/views/StartScreen.h
    src/views/MainWindow.h
)

qt_add_executable(FlashCardStudio
    ${SOURCES}
    ${HEADERS}
    resources.qrc
)

target_include_directories(FlashCardStudio PRIVATE src)

target_link_libraries(FlashCardStudio PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if(WIN32)
    target_sources(FlashCardStudio PRIVATE "${CMAKE_SOURCE_DIR}/windows/app_icon.rc")
    set_target_properties(FlashCardStudio PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(APPLE)
    set_target_properties(FlashCardStudio PROPERTIES MACOSX_BUNDLE TRUE)
endif()

include(GNUInstallDirs)

install(TARGETS FlashCardStudio
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

if(WIN32 AND MINGW)
    foreach(_mingw_runtime_dll
        "libstdc++-6.dll"
        "libgcc_s_seh-1.dll"
        "libwinpthread-1.dll"
    )
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=${_mingw_runtime_dll}
            OUTPUT_VARIABLE _resolved_mingw_runtime_dll
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(EXISTS "${_resolved_mingw_runtime_dll}")
            install(FILES "${_resolved_mingw_runtime_dll}" DESTINATION bin)
        else()
            message(WARNING "MinGW runtime DLL not found: ${_mingw_runtime_dll}")
        endif()
    endforeach()
endif()

qt_generate_deploy_app_script(
    TARGET FlashCardStudio
    OUTPUT_SCRIPT qt_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(CODE [[
  if(NOT IS_ABSOLUTE "${CMAKE_INSTALL_PREFIX}")
    get_filename_component(_deploy_prefix_abs "${CMAKE_INSTALL_PREFIX}" ABSOLUTE BASE_DIR "${CMAKE_BINARY_DIR}")
    set(CMAKE_INSTALL_PREFIX "${_deploy_prefix_abs}")
  endif()
]])
install(SCRIPT ${qt_deploy_script})

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_NAME "FlashCard Studio")
    set(CPACK_PACKAGE_VENDOR "FlashCard Studio")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Flashcards desktop app with spaced repetition")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "FlashCard Studio")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    set(CPACK_PACKAGE_EXECUTABLES "FlashCardStudio" "FlashCard Studio")
    set(CPACK_NSIS_DISPLAY_NAME "FlashCard Studio")
    set(CPACK_NSIS_PACKAGE_NAME "FlashCard Studio")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "bin\\\\FlashCardStudio.exe")
    set(CPACK_NSIS_MENU_LINKS "bin\\\\FlashCardStudio.exe" "FlashCard Studio")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\FlashCardStudio.exe")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/icons/app_icon.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/icons/app_icon.ico")
    set(CPACK_NSIS_MODIFY_PATH OFF)
    #region agent log
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS [=[
WriteRegStr HKCU "Software\\Classes\\.flcr" "" "FlashCardStudio.Project"
WriteRegStr HKCU "Software\\Classes\\FlashCardStudio.Project" "" "FlashCard Studio Project"
WriteRegStr HKCU "Software\\Classes\\FlashCardStudio.Project\\DefaultIcon" "" "$INSTDIR\\bin\\FlashCardStudio.exe,0"
WriteRegStr HKCU "Software\\Classes\\FlashCardStudio.Project\\shell\\open\\command" "" '"$INSTDIR\\bin\\FlashCardStudio.exe" "%1"'
System::Call 'shell32::SHChangeNotify(i,i,p,p)'(0x8000000,0,0,0)
ReadRegStr $0 HKCR ".flcr" ""
ReadRegStr $1 HKCR "FlashCardStudio.Project\\shell\\open\\command" ""
ReadRegStr $2 HKCU "Software\\Classes\\.flcr" ""
ReadRegStr $3 HKCU "Software\\Classes\\FlashCardStudio.Project\\shell\\open\\command" ""
]=])
    #endregion
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS [=[
DeleteRegKey HKCU "Software\\Classes\\FlashCardStudio.Project"
DeleteRegKey HKCU "Software\\Classes\\.flcr"
]=])
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_VERBATIM_VARIABLES TRUE)
include(CPack)
