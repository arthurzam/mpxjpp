#ifndef POIFS_GLOBAL_H
#define POIFS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(POIFS_LIBRARY)
#  define POIFSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POIFSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // POIFS_GLOBAL_H
