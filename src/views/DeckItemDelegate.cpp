#include "DeckItemDelegate.h"

#include "../utils/ThemeManager.h"

#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

DeckItemDelegate::DeckItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_editIcon (QIcon(":/icons/edit.svg"))
    , m_learnIcon(QIcon(":/icons/play_arrow.svg"))
{}

// ── Geometry helpers ─────────────────────────────────────────────────────────

QRect DeckItemDelegate::editButtonRect(const QStyleOptionViewItem& opt) const
{
    const int y = opt.rect.top() + (opt.rect.height() - kButtonSize) / 2;
    // Learn button is rightmost, edit is to its left
    const int learnX = opt.rect.right() - kRightMargin - kButtonSize;
    const int editX  = learnX - kButtonGap - kButtonSize;
    return QRect(editX, y, kButtonSize, kButtonSize);
}

QRect DeckItemDelegate::learnButtonRect(const QStyleOptionViewItem& opt) const
{
    const int y = opt.rect.top() + (opt.rect.height() - kButtonSize) / 2;
    const int x = opt.rect.right() - kRightMargin - kButtonSize;
    return QRect(x, y, kButtonSize, kButtonSize);
}

// ── Paint ────────────────────────────────────────────────────────────────────

void DeckItemDelegate::paint(QPainter* painter,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (index != m_hoveredIndex) return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Semi-transparent button backgrounds
    const QColor bgHover = ThemeManager::instance().hoverButtonColor();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgHover);
    painter->drawRoundedRect(editButtonRect(option),  4, 4);
    painter->drawRoundedRect(learnButtonRect(option), 4, 4);

    // Icons
    m_editIcon .paint(painter, editButtonRect(option));
    m_learnIcon.paint(painter, learnButtonRect(option));

    painter->restore();
}

QSize DeckItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    QSize sh = QStyledItemDelegate::sizeHint(option, index);
    sh.setHeight(std::max(sh.height(), kRowHeight));
    return sh;
}

// ── Editor events (mouse clicks) ─────────────────────────────────────────────

bool DeckItemDelegate::editorEvent(QEvent* event,
                                    QAbstractItemModel* /*model*/,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index)
{
    if (event->type() != QEvent::MouseButtonRelease) return false;

    auto* me = static_cast<QMouseEvent*>(event);
    if (me->button() != Qt::LeftButton) return false;

    const QString deckId = index.data(Qt::UserRole).toString();
    if (deckId.isEmpty()) return false;

    if (editButtonRect(option).contains(me->pos())) {
        emit editRequested(deckId);
        return true;
    }
    if (learnButtonRect(option).contains(me->pos())) {
        emit learnRequested(deckId);
        return true;
    }
    return false;
}

void DeckItemDelegate::setHoveredIndex(const QModelIndex& index)
{
    m_hoveredIndex = QPersistentModelIndex(index);
}
