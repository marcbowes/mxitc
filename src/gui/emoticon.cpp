/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

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
  static bool dictionaryLoaded;
  
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
  Q_FOREACH(const QChar &character, spoken) {
    if (!dictionary.contains(character))
      return QString(); /* no match */
    else
      shorthand.append(dictionary.value(character));
  }
  
  /* returns ":)" for "semicolon.rightbracket" */
  return shorthand;
}

}

}

}

