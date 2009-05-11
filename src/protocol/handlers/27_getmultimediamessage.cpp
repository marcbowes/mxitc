/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "27_getmultimediamessage.h"
#include "protocol/enumerables/chunked_data.h"
#include <QTextCodec>

namespace MXit
{

namespace Protocol
{

namespace Handlers
{

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Populates a packet with the information required to send binary data
**
****************************************************************************/
void GetMultimediaMessage::build(MXit::Network::Packet *packet, VariableHash &variables)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  id=loginname[\1sesid]\0
  **  cm=27\0
  **  ms=chunkedData
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  chunkedData         see appendix G. too long to paste
  **
  ***************************************************************************
  */
  
  /* packet header setup */
  packet->setCommand("27");
  
  /* packet data setup */
  (*packet) << variables["chunkedData"];
}

/****************************************************************************
**
** Author: Tim Sjoberg
**
** Extracts data and sends back to be interpreted by the handler
**
****************************************************************************/
VariableHash GetMultimediaMessage::handle(const QByteArray &packet)
{
  /*
  == PACKET FORMAT
  ***************************************************************************
  **
  **  27\0
  **  errorCode[\1errorMessage]\0
  **  chunkedData
  **
  ***************************************************************************
  
  == DEFINITIONS
  ***************************************************************************
  **
  **  chunkedData         see appendix G. too long to paste
  **
  ***************************************************************************
  */
  
  int i=0, count = 0;
  
  while (count < (packet.startsWith("ln=") ? 3 : 2)) {
    i++;
    if (packet.at(i) == '\0')
      count++;
  }
  
  QByteArray chunkedData = packet.right(packet.size() - i - 1);
  
  int size = chunkedData.size();
  int type, length;
  
  QByteArray packetData = getChunk(chunkedData, type, length);
  
  if (size != (length + 5))
    qDebug() << "Error parsing data or more than one chunk (and not of type 1)";

  return handleChunk(type, length, packetData);
}

QByteArray GetMultimediaMessage::getChunk(QByteArray chunk, int &type, int &length)
{
  type = (int)chunk.at(0);
  
  length = 0;
  length |= (unsigned char)chunk[1];
  for (int i=2; i<5; i++) {
    length = length << 8;
    length |= (unsigned char)chunk[i];
  }
  
  return chunk.right(chunk.size() - 5);
}

VariableHash GetMultimediaMessage::handleChunk(int type, int length, QByteArray chunkData)
{
  //thing
  QByteArray data;
  int dataLength, dataType, position;
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  VariableHash returnData;
  
  switch (type) {
    case MXit::Protocol::Enumerables::ChunkedData::CustomResource:
      qDebug() << "Type 1 Start";
      position = 1;
      
      while (chunkData.at(position) != '\0') {
        data.append(chunkData[position]);
        position++;
      }
      //FIXME: need to save id to variablehash
      qDebug() << "id: " << codec->toUnicode(data);
      
      position++;
      data.clear();
            
      while ((chunkData.at(position) != '\0')&&(chunkData.at(position) != '\1')) {
        data.append(chunkData[position]);
        position++;
      }
      //FIXME: need to save to variable hash
      qDebug() << "handle: " << codec->toUnicode(data);
      
      //get operation FIXME: need to save to variable hash
      qDebug() << "operation: " << (int)chunkData.at(position);
      
      position++; // now at next chunk
      data = getChunk(chunkData.mid(position), dataType, dataLength);
      returnData.unite(handleChunk(dataType, dataLength, data));
      
      qDebug() << "Type 1 End";
      break;
    default:
      qDebug() << "Error: Unknown chunked data type";
      break;
  }
  
  return returnData;
}

}

}

}
