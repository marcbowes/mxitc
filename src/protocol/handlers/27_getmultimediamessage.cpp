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
  //commmon
  QByteArray data;
  VariableHash returnData;
  
  //used in CustomResource
  int dataLength, dataType, position, chunksLen;
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  
  //used in GetFile
  int offset, fileLength, crc;
  
  
  //used in SplashImage
  int anchor, timeToShow;
  
  
  switch (type) {
    case MXit::Protocol::Enumerables::ChunkedData::CustomResource:
      qDebug() << "Type 1 Start";
      position = 1; //because it starts witha \0 for no apparent reason
      
      while (chunkData.at(position) != '\0') {
        data.append(chunkData[position]);
        position++;
      }
      //FIXME: need to save id to variablehash
      qDebug() << "id: " << codec->toUnicode(data);
      
      position++;
      position++; //FIXME: dodging the tab, necessary?
      data.clear();
            
      while ((chunkData.at(position) != '\0')&&(chunkData.at(position) != '\1')) {
        data.append(chunkData[position]);
        position++;
      }
      //FIXME: need to save to variable hash
      qDebug() << "handle: " << codec->toUnicode(data);
      //get operation FIXME: need to save to variable hash
      qDebug() << "operation: " << (int)chunkData.at(position);
      
      position++; // now at chunks len
      
      chunksLen = 0;
      chunksLen |= (unsigned char)chunkData[position];
      position++;
      for (int i=0; i<3; i++) {
        chunksLen = chunksLen << 8;
        chunksLen |= (unsigned char)chunkData[position + i];
      }
      position += 3;
      
      qDebug() << "chunksLen: " << chunksLen;
      
      //now at first chunk of many; FIXME to handle more than one
      data = getChunk(chunkData.mid(position), dataType, dataLength);
      //qDebug() << "Type: " << (int)chunkData[position+2];
      returnData.unite(handleChunk(dataType, dataLength, data));
      
      qDebug() << "Type 1 End";
      break;
    case MXit::Protocol::Enumerables::ChunkedData::SplashImage:
      qDebug() << "Type 2 Start";
      
      //anchor FIXME save to variable hash
      anchor = (unsigned char)chunkData[0];
      
      //timeToShow FIXME save to variable hash
      timeToShow = (unsigned char)chunkData[1];
      
      //bgcolour FIXME save to variable hash
      data = chunkData.mid(2, 4);
      
      //image FIXME save to variable hash
      data = chunkData.mid(6, (length - 6));
      
      qDebug() << "Type 2 End";
      break;
    case MXit::Protocol::Enumerables::ChunkedData::SplashClickThrough:
      //currently empty
      break;
    case MXit::Protocol::Enumerables::ChunkedData::GetFile:
      qDebug() << "Type 8 Start";
      
      //id FIXME save to variable hash
      data = chunkData.left(8);
      
      //offset FIXME save to variable hash
      offset = 0;
      offset |= (unsigned char)chunkData[8];
      for (int i=9; i<12; i++) {
        offset = offset << 8;
        offset |= (unsigned char)chunkData[i];
      }
      
      //fileLength
      fileLength = 0;
      fileLength |= (unsigned char)chunkData[12];
      for (int i=13; i<16; i++) {
        fileLength = fileLength << 8;
        fileLength |= (unsigned char)chunkData[i];
      }
      
      //crc FIXME do check later
      crc = 0;
      crc |= (unsigned char)chunkData[16];
      for (int i=17; i<20; i++) {
        crc = crc << 8;
        crc |= (unsigned char)chunkData[i];
      }
      
      //data FIXME save to variable hash
      data = chunkData.mid(20, fileLength);
      
      qDebug() << "Type 8 End";
      break;
    case MXit::Protocol::Enumerables::ChunkedData::Skin:
      //we shouldnt get this
      break;
    default:
      qDebug() << "Error: Unknown chunked data type " << type;
      break;
  }
  
  return returnData;
}

}

}

}
