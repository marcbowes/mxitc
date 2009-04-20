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

/* QVector for QString's */
typedef QVector         <QString>            StringVec;
typedef QVectorIterator <QString>            StringVecItr;


/****************************************************************************
**
** Author: Marc Bowes
**
** QHash defines
**
****************************************************************************/

#include <QHash>
#include <QHashIterator>

/* QHash of QString => QByteArray */
typedef QHash         <QString, QByteArray>  VariableHash;
typedef QHashIterator <QString, QByteArray>  VariableHashItr;

#endif

