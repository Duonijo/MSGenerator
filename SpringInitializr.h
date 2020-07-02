//
// Created by admin on 02/07/2020.
//

#ifndef MSGENERATOR_SPRINGINITIALIZR_H
#define MSGENERATOR_SPRINGINITIALIZR_H


#include <string>

class SpringInitializr {

private:
    std::string group;
    std::string artifact;
    std::string name;
    std::string description;
    std::string packageName;
    int javaVersion = 8;

public:
    void downloadFile();

    SpringInitializr();

    SpringInitializr(const std::string &group, const std::string &artifact);

    SpringInitializr(std::string group, std::string artifact, const std::string &name,
                     std::string description, const std::string &packageName, int javaVersion);

    SpringInitializr(std::string group, const std::string &artifact, std::string name,
                     std::string packageName, int javaVersion);

    const std::string &getGroup() const;

    void setGroup(const std::string &group);

    const std::string &getArtifact() const;

    void setArtifact(const std::string &artifact);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getDescription() const;

    void setDescription(const std::string &description);

    const std::string &getPackageName() const;

    void setPackageName(const std::string &packageName);

    int getJavaVersion() const;

    void setJavaVersion(int javaVersion);
};


#endif //MSGENERATOR_SPRINGINITIALIZR_H
