#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QCUSTOMUI_LIB)
#  define QCUSTOMUI_EXPORT Q_DECL_EXPORT
# else
#  define QCUSTOMUI_EXPORT Q_DECL_IMPORT
# endif
#else
# define QCUSTOMUI_EXPORT
#endif
