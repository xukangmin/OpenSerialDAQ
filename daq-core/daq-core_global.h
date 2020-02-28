#ifndef DAQCORE_GLOBAL_H
#define DAQCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DAQCORE_LIBRARY)
#  define DAQCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define DAQCORESHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // DAQCORE_GLOBAL_H
