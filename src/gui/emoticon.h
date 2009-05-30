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

namespace MXit
{

namespace GUI
{

namespace Emoticon
{
  #define shorthandToHTML(x)    QString("<img alt=\"%1\" src=\"%2%1.png\"").arg(x).arg(emoticonDir);
  
  typedef QHash<QString, QString> WordHash;
  
  static WordHash shorthandToSpoken(QString shorthand);
};

}

}

#endif /* __MXIT_GUI_EMOTICON_H__ */

