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
    ;/* FIXME: error handling */ //qDebug() << "Error parsing data or more than one chunk (and not of type 1)";

  return handleChunk(type, length, packetData);
}


/****************************************************************************
**
** Author: Tim Sjoberg
**
** FIXME: comment this
**
****************************************************************************/
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


/****************************************************************************
**
** Author: Tim Sjoberg
**
** FIXME: comment this
**
****************************************************************************/
VariableHash GetMultimediaMessage::handleChunk(int type, int length, QByteArray chunkData)
{
  //commmon
  QByteArray data;
  VariableHash returnData;
  returnData["type"] = QByteArray::number(type);
  
  //used in CustomResource
  int dataLength, dataType, position, chunksLen;
  VariableHash tempVariables;
    
  //used in GetFile
  int offset, fileLength, crc;
  
  
  //used in SplashImage
  int anchor, timeToShow;
  
  
  switch (type) {
    case MXit::Protocol::Enumerables::ChunkedData::CustomResource:
      position = 1; //because it starts witha \0 for no apparent reason
      
      //id
      while (chunkData.at(position) != '\0') {
        data.append(chunkData[position]);
        position++;
      }
      
      returnData["id"] = data;
      
      position++;
      data.clear();
      
      //handle      
      position++; //FIXME: dodging the tab, necessary?
      while ((chunkData.at(position) != '\0')&&(chunkData.at(position) != '\1')) {
        data.append(chunkData[position]);
        position++;
      }
      
      returnData["handle"] = data;
      
      //operation
      returnData["operation"] = QByteArray::number((int)chunkData.at(position));
      
      position++; // now at chunks len
      
      chunksLen = 0;
      chunksLen |= (unsigned char)chunkData[position];
      position++;
      for (int i=0; i<3; i++) {
        chunksLen = chunksLen << 8;
        chunksLen |= (unsigned char)chunkData[position + i];
      }
      position += 3;
      
      //now at first chunk of many; FIXME to handle more than one
      data = getChunk(chunkData.mid(position), dataType, dataLength);
      tempVariables = handleChunk(dataType, dataLength, data);
      returnData[QString(returnData["handle"]) + "_type"] = tempVariables["type"];
      returnData[QString(returnData["handle"]) + "_anchor"] = tempVariables["anchor"];
      returnData[QString(returnData["handle"]) + "_timeToShow"] = tempVariables["timeToShow"];
      returnData[QString(returnData["handle"]) + "_bgColour"] = tempVariables["bgColour"];
      returnData[QString(returnData["handle"]) + "_image"] = tempVariables["image"];
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::SplashImage:
      //anchor
      anchor = (unsigned char)chunkData[0];
      if ((anchor != 3)&&(anchor != 0)) {
        ;/* FIXME: error handling */ //qDebug() << "Error parsing splash image: invalid anchor";
        break;
      }
      returnData["anchor"] = QByteArray::number(anchor);
      
      //timeToShow
      timeToShow = (unsigned char)chunkData[1];
      returnData["timeToShow"] = QByteArray::number(timeToShow);
      
      //bgcolour
      data = chunkData.mid(2, 4);
      returnData["bgColour"] = data.toHex();
      
      //image
      data = chunkData.mid(6, (length - 6));
      returnData["image"] = data;
      break;
    case MXit::Protocol::Enumerables::ChunkedData::SplashClickThrough:
      //currently empty
      break;
    case MXit::Protocol::Enumerables::ChunkedData::GetFile:
      //id FIXME save to variable hash
      data = chunkData.left(8);
      returnData["id"] = data;
      
      //offset FIXME save to variable hash
      offset = 0;
      offset |= (unsigned char)chunkData[8];
      for (int i=9; i<12; i++) {
        offset = offset << 8;
        offset |= (unsigned char)chunkData[i];
      }
      returnData["offset"] = QByteArray::number(offset);
      
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
      
      //data
      data = chunkData.mid(20, fileLength);
      returnData["file"] = data;
      break;
    case MXit::Protocol::Enumerables::ChunkedData::Skin:
      //we shouldnt get this
      break;
    default:
      ;/* FIXME: error handling */ //qDebug() << "Error: Unknown chunked data type " << type;
      break;
  }
  
  return returnData;
}

}

}

}
