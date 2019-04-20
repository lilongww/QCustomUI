#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MAINPAGEACTIVITY_LIB)
#  define MAINPAGEACTIVITY_EXPORT Q_DECL_EXPORT
# else
#  define MAINPAGEACTIVITY_EXPORT Q_DECL_IMPORT
# endif
#else
# define MAINPAGEACTIVITY_EXPORT
#endif
