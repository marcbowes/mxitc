/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** Class for styling the main Chat window.
**
****************************************************************************/

#ifndef __MXIT_GUI_THEMECOMPONENTS_CHAT_H__
#define __MXIT_GUI_THEMECOMPONENTS_CHAT_H__

#include <QDir>
#include <QHash>
#include <QPixmap>
#include <QString>

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

class Chat
{
  public:         /* class specific */

  Chat()  {}
  ~Chat() {}

  public:         /* methods */
  
  void load(QDir theme);
  void loadDefaults();

  public:         /* variables */
  
  QHash<QString, QPixmap>
            emoticons;
  QPixmap   group;
  QString   stylesheet;
  QPixmap   unread;
};

}

}

}

#endif /* __MXIT_GUI_THEMECOMPONENTS_CHAT_H__ */

