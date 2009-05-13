/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "message.h"

namespace MXit
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for outgoing chat (to a Contact)
**
****************************************************************************/
Message::Message(const QString &message)
  : contact(NULL), message(message), timestamp(QTime::currentTime())
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for incoming chat (from a Contact)
**
****************************************************************************/
Message::Message(const Contact &contact, const QString &message)
  : contact(&contact), message(message), timestamp (QTime::currentTime())
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Copy constructor
**
****************************************************************************/
Message::Message(const Message &other)
  : contact(other.contact), message(other.message), timestamp(other.timestamp)
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
Message::~Message()
{
  /* Nothing */
}

}

