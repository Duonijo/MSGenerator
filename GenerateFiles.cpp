//
// Created by admin on 06/07/2020.
//

#include "GenerateFiles.h"
#include<algorithm>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <sys/stat.h>
#include <filesystem>

size_t GenerateFiles::writeData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
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
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeData);
    FILE *pageFile;
    pageFile = fopen((springInitializr->getDestination()+"/"+ springInitializr->getProjectName()+"/"+springInitializr->getArtifact()+"/"+pageFilename).c_str(), "wb");
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
    } else if (springInitializr.isZuul()){
        std::string delimiter = "-";
        std::vector<std::string> microservices = findMicroservicesInProject(springInitializr);
        outfile << "server.port=  8762\n"
                   "spring.application.name = zuul-server\n"
                   "eureka.client.service-url.default-zone = http://localhost:8761/eureka/\n"
                   "zuul.ignored-services=*\n"
                   "\n"
                   "\n"
                   "zuul.ribbon.eager-load.enabled= true\n"
                   "ribbon.ReadTimeout=60000"
                   "\n"
                   "\n";
        for(const std::string &ms : microservices){
            outfile << "zuul.routes." << ms << ".service-id = " <<ms <<"\n"
                       "zuul.routes." << ms << ".path = /" << ms.substr(ms.find(delimiter)+1, ms.length())<< "/**\n\n";
        }
    }

    else {
        outfile << "spring.application.name = "+ springInitializr.getArtifact()+"\neureka.client.serviceUrl.defaultZone = http://localhost:8761/eureka\nserver.port = ";
        outfile << "spring.datasource.url=jdbc:mariadb://localhost:3306/\n"
                   "spring.datasource.username=\n"
                   "spring.datasource.password=\n"
                   "spring.datasource.driver-class-name=org.mariadb.jdbc.Driver\n";
        outfile << "spring.jpa.show-sql=true\n"
                   "spring.jpa.hibernate.ddl-auto=update\n"
                   "spring.jpa.properties.hibernate.dialect=org.hibernate.dialect.MariaDB103Dialect\n"
                   "spring.jpa.hibernate.naming.implicit-strategy=org.hibernate.boot.model.naming.ImplicitNamingStrategyLegacyHbmImpl\n"
                   "spring.jpa.hibernate.naming.physical-strategy=org.springframework.boot.orm.jpa.hibernate.SpringPhysicalNamingStrategy\n"
                   "spring.jpa.open-in-view=false";
    }
}

std::vector<std::string> GenerateFiles::findMicroservicesInProject(SpringInitializr &springInitializr) {
    std::vector<std::string> microservices;
    std::string path =springInitializr.getDestination()+"/"+springInitializr.getProjectName()+"/";
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        std::string ms = entry.path().generic_string().erase(0, path.length());
        if(!((ms.find("eureka") != std::string::npos)||(ms.find("zuul") != std::string::npos))){
            microservices.push_back(ms);
        }
    }
    return microservices;
}

void GenerateFiles::addPropertiesToZuul(SpringInitializr &springInitializr) {
    std::string delimiter = "-";
    const std::string& ms = springInitializr.getArtifact();
    std::string path = springInitializr.getDestination()+"/"+springInitializr.getProjectName()+"/"+springInitializr.getZuulPath() + "/" + "src/main/resources/application.properties";
    std::ofstream outfile;

    outfile.open(path, std::ios_base::app); // append instead of overwrite
    outfile << "zuul.routes." << ms << ".service-id = " << ms <<"\n"
               "zuul.routes." << ms << ".path = /" << ms.substr(ms.find(delimiter)+1, ms.length())<< "/**\n\n";
}

