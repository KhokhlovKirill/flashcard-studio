#pragma once

#include <QAbstractTableModel>
#include <memory>
#include "Deck.h"

// MVC Model: exposes cards of one Deck to a QTableView.
// Columns: 0=CardID (hidden), 1=Front preview, 2=Back preview
class CardTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        ColId    = 0,
        ColFront = 1,
        ColBack  = 2,
        ColumnCount
    };

    explicit CardTableModel(QObject* parent = nullptr);

    void setDeck(std::shared_ptr<Deck> deck);
    std::shared_ptr<Deck> currentDeck() const { return m_deck; }
    std::shared_ptr<Card> cardAt(int row) const;

    // QAbstractTableModel interface
    int     rowCount   (const QModelIndex& parent = QModelIndex()) const override;
    int     columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data      (const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Mutation helpers — callers must also call ProjectManager::markModified()
    void addCard();
    void removeCard(int row);

    // Refresh a single row after its HTML text was edited in-place
    void refreshRow(int row);

    // Refresh the whole view (e.g. after switching decks)
    void refresh();

private:
    std::shared_ptr<Deck> m_deck;

    // Returns plain text extracted from an HTML string (for preview column)
    static QString stripHtml(const QString& html);
};
