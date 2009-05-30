/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QStringList>

#include "emoticon.h"

namespace MXit
{

namespace GUI
{

namespace Emoticon
{

/****************************************************************************
**
** Author: Marc Bowes
**
** spoken -> shorthand translation.
**
****************************************************************************/
QString spokenToShorthand(QString spoken)
{
  static Dictionary dictionary;
  static bool dictionaryLoaded (false);
  
  /* build dictionary if it wasn't previously built */
  if (!dictionaryLoaded) {
    /* ordered alphabetically by spoken */
    dictionary["leftbracket"]   = '(';
    dictionary["rightbracket"]  = ')';
    dictionary["semicolon"]     = ':';
    
    dictionaryLoaded = true;
  }
  
  /* do translation */
  QString shorthand;
  Q_FOREACH(const QString &string, spoken.split(".")) {
    if (!dictionary.contains(string))
      return QString(); /* no match */
    else
      shorthand.append(dictionary.value(string));
  }
  
  /* returns ":)" for "semicolon.rightbracket" */
  return shorthand;
}

}

}

}

