#include "LearnWidget.h"

#include "../models/SpacedRepetitionLogic.h"
#include "../models/ProjectManager.h"
#include "../utils/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QKeySequence>
#include <QShortcut>
#include <QFont>
#include <QFontInfo>
#include <QIcon>
#include <QSizePolicy>

// ── Rating button helper ─────────────────────────────────────────────────────

static QPushButton* makeRatingButton(const QString& text,
                                     const QString& colorHex,
                                     QWidget* parent)
{
    auto* btn = new QPushButton(text, parent);
    btn->setMinimumWidth(80);
    btn->setMinimumHeight(36);
    btn->setStyleSheet(
        QString("QPushButton { background-color: %1; color: white; border: none; "
                "border-radius: 6px; font-weight: bold; padding: 4px 8px; }"
                "QPushButton:hover { background-color: %1; opacity: 0.85; }"
                "QPushButton:pressed { padding: 5px 7px 3px 9px; }")
        .arg(colorHex));
    return btn;
}

static int scaledCardPointSize(const QFont& font)
{
    int basePointSize = font.pointSize();
    if (basePointSize <= 0)
        basePointSize = QFontInfo(font).pointSize();
    if (basePointSize <= 0)
        basePointSize = 12;
    return basePointSize * 3;
}

// ── Construction ─────────────────────────────────────────────────────────────

LearnWidget::LearnWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();
}

