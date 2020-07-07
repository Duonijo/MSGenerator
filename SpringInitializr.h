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
    std::string projectName;
    std::string description;
    std::string packageName;
    std::string destination;

    std::string zuulPath;
public:
    const std::string &getZuulPath() const;

    void setZuulPath(const std::string &zuulPath);

private:

    int javaVersion = 8;

    bool eureka{};
    bool zuul{};

public:
    const std::string &getProjectName() const;

    void setProjectName(const std::string &projectName);

    const std::string &getDestination() const;

    void setDestination(const std::string &destination);

    bool isEureka() const;

    void setEureka(bool eureka);

    bool isZuul() const;

    void setZuul(bool zuul);

    std::string toString() const;

    SpringInitializr();

    const std::string &getGroup() const;

    void setGroup(const std::string &group);

    const std::string &getArtifact() const;

    void setArtifact(const std::string &artifact);


    const std::string &getDescription() const;

    void setDescription(const std::string &description);

    const std::string &getPackageName() const;

    void setPackageName(const std::string &packageName);

    int getJavaVersion() const;

    void setJavaVersion(int javaVersion);

};


#endif //MSGENERATOR_SPRINGINITIALIZR_H
