//
// Created by admin on 06/07/2020.
//

#ifndef MSGENERATOR_GENERATEFILES_H
#define MSGENERATOR_GENERATEFILES_H
#include<algorithm>
#include <vector>
#include "SpringInitializr.h"


class GenerateFiles {
public:
    static size_t writeData(void *ptr, size_t size, size_t nmemb, FILE *stream);

    static void insertAnnotation(const std::string &path, const std::string &name, const std::string &annotations, SpringInitializr &springInitializr);

    static void renameFile(const std::string &path, const std::string &name, SpringInitializr &springInitializr);

    static void downloadFile( SpringInitializr *springInitializr);

    static void insertImport(SpringInitializr &springInitializr, std::string &strTemp);

    static void setApplicationProperties(SpringInitializr &springInitializr);

    static std::vector<std::string> findMicroservicesInProject(SpringInitializr &springInitializr);

    static void addPropertiesToZuul(SpringInitializr &springInitializr);

    static void generateMicroservice(SpringInitializr &springInitializr);

    static void generateClasses(SpringInitializr &springInitializr);

    static bool IsPathExist(const std::string &s);

    static void generatePackages(SpringInitializr &springInitializr);


};


#endif //MSGENERATOR_GENERATEFILES_H
