#include "ProjectManager.h"
#include "../utils/FileIO.h"

#include <QDateTime>

ProjectManager& ProjectManager::instance()
{
    static ProjectManager inst;
    return inst;
}

ProjectManager::ProjectManager(QObject* parent)
    : QObject(parent)
{
    m_autosaveTimer = new QTimer(this);
    m_autosaveTimer->setInterval(kAutosaveIntervalMs);
    connect(m_autosaveTimer, &QTimer::timeout, this, &ProjectManager::autosave);
}

bool ProjectManager::newProject(const QString& filePath, const QString& projectName)
{
    auto proj = std::make_unique<Project>();
    proj->projectName = projectName;
    proj->lastModified = QDateTime::currentDateTime();

    if (!FileIO::save(*proj, filePath)) {
        return false;
    }

    m_project  = std::move(proj);
    m_filePath = filePath;
    m_isModified = false;
    m_autosaveTimer->start();

    emit projectLoaded(m_filePath);
    return true;
}

bool ProjectManager::openProject(const QString& filePath)
{
    QString err;
    auto proj = FileIO::load(filePath, err);
    if (!proj) {
        return false;
    }

    m_project  = std::move(proj);
    m_filePath = filePath;
    m_isModified = false;
    m_autosaveTimer->start();

    emit projectLoaded(m_filePath);
    return true;
}

bool ProjectManager::saveProject()
{
    if (!m_project || m_filePath.isEmpty()) return false;

    m_project->lastModified = QDateTime::currentDateTime();
    if (!FileIO::save(*m_project, m_filePath)) return false;

    m_isModified = false;
    return true;
}

bool ProjectManager::saveProjectAs(const QString& filePath)
{
    if (!m_project) return false;

    m_project->lastModified = QDateTime::currentDateTime();
    if (!FileIO::save(*m_project, filePath)) return false;

    m_filePath   = filePath;
    m_isModified = false;
    emit projectLoaded(m_filePath);
    return true;
}

void ProjectManager::closeProject()
{
    m_autosaveTimer->stop();
    m_project.reset();
    m_filePath.clear();
    m_isModified = false;
    emit projectClosed();
}

void ProjectManager::markModified()
{
    if (!m_isModified) {
        m_isModified = true;
        emit projectModified();
    }
}

void ProjectManager::autosave()
{
    if (m_isModified && m_project && !m_filePath.isEmpty()) {
        saveProject();
    }
}