void LearnWidget::setupUi()
{
    // ── Progress area ─────────────────────────────────────────────────────
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(6);

    m_counterLabel = new QLabel("0 / 0", this);
    m_counterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QFont cf = m_counterLabel->font();
    cf.setPointSize(cf.pointSize() - 1);
    m_counterLabel->setFont(cf);

    auto* topRow = new QHBoxLayout;
    topRow->addWidget(m_progressBar, 1);
    topRow->addWidget(m_counterLabel);

    // ── Card frame ────────────────────────────────────────────────────────
    m_cardFrame = new QFrame(this);  // QFrame, not QWidget
    m_cardFrame->setFrameShape(QFrame::StyledPanel);
    m_cardFrame->setObjectName("cardFrame");
    {
        const auto& tm = ThemeManager::instance();
        m_cardFrame->setStyleSheet(
            QString("#cardFrame { background: %1; border-radius: 12px; "
                    "border: 1px solid %2; }")
            .arg(tm.cardBackground().name(), tm.cardBorder().name()));
    }

    auto* shadow = new QGraphicsDropShadowEffect(m_cardFrame);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 40));
    m_cardFrame->setGraphicsEffect(shadow);

    m_cardFront = new QLabel(m_cardFrame);
    m_cardFront->setWordWrap(true);
    m_cardFront->setAlignment(Qt::AlignCenter);
    m_cardFront->setStyleSheet("background: transparent;");
    m_cardFront->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QFont frontFont = m_cardFront->font();
    frontFont.setPointSize(scaledCardPointSize(frontFont));
    m_cardFront->setFont(frontFont);

    m_separatorLabel = new QLabel("─────────", m_cardFrame);
    m_separatorLabel->setAlignment(Qt::AlignCenter);
    m_separatorLabel->setStyleSheet(
        QString("color: %1;")
        .arg(ThemeManager::instance().cardBorder().name()));
    m_separatorLabel->hide();

    m_cardBack = new QLabel(m_cardFrame);
    m_cardBack->setWordWrap(true);
    m_cardBack->setAlignment(Qt::AlignCenter);
    m_cardBack->setStyleSheet("background: transparent;");
    m_cardBack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QFont backFont = m_cardBack->font();
    backFont.setPointSize(scaledCardPointSize(backFont));
    m_cardBack->setFont(backFont);
    m_cardBack->hide();

    auto* cardLayout = new QVBoxLayout(m_cardFrame);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(8);
    cardLayout->addStretch();
    // Stretch factor 0: widgets keep their natural size; stretch items above/below center them.
    // Horizontal centering via Qt::AlignHCenter; QLabel uses Qt::AlignCenter for text.
    cardLayout->addWidget(m_cardFront,       0, Qt::AlignHCenter);
    cardLayout->addWidget(m_separatorLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_cardBack,        0, Qt::AlignHCenter);
    cardLayout->addStretch();

    // ── Control: question page ────────────────────────────────────────────
    m_showAnswerBtn = new QPushButton(
        QIcon(":/icons/visibility.svg"), tr("  Показать ответ"), this);
    m_showAnswerBtn->setToolTip(tr("Показать ответ (Пробел)"));
    m_showAnswerBtn->setMinimumHeight(40);
    m_showAnswerBtn->setIconSize(QSize(18, 18));
    {
        const QString hoverBg =
            ThemeManager::instance().showAnswerHoverBackground().name(QColor::HexArgb);
        m_showAnswerBtn->setStyleSheet(
            "QPushButton { border: 2px solid #1976D2; border-radius: 8px; "
            "color: #1976D2; font-weight: bold; padding: 4px 24px; }"
            "QPushButton:hover { background: " + hoverBg + "; }");
    }

    auto* questionLayout = new QHBoxLayout;
    questionLayout->setContentsMargins(0, 8, 0, 8);
    questionLayout->addStretch();
    questionLayout->addWidget(m_showAnswerBtn);
    questionLayout->addStretch();

    auto* questionPage = new QWidget(this);
    questionPage->setLayout(questionLayout);

    // ── Control: answer page ──────────────────────────────────────────────
    m_againBtn  = makeRatingButton(tr("Снова"),   "#EF5350", this); // red
    m_hardBtn   = makeRatingButton(tr("Трудно"),  "#FF9800", this); // orange
    m_goodBtn   = makeRatingButton(tr("Хорошо"),  "#1976D2", this); // blue
    m_easyBtn   = makeRatingButton(tr("Легко"),   "#43A047", this); // green

    m_againBtn->setToolTip(tr("Карточка не запомнена (Пробел = Хорошо)"));
    m_hardBtn->setToolTip(tr("Было трудно"));
    m_goodBtn->setToolTip(tr("Всё верно (Пробел)"));
    m_easyBtn->setToolTip(tr("Запомнилось легко"));

    m_againLabel = new QLabel("< 1 д", this);
    m_hardLabel  = new QLabel("", this);
    m_goodLabel  = new QLabel("", this);
    m_easyLabel  = new QLabel("", this);

    for (QLabel* l : {m_againLabel, m_hardLabel, m_goodLabel, m_easyLabel}) {
        QFont lf = l->font();
        lf.setPointSize(lf.pointSize() - 2);
        l->setFont(lf);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(
            QString("color: %1;")
            .arg(ThemeManager::instance().mutedTextColor().name()));
    }

    auto makeButtonColumn = [](QPushButton* btn, QLabel* lbl) -> QWidget* {
        auto* w = new QWidget;
        auto* v = new QVBoxLayout(w);
        v->setContentsMargins(0, 0, 0, 0);
        v->setSpacing(2);
        v->addWidget(btn);
        v->addWidget(lbl, 0, Qt::AlignCenter);
        return w;
    };

    auto* answerBtnsLayout = new QHBoxLayout;
    answerBtnsLayout->setContentsMargins(0, 8, 0, 8);
    answerBtnsLayout->setSpacing(8);
    answerBtnsLayout->addStretch();
    answerBtnsLayout->addWidget(makeButtonColumn(m_againBtn, m_againLabel));
    answerBtnsLayout->addWidget(makeButtonColumn(m_hardBtn,  m_hardLabel));
    answerBtnsLayout->addWidget(makeButtonColumn(m_goodBtn,  m_goodLabel));
    answerBtnsLayout->addWidget(makeButtonColumn(m_easyBtn,  m_easyLabel));
    answerBtnsLayout->addStretch();

    auto* answerPage = new QWidget(this);
    answerPage->setLayout(answerBtnsLayout);

    // ── Control: finished page ────────────────────────────────────────────
    auto* finishedLabel = new QLabel(tr("🎉  На сегодня всё!"), this);
    QFont fl = finishedLabel->font();
    fl.setPointSize(fl.pointSize() + 4);
    fl.setBold(true);
    finishedLabel->setFont(fl);
    finishedLabel->setAlignment(Qt::AlignCenter);

    auto* finishedLayout = new QHBoxLayout;
    finishedLayout->addStretch();
    finishedLayout->addWidget(finishedLabel);
    finishedLayout->addStretch();

    auto* finishedPage = new QWidget(this);
    finishedPage->setLayout(finishedLayout);

    // ── Stack the control pages ───────────────────────────────────────────
    m_controlStack = new QStackedWidget(this);
    m_controlStack->addWidget(questionPage); // PageQuestion = 0
    m_controlStack->addWidget(answerPage);   // PageAnswer   = 1
    m_controlStack->addWidget(finishedPage); // PageFinished = 2

    // ── Main layout ───────────────────────────────────────────────────────
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 8, 16, 8);
    mainLayout->setSpacing(8);
    mainLayout->addLayout(topRow);
    mainLayout->addWidget(m_cardFrame, 1);
    mainLayout->addWidget(m_controlStack);
}

void LearnWidget::connectSignals()
{
    connect(m_showAnswerBtn, &QPushButton::clicked, this, &LearnWidget::onShowAnswer);
    connect(m_againBtn,      &QPushButton::clicked, this, &LearnWidget::onAgain);
    connect(m_hardBtn,       &QPushButton::clicked, this, &LearnWidget::onHard);
    connect(m_goodBtn,       &QPushButton::clicked, this, &LearnWidget::onGood);
    connect(m_easyBtn,       &QPushButton::clicked, this, &LearnWidget::onEasy);

    // Space: show answer or press "Good"
    auto* spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    spaceShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(spaceShortcut, &QShortcut::activated, this, [this]() {
        if (m_controlStack->currentIndex() == PageQuestion) {
            onShowAnswer();
        } else if (m_controlStack->currentIndex() == PageAnswer) {
            onGood();
        }
    });
}

// ── applyCurrentTheme ────────────────────────────────────────────────────────

