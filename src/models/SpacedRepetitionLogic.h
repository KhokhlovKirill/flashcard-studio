#pragma once

#include <QList>
#include <memory>
#include "Card.h"
#include "Deck.h"

// Pure-static class implementing the SM-2 spaced repetition algorithm.
// All methods mutate the Card in place and update NextReviewDate.
class SpacedRepetitionLogic
{
public:
    SpacedRepetitionLogic() = delete;

    // Apply the four rating outcomes to a card
    static void applyAgain(Card& c);   // Забыл  — red
    static void applyHard(Card& c);    // Трудно — orange
    static void applyGood(Card& c);    // Хорошо — blue   (default)
    static void applyEasy(Card& c);    // Легко  — green

    // Returns the subset of cards whose NextReviewDate <= today
    static QList<std::shared_ptr<Card>> dueCards(const Deck& deck);

    // Preview the interval (days) that each rating would produce,
    // without mutating the card.  Used for the label under each button.
    static int previewIntervalAgain(const Card& c);
    static int previewIntervalHard (const Card& c);
    static int previewIntervalGood (const Card& c);
    static int previewIntervalEasy (const Card& c);
};
