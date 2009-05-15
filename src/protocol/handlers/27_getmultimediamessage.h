/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Tim Sjoberg, 2009
**
** MXit::Network::Handlers::GetMultimediaMessage deals with retrieving binary data
** see build/handle definitions in .cpp file for implementation details
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_HANDLERS_GETMULTIMEDIAMESSAGE_H__
#define __MXIT_PROTOCOL_HANDLERS_GETMULTIMEDIAMESSAGE_H__

#include "protocol/handler.h"

#define GETMULTIMEDIAMESSAGE 27

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

class GetMultimediaMessage : public Handler
{
  public:
  
  GetMultimediaMessage() { command = GETMULTIMEDIAMESSAGE; }
  
  virtual void buildPacket(MXit::Network::Packet *packet, VariableHash &variables);
  virtual VariableHash handle(const QByteArray &packet);
  
  private:
  
  QByteArray getChunk(QByteArray chunk, int &type, int &length);
  VariableHash handleChunk(int type, int length, QByteArray chunkData);
  QByteArray intToHex(int number, int numberOfBytes);
};

}

}

}

#endif
