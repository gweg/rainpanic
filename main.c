//
//  RAIN PANIC   v1.0 2021
//  Author : GOYO 
//
//  utilisation ram : on peut descendre à #600 environ (la zone basic démarrant à #400)
//  HI user ram  #9800
//
// reste à implémenter, améliorer:
// ------------------------------
//
// - les différents niveaux -> toutes les vagues de pluie.
//
// - gestion de l'activation/desactivation du son et de la musique
// 
// - jambes qui marchent durant le déplacement continue
//
// - collision avec ice : faire tomber Aldo au sol
//
// - implémenter le circle du soleil en asm si possible.
//
// - révision du 31/08/2021
//
//		affichage du sol après chaque collision avec Aldo afin d'éffacer les traces de ses pieds.
//
// BUGS :
// ------
//  
//  - cat shape
//
//  - legs traces
//
/*
  Oric Standar Charset
   =32 !=33 "=34 #=35 $=36 %=37 &=38 '=3
  9 (=40 )=41 *=42 +=43 ,=44 -=45 .=46 /
  =47 0=48 1=49 2=50 3=51 4=52 5=53 6=54
   7=55 8=56 9=57 :=58 ;=59 <=60 ==61 >=
  62 ?=63 @=64 A=65 B=66 C=67 D=68 E=69
  F=70 G=71 H=72 I=73 J=74 K=75 L=76 M=7
  7 N=78 O=79 P=80 Q=81 R=82 S=83 T=84 U
  =85 V=86 W=87 X=88 Y=89 Z=90 [=91 \=92
   ]=93 ^=94 _=95 `=96 a=97 b=98 c=99 d=
  100 e=101 f=102 g=103 h=104 i=105 j=10
  6 k=107 l=108 m=109 n=110 o=111 p=112
  q=113 r=114 s=115 t=116 u=117 v=118 w=
  119 x=120 y=121 z=122 {=123 |=124 }=12
  5 ~=126 =127
  */
  /* screen coding

		   DELAY:248           WAT=0L=0
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  mm~ zzzzzzzzzzzzzzzzzzzzzzzzzzzz ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~           WAVE 0             ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                              ~mm
  mm~                  y           ~mm
  mm~               [\]            ~mm
  mm~               ^_`            ~mm
  mm~              tubvw           ~mm
  ~~~                              ~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~o~o~o~o~~~~~~~~~~  DROPS   0  ~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  */

#include <lib.h>



#define RAINDROPMAX 20       // max drop sprite in the screen
#define jump_max_time 8      // time to keep in jump state
#define benddown_max_time 8     // time to Aldo benddown
#define wait_to_music_delay 6  // delay to temporize the music

#define SCRADDR 0xBB80   	// address of the text screen
#define CARADDR 0xB400   	// address of std chars to redefine them
#define def_ceiling_pos 6 	// default ceiling position =6
  // arrays to manage rain drops, for : x y timer state alstate
#define floor_y 25

#undef DEBUG              // to switch to debug mode !!! : define/undef

unsigned char raindropx[RAINDROPMAX];
unsigned char raindropy[RAINDROPMAX];
//unsigned char raindroptimer[RAINDROPMAX]="";
unsigned char raindropstate[RAINDROPMAX] = "";

int           index_raindrop;    // index to read from the rain[] array

unsigned char codedrop;          // to read drop x position value in Rain[] array.
int           raindroptime;      // to read time drop => time between two drops

unsigned int  timer1, timer2;

char 		  drop_catch_time;   // paper 6 during this time to indicate catch is successfull
char 		  shoot_cat_time;    // paper cyan during this time to indicate shoot with the cat
char          shoot_tile_time;   //
char 		  shoot_fireball_time;
char          drop_floor_time;   //
char          endgame;           // loose game = 1 winned =2
unsigned int  scroll_text;
unsigned char scroll_text_time;
char          px, py;             // ALDO position x & y
unsigned char armsdown;  		 // arms set down
unsigned char legsup;  			 //   legs set up
char          waterlevel;		 // level of water
int           score;		     // game score
int           music_index;       // music in for music array indexing
unsigned char wait_to_music;     // wait time to continue to playing music
unsigned int  wait, wait2;	     // global variable to manage wait time
char          active_sound;      // activated sound state
char          active_music;      // activated music state
char          volume;		     // game sound volume
char          walking;           // manage walking
char 		  walking_alt;       // alternate legs chars to walking

unsigned char jump_time = 0;       // gestion du saut du héro 
unsigned char benddown = 0;		 // time to avoid the fireball
unsigned char standuptime;       // time to standup : to avoid to jump just after stand up

char unsigned catx;              // x cat position 
char unsigned caty;              // y cat position 
char unsigned cat;               // pour savoir si le cat est prensent ou pas
char          dircat;            // direction du cat
char unsigned catwait;           // delai du cat qui reste sur place avant de partir
char          seecat;            // if cat is visible  
char 		  colcat;			 // enable collision with the cat
unsigned char colcattime;	     // duration of collision of Aldo with the cat enabled

char unsigned fireballx;         // x fireball position 
char unsigned firebally;         // y fireball position 
char unsigned fireball;          // pour savoir si le fireball est prensent ou pas
char unsigned dirfireball;       // direction du fireball
char unsigned fireballwait;      // delai du fireball qui reste sur place avant de partir



char unsigned seefireball;
char unsigned colfireballtime;   // duration of collision of Aldo with fireball enabled

char unsigned player_wait;       // close eyes afeter no activity
unsigned char k, lastk;           // to read the key pressed  : k=key(); 	

char unsigned wave_nbr;
char          ceiling_y;         // hauteur du plafond = 8 fixe

unsigned int  game_timer;        // compteur de temps général durant le jeu
char          life; 			 // player lifes
char unsigned tile_fall;         // if tile fall
char unsigned lightning;         // if lightning actived
int  unsigned lightning_time;	 // duration of lightning
char unsigned tile_x;            // position x of the beam
char unsigned tile_y;            // position y of the beam

unsigned char rdindex;

char          altchar;			 // to alternate char for the falling rain drop
unsigned char drop_sliding;		 // y step of vertically scrolling for the rain drop
int           adr;   		     // use for assembly inline
unsigned int  i, j;               // usefull in many time in this program  

unsigned      display_wave_level_timer; // used to display the wave level


unsigned char lx;                // used to display lightning
unsigned char ly;                // used to display lightning


unsigned char being_falling;
// other falling object
unsigned char obj_kernel;
unsigned char obj_ice;
unsigned char obj_x;
unsigned char obj_y;

