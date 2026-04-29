#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QToolButton>
#include "DeckItemDelegate.h"

// Panel showing the flat list of decks with search and add controls.
// Installed as content of the QDockWidget in MainWindow.
class DeckListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeckListWidget(QWidget* parent = nullptr);

    // Rebuilds the list from the current ProjectManager state
    void refresh();

signals:
    void editDeckRequested (const QString& deckId);
    void learnDeckRequested(const QString& deckId);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onAddDeck();
    void onDeleteDeck();
    void onSearchTextChanged(const QString& text);

private:
    void setupUi();
    void connectSignals();
    void filterItems(const QString& text);

    QLineEdit*        m_searchEdit{nullptr};
    QToolButton*      m_addButton{nullptr};
    QListWidget*      m_listWidget{nullptr};
    DeckItemDelegate* m_delegate{nullptr};
};
