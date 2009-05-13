/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** extracts header information from an incoming packet
**
****************************************************************************/

#ifndef __MXIT_PACKETHEADER_H__
#define __MXIT_PACKETHEADER_H__

#include "common/types.h"

namespace MXit
{

namespace Protocol
{

VariableHash packetHeader(const QByteArray &packet);

}

}

#endif

