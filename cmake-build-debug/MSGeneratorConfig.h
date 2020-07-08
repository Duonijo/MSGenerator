# define MSGenerator_VERSION_MAJOR 0
# define MSGenerator_VERSION_MINOR 1

#if defined(__linux) || defined(__linux__) || defined(linux)
# define LINUX

#elif defined(__APPLE__)
# define MACOS

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
# define WINDOWS

#endif
