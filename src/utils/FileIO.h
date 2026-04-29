#pragma once

#include <QString>
#include <memory>

class Project;

// Static utility: serialize/deserialize .flcr files (UTF-8 JSON via QJsonDocument)
class FileIO
{
public:
    FileIO() = delete;

    static bool                    save(const Project& project, const QString& path);
    static std::unique_ptr<Project> load(const QString& path, QString& errorMsg);
};
