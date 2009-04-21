/* This is a QT-ish rewrite of Karl Malbrain's C AES*/

#ifndef __AES_H__
#define __AES_H__

#include <QByteArray>

typedef unsigned char uchar;

namespace MXit {

class AES {
  public:
  
  AES();
  ~AES();
  
  QByteArray encrypt(const QByteArray &orig_key, const QByteArray &data);
  QByteArray decrypt(const QByteArray &orig_key, const QByteArray &data);
  
  private:
  
  void ExpandKey(unsigned char *expkey);
  void ShiftRows(unsigned char *state);
  void InvShiftRows(unsigned char *state);
  void MixSubColumns(unsigned char *state);
  void InvMixSubColumns(unsigned char *state);
  void AddRoundKey(unsigned *state, unsigned *key);
  void realEncrypt(unsigned char *state, unsigned char *expkey);
  void realDecrypt(unsigned char *state, unsigned char *expkey);
  
};

}

#endif
