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

#include <QDir>
#include <QPixmap>

#include "protocol/enumerables/contact.h"

#define DEFAULT_SIZE QSize(16, 16)

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

class Presence
{
  public:         /* class specific */
  
  Presence(const QDir &dir);
  ~Presence();
  
  public:         /* methods */
  
  QPixmap pixmap(Protocol::Enumerables::Contact::Presence presence);
  
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

