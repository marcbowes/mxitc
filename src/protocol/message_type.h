/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
****************************************************************************/

  
#ifndef __MXIT_PROTOCOL_MESSAGETYPE_H__
#define __MXIT_PROTOCOL_MESSAGETYPE_H__

namespace MXit
{

namespace Protocol
{

/****************************************************************************
**
** Author: Marc Bowes
**
** enum definition for message types
**
****************************************************************************/
enum MessageType {
  MessageTypeNormal             = 1,
  MessageTypeChat               = 2,
  MessageTypeHeadline           = 3,
  MessageTypeError              = 4,
  MessageTypeGroupChat          = 5,
  MessageTypeCustomForm         = 6,
  MessageTypeCommand            = 7
};

}

}

#endif
