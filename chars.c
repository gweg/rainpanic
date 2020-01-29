
#include "const.h"

extern unsigned int  i,j;

// redefined 35 characters for graphics and sprites game
unsigned char redefchar[]={

01,01,01,01,02,03,06,06, // 91
63,63,63,63,63,30,00,30, // 92
32,32,32,32,16,48,24,24, // 93
06,06,06,06,07,03,01,00, // 94
63,45,63,51,30,45,63,63, // 95
24,24,24,24,56,48,32,00, // 96
30,63,45,63,51,45,63,30, // 97  // ungry head   - ancien blanc coté gauche du ventre, peut etre recyclé
63,63,63,00,63,63,63,51, // 98
00,30,47,63,63,63,30,00, // 99 - ice boulder
00,00,00,00,00,00,01,01, // 100
51,51,51,00,30,30,63,63, // 101
00,00,00,00,00,00,32,32, // 102
01,01,01,00,01,01,07,07, // 103
51,51,51,00,33,33,33,33, // 104
32,32,32,00,32,32,56,56, // 105
63,63,63,51,30,45,63,63, // 106
0x08,0x08,28,28,62,62,46,28,  // 107   goutte à rattraper 1/2   - bug with 8 value ? must be hex 0x08
00,00,00,00,00,00,00,00, // 108  goutte à rattraper 2/2
00,00,04,04,14,14,10,04, // 109
00,00,00,00,00,00,00,00, // 110
30,63,45,63,63,45,51,30, // 111 head of life number
04,28,31,15,03,03,02,06, // 112 right cat  // bug a cette position dans la memoire verifier si addresse utilsiée ailleurs
01,01,62,60,60,12,38,42, // 113 right cat
16,16,15,07,07,06,12,10, // 114 left cat
04,07,63,62,56,24,40,44, // 115 left cat
00,00,00,00,06,06,07,07, // 116
00,00,00,00,00,03,47,47, // 117
00,00,00,00,00,48,61,61, // 118
00,00,00,00,24,24,56,56, // 119
04,35,0x9,07,07,0x9,35,04, // 120  fire boulder left 04,34,0x09,07,07,0x09,34,04
0x08,17,36,56,56,36,17,0x08, // 121  fire boulder right
62,62,62,62,62,62,62,28, // 122 tile
03,06,12,31,00,00,00,01, // 123 lightning left
00,00,00,60,12,24,48,32, // 124 lightning right
03,02,04,0x08,16,32,00,00// 125 lightning head character redefine
// 126, 127 allready used in them native graphics
}; 

unsigned char redefcharExt[]={

// happy aldo: player les bras en bas 
00,00,00,00,00,00,00,30, // 34 à haut de la tête
00,00,00,00,00,00,00,01, // 35 à gauche de la tête
00,00,00,00,00,00,00,32, // 36 à droite de la tête
03,03,06,06,06,00,06,06, // 37 à gauche du corps
48,48,24,24,24,00,24,24, // 38 à droite du corps
22,61,63,63,30,30,12,00 // 39 kernel

};

// to redefine rain drop char ( 2 char to make drop scrolling over 2 char
void redefine_raindrop()
{
	*(unsigned char*)(CARADDR+864)=8;// == poke (CARADDR+864,8);
	*(unsigned char*)(CARADDR+865)=8;
	*(unsigned char*)(CARADDR+866)=28;
	*(unsigned char*)(CARADDR+867)=28;
	*(unsigned char*)(CARADDR+868)=62;
	*(unsigned char*)(CARADDR+869)=62;
	*(unsigned char*)(CARADDR+870)=46;
	*(unsigned char*)(CARADDR+871)=28;//poke (CARADDR+871,28);
	// 108 blanc reservé pour drop_sliding goutte sur 2eme car
	*(unsigned char*)(CARADDR+856)=0;
	*(unsigned char*)(CARADDR+857)=0;
	*(unsigned char*)(CARADDR+858)=0;
	*(unsigned char*)(CARADDR+859)=0;
	*(unsigned char*)(CARADDR+860)=0;
	*(unsigned char*)(CARADDR+861)=0;
	*(unsigned char*)(CARADDR+862)=0;
	*(unsigned char*)(CARADDR+863)=0;	
}

// redefine characters
void redefine_char()
{
   // 0xB400 - 46080 beginning of charset
   // 46808 = char no 86
  
   char startchar;
   j=0;
   for (i=46808;i<46808+sizeof(redefchar)-1;i++)
	  *(unsigned char*)i=redefchar[j++];
}

// happy ALDO
void redefine_charExt()
{
   // 0xB400 - 46080 beginning of charset
   // 46352 = char no 34
   char startchar;
   j=0;
   for (i=46352;i<46352+sizeof(redefcharExt)-1;i++)
	  *(unsigned char*)i=redefcharExt[j++];
}