void GenerateFiles::generateMicroservice(SpringInitializr &springInitializr) {

    std::string art = springInitializr.getArtifact();
    art.erase(remove(art.begin(), art.end(), '-'), art.end());
    std::string initPackage = "cd src/main/java/com/"+springInitializr.getGroup()+"/"+art+" && mkdir {entities,repositories,services,models,controllers}";
    std::string unzip;
    if(springInitializr.getProjectName().empty()){
        springInitializr.setProjectName("Demo");
    }

    std::string filePath = springInitializr.getDestination() + "/" + springInitializr.getProjectName()+ "/"+ springInitializr.getArtifact();
    std::string app_path = springInitializr.getDestination()+ "/" + springInitializr.getProjectName()+"/"+ springInitializr.getArtifact() +
                           "/src/main/java/com/"+springInitializr.getGroup()+"/"+art;
    int success = mkdir(filePath.c_str(), 0777);
    std::cout << "success : "<<success<<std::endl;
    downloadFile(&springInitializr);

    if (!IsPathExist(springInitializr.getProjectName())) {
        unzip = "cd " + filePath +
                " && unzip -qq " + springInitializr.getArtifact() + ".zip && rm "
                + springInitializr.getArtifact() + ".zip && " + initPackage;
    } else {
        unzip = "cd " + filePath +
                " && unzip -qq " + springInitializr.getArtifact() + ".zip && rm "
                + springInitializr.getArtifact() + ".zip && " + initPackage;
    }
    system(unzip.c_str());
    GenerateFiles::setApplicationProperties(springInitializr);
    if(springInitializr.isEureka()){
        GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@EnableEurekaServer\n@SpringBootApplication", springInitializr);
    } else if(springInitializr.isZuul()){
        GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@SpringBootApplication\n@EnableEurekaClient\n@EnableZuulProxy", springInitializr);
    } else {
        GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@SpringBootApplication\n@EnableEurekaClient\n", springInitializr);
        GenerateFiles::addPropertiesToZuul(springInitializr);
    }
}

bool GenerateFiles::IsPathExist(const std::string &s) {
    struct stat buffer{};
    return (stat (s.c_str(), &buffer) == 0);
}

void GenerateFiles::generateClasses(SpringInitializr &springInitializr) {
    std::string art = springInitializr.getArtifact();
    art.erase(remove(art.begin(), art.end(), '-'), art.end());
    std::string path_entities = springInitializr.getDestination()+"/"+springInitializr.getProjectName()+"/"+springInitializr.getArtifact()+"/src/main/java/com/"+springInitializr.getGroup()+"/"+art+"/entities/";

    bool createClasses = true;
    std::string add;
    while (createClasses){

        std::string name;
        std::string tableName;
        std::string generateTableAnswer;
        bool generateTable;

        std::cout << "Add entities to project ? y / [n] \t";
        std::cin >> add;
        if( add == "y" || add == "yes" || add == "Y" || add == "YES"){
            std::ofstream outfile;
            std::cout << "Entity name : \t";
            std::cin >> name;

            std::cout << "Generate table ? : y / [n] \t";
            std::cin >> generateTableAnswer;
            generateTable = generateTableAnswer == "y" || generateTableAnswer == "yes" ||
                            generateTableAnswer == "Y" || generateTableAnswer == "YES";

            outfile.open(path_entities+name+".java", std::ofstream::binary);

            if(generateTable){
                std::cout << "Table name : \t";
                std::cin >> tableName;

                outfile << "package com."
                << springInitializr.getGroup()<<"."<<art<<".entities;\n"
                "\n"
                "import lombok.AllArgsConstructor;\n"
                "import lombok.Builder;\n"
                "import lombok.Data;\n"
                "import lombok.NoArgsConstructor;\n"
                "\n"
                "import javax.persistence.*;\n"
                "\n"
                "@Data\n"
                "@Entity\n"
                "@Table(name = \""<< tableName <<"\")\n"
                "@AllArgsConstructor\n"
                "@NoArgsConstructor\n"
                "@Builder\n"
                "public class "<< name << " {\n"
                "\n"
                "    @Id\n"
                "    @GeneratedValue(strategy = GenerationType.IDENTITY)\n"
                "    private Long id;\n\n"
                "}";
            } else {
                outfile << "package com." << springInitializr.getGroup()<<"."<<art<<".entities;\n"
                      "\n"
                      "import lombok.AllArgsConstructor;\n"
                      "import lombok.Builder;\n"
                      "import lombok.Data;\n"
                      "import lombok.NoArgsConstructor;\n"
                      "\n"
                      "@Data\n"
                      "@AllArgsConstructor\n"
                      "@NoArgsConstructor\n"
                      "@Builder\n"
                      "public class "<< name << " {\n""\n""}";
            }

            outfile.close();
            std::cout << name << " created.\n\n";
        } else {
            createClasses = false;
        }
    }

}

