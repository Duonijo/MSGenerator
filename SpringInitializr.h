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
    std::string destination;
public:
    const std::string &getDestination() const;

    void setDestination(const std::string &destination);

private:
    int javaVersion = 8;

    bool eureka{};
    bool zuul{};

public:
    bool isEureka() const;

    void setEureka(bool eureka);

    bool isZuul() const;

    void setZuul(bool zuul);


public:
    void downloadFile();

    std::string toString() const;

    SpringInitializr();

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

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

};


#endif //MSGENERATOR_SPRINGINITIALIZR_H