// manque son pour le JUMP, la retombé du jump, l'eau tombe à terre
unsigned char bong_sound[] = { 104,177,184,44,254,29,166,58,27,83,35,74,2,83 };
unsigned char cat_collision_sound[] = { 102,144,100,122,168,7,252,9,215,92,153,10,10,1 };
unsigned char cat_at_door_sound[] = { 28,188,255,21,247,92,136,18,176,229,253,145,1,9 };
unsigned char rain_menu_sound[] = { 105,243,91,101,202,108,209,215,73,142,13,178,71,11 };
unsigned char silence_sound[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
unsigned char player_fallout_sound[] = { 68,64,207,83,241,143,141,119,218,145,164,167,2,211 };
unsigned char player_jump_sound[] = { 238,2,0,0,0,0,77,14,16,0,0,0,4,4 };

unsigned char scroll_y;
unsigned char scroll_y_nbr;
unsigned char scroll_timer;



// external functions
void AdvancedPrint(char x_pos, char y_pos, const char* ptr_message);

// internal functions
void drop_sliding_outside();
void scroll_right(char y, char nbr);
void disp_aldo(char x, char y);
void unshoot();
void display_floor();
void display_ceiling();

char credits_text[] = "             RAIN PANIC  (2020)                "
"           INSTRUCTIONS :  DURING A STRONG THUNDERSTORM HELP MR ALDO TO CATCH ALL THE DROPS OF WATER FALLING FROM HIS HOUSE CEILING "
"           BE CARREFULL OF ROOF TILE FALLING         "
"    USE LEFT AND RIGHT ARROWS TO MOVE ALDO (OR O,P)"
"    USE SPACE TO JUMP        ESC TO EXIT   "
"                                                    "
"  CODING BY GOYO IN C LANGUAGE WITH ORIC OSDK AND FEW INLINE ASSAMBLY CODE              ORIGINAL MUSIC : LADYWASKY      "
"  THANKS TO THE HELP OF MANY MEMBERS OF COE FORUM : DBUG  RETORIC JIBE"
"  LADYWASKY  DRPSY  KENNETH  ISS AND OTHERS ORICIANS ..           "
"  HAVE FUN .....                     ";

// Music : Rainy Day(c) , by LADYWASKY
char game_music[] = {
1,7,12,4,0,0,0,0, 1,7,12,4,0,0,0,0, 1,7,11,4,0,0,0,0, 1,7,11,4,0,0,0,0, 1,7,9,4,0,0,0,0, 3,3,9,4,8,2,0,0,
//Intro,A2,,,,,,,
6,7,0,0,8,3,4,4,  3,3,9,4,8,2,0,0, 7,7,11,4,8,3,4,4, 2,3,0,0,7,2,0,0, 7,7,11,4,7,3,2,4, 2,3,0,0,7,2,0,0,
7,7,11,4,7,3,2,4, 2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,3,3,12,4,5,2,0,0,
7,7,11,4,5,3,12,4,3,3,11,4,5,2,0,0,7,7,9,4,5,3,12,4, 3,3,9,4,8,2,0,0, 6,7,0,0,8,3,4,4,  3,3,12,4,8,2,0,0,
7,7,2,5,8,3,4,4,  2,3,0,0,7,2,0,0, 7,7,12,4,7,3,2,4, 3,3,11,4,7,2,0,0,7,7,9,4,7,3,2,4,  2,3,0,0,5,2,0,0,
6,7,0,0,5,3,12,4, 2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,1,7,12,4,0,0,0,0,3,3,12,4,5,2,0,0, 7,7,11,4,5,3,12,4,
3,3,11,4,5,2,0,0, 6,7,9,4,5,3,12,4,2,3,9,4,8,2,0,0,  6,7,0,0,8,3,4,4, 3,3,12,4,8,2,0,0, 6,7,0,0,8,3,4,4,
3,3,7,4,7,2,0,0,  6,7,0,0,7,3,2,4, 2,3,0,0,7,2,0,0, 7,7,12,4,7,3,2,4, 3,3,11,4,5,2,0,0, 7,7,9,4,5,3,12,4,
3,3,11,4,5,2,0,0,7,7,12,4,5,3,12,4,2,3,0,0,5,2,0,0, 6,7,0,0,5,3,12,4, 2,3,0,0,5,2,0,0,  6,7,0,0,5,3,12,4,
3,3,12,4,8,2,0,0,
//Boucle,A3,,,,,,,
6,7,0,0,8,3,4,4,  3,3,7,4,8,2,0,0, 7,7,7,4,8,3,4,4,  2,3,0,0,7,2,0,0, 7,7,4,4,7,3,2,4,  3,3,7,4,7,2,0,0,
7,7,7,4,7,3,2,4,  2,3,0,0,5,2,0,0, 6,7,0,0,5,3,12,4, 2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,3,3,12,4,5,2,0,0,
7,7,11,4,5,3,12,4,3,3,11,4,5,2,0,0,7,7,9,4,5,3,12,4, 3,3,9,4,8,2,0,0,
//Boucle,A2,,,,,,,
6,7,0,0,8,3,4,4,  3,3,9,4,8,2,0,0, 7,7,11,4,8,3,4,4, 2,3,0,0,7,2,0,0, 7,7,11,4,7,3,2,4, 2,3,0,0,7,2,0,0,
7,7,11,4,7,3,2,4, 2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,3,3,12,4,5,2,0,0,
7,7,11,4,5,3,12,4,3,3,11,4,5,2,0,0,7,7,9,4,5,3,12,4, 3,3,9,4,8,2,0,0, 6,7,0,0,8,3,4,4,  3,3,12,4,8,2,0,0,
7,7,2,5,8,3,4,4,  2,3,0,0,7,2,0,0, 7,7,12,4,7,3,2,4, 3,3,11,4,7,2,0,0,7,7,9,4,7,3,2,4,  2,3,0,0,5,2,0,0,
6,7,0,0,5,3,12,4, 2,3,0,0,5,2,0,0, 7,7,12,4,5,3,12,4,1,7,12,4,0,0,0,0,3,3,12,4,5,2,0,0, 7,7,11,4,5,3,12,4,
3,3,11,4,5,2,0,0, 6,7,9,4,5,3,12,4,2,3,9,4,8,2,0,0,  6,7,0,0,8,3,4,4, 3,3,12,4,8,2,0,0, 6,7,0,0,8,3,4,4,
3,3,7,4,7,2,0,0,  6,7,0,0,7,3,2,4, 2,3,0,0,7,2,0,0,  7,7,12,4,7,3,2,4,3,3,11,4,5,2,0,0, 7,7,9,4,5,3,12,4,
3,3,11,4,5,2,0,0, 7,7,12,4,5,3,12,4,2,3,0,0,5,2,0,0, 6,7,0,0,5,3,12,4,2,3,0,0,5,2,0,0, 6,7,0,0,5,3,12,4,
//Fin,A4,,,,,,,
3,3,2,5,8,2,0,0,  6,7,0,0,8,3,4,4, 3,3,7,4,8,2,0,0,  7,7,7,4,8,3,4,4, 2,3,0,0,7,2,0,0,  7,7,4,4,7,3,2,4,
3,3,7,4,7,2,0,0,  7,7,7,4,7,3,2,4, 3,3,5,4,5,2,0,0,  6,7,0,0,5,3,12,4,3,3,4,5,5,2,0,0,  6,7,0,0,5,3,12,4,
3,3,2,5,5,2,0,0,  1,7,9,4,0,0,0,0, 2,7,0,0,5,2,0,0,  2,7,0,0,5,2,0,0, 2,7,0,0,5,2,0,0 };


//
//   WAVES OF RAINDROP DATA 
//
// RAIN[] -> raindrop array [XPOS(drop), DELAY(between 2 drop),,,,]
//
// rain code signification :
//
// 0-40 = rain drop horizontal position. Must be bewteen 6 and 32 x position (7 left---19 center -- right 31) with step of 2 )
// possible values of x : 7 9  11  13  15  17  19  21  23  25  27  29  31  
//                         8 10  12  14  16  18  20  22  24  26  28  30  32
// 255 = next wave
// 250 = cat appear on the left (cardir)
// 251 = cat appear on the right
// 252 = fireball appear on the left
// 253 = fireball appear on the right
// 200-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)
// 180-196 = Life fall 
// 150-176 = ice fall

//WAVES DATA on a external file.
#include "waves.c"

#define SLOW_DOWN_DURATION 100

// redefined 35 characters for graphics and sprites game
unsigned char redefchar[] = {

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
unsigned char catfish[] = {
00,48,11,05,31,39,11,16, // 112 left catfish
16,49,59,63,62,63,59,33, // 113 left catfish (queue)
02,35,55,63,31,63,55,33, // 114 right fish
00,03,52,40,62,57,52,02  // 115 right fish
};
unsigned char catcat[] = {
04,28,31,15,03,03,02,06, // 112 right cat  
01,01,62,60,60,12,38,42, // 113 right cat
16,16,15,07,07,06,12,10, // 114 left cat
04,07,63,62,56,24,40,44 // 115 left cat}
};
unsigned char redefcharExt[] = {

	// happy aldo: player les bras en bas 
	00,00,00,00,00,00,00,30, // 34 à haut de la tête
	00,00,00,00,00,00,00,01, // 35 à gauche de la tête
	00,00,00,00,00,00,00,32, // 36 à droite de la tête
	03,03,06,06,06,00,06,06, // 37 à gauche du corps
	48,48,24,24,24,00,24,24, // 38 à droite du corps
	22,61,63,63,30,30,12,00 // 39 kernel
};
// redefine characters
void redefine_char()
{
	// 0xB400 - 46080 beginning of charset
	// 46808 = char no 86

	char startchar;
	j = 0;
	for (i = 46808; i < 46808 + sizeof(redefchar) - 1; i++)
		*(unsigned char*)i = redefchar[j++];
}

// happy ALDO
void redefine_charExt()
{
	// 0xB400 - 46080 beginning of charset
	// 46352 = char no 34
	char startchar;
	j = 0;
	for (i = 46352; i < 46352 + sizeof(redefcharExt) - 1; i++)
		*(unsigned char*)i = redefcharExt[j++];
}

void playSoundEffect(unsigned char * soundsequence,unsigned char waitformusic)
{	
	if (active_sound)
	{
	   SoundEffect(soundsequence) ;
	   wait_to_music = waitformusic;
	}
};

// game win animation
void wingame()
{
	int pos_scr;
	int i, j, x, y;
	unsigned char kk;

	// ici bruit de pluie qui s'arrete ?
	j = 0;
	//scanner tout l'écran afin de faire disparaitre les gouttes
	for (pos_scr = SCRADDR; pos_scr < SCRADDR + 1000; pos_scr++)
	{
		if (j++ == 39)
		{
			for (wait = 0; wait < 1000; wait++);
			j = 0;
		}

		drop_sliding_outside();
		// clear the rain
		if ((peek(pos_scr) == 109) ||
			(peek(pos_scr) == 108) ||
			(peek(pos_scr) == 107) ||
			(peek(pos_scr) == 22))
			poke(pos_scr, 32);
	}

	*(unsigned char*)(SCRADDR + (20 * 40) + 4) = 32;
	*(unsigned char*)(SCRADDR + (21 * 40) + 4) = 32;
	*(unsigned char*)(SCRADDR + (22 * 40) + 4) = 32;
	*(unsigned char*)(SCRADDR + (23 * 40) + 4) = 32;
	// Aldo exit the house
	for (j = 0; j < 31; j++)
	{
		for (wait = 0; wait < 1000; wait++);
		scroll_right(1, 23);
		if (walking == 0)
			walking = 1;
		else
			walking = 0;
		disp_aldo(px, py);
	}
	paper(3); //yellow
	//  the cat come to Aldo
	for (j = 3; j < px - 3; j++)
	{
		for (wait = 0; wait < 1500; wait++);
		*(unsigned char*)(SCRADDR + (caty * 40) + j - 2) = 32;
		*(unsigned char*)(SCRADDR + (caty * 40) + j - 1) = 0;
		*(unsigned char*)(SCRADDR + (caty * 40) + j) = 114;
		*(unsigned char*)(SCRADDR + (caty * 40) + j + 1) = 115;
	}
	*(unsigned char*)(0XBBAA + (40 * (caty - 2)) + j - 3) = 1; // color of kernel

	// clear the hires box 
	for (i = 40; i < 4480 + 40; i++)
		poke(0xA01B + i, 64);   // 64 = correct

	 // set the text mode in hires
	for (i = 40; i < 4480 + 40; i += 40)
		poke(0xA01B + i, 27);

	// set the hires mode in text
	for (i = 2; i < 14; i++)
		poke(SCRADDR + (i * 40) + 2, 31);

	// set the colors of inside box
	for (i = 2; i < 14; i++)
	{
		poke(SCRADDR + (i * 40) + 1, 7);  // white before hires blox
		poke(SCRADDR + (i * 40) + 28, 4); // blue before house
	}

	poke(0x24E, 10);
	// set graphicals primitives avaible in text mode
	poke(0x213, 255);
	// draw the sun
	for (j = 1; j < 32; j++)
	{
		curset(90, 64, 1); circle(j, 1);
	}
	poke(SCRADDR + 608, 12); // blink before text
	poke(SCRADDR + 633, 8);  // no blink
	poke(SCRADDR + 80 + 608, 12); // blink before text
	poke(SCRADDR + 80 + 633, 8);  // no blink
	AdvancedPrint(10, 15, "CONGRATULATION ALDO !");
	AdvancedPrint(5, 17, "YOU R SURVIVOR OF THIS STORM !");

	// here play a victory music   ??

	j = 0;
	// sun shine animation
	for (wait = 0; wait < 200; wait++)
	{
		// make sun shining
		if (j == 10)
		{
			*(unsigned char*)(0XBBAA + (40 * (caty - 2)) + px - 5) = 39;
			curset(90, 64, 1); circle(34, 1);
			curset(90, 64, 1); circle(38, 1);
			curset(90, 64, 1); circle(48, 1);
		}
		if (j++ == 20)
		{
			*(unsigned char*)(0XBBAA + (40 * (caty - 2)) + px - 5) = 32;
			curset(90, 64, 1); circle(48, 0);
			curset(90, 64, 1); circle(38, 0);
			curset(90, 64, 1); circle(34, 0);

			j = 0;
		}
		kk = key();
		if (kk == 27)
			break;
	}
}
// music handler
void play_music()
{
	unsigned char p1, p2, n1, o1, n2, o2, n3, o3;

	p1 = game_music[music_index];
	p2 = game_music[music_index + 1];
	n1 = game_music[music_index + 2];
	o1 = game_music[music_index + 3];
	n2 = game_music[music_index + 4];
	o2 = game_music[music_index + 5];
	n3 = game_music[music_index + 6];
	o3 = game_music[music_index + 7];

	//rv=game_music[music_index+8]; 

	play(p2, 0, 0, 0);
	//printf("p1=%d p2=%d o1=%dn1=%d o2=%d n2=%d o3=%d\n",p1,p2,o1,n2,o2,n3,o3);

	if (n1 > 0)
		music(1, o1, n1, volume);
	if (n2 > 0)
		music(2, o2, n2, volume);
	if (n3 > 0)
		music(3, o3, n3, volume);

	music_index += 8;
	if (music_index >= (sizeof(game_music) - 9))
		music_index = 0;


	//music(1,game_music[music_index],game_music[music_index+1],0);
	//play(1,0,8,(1110*game_music[music_index+2]));
	//music_index+=3;
	//if (music_index>=sizeof(game_music))
	//   music_index=0;
}

// drop sliding animation for outside rain
void drop_sliding_outside() // 
{
	asm("ldy #7;"
		"lda $B400+856+16,y;"
		"tax;"

		"suite_ge;"
		"dey;"
		"lda $B400+856+16,y;"
		"iny;"
		"sta $B400+856+16,y;"
		"dey;"
		"bne suite_ge;"
		"txa;"
		"sta $B400+856+16;"

	);
	asm("ldy #7;"
		"lda $B400+856+16,y;"
		"tax;"

		"suite_ge2;"
		"dey;"
		"lda $B400+856+16,y;"
		"iny;"
		"sta $B400+856+16,y;"
		"dey;"
		"bne suite_ge2;"
		"txa;"
		"sta $B400+856+16;"
	);
}

//to redefine rain drop char ( 2 char to make drop scrolling over 2 char
void redefine_raindrop()
{
	*(unsigned char*)(CARADDR + 864) = 8;// == poke (CARADDR+864,8);
	*(unsigned char*)(CARADDR + 865) = 8;
	*(unsigned char*)(CARADDR + 866) = 28;
	*(unsigned char*)(CARADDR + 867) = 28;
	*(unsigned char*)(CARADDR + 868) = 62;
	*(unsigned char*)(CARADDR + 869) = 62;
	*(unsigned char*)(CARADDR + 870) = 46;
	*(unsigned char*)(CARADDR + 871) = 28;//poke (CARADDR+871,28);
	// 108 blanc reservé pour drop_sliding goutte sur 2eme car
	*(unsigned char*)(CARADDR + 856) = 0;
	*(unsigned char*)(CARADDR + 857) = 0;
	*(unsigned char*)(CARADDR + 858) = 0;
	*(unsigned char*)(CARADDR + 859) = 0;
	*(unsigned char*)(CARADDR + 860) = 0;
	*(unsigned char*)(CARADDR + 861) = 0;
	*(unsigned char*)(CARADDR + 862) = 0;
	*(unsigned char*)(CARADDR + 863) = 0;
}
// player falling by collission with cat or tile
void player_falling()
{

	int time;
	//colcat=0; // disable cat collision
	//colfireball=0; // disable fireball collision
	if (active_sound == 1)
	{
		shoot();
		wait_to_music = 3;
	}

	for (i = 0; i < 5; i++)
	{

		disp_aldo(px, py + i);
		//manage_cat();
		//plot(px,py-1+i,32);
		//plot(px+1,py-1+i,32);
		//plot(px-1,py-1+i,32);
		for (time = 0; time < 2000; time++);

		// must manage bottom screen repaint

	}
	display_floor();   // bug dans cette methode voir si pbl de clc ou registre a sauver sur pile ? 
}
// others object falling : ice or kernel
void manage_falling_obj()
{
	if ((obj_kernel != 0) || (obj_ice != 0))
	{
		if (obj_y < floor_y - 3)
		{
			obj_y++;
			if (obj_y > ceiling_y + 1)
			{
				*(unsigned char*)(0XBBAA + (40 * (obj_y - 1)) + obj_x) = 32;
				if (obj_kernel == 1)
				{
					*(unsigned char*)(0XBBAA + (40 * obj_y) + obj_x) = 39;
					*(unsigned char*)(0XBBAA + (40 * obj_y) + obj_x - 1) = 1;
				}
				if (obj_ice == 1)
				{
					*(unsigned char*)(0XBBAA + (40 * obj_y) + obj_x) = 99;
					*(unsigned char*)(0XBBAA + (40 * obj_y) + obj_x - 1) = 6;
				}
				*(unsigned char*)(0XBBAA + (40 * obj_y) + obj_x + 1) = 0;
			}
		}
		else
		{
			poke(0XBBAA + (40 * obj_y) + obj_x, 32);
			if (obj_kernel == 1)
				obj_kernel = 0;

			if (obj_ice == 1)
				obj_ice = 0;
		}
		// fall on Aldo body
		if ((px - 2 == obj_x) && ((obj_y >= py - 1) && (obj_y <= py + 3)))
		{
			if (obj_kernel == 1)
			{
				if (active_sound == 1)
				{
					ping();
					wait_to_music = 12;
				}
				obj_kernel = 0;
				life++;
			}
			if (obj_ice == 1)
			{
				if (active_sound == 1)
				{
					explode();
					wait_to_music = 12;
				}
				obj_ice = 0;
				being_falling = 1;
				player_falling();
				being_falling = 0;
				life--;
			}
			display_floor();

		}
	}
}
// manage lightning 
// tile_x = lightning x
//
void manage_lightning()
{

	if (tile_fall == 0)
	{
		if (lightning == 1)
		{
			// afficher foudre
			tile_fall = 1;

			tile_y = ceiling_y - 3;

			lightning_time = 12; // temps de durée de l'éclair  
			paper(0);
			if (active_sound == 1)
			{
				explode();
				wait_to_music = 12;
			}
		}
	}
	if (lightning_time > 0)
	{
		lightning_time--;
		for (ly = 0; ly < ceiling_y - 1; ly++)
		{
			// display the lightning
			//poke(0XBBAA+(40*ly)+tile_x-1,3);
			*(unsigned char*)(0XBBAA + (40 * ly) + tile_x - 1) = 123 | 128;
			*(unsigned char*)(0XBBAA + (40 * ly) + tile_x) = 124 | 128;
			//poke(0XBBAA+(40*ly)+tile_x+2,4);
		}
		// pointe de l'éclair
		//poke(0XBBAA+(40*(ly))+tile_x-1,3);
		*(unsigned char*)(0XBBAA - 1 + (40 * (ly)) + tile_x) = 125 | 128;
		//poke(0XBBAA+(40*(ly))+tile_x+1,4);
		if (lightning_time == 1)
		{
			// clear ligthning		
			for (ly = 0; ly < ceiling_y - 1; ly++)
			{
				// efface éclair avec gouttes
				if (ly < def_ceiling_pos)
				{
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x - 1) = 109;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x) = 109;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x + 1) = 109;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x + 2) = 109;
				}
				else
				{
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x - 1) = 32;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x) = 32;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x + 1) = 32;
					*(unsigned char*)(0XBBAA + (40 * ly) + tile_x + 2) = 32;
				}
			}
			if (ly < def_ceiling_pos)
				*(unsigned char*)(0XBBAA + ((40 * ly + 2)) + tile_x - 3) = 109;
			else
				*(unsigned char*)(0XBBAA + ((40 * ly + 2)) + tile_x - 3) = 32;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=109;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=109;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=109;
			lightning_time = 0;
			paper(7);
		}
	}
	if (tile_fall == 1)
	{
		// roof tile fall on the player
		if ((px - 2 == tile_x) && ((tile_y >= py - 1) && (tile_y <= py + 3)))
		{
			poke(0XBBAA + (40 * tile_y) + tile_x, 32);
			tile_fall = 0; // 	    
			shoot_tile_time = 3; // 4 main program cycles for show color indacte correct catching
			lightning = 0;
			lightning_time = 0;
			paper(3);
			if (active_sound == 1)
			{
				shoot();
				wait_to_music = 7;
			}
			benddown = 1;
			legsup = 1;
			// clear head zone
			gotoxy(px - 1, py); printf("   ");

			life--;
		}
		if (tile_y < floor_y - 3)
		{
			tile_y++;
			if (tile_y > ceiling_y + 1)
			{
				*(unsigned char*)(0XBBAA + (40 * (tile_y - 1)) + tile_x) = 32;
				*(unsigned char*)(0XBBAA + (40 * tile_y) + tile_x) = 122;
				*(unsigned char*)(0XBBAA + (40 * tile_y) + tile_x + 1) = 0;
				*(unsigned char*)(0XBBAA + (40 * tile_y) + tile_x - 1) = 1;
			}
		}
		else
		{
			lightning = 0;
			// display falling tile
			tile_fall = 0;
			poke(0XBBAA + (40 * tile_y) + tile_x, 32);
		}
	}
}
// scroll right 
// min y pos =1
void scroll_right(char y, char nbr)
{
	scroll_y_nbr = nbr;
	scroll_y = y;
	asm("lda #$AA;"
		"sta write_sr+1;"
		"sta read_sr+1;"
		"lda #$BB;"
		"sta write_sr+2;"
		"sta read_sr+2;"

		// calculate y start position

		"ldy %scroll_y;"
		//"bne saut_sr_y;"
		"suite_scry:"
		"clc;"
		"lda read_sr+1;"
		"adc #40;"
		"bcc saut_sr;"
		"inc write_sr+2;"
		"inc read_sr+2;"
		"saut_sr:"
		"sta write_sr+1;"
		"sta read_sr+1;"
		"dey;"
		"bne suite_scry;"
		//"saut_sr_y:"

		//  here calculate the y start position
		"ldy %scroll_y_nbr;"
		"suite1_sr:"
		"clc;"
		"ldx #37;"
		"suite2_sr:"

		"dex;"
		"read_sr:"
		"lda $1234,x;"
		"inx;"
		"write_sr:"
		"sta $1234,x;"
		"dex;"

		"bne suite2_sr;"
		"lda write_sr+1;"
		"adc #40;"
		"bcc saut_retenue_sr;"
		"inc write_sr+2;"
		"inc read_sr+2;"
		"saut_retenue_sr:"
		"sta write_sr+1;"
		"sta read_sr+1;"
		"dey;"
		"bne suite1_sr;");
}

