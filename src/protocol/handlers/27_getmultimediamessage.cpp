/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "27_getmultimediamessage.h"
#include "protocol/enumerables/chunked_data.h"

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
void GetMultimediaMessage::buildPacket(MXit::Network::Packet *packet, VariableHash &variables)
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
  int type, length;
  
  type = variables["type"].toInt();
  
  QByteArray data;
  
  switch(type) {
    case MXit::Protocol::Enumerables::ChunkedData::RejectFile:
      //id
      data.append(variables["id"]);
      
      //reason
      data.append(variables["reason"]);
      
      //data
      data.append(QByteArray::number(variables["data"].length()));
      data.append(variables["data"]);
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::GetFile:
      //id
      data.append(variables["id"]);
      
      //offset
      data.append(variables["offset"]);
      
      //length
      data.append(variables["length"]);
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::ReceivedFile:
      //id
      data.append(variables["id"]);
      
      //res
      data.append(variables["res"]);
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::SendFileDirect:
      //size
      data.append(variables["size"]);
      
      //numContacts
      data.append(variables["numContacts"]);
      
      //addresses FIXME: this only supports 1 at the moment. we need a way of passing a list
      data.append(QByteArray::number(variables["addresses"].length()));
      data.append(variables["addresses"]);
      
      //name
      data.append(QByteArray::number(variables["name"].length()));
      data.append(variables["name"]);
      
      //mimetype
      data.append(QByteArray::number(variables["mimetype"].length()));
      data.append(variables["mimetype"]);
      
      //description
      data.append(QByteArray::number(variables["description"].length()));
      data.append(variables["description"]);
      
      //crc
      data.append(variables["crc"]);
      
      //bytes
      data.append(variables["bytes"]);
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::ForwardFileDirect:
      //id
      data.append(variables["id"]);
      
      //numContacts
      data.append(variables["numContacts"]);
      
      //addresses
      data.append(QByteArray::number(variables["addresses"].length()));
      data.append(variables["addresses"]);
      
      //name
      data.append(QByteArray::number(variables["name"].length()));
      data.append(variables["name"]);
      
      //description
      data.append(QByteArray::number(variables["description"].length()));
      data.append(variables["description"]);
      
      break;
    default:
      //FIXME: report error
      break;
  }
  
  length = data.size();
  
  data.prepend(intToHex(length, 4));
  data.prepend(variables["type"]); 
  
  /* packet data setup */
  (*packet) << data;
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
  
  QByteArray chunkedData = packet;
  
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
  //commmon FIXME: cleanup. data isnt actually necessary i dont think
  QByteArray data;
  VariableHash returnData;
  returnData["type"] = QByteArray::number(type);
  
  //used in CustomResource
  int dataLength, dataType, position, chunksLen;
  VariableHash tempVariables;
  VariableHashItr tempItr(tempVariables);
    
  //used in GetFile
  int offset, fileLength, crc;
  
  
  //used in SplashImage
  int anchor, timeToShow;
  
  
  switch (type) {
    case MXit::Protocol::Enumerables::ChunkedData::CustomResource:
      position = 0;

      //id length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //id
      returnData["id"] = chunkData.mid(position, dataLength);
      
      position+= dataLength;

      //handle length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;

      //handle      
      returnData["handle"] = chunkData.mid(position, dataLength);
      
      position += dataLength;

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

      //now at first chunk of many; FIXME going to get collisions of "handle"_type. but thats a protocol problem
      while (position < length) {
        tempVariables.clear();

        data = getChunk(chunkData.mid(position), dataType, dataLength);
        tempVariables = handleChunk(dataType, dataLength, data);
        
        tempItr = tempVariables;
        while (tempItr.hasNext()) {
          tempItr.next();
          returnData[QString(returnData["handle"]) + "_" + tempItr.key()] = tempItr.value();
        }
        
        position += (5 + dataLength);
      }
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
    case MXit::Protocol::Enumerables::ChunkedData::OfferFile:
      //id
      returnData["id"] = chunkData.left(8);
      
      position = 8;
      
      //contactAddress length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //contactAddress
      returnData["contactAddress"] = chunkData.mid(position, dataLength);
      
      position += dataLength;
      
      //size
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      position++;
      for (int i=0; i<3; i++) {
        dataLength = dataLength << 8;
        dataLength |= (unsigned char)chunkData[position + i];
      }
      
      returnData["size"] = QByteArray::number(dataLength);
      
      position += 3;
      
      //name length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //name
      returnData["name"] = chunkData.mid(position, dataLength);
      
      position += dataLength;
      
      //mimetype length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //mimetype
      returnData["mimetype"] = chunkData.mid(position, dataLength);
      
      position += dataLength;
      
      //timeStamp FIXME: dont skip... maybe
      position += 8;
      
      //description length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //mimetype
      returnData["description"] = chunkData.mid(position, dataLength);
      
      position += dataLength;
      
      //alt length
      dataLength = 0;
      dataLength |= (unsigned char)chunkData[position];
      dataLength = dataLength << 8;
      position++;
      dataLength |= (unsigned char)chunkData[position];
      
      position++;
      
      //mimetype
      returnData["alt"] = chunkData.mid(position, dataLength);
      
      position += dataLength;
      
      //flags
      returnData["flags"] = chunkData.mid(position, 4);
      
      
      break;
    case MXit::Protocol::Enumerables::ChunkedData::GetFile:
      //id
      data = chunkData.left(8);
      returnData["id"] = data;
      
      //offset
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

QByteArray GetMultimediaMessage::intToHex(int number, int numberOfBytes)
{
  QByteArray output = QByteArray::number(number, 16);
  while (output.size() < (numberOfBytes*2))
    output.prepend("0");
  return output.toHex();
}

}

}

}
