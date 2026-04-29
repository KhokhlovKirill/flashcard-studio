#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QLabel>
#include <QAction>
#include <QActionGroup>
#include <memory>

class DeckListWidget;
class EditWidget;
class LearnWidget;
class Deck;

// Application's main editing window.
// Shown after a project is created/opened from StartScreen.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    // Show the Edit view for a specific deck
    void showEditDeck(const QString& deckId);

    // Show the Learn (spaced-repetition) session for a specific deck
    void showLearnDeck(const QString& deckId);

protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
    // File menu (also invoked from main.cpp via QMetaObject::invokeMethod)
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onCloseProject();

    // Edit menu
    void onUndo();
    void onRedo();

    // ProjectManager signals
    void onProjectLoaded(const QString& filePath);
    void onProjectClosed();
    void onProjectModified();

    // DeckList requests
    void onEditDeckRequested(const QString& deckId);
    void onLearnDeckRequested(const QString& deckId);

    // Learn finished
    void onLearnSessionFinished();

    // About
    void onAbout();

    // Theme
    void applyCurrentTheme();

private:
    void createMenus();
    void createDockWidget();
    void createCentralStack();
    void connectSignals();

    void updateTitleBar();
    bool maybeSaveProject();   // returns false if user cancels
    void addToRecentFiles(const QString& filePath, const QString& projectName);

    // ── Widgets ───────────────────────────────────────────────────────────
    QDockWidget*     m_deckDock{nullptr};
    DeckListWidget*  m_deckList{nullptr};

    QStackedWidget*  m_stack{nullptr};
    QWidget*         m_placeholderPage{nullptr};
    QLabel*          m_placeholderLabel{nullptr};
    EditWidget*      m_editWidget{nullptr};
    LearnWidget*     m_learnWidget{nullptr};

    enum StackPage { PagePlaceholder = 0, PageEdit = 1, PageLearn = 2 };

    // ── Actions ───────────────────────────────────────────────────────────
    QAction* m_newAction{nullptr};
    QAction* m_openAction{nullptr};
    QAction* m_saveAction{nullptr};
    QAction* m_saveAsAction{nullptr};
    QAction* m_closeAction{nullptr};
    QAction* m_undoAction{nullptr};
    QAction* m_redoAction{nullptr};
    QAction* m_lightThemeAction{nullptr};
    QAction* m_darkThemeAction{nullptr};
    QAction* m_aboutAction{nullptr};
};