void scroll_left(char y, char nbr)
{
	scroll_y_nbr = nbr;
	scroll_y = y;


	asm("lda #$AA;"  // #$AA
		"sta write1_sl+1;"
		"sta read1_sl+1;"
		"lda #$BB;"
		"sta write1_sl+2;"
		"sta read1_sl+2;"
		// calculate y start position

		"ldy %scroll_y;" // 1 = minimum value
		//"bne saut_sl_y;"
		"suite_scly:"
		"clc;"
		"lda read1_sl+1;"
		"adc #40;"
		"bcc saut_sl;"
		"inc write1_sl+2;"
		"inc read1_sl+2;"
		"saut_sl:"
		"sta write1_sl+1;"
		"sta read1_sl+1;"
		"dey;"
		"bne suite_scly;"
		//"saut_sl_y:"
		// left scrolling
		"ldy %scroll_y_nbr;"
		"suite11_sl:"
		"ldx #0;"
		"suite21_sl:"

		"inx;"
		"read1_sl:"
		"lda $1234,x;"
		"dex;"
		"write1_sl:"
		"sta $1234,x;"
		"inx;"
		"cpx #37;"  // instead 38 for bug on title page
		"bne suite21_sl;"
		"clc;"
		"lda read1_sl+1;"
		"adc #40;"
		"bcc saut_retenue1_sl;"
		"inc write1_sl+2;"
		"inc read1_sl+2;"
		"saut_retenue1_sl:"
		"sta write1_sl+1;"
		"sta read1_sl+1;"
		"dey;"
		"bne suite11_sl;");
}
// house shaking bc ceiling go down
void house_shaking()
{
	int j;
	for (j = 0; j < 4; j++)
	{
		scroll_right(1, 23);
		for (wait = 0; wait < 500; wait++);
		scroll_left(1, 23);
		for (wait = 0; wait < 500; wait++);
	}    // scroll_left()	
}

