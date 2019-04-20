#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EDITACTIVITY_LIB)
#  define EDITACTIVITY_EXPORT Q_DECL_EXPORT
# else
#  define EDITACTIVITY_EXPORT Q_DECL_IMPORT
# endif
#else
# define EDITACTIVITY_EXPORT
#endif
