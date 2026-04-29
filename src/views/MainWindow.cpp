#include "MainWindow.h"

#include "DeckListWidget.h"
#include "EditWidget.h"
#include "LearnWidget.h"

#include "../models/ProjectManager.h"
#include "../models/Project.h"
#include "../utils/ThemeManager.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>
#include <QIcon>
#include <QTextEdit>

// ── Construction ─────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/app_icon.svg"));
    resize(1100, 720);

    createMenus();
    createDockWidget();
    createCentralStack();
    connectSignals();

    updateTitleBar();
}

// ── UI setup ─────────────────────────────────────────────────────────────────

void MainWindow::createMenus()
{
    // ── File ──────────────────────────────────────────────────────────────
    m_newAction = new QAction(
        QIcon(":/icons/note_add.svg"), tr("Новый проект…"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setToolTip(tr("Создать новый проект"));

    m_openAction = new QAction(
        QIcon(":/icons/folder_open.svg"), tr("Открыть…"), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setToolTip(tr("Открыть проект (.flcr)"));

    m_saveAction = new QAction(
        QIcon(":/icons/save.svg"), tr("Сохранить"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setToolTip(tr("Сохранить проект"));

    m_saveAsAction = new QAction(
        QIcon(":/icons/save_as.svg"), tr("Сохранить как…"), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setToolTip(tr("Сохранить проект в другом месте"));

    m_closeAction = new QAction(
        QIcon(":/icons/close.svg"), tr("Закрыть проект"), this);
    m_closeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
    m_closeAction->setToolTip(tr("Закрыть текущий проект"));

    auto* fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_closeAction);

    // ── Edit ──────────────────────────────────────────────────────────────
    m_undoAction = new QAction(
        QIcon(":/icons/undo.svg"), tr("Отменить"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);

    m_redoAction = new QAction(
        QIcon(":/icons/redo.svg"), tr("Повторить"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);

    auto* editMenu = menuBar()->addMenu(tr("Правка"));
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);

    // ── Help ──────────────────────────────────────────────────────────────
    m_aboutAction = new QAction(
        QIcon(":/icons/info.svg"), tr("О программе…"), this);

    auto* helpMenu = menuBar()->addMenu(tr("Справка"));
    helpMenu->addAction(m_aboutAction);

    // ── View (theme) ──────────────────────────────────────────────────────
    m_lightThemeAction = new QAction(tr("Светлая тема"), this);
    m_lightThemeAction->setCheckable(true);

    m_darkThemeAction = new QAction(tr("Тёмная тема"), this);
    m_darkThemeAction->setCheckable(true);

    auto* themeGroup = new QActionGroup(this);
    themeGroup->addAction(m_lightThemeAction);
    themeGroup->addAction(m_darkThemeAction);
    themeGroup->setExclusive(true);

    const bool isDark = (ThemeManager::instance().currentTheme() == AppTheme::Dark);
    m_lightThemeAction->setChecked(!isDark);
    m_darkThemeAction->setChecked(isDark);

    connect(m_lightThemeAction, &QAction::triggered, this, [this]() {
        ThemeManager::instance().setTheme(AppTheme::Light);
    });
    connect(m_darkThemeAction, &QAction::triggered, this, [this]() {
        ThemeManager::instance().setTheme(AppTheme::Dark);
    });

    auto* viewMenu = menuBar()->addMenu(tr("Вид"));
    viewMenu->addAction(m_lightThemeAction);
    viewMenu->addAction(m_darkThemeAction);
}

void MainWindow::createDockWidget()
{
    m_deckList = new DeckListWidget(this);

    m_deckDock = new QDockWidget(tr("Колоды"), this);
    m_deckDock->setObjectName("DeckDock");
    m_deckDock->setWidget(m_deckList);
    m_deckDock->setFeatures(QDockWidget::DockWidgetMovable |
                            QDockWidget::DockWidgetClosable);
    m_deckDock->setMinimumWidth(200);
    addDockWidget(Qt::LeftDockWidgetArea, m_deckDock);
}

void MainWindow::createCentralStack()
{
    // Placeholder page
    m_placeholderPage = new QWidget(this);
    m_placeholderLabel = new QLabel(tr("Выберите колоду из панели слева"), m_placeholderPage);
    m_placeholderLabel->setAlignment(Qt::AlignCenter);
    const QColor muted = ThemeManager::instance().mutedTextColor();
    m_placeholderLabel->setStyleSheet(
        QString("color: %1; font-size: 14px;").arg(muted.name()));
    auto* phLayout = new QVBoxLayout(m_placeholderPage);
    phLayout->addWidget(m_placeholderLabel);

    m_editWidget  = new EditWidget(this);
    m_learnWidget = new LearnWidget(this);

    m_stack = new QStackedWidget(this);
    m_stack->addWidget(m_placeholderPage); // PagePlaceholder = 0
    m_stack->addWidget(m_editWidget);      // PageEdit        = 1
    m_stack->addWidget(m_learnWidget);     // PageLearn       = 2

    setCentralWidget(m_stack);
}

void MainWindow::connectSignals()
{
    // File actions
    connect(m_newAction,    &QAction::triggered, this, &MainWindow::onNewProject);
    connect(m_openAction,   &QAction::triggered, this, &MainWindow::onOpenProject);
    connect(m_saveAction,   &QAction::triggered, this, &MainWindow::onSaveProject);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::onSaveProjectAs);
    connect(m_closeAction,  &QAction::triggered, this, &MainWindow::onCloseProject);

    // Edit actions
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    // About
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    // Deck list signals
    connect(m_deckList, &DeckListWidget::editDeckRequested,
            this, &MainWindow::onEditDeckRequested);
    connect(m_deckList, &DeckListWidget::learnDeckRequested,
            this, &MainWindow::onLearnDeckRequested);

    // ProjectManager
    auto& pm = ProjectManager::instance();
    connect(&pm, &ProjectManager::projectLoaded,
            this, &MainWindow::onProjectLoaded);
    connect(&pm, &ProjectManager::projectClosed,
            this, &MainWindow::onProjectClosed);
    connect(&pm, &ProjectManager::projectModified,
            this, &MainWindow::onProjectModified);
    connect(&pm, &ProjectManager::deckListChanged,
            m_deckList, &DeckListWidget::refresh);
    connect(&pm, &ProjectManager::deckListChanged,
            this, [this]() {
                // If editing a deck that was removed, go back to placeholder
                if (m_stack->currentIndex() == PageEdit) {
                    auto deck = m_editWidget->currentDeck();
                    if (deck && !ProjectManager::instance().project()->findDeck(deck->deckId))
                        m_stack->setCurrentIndex(PagePlaceholder);
                }
            });

    // Learn finished
    connect(m_learnWidget, &LearnWidget::sessionFinished,
            this, &MainWindow::onLearnSessionFinished);
}

// ── Private helpers ───────────────────────────────────────────────────────────

void MainWindow::updateTitleBar()
{
    auto& pm = ProjectManager::instance();
    if (!pm.project()) {
        setWindowTitle(tr("FlashCard Studio"));
        return;
    }
    const QString name     = pm.project()->projectName;
    const QString modified = pm.isModified() ? " *" : "";
    setWindowTitle(QString("%1%2 — FlashCard Studio").arg(name, modified));
}

bool MainWindow::maybeSaveProject()
{
    auto& pm = ProjectManager::instance();
    if (!pm.project() || !pm.isModified())
        return true;

    const auto reply = QMessageBox::question(
        this,
        tr("Несохранённые изменения"),
        tr("Сохранить изменения в «%1»?").arg(pm.project()->projectName),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (reply == QMessageBox::Cancel)  return false;
    if (reply == QMessageBox::Discard) return true;
    return pm.saveProject();
}

void MainWindow::addToRecentFiles(const QString& filePath,
                                  const QString& projectName)
{
    QSettings settings;
    QStringList list = settings.value("recentFiles").toStringList();
    const QString entry = filePath + "::" + projectName;

    list.removeAll(entry);
    list.prepend(entry);

    // Keep only the 10 most recent
    while (list.size() > 10)
        list.removeLast();

    settings.setValue("recentFiles", list);
}

void MainWindow::applyCurrentTheme()
{
    // Update check state of theme menu items
    const bool isDark = (ThemeManager::instance().currentTheme() == AppTheme::Dark);
    m_lightThemeAction->setChecked(!isDark);
    m_darkThemeAction->setChecked(isDark);

    // Update placeholder label colour (it uses an inline stylesheet)
    const QColor muted = ThemeManager::instance().mutedTextColor();
    m_placeholderLabel->setStyleSheet(
        QString("color: %1; font-size: 14px;").arg(muted.name()));

    // Propagate to the learn widget (which has its own inline styles)
    m_learnWidget->applyCurrentTheme();
}

// ── Public ────────────────────────────────────────────────────────────────────

void MainWindow::showEditDeck(const QString& deckId)
{
    auto deck = ProjectManager::instance().project()
                    ? ProjectManager::instance().project()->findDeck(deckId)
                    : nullptr;
    if (!deck) return;

    m_editWidget->setDeck(deck);
    m_stack->setCurrentIndex(PageEdit);
}

void MainWindow::showLearnDeck(const QString& deckId)
{
    auto deck = ProjectManager::instance().project()
                    ? ProjectManager::instance().project()->findDeck(deckId)
                    : nullptr;
    if (!deck) return;

    m_learnWidget->startSession(deck);
    m_stack->setCurrentIndex(PageLearn);
}

// ── closeEvent ────────────────────────────────────────────────────────────────

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (maybeSaveProject()) {
        ProjectManager::instance().closeProject();
        event->accept();
    } else {
        event->ignore();
    }
}

// ── File slots ────────────────────────────────────────────────────────────────

void MainWindow::onNewProject()
{
    if (!maybeSaveProject()) return;

    const QString name = QInputDialog::getText(
        this, tr("Новый проект"), tr("Имя проекта:"));
    if (name.trimmed().isEmpty()) return;

    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Место сохранения"),
        name.trimmed() + ".flcr",
        tr("FlashCard Studio (*.flcr)"));
    if (filePath.isEmpty()) return;

    auto& pm = ProjectManager::instance();
    if (!pm.newProject(filePath, name.trimmed())) {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось создать проект."));
        return;
    }

    addToRecentFiles(filePath, name.trimmed());
}

void MainWindow::onOpenProject()
{
    if (!maybeSaveProject()) return;

    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Открыть проект"),
        QString(),
        tr("FlashCard Studio (*.flcr);;Все файлы (*)"));
    if (filePath.isEmpty()) return;

    QString errorMsg;
    auto& pm = ProjectManager::instance();
    if (!pm.openProject(filePath)) {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось открыть файл: %1").arg(filePath));
        return;
    }

    if (pm.project())
        addToRecentFiles(filePath, pm.project()->projectName);
}

void MainWindow::onSaveProject()
{
    ProjectManager::instance().saveProject();
}

void MainWindow::onSaveProjectAs()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Сохранить как"),
        QString(),
        tr("FlashCard Studio (*.flcr)"));
    if (filePath.isEmpty()) return;

    auto& pm = ProjectManager::instance();
    if (!pm.saveProjectAs(filePath)) {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось сохранить файл."));
        return;
    }
    if (pm.project())
        addToRecentFiles(filePath, pm.project()->projectName);
}

void MainWindow::onCloseProject()
{
    if (!maybeSaveProject()) return;
    ProjectManager::instance().closeProject();
}

// ── Edit slots ────────────────────────────────────────────────────────────────

void MainWindow::onUndo()
{
    // Delegate to the focused QTextEdit's undo stack if in edit mode
    if (m_stack->currentIndex() == PageEdit) {
        QWidget* fw = focusWidget();
        if (auto* te = qobject_cast<QTextEdit*>(fw))
            te->undo();
    }
}

void MainWindow::onRedo()
{
    if (m_stack->currentIndex() == PageEdit) {
        QWidget* fw = focusWidget();
        if (auto* te = qobject_cast<QTextEdit*>(fw))
            te->redo();
    }
}

// ── ProjectManager slots ──────────────────────────────────────────────────────

void MainWindow::onProjectLoaded(const QString& /*filePath*/)
{
    m_deckList->refresh();
    m_stack->setCurrentIndex(PagePlaceholder);
    m_deckDock->show();
    updateTitleBar();
}

void MainWindow::onProjectClosed()
{
    m_stack->setCurrentIndex(PagePlaceholder);
    m_deckDock->hide();
    updateTitleBar();
    // Navigation back to the StartScreen is handled by main.cpp
    // which connects ProjectManager::projectClosed to showStartScreen().
    hide();
}

void MainWindow::onProjectModified()
{
    updateTitleBar();
}

// ── DeckList slots ────────────────────────────────────────────────────────────

void MainWindow::onEditDeckRequested(const QString& deckId)
{
    showEditDeck(deckId);
}

void MainWindow::onLearnDeckRequested(const QString& deckId)
{
    showLearnDeck(deckId);
}

void MainWindow::onLearnSessionFinished()
{
    // Return to edit view of the same deck after the session
    if (m_learnWidget) {
        // Just fall back to placeholder; user can pick another action
        m_stack->setCurrentIndex(PagePlaceholder);
    }
}

// ── About slot ────────────────────────────────────────────────────────────────

void MainWindow::onAbout()
{
    QMessageBox::about(
        this,
        tr("О FlashCard Studio"),
        tr("<h3>FlashCard Studio</h3>"
           "<p>Приложение для интервального повторения карточек (SM-2).</p>"
           "<p>Построено на Qt 6 / C++17.</p>"));
}
