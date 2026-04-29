#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

// Initial "backstage" screen shown before any project is open.
// Left panel: accent-coloured branding + action buttons.
// Right panel: recent files list (from QSettings).
class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget* parent = nullptr);

    // Reload the recent-files list from QSettings (call after a project closes)
    void refreshRecentFiles();

public slots:
    void applyCurrentTheme();

signals:
    void newProjectRequested();
    void openProjectRequested();
    void openRecentRequested(const QString& filePath);

private slots:
    void onNewClicked();
    void onOpenClicked();
    void onRecentItemDoubleClicked(QListWidgetItem* item);

private:
    void setupUi();
    void connectSignals();

    QPushButton* makeActionButton(const QIcon& icon, const QString& text);

    QListWidget* m_recentList{nullptr};
    QLabel*      m_noRecentLabel{nullptr};
};
