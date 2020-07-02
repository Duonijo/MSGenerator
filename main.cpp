#include <iostream>
#include "SpringInitializr.h"

// #include libcurl here

static void show_usage(const std::string& name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-g,--group\t\tSpecify the group name\n"
              << "\t-a,--artifact\t\tSpecify the artifact name\n"
              << "\t-e,--eureka\t\tSpecify if micro service is Eureka Server\n"
              << "\t-z,--zuul\t\tSpecify if micro service is Zuul Server\n"
              << std::endl;
}

int main(int argc, char *argv[]) {

    std::string group;
    std::string artifact;
    std::string name;
    std::string description;
    std::string packageName;
    bool isEurekaServer = false;
    bool isZuulServer = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "--help") || (arg == "-h")) {
            show_usage(argv[0]);
            return 0;
        }
        else if ( arg == "--group" || arg == "-g") {
            if (i + 1 < argc) {
                group = argv[++i];
            }
        }
        else if (arg == "--artifact" || arg == "-a") {
            if (i + 1 < argc) {
                artifact = argv[++i];
            }
        }
        else if (arg == "--eureka" || arg == "-e") {
            isEurekaServer = true;
        }
        else if (arg == "--zuul" || arg == "-z"){
            isZuulServer = true;
        }
    }
    SpringInitializr springInitializr(group, artifact);

    //call libcurl to download project from Spring Initializr

    //add directories to our new project as entities, repositories . . .
    std::cout << springInitializr.toString() << std::endl;

    /*std::cout << "Object : " << springInitializr.getGroup() << " ; " << springInitializr.getArtifact()
            << " ; Eureka Server : " << isEurekaServer
            << " ; Zuul Server : " << isZuulServer
            << std::endl;*/
    return 0;
}
