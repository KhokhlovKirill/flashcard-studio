#include "Card.h"
#include <QUuid>
#include <QDate>

Card::Card()
    : cardId(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , repetitions(0)
    , easeFactor(2.5)
    , interval(1)
    , nextReviewDate(QDate::currentDate())
{}

Card Card::createNew()
{
    return Card{};
}
