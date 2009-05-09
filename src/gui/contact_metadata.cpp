

#include "contact_metadata.h"

namespace MXit
{

namespace GUI
{


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

ContactMetaData::ContactMetaData()
{
  QSize sz(16,16);
  QPixmap gray =    QPixmap ( sz ); gray  .fill(Qt::gray);
  QPixmap green =   QPixmap ( sz ); green .fill(Qt::green);
  QPixmap orange =  QPixmap ( sz ); orange.fill(QColor(255,215,0));
  QPixmap blue =    QPixmap ( sz ); blue  .fill(Qt::blue);
  QPixmap red =     QPixmap ( sz ); red   .fill(Qt::red);
  
  presenceIconsHash[OFFLINE]         = gray;
  presenceIconsHash[ONLINE]          = green;
  presenceIconsHash[AWAY]            = orange;
  presenceIconsHash[AVAILABLE]       = blue;
  presenceIconsHash[DO_NOT_DISTURB]  = red;
  
  presencePrecedenceHash[ONLINE]          = 0;
  presencePrecedenceHash[AVAILABLE]       = 1;
  presencePrecedenceHash[AWAY]            = 2;
  presencePrecedenceHash[DO_NOT_DISTURB]  = 3;
  presencePrecedenceHash[OFFLINE]         = 4;
}
    
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
ContactMetaData::~ContactMetaData()
{
  
}


    
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
const QHash<Presence, unsigned int>& ContactMetaData::presencePrecedence() const
{
  return presencePrecedenceHash;
}
    
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
  
const QHash<Presence, QIcon>& ContactMetaData::presenceIcons() const
{
  return presenceIconsHash;
}

} /* end namespace GUI */

} /* end namespace MXit */








