/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** contact presence info
**
****************************************************************************/


#ifndef __MXIT_GUI_THEMECOMPONENTS_CONTACT_TYPE_H__
#define __MXIT_GUI_THEMECOMPONENTS_CONTACT_TYPE_H__

#include <QDir>
#include <QPixmap>

#include "protocol/enumerables/contact.h"

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

namespace Contact
{

class Type
{
  public:         /* class specific */
  
  Type()  {}
  ~Type() {}
  
  public:         /* methods */
  
  void load(const QDir &dir);
  void loadDefaults();
  QPixmap pixmap(Protocol::Enumerables::Contact::Type type);
  
  private:        /* variables */
  
  QPixmap aim,
          bot,
          chat_room,
          gallery,
          google_talk,
          icq,
          info,
          jabber,
          msn,
          multimx,
          mxit,
          yahoo,
  ;
};

}

}

}

}

#endif /* __MXIT_GUI_THEMECOMPONENTS_CONTACT_TYPE_H__ */

