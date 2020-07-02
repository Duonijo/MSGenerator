//
// Created by admin on 02/07/2020.
//

#include "SpringInitializr.h"

#include <utility>

SpringInitializr::SpringInitializr(std::string group, std::string artifact, const std::string &name,
                                   std::string description, const std::string &packageName, int javaVersion) : group(std::move(
        group)), artifact(std::move(artifact)), name(name), description(std::move(description)), packageName(packageName), javaVersion(javaVersion) {}

SpringInitializr::SpringInitializr(std::string group, const std::string &artifact, std::string name,
                                   std::string packageName, int javaVersion) : group(std::move(group)), artifact(artifact),
                                                                                      name(std::move(name)),
                                                                                      packageName(std::move(packageName)),
                                                                                      javaVersion(javaVersion) {}

SpringInitializr::SpringInitializr() = default;

void SpringInitializr::downloadFile() {

}

SpringInitializr::SpringInitializr(const std::string &group, const std::string &artifact) : group(group),
                                                                                            artifact(artifact) {}

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

const std::string &SpringInitializr::getName() const {
    return name;
}

void SpringInitializr::setName(const std::string &name) {
    SpringInitializr::name = name;
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
