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

#include <QPixmap>

namespace MXit
{

namespace GUI
{

class Emoticon
{
  public:         /* class specific */

  Emoticon(const QString &shorthand, const QString &spoken, const QString &file);
  ~Emoticon() {}
  
  static void HtmlToShorthand(QString &message);

  public:         /* methods */
  
  void shorthandToHtml(QString &message, const QString &path) const;
  
  public:         /* variables */
  
  QString file;
  QString shorthand;
  QString spoken;
};

}

}

#endif /* __MXIT_GUI_EMOTICON_H__ */

