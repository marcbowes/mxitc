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
  
  void padData();
  void expandKey();
  void ShiftRows();
  void InvShiftRows();
  void MixSubColumns();
  void InvMixSubColumns();
  void AddRoundKey();
  
  int numberOfBlocks;
  int round;
  
  QByteArray *state;
  QByteArray *key;
  
  QByteArray *SBox; //forward s-box
  QByteArray *InvSBox; //inverse s-box
  QByteArray *Xtime2SBox; //combined Xtimes2[Sbox[]]
  QByteArray *Xtime3SBox; //combined Xtimes3[Sbox[]]
  
// modular multiplication tables
// based on:

// Xtime2[x] = (x & 0x80 ? 0x1b : 0) ^ (x + x)
// Xtime3[x] = x^Xtime2[x];
  QByteArray *Xtime2;
  QByteArray *Xtime9;
  QByteArray *XtimeB;
  QByteArray *XtimeD;
  QByteArray *XtimeE;

  QByteArray *Rcon;
  
};

}

#endif
