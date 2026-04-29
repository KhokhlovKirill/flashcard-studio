#pragma once

#include <QWidget>
#include <QTableView>
#include <QTextEdit>
#include <QSplitter>
#include <QLabel>
#include <QTimer>
#include <QAction>
#include <memory>

#include "../models/CardTableModel.h"
#include "../models/Deck.h"
#include "../models/Card.h"

// Card editor view.
// Top half: toolbar + QTableView (CardTableModel).
// Bottom half: dual rich-text editor (Front / Back) with formatting toolbar.
// Card text changes are debounced (500 ms) before writing back to the model.
class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget* parent = nullptr);

    // Switch to a different deck; resets selection and editor state
    void setDeck(std::shared_ptr<Deck> deck);
    std::shared_ptr<Deck> currentDeck() const { return m_deck; }

private slots:
    void onAddCard();
    void onDeleteCard();
    void onSelectionChanged();

    void onFrontTextChanged();
    void onBackTextChanged();
    void commitFrontToModel();
    void commitBackToModel();

    void onCurrentCharFormatChanged(const QTextCharFormat& fmt);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onUnderlineToggled(bool checked);

private:
    void setupUi();
    void connectSignals();

    void loadCardIntoEditor(std::shared_ptr<Card> card);
    void clearEditor();
    void updateStats();

    QTextEdit* focusedEditor() const;

    // ── Data ─────────────────────────────────────────────────────────────
    CardTableModel*    m_model{nullptr};
    std::shared_ptr<Deck> m_deck;
    std::shared_ptr<Card> m_currentCard;
    bool m_updatingEditor{false};  // guard against re-entrant signals

    // ── Top section ───────────────────────────────────────────────────────
    QLabel*    m_deckLabel{nullptr};
    QLabel*    m_statsLabel{nullptr};
    QTableView* m_tableView{nullptr};

    // ── Bottom section (editor) ───────────────────────────────────────────
    QTextEdit* m_frontEdit{nullptr};
    QTextEdit* m_backEdit{nullptr};

    // Debounce timers — fire 500 ms after the last keystroke
    QTimer* m_frontDebounce{nullptr};
    QTimer* m_backDebounce{nullptr};

    // ── Formatting actions (shared between both editors) ──────────────────
    QAction* m_boldAction{nullptr};
    QAction* m_italicAction{nullptr};
    QAction* m_underlineAction{nullptr};
};
