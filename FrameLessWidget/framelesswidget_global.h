#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FRAMELESSWIDGET_LIB)
#  define FRAMELESSWIDGET_EXPORT Q_DECL_EXPORT
# else
#  define FRAMELESSWIDGET_EXPORT Q_DECL_IMPORT
# endif
#else
# define FRAMELESSWIDGET_EXPORT
#endif
