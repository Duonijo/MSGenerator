//
// Created by admin on 02/07/2020.
//

#include "SpringInitializr.h"

#include <utility>
#include <curl/curl.h>

SpringInitializr::SpringInitializr() = default;


const std::string &SpringInitializr::getGroup() const {
    return group;
}

void SpringInitializr::setGroup(const std::string &group) {
    SpringInitializr::group = group;
}

const std::string &SpringInitializr::getArtifact() const {
    return artifact;
}

void SpringInitializr::setArtifact(const std::string &artifact) {
    SpringInitializr::artifact = artifact;
}

const std::string &SpringInitializr::getDescription() const {
    return description;
}

void SpringInitializr::setDescription(const std::string &description) {
    SpringInitializr::description = description;
}

const std::string &SpringInitializr::getPackageName() const {
    return packageName;
}

void SpringInitializr::setPackageName(const std::string &packageName) {
    SpringInitializr::packageName = packageName;
}

int SpringInitializr::getJavaVersion() const {
    return javaVersion;
}

void SpringInitializr::setJavaVersion(int javaVersion) {
    SpringInitializr::javaVersion = javaVersion;
}

std::string SpringInitializr::toString() const {
    return std::string("Group : " + this->getGroup() + " ; Artifact : " + this->getArtifact()
                       + " ; Eureka : " + (this->isEureka() ? "true" : "false" ) + " ; Zuul : " + (this->isZuul() ? "true":"false")
    );
}

bool SpringInitializr::isEureka() const {
    return eureka;
}

void SpringInitializr::setEureka(bool eureka) {
    SpringInitializr::eureka = eureka;
}

bool SpringInitializr::isZuul() const {
    return zuul;
}

void SpringInitializr::setZuul(bool zuul) {
    SpringInitializr::zuul = zuul;
}


const std::string &SpringInitializr::getDestination() const {
    return destination;
}

void SpringInitializr::setDestination(const std::string &destination) {
    SpringInitializr::destination = destination;
}

const std::string &SpringInitializr::getProjectName() const {
    return projectName;
}

void SpringInitializr::setProjectName(const std::string &projectName) {
    SpringInitializr::projectName = projectName;
}

const std::string &SpringInitializr::getZuulPath() const {
    return zuulPath;
}

void SpringInitializr::setZuulPath(const std::string &zuulPath) {
    SpringInitializr::zuulPath = zuulPath;
}
