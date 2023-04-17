#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QT_PLAYER_LIB)
#  define QT_PLAYER_EXPORT Q_DECL_EXPORT
# else
#  define QT_PLAYER_EXPORT Q_DECL_IMPORT
# endif
#else
# define QT_PLAYER_EXPORT
#endif

//https://github.com/DragonPang/QtPlayer