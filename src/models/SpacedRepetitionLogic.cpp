#include "SpacedRepetitionLogic.h"

#include <QDate>
#include <algorithm>
#include <cmath>

// ── SM-2 helpers ─────────────────────────────────────────────────────────────

static int safeInterval(double raw)
{
    return std::max(1, static_cast<int>(std::round(raw)));
}

// ── Public API ───────────────────────────────────────────────────────────────

void SpacedRepetitionLogic::applyAgain(Card& c)
{
    c.repetitions = 0;
    c.interval    = 1;
    c.easeFactor  = std::max(1.3, c.easeFactor - 0.20);
    // Card stays in today's queue — NextReviewDate not pushed into the future
    c.nextReviewDate = QDate::currentDate();
}

void SpacedRepetitionLogic::applyHard(Card& c)
{
    c.repetitions += 1;
    c.interval     = safeInterval(c.interval * 1.2);
    c.nextReviewDate = QDate::currentDate().addDays(c.interval);
}

void SpacedRepetitionLogic::applyGood(Card& c)
{
    c.repetitions += 1;
    c.interval     = safeInterval(c.interval * c.easeFactor);
    c.nextReviewDate = QDate::currentDate().addDays(c.interval);
}

void SpacedRepetitionLogic::applyEasy(Card& c)
{
    c.repetitions += 1;
    c.easeFactor  += 0.15;
    c.interval     = safeInterval(c.interval * c.easeFactor * 1.3);
    c.nextReviewDate = QDate::currentDate().addDays(c.interval);
}

QList<std::shared_ptr<Card>> SpacedRepetitionLogic::dueCards(const Deck& deck)
{
    const QDate today = QDate::currentDate();
    QList<std::shared_ptr<Card>> result;
    for (const auto& card : deck.cards) {
        if (card->nextReviewDate <= today) {
            result.append(card);
        }
    }
    return result;
}

// ── Preview helpers (non-mutating) ───────────────────────────────────────────

int SpacedRepetitionLogic::previewIntervalAgain(const Card&)
{
    return 1;
}

int SpacedRepetitionLogic::previewIntervalHard(const Card& c)
{
    return safeInterval(c.interval * 1.2);
}

int SpacedRepetitionLogic::previewIntervalGood(const Card& c)
{
    return safeInterval(c.interval * c.easeFactor);
}

int SpacedRepetitionLogic::previewIntervalEasy(const Card& c)
{
    return safeInterval(c.interval * (c.easeFactor + 0.15) * 1.3);
}
