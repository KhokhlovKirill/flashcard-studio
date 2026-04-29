#include "Deck.h"
#include <QUuid>

Deck::Deck()
    : deckId(QUuid::createUuid().toString(QUuid::WithoutBraces))
{}

Deck::Deck(const QString& id, const QString& name)
    : deckId(id)
    , name(name)
{}

std::shared_ptr<Card> Deck::findCard(const QString& cardId) const
{
    auto it = std::find_if(cards.cbegin(), cards.cend(),
        [&cardId](const std::shared_ptr<Card>& c) {
            return c->cardId == cardId;
        });
    return (it != cards.cend()) ? *it : nullptr;
}

void Deck::addCard(std::shared_ptr<Card> card)
{
    cards.push_back(std::move(card));
}

bool Deck::removeCard(const QString& cardId)
{
    auto it = std::find_if(cards.begin(), cards.end(),
        [&cardId](const std::shared_ptr<Card>& c) {
            return c->cardId == cardId;
        });
    if (it != cards.end()) {
        cards.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Deck> Deck::createNew(const QString& name)
{
    auto deck = std::make_shared<Deck>();
    deck->name = name;
    return deck;
}
