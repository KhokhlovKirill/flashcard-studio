# FlashCard Studio — User Guide

## Table of Contents

1. [What is FlashCard Studio?](#1-what-is-flashcard-studio)
2. [Installation](#2-installation)
3. [First Launch](#3-first-launch)
4. [Creating and Opening a Project](#4-creating-and-opening-a-project)
5. [Working with Decks](#5-working-with-decks)
6. [Editing Cards](#6-editing-cards)
7. [Study Mode](#7-study-mode)
8. [Saving and Auto-Save](#8-saving-and-auto-save)
9. [Themes](#9-themes)
10. [Keyboard Shortcuts](#10-keyboard-shortcuts)
11. [Frequently Asked Questions](#11-frequently-asked-questions)

---

## 1. What is FlashCard Studio?

**FlashCard Studio** is a desktop application for creating and studying flashcard decks. It helps you memorize any information more effectively using the **Spaced Repetition** method.

### How does spaced repetition work?

The idea is simple: cards you know well are shown less frequently, while cards you struggle with appear more often. This way you spend your time on what you haven't learned yet, rather than reviewing what you already know.

The application automatically calculates when each card should be shown again and presents it at exactly the right moment.

### What can you use it for?

- Learning foreign words and phrases
- Preparing for exams and tests
- Memorizing dates, formulas, and terminology
- Reviewing any study material

---

## 2. Installation

### System Requirements

- Operating system: Windows 10 or Windows 11 (64-bit)
- RAM: 100 MB (for the application)
- Disk space: ~80 MB

### Installing from the Installer

1. Run the file `FlashCard Studio-1.0.0-win64.exe`.
2. Follow the setup wizard instructions.
3. After installation, a **FlashCard Studio** shortcut will appear on your desktop and in the Start menu.
4. Project files with the `.flcr` extension will automatically open with a double-click.

---

## 3. First Launch

When you start the application, the **Start Screen** opens.

```
┌──────────────────┬─────────────────────────────────┐
│                  │  🕒 Recent Projects              │
│  FlashCard       │                                  │
│  Studio          │  (list of recently opened        │
│                  │   .flcr files)                   │
│  📄 New          │                                  │
│     Project      │                                  │
│                  │                                  │
│  📂 Open         │                                  │
│     Project      │                                  │
└──────────────────┴─────────────────────────────────┘
```

- The **left panel** contains buttons for creating a new project or opening an existing one.
- The **right panel** shows a list of recently opened projects. Double-clicking an entry opens it instantly.

---

## 4. Creating and Opening a Project

### Creating a New Project

1. Click the **"New Project"** button on the start screen  
   *or* select **File → New** (`Ctrl+N`) from the main window.
2. In the save dialog, choose a folder and enter a file name. The file will be saved with the `.flcr` extension.
3. The application will switch to the main window — the project is created and ready to use.

### Opening an Existing Project

- Click **"Open Project"** on the start screen  
  *or* select **File → Open** (`Ctrl+O`).
- In the dialog, locate and select a `.flcr` file.

You can also open a `.flcr` file by double-clicking it in Windows Explorer — the application will launch automatically.

### Closing a Project

- Select **File → Close Project**.
- The application will return to the start screen. If there are unsaved changes, the program will ask whether to save them.

---

## 5. Working with Decks

A **deck** is a named group of cards — for example, "English Verbs", "History of Russia", or "Physics Formulas".

All your decks are listed in the **sidebar** on the left.

### Creating a Deck

Click the **➕** button at the top of the sidebar. Enter a name for the new deck and press Enter.

### Searching for a Deck

Type text into the **🔍 Search decks...** field — the list will filter in real time.

### Deck Actions

When you hover the cursor over any deck, two buttons appear on the right:

| Button | Action |
|--------|--------|
| ✏️ | Open the deck in the **editor** (add or modify cards) |
| ▶️ | Start a **study session** for this deck |

### Deleting a Deck

- Click on a deck to select it, then click the **🗑️** button in the sidebar.
- Confirm the deletion in the dialog. All cards in the deck will be deleted along with it.

---

## 6. Editing Cards

Click the ✏️ button next to a deck to open the **edit screen**.

```
┌──────────────────────────────────────────────────────────────┐
│  Deck Name              [➕ Add]  [🗑️ Delete]  Cards: 12     │
├──────────────────────────────────────────────────────────────┤
│  Front                     │  Back                           │
│  ─────────────────────────────────────────────────────────   │
│  hola                      │  hello                          │
│  guten Morgen              │  good morning                   │
│  ▶ (selected row)          │                                 │
├──────────────────────────────────────────────────────────────┤
│  [B] [I] [U]               │  [B] [I] [U]                    │
│  ┌──────────────────────┐  │  ┌──────────────────────────┐   │
│  │ Front side           │  │  │ Back side                │   │
│  │ (question input)     │  │  │ (answer input)           │   │
│  └──────────────────────┘  │  └──────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
```

### Adding a Card

1. Click the **➕ Add** button or press `Ctrl+N`.
2. A new empty row will appear in the table and be selected automatically.
3. Two input fields will open at the bottom of the screen — enter the **question** (front side) and the **answer** (back side).

### Editing a Card

1. Click a row in the table to select a card.
2. The card's content will appear in the fields below.
3. Make your changes — they will be saved automatically in memory 500 ms after you stop typing.

### Text Formatting

A formatting toolbar appears above each input field:

| Button | Action | Shortcut |
|--------|--------|----------|
| **B** | Bold text | `Ctrl+B` |
| *I* | Italic | `Ctrl+I` |
| <u>U</u> | Underline | `Ctrl+U` |

Select text and click the desired button or use the keyboard shortcut.

### Deleting a Card

Select a card in the table and click the **🗑️ Delete** button (or press `Del`). The action requires confirmation.

---

## 7. Study Mode

Click the **▶️** button next to a deck in the sidebar to open the **study screen**.

The application automatically selects the cards that need to be reviewed today (those whose review date has arrived).

### Session Flow

**Step 1 — Question**

The **front side** of the card is displayed on screen. Read it and try to recall the answer.

```
┌─────────────────────────────┐
│                             │
│         hola                │
│                             │
└─────────────────────────────┘

         [👁️ Show Answer]
```

Press the **"Show Answer"** button or the `Space` key.

**Step 2 — Rating**

The **back side** of the card is revealed. Rate how well you remembered the answer by choosing one of four buttons:

```
┌─────────────────────────────┐
│         hola                │
│  ──────────────────────── │
│         hello               │
└─────────────────────────────┘

 [✖️ Again]  [😟 Hard]  [🙂 Good]  [⭐ Easy]
   1 d.        4 d.       15 d.      28 d.
```

Below each button you can see **how many days** the card will reappear if you choose that rating.

### Rating Button Descriptions

| Button | Color | When to press |
|--------|-------|---------------|
| **Again** | Red | You don't remember the answer at all. The card will return today. |
| **Hard** | Orange | You remembered with difficulty or made errors. The card will come back soon. |
| **Good** | Blue | You remembered correctly, but not instantly. Standard interval. |
| **Easy** | Green | You remembered immediately and effortlessly. The card will come back much later. |

> **Tip:** Try to rate honestly! The program will remember your choice and find the optimal time for the next review.

### Session Progress

A progress bar and a counter of remaining cards are displayed at the top of the screen.

### Completing a Session

When all cards have been reviewed, a completion screen appears:

```
        🎉
   All done for today!
```

Click the "Back" button in the upper-left corner to return to the deck list.

### What if no cards appear?

If pressing ▶️ shows no cards, it means there are no cards due for review today — all intervals are still active. Come back tomorrow!

---

## 8. Saving and Auto-Save

### Manual Save

- **Save** (`Ctrl+S`) — saves the project to the current file.
- **Save As** — saves the project to a new `.flcr` file (the original file remains unchanged).

### Auto-Save

The application **automatically saves** the project every 3 minutes if there are unsaved changes. You can keep working — your data will not be lost.

### Change Indicator

The window title displays a `*` (asterisk) next to the project name when there are unsaved changes. The asterisk disappears after saving.

---

## 9. Themes

The application supports two visual themes:

- **Light Theme** — white background, suitable for working in good lighting conditions.
- **Dark Theme** — dark background, reduces eye strain in dim or nighttime settings.

To switch, select **View → Light Theme** or **View → Dark Theme**.

Your choice is saved and will be applied automatically on every launch.

---

## 10. Keyboard Shortcuts

### General

| Keys | Action |
|------|--------|
| `Ctrl+N` | Create a new project |
| `Ctrl+O` | Open a project |
| `Ctrl+S` | Save the project |
| `Ctrl+Z` | Undo the last action |
| `Ctrl+Y` | Redo the undone action |

### In the Card Editor

| Keys | Action |
|------|--------|
| `Ctrl+N` | Add a new card |
| `Del` | Delete the selected card |
| `Ctrl+B` | Bold text |
| `Ctrl+I` | Italic text |
| `Ctrl+U` | Underline text |

### In Study Mode

| Keys | Action |
|------|--------|
| `Space` | Show Answer / Rate as "Good" |

---

## 11. Frequently Asked Questions

**Where is my data stored?**  
All data is stored in the `.flcr` file you specified when creating the project. The application does not send any data to the internet.

**Can I transfer a project to another computer?**  
Yes. Simply copy the `.flcr` file to another computer and open it via **File → Open** or by double-clicking it in Explorer (provided FlashCard Studio is installed).

**Why do some cards not appear in the study session?**  
This is normal. If you recently rated a card as "Good" or "Easy", the program has scheduled the next review several days in the future. Come back later — the card will appear when the time is right.

**What happens if I press "Again"?**  
The card will be placed back at the end of the current session queue — you will see it again today. Its interval will also be reset to 1 day.

**Can I add formatting (bold, italic) to cards?**  
Yes. In the editor, select the text you want to format and click the **B**, *I*, or <u>U</u> buttons above the input field, or use the keyboard shortcuts `Ctrl+B`, `Ctrl+I`, `Ctrl+U`.

**How do I delete a deck?**  
Hover over the deck in the sidebar. Action buttons will appear — click the delete icon. Confirm the action in the dialog that appears.

**What does the number under the Again / Hard / Good / Easy buttons mean?**  
It's a preview — showing how many days until the card appears again if you choose that rating. For example, "15 d." means the card will return in 15 days.

**The application closed unexpectedly — did I lose my data?**  
No. Auto-save triggers every 3 minutes. At most, you may lose changes made within the last 3 minutes before the unexpected close. For extra safety, use `Ctrl+S` frequently.
