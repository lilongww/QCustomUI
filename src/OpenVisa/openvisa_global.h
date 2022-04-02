#pragma

#ifdef BUILD_OPENVISA_STATIC
#define OPENVISA_EXPORT
#else
#ifdef WIN32
#ifdef OPENVISA_LIB
#define OPENVISA_EXPORT __declspec(dllexport)
#else
#define OPENVISA_EXPORT __declspec(dllimport)
#endif
#endif
#endif
