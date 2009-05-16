/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** MXit::GUI::Theme acts as a holder for theming components
**
****************************************************************************/

#ifndef __MXIT_GUI_THEME_H__
#define __MXIT_GUI_THEME_H__

#include <QDir>
#include <QPixmap>

#include "theme_components/chat.h"
#include "theme_components/contact/base.h"

namespace MXit
{

namespace GUI
{

class Theme
{
  public:         /* class specific */
  
  Theme()   {}
  ~Theme()  {}

  public:         /* methods */
  
  void load(QDir theme);
  void loadDefaults();
  
  public:        /* variables */
  
  ThemeComponents::Chat           chat;
  ThemeComponents::Contact::Base  contact;
  QDir                            location;
  QPixmap                         windowIcon;
};

}

}

#endif /* __MXIT_GUI_THEME_H__ */

