#ifndef POIFS_GLOBAL_H
#define POIFS_GLOBAL_H

//#include <QtCore/qglobal.h>

//#if defined(POIFS_LIBRARY)
//#  define POIFSSHARED_EXPORT Q_DECL_EXPORT
//#else
//#  define POIFSSHARED_EXPORT Q_DECL_IMPORT
//#endif

#define POIFS_GETTER(varName, type) \
    type varName() const { \
        return m_##varName; \
    }

#define POIFS_SETTER(varName, type) \
    void set_##varName(type varName) { \
        m_##varName = varName; \
    }

#define POIFS_GETTER_SETTER(varName, type) \
    POIFS_GETTER(varName, type) \
    POIFS_SETTER(varName, type)

#endif // POIFS_GLOBAL_H
