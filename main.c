//  RAIN PANIC   alpha version
//  GOYO 2019 (C)
//
//
// reste à implémenter:
// -------------------
//
// les différents niveaux -> toutes les vagues de pluie . actuellement que la 1ere vague
//
// l'eau qui monte dans la maison lorsque les gouttes tombent au sol
// 
// gestion de l'actication du son et de la musique
// 
//
// BUGS :
// ------
//  
// goutte dans la maison parfois ne s'efface pas du plafond.
//
//
//  autres idées
//  ------------
// type de goutte : coeur qui tombe : vie + 1  ?
// 
//
// siphon ?: vide eau  en forme de robinet ?
//
//
//
/*  chars coding screen
  25C=7,64  WAVE 1,zzzzzzzzzzzzzzzANDz    
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm  
  m ~zzlzzzzzzzzzzzzzzzzzzzzzzzzzzz~mm  
  m ~                              ~mm  
  mm~  l                           ~mm  
  mm~  k                           ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~                              ~mm  
  mm~            [\]               ~mm  
  mm~            ^_`               ~mm  
  mm~            abc               ~mm  
  mm~            ghi               ~mm  
  ~~~                              ~~~  
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
  ~o~o~o~~~~~~~~~~~~~~~~~~SCORE 0 ~~~~  
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
 
  Oric Std Charset                                
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
  
  
#include <lib.h>

#define RAINDROPMAX 24       // max drop sprite in the screen
#define jump_max_time 8      // time to keep in jump state
#define music_tempo_delay 4  // delay to temporize the music

#define SCRADDR 0xBB80   // address of the text screen
#define CARADDR 0xB400   // address of std chars to redefine them

// arrays to manage rain drops, for : x y timer state alstate

unsigned char raindropx[RAINDROPMAX];
unsigned char raindropy[RAINDROPMAX];
//unsigned char raindroptimer[RAINDROPMAX]="";
unsigned char raindropstate[RAINDROPMAX]="";

int           index_raindrop;   // index to read from the rain[] array

unsigned char codedrop;          // to read drop x position value in Rain[] array.
int           raindroptime;      // to read time drop => time between two drops

char 		  drop_catch_time=0;
char 		  shoot_cat_time=0;
char          shoot_tile_time=0;
char          drop_floor_time=0;
char          endgame;           // loose game = 1 winned =2
unsigned int  scroll_text;
unsigned char scroll_text_time;
char          px,py;             // player position x & y
char          waterlevel;
int           score;
int           music_index;       // music in for music array indexing
int           music_tempo;       // music temporisation 
unsigned int  wait;
char          active_sound;
char          active_music;
char          sound_volume;
unsigned char *ptr;              // for sound effect
char          walking;           // manage walking
char 		  walking_alt;       // alternate legs chars to walking
unsigned char jump_time=0;       // gestion du saut du héro 

char unsigned armstandup;        // to manage the rams state

char unsigned catx;              // x cat position 
char unsigned caty;              // y cat position 
char unsigned cat;               // pour savoir si le cat est prensent ou pas
char          dircat;            // direction du cat
char unsigned catwait;           // delai du cat qui reste sur place avant de partir
char          seecat;            // if cat is visible  
char unsigned player_wait;       // close eyes afeter no activity
unsigned char k,k2;	             // to read the key pressed  : k=key(); 	

char unsigned wave_nbr;
char          ceiling_y;         // hauteur du plafond = 8 fixe

unsigned int  game_timer;        // compteur de temps général durant le jeu
char          life; 			 // player lifes
char unsigned tile_fall;         // si foudre active
char unsigned lightning_dice;    // dé de hazard pour la foudre
int  unsigned lightning_time;
char unsigned tile_x;            // position x of the beam
char unsigned tile_y;            // position y of the beam

unsigned char rdindex; 
unsigned char floor_y=25;
char          altchar;			 // to alternate char for the falling rain drop
unsigned char drop_sliding;		 // y step of vertically scrolling for the rain drop
int           adr;   		     // use for assembly inline
unsigned int  i,j;               // usefull in many time in this program  
				
unsigned int  wait_fall_raindrop=11;  // delay of time for the speed of falling rain drop
unsigned      display_wave_level_timer;
unsigned char xwall;

unsigned char lx;                // or lightning x position
unsigned char ly;                // for lightning y position

unsigned char being_falling;

// ma,que son pour le JUMP, la retombé du jump, l'eau tombe à terre
unsigned char bong_sound[]={104,177,184,44,254,29,166,58,27,83,35,74,2,83};
unsigned char cat_collision_sound[]={102,144,100,122,168,7,252,9,215,92,153,10,10,1};
unsigned char cat_at_door_sound[]={28,188,255,21,247,92,136,18,176,229,253,145,1,9};
unsigned char rain_menu_sound[]={105,243,91,101,202,108,209,215,73,142,13,178,71,11};                          
unsigned char silence_sound[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};                                        
unsigned char player_fallout_sound[]={68,64,207,83,241,143,141,119,218,145,164,167,2,211};


void AdvancedPrint(char x_pos,char y_pos,const char *ptr_message);

char credits_text[]="             RAIN PANIC                "
					"           INSTRUCTIONS :  DURING A STRONG THUNDERSTORM HELP MR ALDO TO CATCH ALL THE DROPS OF WATER FALLING FROM HIS HOUSE CEILING "
					"    USE LEFT AND RIGHT ARROWS TO MOVE ALDO"
					"    USE SPACE TO JUMP        ESC TO EXIT   "
					"                                                    "
					"  CODING BY GOYO, IN C LANGUAGE WITH ORIC OSDK AND FEW ASSAMBLY CODE"
					"  THANKS TO THE HELP OF MANY MEMBERS OF COE FORUM : DBUG  LAURENTD75"
					"  LADYWASKY  DRPSY  KENNETH  ISS AND OTHERS ORICIANS ..           "
					"  HAVE FUN .....                     ";
				
char game_music[] = {3,10,2,4,1,4,4,3,2,4,5,3,4,7,1,4,5,2,4,3,4,3,12,2,3,8,4,3,10,1,
3,12,1,4,1,4,3,10,2,3,10,3,3,9,1,3,10,2,3,12,4,3,9,2,3,5,4,3,10,2,  
4,1,4,4,3,2,4,5,3,4,7,1,4,5,2,4,3,4,3,12,2,3,8,4,3,10,1,3,12,1,     
4,1,3,3,12,1,3,10,2,3,9,2,3,7,2,3,9,2,3,10,4,3,10,2,3,10,6,0,0};


//
//   WAVES OF RAINDROP DATA 
//
// RAIN[] -> raindrop array [XPOS(drop), DELAY(between 2 drop),,,,]
// rain drop must be bewteen 6 and 32 x position
// 255 = next wave
// 250 = cat appear on the left (cardir)
// 251 = cat appear on the right
// 248 = Life fall   - futur append ?
// 247 = siphon fall - futur append ?
// 200-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)

// 0x08 or 0x09 must be in hexa don't know why. Its look lika a bug ?!
unsigned char rain[] = {   //----------------------------------------------|new wave|
// wave 1

  07,50 ,32,30 ,19,30,  7,30  ,32,40 ,7,60,  16,0,  18,0, 20,30,
  07,0  ,9,0    ,251,50 ,26,0  ,28,0,  30,0,  32,30, 
  15,0  ,17,0   ,19,0  ,21,0,  23,30,   // 0 force to align drops in same line
  07,30 ,9,10  ,11,30 ,251,0    ,28,30  ,30,50,  252,0, 07,40, 251,0 ,32,30, 22,50,
  251,0, 21,40,252,0, 21,40, 251,0,21,30,  // 38eme

  13,35 ,30,35, 251,0 ,07,35 ,251,0, 32,35, 250,0 ,18,35, 215,0, 250,0, 07,35, 251,0, 32,35, 250,0 ,07,35,
  251,0, 224,0 ,32,35 ,251,0, 07,35, 251,0, 215,0, 32,35, 251,0, 215,0, 07,35, 31,35, 17,35, 14,35, 251,0, 
  25,35, 207,0, 07,35, 32,35, 07,35, 32,35, 251,0, 20,35, 220,0, 32,35, 07,35, 215,0, 31,35, 215,0, 07,35, 226,0, 30,35, 
  255,0,
  250,0,  0x08,35,  200,0, 208,0,  29,35,  251,0,  0x09,35, 220,0,28, 35,251,  0,10,  35, 251,   0,13,   40, 230,   0,30, 40, 16,40, 208,0, 07,40, 250,0,32,40, 
  18,40,  07,40,    10,40, 251,0,  15,40,  11,40,  24,40,   255,0x0,  32,40,  05,40,  07,0x00, 0x8,0x00, 0x9,0x00, 10,0X00,
  11,0x0, 12,0X0,  13,0x0, 14,0x0, 15,0x0, 16,0x0, 17,0x0,  18,0x0,   19,0x0, 20,0x0, 21,0x0,   22,0x0,  23,0x0,   24,0x0, 25,0x0,26,0x0,27,0x0,28,0x0,29,0x0,30,0x0,31,0x0,32,0x0,33,0x0,34,0x0
  
};

void play_music()
{

	music(1,game_music[music_index],game_music[music_index+1],0);
	play(1,0,1,(1110*game_music[music_index+2]));
	music_index+=3;
	if (music_index>=sizeof(game_music))
	{
	   music_index=0;
	}
}

// redefined characters for graphics and sprites game
unsigned char redefchar[]={

// futur possibilité : player les bras en bas 
// 00,00,00,00,00,00,00,30, // 86 à haut de la tête
//00,00,00,00,00,00,00,01, // 87 à gauche de la tête
//00,00,00,00,00,00,00,32, // 88 à droite de la tête
//03,03,06,06,06,00,06,06, // 89 à gauche du corps
//48,48,24,24,24,00,24,24, // 90 à droite du corps

01,01,01,01,02,03,06,06, // 91
63,63,63,63,63,30,00,30, // 92
32,32,32,32,16,48,24,24, // 93
06,06,06,06,07,03,01,00, // 94
63,45,63,51,30,45,63,63, // 95
24,24,24,24,56,48,32,00, // 96
00,00,00,00,00,00,00,00, // 97 blanc coté gauche du ventre, peut etre recyclé
63,63,63,00,63,63,63,51, // 98
00,00,00,00,00,00,00,00, // 99 blanc // peut etre recyclé  pour faire coeur 
00,00,00,00,00,00,01,01, // 100
51,51,51,00,30,30,63,63, // 101
00,00,00,00,00,00,32,32, // 102
01,01,01,00,01,01,07,07, // 103
51,51,51,00,33,33,33,33, // 104
32,32,32,00,32,32,56,56, // 105
63,63,63,51,30,45,63,63, // 106
0x08,0x08,28,28,62,62,46,28,  // 107   goutte à rattraper 1/2 
00,00,00,00,00,00,00,00, // 108  goutte à rattraper 2/2
//00,0x08,28,28,62,62,46,28, // 109  goutte de l'extérieur de la maison (pour animation) et page d'intro
00,00,04,04,14,14,10,04,
00,00,00,00,00,00,00,00, // 110
30,63,45,63,63,45,51,30, // 111 head of life number
04,28,31,15,03,03,02,06, // 112
01,01,62,60,60,12,38,42, // 113
16,16,15,07,07,06,12,10, // 114
04,07,63,62,56,24,40,44, // 115
00,00,00,00,06,06,07,07, // 116
00,00,00,00,00,03,47,47, // 117
00,00,00,00,00,48,61,61, // 118
00,00,00,00,24,24,56,56, // 119
01,03,03,07,15,31,31,63, // 120
32,48,48,56,62,60,60,63, // 121
62,62,62,62,62,62,62,28, // 122 tile
03,06,12,31,00,00,00,01, // 123 lightning left
00,00,00,60,12,24,48,32, // 124 lightning right
03,02,04,0x08,16,32,00,00// 125 lightning point 3eme caractere redefinit
// 126, 127 allready used in them native graphics
}; 

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
// player falling by collission with cat or tile
void player_falling()  
{
   char i;
   int time;
   for (i=0;i<5;i++)
   {
       
       disp_player(px,py+i);
	   //plot(px,py-1+i,32);
	   //plot(px+1,py-1+i,32);
	   //plot(px-1,py-1+i,32);
	   for (time=0;time<2500;time++);
	   if (active_sound==1)
	   {
	      shoot();
		 music_tempo=7;  
	   }
	  // must manage bottom screen repaint
	}
     display_floor();   // bug dans cette methode voir si pbl de clc ou registre a sauver sur pile ? 
 }
// manage lightning
void manage_lightning()
{ 
     if (tile_fall==0)
	 {
	  
              
		if (lightning_dice==1)
		{
		   // afficher foudre
		   tile_fall=1;
		
		   tile_y=ceiling_y-3;
		 //  tile_y=5;
		   tile_fall=1;
		   
		   lightning_time=12; // temps de durée de l'éclair  
		   if (active_sound==1)
		   { 
			      explode();
				  music_tempo=12;
		   }
		}
     }
	 if (lightning_time>0)
	 {
	    lightning_time--;
	    for (ly=0;ly<ceiling_y-1;ly++)
		{
		    // affiche éclair
			//poke(0XBBAA+(40*ly)+tile_x-1,3);
			*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=123|128;
			*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=124|128;
			//poke(0XBBAA+(40*ly)+tile_x+2,4);
		}
		// pointe de l'éclair
		//poke(0XBBAA+(40*(ly))+tile_x-1,3);
		*(unsigned char*)(0XBBAA-1+(40*(ly))+tile_x)=125|128;
		//poke(0XBBAA+(40*(ly))+tile_x+1,4);
		if (lightning_time==1)
		{
		
			for (ly=0;ly<ceiling_y-1;ly++)
			{
			    // efface éclair avec gouttes
				*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=109;
				*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=109;
				*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=109;
				*(unsigned char*)(0XBBAA+(40*ly)+tile_x+2)=109;	
			}
			*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=109;
			*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=109;
			*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=109;
		    lightning_time=0;
		}
	 } 
	 if (tile_fall==1)  
	 {
		   // if tile fall on the player head
		   if ((px-2==tile_x )&&((tile_y>=py-1)&&(tile_y<=py+3)))
		   {
		    
               poke(0XBBAA+(40*tile_y)+tile_x,32);		
		       tile_fall=0; // 	    
			   shoot_tile_time=3; // 4 main program cycles for show color indacte correct catching
			   paper(3);
		 	   if (active_sound==1)
			   { 
			      shoot();
				  music_tempo=7;
			   }
			   being_falling=1;
			   player_falling();
			   being_falling=0,
			   life--;
		   }

	     if (tile_y<floor_y-3)
		 {
			tile_y++;		 
		     if (tile_y>ceiling_y+1)
             {          		
				*(unsigned char*)(0XBBAA+(40*(tile_y-1))+tile_x)=32;
				*(unsigned char*)(0XBBAA+(40*tile_y)+tile_x)=122;
				*(unsigned char*)(0XBBAA+(40*tile_y)+tile_x+1)=0;
				*(unsigned char*)(0XBBAA+(40*tile_y)+tile_x-1)=1;		   
			 }
			 else
			 {
			 }
		 }
		 else
		 {
			lightning_dice=0;
		    // display falling tile
			tile_fall=0;
			poke(0XBBAA+(40*tile_y)+tile_x,32);
		  
		 }
	 }

}
void manage_cat()
{
	unsigned int wait;	

	if (cat==0) //si pas de présence du cat générer une valeur aléatoire pour savoir s'il apparait
	{
		//seecat=rand()/(32768/10);
	
		 if (seecat==1) //Apparait à gauche
		 { 
   		     catx=3;
			 cat=1;
			 dircat=0;
			 catwait=10;
		 }
		 if (seecat==2)// apparait à droite
		 {
			
		     catx=34;
			 cat=1;
			 dircat=1;
			 catwait=10;
		 }
		 if (active_sound)
		 {
			//SoundEffect(cat_at_door_sound);				
			//music_tempo=2;
		}
	}
	else // cat ==1 , gérer la présence du cat et son déplacement
	{
         // affichage du cat
		 //   forme du cat de gauche à droite
		 if (dircat==0)
		 {
		 
	        *(unsigned char*)(0Xbb80+(caty*40)+catx-2)=32;
			*(unsigned char*)(0Xbb80+(caty*40)+catx-1)=0;
			*(unsigned char*)(0Xbb80+(caty*40)+catx)=114;
			*(unsigned char*)(0Xbb80+(caty*40)+catx+1)=115;
			
		 }
		 else  // forme du cat de droite à gauche
		 {
		 	*(unsigned char*) (0Xbb80+(caty*40)+catx)=112;
			*(unsigned char*) (0Xbb80+(caty*40)+catx+1)=113;
			*(unsigned char*) (0Xbb80+(caty*40)+catx-1)=0;
			*(unsigned char*)(0Xbb80+(caty*40)+catx+2)=32;
			
		 }
	     if (catwait>0) 
		 {
		     catwait--;
		 }
	     if (dircat==0) // de gauche à droite
		 {  
		    if (catwait==0)
			{
		       catx++;
			}
			if (catx==37)  //arrivé du cat à la position 36
			{
			   // effacement du cat par la pluie et mur
			  cat=0;
			 
			  seecat=0;
		      *(unsigned char*)(0Xbb80+(caty*40)+catx-2)=126;
			   
			  *(unsigned char*)(0Xbb80+(caty*40)+catx-1)=4;
			  *(unsigned char*)(0Xbb80+(caty*40)+catx)=109;
			}
		 } 
		 if (dircat==1) // de droite à gauche
		 {
		    if (catwait==0)
			{
		       catx--;
			}
			if (catx==2)  //arrivé du cat à la position 1
			{
				seecat=0;
			 // effacement du cat par la pluie et mur
				cat=0;
				*(unsigned char*)(0Xbb80+(caty*40)+catx)=109;
		     	*(unsigned char*)(0Xbb80+(caty*40)+catx+1)=0;
		    	*(unsigned char*)(0Xbb80+(caty*40)+catx+2)=126;
		
			}
		 }
        // gotoxy(30,2);printf("catx=%d ",catx);
        //gerer la collision du cat avec le héro
	    if (((catx==px)||(catx+1==px))&&(jump_time<3))
		{
	
		   if (active_sound==1)
		   { 	
			
				SoundEffect(cat_collision_sound);				
				music_tempo=9;				
				player_falling();
				
   	       }
		   life--;
		   shoot_cat_time=3; // 4 main program cycles for show color indacte correct catching
		   paper(3);
		   ink(7);
		   
		   if (dircat==1)
		   {
		      catx--;
		   }
		   if (dircat==0)
		   {
		     catx++;
		   }
		}
    }
}
void disp_player(char x,char y)
{
    if (armstandup==0)
	{
    // sceau
	    *(unsigned char*)(0xbb80+(y*40)+x-1)=91;
		*(unsigned char*)(0xbb80+(y*40)+x)=92;
		*(unsigned char*)(0xbb80+(y*40)+x+1)=93;
	// blancs
	    *(unsigned char*)(0xbb80+(y*40)+x-2)=5;
		//poke (0xbb80+(y*40)+x-2,5);
		*(unsigned char*)(0xbb80+(y*40)+x+2)=0;
	
		*(unsigned char*)(0xbb80+((y+1)*40)+x-1)=94;
		*(unsigned char*)(0xbb80+((y+1)*40)+x+1)=96;
	// blancs
		*(unsigned char*)(0xbb80+((y+1)*40)+x-2)=1;
		*(unsigned char*)(0xbb80+((y+1)*40)+x+2)=0;
	}
    // tete
	
    if (player_wait>100)  
	{
		*(unsigned char*)(0xbb80+((y+1)*40)+x)=106;
	}
	else  // close eyes after 100 cycle
	{	
		*(unsigned char*)(0xbb80+((y+1)*40)+x)=95;	
	}


	if (jump_time<3)
	{
	// ventre en mode non saut	
		*(unsigned char*)(0xbb80+((y+2)*40)+x-1)=97;
		*(unsigned char*)(0xbb80+((y+2)*40)+x)=98;
		*(unsigned char*)(0xbb80+((y+2)*40)+x+1)=99;		
    	*(unsigned char*)(0xbb80+((y+2)*40)+x-2)=1;	
		*(unsigned char*)(0xbb80+((y+2)*40)+x+2)=0;
	}
	else // ventre en mode saut
	{
		*(unsigned char*)(0xbb80+((y+2)*40)+x)=98;
	}
    // blancs
//	poke (0xbb80+((y+2)*40)+x-2,0);
//	poke (0xbb80+((y+2)*40)+x+2,0);
    // blancs pour effacer les jambes du saut
//	poke (0xbb80+((y+2)*40)+x-3,0);
//	poke (0xbb80+((y+2)*40)+x+3,0);

    // getion du saut pour mettre les jambes en l'air
    if (jump_time>2)  // >0
	{
	    // mettre les jambes sur les cotés 2xcar de chaque coté
		*(unsigned char*)(0xbb80+((y+2)*40)+x-1)=117;
		*(unsigned char*)(0xbb80+((y+2)*40)+x-2)=116;
        if (px<6)  // redessine le mur à gauche
	    {
	        *(unsigned char*)(0xbb80+((y+2)*40)+x-3)=126;
	    }
	    else
	    {
	       *(unsigned char*)(0xbb80+((y+2)*40)+x-3)=2;
		}
		//poke (0xbb80+((y+2)*40)+x,127);
		*(unsigned char*)(0xbb80+((y+2)*40)+x+1)=118;   
     	*(unsigned char*)(0xbb80+((y+2)*40)+x+2)=119; 
		
		if (px>31)  // redessine le mur à gauche
	    {
			*(unsigned char*)(0xbb80+((y+2)*40)+x+3)=126;  		
		}
		else
		{
			*(unsigned char*)(0xbb80+((y+2)*40)+x+3)=0;
		}
		if (jump_time==jump_max_time) // pour n'effacer qu'un fois les jambes du bas
		{
			// blancs à la place des jambes du bas
			*(unsigned char*)(0xbb80+((y+3)*40)+x-1)=32;
			*(unsigned char*)(0xbb80+((y+3)*40)+x)=32;
			*(unsigned char*)(0xbb80+((y+3)*40)+x+1)=32;
		}

	}
	else // manage leds on ground
	{
		
		if (walking==1)  // walking mode
		{
			
			*(unsigned char*)(0xbb80+((y+3)*40)+x-1)=100;
			*(unsigned char*)(0xbb80+((y+3)*40)+x)=101;
			*(unsigned char*)(0xbb80+((y+3)*40)+x+1)=102;
		}
		else
		{
			*(unsigned char*)(0xbb80+((y+3)*40)+x-1)=103;
			*(unsigned char*)(0xbb80+((y+3)*40)+x)=104;
			*(unsigned char*)(0xbb80+((y+3)*40)+x+1)=105;
		}
		if (px<8) // redraw wall on the left
		{
			*(unsigned char*)(0xbb80+((py+2)*40)+px-2)=4; 
			*(unsigned char*)(0xbb80+((py+2)*40)+px-3)=126|128; 
			
		}
		else
		{
			*(unsigned char*)(0xbb80+((y+2)*40)+x-3)=32;
		}
		
		if (px>31) // redraw wall on the right
		{
				*(unsigned char*)(0xbb80+((py+2)*40)+px+2)=4;
				*(unsigned char*)(0xbb80+((py+2)*40)+px+3)=126|128; 
				*(unsigned char*)(0xbb80+((py+1)*40)+px+2)=4;
				*(unsigned char*)(0xbb80+((py+1)*40)+px+3)=126|128; 
				*(unsigned char*)(0xbb80+((py)*40)+px+2)=4;
				*(unsigned char*)(0xbb80+((py)*40)+px+3)=126|128; 
		}
		else
		{
			*(unsigned char*)(0xbb80+((y+2)*40)+x+3)=32; 
		}
	}
	// blank char next the legs
	*(unsigned char*)(0xbb80+((y+3)*40)+x-2)=2;
	*(unsigned char*)(0xbb80+((y+3)*40)+x+2)=0;
    if (being_falling==1)
	{
		*(unsigned char*)(0xbb80+((y-1)*40)+x-1)=32;
		*(unsigned char*)(0xbb80+((y-1)*40)+x)=32;
        *(unsigned char*)(0xbb80+((y-1)*40)+x+1)=32;
	}
}
void display_score()
{
   gotoxy(26,26);printf("\5SCORE %d ",score);
}

// raindrop managment
//

void manage_rain()
{
	if (raindroptime>0)
	   raindroptime--; // espace de temps entre les gouttes: temps en therme de cycle
	
	
	//gotoxy(5,5);printf("TIME=%d  ",raindroptime);
	
	for (rdindex=0;rdindex<RAINDROPMAX;rdindex++)
	{	
		// activate a new drop
		// here we find a free place in the raindrop array if raindroptime=0
		if (raindropstate[rdindex]==0 && raindroptime<1)
		{
			raindropstate[rdindex]=1; // cellule de la goutte devient occupée
				
			if (altchar==0)
				raindropy[rdindex]=ceiling_y+1;
			else
				raindropy[rdindex]=ceiling_y+2;		
				
			//gotoxy(5,6);printf("TIME=%d  ",raindroptime);
			// 1rst code read from the rain array
			codedrop=rain[index_raindrop];			
			// initialiser la position x de la nouvelle goute depuis le tableau rain[]
			raindropx[rdindex]=codedrop;
			
			// delai entre 2 gouttes
			//décompte lors du déplacement de la goutte
			// à lire dans le tableau des vagues
			// second code read from the rain array	
			// raindroptime = wait delay to read next rain drop
			raindroptime=rain[index_raindrop+1];
					
			if (codedrop>199)
			{
				raindropstate[rdindex]=0;
				if (codedrop>199&&codedrop<227)
				{
					tile_x=codedrop-200;
				    // get codedrop for X position of lightening
					lightning_dice=1;
				}
				
				// if codedrop = 250 then cat appear on the right, 251 on the left
				if (codedrop==250)
				{
					if (seecat==0)
						seecat=1;
				}
				if (codedrop==251)
				{
					if (seecat==0)
						seecat=2;
				}
				// new wave of drops, ceiling fall one step
				if (codedrop==255)  // si la position de x=255 c'est une nouvelle vague de gouttes
				{
				
					wave_nbr++;  // vague des gouttes suivantes
					ceiling_y++; // le plafond s'affesse d'un caractere
			
					//raindroptimer[rdindex]=wait_fall_raindrop; //
			
					if (active_sound==1)
					{ 
						explode();
						music_tempo=12;
					}
					display_ceiling(); // affichage du plafond
			
					if (ceiling_y==py) // si le plafond est au niveau de la tete du hero
					{	
						if (active_sound=1)
							explode();
						music_tempo=12;
						// fin partie
					}
					
					display_wave_level_timer=100; // durée d'affichage du numero de la vague suivante

				}
			  
			}
		//	raindroptimer[rdindex]=wait_fall_raindrop; // wait_fall_raindrop
			
			// increment the raindrop array index
			if (index_raindrop<sizeof(rain)-2)
			{
				index_raindrop+=2;
			}
			else
			{					
			   endgame=2; // = winned
			   break;
			   // end of game Winned
			}
			break; // exit if we found a free place in array
		}						
	} // end for (rdindex=0	
	
	
	// here manage active rain drop in array
	for (rdindex=0;rdindex<RAINDROPMAX;rdindex++)
	{		
	    // here we dont wait for raindroptime=0 bc the player must be catch a raindrop during sliding
		if (raindropstate[rdindex]==1)
		{
			// erase on screen drop character on his final position
			
			// player catch a rain drop
			// prendre en compte le altchar pour le y
			if ((px==raindropx[rdindex])&&
			   ((py-2<raindropy[rdindex])&&(py+1>raindropy[rdindex])))
			{
				score++;
				drop_catch_time=2; //  main program 4 cycles for show color that indacate correct catching
				paper(6);
				ink(7);
				raindropstate[rdindex]=0;
				poke(0xbb80+((raindropy[rdindex]-1)*40)+raindropx[rdindex],32);
				poke(0xbb80+(raindropy[rdindex]*40)+raindropx[rdindex],32);
				//poke(0xbb80+((raindropy[rdindex]+1)*40)+raindropx[rdindex],32);
				
				// remplacer advancedprint par poke qui est + rapide pour 1xchar
				if (active_sound==1)
				{
					SoundEffect(bong_sound);
					music_tempo=7;							
				}
			}
		}
	} //end for (rdindex=0
	
	// here manage sliding state = 0 to alternate the two drop char
	if (drop_sliding==0) // etat initial du drop_sliding , la goutte a entierement glissé d'un char a l'autre
	{
		for (rdindex=0;rdindex<RAINDROPMAX;rdindex++)
		{			
			// if array cellule is busy  = rain drop active
			if (raindropstate[rdindex]==1)
			{
				
			// display an active raindrop
						
				if (altchar==0)
				{
				    *(unsigned char*)(0xbb80+(raindropy[rdindex]*40)+raindropx[rdindex])=107;
					*(unsigned char*)(0xbb80+((raindropy[rdindex]+1)*40)+raindropx[rdindex])=108;
			
				}
				else
				{
				    *(unsigned char*)(0xbb80+(raindropy[rdindex]*40)+raindropx[rdindex])=108;
					*(unsigned char*)(0xbb80+((raindropy[rdindex]+1)*40)+raindropx[rdindex])=107;
				}				
				*(unsigned char*)(0xbb80+((raindropy[rdindex]+1)*40)+(raindropx[rdindex]-1))=4;
				
				// erase drop trace on y axis (on ceiling)
				if (raindropy[rdindex]>ceiling_y+2)
					*(unsigned char*)(0xbb80+((raindropy[rdindex]-1)*40)+raindropx[rdindex])=32;					
			
				}
			else
			{
				for(wait=0;wait<10;wait++);
			}
		}	
		drop_sliding++;
	}
	else // manage intermediate rain drop sliding state  - etat de glissement de goutte intermédiaire
	{
	   // reserve time for late compentation
	   for(wait=0;wait<50;wait++);
	   // rain drop char (107&108) verticaly scroll managing
	   
	   // alternate altchar to swap position of 2 char of raindrop 
	   if (altchar==0)  // raindrop scroll 107->108  lda %altchar;
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
	   if (altchar==1) // raindrop scroll 108->107
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
		if (drop_sliding>8)  
		{  
			drop_sliding=0;			
			// altcharernate char
			if (altchar==0)		 
			   altchar=1;
			else
			   altchar=0;
			   // on incrémente le y de la boucle (après 8 drop_sliding d'octet vers le bas)
			for (rdindex=0;rdindex<RAINDROPMAX;rdindex++)
			{		
				if (raindropstate[rdindex]==1)
				{
					// clear tile over drop
				
					// drop fall on the floor or water 
					if (raindropy[rdindex]>=(floor_y-3-waterlevel))
					{
						raindropstate[rdindex]=0;
						poke(0xbb80+((raindropy[rdindex]-1)*40)+raindropx[rdindex],32);
						poke(0xbb80+(raindropy[rdindex]*40)+raindropx[rdindex],32);
					    poke(0xbb80+((raindropy[rdindex]+1)*40)+raindropx[rdindex],32);
						drop_floor_time=3;
						paper(5);
						ink(3);
						if (waterlevel<4)
							waterlevel++;
					}
					
					raindropy[rdindex]++;
				}
				else
					for(wait=0;wait<10;wait++);	
				
			} //end for (rdindex=0
		}  
   } // end of else of if (drop_sliding==0)
}
void display_outside()
{

	unsigned char i;
	for (i=1;i<28;i++)
	{
		if (i<7)
		{
			AdvancedPrint(2,i,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
			poke(0xBB80+1+(i*40),4);
		}
		if ((i>6)&&(i<24))
		{
			poke(0xBB80+1+(i*40),4);
			AdvancedPrint(2,i,"mm                               mmm");
			//poke(0xBB80+2+(i*40),4);
			//poke(0xBB80+3+(i*40),0);
			poke(0xBB80+33+(i*40),4);
			//poke(0xBB80+36+(i*40),4);			
		}	
	}	

}
void display_floor()
{
   char i;
	for (i=24;i<28;i++)
	{
		AdvancedPrint(2,i,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		poke(0xBB80+1+(i*40),5);
	}
	// floor
	
	asm("ldy #30;"
		"lda #127;"
	"suite_floor;"
		"sta $BB80+4+(24*40),y;"
		"dey;"				
		"bne suite_floor;"					
	);
	
	//poke(0xBB80+3+(7*40),1);
	//poke(0xBB80+3+(8*40),2);			
	
}
void display_ceiling()
{	// this routine take ceiling_y to draw it at the y=ceiling_y
	unsigned char yy;
	yy=ceiling_y;
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
		"ldy #30;"  // 30 char
		"lda #109;" // rain drop
	"suite_ceiling2;"
	"write_cei2;"
		"sta $1234,y;"
		"dey;"				
		"bne suite_ceiling2;");
	poke(0xBB80+2+((ceiling_y+1)*40),1); //red color for ceiling tile
	poke(0xBB80+2+((ceiling_y)*40),4); //red color for ceiling tile
}
	
display_left_wall()
{
	xwall=0x98+4;		
	asm("lda #$BC;"
		"sta write_wall+2;"
		"lda %xwall;" // +murx=x +4 en partant de la gauche
		"sta write_wall+1;"
		"ldy #18;"	// wall height			
	"suite_wall;"
		"clc;"
		"lda #126;"
		"ora #128;" // inverted color
		"write_wall;"
		"sta $1234;"
		"lda write_wall+1;"
		"adc #40;"
		"bcc saut_wall;"
			"inc write_wall+2;"
		"saut_wall;"	
		"sta write_wall+1;"
		"clc;"
		"dey;"				
		"bne suite_wall;"
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
					"lda #122;"  // put bric
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
/*
 if (peek(i)==32)
	       *(unsigned char*)i=122|128;
	   if (peek(i)==33)
	       *(unsigned char*)i=32|128;
*/	 



