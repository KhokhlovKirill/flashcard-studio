#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QMessageBox>

#include "views/StartScreen.h"
#include "views/MainWindow.h"
#include "models/ProjectManager.h"
#include "utils/ThemeManager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Application meta-info (used by QSettings)
    QApplication::setApplicationName("FlashCardStudio");
    QApplication::setOrganizationName("FlashCardStudio");
    QApplication::setApplicationVersion("1.0");

    // Apply the persisted theme (Fusion style + palette) before any window
    // is created so that all widget constructors already see the right colours.
    ThemeManager::instance().applyCurrentTheme();

    // ── Widgets ───────────────────────────────────────────────────────────
    // Both are created up front; we show/hide them as needed.
    StartScreen startScreen;
    MainWindow  mainWindow;

    // ── Theme change: re-apply inline styles in all top-level windows ─────
    QObject::connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
                     &startScreen, &StartScreen::applyCurrentTheme);
    QObject::connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
                     &mainWindow, &MainWindow::applyCurrentTheme);

    // ── Open-from-command-line support ────────────────────────────────────
    if (argc >= 2) {
        const QString path = QString::fromLocal8Bit(argv[1]);
        if (QFileInfo::exists(path)) {
            auto& pm = ProjectManager::instance();
            if (pm.openProject(path)) {
                mainWindow.show();
                return app.exec();
            }
            QMessageBox::warning(nullptr,
                QObject::tr("Ошибка"),
                QObject::tr("Не удалось открыть файл:\n%1").arg(path));
        }
    }

    // ── StartScreen → MainWindow ──────────────────────────────────────────

    // Helper: opens the main window after a project has been loaded
    auto showMainWindow = [&]() {
        startScreen.hide();
        mainWindow.show();
        mainWindow.raise();
        mainWindow.activateWindow();
    };

    // Helper: returns to start screen (called when the project is closed)
    auto showStartScreen = [&]() {
        mainWindow.hide();
        startScreen.refreshRecentFiles();
        startScreen.show();
        startScreen.raise();
        startScreen.activateWindow();
    };

    // ── StartScreen signals ───────────────────────────────────────────────

    QObject::connect(&startScreen, &StartScreen::newProjectRequested,
                     [&]() {
                         // Delegate to MainWindow's File > New action so
                         // the dialog + QSettings wiring lives in one place.
                         QMetaObject::invokeMethod(&mainWindow, "onNewProject",
                                                   Qt::DirectConnection);
                         if (ProjectManager::instance().project())
                             showMainWindow();
                     });

    QObject::connect(&startScreen, &StartScreen::openProjectRequested,
                     [&]() {
                         QMetaObject::invokeMethod(&mainWindow, "onOpenProject",
                                                   Qt::DirectConnection);
                         if (ProjectManager::instance().project())
                             showMainWindow();
                     });

    QObject::connect(&startScreen, &StartScreen::openRecentRequested,
                     [&](const QString& filePath) {
                         auto& pm = ProjectManager::instance();
                         if (!pm.openProject(filePath)) {
                             QMessageBox::warning(&startScreen,
                                 QObject::tr("Ошибка"),
                                 QObject::tr("Не удалось открыть файл:\n%1").arg(filePath));
                             return;
                         }
                         showMainWindow();
                     });

    // ── ProjectManager → start screen ─────────────────────────────────────
    // When the project is closed (including from MainWindow's close button),
    // return to the start screen.
    QObject::connect(&ProjectManager::instance(), &ProjectManager::projectClosed,
                     [&]() {
                         // MainWindow::closeEvent calls closeProject(), which
                         // emits this signal.  If the window is still visible,
                         // close it first (no-op if already hidden).
                         if (mainWindow.isVisible())
                             mainWindow.hide();
                         showStartScreen();
                     });

    // ── Show start screen ─────────────────────────────────────────────────
    startScreen.show();

    return app.exec();
}
