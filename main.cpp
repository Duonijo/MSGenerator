#include <iostream>
#include "SpringInitializr.h"
#include<algorithm>
#include <sys/stat.h>
#include <fstream>
#include "GenerateFiles.h"



static bool IsPathExist(const std::string &s)
{
    struct stat buffer{};
    return (stat (s.c_str(), &buffer) == 0);
}

static void show_usage(const std::string& name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-d,--destination\t\tSpecify project destination\n"
              << "\t-g,--group\t\tSpecify the group name\n"
              << "\t-a,--artifact\t\tSpecify the artifact name\n"
              << "\t-e,--eureka\t\tSpecify if micro service is Eureka Server\n"
              << "\t-z,--zuul\t\tSpecify if micro service is Zuul Server\n"
              << std::endl;
}


int main(int argc, char *argv[]) {

    SpringInitializr springInitializr;


    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "--help") || (arg == "-h")) {
            show_usage(argv[0]);
            return 0;
        }
        else if ( arg == "--group" || arg == "-g") {
            if (i + 1 < argc) {
                springInitializr.setGroup(argv[++i]);
            }
        }
        else if (arg == "--artifact" || arg == "-a") {
            if (i + 1 < argc) {
                springInitializr.setArtifact(argv[++i]);
            }
        }
        else if (arg == "--eureka" || arg == "-e") {
            springInitializr.setEureka(true);
        }
        else if (arg == "--zuul" || arg == "-z"){
            springInitializr.setZuul(true);
        }

        else if (arg == "--name" || arg == "-n"){
            if (i + 1 < argc) {
                springInitializr.setProjectName(argv[++i]);
            }
        }

        else if (arg == "--destination" || arg == "-d"){
            if (i + 1 < argc) {
                springInitializr.setDestination(argv[++i]);
            }
        }
    }

    if(!springInitializr.getDestination().empty()){
        GenerateFiles::downloadFile(&springInitializr);

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
        if (!IsPathExist(springInitializr.getProjectName())) {
            unzip = "mkdir -p " + filePath +
                    " && mv " + springInitializr.getArtifact() + ".zip " + filePath + " && cd " + filePath +
                    " && unzip -qq " + springInitializr.getArtifact() + ".zip && rm "
                    + springInitializr.getArtifact() + ".zip && " + initPackage;
        } else {
            unzip = "mkdir -p " + filePath +
                    " && mv " + springInitializr.getArtifact() + ".zip " + filePath + " && cd " + filePath +
                    " && unzip -qq " + springInitializr.getArtifact() + ".zip && rm "
                    + springInitializr.getArtifact() + ".zip && " + initPackage;
        }
        system(unzip.c_str());

        if(springInitializr.isEureka()){
            GenerateFiles::setApplicationProperties(springInitializr);
            GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@EnableEurekaServer\n@SpringBootApplication", springInitializr);
        } else if(springInitializr.isZuul()){
            GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@SpringBootApplication\n@EnableEurekaClient\n@EnableZuulProxy", springInitializr);
        } else {
            GenerateFiles::insertAnnotation(app_path, "DemoApplication", "@SpringBootApplication\n@EnableEurekaClient\n", springInitializr);
        }
        std::cout << "MICROSERVICE : " << springInitializr.getArtifact() << " has been create." << std::endl;
        return 0;
    }
    else {
        std::cout << "Specify destination" << std::endl;
        return 0;
    }

}
