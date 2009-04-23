/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Timothy Sjoberg, 2009
**
** MXit::Protocol::AES is a "Qt-ish" rewrite of Karl Malbrain's aes.c
**
****************************************************************************/

#ifndef __AES_H__
#define __AES_H__

#include <QByteArray>

typedef unsigned char uchar;

namespace MXit
{

namespace Protocol
{

class AES
{
  public:         /* class specific */
  
  AES();
  ~AES();
  
  public:         /* methods */
  
  QByteArray encrypt(const QByteArray &orig_key, const QByteArray &data);
  QByteArray decrypt(const QByteArray &orig_key, const QByteArray &data);
  
  private:        /* methods */
  
  void AddRoundKey(unsigned *state, unsigned *key);
  void ExpandKey(unsigned char *expkey);
  void InvMixSubColumns(unsigned char *state);
  void InvShiftRows(unsigned char *state);
  void MixSubColumns(unsigned char *state);
  void realDecrypt(unsigned char *state, unsigned char *expkey);
  void realEncrypt(unsigned char *state, unsigned char *expkey);
  void ShiftRows(unsigned char *state);
  
};

}

}

#endif

