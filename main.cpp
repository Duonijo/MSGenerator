#include <iostream>

#include "SpringInitializr.h"


static void show_usage(const std::string& name)
{
    std::cerr << "Usage: " << name << " <option(s)> SOURCES\n"
              << "Options:\n"
              << "\t-h,--help\tShow this help message\n"
              << "\t-g,--group GROUP\tSpecify the group name\n"
              << "\t-a,--a ARTIFACT\tSpecify the artifact name\n"
              << std::endl;
}

int main(int argc, char *argv[]) {

    std::string group;
    std::string artifact;
    std::string name;
    std::string description;
    std::string packageName;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        }
        else if ( arg == "--group" || arg == "-g") {
            if (i + 1 < argc) {
                group = argv[++i];
            }
        }
        else if (arg == "--artifact" || "-a"){
            if (i + 1 < argc) {
                artifact = argv[++i];
            }
        }
    }

    SpringInitializr springInitializr(group, artifact);

    std::cout << "Object : " << springInitializr.getGroup() << " ; " << springInitializr.getArtifact() << std::endl;
    return 0;
}
