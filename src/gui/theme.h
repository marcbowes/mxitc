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

#include "theme_components/contact/base.h"

namespace MXit
{

namespace GUI
{

class Theme
{
  public:         /* class specific */
  
  Theme();
  ~Theme();

  public:         /* methods */
  
  void load(QDir theme);
  
  public:         /* variables */
  
  QDir                            dir;
  ThemeComponents::Contact::Base  contact;
};

}

}

#endif /* __MXIT_GUI_THEME_H__ */

