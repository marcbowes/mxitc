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
  
  int position = 0;
  int size = chunkedData.size();
  
  while (position < chunkedData.size()) {
    //getting type byte
    int type = (int)chunkedData.at(position);
    qDebug() << "type: " << type;
    
    position++;
    
    //now at first length byte. getting length
    //watch this haxx
    int length = 0;
    length |= (unsigned char)chunkedData[position];
    position++;
    for (int j=0; j<3; j++) {
      length = length << 8;
      length |= (unsigned char)chunkedData[position + j];
    }
    position += 3;
    //random null terminator. nfc what its for
    position++;
    
    qDebug() << "size: " << size;
    qDebug() << "length: " << length;
    qDebug() << "position at data: " << position;
        
    //now at first data byte
    //getting data
    QByteArray data = chunkedData.mid(position, length);
    
    
    QByteArray tempData;
    int penis = 0;
    int tdpenis = 0;
    int chunkLength = 0;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    
    switch (type) {
      case MXit::Protocol::Enumerables::ChunkedData::CustomResource:
        
        
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
        
        chunkLength |= data[penis];
        penis++;
        for (int j=0; j<3; j++) {
          chunkLength = chunkLength << 8;
          chunkLength |= (unsigned char)data[penis + j];
        }
        penis += 3;
        
        qDebug() << "chunk length: " << chunkLength;
        
        penis += chunkLength;
        
        if (penis != data.size())
          qDebug() << "Error parsing chunked data";
        
        break;
      default:
        qDebug() << "Error: Unknown chunked data type";
        break;
    }
    
    //moving to next chunk
    position += (length - 1);
  }
  
  return VariableHash();
}

}

}

}
