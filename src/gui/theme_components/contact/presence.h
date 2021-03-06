/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** contact presence info
**
****************************************************************************/


#ifndef __MXIT_GUI_THEMECOMPONENTS_CONTACT_PRESENCE_H__
#define __MXIT_GUI_THEMECOMPONENTS_CONTACT_PRESENCE_H__

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

class Presence
{
  public:         /* class specific */
  
  Presence()    {}
  ~Presence()   {}
  
  public:         /* methods */
  
  void load(const QDir &dir);
  void loadDefaults();
  QPixmap pixmap(Protocol::Enumerables::Contact::Presence presence);
  
  private:        /* variables */
  
  QPixmap available,
          away,
          dnd,
          offline,
          online,
          unaffiliated
  ;
};

}

}

}

}

#endif /* __MXIT_GUI_THEMECOMPONENTS_CONTACT_PRESENCE_H__ */

