#include "DeckListWidget.h"

#include "../models/ProjectManager.h"
#include "../models/Project.h"
#include "../models/Deck.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QIcon>
#include <QFont>

DeckListWidget::DeckListWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();
}

void DeckListWidget::setupUi()
{
    // Search bar + Add button row
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("🔍  Поиск колоды..."));
    m_searchEdit->setClearButtonEnabled(true);

    m_addButton = new QToolButton(this);
    m_addButton->setIcon(QIcon(":/icons/add.svg"));
    m_addButton->setToolTip(tr("Добавить колоду"));
    m_addButton->setAutoRaise(true);
    m_addButton->setIconSize(QSize(20, 20));

    auto* topRow = new QHBoxLayout;
    topRow->setContentsMargins(4, 4, 4, 4);
    topRow->setSpacing(4);
    topRow->addWidget(m_searchEdit, 1);
    topRow->addWidget(m_addButton);

    // Deck list
    m_listWidget = new QListWidget(this);
    m_listWidget->setFrameShape(QFrame::NoFrame);
    m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Delegate for hover buttons
    m_delegate = new DeckItemDelegate(m_listWidget);
    m_listWidget->setItemDelegate(m_delegate);

    // Enable hover tracking so the delegate can draw hover buttons
    m_listWidget->viewport()->setMouseTracking(true);
    m_listWidget->viewport()->installEventFilter(this);

    // Delete action (context menu)
    auto* deleteAction = new QAction(QIcon(":/icons/delete.svg"), tr("Удалить колоду"), this);
    deleteAction->setToolTip(tr("Удалить выбранную колоду"));
    m_listWidget->addAction(deleteAction);
    connect(deleteAction, &QAction::triggered, this, &DeckListWidget::onDeleteDeck);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(topRow);
    mainLayout->addWidget(m_listWidget, 1);
}

void DeckListWidget::connectSignals()
{
    connect(m_addButton, &QToolButton::clicked,
            this, &DeckListWidget::onAddDeck);

    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &DeckListWidget::onSearchTextChanged);

    connect(m_delegate, &DeckItemDelegate::editRequested,
            this, &DeckListWidget::editDeckRequested);

    connect(m_delegate, &DeckItemDelegate::learnRequested,
            this, &DeckListWidget::learnDeckRequested);
}

// ── Event filter for hover tracking ─────────────────────────────────────────

bool DeckListWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_listWidget->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            auto* me = static_cast<QMouseEvent*>(event);
            const QModelIndex idx = m_listWidget->indexAt(me->pos());
            m_delegate->setHoveredIndex(idx);
            m_listWidget->viewport()->update();
        } else if (event->type() == QEvent::Leave) {
            m_delegate->setHoveredIndex(QModelIndex());
            m_listWidget->viewport()->update();
        }
    }
    return QWidget::eventFilter(watched, event);
}

// ── Slots ────────────────────────────────────────────────────────────────────

void DeckListWidget::onAddDeck()
{
    auto& pm = ProjectManager::instance();
    if (!pm.hasProject()) return;

    bool ok = false;
    const QString name = QInputDialog::getText(
        this, tr("Новая колода"), tr("Название колоды:"),
        QLineEdit::Normal, QString(), &ok);

    if (!ok || name.trimmed().isEmpty()) return;

    auto deck = Deck::createNew(name.trimmed());
    pm.project()->addDeck(deck);
    pm.markModified();
    emit pm.deckListChanged();
    refresh();
}

void DeckListWidget::onDeleteDeck()
{
    auto* item = m_listWidget->currentItem();
    if (!item) return;

    const QString deckId = item->data(Qt::UserRole).toString();
    auto& pm = ProjectManager::instance();
    if (!pm.hasProject()) return;

    const int answer = QMessageBox::question(
        this, tr("Удалить колоду"),
        tr("Удалить колоду «%1»?\nВсе карточки будут потеряны.").arg(item->text()),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes) return;

    pm.project()->removeDeck(deckId);
    pm.markModified();
    emit pm.deckListChanged();
    refresh();
}

void DeckListWidget::onSearchTextChanged(const QString& text)
{
    filterItems(text);
}

// ── Public ───────────────────────────────────────────────────────────────────

void DeckListWidget::refresh()
{
    m_listWidget->clear();

    auto& pm = ProjectManager::instance();
    if (!pm.hasProject()) return;

    const QIcon deckIcon(":/icons/deck.svg");

    for (const auto& deck : pm.project()->decks) {
        auto* item = new QListWidgetItem(deckIcon, deck->name);
        item->setData(Qt::UserRole, deck->deckId);
        item->setSizeHint(QSize(0, 40));
        m_listWidget->addItem(item);
    }

    filterItems(m_searchEdit->text());
}

void DeckListWidget::filterItems(const QString& text)
{
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem* item = m_listWidget->item(i);
        const bool visible = text.isEmpty() ||
                             item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!visible);
    }
}
