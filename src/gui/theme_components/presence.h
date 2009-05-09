/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** contact presence info
**
****************************************************************************/


#ifndef __MXIT_GUI_THEMECOMPONENTS_PRESENCE_H__
#define __MXIT_GUI_THEMECOMPONENTS_PRESENCE_H__

#include <QPixmap>

#include "protocol/enumerables/contact.h"

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

class Presence
{
  public:         /* class specific */
  
  Presence();
  ~Presence();
  
  public:         /* methods */
  
  QPixmap pixmap(Protocol::Enumerables::Contact::Presence presence,
    const QString &extensions = "png");
  
  private:        /* variables */
  
  QPixmap available,
          away,
          dnd,
          offline,
          online
  ;
};

}

}

}

#endif /* __MXIT_GUI_THEMECOMPONENTS_PRESENCE_H__ */

