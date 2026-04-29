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
    set_target_properties(FlashCardStudio PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(APPLE)
    set_target_properties(FlashCardStudio PROPERTIES MACOSX_BUNDLE TRUE)
endif()
