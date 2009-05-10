/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
****************************************************************************/

  
#ifndef __MXIT_PROTOCOL_ENUMERABLES_CHUNKEDDATA_H__
#define __MXIT_PROTOCOL_ENUMERABLES_CHUNKEDDATA_H__

namespace MXit
{

namespace Protocol
{

namespace Enumerables
{

namespace ChunkedData
{

/****************************************************************************
**
** Author: Tim Sjoberg
**
** enum definition for chunked data types
**
****************************************************************************/
enum Type {
  None                = 0,
  CustomResource      = 1,
  SplashImage         = 2,
  SplashClickThrough  = 3,
  OfferFile           = 6,
  RejectFile          = 7,
  GetFile             = 8,
  ReceivedFile        = 9,
  SendFileDirect      = 10,
  ForwardFileDirect   = 11,
  Skin                = 12,
  End                 = 126,
  ExtendedType        = 127
};

}

}

}

}

#endif
