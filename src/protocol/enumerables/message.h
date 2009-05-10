/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
****************************************************************************/
  
#ifndef __MXIT_PROTOCOL_ENUMERABLES_MESSAGE_H__
#define __MXIT_PROTOCOL_ENUMERABLES_MESSAGE_H__

namespace MXit
{

namespace Protocol
{

namespace Enumerables
{

namespace Message
{

/****************************************************************************
**
** Author: Marc Bowes
**
** enum definition for message types
**
****************************************************************************/
enum Type {
  Normal        = 1,
  Chat          = 2,
  Headline      = 3,
  Error         = 4,
  GroupChat     = 5,
  CustomForm    = 6,
  Command       = 7
};

}

}

}

}

#endif
