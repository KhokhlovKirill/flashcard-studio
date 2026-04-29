#include "ThemeManager.h"

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QSettings>

ThemeManager::ThemeManager()
{
    QSettings s;
    const int stored = s.value("theme", static_cast<int>(AppTheme::Light)).toInt();
    m_theme = static_cast<AppTheme>(stored);
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

void ThemeManager::applyCurrentTheme()
{
    applyPalette();
}

void ThemeManager::setTheme(AppTheme theme)
{
    if (m_theme == theme) return;
    m_theme = theme;
    QSettings().setValue("theme", static_cast<int>(theme));
    applyPalette();
    emit themeChanged(theme);
}

void ThemeManager::applyPalette()
{
    // Fusion ignores Windows accent colours and OS dark-mode completely.
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette p;
    if (m_theme == AppTheme::Dark) {
        p.setColor(QPalette::Window,          QColor(0x2b, 0x2b, 0x2b));
        p.setColor(QPalette::WindowText,      QColor(0xe0, 0xe0, 0xe0));
        p.setColor(QPalette::Base,            QColor(0x1e, 0x1e, 0x1e));
        p.setColor(QPalette::AlternateBase,   QColor(0x2d, 0x2d, 0x2d));
        p.setColor(QPalette::ToolTipBase,     QColor(0x3c, 0x3c, 0x3c));
        p.setColor(QPalette::ToolTipText,     QColor(0xe0, 0xe0, 0xe0));
        p.setColor(QPalette::Text,            QColor(0xe0, 0xe0, 0xe0));
        p.setColor(QPalette::Button,          QColor(0x3c, 0x3c, 0x3c));
        p.setColor(QPalette::ButtonText,      QColor(0xe0, 0xe0, 0xe0));
        p.setColor(QPalette::BrightText,      QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::Link,            QColor(0x42, 0x9c, 0xf0));
        p.setColor(QPalette::Highlight,       QColor(0x19, 0x76, 0xd2));
        p.setColor(QPalette::HighlightedText, QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::Dark,            QColor(0x1a, 0x1a, 0x1a));
        p.setColor(QPalette::Mid,             QColor(0x38, 0x38, 0x38));
        p.setColor(QPalette::Midlight,        QColor(0x40, 0x40, 0x40));
        p.setColor(QPalette::Light,           QColor(0x45, 0x45, 0x45));
        p.setColor(QPalette::Shadow,          QColor(0x0d, 0x0d, 0x0d));
        p.setColor(QPalette::PlaceholderText, QColor(0x75, 0x75, 0x75));
    } else {
        p.setColor(QPalette::Window,          QColor(0xf5, 0xf5, 0xf5));
        p.setColor(QPalette::WindowText,      QColor(0x21, 0x21, 0x21));
        p.setColor(QPalette::Base,            QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::AlternateBase,   QColor(0xf0, 0xf0, 0xf0));
        p.setColor(QPalette::ToolTipBase,     QColor(0xff, 0xff, 0xe0));
        p.setColor(QPalette::ToolTipText,     QColor(0x21, 0x21, 0x21));
        p.setColor(QPalette::Text,            QColor(0x21, 0x21, 0x21));
        p.setColor(QPalette::Button,          QColor(0xe8, 0xe8, 0xe8));
        p.setColor(QPalette::ButtonText,      QColor(0x21, 0x21, 0x21));
        p.setColor(QPalette::BrightText,      QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::Link,            QColor(0x19, 0x76, 0xd2));
        p.setColor(QPalette::Highlight,       QColor(0x19, 0x76, 0xd2));
        p.setColor(QPalette::HighlightedText, QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::Dark,            QColor(0xbd, 0xbd, 0xbd));
        p.setColor(QPalette::Mid,             QColor(0xc8, 0xc8, 0xc8));
        p.setColor(QPalette::Midlight,        QColor(0xe0, 0xe0, 0xe0));
        p.setColor(QPalette::Light,           QColor(0xff, 0xff, 0xff));
        p.setColor(QPalette::Shadow,          QColor(0x75, 0x75, 0x75));
        p.setColor(QPalette::PlaceholderText, QColor(0x9e, 0x9e, 0x9e));
    }

    QApplication::setPalette(p);
}

// ── Per-theme colour helpers ──────────────────────────────────────────────────

QColor ThemeManager::mutedTextColor() const
{
    return m_theme == AppTheme::Dark
        ? QColor(0x75, 0x75, 0x75)
        : QColor(0x9e, 0x9e, 0x9e);
}

QColor ThemeManager::cardBackground() const
{
    return m_theme == AppTheme::Dark
        ? QColor(0x2d, 0x2d, 0x2d)
        : QColor(0xff, 0xff, 0xff);
}

QColor ThemeManager::cardBorder() const
{
    return m_theme == AppTheme::Dark
        ? QColor(0x44, 0x44, 0x44)
        : QColor(0xe0, 0xe0, 0xe0);
}

QColor ThemeManager::showAnswerHoverBackground() const
{
    return m_theme == AppTheme::Dark
        ? QColor(25, 118, 210, 38)   // subtle blue tint
        : QColor(0xe3, 0xf2, 0xfd);  // #E3F2FD
}

QColor ThemeManager::hoverButtonColor() const
{
    return m_theme == AppTheme::Dark
        ? QColor(255, 255, 255, 30)
        : QColor(0, 0, 0, 25);
}
