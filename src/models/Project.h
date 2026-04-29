#pragma once

#include <QString>
#include <QDateTime>
#include <vector>
#include <memory>
#include "Deck.h"

// Root data object for a .flcr project file
class Project {
public:
    Project() = default;

    QString   projectName;
    QDateTime lastModified;
    std::vector<std::shared_ptr<Deck>> decks;

    std::shared_ptr<Deck> findDeck(const QString& deckId) const;
    void addDeck(std::shared_ptr<Deck> deck);
    bool removeDeck(const QString& deckId);
};
