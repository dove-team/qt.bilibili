#ifndef QT_BILIBILI_LIBS_GLOBAL_H
#define QT_BILIBILI_LIBS_GLOBAL_H

#include <QtCore/qglobal.h>

# if defined(QT_BILIBILI_LIBS_LIBRARY)
#  define QT_BILIBILI_LIBS_EXPORT Q_DECL_EXPORT
# else
#  define QT_BILIBILI_LIBS_EXPORT Q_DECL_IMPORT
# endif

#endif