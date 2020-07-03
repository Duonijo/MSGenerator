#include <iostream>
#include <curl/curl.h>
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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
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
                springInitializr.setName(argv[++i]);
            }
        }
    }

    springInitializr.downloadFile();

    std::cout << springInitializr.toString() << std::endl;
    std::cout << "FINI" << std::endl;
    return 0;
}