void manage_cat()
{
#ifdef DEBUG
	gotoxy(30, 0); printf("WA LVL=%d", waterlevel);
#endif

	if (cat == 0)
	{
		j = 0;

		if (waterlevel > 0)
		{  // redefine chars for catfish apparence
			for (i = 46976; i < 46976 + 32; i++)
				*(unsigned char*)i = catfish[j++];

		}
		else
		{  // redefine chars for cat apparence
			for (i = 46976; i < 46976 + 32; i++)
				*(unsigned char*)i = catcat[j++];
		}

		if (seecat == 1) //appear on left
		{
			catx = 3;
			cat = 1;
			dircat = 0;
			catwait = 10;
			
			playSoundEffect(cat_at_door_sound,2);
			
			
		}
		if (seecat == 2)// appear on right
		{

			catx = 34;
			cat = 1;
			dircat = 1;
			catwait = 10;
			
			playSoundEffect(cat_at_door_sound,2);
			
			
		}

	}
	else // cat ==1 , gérer la présence du cat et son déplacement
	{
		// affichage du cat
		//   forme du cat de gauche à droite
		if (dircat == 0)
		{

			*(unsigned char*)(SCRADDR + (caty * 40) + catx - 2) = 32;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx - 1) = 0;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx) = 114;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx + 1) = 115;

		}
		else  // forme du cat de droite à gauche
		{
			*(unsigned char*)(SCRADDR + (caty * 40) + catx) = 112;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx + 1) = 113;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx - 1) = 0;
			*(unsigned char*)(SCRADDR + (caty * 40) + catx + 2) = 32;

		}
		if (catwait > 0)
		{
			catwait--;
		}
		if (dircat == 0) // de gauche à droite
		{
			if (catwait == 0)
			{
				catx++;
			}
			if (catx == 7)  // restore left zone
			{
				*(unsigned char*)(SCRADDR + (caty * 40) + 1) = 4;
				*(unsigned char*)(SCRADDR + (caty * 40) + 2) = 109;
				*(unsigned char*)(SCRADDR + (caty * 40) + 3) = 109;
				//*(unsigned char*)(SCRADDR+(caty*40)+4)=126|128;
			}
			if (catx == 37)  // resore right zone
			{
				// effacement du cat par la pluie et mur
				cat = 0;

				seecat = 0;
				*(unsigned char*)(SCRADDR + (caty * 40) + catx - 3) = 4;
				//*(unsigned char*)(SCRADDR+(caty*40)+catx-2)=126|128;

				*(unsigned char*)(SCRADDR + (caty * 40) + catx - 1) = 109;
				*(unsigned char*)(SCRADDR + (caty * 40) + catx) = 109;
			}
		}
		if (dircat == 1) // de droite à gauche
		{
			if (catwait == 0)
			{
				catx--;
			}
			if (catx == 31)
			{
				// restore right wall
				*(unsigned char*)(SCRADDR + (caty * 40) + 34) = 4;
				*(unsigned char*)(SCRADDR + (caty * 40) + 35) = 126 | 128;
				*(unsigned char*)(SCRADDR + (caty * 40) + 36) = 109;
				*(unsigned char*)(SCRADDR + (caty * 40) + 37) = 109;
			}
			if (catx == 2)  //arrivé du cat à la position 1
			{
				seecat = 0;
				// effacement du cat par la pluie et mur
				cat = 0;
				// restore left wall
				*(unsigned char*)(SCRADDR + (caty * 40) + catx) = 109;
				*(unsigned char*)(SCRADDR + (caty * 40) + catx + 1) = 109;
				*(unsigned char*)(SCRADDR + (caty * 40) + catx + 2) = 126 | 128;

			}
		}
		// handle cat collision with Aldo
		if (((catx == px) || (catx + 1 == px)) && (jump_time < 3) && (colcattime == 0))
		{

			
			playSoundEffect(cat_collision_sound,3);
		
			if (dircat == 1)
				catx -= 2;
			if (dircat == 2)
				catx += 3;
			paper(3);
			ink(7);
			being_falling = 1;
			armsdown = 1;

			//player_falling();
			benddown = 1;
			legsup = 1;
			gotoxy(px - 1, py); printf("   ");

			armsdown = 0;
			being_falling = 0,
				life--;
			colcat = 1; //enable cat collision
			shoot_cat_time = 3; // keepn 3 cycle time tp show cyan color paper indicate correct catching
			colcattime = 27;
			unshoot();
			display_floor();
		}
		if (colcattime > 0)
			colcattime--;
	}
}
void manage_fireball()
{
#ifdef DEBUG
	gotoxy(30, 0); printf("WAT=%d", waterlevel);
#endif

	if (fireball == 0)
	{
		j = 0;
		if (seefireball == 1) //Apparait à gauche
		{
			//tile_x=20;
			//lightning_time=12; // temps de durée de l'éclair  

			fireballx = 3;
			fireball = 1;
			dirfireball = 0;
			fireballwait = 10;
			if (active_sound)
			{
				explode();
				wait_to_music = 6;
			}
			lightning = 1;
			tile_x = 20;
		}
		if (seefireball == 2)// apparait à droite
		{

			fireballx = 34;
			fireball = 1;
			dirfireball = 1;
			fireballwait = 10;
			if (active_sound)
			{
				explode();
				wait_to_music = 6;
			}
			lightning = 1;
			tile_x = 20;
		}
	}
	else // fireball ==1 , gérer la présence du fireball et son déplacement
	{
		*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx) = 120;
		*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx + 1) = 121;
		*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx - 1) = 3;
		*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx + 2) = 32;

		if (fireballwait > 0)
			fireballwait--;

		if (dirfireball == 0) // de gauche à droite
		{
			if (fireballwait == 0)
			{
				fireballx++;
			}
			if (fireballx == 7)  // restore wall
			{
				*(unsigned char*)(SCRADDR + (firebally * 40) + 1) = 4;
				*(unsigned char*)(SCRADDR + (firebally * 40) + 2) = 109;
				*(unsigned char*)(SCRADDR + (firebally * 40) + 3) = 109;
				*(unsigned char*)(SCRADDR + (firebally * 40) + 4) = 126 | 128;
			}
			if (fireballx == 37)  //arrived at position 36
			{
				fireball = 0;
				seefireball = 0;
				// restore wall
				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx - 3) = 4;
				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx - 2) = 126 | 128;

				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx - 1) = 109;
				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx) = 109;
			}
		}
		if (dirfireball == 1) // de droite à gauche
		{
			if (fireballwait == 0)
				fireballx--;
			if (fireballx == 31)
			{
				// restore right zone
				*(unsigned char*)(SCRADDR + (firebally * 40) + 34) = 4;
				//*(unsigned char*)(SCRADDR+(firebally*40)+35)=126|128;
				*(unsigned char*)(SCRADDR + (firebally * 40) + 36) = 109;
				*(unsigned char*)(SCRADDR + (firebally * 40) + 37) = 109;
			}
			if (fireballx == 2)  //fireball arrived at position 1
			{
				seefireball = 0;
				fireball = 0;
				// restore left zone
				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx) = 109;
				*(unsigned char*)(SCRADDR + (firebally * 40) + fireballx + 1) = 109;
				//*(unsigned char*)(SCRADDR+(firebally*40)+fireballx+2)=126|128;
			}
		}
		// handler fireball collision with Aldo
		if (((fireballx == px) || (fireballx + 1 == px)) && (benddown == 0) && (colfireballtime == 0))
		{

			//  faire plutot un benddown_time et pas benddown

			
			SoundEffect(cat_collision_sound,3);
				
			paper(3);
			ink(7);

			benddown = 1;
			legsup = 1;
			gotoxy(px - 1, py); printf("   ");

			armsdown = 0;
			shoot_fireball_time = 4;

			life--;
			colfireballtime = 27;
			unshoot();
			display_floor();
		}
		if (colfireballtime > 0)
			colfireballtime--;
	}
}

