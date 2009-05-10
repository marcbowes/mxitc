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
Theme::Theme(const QDir &active)
  : dir(active), presence(dir)
{
  // nothing here
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
  // nothing here
}

}

}

