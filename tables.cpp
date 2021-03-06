
#include <avr/pgmspace.h>

extern const uint16_t sid_frequency[] PROGMEM = {
    0x008b,0x008b,0x008c,0x008c,0x008d,0x008d,0x008e,0x008e,
    0x008f,0x008f,0x0090,0x0090,0x0091,0x0091,0x0092,0x0092,
    0x0093,0x0094,0x0094,0x0095,0x0095,0x0096,0x0096,0x0097,
    0x0097,0x0098,0x0098,0x0099,0x009a,0x009a,0x009b,0x009b,
    0x009c,0x009c,0x009d,0x009d,0x009e,0x009f,0x009f,0x00a0,
    0x00a0,0x00a1,0x00a2,0x00a2,0x00a3,0x00a3,0x00a4,0x00a4,
    0x00a5,0x00a6,0x00a6,0x00a7,0x00a7,0x00a8,0x00a9,0x00a9,
    0x00aa,0x00ab,0x00ab,0x00ac,0x00ac,0x00ad,0x00ae,0x00ae,
    0x00af,0x00b0,0x00b0,0x00b1,0x00b1,0x00b2,0x00b3,0x00b3,
    0x00b4,0x00b5,0x00b5,0x00b6,0x00b7,0x00b7,0x00b8,0x00b9,
    0x00b9,0x00ba,0x00bb,0x00bb,0x00bc,0x00bd,0x00bd,0x00be,
    0x00bf,0x00bf,0x00c0,0x00c1,0x00c2,0x00c2,0x00c3,0x00c4,
    0x00c4,0x00c5,0x00c6,0x00c7,0x00c7,0x00c8,0x00c9,0x00c9,
    0x00ca,0x00cb,0x00cc,0x00cc,0x00cd,0x00ce,0x00cf,0x00cf,
    0x00d0,0x00d1,0x00d2,0x00d2,0x00d3,0x00d4,0x00d5,0x00d5,
    0x00d6,0x00d7,0x00d8,0x00d9,0x00d9,0x00da,0x00db,0x00dc,
    0x00dc,0x00dd,0x00de,0x00df,0x00e0,0x00e1,0x00e1,0x00e2,
    0x00e3,0x00e4,0x00e5,0x00e5,0x00e6,0x00e7,0x00e8,0x00e9,
    0x00ea,0x00ea,0x00eb,0x00ec,0x00ed,0x00ee,0x00ef,0x00f0,
    0x00f1,0x00f1,0x00f2,0x00f3,0x00f4,0x00f5,0x00f6,0x00f7,
    0x00f8,0x00f8,0x00f9,0x00fa,0x00fb,0x00fc,0x00fd,0x00fe,
    0x00ff,0x0100,0x0101,0x0102,0x0103,0x0103,0x0104,0x0105,
    0x0106,0x0107,0x0108,0x0109,0x010a,0x010b,0x010c,0x010d,
    0x010e,0x010f,0x0110,0x0111,0x0112,0x0113,0x0114,0x0115,
    0x0116,0x0117,0x0118,0x0119,0x011a,0x011b,0x011c,0x011d,
    0x011e,0x011f,0x0120,0x0121,0x0122,0x0123,0x0124,0x0125,
    0x0126,0x0128,0x0129,0x012a,0x012b,0x012c,0x012d,0x012e,
    0x012f,0x0130,0x0131,0x0132,0x0134,0x0135,0x0136,0x0137,
    0x0138,0x0139,0x013a,0x013b,0x013d,0x013e,0x013f,0x0140,
    0x0141,0x0142,0x0144,0x0145,0x0146,0x0147,0x0148,0x0149,
    0x014b,0x014c,0x014d,0x014e,0x014f,0x0151,0x0152,0x0153,
    0x0154,0x0156,0x0157,0x0158,0x0159,0x015b,0x015c,0x015d,
    0x015e,0x0160,0x0161,0x0162,0x0163,0x0165,0x0166,0x0167,
    0x0169,0x016a,0x016b,0x016d,0x016e,0x016f,0x0171,0x0172,
    0x0173,0x0175,0x0176,0x0177,0x0179,0x017a,0x017b,0x017d,
    0x017e,0x017f,0x0181,0x0182,0x0184,0x0185,0x0186,0x0188,
    0x0189,0x018b,0x018c,0x018e,0x018f,0x0190,0x0192,0x0193,
    0x0195,0x0196,0x0198,0x0199,0x019b,0x019c,0x019e,0x019f,
    0x01a1,0x01a2,0x01a4,0x01a5,0x01a7,0x01a8,0x01aa,0x01ab,
    0x01ad,0x01ae,0x01b0,0x01b2,0x01b3,0x01b5,0x01b6,0x01b8,
    0x01b9,0x01bb,0x01bd,0x01be,0x01c0,0x01c2,0x01c3,0x01c5,
    0x01c6,0x01c8,0x01ca,0x01cb,0x01cd,0x01cf,0x01d0,0x01d2,
    0x01d4,0x01d5,0x01d7,0x01d9,0x01db,0x01dc,0x01de,0x01e0,
    0x01e2,0x01e3,0x01e5,0x01e7,0x01e9,0x01ea,0x01ec,0x01ee,
    0x01f0,0x01f1,0x01f3,0x01f5,0x01f7,0x01f9,0x01fa,0x01fc,
    0x01fe,0x0200,0x0202,0x0204,0x0206,0x0207,0x0209,0x020b,
    0x020d,0x020f,0x0211,0x0213,0x0215,0x0217,0x0219,0x021b,
    0x021d,0x021e,0x0220,0x0222,0x0224,0x0226,0x0228,0x022a,
    0x022c,0x022e,0x0230,0x0232,0x0234,0x0237,0x0239,0x023b,
    0x023d,0x023f,0x0241,0x0243,0x0245,0x0247,0x0249,0x024b,
    0x024d,0x0250,0x0252,0x0254,0x0256,0x0258,0x025a,0x025d,
    0x025f,0x0261,0x0263,0x0265,0x0268,0x026a,0x026c,0x026e,
    0x0271,0x0273,0x0275,0x0277,0x027a,0x027c,0x027e,0x0281,
    0x0283,0x0285,0x0288,0x028a,0x028c,0x028f,0x0291,0x0293,
    0x0296,0x0298,0x029b,0x029d,0x029f,0x02a2,0x02a4,0x02a7,
    0x02a9,0x02ac,0x02ae,0x02b1,0x02b3,0x02b6,0x02b8,0x02bb,
    0x02bd,0x02c0,0x02c2,0x02c5,0x02c7,0x02ca,0x02cc,0x02cf,
    0x02d2,0x02d4,0x02d7,0x02da,0x02dc,0x02df,0x02e2,0x02e4,
    0x02e7,0x02ea,0x02ec,0x02ef,0x02f2,0x02f4,0x02f7,0x02fa,
    0x02fd,0x02ff,0x0302,0x0305,0x0308,0x030b,0x030d,0x0310,
    0x0313,0x0316,0x0319,0x031c,0x031f,0x0321,0x0324,0x0327,
    0x032a,0x032d,0x0330,0x0333,0x0336,0x0339,0x033c,0x033f,
    0x0342,0x0345,0x0348,0x034b,0x034e,0x0351,0x0354,0x0357,
    0x035a,0x035d,0x0361,0x0364,0x0367,0x036a,0x036d,0x0370,
    0x0373,0x0377,0x037a,0x037d,0x0380,0x0384,0x0387,0x038a,
    0x038d,0x0391,0x0394,0x0397,0x039b,0x039e,0x03a1,0x03a5,
    0x03a8,0x03ab,0x03af,0x03b2,0x03b6,0x03b9,0x03bd,0x03c0,
    0x03c4,0x03c7,0x03ca,0x03ce,0x03d2,0x03d5,0x03d9,0x03dc,
    0x03e0,0x03e3,0x03e7,0x03eb,0x03ee,0x03f2,0x03f5,0x03f9,
    0x03fd,0x0401,0x0404,0x0408,0x040c,0x040f,0x0413,0x0417,
    0x041b,0x041f,0x0422,0x0426,0x042a,0x042e,0x0432,0x0436,
    0x043a,0x043d,0x0441,0x0445,0x0449,0x044d,0x0451,0x0455,
    0x0459,0x045d,0x0461,0x0465,0x0469,0x046e,0x0472,0x0476,
    0x047a,0x047e,0x0482,0x0486,0x048b,0x048f,0x0493,0x0497,
    0x049b,0x04a0,0x04a4,0x04a8,0x04ad,0x04b1,0x04b5,0x04ba,
    0x04be,0x04c2,0x04c7,0x04cb,0x04d0,0x04d4,0x04d9,0x04dd,
    0x04e2,0x04e6,0x04eb,0x04ef,0x04f4,0x04f8,0x04fd,0x0502,
    0x0506,0x050b,0x0510,0x0514,0x0519,0x051e,0x0522,0x0527,
    0x052c,0x0531,0x0536,0x053a,0x053f,0x0544,0x0549,0x054e,
    0x0553,0x0558,0x055d,0x0562,0x0567,0x056c,0x0571,0x0576,
    0x057b,0x0580,0x0585,0x058a,0x058f,0x0594,0x0599,0x059f,
    0x05a4,0x05a9,0x05ae,0x05b4,0x05b9,0x05be,0x05c4,0x05c9,
    0x05ce,0x05d4,0x05d9,0x05de,0x05e4,0x05e9,0x05ef,0x05f4,
    0x05fa,0x05ff,0x0605,0x060a,0x0610,0x0616,0x061b,0x0621,
    0x0627,0x062c,0x0632,0x0638,0x063e,0x0643,0x0649,0x064f,
    0x0655,0x065b,0x0661,0x0666,0x066c,0x0672,0x0678,0x067e,
    0x0684,0x068a,0x0690,0x0696,0x069d,0x06a3,0x06a9,0x06af,
    0x06b5,0x06bb,0x06c2,0x06c8,0x06ce,0x06d4,0x06db,0x06e1,
    0x06e7,0x06ee,0x06f4,0x06fb,0x0701,0x0708,0x070e,0x0715,
    0x071b,0x0722,0x0728,0x072f,0x0736,0x073c,0x0743,0x074a,
    0x0751,0x0757,0x075e,0x0765,0x076c,0x0773,0x077a,0x0781,
    0x0788,0x078e,0x0795,0x079d,0x07a4,0x07ab,0x07b2,0x07b9,
    0x07c0,0x07c7,0x07ce,0x07d6,0x07dd,0x07e4,0x07eb,0x07f3,
    0x07fa,0x0802,0x0809,0x0810,0x0818,0x081f,0x0827,0x082e,
    0x0836,0x083e,0x0845,0x084d,0x0855,0x085c,0x0864,0x086c,
    0x0874,0x087b,0x0883,0x088b,0x0893,0x089b,0x08a3,0x08ab,
    0x08b3,0x08bb,0x08c3,0x08cb,0x08d3,0x08dc,0x08e4,0x08ec,
    0x08f4,0x08fd,0x0905,0x090d,0x0916,0x091e,0x0927,0x092f,
    0x0937,0x0940,0x0949,0x0951,0x095a,0x0962,0x096b,0x0974,
    0x097d,0x0985,0x098e,0x0997,0x09a0,0x09a9,0x09b2,0x09bb,
    0x09c4,0x09cd,0x09d6,0x09df,0x09e8,0x09f1,0x09fb,0x0a04,
    0x0a0d,0x0a16,0x0a20,0x0a29,0x0a33,0x0a3c,0x0a45,0x0a4f,
    0x0a59,0x0a62,0x0a6c,0x0a75,0x0a7f,0x0a89,0x0a93,0x0a9c,
    0x0aa6,0x0ab0,0x0aba,0x0ac4,0x0ace,0x0ad8,0x0ae2,0x0aec,
    0x0af6,0x0b00,0x0b0a,0x0b15,0x0b1f,0x0b29,0x0b33,0x0b3e,
    0x0b48,0x0b53,0x0b5d,0x0b68,0x0b72,0x0b7d,0x0b88,0x0b92,
    0x0b9d,0x0ba8,0x0bb2,0x0bbd,0x0bc8,0x0bd3,0x0bde,0x0be9,
    0x0bf4,0x0bff,0x0c0a,0x0c15,0x0c21,0x0c2c,0x0c37,0x0c42,
    0x0c4e,0x0c59,0x0c65,0x0c70,0x0c7c,0x0c87,0x0c93,0x0c9e,
    0x0caa,0x0cb6,0x0cc2,0x0ccd,0x0cd9,0x0ce5,0x0cf1,0x0cfd,
    0x0d09,0x0d15,0x0d21,0x0d2d,0x0d3a,0x0d46,0x0d52,0x0d5e,
    0x0d6b,0x0d77,0x0d84,0x0d90,0x0d9d,0x0da9,0x0db6,0x0dc3,
    0x0dcf,0x0ddc,0x0de9,0x0df6,0x0e03,0x0e10,0x0e1d,0x0e2a,
    0x0e37,0x0e44,0x0e51,0x0e5f,0x0e6c,0x0e79,0x0e87,0x0e94,
    0x0ea2,0x0eaf,0x0ebd,0x0ecb,0x0ed8,0x0ee6,0x0ef4,0x0f02,
    0x0f10,0x0f1d,0x0f2b,0x0f3a,0x0f48,0x0f56,0x0f64,0x0f72,
    0x0f81,0x0f8f,0x0f9d,0x0fac,0x0fba,0x0fc9,0x0fd7,0x0fe6,
    0x0ff5,0x1004,0x1012,0x1021,0x1030,0x103f,0x104e,0x105d,
    0x106d,0x107c,0x108b,0x109a,0x10aa,0x10b9,0x10c9,0x10d8,
    0x10e8,0x10f7,0x1107,0x1117,0x1127,0x1137,0x1147,0x1157,
    0x1167,0x1177,0x1187,0x1197,0x11a7,0x11b8,0x11c8,0x11d9,
    0x11e9,0x11fa,0x120a,0x121b,0x122c,0x123d,0x124e,0x125e,
    0x126f,0x1281,0x1292,0x12a3,0x12b4,0x12c5,0x12d7,0x12e8,
    0x12fa,0x130b,0x131d,0x132f,0x1340,0x1352,0x1364,0x1376,
    0x1388,0x139a,0x13ac,0x13bf,0x13d1,0x13e3,0x13f6,0x1408,
    0x141b,0x142d,0x1440,0x1453,0x1466,0x1478,0x148b,0x149e,
    0x14b2,0x14c5,0x14d8,0x14eb,0x14ff,0x1512,0x1526,0x1539,
    0x154d,0x1560,0x1574,0x1588,0x159c,0x15b0,0x15c4,0x15d8,
    0x15ed,0x1601,0x1615,0x162a,0x163e,0x1653,0x1667,0x167c,
    0x1691,0x16a6,0x16bb,0x16d0,0x16e5,0x16fa,0x1710,0x1725,
    0x173a,0x1750,0x1765,0x177b,0x1791,0x17a7,0x17bd,0x17d3,
    0x17e9,0x17ff,0x1815,0x182b,0x1842,0x1858,0x186f,0x1885,
    0x189c,0x18b3,0x18ca,0x18e1,0x18f8,0x190f,0x1926,0x193d,
    0x1955,0x196c,0x1984,0x199b,0x19b3,0x19cb,0x19e3,0x19fb,
    0x1a13,0x1a2b,0x1a43,0x1a5b,0x1a74,0x1a8c,0x1aa5,0x1abd,
    0x1ad6,0x1aef,0x1b08,0x1b21,0x1b3a,0x1b53,0x1b6d,0x1b86,
    0x1b9f,0x1bb9,0x1bd3,0x1bec,0x1c06,0x1c20,0x1c3a,0x1c54,
    0x1c6f,0x1c89,0x1ca3,0x1cbe,0x1cd9,0x1cf3,0x1d0e,0x1d29,
    0x1d44,0x1d5f,0x1d7a,0x1d96,0x1db1,0x1dcc,0x1de8,0x1e04,
    0x1e20,0x1e3b,0x1e57,0x1e74,0x1e90,0x1eac,0x1ec8,0x1ee5,
    0x1f02,0x1f1e,0x1f3b,0x1f58,0x1f75,0x1f92,0x1faf,0x1fcd,
    0x1fea,0x2008,0x2025,0x2043,0x2061,0x207f,0x209d,0x20bb,
    0x20da,0x20f8,0x2116,0x2135,0x2154,0x2173,0x2192,0x21b1,
    0x21d0,0x21ef,0x220f,0x222e,0x224e,0x226e,0x228e,0x22ae,
    0x22ce,0x22ee,0x230e,0x232f,0x234f,0x2370,0x2391,0x23b2,
    0x23d3,0x23f4,0x2415,0x2437,0x2458,0x247a,0x249c,0x24bd,
    0x24df,0x2502,0x2524,0x2546,0x2569,0x258b,0x25ae,0x25d1,
    0x25f4,0x2617,0x263a,0x265e,0x2681,0x26a5,0x26c9,0x26ed,
    0x2711,0x2735,0x2759,0x277e,0x27a2,0x27c7,0x27ec,0x2811,
    0x2836,0x285b,0x2880,0x28a6,0x28cc,0x28f1,0x2917,0x293d,
    0x2964,0x298a,0x29b0,0x29d7,0x29fe,0x2a25,0x2a4c,0x2a73,
    0x2a9a,0x2ac1,0x2ae9,0x2b11,0x2b39,0x2b61,0x2b89,0x2bb1,
    0x2bda,0x2c02,0x2c2b,0x2c54,0x2c7d,0x2ca6,0x2ccf,0x2cf9,
    0x2d23,0x2d4c,0x2d76,0x2da0,0x2dcb,0x2df5,0x2e20,0x2e4a,
    0x2e75,0x2ea0,0x2ecb,0x2ef7,0x2f22,0x2f4e,0x2f7a,0x2fa6,
    0x2fd2,0x2ffe,0x302a,0x3057,0x3084,0x30b1,0x30de,0x310b,
    0x3138,0x3166,0x3194,0x31c2,0x31f0,0x321e,0x324c,0x327b,
    0x32aa,0x32d8,0x3308,0x3337,0x3366,0x3396,0x33c6,0x33f6,
    0x3426,0x3456,0x3486,0x34b7,0x34e8,0x3519,0x354a,0x357b,
    0x35ad,0x35df,0x3610,0x3642,0x3675,0x36a7,0x36da,0x370c,
    0x373f,0x3773,0x37a6,0x37d9,0x380d,0x3841,0x3875,0x38a9,
    0x38de,0x3913,0x3947,0x397c,0x39b2,0x39e7,0x3a1d,0x3a53,
    0x3a89,0x3abf,0x3af5,0x3b2c,0x3b62,0x3b99,0x3bd1,0x3c08,
    0x3c40,0x3c77,0x3caf,0x3ce8,0x3d20,0x3d59,0x3d91,0x3dca,
    0x3e04,0x3e3d,0x3e77,0x3eb0,0x3eea,0x3f25,0x3f5f,0x3f9a,
    0x3fd5,0x4010,0x404b,0x4087,0x40c2,0x40fe,0x413b,0x4177,
    0x41b4,0x41f0,0x422d,0x426b,0x42a8,0x42e6,0x4324,0x4362,
    0x43a0,0x43df,0x441e,0x445d,0x449c,0x44dc,0x451c,0x455c,
    0x459c,0x45dc,0x461d,0x465e,0x469f,0x46e0,0x4722,0x4764,
    0x47a6,0x47e8,0x482b,0x486e,0x48b1,0x48f4,0x4938,0x497b,
    0x49bf,0x4a04,0x4a48,0x4a8d,0x4ad2,0x4b17,0x4b5d,0x4ba3,
    0x4be9,0x4c2f,0x4c75,0x4cbc,0x4d03,0x4d4b,0x4d92,0x4dda,
    0x4e22,0x4e6a,0x4eb3,0x4efc,0x4f45,0x4f8e,0x4fd8,0x5022,
    0x506c,0x50b7,0x5101,0x514c,0x5198,0x51e3,0x522f,0x527b,
    0x52c8,0x5314,0x5361,0x53ae,0x53fc,0x544a,0x5498,0x54e6,
    0x5534,0x5583,0x55d3,0x5622,0x5672,0x56c2,0x5712,0x5763,
    0x57b4,0x5805,0x5856,0x58a8,0x58fa,0x594d,0x599f,0x59f2,
    0x5a46,0x5a99,0x5aed,0x5b41,0x5b96,0x5bea,0x5c40,0x5c95,
    0x5ceb,0x5d41,0x5d97,0x5dee,0x5e45,0x5e9c,0x5ef4,0x5f4c,
    0x5fa4,0x5ffc,0x6055,0x60ae,0x6108,0x6162,0x61bc,0x6216,
    0x6271,0x62cc,0x6328,0x6384,0x63e0,0x643c,0x6499,0x64f6,
    0x6554,0x65b1,0x6610,0x666e,0x66cd,0x672c,0x678c,0x67ec,
    0x684c,0x68ac,0x690d,0x696f,0x69d0,0x6a32,0x6a94,0x6af7,
    0x6b5a,0x6bbe,0x6c21,0x6c85,0x6cea,0x6d4f,0x6db4,0x6e19,
    0x6e7f,0x6ee6,0x6f4c,0x6fb3,0x701b,0x7083,0x70eb,0x7153,
    0x71bc,0x7226,0x728f,0x72f9,0x7364,0x73cf,0x743a,0x74a6,
    0x7512,0x757e,0x75eb,0x7658,0x76c5,0x7733,0x77a2,0x7811,
    0x7880,0x78ef,0x795f,0x79d0,0x7a40,0x7ab2,0x7b23,0x7b95,
    0x7c08,0x7c7a,0x7cee,0x7d61,0x7dd5,0x7e4a,0x7ebf,0x7f34,
    0x7faa,0x8020,0x8097,0x810e,0x8185,0x81fd,0x8276,0x82ee,
    0x8368,0x83e1,0x845b,0x84d6,0x8551,0x85cc,0x8648,0x86c5,
    0x8741,0x87bf,0x883c,0x88bb,0x8939,0x89b8,0x8a38,0x8ab8,
    0x8b38,0x8bb9,0x8c3a,0x8cbc,0x8d3e,0x8dc1,0x8e44,0x8ec8,
    0x8f4c,0x8fd1,0x9056,0x90dc,0x9162,0x91e9,0x9270,0x92f7,
    0x937f,0x9408,0x9491,0x951b,0x95a5,0x962f,0x96ba,0x9746,
    0x97d2,0x985e,0x98eb,0x9979,0x9a07,0x9a96,0x9b25,0x9bb5,
    0x9c45,0x9cd5,0x9d67,0x9df8,0x9e8b,0x9f1d,0x9fb1,0xa045,
    0xa0d9,0xa16e,0xa203,0xa299,0xa330,0xa3c7,0xa45f,0xa4f7,
    0xa590,0xa629,0xa6c3,0xa75d,0xa7f8,0xa894,0xa930,0xa9cc,
    0xaa69,0xab07,0xaba6,0xac45,0xace4,0xad84,0xae25,0xaec6,
    0xaf68,0xb00a,0xb0ad,0xb151,0xb1f5,0xb29a,0xb33f,0xb3e5,
    0xb48c,0xb533,0xb5db,0xb683,0xb72c,0xb7d5,0xb880,0xb92a,
    0xb9d6,0xba82,0xbb2f,0xbbdc,0xbc8a,0xbd38,0xbde8,0xbe98,
    0xbf48,0xbff9,0xc0ab,0xc15d,0xc210,0xc2c4,0xc378,0xc42d,
    0xc4e3,0xc599,0xc650,0xc708,0xc7c0,0xc879,0xc933,0xc9ed,
    0xcaa8,0xcb63,0xcc20,0xccdd,0xcd9a,0xce59,0xcf18,0xcfd8,
    0xd098,0xd159,0xd21b,0xd2dd,0xd3a1,0xd465,0xd529,0xd5ef,
    0xd6b5,0xd77c,0xd843,0xd90b,0xd9d4,0xda9e,0xdb68,0xdc33,
    0xdcff,0xddcc,0xde99,0xdf67,0xe036,0xe106,0xe1d6,0xe2a7,
    0xe379,0xe44c,0xe51f,0xe5f3,0xe6c8,0xe79e,0xe874,0xe94c,
    0xea24,0xeafc,0xebd6,0xecb0,0xed8b,0xee67,0xef44,0xf022,
    0xf100,0xf1df,0xf2bf,0xf3a0,0xf481,0xf564,0xf647,0xf72b,
    0xf810,0xf8f5,0xf9dc,0xfac3,0xfbab,0xfc94,0xfd7e,0xfe69,
    0xff55,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
    0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff
};


