#include <iostream>
#include "SpringInitializr.h"
#include<algorithm>
#include "GenerateFiles.h"


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
              << "\t-u,--update\t\tUpdate Zuul properties\n"
              <<"\n\nDatabases properties are set by Default with MariaDB\n"
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

        else if (arg == "--update" || arg == "-u"){
            if (i + 1 < argc) {
                springInitializr.setZuulPath(argv[++i]);
            }
        }
    }

    if(!springInitializr.getDestination().empty()){
        GenerateFiles::generateMicroservice(springInitializr);
        GenerateFiles::generateClasses(springInitializr);

        std::cout << "MICROSERVICE : " << springInitializr.getArtifact() << " has been create." << std::endl;
        return 0;
    }
    else {
        std::cout << "Specify destination" << std::endl;
        return 0;
    }

}
