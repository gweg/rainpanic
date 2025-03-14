//   WAVES OF RAINDROP DATA 
//   Starting Process :   Goyo
//   Ending Levels    :  DmaCoco
//
// RAIN[] -> raindrop array [XPOS(drop), DELAY(between 2 drop),,,,]
// rain code signification :
//    0-40 = rain drop horizontal position. Must be bewteen 6 and 32 x position (7 left---19 center -- right 31) with step of 2 )
//           possible values of x : 7 9  11  13  15  17  19  21  23  25  27  29  31  
//                                  8 10  12  14  16  18  20  22  24  26  28  30  32
//    255 = next wave
//    250 = cat appear on the left (cardir)
//    251 = cat appear on the right
//    252 = fireball appear on the left
//    253 = fireball appear on the right
//    200-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)
//    180-196 = Life fall 
//    150-176 = ice fall

// 0x08 or 0x09 must be in hexa don't know why. Its look lika a bug ?!

unsigned char rain[] = 
{ //----------------------------------------------|new wave|

  13,2,18,1,216,4,21,1,219,1, 
  25,2,25,1,27,1,29,1,29,1,23,0,29,1,25,0,27,4,
  19,1,167,4,21,1,169,3,

  11,0,13,0,15,1,11,0,15,1,11,0,13,0,15,3,
  16,0,20,1,16,0,18,0,20,1,16,0,18,0,20,3,
  21,1,21,1,21,3,
  25,0,27,0,29,1,25,1,25,0,27,0,29,3,255,0,

 // wave 1 echauffement
  18,4,166,1,7,3,31,3, // gouttes init
  25,1,21,1,17,3,
  223,2,     // eclair pos
  250,4,    // chat gauche
  251,4,    // chat droite
  7,2,11,2,15,2,
  207,3,     // eclair pos 8
  252,6,    // fireball gauche 
  15,2,
  253,4,    // fireball droite
  31,2,25,2,21,2,
  7,1,9,0,11,1,13,1,13,0,15,1,15,0,17,1,21,1,23,0,25,1,27,1,29,0,31,4,
  
   //   **, **    
  //  *  *  *
  // *       *  O R I C
  //  *     *
  //    * * 
  //     *

  19,1,17,0,21,1,15,0,23,1,167,0,15,0,19,0,23,4,17,0,
  19,0,21,2,13,0,15,1,11,1,11,1,13,0,15,2, 23,0,25,0,
  27,1,23,1,23,0,25,1,23,1,23,0,25,0,27,2, 0x09,0,11,1,
  7,0,13,1,7,0,13,1,7,0,13,1,0x09,0,11,4,255,0,

  // Wave 2
  // introduction du chat
  17,1,19,1,21,1,250,4,21,1,19,1,17,1,251,4,21,0,23,0,
  25,1,25,0,23,0,21,2, 11,0,13,0,15,1,15,0,13,0,11,2,
  169,1,167,1,163,2, 07,0,0x09,0,11,1,11,0,0x09,0,7,3,250,2,
  17,2,17,2,165,0,251,4,

  15, 1, 11, 2, 21, 3, 23, 1, 11, 0, 169, 3, 169, 4, 19, 1, 31, 1, 25, 2, 31, 2, 19, 1, 7, 2, 11, 0, 170, 1,
  250,1,29, 5, 31, 1, 25, 2, 11, 6, 23, 2, 25, 1, 11, 1, 0x09, 0, 164, 1, 158, 1, 23, 2, 23, 1, 0x09, 4, 27, 2, 
  29, 5, 31, 1, 25, 2, 11, 6, 23, 2, 25, 1, 11, 1, 0x09, 0, 167, 1, 169,1, 251,1,23, 2, 23, 1, 0x09, 4, 27, 2, 255, 0,

  // Wave 3
  13, 3, 11, 1, 21, 2, 11, 1, 17, 1, 13, 2, 31, 5, 11, 1, 13, 6, 17,1,15,1,
  13, 3, 215,1,0x09, 2, 31, 4, 25, 1, 25, 1, 0x09, 2, 11, 1, 13, 1, 15, 3, 17, 1, 7, 3, 
  13, 1, 0x09, 1, 13, 2, 23, 1, 21,1,19,1,17,1,15,1,165,1,167,1,163,1,

  11, 1, 19, 1, 31, 1, 11, 1, 25, 0, 162, 1, 19, 5, 25, 1, 15, 2, 29, 1, 27, 3, 21, 2, 29, 2,
  13, 0, 172, 2, 25, 0, 164, 1, 7, 1, 29, 0, 158, 1, 25, 5, 25, 6, 13, 4, 
  21, 2, 31, 2, 17, 0, 162, 2, 19, 1, 27, 1, 255, 0,

// Wave 4
    11,1,29,1, 9, 1, 175, 1, 21, 2, 17, 2, 25, 1, 205, 1, 21, 0, 157, 1, 13, 1, 13, 0, 157, 1, 17, 3, 21, 1, 
    19, 1, 193, 1, 197, 1, 185, 1, 15, 1, 13,1,11,1,17,1, 159, 0, 205, 1, 29, 0, 191, 1, 23, 0, 171, 1, 171, 2, 17, 0, 169, 3,
    19, 0, 187, 1, 25, 0, 165, 2, 17, 1,29, 4, 203, 3, 159, 1, 27, 1, 21, 1, 0x09, 3, 189, 1, 165, 1, 25, 3, 27,1,31, 1,

    13, 1, 188, 1, 224, 3, 23, 2, 23, 1, 27, 1, 19, 1, 17, 2, 25, 1, 23, 0, 216, 1, 
    23, 0, 194, 1, 190, 1, 11, 1, 25, 2, 188, 2, 210, 0, 202, 2, 0x09, 3, 
  0x09, 1, 21, 1, 13, 1, 162,0, 23, 2, 29, 1, 25, 1, 11, 0, 165,0, 185, 1, 155,1, 199, 1, 27, 1, 29, 0, 195, 6, 25, 1,
  205, 1, 31, 1, 215, 1, 0x09, 1, 25, 1, 23, 7, 185, 3, 155,1, 15, 1, 27, 1, 21, 1, 25, 2, 
  255, 0,

// Wave 9
  23, 1, 29, 1, 23, 1, 23, 1, 217, 1, 17, 5, 17, 3, 13, 2, 29, 4, 15, 1,
  204, 1, 21, 4, 15, 0, 203, 1, 19, 2, 13, 1, 25, 1, 211, 2, 23, 3, 207, 4, 208, 2,
  11, 1, 212, 1, 29, 1, 220, 0, 183, 1, 23, 3, 27, 1, 198, 2, 23, 1, 23, 0, 208, 1, 208, 1, 23, 0, 224, 4, 216, 1, 23, 2,
  21, 0, 218, 0, 202, 1, 15, 0, 204, 1, 212, 1, 13, 1, 7, 1, 23, 1, 7, 2, 31, 3, 29, 1, 23, 4, 
  255, 0,

// Wave 7
  11,1,15,1,19,1,15,1,11,0, 165,1,185,1,205,1,
  25, 0, 194, 2, 0x09, 2, 166, 1, 17, 1, 31, 1, 0x09, 3, 221, 3, 13, 0, 
  169, 1, 213, 1, 13, 1, 19, 1, 27, 1, 21, 0, 159, 3, 

  11, 1, 213, 1, 221, 1, 31, 2, 157, 2, 11, 3, 0x09, 1, 211, 2, 215, 1, 17, 4,
  11, 1, 29, 3, 19, 1, 194, 1, 205, 1, 7, 1, 218, 1, 170, 2, 19, 1, 17, 1, 17, 2, 25, 3, 23, 3,
  166, 0, 187, 2, 19, 1, 170, 1, 31, 1, 21, 3, 19, 0, 209, 1, 13, 4, 29, 0, 160, 3, 255, 0,

// Wave 8
  31, 1, 7, 4, 159, 1, 163,0,167,1, 159,0,31, 0, 221,1, 217,0, 251, 4, 215, 2, 27, 0,159, 1, 21,0,157, 0, 15,1,179,0, 251, 0, 166, 5,
  11, 1, 23, 2, 250, 1, 0x09, 2, 205, 1, 19, 3, 29, 2, 29, 2, 29, 1, 11, 1, 15, 0, 224, 1, 0x09, 1, 0x09, 2,
  7, 3, 15, 1, 250, 0, 205, 1, 19, 1, 17, 1, 207, 1, 11, 1, 164, 1, 29, 1, 0x09, 1, 17, 0, 220, 3, 17, 0, 206, 1, 174, 2, 21, 1, 7, 0, 158, 2,
  17, 1, 206, 4, 23, 1, 19, 2, 27, 3, 

// wave 9
  0x09,1, 11,1, 15,1, 19,1, 23,1, 27,1, 31,1,
  31,1,27,1,23,0,19,0,15,1,11,0,7,1,
  9,1,250,2,19,1,31,1,27,2,251,3,27,2,21,1,15,2,7,2,
  255,0,

// wave 10 time=3/2      x position (7 left---19 center -- right 31) 
  19,2, 31,2, 7,2,  19,2, 32,3, 250,0, 7,2, 251,0, 32,3, 19,3, 07,3,  32,3,  251,0 ,7,3, 16,3, 250,0, 18,3, 31,3,
  07,3  ,9,3  ,250,0 ,26,3  ,28,3, 251,0,  32,3, 7,3, 250,0, 32,3,   15,3  ,17,3, 251,0 ,19,3  ,21,3, 250,0, 23,3, 
  07,3 ,9,3 ,11,3 ,251,0 ,28,3  ,32,3,  250,0, 07,3, 251,0 ,32,3, 22,3,  251,0, 21,3,  250,0, 7,3, 251,0, 32,2, 250, 0 , 19,6,  // 39eme

  // wave 5
  250,0, 0x08,3,212,0, 13,2,  
  29,3, 251,0, 0x09,3, 230,0, 28,3, 251,0, 10,3, 251,0, 13,4, 230,0, 30,4,  16,4, 208,0, 
  07,4, 250,0, 32,4, 18,4, 07,4, 10,4, 251,0, 15,4, 11,4, 24,4, 
  255,0, 

  // wave 3 time =2 assez difficile
  13,3 ,30,2, 251,0, 07,3, 215,0, 32,3, 250,0 ,18,3, 250,0, 11,3, 27,2, 217,0, 
  11,2, 251,0, 29,3, 250,0, 11,3, 251,0, 29,3,  250,0, 0x09,3, 207,0, 
  31,2, 17,3, 217,0, 32,2, 224,0, 7,1, 11,1,13,1, 15,1,19,6, 
  
  // wave 4
  14,2, 251,0 ,25,2,  215,0, 250,0, 32,2, 07,2, 251,0, 32,2, // 73eme
  14,2,250,0, 16,2, 251,0, 18,2, 250,0, 20,2,217,0,14,2,218,0,32,2,
  07,2, 32,2, 251,0, 9,3, 220,0, 168,3, // ice here  reprendre ici 
  32,3, 07,3, 215,0, 31,3, 215,0, 07,3, 216,0, 30,3, 
  255,0,  

  // wave 6 the end
  19,1, 193,2,                   // vie suppliémentaire
  9,1, 11,1, 251,0, 13,1, 15,1, 17,1,19,3, 
  25,1,23,1,21,2,250,0, 19,1, 17,1, 15,2, 250,0, 25,2,27,4,
  250,0, 15,3, 251,0, 17,0 ,19,1, 17,0,19,2, 
  250,0, 7,0, 9,1, 7,0,9,2, 250,0, 11,0, 13,1,11,0, 13,2, 250,0, 
  13,0 ,15,0, 17,3,251,0, 
  
  23,0, 251,0, 25,0, 27,0, 29,2, 
  7,0, 250,0, 9,0, 11,0, 251,0, 13,0 ,15,3,
  21,0, 23,0, 25,0, 27,0, 29,0, 31,3,
  7,0,9,0,11,1, 14,0,16,0,18,1, 21,0,23,0,25,1, 27,1, 
  19,2, 19,4, 
  250,0,161,1,163,1,167,1,169,1,253,0

    // one line
  //  [x,y] y =0 -> force to align drops in same line
 };
