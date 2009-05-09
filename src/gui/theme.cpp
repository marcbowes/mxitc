/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "theme.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Default constructor
** Constructs all theme components
**
****************************************************************************/
Theme::Theme()
{
  using namespace ThemeComponents;
  presence = new Presence();
}

/****************************************************************************
**
** Author: Marc Bowes
**
** descontructor, desconstructs theme components
**
****************************************************************************/
Theme::~Theme()
{
  delete presence;
}

}

}

