#pragma once

#include <QObject>
#include <QColor>

enum class AppTheme { Light, Dark };

// Singleton that owns the application-wide visual theme.
// Uses Qt Fusion style + custom QPalette so the look is fully independent of
// the host OS dark-mode setting.
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager& instance();

    AppTheme currentTheme() const { return m_theme; }

    // Call once (in main()) after QApplication is created but before any
    // windows are constructed.  Also called internally by setTheme().
    void applyCurrentTheme();

    // Change the theme, persist the choice, re-apply, and emit themeChanged.
    void setTheme(AppTheme theme);

    // ── Per-theme colour helpers used by widgets with inline stylesheets ──
    QColor mutedTextColor()            const;
    QColor cardBackground()            const;
    QColor cardBorder()                const;
    QColor showAnswerHoverBackground() const;
    QColor hoverButtonColor()          const;

signals:
    void themeChanged(AppTheme newTheme);

private:
    explicit ThemeManager();
    void applyPalette();

    AppTheme m_theme{AppTheme::Light};
};
