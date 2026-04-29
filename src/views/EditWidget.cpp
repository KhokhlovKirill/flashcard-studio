#include "EditWidget.h"

#include "../models/ProjectManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFont>
#include <QSplitter>
#include <QGroupBox>
#include <QIcon>
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>

EditWidget::EditWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();
}

// ── UI construction ──────────────────────────────────────────────────────────

void EditWidget::setupUi()
{
    m_model = new CardTableModel(this);

    // ── Card list toolbar ─────────────────────────────────────────────────
    auto* cardToolBar = new QToolBar(this);
    cardToolBar->setMovable(false);
    cardToolBar->setIconSize(QSize(18, 18));
    cardToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QAction* addCardAction = cardToolBar->addAction(
        QIcon(":/icons/add.svg"), tr("Добавить карточку"));
    addCardAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    addCardAction->setToolTip(tr("Добавить карточку (Ctrl+N)"));

    QAction* delCardAction = cardToolBar->addAction(
        QIcon(":/icons/delete.svg"), tr("Удалить карточку"));
    delCardAction->setShortcut(QKeySequence::Delete);
    delCardAction->setToolTip(tr("Удалить карточку (Del)"));

    cardToolBar->addSeparator();
    m_statsLabel = new QLabel(tr("0 карточек"), this);
    m_statsLabel->setContentsMargins(4, 0, 4, 0);
    cardToolBar->addWidget(m_statsLabel);

    connect(addCardAction, &QAction::triggered, this, &EditWidget::onAddCard);
    connect(delCardAction, &QAction::triggered, this, &EditWidget::onDeleteCard);

    // Deck name label
    m_deckLabel = new QLabel(this);
    QFont f = m_deckLabel->font();
    f.setBold(true);
    f.setPointSize(f.pointSize() + 1);
    m_deckLabel->setFont(f);
    m_deckLabel->setContentsMargins(6, 4, 4, 4);

    // ── Table view ────────────────────────────────────────────────────────
    m_tableView = new QTableView(this);
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->horizontalHeader()->setSectionResizeMode(
        CardTableModel::ColId, QHeaderView::Fixed);
    m_tableView->setColumnHidden(CardTableModel::ColId, true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->verticalHeader()->setVisible(false);

    auto* topWidget = new QWidget(this);
    auto* topLayout = new QVBoxLayout(topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(0);
    topLayout->addWidget(m_deckLabel);
    topLayout->addWidget(cardToolBar);
    topLayout->addWidget(m_tableView, 1);

    // ── Formatting toolbar ────────────────────────────────────────────────
    m_boldAction      = new QAction(QIcon(":/icons/format_bold.svg"),      tr("Жирный"),     this);
    m_italicAction    = new QAction(QIcon(":/icons/format_italic.svg"),    tr("Курсив"),      this);
    m_underlineAction = new QAction(QIcon(":/icons/format_underlined.svg"),tr("Подчёркнутый"), this);

    m_boldAction->setCheckable(true);
    m_italicAction->setCheckable(true);
    m_underlineAction->setCheckable(true);

    m_boldAction->setShortcut(QKeySequence::Bold);
    m_italicAction->setShortcut(QKeySequence::Italic);
    m_underlineAction->setShortcut(QKeySequence::Underline);

    m_boldAction->setToolTip(tr("Жирный (Ctrl+B)"));
    m_italicAction->setToolTip(tr("Курсив (Ctrl+I)"));
    m_underlineAction->setToolTip(tr("Подчёркнутый (Ctrl+U)"));

    auto* fmtBar = new QToolBar(this);
    fmtBar->setMovable(false);
    fmtBar->setIconSize(QSize(18, 18));
    fmtBar->addAction(m_boldAction);
    fmtBar->addAction(m_italicAction);
    fmtBar->addAction(m_underlineAction);

    // ── Editor panels ─────────────────────────────────────────────────────
    m_frontEdit = new QTextEdit(this);
    m_frontEdit->setPlaceholderText(tr("Вопрос (лицевая сторона)…"));
    m_frontEdit->setMinimumHeight(80);

    m_backEdit = new QTextEdit(this);
    m_backEdit->setPlaceholderText(tr("Ответ (обратная сторона)…"));
    m_backEdit->setMinimumHeight(80);

    auto* editorsLayout = new QHBoxLayout;
    editorsLayout->setContentsMargins(4, 0, 4, 4);
    editorsLayout->setSpacing(6);

    auto* frontBox  = new QGroupBox(tr("Вопрос"), this);
    auto* frontVBox = new QVBoxLayout(frontBox);
    frontVBox->setContentsMargins(4, 4, 4, 4);
    frontVBox->addWidget(m_frontEdit);

    auto* backBox  = new QGroupBox(tr("Ответ"), this);
    auto* backVBox = new QVBoxLayout(backBox);
    backVBox->setContentsMargins(4, 4, 4, 4);
    backVBox->addWidget(m_backEdit);

    editorsLayout->addWidget(frontBox, 1);
    editorsLayout->addWidget(backBox, 1);

    auto* bottomWidget = new QWidget(this);
    auto* bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setSpacing(0);
    bottomLayout->addWidget(fmtBar);
    bottomLayout->addLayout(editorsLayout, 1);

    // ── Main splitter ─────────────────────────────────────────────────────
    auto* splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(topWidget);
    splitter->addWidget(bottomWidget);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 2);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(splitter);

    // ── Debounce timers ───────────────────────────────────────────────────
    m_frontDebounce = new QTimer(this);
    m_frontDebounce->setSingleShot(true);
    m_frontDebounce->setInterval(500);

    m_backDebounce = new QTimer(this);
    m_backDebounce->setSingleShot(true);
    m_backDebounce->setInterval(500);
}

void EditWidget::connectSignals()
{
    connect(m_tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &EditWidget::onSelectionChanged);

    connect(m_frontEdit, &QTextEdit::textChanged,
            this, &EditWidget::onFrontTextChanged);
    connect(m_backEdit, &QTextEdit::textChanged,
            this, &EditWidget::onBackTextChanged);

    connect(m_frontDebounce, &QTimer::timeout,
            this, &EditWidget::commitFrontToModel);
    connect(m_backDebounce, &QTimer::timeout,
            this, &EditWidget::commitBackToModel);

    connect(m_frontEdit, &QTextEdit::currentCharFormatChanged,
            this, &EditWidget::onCurrentCharFormatChanged);
    connect(m_backEdit, &QTextEdit::currentCharFormatChanged,
            this, &EditWidget::onCurrentCharFormatChanged);

    connect(m_boldAction,      &QAction::toggled, this, &EditWidget::onBoldToggled);
    connect(m_italicAction,    &QAction::toggled, this, &EditWidget::onItalicToggled);
    connect(m_underlineAction, &QAction::toggled, this, &EditWidget::onUnderlineToggled);
}

// ── Public ───────────────────────────────────────────────────────────────────

void EditWidget::setDeck(std::shared_ptr<Deck> deck)
{
    m_deck = std::move(deck);
    m_model->setDeck(m_deck);
    m_currentCard.reset();
    clearEditor();
    updateStats();

    if (m_deck) {
        m_deckLabel->setText(m_deck->name);
    }

    // Select first card if available
    if (m_model->rowCount() > 0) {
        m_tableView->selectRow(0);
    }
}

// ── Slots ────────────────────────────────────────────────────────────────────

void EditWidget::onAddCard()
{
    m_model->addCard();
    const int newRow = m_model->rowCount() - 1;
    m_tableView->selectRow(newRow);
    m_tableView->scrollToBottom();
    ProjectManager::instance().markModified();
    updateStats();
}

void EditWidget::onDeleteCard()
{
    const QModelIndex idx = m_tableView->currentIndex();
    if (!idx.isValid()) return;

    m_model->removeCard(idx.row());
    ProjectManager::instance().markModified();
    updateStats();
    clearEditor();
}

void EditWidget::onSelectionChanged()
{
    const QModelIndex idx = m_tableView->currentIndex();
    if (!idx.isValid()) {
        m_currentCard.reset();
        clearEditor();
        return;
    }
    m_currentCard = m_model->cardAt(idx.row());
    loadCardIntoEditor(m_currentCard);
}

void EditWidget::onFrontTextChanged()
{
    if (m_updatingEditor) return;
    m_frontDebounce->start(); // restarts the timer if already running
}

void EditWidget::onBackTextChanged()
{
    if (m_updatingEditor) return;
    m_backDebounce->start();
}

void EditWidget::commitFrontToModel()
{
    if (!m_currentCard) return;
    m_currentCard->front = m_frontEdit->toHtml();

    const int row = m_tableView->currentIndex().row();
    m_model->refreshRow(row);
    ProjectManager::instance().markModified();
}

void EditWidget::commitBackToModel()
{
    if (!m_currentCard) return;
    m_currentCard->back = m_backEdit->toHtml();

    const int row = m_tableView->currentIndex().row();
    m_model->refreshRow(row);
    ProjectManager::instance().markModified();
}

void EditWidget::onCurrentCharFormatChanged(const QTextCharFormat& fmt)
{
    // Keep formatting buttons in sync with cursor position
    m_boldAction->blockSignals(true);
    m_italicAction->blockSignals(true);
    m_underlineAction->blockSignals(true);

    m_boldAction->setChecked(fmt.fontWeight() >= QFont::Bold);
    m_italicAction->setChecked(fmt.fontItalic());
    m_underlineAction->setChecked(fmt.fontUnderline());

    m_boldAction->blockSignals(false);
    m_italicAction->blockSignals(false);
    m_underlineAction->blockSignals(false);
}

void EditWidget::onBoldToggled(bool checked)
{
    QTextEdit* te = focusedEditor();
    if (!te) return;
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    te->mergeCurrentCharFormat(fmt);
}

void EditWidget::onItalicToggled(bool checked)
{
    QTextEdit* te = focusedEditor();
    if (!te) return;
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    te->mergeCurrentCharFormat(fmt);
}

void EditWidget::onUnderlineToggled(bool checked)
{
    QTextEdit* te = focusedEditor();
    if (!te) return;
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    te->mergeCurrentCharFormat(fmt);
}

// ── Helpers ──────────────────────────────────────────────────────────────────

void EditWidget::loadCardIntoEditor(std::shared_ptr<Card> card)
{
    m_updatingEditor = true;
    if (card) {
        m_frontEdit->setHtml(card->front);
        m_backEdit->setHtml(card->back);
    } else {
        m_frontEdit->clear();
        m_backEdit->clear();
    }
    m_updatingEditor = false;
}

void EditWidget::clearEditor()
{
    m_updatingEditor = true;
    m_frontEdit->clear();
    m_backEdit->clear();
    m_updatingEditor = false;
}

void EditWidget::updateStats()
{
    const int count = m_model->rowCount();
    m_statsLabel->setText(tr("%n карточек", "", count));
}

QTextEdit* EditWidget::focusedEditor() const
{
    QWidget* fw = QApplication::focusWidget();
    if (fw == m_frontEdit) return m_frontEdit;
    if (fw == m_backEdit)  return m_backEdit;
    // Fallback: if neither has explicit focus, return the last one that did
    return nullptr;
}
