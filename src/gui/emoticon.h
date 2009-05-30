/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** Class for emoticon converting between spoken, shorthand and HTML.
**
****************************************************************************/

#ifndef __MXIT_GUI_EMOTICON_H__
#define __MXIT_GUI_EMOTICON_H__

#include <QHash>
#include <QStringList>

namespace MXit
{

namespace GUI
{

namespace Emoticon
{

  #define shorthandToHTML(x) QString("<img alt=\"%1\" src=\"%2%1.png\" />").arg(x).arg(emoticonDir);
  
  typedef QHash<QString, QChar> Dictionary;
  
  /****************************************************************************
  **
  ** Author: Marc Bowes
  **
  ** spoken -> shorthand translation.
  **
  ****************************************************************************/
  static QString spokenToShorthand(QString spoken)
  {
    static Dictionary dictionary;
    static bool dictionaryLoaded (false); /* only set false first time */
    
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

#endif /* __MXIT_GUI_EMOTICON_H__ */

