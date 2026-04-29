#include "StartScreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QFileInfo>
#include <QIcon>
#include <QFont>
#include <QFrame>
#include <QListWidgetItem>
#include <QSplitter>
#include <QPalette>
#include <QApplication>

#include "../utils/ThemeManager.h"

// ─ Key used in QSettings ─────────────────────────────────────────────────────
static const char* kRecentKey = "recentFiles";

// ── Construction ─────────────────────────────────────────────────────────────

StartScreen::StartScreen(QWidget* parent)
    : QWidget(parent)
{
    setWindowIcon(QIcon(":/icons/app_icon.svg"));
    setupUi();
    connectSignals();
    refreshRecentFiles();
}

void StartScreen::setupUi()
{
    // ═══════════════════════════════════════════
    //  LEFT PANEL — branding + action buttons
    // ═══════════════════════════════════════════
    auto* leftPanel = new QWidget(this);
    leftPanel->setObjectName("startLeft");
    leftPanel->setStyleSheet(
        "#startLeft { background-color: #1976D2; }");
    leftPanel->setMinimumWidth(260);
    leftPanel->setMaximumWidth(340);

    // App icon
    auto* appIconLabel = new QLabel(leftPanel);
    appIconLabel->setPixmap(
        QIcon(":/icons/app_icon.svg").pixmap(64, 64));
    appIconLabel->setAlignment(Qt::AlignCenter);

    // App name
    auto* appNameLabel = new QLabel(tr("FlashCard Studio"), leftPanel);
    QFont nf;
    nf.setPointSize(18);
    nf.setBold(true);
    appNameLabel->setFont(nf);
    appNameLabel->setAlignment(Qt::AlignCenter);
    appNameLabel->setStyleSheet("color: white;");

    // Buttons
    auto* newBtn  = makeActionButton(
        QIcon(":/icons/note_add.svg"), tr("  Новый проект"));
    auto* openBtn = makeActionButton(
        QIcon(":/icons/folder_open.svg"), tr("  Открыть проект"));

    connect(newBtn,  &QPushButton::clicked, this, &StartScreen::onNewClicked);
    connect(openBtn, &QPushButton::clicked, this, &StartScreen::onOpenClicked);

    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(24, 40, 24, 40);
    leftLayout->setSpacing(12);
    leftLayout->addStretch();
    leftLayout->addWidget(appIconLabel);
    leftLayout->addSpacing(8);
    leftLayout->addWidget(appNameLabel);
    leftLayout->addSpacing(32);
    leftLayout->addWidget(newBtn);
    leftLayout->addWidget(openBtn);
    leftLayout->addStretch();

    // ═══════════════════════════════════════════
    //  RIGHT PANEL — recent files
    // ═══════════════════════════════════════════
    auto* rightPanel = new QWidget(this);

    auto* recentTitle = new QLabel(tr("Последние проекты"), rightPanel);
    QFont tf;
    tf.setPointSize(13);
    tf.setBold(true);
    recentTitle->setFont(tf);

    m_recentList = new QListWidget(rightPanel);
    m_recentList->setFrameShape(QFrame::NoFrame);
    m_recentList->setAlternatingRowColors(true);
    m_recentList->setIconSize(QSize(24, 24));

    m_noRecentLabel = new QLabel(tr("Нет последних проектов"), rightPanel);
    m_noRecentLabel->setAlignment(Qt::AlignCenter);
    const QColor muted = ThemeManager::instance().mutedTextColor();
    m_noRecentLabel->setStyleSheet(
        QString("color: %1; font-size: 12px;").arg(muted.name()));

    auto* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(24, 40, 24, 24);
    rightLayout->setSpacing(12);
    rightLayout->addWidget(recentTitle);
    rightLayout->addWidget(m_recentList, 1);
    rightLayout->addWidget(m_noRecentLabel);

    // ═══════════════════════════════════════════
    //  Splitter
    // ═══════════════════════════════════════════
    auto* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setHandleWidth(0);

    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(splitter);
}

void StartScreen::connectSignals()
{
    connect(m_recentList, &QListWidget::itemDoubleClicked,
            this, &StartScreen::onRecentItemDoubleClicked);
}

// ── Helpers ───────────────────────────────────────────────────────────────────

QPushButton* StartScreen::makeActionButton(const QIcon& icon, const QString& text)
{
    auto* btn = new QPushButton(icon, text);
    btn->setIconSize(QSize(18, 18));
    btn->setMinimumHeight(40);
    btn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.15); color: white; "
        "border: 1px solid rgba(255,255,255,0.35); border-radius: 8px; "
        "font-size: 12px; font-weight: bold; padding: 6px 16px; text-align: left; }"
        "QPushButton:hover { background: rgba(255,255,255,0.28); }"
        "QPushButton:pressed { background: rgba(255,255,255,0.10); }");
    return btn;
}

// ── Theme ─────────────────────────────────────────────────────────────────────

void StartScreen::applyCurrentTheme()
{
    const QColor muted = ThemeManager::instance().mutedTextColor();
    m_noRecentLabel->setStyleSheet(
        QString("color: %1; font-size: 12px;").arg(muted.name()));
}

// ── Public ────────────────────────────────────────────────────────────────────

void StartScreen::refreshRecentFiles()
{
    m_recentList->clear();

    QSettings settings;
    const QStringList recent = settings.value(kRecentKey).toStringList();

    for (const QString& entry : recent) {
        // Format: "filePath::projectName"
        const int sep = entry.lastIndexOf("::");
        if (sep < 0) continue;

        const QString filePath    = entry.left(sep);
        const QString projectName = entry.mid(sep + 2);

        auto* item = new QListWidgetItem(
            QIcon(":/icons/deck.svg"),
            QString("%1\n%2").arg(projectName, filePath));
        item->setData(Qt::UserRole, filePath);
        item->setSizeHint(QSize(0, 52));
        item->setToolTip(filePath);
        m_recentList->addItem(item);
    }

    const bool hasRecent = (m_recentList->count() > 0);
    m_recentList->setVisible(hasRecent);
    m_noRecentLabel->setVisible(!hasRecent);
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void StartScreen::onNewClicked()
{
    emit newProjectRequested();
}

void StartScreen::onOpenClicked()
{
    emit openProjectRequested();
}

void StartScreen::onRecentItemDoubleClicked(QListWidgetItem* item)
{
    if (!item) return;
    const QString filePath = item->data(Qt::UserRole).toString();
    if (!filePath.isEmpty())
        emit openRecentRequested(filePath);
}