void disp_aldo(char x, char y)
{
	unsigned char aldoy;
	aldoy = y;
	if (benddown == 1)
		aldoy = y + 1;

	// arms up
	if (armsdown == 0)
	{
		// sceau
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x - 1) = 91;
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x) = 92;
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x + 1) = 93;
		// blancs
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x - 2) = 5;
		//poke (SCRADDR+(aldoy*40)+x-2,5);
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x + 2) = 0;

		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x - 1) = 94;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x + 1) = 96;
		// blancs
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x - 2) = 1;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x + 2) = 0;
	}
	else  // arms down Aldo Win
	{
		*(unsigned char*)(SCRADDR + ((aldoy) * 40) + x - 1) = 0;
		*(unsigned char*)(SCRADDR + ((aldoy) * 40) + x + 1) = 0;
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x - 2) = 5;

		*(unsigned char*)(SCRADDR + (aldoy * 40) + x) = 34;
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x - 2) = 5;
		*(unsigned char*)(SCRADDR + (aldoy * 40) + x + 2) = 0;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x - 1) = 35;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x) = 95;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x + 1) = 36;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x - 2) = 1;
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x + 2) = 0;
	}

	// head smile after waiting 64 loop

	if (player_wait > 64)
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x) = 106;
	else  // close eyes after 100 main loop cycle
	{
		*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + x) = 95;
	}

	if (armsdown == 0)
	{
		if (jump_time < 2 && benddown == 0)
		{
			// ventre en mode non saut	
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 1) = 1;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x) = 98;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 1) = 32;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 2) = 1;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 2) = 0;
		}
		else // ventre en mode saut
		{
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x) = 98;
		}
	}
	else // arms up
	{
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 2) = 1;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 1) = 37;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 1) = 38;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x) = 98;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 2) = 0;
	}
	// blancs
//	poke (SCRADDR+((y+2)*40)+x-2,0);
//	poke (SCRADDR+((y+2)*40)+x+2,0);
	// blancs pour effacer les jambes du saut
//	poke (SCRADDR+((y+2)*40)+x-3,0);
//	poke (SCRADDR+((y+2)*40)+x+3,0);

	// getion du saut pour mettre les jambes en l'air
	// legs up
	if (legsup == 1)
	{
		// mettre les jambes sur les cotés 2xcar de chaque coté
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 1) = 117;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 2) = 116;
		if (px < 6)  // redessine le mur à gauche
		{
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 3) = 126;
		}
		else
		{
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 3) = 2;
		}
		//poke (SCRADDR+((aldoy+2)*40)+x,127);
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 1) = 118;
		*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 2) = 119;

		if (px < 32)  // redessine le mur à gauche
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 3) = 0;


		if (jump_time == jump_max_time && benddown == 0)
		{
			// blancs à la place des jambes du bas
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x - 1) = 32;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x) = 32;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x + 1) = 32;
		}
	}
	else // manage legs on ground 
	{
		if (walking_alt == 1)  // walking mode
		{			
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x - 1) = 100;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x) = 101;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x + 1) = 102;
		
		}
		else
		{
			
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x - 1) = 103;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x) = 104;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x + 1) = 105;
		
		}
		if ((px > 7) && (endgame == 0)) // redraw wall on the left
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x - 3) = 32;


		if ((px > 30) && (endgame == 0)) // redraw wall on the right
		{
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + 34) = 4;
			*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + 35) = 126 | 128;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + 34) = 4;
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + 35) = 126 | 128;
			*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + 34) = 4;
			*(unsigned char*)(SCRADDR + ((aldoy + 1) * 40) + 35) = 126 | 128;
			*(unsigned char*)(SCRADDR + ((aldoy) * 40) + 34) = 4;
			*(unsigned char*)(SCRADDR + ((aldoy) * 40) + 35) = 126 | 128;
		}
		else
		{
			*(unsigned char*)(SCRADDR + ((aldoy + 2) * 40) + x + 3) = 32;
		}
	}
	if (benddown == 0)
	{
		// blank char next the legs
		*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x - 2) = 2;
		*(unsigned char*)(SCRADDR + ((aldoy + 3) * 40) + x + 2) = 0;
	}

	if (being_falling == 1)
	{
		*(unsigned char*)(SCRADDR + ((aldoy - 1) * 40) + x - 1) = 32;
		*(unsigned char*)(SCRADDR + ((aldoy - 1) * 40) + x) = 32;
		*(unsigned char*)(SCRADDR + ((aldoy - 1) * 40) + x + 1) = 32;
	}

}
void unshoot()
{
	for (wait = 0; wait < 30; wait++)
	{
		// faire tourner des étoile !!
		disp_aldo(px, py);
		for (wait2 = 0; wait2 < 4; wait2++)
			ink(1);
		drop_sliding_outside();
		ink(4);
	}
}
void display_score()
{
	sprintf(SCRADDR + (40 * 26) + 20, "\23\4DROPS \27\1%d\27\4", score);

}

// raindrop managment
//

