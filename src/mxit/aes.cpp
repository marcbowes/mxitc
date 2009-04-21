#include "aes.h"

#define Nb 4			// number of columns in the state & expanded key
#define Nk 4			// number of columns in a key
#define Nr 10			// number of rounds in encryption

namespace MXit {

AES::AES()
{
  numberOfBlocks = 0;
  
  SBox = new QByteArray(QByteArray::fromHex
("637c777bf26b6fc53001672bfed7ab76ca82c97dfa5947f0add4a2af9ca472c0b7fd9326363ff7cc34a5\
e5f171d8311504c723c31896059a071280e2eb27b27509832c1a1b6e5aa0523bd6b329e32f8453d100ed20\
fcb15b6acbbe394a4c58cfd0efaafb434d338545f9027f503c9fa851a3408f929d38f5bcb6da2110fff3d2\
cd0c13ec5f974417c4a77e3d645d197360814fdc222a908846eeb814de5e0bdbe0323a0a4906245cc2d3ac\
629195e479e7c8376d8dd54ea96c56f4ea657aae08ba78252e1ca6b4c6e8dd741f4bbd8b8a703eb5664803\
f60e613557b986c11d9ee1f8981169d98e949b1e87e9ce5528df8ca1890dbfe6426841992d0fb054bb16"));

  InvSBox = new QByteArray(QByteArray::fromHex
("52096ad53036a538bf40a39e81f3d7fb7ce339829b2fff87348e4344c4dee9cb547b9432a6c2233dee4c\
950b42fac34e082ea16628d924b2765ba2496d8bd12572f8f66486689816d4a45ccc5d65b6926c704850fd\
edb9da5e154657a78d9d8490d8ab008cbcd30af7e45805b8b34506d02c1e8fca3f0f02c1afbd0301138a6b\
3a9111414f67dcea97f2cfcef0b4e67396ac7422e7ad3585e2f937e81c75df6e47f11a711d29c5896fb762\
0eaa18be1bfc563e4bc6d279209adbc0fe78cd5af41fdda8338807c731b11210592780ec5f60517fa919b5\
4a0d2de57a9f93c99cefa0e03b4dae2af5b0c8ebbb3c83539961172b047eba77d626e169146355210c7d"));

  Xtime2SBox = new QByteArray(QByteArray::fromHex
("c6f8eef6ffd6de916002ce56e7b54dec8f1f89faefb28efb41b35f452353e49b75e13d4c6c7ef5836851\
d1f9e2ab622a0895469d30370a2f0e241bdfcd4e7fea121d583436dcb45ba476b77d52dd5e13a6b900c140\
e379b6d48d67729498b085bbc54fed869a66118ae904fea078254ba25d80053f2170f16377af4220e5fdbf\
811826c3be35882e9355fc7ac8ba32e6c0199ea344543b0b8cc76b28a7bc16addb647414920c48b89fbd43\
c43931d3f2d58b6eda01b19c49d8acf3cfcaf447106ff04a5c38577397cba1e83e96610d0fe07c71cc9006\
f71cc26aae6917993a27d9eb2b22d2a907332d3c15c987aa50a50359091a65d784d082295a1e7ba86d2c"));

  Xtime3SBox = new QByteArray(QByteArray::fromHex
("a584998d0dbdb1545003a97d1962e69a459d408715ebc90bec67fdeabff7965bc21cae6a5a41024f5cf4\
34089373533f0c52655e28a10fb509369b3d2669cd9f1b9e742e2db2eefbf64d61ce7b3e7197f568002c60\
1fc8edbe46d94bded4e84a6b2ae516c5d75594cf100681f044bae3f3fec08aadbc4804dfc17563301a0e6d\
4c14352fe1a2cc3957f28247ace72b95a098d17f667eab83ca29d33c79e21d763b564e1edb0a6ce45d6eef\
a6a8a4378b324359b78c64d2e0b4fa0725af8ee918d5886f7224f1c751237c9c21dddc86859042c4aad805\
0112a35ff9d0915827b93813b333bb7089a7b622922049ff787a8ff88017da31c6b8c3b07711cbfcd63a"));

  Xtime2 = new QByteArray(QByteArray::fromHex
("00020406080a0c0e10121416181a1c1e20222426282a2c2e30323436383a3c3e40424446484a4c4e5052\
5456585a5c5e60626466686a6c6e70727476787a7c7e80828486888a8c8e90929496989a9c9ea0a2a4a6a8\
aaacaeb0b2b4b6b8babcbec0c2c4c6c8caccced0d2d4d6d8dadcdee0e2e4e6e8eaeceef0f2f4f6f8fafcfe\
1b191f1d131117150b090f0d030107053b393f3d333137352b292f2d232127255b595f5d535157554b494f\
4d434147457b797f7d737177756b696f6d636167659b999f9d939197958b898f8d83818785bbb9bfbdb3b1\
b7b5aba9afada3a1a7a5dbd9dfddd3d1d7d5cbc9cfcdc3c1c7c5fbf9fffdf3f1f7f5ebe9efede3e1e7e5"));

  Xtime9 = new QByteArray(QByteArray::fromHex
("0009121b242d363f48415a536c657e779099828bb4bda6afd8d1cac3fcf5eee73b3229201f160d04737a\
6168575e454caba2b9b08f869d94e3eaf1f8c7ced5dc767f646d525b40493e372c251a130801e6eff4fdc2\
cbd0d9aea7bcb58a8398914d445f5669607b72050c171e2128333addd4cfc6f9f0ebe2959c878eb1b8a3aa\
ece5fef7c8c1dad3a4adb6bf8089929b7c756e6758514a43343d262f1019020bd7dec5ccf3fae1e89f968d\
84bbb2a9a0474e555c636a71780f061d142b2239309a938881beb7aca5d2dbc0c9f6ffe4ed0a0318112e27\
3c35424b5059666f747da1a8b3ba858c979ee9e0fbf2cdc4dfd63138232a151c070e79706b625d544f46"));

  XtimeB = new QByteArray(QByteArray::fromHex
("000b161d2c273a3158534e45747f6269b0bba6ad9c978a81e8e3fef5c4cfd2d97b706d66575c414a2328\
353e0f041912cbc0ddd6e7ecf1fa9398858ebfb4a9a2f6fde0ebdad1ccc7aea5b8b38289949f464d505b6a\
617c771e1508033239242f8d869b90a1aab7bcd5dec3c8f9f2efe43d362b20111a070c656e737849425f54\
f7fce1eadbd0cdc6afa4b9b28388959e474c515a6b607d761f1409023338252e8c879a91a0abb6bdd4dfc2\
c9f8f3eee53c372a21101b060d646f727948435e55010a171c2d263b3059524f44757e6368b1baa7ac9d96\
8b80e9e2fff4c5ced3d87a716c67565d404b2229343f0e051813cac1dcd7e6edf0fb9299848fbeb5a8a3"));

  XtimeD = new QByteArray(QByteArray::fromHex
("000d1a1734392e236865727f5c51464bd0ddcac7e4e9fef3b8b5a2af8c81969bbbb6a1ac8f829598d3de\
c9c4e7eafdf06b66717c5f524548030e1914373a2d206d60777a5954434e05081f12313c2b26bdb0a7aa89\
84939ed5d8cfc2e1ecfbf6d6dbccc1e2eff8f5beb3a4a98a87909d060b1c11323f28256e6374795a57404d\
dad7c0cdeee3f4f9b2bfa8a5868b9c910a07101d3e332429626f7875565b4c41616c7b7655584f42090413\
1e3d30272ab1bcaba685889f92d9d4c3ceede0f7fab7baada0838e9994dfd2c5c8ebe6f1fc676a7d70535e\
49440f0215183b36212c0c01161b3835222f64697e73505d4a47dcd1c6cbe8e5f2ffb4b9aea3808d9a97"));

  XtimeE = new QByteArray(QByteArray::fromHex
("000e1c123836242a707e6c624846545ae0eefcf2d8d6c4ca909e8c82a8a6b4badbd5c7c9e3edfff1aba5\
b7b9939d8f813b352729030d1f114b455759737d6f61ada3b1bf959b8987ddd3c1cfe5ebf9f74d43515f75\
7b69673d33212f050b191776786a644e40525c06081a143e30222c96988a84aea0b2bce6e8faf4ded0c2cc\
414f5d537977656b313f2d230907151ba1afbdb39997858bd1dfcdc3e9e7f5fb9a948688a2acbeb0eae4f6\
f8d2dccec07a746668424c5e500a041618323c2e20ece2f0fed4dac8c69c92808ea4aab8b60c02101e343a\
28267c72606e444a585637392b250f01131d47495b557f71636dd7d9cbc5efe1f3fda7a9bbb59f91838d"));

  Rcon = new QByteArray(QByteArray::fromHex("0001020408102040801b36"));
}

AES::~AES()
{
  delete SBox;
  delete InvSBox;
  delete Xtime2SBox;
  delete Xtime3SBox;
  delete Xtime2;
  delete Xtime9;
  delete XtimeB;
  delete XtimeD;
  delete XtimeE;
  delete Rcon;
}

QByteArray AES::encrypt(const QByteArray &orig_key, const QByteArray &data)
{
  state = new QByteArray(data);
  padData();
  
  for (currentBlock = 0; currentBlock < numberOfBlocks; currentBlock++) {
    key = new QByteArray(orig_key);
    
    expandKey();
    
    round = 0;
    AddRoundKey();
    
    for (round = 1; round < (Nr + 1); round++) {
      if (round < Nr)
        MixSubColumns();
      else
        ShiftRows();
      
      AddRoundKey();
    }
    
    delete key;
  }
  
  QByteArray returnArray(*state);
  
  delete state;
  return returnArray;
}

QByteArray AES::decrypt(const QByteArray &orig_key, const QByteArray &data)
{
  
}

//CORRECT ISO10126 using a blocklength of 16 (128bit)
void AES::padData()
{
  int length = state->size();
  int i;
  for (i=length; (i % 16) != 0; i++)
    (*state)[currentBlock+i] = 'X';
  if (i != length)
    (*state)[currentBlock+i-1] = (i - length);
  numberOfBlocks = (i / 16);
}

void AES::expandKey()
{
  char tmp0, tmp1, tmp2, tmp3, tmp4;
  
  for(int i = Nk; i < Nb * (Nr + 1); i++ ) {
    tmp0 = (*key)[4*i - 4];
    tmp1 = (*key)[4*i - 3];
    tmp2 = (*key)[4*i - 2];
    tmp3 = (*key)[4*i - 1];
    if( !(i % Nk) ) {
      tmp4 = tmp3;
      tmp3 = (*SBox)[tmp0];
      tmp0 = (*SBox)[tmp1] ^ (*Rcon)[i/Nk];
      tmp1 = (*SBox)[tmp2];
      tmp2 = (*SBox)[tmp4];
    } else if( Nk > 6 && i % Nk == 4 ) {
      tmp0 = (*SBox)[tmp0];
      tmp1 = (*SBox)[tmp1];
      tmp2 = (*SBox)[tmp2];
      tmp3 = (*SBox)[tmp3];
    }
    
    (*key)[4*i+0] = (*key)[4*i - 4*Nk + 0] ^ tmp0;
    (*key)[4*i+1] = (*key)[4*i - 4*Nk + 1] ^ tmp1;
    (*key)[4*i+2] = (*key)[4*i - 4*Nk + 2] ^ tmp2;
    (*key)[4*i+3] = (*key)[4*i - 4*Nk + 3] ^ tmp3;
  }
}

void AES::ShiftRows()
{
  char tmp;
  
  // just substitute row 0
  (*state)[currentBlock+0] = (*SBox)[(*state)[currentBlock+0]];
  (*state)[currentBlock+4] = (*SBox)[(*state)[currentBlock+4]];
  (*state)[currentBlock+8] = (*SBox)[(*state)[currentBlock+8]];
  (*state)[currentBlock+12] = (*SBox)[(*state)[currentBlock+12]];
  
  // rotate row 1
  tmp = (*SBox)[(*state)[currentBlock+1]];
  (*state)[currentBlock+1] = (*SBox)[(*state)[currentBlock+5]];
  (*state)[currentBlock+5] = (*SBox)[(*state)[currentBlock+9]];
  (*state)[currentBlock+9] = (*SBox)[(*state)[currentBlock+13]];
  (*state)[currentBlock+13] = tmp;
  
  // rotate row 2
  tmp = (*SBox)[(*state)[currentBlock+2]];
  (*state)[currentBlock+2] = (*SBox)[(*state)[currentBlock+10]];
  (*state)[currentBlock+10] = tmp;
  tmp = (*SBox)[(*state)[currentBlock+6]];
  (*state)[currentBlock+6] = (*SBox)[(*state)[currentBlock+14]];
  (*state)[currentBlock+14] = tmp;
  
  // rotate row 3
  tmp = (*SBox)[(*state)[currentBlock+15]];
  (*state)[currentBlock+15] = (*SBox)[(*state)[currentBlock+11]];
  (*state)[currentBlock+11] = (*SBox)[(*state)[currentBlock+7]];
  (*state)[currentBlock+7] = (*SBox)[(*state)[currentBlock+3]];
  (*state)[currentBlock+3] = tmp;
}

void AES::InvShiftRows()
{
  char tmp;
  
  // restore row 0
  (*state)[currentBlock+0] = (*InvSBox)[(*state)[currentBlock+0]];
  (*state)[currentBlock+4] = (*InvSBox)[(*state)[currentBlock+4]];
  (*state)[currentBlock+8] = (*InvSBox)[(*state)[currentBlock+8]];
  (*state)[currentBlock+12] = (*InvSBox)[(*state)[currentBlock+12]];
  
  // restore row 1
  tmp = (*InvSBox)[(*state)[currentBlock+13]];
  (*state)[currentBlock+13] = (*InvSBox)[(*state)[currentBlock+9]];
  (*state)[currentBlock+9] = (*InvSBox)[(*state)[currentBlock+5]];
  (*state)[currentBlock+5] = (*InvSBox)[(*state)[currentBlock+1]];
  (*state)[currentBlock+1] = tmp;
  
  // restore row 2
  tmp = (*InvSBox)[(*state)[currentBlock+2]];
  (*state)[currentBlock+2] = (*InvSBox)[(*state)[currentBlock+10]];
  (*state)[currentBlock+10] = tmp;
  tmp = (*InvSBox)[(*state)[currentBlock+6]];
  (*state)[currentBlock+6] = (*InvSBox)[(*state)[currentBlock+14]];
  (*state)[currentBlock+14] = tmp;
  
  // restore row 3
  tmp = (*InvSBox)[(*state)[currentBlock+3]];
  (*state)[currentBlock+3] = (*InvSBox)[(*state)[currentBlock+7]];
  (*state)[currentBlock+7] = (*InvSBox)[(*state)[currentBlock+11]];
  (*state)[currentBlock+11] = (*InvSBox)[(*state)[currentBlock+15]];
  (*state)[currentBlock+15] = tmp;
}

void AES::MixSubColumns()
{
  // mixing column 0
  (*state)[currentBlock+0] = (*Xtime2SBox)[(*state)[currentBlock+0]] ^ (*Xtime3SBox)[(*state)[currentBlock+5]] ^ (*SBox)[(*state)[currentBlock+10]] ^ (*SBox)[(*state)[currentBlock+15]];
  (*state)[currentBlock+1] = (*SBox)[(*state)[currentBlock+0]] ^ (*Xtime2SBox)[(*state)[currentBlock+5]] ^ (*Xtime3SBox)[(*state)[currentBlock+10]] ^ (*SBox)[(*state)[currentBlock+15]];
  (*state)[currentBlock+2] = (*SBox)[(*state)[currentBlock+0]] ^ (*SBox)[(*state)[currentBlock+5]] ^ (*Xtime2SBox)[(*state)[currentBlock+10]] ^ (*Xtime3SBox)[(*state)[currentBlock+15]];
  (*state)[currentBlock+3] = (*Xtime3SBox)[(*state)[currentBlock+0]] ^ (*SBox)[(*state)[currentBlock+5]] ^ (*SBox)[(*state)[currentBlock+10]] ^ (*Xtime2SBox)[(*state)[currentBlock+15]];
  
  // mixing column 1
  (*state)[currentBlock+4] = (*Xtime2SBox)[(*state)[currentBlock+4]] ^ (*Xtime3SBox)[(*state)[currentBlock+9]] ^ (*SBox)[(*state)[currentBlock+14]] ^ (*SBox)[(*state)[currentBlock+3]];
  (*state)[currentBlock+5] = (*SBox)[(*state)[currentBlock+4]] ^ (*Xtime2SBox)[(*state)[currentBlock+9]] ^ (*Xtime3SBox)[(*state)[currentBlock+14]] ^ (*SBox)[(*state)[currentBlock+3]];
  (*state)[currentBlock+6] = (*SBox)[(*state)[currentBlock+4]] ^ (*SBox)[(*state)[currentBlock+9]] ^ (*Xtime2SBox)[(*state)[currentBlock+14]] ^ (*Xtime3SBox)[(*state)[currentBlock+3]];
  (*state)[currentBlock+7] = (*Xtime3SBox)[(*state)[currentBlock+4]] ^ (*SBox)[(*state)[currentBlock+9]] ^ (*SBox)[(*state)[currentBlock+14]] ^ (*Xtime2SBox)[(*state)[currentBlock+3]];
  
  // mixing column 2
  (*state)[currentBlock+8] = (*Xtime2SBox)[(*state)[currentBlock+8]] ^ (*Xtime3SBox)[(*state)[currentBlock+13]] ^ (*SBox)[(*state)[currentBlock+2]] ^ (*SBox)[(*state)[currentBlock+7]];
  (*state)[currentBlock+9] = (*SBox)[(*state)[currentBlock+8]] ^ (*Xtime2SBox)[(*state)[currentBlock+13]] ^ (*Xtime3SBox)[(*state)[currentBlock+2]] ^ (*SBox)[(*state)[currentBlock+7]];
  (*state)[currentBlock+10]  = (*SBox)[(*state)[currentBlock+8]] ^ (*SBox)[(*state)[currentBlock+13]] ^ (*Xtime2SBox)[(*state)[currentBlock+2]] ^ (*Xtime3SBox)[(*state)[currentBlock+7]];
  (*state)[currentBlock+11]  = (*Xtime3SBox)[(*state)[currentBlock+8]] ^ (*SBox)[(*state)[currentBlock+13]] ^ (*SBox)[(*state)[currentBlock+2]] ^ (*Xtime2SBox)[(*state)[currentBlock+7]];

	// mixing column 3
	(*state)[currentBlock+12] = (*Xtime2SBox)[(*state)[currentBlock+12]] ^ (*Xtime3SBox)[(*state)[currentBlock+1]] ^ (*SBox)[(*state)[currentBlock+6]] ^ (*SBox)[(*state)[currentBlock+11]];
	(*state)[currentBlock+13] = (*SBox)[(*state)[currentBlock+12]] ^ (*Xtime2SBox)[(*state)[currentBlock+1]] ^ (*Xtime3SBox)[(*state)[currentBlock+6]] ^ (*SBox)[(*state)[currentBlock+11]];
	(*state)[currentBlock+14] = (*SBox)[(*state)[currentBlock+12]] ^ (*SBox)[(*state)[currentBlock+1]] ^ (*Xtime2SBox)[(*state)[currentBlock+6]] ^ (*Xtime3SBox)[(*state)[currentBlock+11]];
	(*state)[currentBlock+15] = (*Xtime3SBox)[(*state)[currentBlock+12]] ^ (*SBox)[(*state)[currentBlock+1]] ^ (*SBox)[(*state)[currentBlock+6]] ^ (*Xtime2SBox)[(*state)[currentBlock+11]];
}

void AES::InvMixSubColumns()
{
  char tmp[4 * Nb];
  
  // restore column 0
  tmp[0] = (*XtimeE)[(*state)[currentBlock+0]] ^ (*XtimeB)[(*state)[currentBlock+1]] ^ (*XtimeD)[(*state)[currentBlock+2]] ^ (*Xtime9)[(*state)[currentBlock+3]];
  tmp[5] = (*Xtime9)[(*state)[currentBlock+0]] ^ (*XtimeE)[(*state)[currentBlock+1]] ^ (*XtimeB)[(*state)[currentBlock+2]] ^ (*XtimeD)[(*state)[currentBlock+3]];
  tmp[10] = (*XtimeD)[(*state)[currentBlock+0]] ^ (*Xtime9)[(*state)[currentBlock+1]] ^ (*XtimeE)[(*state)[currentBlock+2]] ^ (*XtimeB)[(*state)[currentBlock+3]];
  tmp[15] = (*XtimeB)[(*state)[currentBlock+0]] ^ (*XtimeD)[(*state)[currentBlock+1]] ^ (*Xtime9)[(*state)[currentBlock+2]] ^ (*XtimeE)[(*state)[currentBlock+3]];
  
  // restore column 1
  tmp[4] = (*XtimeE)[(*state)[currentBlock+4]] ^ (*XtimeB)[(*state)[currentBlock+5]] ^ (*XtimeD)[(*state)[currentBlock+6]] ^ (*Xtime9)[(*state)[currentBlock+7]];
  tmp[9] = (*Xtime9)[(*state)[currentBlock+4]] ^ (*XtimeE)[(*state)[currentBlock+5]] ^ (*XtimeB)[(*state)[currentBlock+6]] ^ (*XtimeD)[(*state)[currentBlock+7]];
  tmp[14] = (*XtimeD)[(*state)[currentBlock+4]] ^ (*Xtime9)[(*state)[currentBlock+5]] ^ (*XtimeE)[(*state)[currentBlock+6]] ^ (*XtimeB)[(*state)[currentBlock+7]];
  tmp[3] = (*XtimeB)[(*state)[currentBlock+4]] ^ (*XtimeD)[(*state)[currentBlock+5]] ^ (*Xtime9)[(*state)[currentBlock+6]] ^ (*XtimeE)[(*state)[currentBlock+7]];
  
  // restore column 2
  tmp[8] = (*XtimeE)[(*state)[currentBlock+8]] ^ (*XtimeB)[(*state)[currentBlock+9]] ^ (*XtimeD)[(*state)[currentBlock+10]] ^ (*Xtime9)[(*state)[currentBlock+11]];
  tmp[13] = (*Xtime9)[(*state)[currentBlock+8]] ^ (*XtimeE)[(*state)[currentBlock+9]] ^ (*XtimeB)[(*state)[currentBlock+10]] ^ (*XtimeD)[(*state)[currentBlock+11]];
  tmp[2]  = (*XtimeD)[(*state)[currentBlock+8]] ^ (*Xtime9)[(*state)[currentBlock+9]] ^ (*XtimeE)[(*state)[currentBlock+10]] ^ (*XtimeB)[(*state)[currentBlock+11]];
  tmp[7]  = (*XtimeB)[(*state)[currentBlock+8]] ^ (*XtimeD)[(*state)[currentBlock+9]] ^ (*Xtime9)[(*state)[currentBlock+10]] ^ (*XtimeE)[(*state)[currentBlock+11]];
  
  // restore column 3
  tmp[12] = (*XtimeE)[(*state)[currentBlock+12]] ^ (*XtimeB)[(*state)[currentBlock+13]] ^ (*XtimeD)[(*state)[currentBlock+14]] ^ (*Xtime9)[(*state)[currentBlock+15]];
  tmp[1] = (*Xtime9)[(*state)[currentBlock+12]] ^ (*XtimeE)[(*state)[currentBlock+13]] ^ (*XtimeB)[(*state)[currentBlock+14]] ^ (*XtimeD)[(*state)[currentBlock+15]];
  tmp[6] = (*XtimeD)[(*state)[currentBlock+12]] ^ (*Xtime9)[(*state)[currentBlock+13]] ^ (*XtimeE)[(*state)[currentBlock+14]] ^ (*XtimeB)[(*state)[currentBlock+15]];
  tmp[11] = (*XtimeB)[(*state)[currentBlock+12]] ^ (*XtimeD)[(*state)[currentBlock+13]] ^ (*Xtime9)[(*state)[currentBlock+14]] ^ (*XtimeE)[(*state)[currentBlock+15]];
  
  for(int i=0; i < (4 * Nb); i++)
    (*state)[currentBlock+i] = (*InvSBox)[tmp[i]];
}

void AES::AddRoundKey()
{
  for(int i = 0; i < 16; i++ )
  {
    (*state)[currentBlock+i] = (*state)[currentBlock+i] ^ (*key)[i + (round*Nb)];
  }
}

}

