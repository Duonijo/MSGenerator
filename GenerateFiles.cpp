//
// Created by admin on 06/07/2020.
//

#include "GenerateFiles.h"
#include<algorithm>
#include <fstream>
#include <iostream>
#include <curl/curl.h>

size_t GenerateFiles::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void GenerateFiles::insertAnnotation(const std::string &path, const std::string &name, const std::string &annotations, SpringInitializr &springInitializr) {
    std::ifstream fileIn(path+"/"+name+".java"); //File to read from
    std::ofstream fileOut(path+"/"+name+"Modified.java"); //Temporary file

    bool importDone = false;
    if(!fileIn || !fileOut)
    {
        std::cout << "Error opening files!" <<  std::endl;
    }

    std::string strTemp;
    while(!fileIn.eof())
    {
        getline(fileIn, strTemp);
        if(strTemp.find("@SpringBootApplication") != std::string::npos ){
            strTemp = annotations + "\n";
        } else if(strTemp.find("import") != std::string::npos && !importDone) {
            insertImport(springInitializr, strTemp);
            importDone = true;
        } else if(strTemp.find(name) != std::string::npos){
            size_t pos = strTemp.find(name);
            if(springInitializr.isEureka()){
                strTemp.replace(pos, name.length(), "EurekaServer");
            } else if(springInitializr.isZuul()){
                strTemp.replace(pos, name.length(), "ZuulServer");
            } else {
                strTemp.replace(pos, name.length(), "MainApplication");
            }
        }
        else if(strTemp.find("public static void main") != std::string::npos && !springInitializr.isEureka() && !springInitializr.isZuul()){
            std::string restTemplate = "    @Bean\n"
                                       "    @LoadBalanced\n"
                                       "    RestTemplate getRestTemplate(){\n"
                                       "        return new RestTemplate();\n"
                                       "    }\n\n";
            std::string line = restTemplate + strTemp;
            strTemp = line;

        }
        strTemp += "\n";
        fileOut << strTemp;
    }
    std::remove((path+"/"+name+".java").c_str());
    renameFile(path, name, springInitializr);

}

void GenerateFiles::renameFile(const std::string &path, const std::string &name, SpringInitializr &springInitializr) {

    if (springInitializr.isEureka()){
        std::rename(( path+"/"+name+"Modified.java").c_str() , ((path+"/"+"EurekaServer.java").c_str()));
    } else if (springInitializr.isZuul()){
        std::rename(( path+"/"+name+"Modified.java").c_str() , ((path+"/"+"ZuulServer.java").c_str()));

    } else {
        std::rename(( path+"/"+name+"Modified.java").c_str() , ((path+"/"+"MainApplication.java").c_str()));
    }
}

void GenerateFiles::downloadFile(SpringInitializr *springInitializr) {
    CURL *curl_handle;
    char* projectName = const_cast<char *>(springInitializr->getArtifact().c_str());
    std::string pageFilename = projectName;
    pageFilename.append(".zip");

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    std::string url =
            "https://start.spring.io/starter.zip?groupId=com." + springInitializr->getGroup() + "&artifactId=" +
            springInitializr->getArtifact() + "&dependencies=devtools,lombok,web";
    url.append(springInitializr->isEureka() ? ",cloud-eureka-server" : ",jpa,cloud-eureka");
    if(springInitializr->isZuul() && !springInitializr->isEureka()){
        url.append(",cloud-zuul");
    } else if (springInitializr->isEureka() && springInitializr->isZuul()){
        std::cout << "Can't be a server zuul and eureka at once" << std::endl;
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    FILE *pageFile;
    pageFile = fopen(pageFilename.c_str(), "wb");
    if(pageFile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pageFile);
        curl_easy_perform(curl_handle);
        fclose(pageFile);
    }
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

void GenerateFiles::insertImport(SpringInitializr &springInitializr, std::string &strTemp) {
    if(springInitializr.isEureka()){
        strTemp += "\nimport org.springframework.cloud.netflix.eureka.server.EnableEurekaServer;";
    } else if (springInitializr.isZuul()){
        strTemp += "\nimport org.springframework.cloud.netflix.eureka.EnableEurekaClient;\nimport org.springframework.cloud.netflix.zuul.EnableZuulProxy;";
    } else {
        strTemp += "\nimport org.springframework.cloud.netflix.eureka.EnableEurekaClient;\nimport org.springframework.context.annotation.Bean;\n"
                   "import org.springframework.web.client.RestTemplate;\n"
                   "import org.springframework.cloud.client.loadbalancer.LoadBalanced;\n";
    }
}

void GenerateFiles::setApplicationProperties(SpringInitializr &springInitializr) {
    std::ofstream outfile;
    std::string app_properties_path =
            springInitializr.getDestination() + "/" + springInitializr.getProjectName() + "/" +
            springInitializr.getArtifact() +
            "/src/main/resources/application.properties";
    outfile.open(app_properties_path, std::ios_base::app); // append instead of overwrite

    if (springInitializr.isEureka()) {
        outfile << "spring.application.name = eureka_server\n";
        outfile << "server.port = 8761\n";
        outfile << "eureka.client.register-with-eureka = false\n";
        outfile << "eureka.client.fetch-registry = false\n";
    }

}
