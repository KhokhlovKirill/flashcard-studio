#include "FileIO.h"

#include "../models/Project.h"
#include "../models/Deck.h"
#include "../models/Card.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDate>

// ── JSON keys ────────────────────────────────────────────────────────────────
static const char* kProjectName   = "ProjectName";
static const char* kLastModified  = "LastModified";
static const char* kDecks         = "Decks";
static const char* kDeckID        = "DeckID";
static const char* kDeckName      = "Name";
static const char* kCards         = "Cards";
static const char* kCardID        = "CardID";
static const char* kFront         = "Front";
static const char* kBack          = "Back";
static const char* kRepetitions   = "Repetitions";
static const char* kEaseFactor    = "EaseFactor";
static const char* kInterval      = "Interval";
static const char* kNextReview    = "NextReviewDate";

// ── Serialization ────────────────────────────────────────────────────────────

static QJsonObject cardToJson(const Card& c)
{
    QJsonObject obj;
    obj[kCardID]      = c.cardId;
    obj[kFront]       = c.front;
    obj[kBack]        = c.back;
    obj[kRepetitions] = c.repetitions;
    obj[kEaseFactor]  = c.easeFactor;
    obj[kInterval]    = c.interval;
    obj[kNextReview]  = c.nextReviewDate.toString(Qt::ISODate);
    return obj;
}

static QJsonObject deckToJson(const Deck& d)
{
    QJsonObject obj;
    obj[kDeckID]   = d.deckId;
    obj[kDeckName] = d.name;

    QJsonArray cards;
    for (const auto& card : d.cards) {
        cards.append(cardToJson(*card));
    }
    obj[kCards] = cards;
    return obj;
}

bool FileIO::save(const Project& project, const QString& path)
{
    QJsonObject root;
    root[kProjectName]  = project.projectName;
    root[kLastModified] = project.lastModified.toString(Qt::ISODate);

    QJsonArray decks;
    for (const auto& deck : project.decks) {
        decks.append(deckToJson(*deck));
    }
    root[kDecks] = decks;

    QJsonDocument doc(root);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

// ── Deserialization ──────────────────────────────────────────────────────────

static std::shared_ptr<Card> cardFromJson(const QJsonObject& obj)
{
    auto card           = std::make_shared<Card>();
    card->cardId        = obj[kCardID].toString(card->cardId);
    card->front         = obj[kFront].toString();
    card->back          = obj[kBack].toString();
    card->repetitions   = obj[kRepetitions].toInt(0);
    card->easeFactor    = obj[kEaseFactor].toDouble(2.5);
    card->interval      = obj[kInterval].toInt(1);
    card->nextReviewDate = QDate::fromString(obj[kNextReview].toString(), Qt::ISODate);
    if (!card->nextReviewDate.isValid()) {
        card->nextReviewDate = QDate::currentDate();
    }
    return card;
}

static std::shared_ptr<Deck> deckFromJson(const QJsonObject& obj)
{
    auto deck  = std::make_shared<Deck>(obj[kDeckID].toString(), obj[kDeckName].toString());

    const QJsonArray cards = obj[kCards].toArray();
    for (const QJsonValue& val : cards) {
        deck->addCard(cardFromJson(val.toObject()));
    }
    return deck;
}

std::unique_ptr<Project> FileIO::load(const QString& path, QString& errorMsg)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMsg = QObject::tr("Cannot open file: %1").arg(path);
        return nullptr;
    }

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseErr);
    if (doc.isNull()) {
        errorMsg = parseErr.errorString();
        return nullptr;
    }

    const QJsonObject root = doc.object();
    auto project = std::make_unique<Project>();
    project->projectName  = root[kProjectName].toString("Untitled");
    project->lastModified = QDateTime::fromString(root[kLastModified].toString(), Qt::ISODate);
    if (!project->lastModified.isValid()) {
        project->lastModified = QDateTime::currentDateTime();
    }

    const QJsonArray decks = root[kDecks].toArray();
    for (const QJsonValue& val : decks) {
        project->addDeck(deckFromJson(val.toObject()));
    }

    return project;
}
