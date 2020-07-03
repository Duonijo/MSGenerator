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
                        "&dependencies=jpa,devtools,lombok,web";
    if(this->isEureka()){
        url.append(",cloud-eureka-server");
    } else url.append(",cloud-eureka");


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


    const char *archive;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
    int err;
    int i, len;
    int fd;
    long long sum;
    prg = argv[0];
    if (argc != 2) {
        fprintf(stderr, "usage: %s archive/n", prg);
        return 1;
    }

    archive = argv[1];
    if ((za = zip_open(archive, 0, &err)) == NULL) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "%s: can't open zip archive `%s': %s/n", prg,
                archive, buf);
        return 1;
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            printf("==================/n");
            len = strlen(sb.name);
            printf("Name: [%s], ", sb.name);
            printf("Size: [%llu], ", sb.size);
            printf("mtime: [%u]/n", (unsigned int)sb.mtime);
            if (sb.name[len - 1] == '/') {
                safe_create_dir(sb.name);
            } else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    fprintf(stderr, "boese, boese/n");
                    exit(100);
                }

                fd = open(sb.name, O_RDWR | O_TRUNC | O_CREAT, 0644);
                if (fd < 0) {
                    fprintf(stderr, "boese, boese/n");
                    exit(101);
                }

                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        fprintf(stderr, "boese, boese/n");
                        exit(102);
                    }
                    write(fd, buf, len);
                    sum += len;
                }
                close(fd);
                zip_fclose(zf);
            }
        } else {
            printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1) {
        fprintf(stderr, "%s: can't close zip archive `%s'/n", prg, archive);
        return 1;
    }
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
