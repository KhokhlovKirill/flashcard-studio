#pragma once

#include <QObject>
#include <QTimer>
#include <memory>
#include "Project.h"

// Singleton that owns the currently open Project and coordinates persistence.
// All mutating operations must go through this class so the dirty flag is set.
class ProjectManager : public QObject
{
    Q_OBJECT

public:
    static ProjectManager& instance();

    // Project lifecycle
    bool newProject(const QString& filePath, const QString& projectName);
    bool openProject(const QString& filePath);
    bool saveProject();
    bool saveProjectAs(const QString& filePath);
    void closeProject();

    // Accessors (read-only from outside)
    bool     isModified() const noexcept { return m_isModified; }
    bool     hasProject() const noexcept { return m_project != nullptr; }
    QString  filePath()   const noexcept { return m_filePath; }
    Project* project()    const noexcept { return m_project.get(); }

    // Call whenever in-memory data changes so the dirty flag is set
    void markModified();

signals:
    void projectLoaded(const QString& filePath);
    void projectClosed();
    void projectModified();
    void deckListChanged();
    void cardListChanged(const QString& deckId);

public slots:
    void autosave();

private:
    explicit ProjectManager(QObject* parent = nullptr);
    ~ProjectManager() override = default;
    ProjectManager(const ProjectManager&) = delete;
    ProjectManager& operator=(const ProjectManager&) = delete;

    std::unique_ptr<Project> m_project;
    QString m_filePath;
    bool    m_isModified{false};
    QTimer* m_autosaveTimer{nullptr};

    static constexpr int kAutosaveIntervalMs = 3 * 60 * 1000; // 3 minutes
};
