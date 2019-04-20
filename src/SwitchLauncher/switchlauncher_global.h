#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SWITCHLAUNCHER_LIB)
#  define SWITCHLAUNCHER_EXPORT Q_DECL_EXPORT
# else
#  define SWITCHLAUNCHER_EXPORT Q_DECL_IMPORT
# endif
#else
# define SWITCHLAUNCHER_EXPORT
#endif