display_right_wall()
{
	xwall=0x98+35;		
	asm("lda #$BC;"
		"sta write_wall2+2;"
		"lda %xwall;" // +murx=x +4 en partant de la gauche
		"sta write_wall2+1;"
		"ldy #18;"				
	"suite_wall2;"
		"clc;"
		"lda #126;"
		"ora #128;"   // inverted color
		"write_wall2;"
		"sta $1234;"
		"lda write_wall2+1;"
		"adc #40;"
		"bcc saut_wall2;"
			"inc write_wall2+2;"
		"saut_wall2;"	
		"sta write_wall2+1;"
		"clc;"
		"dey;"				
		"bne suite_wall2;"
	);
}
 
void display_house()
{
	display_ceiling();
	display_left_wall();
	display_right_wall();  	
}			

void main_game_loop()
{
	
	poke(0x26A,10); // 11 curseur revient ?
	
	poke(0x24F,1);
	poke(0x24E,1);
	
	do 
	{
		//walking=0; // etat des jambes normal
	
		drop_sliding_outside();   
		
		// water flood level
		for (i=23;i>23-waterlevel;i--)
		{
		    poke(0xBB80+(40*i)+5,22);//left 
	        poke(0xBB80+(40*i)+36,23);//right
		}
		
		if (waterlevel==4)
		{
			// noyage animation
			endgame=1; // loose
		}
		// time to keep paper & ink flash
		if (drop_floor_time>0)
		{
			if (drop_floor_time==1)
			{
				paper(7);
				ink(4);
				display_floor();
			}
			drop_floor_time--;	
		}
		// time to keep paper & ink flash
		if (drop_catch_time>0)
		{
			if (drop_catch_time==1)
			{
				paper(7);
				ink(4);
				display_floor();
			}
			drop_catch_time--;	
		}
		// time to keep paper & ink flash
		if (shoot_cat_time>0)
		{
			if (shoot_cat_time==1)
			{
				paper(7);
				ink(4);
				display_floor();
			}
			shoot_cat_time--;	
		}
		// time to keep paper & ink flash
		if (shoot_tile_time>0)
		{
			if (shoot_tile_time==1)
			{
				paper(7);
				ink(4);
				display_floor();
			}
			shoot_tile_time--;	
		}
		
		music_tempo--;
		
	    if (active_music)
		{
			if (music_tempo==0)
			{
				play_music();
				music_tempo=music_tempo_delay;
			}
		}
		
		//  if (game_timer>10)
		//	{  
		//    wined(); // test la fin
		//	break;
		//	}
	
		manage_lightning();	
		manage_rain();

		
		if (game_timer>1)
		{
		   k=key();
		}
		
		if (walking==1)
			if (walking_alt==0)
			{
			   walking=0;
			   walking_alt=1;
			}
			else
			{
			   walking=1;
			   walking_alt=0;
			}
		//gotoxy(2,0);printf("IDX=%d CODE=%d TIME=%d  "  ,index_raindrop/2,codedrop,raindroptime);
        // saut à utiliser quand le cat passe
		
		disp_player(px,py);
		
		
		//if (game_timer>132)  //  cat appear
		{
	     	manage_cat(); //
		}
		// manage jumping timer
		if (jump_time>0) 
		{
		   jump_time--;
		   if (jump_time==1) // player Fallout
		   {
			  if (active_sound)			  
		         SoundEffect(player_fallout_sound);
			  
		   }
		   wait=0;
		}
		// space key pressed
		if (((k==32)&&(jump_time==0))||((k==11)&&(jump_time==0)))
		{
		    jump_time=jump_max_time; 
			wait=0;
        }
		
		// right key pressed
        if((k==9 || k=='P')&&(px<32)&&(jump_time==0))
		{
		    walking=1; // legs in walking mode
	     	px+=1;
			wait=0;
        }		
		// left key pressed
        if((k==  8 || k=='O')&&(px>7)&&(jump_time==0))
		{
		    walking=1; // legs in walking mode
			
			px+=-1;
			wait=0;
	    }
		// escape key pressed
		if (k==27)
		{		
			life=0;
		//    goto fin;
			endgame=1;
		}
		if (k==83) // touche 'S'
		{		
		    
			if (active_sound==1)
			{
				active_sound=0;
				gotoxy(15,0);printf("SOUND OFF");
			}
			else
			{
				active_sound=1;
				gotoxy(15,0);printf("SOUND ON");
			}
			for(i=0;i<15000;i++);				
			
			gotoxy(15,0);printf("         ");
			k=0;
		}
		if (k==59) // touche 'M'
		{		
			if (active_music==1)
			{
				active_music=0;
				gotoxy(15,0);printf("MUSIC OFF");
			}
			else
			{
				active_music=1;
				gotoxy(15,0);printf("MUSIC ON");
			}
			for(i=0;i<15000;i++);				
			
			gotoxy(15,0);printf("         ");
			k=0;
		}
		if (k==88) // Pause 'X'		
		{
		    for(i=0;i<5000;i++);
		/*
		    do
			{
			   k2=key();
			}
			while(k2!=87);
			*/
		}
		
		if(k==87)  // 'W' win game!
		{
		    
			
		}
		if(k==127)  // game pause
		{
		    do
		    {
		   
				k=key();
			}
			while(k!=0);
			
		}
		if(k==49)  // just for debugging
		{
		  endgame=2;
		}
		if(k==67)  // 'C' for debugging ceiling falling
		{
		    wave_nbr++;  // vague des gouttes suivantes
			
			if (ceiling_y<18) 
				ceiling_y++; // le plafond s'affesse d'un caractere

			//raindroptimer[rdindex]=wait_fall_raindrop; //

			if (active_sound==1)
			{ 
				explode();
				music_tempo=12;
			}
			display_ceiling(); // affichage du plafond

			if (ceiling_y==py) // si le plafond est au niveau de la tete du hero
			{	
				if (active_sound=1)
					explode();
				music_tempo=12;
				// fin partie
			}
			
			display_wave_level_timer=100; // durée d'affichage du numero de la vague suivante
		}
		if (k==0) // player do nothing
		{
			player_wait++;  // wait sans rien faire;
			walking=0;
		}
		else  // if activity keep eyes open
		{
			player_wait=0.;
		}
	    
		
	    if (player_wait>200)
		{
	    	player_wait=100;
		}
		
		// display life state :-/
		
		if (life==0)
		{
			poke(0xBF93,32);
			poke(0xBF95,32);
			poke(0xBF97,32);
			
		}			
		if (life==1)
		{
			poke(0xBF93,111 | 128);
			poke(0xBF95,32);
			poke(0xBF97,32);
		}
		if (life==2)
		{
			poke(0xBF93,111|128);
			poke(0xBF95,111|128);
			poke(0xBF97,32);
		}
		if (life==3)
		{
			poke(0xBF93,111|128);
			poke(0xBF95,111|128);
			poke(0xBF97,111|128);
		}					
	
		
		game_timer++; // compteur de temps dans le jeu utile pour plusieurs raisons, nottemment déclenchement vagues de gouttages
		
 		display_score();
	
	} while(endgame==0); 
    poke(0x24E,10);
}

