#ifndef LIBCONTROL_GLOBAL_H
#define LIBCONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBCONTROL_LIBRARY)
#  define LIBCONTROL_EXPORT Q_DECL_EXPORT
#else
#  define LIBCONTROL_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBCONTROL_GLOBAL_H
