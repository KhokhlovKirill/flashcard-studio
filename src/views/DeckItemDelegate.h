#pragma once

#include <QStyledItemDelegate>
#include <QPersistentModelIndex>
#include <QIcon>

// Custom delegate that draws two hover-action buttons (Edit / Learn) on the
// right side of each deck row when the mouse is over it.
// The parent view is responsible for calling setHoveredRow() from its
// mouseMoveEvent / Leave event filter so the delegate can repaint selectively.
class DeckItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit DeckItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

    bool editorEvent(QEvent* event, QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

    void setHoveredIndex(const QModelIndex& index);

signals:
    void editRequested (const QString& deckId);
    void learnRequested(const QString& deckId);

private:
    static constexpr int kButtonSize  = 22;
    static constexpr int kButtonGap   =  4;
    static constexpr int kRightMargin =  6;
    static constexpr int kRowHeight   = 40;

    QRect editButtonRect (const QStyleOptionViewItem& opt) const;
    QRect learnButtonRect(const QStyleOptionViewItem& opt) const;

    QPersistentModelIndex m_hoveredIndex;
    QIcon m_editIcon;
    QIcon m_learnIcon;
};