void LearnWidget::applyCurrentTheme()
{
    const auto& tm = ThemeManager::instance();

    m_cardFrame->setStyleSheet(
        QString("#cardFrame { background: %1; border-radius: 12px; "
                "border: 1px solid %2; }")
        .arg(tm.cardBackground().name(), tm.cardBorder().name()));

    m_separatorLabel->setStyleSheet(
        QString("color: %1;").arg(tm.cardBorder().name()));

    const QString hoverBg =
        tm.showAnswerHoverBackground().name(QColor::HexArgb);
    m_showAnswerBtn->setStyleSheet(
        "QPushButton { border: 2px solid #1976D2; border-radius: 8px; "
        "color: #1976D2; font-weight: bold; padding: 4px 24px; }"
        "QPushButton:hover { background: " + hoverBg + "; }");

    const QString mutedColor = tm.mutedTextColor().name();
    for (QLabel* l : {m_againLabel, m_hardLabel, m_goodLabel, m_easyLabel})
        l->setStyleSheet(QString("color: %1;").arg(mutedColor));
}

// ── Public ───────────────────────────────────────────────────────────────────

void LearnWidget::startSession(std::shared_ptr<Deck> deck)
{
    m_deck   = std::move(deck);
    m_queue  = SpacedRepetitionLogic::dueCards(*m_deck);
    m_totalCards = m_queue.size();

    updateProgress();

    if (m_queue.isEmpty()) {
        showFinishedScreen();
    } else {
        showCurrentCard();
    }
}

// ── Private helpers ───────────────────────────────────────────────────────────

void LearnWidget::showCurrentCard()
{
    m_answerVisible = false;
    m_cardBack->hide();
    m_separatorLabel->hide();

    if (m_queue.isEmpty()) {
        showFinishedScreen();
        return;
    }

    const auto& card = m_queue.first();
    const QString frontText = card->front.isEmpty()
                               ? tr("<i>(Пустая карточка)</i>")
                               : card->front;
    m_cardFront->setText(frontText);
    m_cardBack->setText(card->back);

    m_controlStack->setCurrentIndex(PageQuestion);
    updateIntervalLabels();
}

void LearnWidget::advanceToNext()
{
    updateProgress();
    showCurrentCard();
}

void LearnWidget::showFinishedScreen()
{
    m_cardFront->setText(QString());
    m_cardBack->hide();
    m_separatorLabel->hide();
    m_controlStack->setCurrentIndex(PageFinished);
    emit sessionFinished();
}

void LearnWidget::updateProgress()
{
    const int done = m_totalCards - m_queue.size();
    const int pct  = (m_totalCards > 0)
                     ? (done * 100) / m_totalCards
                     : 100;
    m_progressBar->setValue(pct);
    m_counterLabel->setText(QString("%1 / %2").arg(done).arg(m_totalCards));
}

static QString intervalText(int days)
{
    if (days <= 0)  return "<1 д";
    if (days == 1)  return "1 д";
    if (days < 7)   return QString("%1 д").arg(days);
    if (days < 30)  return QString("%1 нед").arg(days / 7);
    if (days < 365) return QString("%1 мес").arg(days / 30);
    return QString("%1 г").arg(days / 365);
}

void LearnWidget::updateIntervalLabels()
{
    if (m_queue.isEmpty()) return;
    const Card& c = *m_queue.first();

    m_againLabel->setText(intervalText(SpacedRepetitionLogic::previewIntervalAgain(c)));
    m_hardLabel ->setText(intervalText(SpacedRepetitionLogic::previewIntervalHard(c)));
    m_goodLabel ->setText(intervalText(SpacedRepetitionLogic::previewIntervalGood(c)));
    m_easyLabel ->setText(intervalText(SpacedRepetitionLogic::previewIntervalEasy(c)));
}

// ── Button slots ─────────────────────────────────────────────────────────────

void LearnWidget::onShowAnswer()
{
    m_answerVisible = true;
    m_cardBack->show();
    m_separatorLabel->show();
    m_controlStack->setCurrentIndex(PageAnswer);
    updateIntervalLabels();
}

void LearnWidget::onAgain()
{
    if (m_queue.isEmpty()) return;
    auto card = m_queue.takeFirst();
    SpacedRepetitionLogic::applyAgain(*card);
    m_queue.append(card);              // goes to the end of today's queue
    ProjectManager::instance().markModified();
    advanceToNext();
}

void LearnWidget::onHard()
{
    if (m_queue.isEmpty()) return;
    auto card = m_queue.takeFirst();
    SpacedRepetitionLogic::applyHard(*card);
    ProjectManager::instance().markModified();
    advanceToNext();
}

void LearnWidget::onGood()
{
    if (m_queue.isEmpty()) return;
    auto card = m_queue.takeFirst();
    SpacedRepetitionLogic::applyGood(*card);
    ProjectManager::instance().markModified();
    advanceToNext();
}

void LearnWidget::onEasy()
{
    if (m_queue.isEmpty()) return;
    auto card = m_queue.takeFirst();
    SpacedRepetitionLogic::applyEasy(*card);
    ProjectManager::instance().markModified();
    advanceToNext();
}
