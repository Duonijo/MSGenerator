//
// Created by admin on 02/07/2020.
//

#include "SpringInitializr.h"

#include <utility>
#include <curl/curl.h>
#include <iostream>

SpringInitializr::SpringInitializr() = default;

void SpringInitializr::downloadFile() {
    CURL *curl_handle;
    char* projectName = const_cast<char *>(this->getName().c_str());
    const char* pageFilename = strcat(projectName, ".zip");
    FILE *pageFile;

    std::string url = "https://start.spring.io/starter.zip?"
                        "groupId=" + this->getGroup() +
                        "&artifactId=" + this->getArtifact() +
                        "&dependencies=devtools,lombok,web";
    if(this->isEureka()){
        url.append(",cloud-eureka-server");
    } else url.append(",jpa,cloud-eureka");


    std::cout << url << std::endl;
    std::cout << &url;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    pageFile = fopen(pageFilename, "wb");
    if(pageFile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pageFile);
        curl_easy_perform(curl_handle);
        fclose(pageFile);
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

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

size_t SpringInitializr::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
