/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "27_getmultimediamessage.h"
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
  
  while (count < 3) {
    i++;
    if (packet.at(i) == '\0')
      count++;
  }
  
  QByteArray chunkedData = packet.right(packet.size() - i - 1);
  
  int position = 0;
  int size = chunkedData.size();
  
  //while (position < chunkedData.size()) {
    qDebug() << "position at type: " << position;
    //getting type byte
    int type = (int)chunkedData.at(position);
    qDebug() << "type: " << type;
    
    position++;
    
    qDebug() << "position at length: " << position;
    //now at first length byte. getting length
    //watch this haxx
    int length = 0;
    length |= chunkedData[position];
    qDebug() << "length: " << length;
    position++;
    for (int j=0; j<3; j++) {
      qDebug() << "length before shift: " << length;
      length = length << 8;
      qDebug() << "length after shift: " << length;
      length |= (unsigned char)chunkedData[position + j];
    }
    position += 3;
    //random null terminator. nfc what its for
    position++;
    
    qDebug() << "size: " << size;
    qDebug() << "length: " << length;
    qDebug() << "position + length: " << position + length;
    
    qDebug() << "position at data: " << position;
    //if (length < 0) break;
    
    //now at first data byte
    //getting data
    QByteArray data = chunkedData.mid(position, length);
    
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    
    int penis = 0;
    int tdpenis = 0;
    QByteArray tempData;
    while (data.at(penis) != '\0') {
      tempData[tdpenis] = data[penis];
      penis++;
      tdpenis++;
    }
    qDebug() << "id: " << codec->toUnicode(tempData);
    
    penis++;
    tempData.clear();
    tdpenis = 0;
    
    while ((data.at(penis) != '\0')&&(data.at(penis) != '\1')) {
      tempData[tdpenis] = data[penis];
      penis++;
      tdpenis++;
    }
    
    qDebug() << "handle: " << codec->toUnicode(tempData);
    
    //no null to dodge
    qDebug() << "operation: " << (int)data.at(penis);
    
    penis++; // moving on
    
    int chunkLength = 0;
    chunkLength |= data[penis];
    qDebug() << "chunk length debug";
    qDebug() << chunkLength;
    penis++;
    for (int j=0; j<3; j++) {
      chunkLength = chunkLength << 8;
      qDebug() << "before: " << chunkLength;
      chunkLength |= (unsigned char)data[penis + j];
      qDebug() << "what it is: " << (int)(unsigned char)data[penis + j];
      qDebug() << "after: " << chunkLength;
    }
    penis += 3;
    
    qDebug() << "chunk length: " << chunkLength;
    
    penis += chunkLength;
    
    qDebug() << "penis: " << penis;
    qDebug() << "total: " << data.size();
    
    //moving to next chunk
    position += length;
    
    
    
    //qDebug() << string;
  //}
  
  return VariableHash();
}

}

}

}
