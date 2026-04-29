#pragma once

#include <QString>
#include <QDate>

// Plain data class — no Qt parent, managed via shared_ptr
class Card {
public:
    Card();

    QString cardId;        // UUID without braces
    QString front;         // HTML rich text (from QTextEdit)
    QString back;          // HTML rich text (from QTextEdit)

    // SM-2 spaced repetition metadata
    int    repetitions{0};
    double easeFactor{2.5};
    int    interval{1};
    QDate  nextReviewDate;

    // Factory: creates a new Card with fresh UUID and default SM-2 values
    static Card createNew();
};