void init_default_var()
{
	// initialisation des variables globales
	raindroptime=0; 			//delai d'wait entre entre 2 gouttes
	waterlevel=0; 				// niveau de l'eau au sol
	ceiling_y=6;
	endgame=0;					// flag to game end
	px=18;						// position X du joueur
	py=20;						// position Y du joueur
	score=0;					// score du joueur
	walking=0;					// indicateur de walking du joueur ( non static)
    walking_alt=0;				// to change legs char to walking
	wait=0;					    // pour besoin de pause dans le jeu
	player_wait=0;
	catx=0;  					// position X du cat
	caty=23;           			// position Y du cat
    cat=0; 						// pour savoir si le cat est prensent ou pas
	seecat=0;
	
    jump_time=0; 				// pas d'etat de saut par defaut . indicateur de saut du joueur, si en état de saut 
    wave_nbr=1;				    // numéro de vague de gouttes en cours . vague de goutte=niveau
    index_raindrop=0; 			// se positionner au début du tableau des positions et timing des gouttes
	drop_catch_time=0;
    shoot_cat_time=0;
    shoot_tile_time=0;
	game_timer=0;

	life=3;                     // nombre de life du personnage
    tile_fall=0;  				// tuile non active par défaut
	tile_x=0;					// x tile position
	tile_y=0;					// y tile position
	lightning_dice=0;			// random lightning appear
	lightning_time=0;			// lightning appear duration
    
	k=0;						// k = key(); contient la touche enfoncée
	game_timer=0;
	armstandup=0;
	music_index=0;
	music_tempo=music_tempo_delay;
	active_sound=0;
	active_music=0;
	rdindex=0;					// index initialize
	display_wave_level_timer=0;
	being_falling=0;
	altchar=1;					// used to alternate between 2 drop character
	drop_sliding=0;
	scroll_text=0;
	scroll_text_time=0;
	// redef drop
	redefine_raindrop();
	
	// raindrop array initialize
	for (i=0;i<RAINDROPMAX;i++)
	{
		raindropx[i]=0;
		raindropy[i]=0;
		//raindroptimer[i]=0;
		raindropstate[i]=0;
	}
}
// display menu/intro page
display_menu()
{
	paper(6);ink(4);

	// scrolling hybrid graphics mode : Text & Hires
	poke(0xbb80+40,30); 
	for (i=8;i<16;i++)
		poke(0xA000+(40*i)+2,26);

	// Scrolling Text colors
	*(unsigned char*)(0xA000+(40*8)+1)=1&7;
	*(unsigned char*)(0xA000+(40*9)+1)=5&7;
	*(unsigned char*)(0xA000+(40*10)+1)=3&7;
	*(unsigned char*)(0xA000+(40*11)+1)=7&7;
	*(unsigned char*)(0xA000+(40*12)+1)=3&7;
	*(unsigned char*)(0xA000+(40*13)+1)=5&7;
	*(unsigned char*)(0xA000+(40*14)+1)=1&7;
	*(unsigned char*)(0xA000+(40*15)+1)=0&7;
	
    AdvancedPrint(0,0,"                                        ");
	AdvancedPrint(2,1,"                                      ");
	// efface le fond
	for (i=16;i<28;i++)
	{
		AdvancedPrint(2,i,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
		poke(0xBB80+1+(i*40),4);
	}
	
	// screen of menu page
	AdvancedPrint(2,2,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2,3,"mmmmmm     mmmm    mmm mm mmmm mmmmmmm");
	AdvancedPrint(2,4,"mmmmm! !!!! mm !!!! m!mm!  mm! mmmmmmm");
	AdvancedPrint(2,5,"mmmmm! mmm! m! mmm! mm m! ! m! mmmmmmm");
	AdvancedPrint(2,6,"mmmmm!     mm!      m! m! m! ! mmmmmmm");
	AdvancedPrint(2,7,"mmmmm! !!! mm! !!!! m! m! mm!  mmmmmmm");
	AdvancedPrint(2,8,"mmmmm! mm!! m! mmm! m! m! mmm! mmmmmmm");
	AdvancedPrint(2,9,"mmmmm!mmmm!mm!mmmm!mm!mm!mmmm!mmmmmmmm");
	AdvancedPrint(2,10,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2,11, "mm     mmmm    mmm mmmm mm mmm    mmmm");
	AdvancedPrint(2,12,"m! !!!! mm !!!! m!  mm! m!mmm !!!! mmm");
	AdvancedPrint(2,13,"m! mmm! m! mmm! m! ! m! mm m! mmm!mmmm");
	AdvancedPrint(2,14,"m!     mm!      m! m! ! m! m! mmmmmmmm");
	AdvancedPrint(2,15,"m! !!!mmm! !!!! m! mm!  m! m! mmmm mmm");
	AdvancedPrint(2,16,"m! mmmmmm! mmm! m! mmm! m! mm!    mmmm");
	AdvancedPrint(2,17,"m!mmmmmmm!mmmm!mm!mmmm!mm!mmmm!!!mmmmm");
	AdvancedPrint(10,19,"PRESS SPACE  TO PLAY");
	AdvancedPrint(7,21,"DIRECTION : ARROWS KEY FOR");
	AdvancedPrint(4,23,"LEFT & RIGHT <- -> & SPACE TO JUMP");
    // text color   // 12 = blink
	
    //AdvancedPrint(6,25," S TO ACTIVATE SOUND - M MUSIC ");
	// change colors of title for inverted them
	/*
	for (i=0xBB80+80;i<0xBB80+1000;i++)
	{
	   if (i<0xBB70+740)
	   {
	   if (peek(i)==32)
	       *(unsigned char*)i=122|128;
	   if (peek(i)==33)
	       *(unsigned char*)i=32|128;
	   }
	   // text
       if ((peek(i)>32)&&(peek(i)<96))
	       *(unsigned char*)i=peek(i)|128;
	
	 }
    */
	 color_inverse_menu();
	 
	 poke(0xBB80+(19*40)+9,12);
	 poke(0xBB80+(19*40)+8,0);
	 poke(0xBB80+(19*40)+31,4);
	 poke(0xBB80+(19*40)+30,8);
	 
	 
	 poke(0xBB80+(21*40)+6,0);
	 poke(0xBB80+(21*40)+33,4);
	 
	 poke(0xBB80+(23*40)+3,0);
	 poke(0xBB80+(23*40)+38,4);
}
void main()
{
	unsigned char kmenu=00;
	unsigned int d1,d2; 
	redefine_char();
	
	sound_volume=7; // set volume to middle
	
    init_default_var();
	// cursor hide ?
    *(unsigned char*)0x26A=10;
   
	cls();
	
	display_outside(); 

	//display_house();
	//display_floor();
	display_menu();
	game_timer=0;
	//SoundEffect(rain_menu_sound,sound_volume);
	
	do 
	{
		game_timer++;
		scroll_text_time++;
		
		// SIMPLE TEXT SCROLLING
		if (scroll_text_time==80)
		{
			
			asm(
			"lda #40;"
			"sta $FFFF;"
			"ldy #0;" // 40 column
			"clc;"		
	      "suite_scl;"
			"iny;"
			"lda $BB82+40,y;"
			"dey;"
			"sta $BB82+40,y;"
			"iny;"
			"cpy #38;"
		    "bne suite_scl;");
	
			*(unsigned char*)(0xBB80+40+39)=credits_text[scroll_text];		
			scroll_text++;		
			if (scroll_text==sizeof(credits_text)-1)
				scroll_text=0;
				scroll_text_time=0;
		}
		if (game_timer>(75-(sound_volume*5)))
		{	       
			drop_sliding_outside();
			game_timer=0;
		}
		
			    		
	   // gotoxy(2,0);printf("ENDGAME=%d",endgame);
	    kmenu=key();
		   
		//drop_sliding++;
		//gotoxy(2,0);
	    //printf("KEY=%d ",kmenu);
		
		
		if (kmenu==83) // touche 'S'
		{		
		 if (sound_volume<15)
				sound_volume++;
			else
				sound_volume=0;
		     
			
		//	SoundEffect(rain_menu_sound,sound_volume);
		}
		// if press space key to start game
		if (kmenu==32)
		{
			
			SoundEffect(silence_sound);
			init_default_var();
			cls();
			gotoxy(0,0);printf("                                        ");
			paper(7);
			display_outside();
			display_house();
		
			display_floor();
		
			main_game_loop();
			if (endgame==1) // GAME OVER
			{ 
			
				gotoxy(16,12);printf( "GAME OVER ");
				gotoxy(13,14);printf(" YOUR SCORE :%d",score);
				for(wait=0;wait<40000;wait++);
				//poke(0xBB80+(13*40)+9,12);
				//poke(0xBB80+(13*40)+8,0);
				//poke(0xBB80+(15*40)+31,4);
				//poke(0xBB80+(15*40)+30,8);
	 
			}
			endgame=0;
			display_menu();
			kmenu=0;
			
		}
	}
	while(kmenu!=65);
	//fin:
	//cls();   
	return;
	//while(1)
	// restore ram from rom (0xFC78) charset, but bug stilling, good address ?
	j=CARADDR+(32*8);
	
	for (i=0xfc78;i<0xfea8;i++)
	{
		*(unsigned char*)j=peek(i);
		j++;
	}
	printf("PLEASE RESET YOUR ORIC");
	return;
}