void manage_rain()
{

	//	if (raindroptime>0)
	//	   raindroptime--; // espace de temps entre les gouttes: temps en therme de cycle
		//gotoxy(5,5);printf("TIME=%d  ",raindroptime);
	for (rdindex = 0; rdindex < RAINDROPMAX; rdindex++)
	{
		// activate a new drop
		// here we find a free place in the raindrop to create a new drop . array if raindroptime=0
		if (raindropstate[rdindex] == 0 && raindroptime < 1)
		{
			raindropstate[rdindex] = 1; // cellule de la goutte devient occupée

			if (altchar == 0)
				raindropy[rdindex] = ceiling_y + 1;
			else
				raindropy[rdindex] = ceiling_y + 2;

			// 1rst code read from the rain array
			codedrop = rain[index_raindrop];
			// initialiser la position x de la nouvelle goute depuis le tableau rain[]
			raindropx[rdindex] = codedrop;
			if (codedrop < 150)
				poke(SCRADDR + (40 * (ceiling_y + 1)) + codedrop, 122);

			// delai entre 2 gouttes
			//décompte lors du déplacement de la goutte
			// à lire dans le tableau des vagues
			// second code read from the rain array	
			// raindroptime = wait delay to read next rain drop
			raindroptime = rain[index_raindrop + 1];

			// specials code :		
			if (codedrop > 149)
			{
				raindropstate[rdindex] = 0;
				// lightning
				if (codedrop > 199 && codedrop < 227)
				{
					tile_x = codedrop - 200;
					// get codedrop for X position of lightening
					lightning = 1;
				}
				// kernel (life) 180-196 
				if (codedrop > 179 && codedrop < 197)
				{
					obj_x = codedrop - 180;
					obj_y = ceiling_y - 3;
					// get codedrop for X position of lightening
					obj_kernel = 1;
				}
				// 150-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)
				// ice 150-176
				if (codedrop > 149 && codedrop < 178)
				{
					obj_x = codedrop - 150;
					obj_y = ceiling_y - 3;
					// get codedrop for X position of lightening
					obj_ice = 1;
				}
				// if codedrop = 250 then cat appear on the right, 251 on the left
				if (codedrop == 250)
				{
					if (seecat == 0)
						seecat = 1;
				}
				if (codedrop == 251)
				{
					if (seecat == 0)
						seecat = 2;
				}
				if (codedrop == 252)
				{
					if (seefireball == 0)
						seefireball = 1;
				}
				if (codedrop == 253)
				{
					if (seefireball == 0)
						seefireball = 2;
				}

				// new wave of drops, ceiling fall one step
				if (codedrop == 255)  // si la position de x=255 c'est une nouvelle vague de gouttes
				{


					if (wave_nbr > 0)
					{
						ceiling_y++; // le plafond s'affesse d'une ligne
						house_shaking();
						if (active_sound == 1)
						{
							explode();
							wait_to_music = 12;
						}
						display_ceiling(); // affichage du plafond
					}
					wave_nbr++;  // vague des gouttes suivantes
					//raindroptimer[rdindex]=wait_fall_raindrop; //	
					if (ceiling_y == py) // si le plafond est au niveau de la tete du hero
					{
						if (active_sound = 1)
						{
							explode();
							wait_to_music = 12;
						}
						// fin partie
						endgame = 1;
					}
					display_wave_level_timer = 30; // durée d'affichage du numero de la vague suivante
				}
			}
			//	raindroptimer[rdindex]=wait_fall_raindrop; // wait_fall_raindrop
				// increment the raindrop array index
			if (index_raindrop < sizeof(rain))
				index_raindrop += 2;
			else
				endgame = 2;  // end of game Win 

		} //if (raindropstate[rdindex]==0 && raindroptime<1)					
		if (endgame > 0) break;
	} // end for (rdindex=0	

	if (endgame > 0)
		return;
	// here manage active rain drop in array
	for (rdindex = 0; rdindex < RAINDROPMAX; rdindex++)
	{
		// here we dont wait for raindroptime=0 bc the player must be catch a raindrop during sliding
		if (raindropstate[rdindex] == 1)
		{
			// erase on screen drop character on his final position

			// player catch a rain drop
			// prendre en compte le altchar pour le y
			if ((px == raindropx[rdindex]) &&
				((py - 2 + benddown < raindropy[rdindex]) && (py + 1 + benddown > raindropy[rdindex])))
			{
				score++;
				drop_catch_time = 2; //  main program 4 cycles for show color that indacate correct catching
				paper(6);
				ink(7);
				raindropstate[rdindex] = 0;
				poke(SCRADDR + ((raindropy[rdindex] - 1) * 40) + raindropx[rdindex], 32);
				poke(SCRADDR + (raindropy[rdindex] * 40) + raindropx[rdindex], 32);
				//poke(SCRADDR+((raindropy[rdindex]+1)*40)+raindropx[rdindex],32);

				// remplacer advancedprint par poke qui est + rapide pour 1xchar
				
				playSoundEffect(bong_sound,2);
					
				
			}
		}
	} //end for (rdindex=0

	// here manage sliding state = 0 to alternate the two drop char
	if (drop_sliding == 0) // etat initial du drop_sliding , la goutte a entierement glissé d'un char a l'autre
	{

		for (rdindex = 0; rdindex < RAINDROPMAX; rdindex++)
		{
			// if array cellule is busy  = rain drop active
			if (raindropstate[rdindex] == 1)
			{

				// display an active raindrop

				if (altchar == 0)
				{
					*(unsigned char*)(SCRADDR + (raindropy[rdindex] * 40) + raindropx[rdindex]) = 107;
					*(unsigned char*)(SCRADDR + ((raindropy[rdindex] + 1) * 40) + raindropx[rdindex]) = 108;

				}
				else
				{
					*(unsigned char*)(SCRADDR + (raindropy[rdindex] * 40) + raindropx[rdindex]) = 108;
					*(unsigned char*)(SCRADDR + ((raindropy[rdindex] + 1) * 40) + raindropx[rdindex]) = 107;
				}
				*(unsigned char*)(SCRADDR + ((raindropy[rdindex] + 1) * 40) + (raindropx[rdindex] - 1)) = 4;

				// erase drop trace on y axis (on ceiling)
				if (raindropy[rdindex] > ceiling_y + 2)
					*(unsigned char*)(SCRADDR + ((raindropy[rdindex] - 1) * 40) + raindropx[rdindex]) = 32;

			}
			else
			{
				for (wait = 0; wait < 10; wait++);
			}
		}
		drop_sliding++;
	}
	else // manage intermediate rain drop sliding state  - etat de glissement de goutte intermédiaire
	{
		// reserve time to time compentation with the previous loop
		for (wait = 0; wait < 50; wait++);
		// rain drop char (107&108) verticaly scroll managing

		// alternate altchar to swap position of 2 char of raindrop 
		if (altchar == 0)  // raindrop scroll 107->108  lda %altchar;
		{
			asm("ldy #15;"
				"clc;"
				"suite3;"
				"dey;"
				"lda $B400+856,y;"
				"iny;"
				"sta $B400+856,y;"
				"dey;"
				"cpy #0;"
				"bne suite3;"
				"lda #0;"
				"sta $B400+856;"
			);
		}
		if (altchar == 1) // raindrop scroll 108->107
		{
			// possible changement de la vitesse de gliseement 
			// avec for(i=0;i<vitesse;i++) pour répéter le nombre de drop_sliding
			// il faut aussi changer la ligne en dessous de l'asm suvante (drop_sliding++)
			asm("lda $B400+856+15;"
				"tax;"
				"ldy #15;"

				"suite1;"
				"clc;"
				"dey;"
				"lda $B400+856,y;"
				"iny;"
				"sta $B400+856,y;"
				"dey;"
				"cpy #7;"
				"bne suite1;"
				"sta $B400+856,0;"
				"ldy #7;"
				"suite2;"
				"clc;"
				"dey;"
				"lda $B400+856,y;"
				"iny;"
				"sta $B400+856,y;"
				"dey;"
				"cpy #0;"
				"bne suite2;"
				"txa;"
				"sta $B400+856;"
			);
		}
		drop_sliding++; // scroll to next line index 
		if (drop_sliding > 8)
		{
			drop_sliding = 0;
			// altcharernate char
			if (altchar == 0)
			{
				altchar = 1;
				if (raindroptime > 0)
					raindroptime--; // espace de temps entre les gouttes: temps en therme de cycle			  
			}
			else
				altchar = 0;
			// on incrémente le y de la boucle (après 8 drop_sliding d'octet vers le bas)
			for (rdindex = 0; rdindex < RAINDROPMAX; rdindex++)
			{
				if (raindropstate[rdindex] == 1)
				{
					// clear tile over drop

					// drop fall on the floor or water 
					if (raindropy[rdindex] >= (floor_y - 3 - waterlevel))
					{
						raindropstate[rdindex] = 0;
						poke(SCRADDR + ((raindropy[rdindex] - 1) * 40) + raindropx[rdindex], 32);
						poke(SCRADDR + (raindropy[rdindex] * 40) + raindropx[rdindex], 32);
						poke(SCRADDR + ((raindropy[rdindex] + 1) * 40) + raindropx[rdindex], 32);
						drop_floor_time = 3;
						paper(5);
						ink(3);
						if (waterlevel < 4)
							waterlevel++;
					}

					raindropy[rdindex]++;
				}
				else
					for (wait = 0; wait < 10; wait++);

			} //end for (rdindex=0
		}
	} // end of else of if (drop_sliding==0)

}
void display_outside()
{
	unsigned char i;
	for (i = 1; i < 28; i++)
	{
		if (i < 7)
		{
			AdvancedPrint(2, i, "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
			poke(SCRADDR + 1 + (i * 40), 4);
		}
		if ((i > 6) && (i < 24))
		{
			poke(SCRADDR + 1 + (i * 40), 4);
			AdvancedPrint(2, i, "mm                               mmm");
			//poke(SCRADDR+2+(i*40),4);
			//poke(SCRADDR+3+(i*40),0);
			poke(SCRADDR + 33 + (i * 40), 4);
			//poke(SCRADDR+36+(i*40),4);			
		}
	}
}
void display_floor()
{
	// display ground
	char i;
	for (i = 24; i < 28; i++)
		AdvancedPrint(2, i, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	//poke(SCRADDR+1+(i*40),5);

// floor of the house

	asm("ldy #30;"
		"lda #127;"
		"suite_floor;"
		"sta $BB80+4+(24*40),y;"
		"dey;"
		"bne suite_floor;"
	);
	
	//poke(SCRADDR+3+(7*40),1);
	//poke(SCRADDR+3+(8*40),2);			
}
void display_ceiling()
{	// this routine take ceiling_y to draw it at the y=ceiling_y
	unsigned char yy;
	yy = ceiling_y;

	//poke(SCRADDR+2+((ceiling_y+1)*40),1); //red color for ceiling tile
	//poke(SCRADDR+36+((ceiling_y+1)*40),4); //blue color for ceiling tile

	asm("sta $FFFF;" // calculate y position
		"lda #$BB;"
		"sta write_cei+2;"
		"lda #$80;"
		"sta write_cei+1;"
		"ldy %yy;"
		"iny;" // +1
		"suite_cei;"
		"clc;"
		"lda write_cei+1;"
		"adc #40;"
		"bcc saut_cei;"
		"inc write_cei+2;"
		"saut_cei;"
		"sta write_cei+1;"
		"dey;"
		"bne suite_cei;"
		"inc write_cei+1;"
		"inc write_cei+1;"
		"inc write_cei+1;"
		"inc write_cei+1;"
		"clc;"
		"ldy #30;"
		"lda #122;"   // tile char
		"suite_ceiling;"
		"write_cei;"
		"sta $1234,y;"
		"dey;"
		"bne suite_ceiling;");
	yy--;
	if (ceiling_y > 6)
	{
		// same code but for line just over the roof (line of rain drop to erase roof line)
		asm("sta $FFFF;"
			"lda #$BB;"
			"sta write_cei2+2;"
			"lda #$80;"
			"sta write_cei2+1;"
			"ldy %yy;"
			"iny;" // +1
			"suite_cei2;"
			"clc;"
			"lda write_cei2+1;"
			"adc #40;"
			"bcc saut_cei2;"
			"inc write_cei2+2;"
			"saut_cei2;"
			"sta write_cei2+1;"
			"dey;"
			"bne suite_cei2;"
			"inc write_cei2+1;"
			"inc write_cei2+1;"
			"inc write_cei2+1;"
			"inc write_cei2+1;"
			"clc;"
			"ldy #30;"  // 33 x char
			"lda #32;" // rain drop=109
			"suite_ceiling2;"
			"write_cei2;"
			"sta $1234,y;"
			"dey;"
			"bne suite_ceiling2;");
		poke(SCRADDR + 5 + ((ceiling_y) * 40), 22); // white blue color
		poke(SCRADDR + 36 + ((ceiling_y) * 40), 23); //white color
	}

	poke(SCRADDR + 5 + ((ceiling_y + 1) * 40), 1 | 128); //red color for ceiling tile
	poke(SCRADDR + 34 + ((ceiling_y + 1) * 40), 4 | 128); //blue color for ceiling tile
}

display_left_wall()
{

	asm("lda #254;"
		"sta $BB80+(7*40)+4;"
		"sta $BB80+(8*40)+4;"
		"sta $BB80+(9*40)+4;"
		"sta $BB80+(10*40)+4;"
		"sta $BB80+(11*40)+4;"
		"sta $BB80+(12*40)+4;"
		"sta $BB80+(13*40)+4;"
		"sta $BB80+(14*40)+4;"
		"sta $BB80+(15*40)+4;"
		"sta $BB80+(16*40)+4;"
		"sta $BB80+(17*40)+4;"
		"sta $BB80+(18*40)+4;"
		"sta $BB80+(19*40)+4;"
		"sta $BB80+(20*40)+4;"
		"sta $BB80+(21*40)+4;"
		"sta $BB80+(22*40)+4;"
		"sta $BB80+(23*40)+4;"
	);
}
display_right_wall()
{
	asm("lda #254;"
		"sta $BB80+(7*40)+35;"
		"sta $BB80+(8*40)+35;"
		"sta $BB80+(9*40)+35;"
		"sta $BB80+(10*40)+35;"
		"sta $BB80+(11*40)+35;"
		"sta $BB80+(12*40)+35;"
		"sta $BB80+(13*40)+35;"
		"sta $BB80+(14*40)+35;"
		"sta $BB80+(15*40)+35;"
		"sta $BB80+(16*40)+35;"
		"sta $BB80+(17*40)+35;"
		"sta $BB80+(18*40)+35;"
		"sta $BB80+(19*40)+35;"
		"sta $BB80+(20*40)+35;"
		"sta $BB80+(21*40)+35;"
		"sta $BB80+(22*40)+35;"
		"sta $BB80+(23*40)+35;"
	);
}

color_inverse_menu()
{
	asm("lda #$BB;"
		"sta read_menu+2;"
		"sta write_menu+2;"
		"lda #$D1;"
		"sta read_menu+1;"
		"sta write_menu+1;"

		"ldy #25;"	// page height	
		"suite_y:"
		"clc;"
		"ldx #38;"
		"suite_x:"
		"clc;"
		"read_menu:"
		"lda $1234,x;"
		"cmp #33;"   // if (a == 33) =='!'
		"bne saut_33;"
		"lda #32;"   // put space char
		"ora #128;"  // inverse color
		"saut_33:"
		"cpy #10;"
		"bcc sauty;"
		"cmp #32;"   // if (a == 32)
		"bne saut_32;"
		"lda #122;"  // put brick
		"ora #128;"  // inverse color
		"saut_32:"
		"sauty:"
		"write_menu;"
		"sta $1234,x;"
		"dex;"
		"bne suite_x;"
		"clc;"
		"lda read_menu+1;"  // load low address
		"adc #40;"			// a=a+40
		"bcc saut_menu;"    // si pas de retenue on saute à saut_menu
		"inc read_menu+2;"  // ajout de la retenue
		"inc write_menu+2;"
		"saut_menu;"
		"sta read_menu+1;"
		"sta write_menu+1;"

		"dey;"
		"bne suite_y;");
}

void display_house()
{
	display_ceiling();
}

#
#
#  MAIN GAME LOOP
#
void main_game_loop()
{
	int tt;
	poke(0x26A, 10); // 11 curseur revient ?

	poke(0x24F, 1);
	poke(0x24E, 1);

	do
	{
#if def DEBUG
		timer1 = deek(0x276);
#endif

#ifdef SLOW_DOWN_DURATION 
		for (tt = 0; tt < SLOW_DOWN_DURATION; tt++);
#endif
		//poke(782,64); disabled Interruptions Keyboard Wait Timer 

		drop_sliding_outside();
		disp_aldo(px, py);
		// display the cat
		manage_lightning();
		manage_falling_obj();
		manage_cat();
		manage_fireball();
		manage_rain();
		display_right_wall(); // May be better ...
		display_left_wall();
		// display Aldo

		//poke(782,192);


		// water flood level
		for (i = 23; i > 23 - waterlevel; i--)
		{
			poke(SCRADDR + (40 * i) + 5, 22);//left 
			poke(SCRADDR + (40 * i) + 36, 23);//right
		}

		if (waterlevel == 4)
		{
			// noyade animation
			endgame = 1; // loose
		}
		// time to keep paper & ink flash
		if (drop_floor_time > 0)
		{
			if (drop_floor_time == 1)
			{
				paper(7);
				ink(4);
			}
			drop_floor_time--;
		}
		// time to keep paper & ink flash
		if (drop_catch_time > 0)
		{
			if (drop_catch_time == 1)
			{
				paper(7);
				ink(4);
			}
			drop_catch_time--;
		}
		// time to keep paper & ink flash
		if (shoot_cat_time > 0)
		{
			if (shoot_cat_time == 1)
			{
				paper(7);
				ink(4);
			}
			shoot_cat_time--;
		}
		// time to keep paper & ink flash
		if (shoot_tile_time > 0)
		{
			if (shoot_tile_time == 1)
			{
				paper(7);
				ink(4);

			}
			shoot_tile_time--;
		}
		if (shoot_fireball_time > 0)
		{
			if (shoot_fireball_time == 1)
			{
				paper(7);
				ink(4);
			}
			shoot_fireball_time--;
		}
		wait_to_music--;

		if (active_music)
		{
			if (wait_to_music == 0)
			{
				play_music();
				wait_to_music = wait_to_music_delay;
			}
		}

		//  if (game_timer>10)
		//	{  
		//    wined(); // test la fin
		//	break;
		//	}

		//

		//

		if (game_timer > 1)
			k = key();

		if (walking == 1)
			if (walking_alt == 0)			
				walking_alt = 1;							
			else
				walking_alt = 0;					
		
		//gotoxy(2,0);printf("IDX=%d CODE=%d TIME=%d  "  ,index_raindrop/2,codedrop,raindroptime);
		//gotoxy(2,0);printf("walking_alt=%d",walking_alt);
		//sprintf(SCRADDR+2,"IDX=%d"  ,index_raindrop/2);


		// manage jumping timer
		if (jump_time > 0)
		{
			jump_time--;
			if (jump_time == 1) // player Fallout
			{
				
				playSoundEffect(player_fallout_sound,2);

				display_floor();

				legsup = 0;
			}
			wait = 0;
		}
		if (standuptime > 0)
			standuptime--;
		//gotoxy(2,25);printf("lastk=%d",lastk);
		

		//  Manage Keyboard durin playing

		if (jump_time == 0)
		{
			// KEY UP OR <SPACE>
			if ((k == 32) || (k == 11)) // keys 'space" or <up arrow>
			{
				if (benddown == 1) // stand up from ground
				{
					standuptime = 4;  // 4 game loop cycle

					legsup = 0;
					benddown = 0;
					jump_time = 0;
					wait = 0; // cancel the pause
					// have to clear key buffer
					//poke(0x24E,10);
					// redraw floor 
					display_floor();
				}
				else		// else jump
				{
					if (standuptime == 0)
					{
						jump_time = jump_max_time;
						wait = 0;
						
						playSoundEffect(player_jump_sound,2);
							
						
						legsup = 1;
					}
				}
			}
		}
		//# KEY DOWN : Adlo benddown     key = <down arrow>
		if ((k == 10) && (benddown == 0) && (jump_time == 0))
		{
			benddown = 1;
			legsup = 1;
			// faire uniqement benddown_time=1 au lieu de zero c'est tout
			// pour que Aldo reste au sol tant qu'il ne fait pas de jump ou fleche du haut

			// clear head zone
			gotoxy(px - 1, py); printf("   ");
		}

		//if (benddown_time>0)
		//	benddown_time--;	
#ifdef DEBUG
		if (benddown == 1) 
 		
			gotoxy(25, 0); printf("STOOP");
		
		else
		
			gotoxy(25, 0); printf("     ");
		
#endif

		// RIGHT key pressed   'P' or <right arrow>
		if ((k == 9 || k == 'P') && (px < 32) && (jump_time == 0))
		{
			if (benddown == 1)
			{
				benddown = 0;
				legsup = 0;
				// clear left foot
				*(unsigned char*)(SCRADDR + ((py + 3) * 40) + px - 2) = 32;
			}
			walking = 1; // legs in walking mode
			px += 1;
			wait = 0;
		}
		
		// LEFT key pressed 'O' or <left arrow>
		if ((k == 8 || k == 'O') && (px > 7) && (jump_time == 0))
		{
			if (benddown == 1)
			{
				benddown = 0;
				legsup = 0;
				// clear right foot
				*(unsigned char*)(SCRADDR + ((py + 3) * 40) + px + 2) = 32;
			}

			walking = 1; // legs in walking mode
			px += -1;
			wait = 0;
		}
		
		// escape key pressed
		if (k == 27)
		{
			life = 0;
			//    goto fin;
			endgame = 1;
		}
#ifdef DEBUG
		if (k == 76) //'l' throw a lightning
		{
			tile_x = 22;
			lightning = 1;
		}
#endif
		if (k == 83 && lastk != 83) // touche 'S'
		{
			if (active_sound == 1)
			{
				active_sound = 0;
				gotoxy(15, 0); printf("SOUND OFF");
				ping();
				playSoundEffect(silence_sound,0);
			}
			else
			{
				active_sound = 1;
				gotoxy(15, 0); printf("SOUND ON");
				ping();
			}
			for (i = 0; i < 15000; i++);
			gotoxy(15, 0); printf("         ");
		}
		if (k == 59 && lastk != 59) // touche 'M'
		{
			if (active_music == 1)
			{
				active_music = 0;
				gotoxy(15, 0); printf("MUSIC OFF");							
				playSoundEffect(silence_sound,0);
				ping();
			}
			else
			{

				active_music = 1;
				gotoxy(15, 0); printf("MUSIC ON");
				wait_to_music = 0;
				ping();
			}
			for (i = 0; i < 15000; i++);
			gotoxy(15, 0); printf("         ");

		}

#ifdef DEBUG
		if (k == 68)  // 'D' direct to method for debugging
		{
			endgame = 2;
			armsdown = 1;
			playSoundEffect(silence_sound,0);
			wingame();
			k = 0;
			return;
		}
#endif
		if (k == 127)  // <back space> game pause
		{
			playSoundEffect(silence_sound,0);
			sprintf(SCRADDR + (40 * 14) + 18, "PAUSE");
			do
			{
				k = key();
				drop_sliding_outside();
				for (wait = 0; wait < 600; wait++);
			} while (k == 127 || k == 0);
			k = 0;
			sprintf(SCRADDR + (40 * 14) + 18, "       ");
		}

#ifdef DEBUG
		if (k == 49)  // do end game
		{
			endgame = 2;
		}
		if (k == 67)  // 'C' for debugging ceiling falling
		{
			wave_nbr++;  // vague des gouttes suivantes
			house_shaking();
			if (ceiling_y < 18)
				ceiling_y++; // le plafond s'affesse d'un caractere

			//raindroptimer[rdindex]=wait_fall_raindrop; //

			if (active_sound == 1)
			{
				explode();
				wait_to_music = 12;
			}
			display_ceiling(); // affichage du plafond

			if (ceiling_y == py) // si le plafond est au niveau de la tete du hero
			{
				if (active_sound = 1)
					explode();
				wait_to_music = 12;
				// fin partie
			}

			display_wave_level_timer = 30; // durée d'affichage du numero de la vague suivante
		}
#endif
		if (k == 0) // player do nothing
		{
			player_wait++;  // wait sans rien faire;
			walking = 0;
		}
		else  // if activity keep eyes open		
			player_wait = 0.;
		

#ifdef DEBUG
		if (k == 78) // 'N' next wave only to devlopper
		{


			drop_sliding == 0;
			while ((rain[index_raindrop] != 255) && (index_raindrop < sizeof(rain) - 1))
			{

				index_raindrop += 2;
			}

			k = 0;
		}
#endif
		if (endgame == 2)  // win the game
		{
			playSoundEffect(silence_sound,0);
			wait_to_music = 7;
			armsdown = 1;
			wingame();
			k = 0;
		}

		if (player_wait > 200)
			player_wait = 100;

		// display life state :-/


		if (life >= 1)
			poke(0xBF93, 111 | 128);
		else
			poke(0xBF93, 97);
		if (life >= 2)
			poke(0xBF95, 111 | 128);
		else
			poke(0xBF95, 97);
		if (life >= 3)
			poke(0xBF97, 111 | 128);
		else
			poke(0xBF97, 97);
		if (life >= 4)
			poke(0xBF99, 111 | 128);
		else
			poke(0xBF99, 97);

		if (life == 0)  // loose Game
			endgame = 1;



		// wave display
		if (display_wave_level_timer > 0)
		{
			sprintf(SCRADDR + (40 * 14) + 17, "WAVE %d", wave_nbr);
			if (display_wave_level_timer == 1)
			{
				sprintf(SCRADDR + (40 * 14) + 17, "       ");
			}
			display_wave_level_timer--;
		}


		game_timer++; // compteur de temps dans le jeu utile pour plusieurs raisons, nottemment déclenchement vagues de gouttages
		display_score();
		lastk = k;  // save last key pressed
#ifdef DEBUG
		timer2 = deek(0x276);
		sprintf(SCRADDR + 10, "DELAY:%d  ", (timer1 - timer2));
#endif
	} while (endgame == 0);
	playSoundEffect(silence_sound,0);
	poke(0x24E, 10);
}

void init_default_var()
{
	// initialisation des variables globales
	raindroptime = 0; 			//delai d'wait entre entre 2 gouttes
	waterlevel = 0; 				// niveau de l'eau au sol
	ceiling_y = def_ceiling_pos;	// default ceiling position
	endgame = 0;					// flag to game end
	px = 18;						// position X du joueur
	py = 20;						// position Y du joueur
	armsdown = 0;					// arms down =1
	legsup = 0;					// to jump & benddown
	score = 0;					// score du joueur
	walking = 0;					// indicateur de walking du joueur ( non static)
	walking_alt = 0;				// to change legs char to walking
	wait = 0;					    // pour besoin de pause dans le jeu
	player_wait = 0;
	catx = 0;  					// position X du cat
	caty = 23;           			// position Y du cat
	cat = 0; 						// pour savoir si le cat est prensent ou pas
	seecat = 0;

	fireballx = 0;                // x fireball position 
	firebally = 20;               // y fireball position 
	fireball = 0;                 // pour savoir si le fireball est prensent ou pas
	dirfireball = 0;              // direction du fireball
	fireballwait = 0;             // delai du fireball qui reste sur place avant de partir
	colcattime = 0;

	seefireball = 0;
	colfireballtime = 0;

	jump_time = 0; 				// pas d'etat de saut par defaut . indicateur de saut du joueur, si en état de saut 
	benddown = 0;
	standuptime = 0;

	index_raindrop = 0 * 2; 	    // ****** must be pair. se positionner au début du tableau des positions et timing des gouttes
	drop_catch_time = 0;          //  
	shoot_cat_time = 0;
	shoot_fireball_time = 0;
	colcat = 1;					// enable cat collision
	shoot_tile_time = 0;
	game_timer = 0;

#ifdef DEBUG
	life = 10;                    // nombre de life du personnage normal : 3
#else
	life = 4;
#endif
	tile_fall = 0;  				// tuile non active par défaut
	tile_x = 0;					// x tile position
	tile_y = 0;					// y tile position
	obj_kernel = 0;
	obj_ice = 0;
	obj_y = 0;
	lightning = 0;	      		//        lightning appear
	lightning_time = 0;			// lightning appear duration

	k = 0;						// k = key(); contient la touche enfoncée
	game_timer = 0;

	music_index = 0;
	wait_to_music = wait_to_music_delay;
	volume = 7;

	active_sound = 1;
	active_music = 1;
	rdindex = 0;					// index initialize
	display_wave_level_timer = 30; // number of game loop cycle to keep on screen the wave level
	being_falling = 0;
	altchar = 1;					// used to alternate between 2 drop character
	drop_sliding = 0;
	scroll_text = 0;
	scroll_text_time = 0;

	wave_nbr = 0;				    // numéro de vague de gouttes en cours . vague de goutte=niveau

	// redef drop
	redefine_raindrop();

	// clear raindrop array 
	for (i = 0; i < RAINDROPMAX; i++)
	{
		raindropx[i] = 0;
		raindropy[i] = 0;
		//raindroptimer[i]=0;
		raindropstate[i] = 0;
	}

	// *********** for developpment -> goto define wave nbr 
	//while ((rain[index_raindrop+2]!=255)&&(index_raindrop<sizeof(rain)-1))
	//	index_raindrop+=2;

}
// display menu/intro page
display_menu()
{
	paper(6); ink(4);

	// scrolling hybrid graphics mode : Text & Hires
	poke(SCRADDR + 40, 30);
	for (i = 8; i < 16; i++)
		poke(0xA000 + (40 * i) + 2, 26);

	// Scrolling Text colors
	*(unsigned char*)(0xA000 + (40 * 8) + 1) = 1 & 7;
	*(unsigned char*)(0xA000 + (40 * 9) + 1) = 5 & 7;
	*(unsigned char*)(0xA000 + (40 * 10) + 1) = 3 & 7;
	*(unsigned char*)(0xA000 + (40 * 11) + 1) = 7 & 7;
	*(unsigned char*)(0xA000 + (40 * 12) + 1) = 3 & 7;
	*(unsigned char*)(0xA000 + (40 * 13) + 1) = 5 & 7;
	*(unsigned char*)(0xA000 + (40 * 14) + 1) = 1 & 7;
	*(unsigned char*)(0xA000 + (40 * 15) + 1) = 0 & 7;

	AdvancedPrint(0, 0, "                                        ");
	AdvancedPrint(2, 1, "                                      ");
	// clear background
	for (i = 16; i < 28; i++)
	{
		AdvancedPrint(2, i, "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
		poke(SCRADDR + 1 + (i * 40), 4);
	}

	// screen of menu page
	AdvancedPrint(2, 2, "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2, 3, "mmmmmmm     mmmm    mmm mm mmmm mmmmmm");
	AdvancedPrint(2, 4, "mmmmmm! !!!! mm !!!! m!mm!  mm! mmmmmm");
	AdvancedPrint(2, 5, "mmmmmm! mmm! m! mmm! mm m! ! m! mmmmmm");
	AdvancedPrint(2, 6, "mmmmmm!     mm!      m! m! m! ! mmmmmm");
	AdvancedPrint(2, 7, "mmmmmm! !!! mm! !!!! m! m! mm!  mmmmmm");
	AdvancedPrint(2, 8, "mmmmmm! mm!! m! mmm! m! m! mmm! mmmmmm");
	AdvancedPrint(2, 9, "mmmmmm!mmmm!mm!mmmm!mm!mm!mmmm!mmmmmmm");
	AdvancedPrint(2, 10, "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2, 11, "mmm     mmmm    mmm mmmm mm mmm    mmm");
	AdvancedPrint(2, 12, "mm! !!!! mm !!!! m!  mm! m!mmm !!!! mm");
	AdvancedPrint(2, 13, "mm! mmm! m! mmm! m! ! m! mm m! mmm!mmm");
	AdvancedPrint(2, 14, "mm!     mm!      m! m! ! m! m! mmmmmmm");
	AdvancedPrint(2, 15, "mm! !!!mmm! !!!! m! mm!  m! m! mmmm mm");
	AdvancedPrint(2, 16, "mm! mmmmmm! mmm! m! mmm! m! mm!    mmm");
	AdvancedPrint(2, 17, "mm!mmmmmmm!mmmm!mm!mmmm!mm!mmmm!!!mmmm");
	AdvancedPrint(6, 21, "\23\4DIRECTION : ARROWS KEY < > \4\26");
	AdvancedPrint(13, 23, "\23\4SPACE TO JUMP\4\26");
	AdvancedPrint(10, 25, "\23\4PRESS SPACE TO PLAY\4\26");
	// text color   // 12 = blink

	color_inverse_menu();

	poke(SCRADDR + (25 * 40) + 9, 12);
	poke(SCRADDR + (25 * 40) + 31, 8);


	// to debbug
	// gotoxy(20,0);printf("CODE=%d IDX=%d",rain[index_raindrop],index_raindrop);
}
void main()
{
	unsigned char kmenu = 00;
	unsigned int d1, d2;
	int timer, raintimer;
	redefine_char();
	redefine_charExt();
	volume = 7; // set volume to middle

	init_default_var();
	// cursor hide ?
	*(unsigned char*)0x26A = 10;

	cls();

	display_outside();

	//display_house();
	//display_floor();
	display_menu();

	game_timer = 0;
	//SoundEffect(rain_menu_sound,volume);

	do
	{

		scroll_text_time++;

		timer++;
		raintimer++;
		if (timer == 400)
		{
			scroll_left(1, 8);
			scroll_right(10, 8);
		}
		if (timer == 800)
		{
			scroll_right(1, 8);
			scroll_left(10, 8);
			timer = 0;
		}

		// SIMPLE TEXT SCROLLING
		if (scroll_text_time == 192)
		{

			asm(
				"lda #40;"
				"sta $FFFF;"  // to debug
				"ldy #0;" // start at column 0
				"clc;"
				"suite_scl;"
				"iny;"
				"lda $BB82+40,y;"
				"dey;"
				"sta $BB82+40,y;"
				"iny;"
				"cpy #38;"
				"bne suite_scl;");

			*(unsigned char*)(SCRADDR + 40 + 39) = credits_text[scroll_text];
			scroll_text++;
			if (scroll_text == sizeof(credits_text) - 1)
			{
				scroll_text = 0;
				scroll_text_time = 0;
			}
			scroll_text_time = 0;
		}

		if (raintimer == 70)
		{
			drop_sliding_outside();
			raintimer = 0;
		}

		// gotoxy(2,0);printf("ENDGAME=%d",endgame);
		kmenu = key();

		//drop_sliding++;
#ifdef DEBUG
		if (kmenu != 0)
		{
			gotoxy(30, 0);
			printf("KEY=%d ", kmenu);
		}
#endif
		if (kmenu == 83) // touche 'S'
		{
			if (volume < 15)
				volume++;
			else
				volume = 0;


			//	SoundEffect(rain_menu_sound,sound_volume);
		}


		// if press space key to start game
		if (kmenu == 32)
		{

			playSoundEffect(silence_sound,0);
			init_default_var();
			cls();
			gotoxy(0, 0); printf("                                        ");
			paper(7);
			display_outside();
			display_house();

			display_floor();

			main_game_loop();
			if (endgame == 1) // GAME OVER
			{
				// do here GAME OVER MUSIC
				gotoxy(16, 14); printf("GAME OVER ");
				//gotoxy(16,14);printf(" SCORE:%d",score);
				for (wait = 0; wait < 40000; wait++);
				//poke(SCRADDR+(13*40)+9,12);
				//poke(SCRADDR+(13*40)+8,0);
				//poke(SCRADDR+(15*40)+31,4);
				//poke(SCRADDR+(15*40)+30,8);

			}
			endgame = 0;
			cls();
			display_menu();
			kmenu = 0;

		}
	} while (kmenu != 65);
	//fin:
	//cls();   
	//return;  // unreachable code here 
	//while(1)
	// restore ram from rom (0xFC78) charset, but bug stilling, good address ?
	j = CARADDR + (32 * 8);
	cls();
	for (i = 0xfc78; i < 0xfc78 + (8 * 95); i++)
	{
		*(unsigned char*)j = peek(i);
		j++;
	}
	printf("PLEASE RESET YOUR ORIC");
	return;
}


