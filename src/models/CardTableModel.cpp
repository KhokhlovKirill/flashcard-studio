#include "CardTableModel.h"

#include <QTextDocument>

CardTableModel::CardTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{}

void CardTableModel::setDeck(std::shared_ptr<Deck> deck)
{
    beginResetModel();
    m_deck = std::move(deck);
    endResetModel();
}

std::shared_ptr<Card> CardTableModel::cardAt(int row) const
{
    if (!m_deck || row < 0 || row >= static_cast<int>(m_deck->cards.size()))
        return nullptr;
    return m_deck->cards[static_cast<std::size_t>(row)];
}

int CardTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || !m_deck) return 0;
    return static_cast<int>(m_deck->cards.size());
}

int CardTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return ColumnCount;
}

QVariant CardTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !m_deck) return {};
    const int row = index.row();
    if (row < 0 || row >= static_cast<int>(m_deck->cards.size())) return {};

    const Card& card = *m_deck->cards[static_cast<std::size_t>(row)];

    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        switch (index.column()) {
        case ColId:    return card.cardId;
        case ColFront: return stripHtml(card.front);
        case ColBack:  return stripHtml(card.back);
        default:       return {};
        }
    }

    if (role == Qt::UserRole) {
        // Expose raw HTML for the editor to use
        switch (index.column()) {
        case ColFront: return card.front;
        case ColBack:  return card.back;
        default:       return {};
        }
    }

    return {};
}

QVariant CardTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
    case ColId:    return tr("ID");
    case ColFront: return tr("Вопрос");
    case ColBack:  return tr("Ответ");
    default:       return {};
    }
}

void CardTableModel::addCard()
{
    if (!m_deck) return;
    const int row = static_cast<int>(m_deck->cards.size());
    beginInsertRows(QModelIndex(), row, row);
    m_deck->addCard(std::make_shared<Card>(Card::createNew()));
    endInsertRows();
}

void CardTableModel::removeCard(int row)
{
    if (!m_deck || row < 0 || row >= static_cast<int>(m_deck->cards.size())) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_deck->cards.erase(m_deck->cards.begin() + row);
    endRemoveRows();
}

void CardTableModel::refreshRow(int row)
{
    if (!m_deck || row < 0 || row >= static_cast<int>(m_deck->cards.size())) return;
    const QModelIndex topLeft     = index(row, ColFront);
    const QModelIndex bottomRight = index(row, ColBack);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

void CardTableModel::refresh()
{
    beginResetModel();
    endResetModel();
}

QString CardTableModel::stripHtml(const QString& html)
{
    if (html.isEmpty()) return {};
    QTextDocument doc;
    doc.setHtml(html);
    return doc.toPlainText().simplified();
}
