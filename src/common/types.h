/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** defines some types for use with Qt template holders
**
****************************************************************************/

#ifndef __MXIT_TYPES_H__
#define __MXIT_TYPES_H__

#include <QString>

/****************************************************************************
**
** Author: Marc Bowes
**
** QVector defines
**
****************************************************************************/

#include <QVector>
#include <QVectorIterator>

namespace MXit
{

/* QVector for QString's */
typedef QVector         <QString>             StringVec;
typedef QVectorIterator <QString>             StringVecItr;

/* QVector for QByteArray's */
typedef QVector         <QByteArray>          ByteArrayVec;
typedef QVectorIterator <QByteArray>          ByteArrayVecItr;

}


/****************************************************************************
**
** Author: Marc Bowes
**
** QHash defines
**
****************************************************************************/

#include <QHash>
#include <QHashIterator>

namespace MXit
{

/* QHash of QString => QByteArray */
typedef QHash         <QString, QByteArray>   VariableHash;
typedef QHashIterator <QString, QByteArray>   VariableHashItr;
typedef QHash         <QString, QByteArray>::const_iterator
                                              VariableHashConstItr;

}

#endif

