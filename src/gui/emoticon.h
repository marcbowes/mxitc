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

  Emoticon(const QString &shorthand, const QString &spoken, const QPixmap &pixmap);
  ~Emoticon() {}
  
  static void HtmlToShorthand(QString &message);

  public:         /* methods */
  
  void shorthandToHtml(QString &message, const QString &path);
  
  public:         /* variables */
  
  QPixmap pixmap;
  QString shorthand;
  QString spoken;
};

}

}

#endif /* __MXIT_GUI_EMOTICON_H__ */