// bytes 4096


extern const uint8_t sin_table[] PROGMEM = {
    0x00,0x00,0x01,0x02,0x03,0x03,0x04,0x05,
    0x06,0x07,0x07,0x08,0x09,0x0a,0x0a,0x0b,
    0x0c,0x0d,0x0e,0x0e,0x0f,0x10,0x11,0x12,
    0x12,0x13,0x14,0x15,0x15,0x16,0x17,0x18,
    0x18,0x19,0x1a,0x1b,0x1c,0x1c,0x1d,0x1e,
    0x1f,0x1f,0x20,0x21,0x22,0x22,0x23,0x24,
    0x25,0x25,0x26,0x27,0x28,0x28,0x29,0x2a,
    0x2b,0x2b,0x2c,0x2d,0x2e,0x2e,0x2f,0x30,
    0x30,0x31,0x32,0x33,0x33,0x34,0x35,0x36,
    0x36,0x37,0x38,0x38,0x39,0x3a,0x3a,0x3b,
    0x3c,0x3d,0x3d,0x3e,0x3f,0x3f,0x40,0x41,
    0x41,0x42,0x43,0x43,0x44,0x45,0x45,0x46,
    0x47,0x47,0x48,0x49,0x49,0x4a,0x4a,0x4b,
    0x4c,0x4c,0x4d,0x4e,0x4e,0x4f,0x4f,0x50,
    0x51,0x51,0x52,0x53,0x53,0x54,0x54,0x55,
    0x55,0x56,0x57,0x57,0x58,0x58,0x59,0x59,
    0x5a,0x5b,0x5b,0x5c,0x5c,0x5d,0x5d,0x5e,
    0x5e,0x5f,0x5f,0x60,0x60,0x61,0x61,0x62,
    0x62,0x63,0x63,0x64,0x64,0x65,0x65,0x66,
    0x66,0x67,0x67,0x68,0x68,0x69,0x69,0x69,
    0x6a,0x6a,0x6b,0x6b,0x6c,0x6c,0x6c,0x6d,
    0x6d,0x6e,0x6e,0x6e,0x6f,0x6f,0x70,0x70,
    0x70,0x71,0x71,0x71,0x72,0x72,0x73,0x73,
    0x73,0x74,0x74,0x74,0x75,0x75,0x75,0x75,
    0x76,0x76,0x76,0x77,0x77,0x77,0x77,0x78,
    0x78,0x78,0x79,0x79,0x79,0x79,0x7a,0x7a,
    0x7a,0x7a,0x7a,0x7b,0x7b,0x7b,0x7b,0x7b,
    0x7c,0x7c,0x7c,0x7c,0x7c,0x7d,0x7d,0x7d,
    0x7d,0x7d,0x7d,0x7d,0x7e,0x7e,0x7e,0x7e,
    0x7e,0x7e,0x7e,0x7e,0x7f,0x7f,0x7f,0x7f,
    0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
    0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
    0x80,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
    0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,
    0x7f,0x7f,0x7f,0x7f,0x7f,0x7e,0x7e,0x7e,
    0x7e,0x7e,0x7e,0x7e,0x7e,0x7d,0x7d,0x7d,
    0x7d,0x7d,0x7d,0x7d,0x7c,0x7c,0x7c,0x7c,
    0x7c,0x7b,0x7b,0x7b,0x7b,0x7b,0x7a,0x7a,
    0x7a,0x7a,0x7a,0x79,0x79,0x79,0x79,0x78,
    0x78,0x78,0x77,0x77,0x77,0x77,0x76,0x76,
    0x76,0x75,0x75,0x75,0x75,0x74,0x74,0x74,
    0x73,0x73,0x73,0x72,0x72,0x71,0x71,0x71,
    0x70,0x70,0x70,0x6f,0x6f,0x6e,0x6e,0x6e,
    0x6d,0x6d,0x6c,0x6c,0x6c,0x6b,0x6b,0x6a,
    0x6a,0x69,0x69,0x69,0x68,0x68,0x67,0x67,
    0x66,0x66,0x65,0x65,0x64,0x64,0x63,0x63,
    0x62,0x62,0x61,0x61,0x60,0x60,0x5f,0x5f,
    0x5e,0x5e,0x5d,0x5d,0x5c,0x5c,0x5b,0x5b,
    0x5a,0x59,0x59,0x58,0x58,0x57,0x57,0x56,
    0x55,0x55,0x54,0x54,0x53,0x53,0x52,0x51,
    0x51,0x50,0x4f,0x4f,0x4e,0x4e,0x4d,0x4c,
    0x4c,0x4b,0x4a,0x4a,0x49,0x49,0x48,0x47,
    0x47,0x46,0x45,0x45,0x44,0x43,0x43,0x42,
    0x41,0x41,0x40,0x3f,0x3f,0x3e,0x3d,0x3d,
    0x3c,0x3b,0x3a,0x3a,0x39,0x38,0x38,0x37,
    0x36,0x36,0x35,0x34,0x33,0x33,0x32,0x31,
    0x30,0x30,0x2f,0x2e,0x2e,0x2d,0x2c,0x2b,
    0x2b,0x2a,0x29,0x28,0x28,0x27,0x26,0x25,
    0x25,0x24,0x23,0x22,0x22,0x21,0x20,0x1f,
    0x1f,0x1e,0x1d,0x1c,0x1c,0x1b,0x1a,0x19,
    0x18,0x18,0x17,0x16,0x15,0x15,0x14,0x13,
    0x12,0x12,0x11,0x10,0x0f,0x0e,0x0e,0x0d,
    0x0c,0x0b,0x0a,0x0a,0x09,0x08,0x07,0x07,
    0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x00,
    0x00,0x00,0xff,0xfe,0xfd,0xfd,0xfc,0xfb,
    0xfa,0xf9,0xf9,0xf8,0xf7,0xf6,0xf6,0xf5,
    0xf4,0xf3,0xf2,0xf2,0xf1,0xf0,0xef,0xee,
    0xee,0xed,0xec,0xeb,0xeb,0xea,0xe9,0xe8,
    0xe8,0xe7,0xe6,0xe5,0xe4,0xe4,0xe3,0xe2,
    0xe1,0xe1,0xe0,0xdf,0xde,0xde,0xdd,0xdc,
    0xdb,0xdb,0xda,0xd9,0xd8,0xd8,0xd7,0xd6,
    0xd5,0xd5,0xd4,0xd3,0xd2,0xd2,0xd1,0xd0,
    0xd0,0xcf,0xce,0xcd,0xcd,0xcc,0xcb,0xca,
    0xca,0xc9,0xc8,0xc8,0xc7,0xc6,0xc6,0xc5,
    0xc4,0xc3,0xc3,0xc2,0xc1,0xc1,0xc0,0xbf,
    0xbf,0xbe,0xbd,0xbd,0xbc,0xbb,0xbb,0xba,
    0xb9,0xb9,0xb8,0xb7,0xb7,0xb6,0xb6,0xb5,
    0xb4,0xb4,0xb3,0xb2,0xb2,0xb1,0xb1,0xb0,
    0xaf,0xaf,0xae,0xad,0xad,0xac,0xac,0xab,
    0xab,0xaa,0xa9,0xa9,0xa8,0xa8,0xa7,0xa7,
    0xa6,0xa5,0xa5,0xa4,0xa4,0xa3,0xa3,0xa2,
    0xa2,0xa1,0xa1,0xa0,0xa0,0x9f,0x9f,0x9e,
    0x9e,0x9d,0x9d,0x9c,0x9c,0x9b,0x9b,0x9a,
    0x9a,0x99,0x99,0x98,0x98,0x97,0x97,0x97,
    0x96,0x96,0x95,0x95,0x94,0x94,0x94,0x93,
    0x93,0x92,0x92,0x92,0x91,0x91,0x90,0x90,
    0x90,0x8f,0x8f,0x8f,0x8e,0x8e,0x8d,0x8d,
    0x8d,0x8c,0x8c,0x8c,0x8b,0x8b,0x8b,0x8b,
    0x8a,0x8a,0x8a,0x89,0x89,0x89,0x89,0x88,
    0x88,0x88,0x87,0x87,0x87,0x87,0x86,0x86,
    0x86,0x86,0x86,0x85,0x85,0x85,0x85,0x85,
    0x84,0x84,0x84,0x84,0x84,0x83,0x83,0x83,
    0x83,0x83,0x83,0x83,0x82,0x82,0x82,0x82,
    0x82,0x82,0x82,0x82,0x81,0x81,0x81,0x81,
    0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
    0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
    0x80,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
    0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
    0x81,0x81,0x81,0x81,0x81,0x82,0x82,0x82,
    0x82,0x82,0x82,0x82,0x82,0x83,0x83,0x83,
    0x83,0x83,0x83,0x83,0x84,0x84,0x84,0x84,
    0x84,0x85,0x85,0x85,0x85,0x85,0x86,0x86,
    0x86,0x86,0x86,0x87,0x87,0x87,0x87,0x88,
    0x88,0x88,0x89,0x89,0x89,0x89,0x8a,0x8a,
    0x8a,0x8b,0x8b,0x8b,0x8b,0x8c,0x8c,0x8c,
    0x8d,0x8d,0x8d,0x8e,0x8e,0x8f,0x8f,0x8f,
    0x90,0x90,0x90,0x91,0x91,0x92,0x92,0x92,
    0x93,0x93,0x94,0x94,0x94,0x95,0x95,0x96,
    0x96,0x97,0x97,0x97,0x98,0x98,0x99,0x99,
    0x9a,0x9a,0x9b,0x9b,0x9c,0x9c,0x9d,0x9d,
    0x9e,0x9e,0x9f,0x9f,0xa0,0xa0,0xa1,0xa1,
    0xa2,0xa2,0xa3,0xa3,0xa4,0xa4,0xa5,0xa5,
    0xa6,0xa7,0xa7,0xa8,0xa8,0xa9,0xa9,0xaa,
    0xab,0xab,0xac,0xac,0xad,0xad,0xae,0xaf,
    0xaf,0xb0,0xb1,0xb1,0xb2,0xb2,0xb3,0xb4,
    0xb4,0xb5,0xb6,0xb6,0xb7,0xb7,0xb8,0xb9,
    0xb9,0xba,0xbb,0xbb,0xbc,0xbd,0xbd,0xbe,
    0xbf,0xbf,0xc0,0xc1,0xc1,0xc2,0xc3,0xc3,
    0xc4,0xc5,0xc6,0xc6,0xc7,0xc8,0xc8,0xc9,
    0xca,0xca,0xcb,0xcc,0xcd,0xcd,0xce,0xcf,
    0xd0,0xd0,0xd1,0xd2,0xd2,0xd3,0xd4,0xd5,
    0xd5,0xd6,0xd7,0xd8,0xd8,0xd9,0xda,0xdb,
    0xdb,0xdc,0xdd,0xde,0xde,0xdf,0xe0,0xe1,
    0xe1,0xe2,0xe3,0xe4,0xe4,0xe5,0xe6,0xe7,
    0xe8,0xe8,0xe9,0xea,0xeb,0xeb,0xec,0xed,
    0xee,0xee,0xef,0xf0,0xf1,0xf2,0xf2,0xf3,
    0xf4,0xf5,0xf6,0xf6,0xf7,0xf8,0xf9,0xf9,
    0xfa,0xfb,0xfc,0xfd,0xfd,0xfe,0xff,0x00
};


// bytes 1024
