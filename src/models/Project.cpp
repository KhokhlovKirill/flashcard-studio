#include "Project.h"
#include <algorithm>

std::shared_ptr<Deck> Project::findDeck(const QString& deckId) const
{
    auto it = std::find_if(decks.cbegin(), decks.cend(),
        [&deckId](const std::shared_ptr<Deck>& d) {
            return d->deckId == deckId;
        });
    return (it != decks.cend()) ? *it : nullptr;
}

void Project::addDeck(std::shared_ptr<Deck> deck)
{
    decks.push_back(std::move(deck));
}

bool Project::removeDeck(const QString& deckId)
{
    auto it = std::find_if(decks.begin(), decks.end(),
        [&deckId](const std::shared_ptr<Deck>& d) {
            return d->deckId == deckId;
        });
    if (it != decks.end()) {
        decks.erase(it);
        return true;
    }
    return false;
}
