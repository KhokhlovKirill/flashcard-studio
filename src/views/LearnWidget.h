#pragma once

#include <QWidget>
#include <QFrame>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QList>
#include <memory>

#include "../models/Deck.h"
#include "../models/Card.h"

// Study (spaced repetition) view.
// Manages the session queue, shows front/back of each card, and applies SM-2
// ratings.  Emits sessionFinished() when the queue is exhausted.
class LearnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LearnWidget(QWidget* parent = nullptr);

    // Builds the due-card queue for the given deck and starts the session
    void startSession(std::shared_ptr<Deck> deck);

public slots:
    void applyCurrentTheme();

signals:
    void sessionFinished();

private slots:
    void onShowAnswer();
    void onAgain();
    void onHard();
    void onGood();
    void onEasy();

private:
    void setupUi();
    void connectSignals();

    void showCurrentCard();
    void advanceToNext();
    void showFinishedScreen();
    void updateProgress();
    void updateIntervalLabels();

    // ── Session state ─────────────────────────────────────────────────────
    std::shared_ptr<Deck>              m_deck;
    QList<std::shared_ptr<Card>>       m_queue;
    int                                m_totalCards{0};
    bool                               m_answerVisible{false};

    // ── Widgets ───────────────────────────────────────────────────────────
    QProgressBar*   m_progressBar{nullptr};
    QLabel*         m_counterLabel{nullptr};

    // Card display
    QFrame*         m_cardFrame{nullptr};
    QLabel*         m_cardFront{nullptr};
    QLabel*         m_cardBack{nullptr};
    QLabel*         m_separatorLabel{nullptr};

    // Control panels stacked: 0=question, 1=answer, 2=finished
    QStackedWidget* m_controlStack{nullptr};

    // Question panel
    QPushButton*    m_showAnswerBtn{nullptr};

    // Answer panel
    QPushButton*    m_againBtn{nullptr};
    QPushButton*    m_hardBtn{nullptr};
    QPushButton*    m_goodBtn{nullptr};
    QPushButton*    m_easyBtn{nullptr};

    // Sub-labels showing the calculated interval under each button
    QLabel*         m_againLabel{nullptr};
    QLabel*         m_hardLabel{nullptr};
    QLabel*         m_goodLabel{nullptr};
    QLabel*         m_easyLabel{nullptr};

    enum ControlPage { PageQuestion = 0, PageAnswer = 1, PageFinished = 2 };
};
