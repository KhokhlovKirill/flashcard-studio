#pragma once

#include <QString>
#include <vector>
#include <memory>
#include <algorithm>
#include "Card.h"

// Plain data class — no Qt parent, managed via shared_ptr
class Deck {
public:
    Deck();
    Deck(const QString& id, const QString& name);

    QString deckId;  // UUID without braces
    QString name;
    std::vector<std::shared_ptr<Card>> cards;

    std::shared_ptr<Card> findCard(const QString& cardId) const;
    void addCard(std::shared_ptr<Card> card);
    bool removeCard(const QString& cardId);

    // Factory: creates a new Deck with fresh UUID
    static std::shared_ptr<Deck> createNew(const QString& name);
};
