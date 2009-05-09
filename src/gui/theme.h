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

#include "theme_components/presence.h"

namespace MXit
{

namespace GUI
{

class Theme
{
  public:         /* class specific */
  
  Theme();
  ~Theme();
  
  public:         /* variables */
  
  ThemeComponents::Presence  *presence;
};

}

}

#endif /* __MXIT_GUI_THEME_H__ */

