//  RAIN PANIC   alpha version
//  GOYO 2019 (C)
//
//  HIMEM on peut descendre à #600 (basic à #400)
//  HI user ram  #9800
// reste à implémenter:
// -------------------
//
// -les différents niveaux -> toutes les vagues de pluie . actuellement que la 1ere vague
//
// -gestion de l'actication du son et de la musique
// 
// - jambes qui marchent durant le déplacement continue
//
// - collision avec ice : faire tomber Aldo
//
// BUGS :
// ------
//  
//   chat shape
//
//   legs traces
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

#include "const.h"
#include "sounds.h"
#include "music.h"
#include "lightning.h"


#undef DEBUG              // to switch to debug mode !!! : define/undef

unsigned char raindropx[RAINDROPMAX];
unsigned char raindropy[RAINDROPMAX];
//unsigned char raindroptimer[RAINDROPMAX]="";
unsigned char raindropstate[RAINDROPMAX]="";

int           index_raindrop;    // index to read from the rain[] array

unsigned char codedrop;          // to read drop x position value in Rain[] array.
int           raindroptime;      // to read time drop => time between two drops

unsigned int  timer1,timer2;

char 		  drop_catch_time;   // paper 6 during this time to indicate catch is successfull
char 		  shoot_cat_time;    // paper cyan during this time to indicate shoot with the cat
char          shoot_tile_time;   //
char 		  shoot_fireball_time;
char          drop_floor_time;   //
char          endgame;           // loose game = 1 winned =2
unsigned int  scroll_text;
unsigned char scroll_text_time;
char          px,py;             // ALDO position x & y
unsigned char armsdown;  		 // arms set down
unsigned char legsup;  			 //   legs set up
char          waterlevel;		 // level of water
int           score;		     // game score
int           music_index;       // music in for music array indexing
int           music_tempo;       // music temporisation 
unsigned int  wait,wait2;	     // global variable to manage wait time
char          active_sound;      // activated sound state
char          active_music;      // activated music state
char          volume;		     // game sound volume
char          walking;           // manage walking
char 		  walking_alt;       // alternate legs chars to walking

unsigned char jump_time=0;       // gestion du saut du héro 
unsigned char benddown=0;		 // time to avoid the fireball
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
unsigned char k,lastk;           // to read the key pressed  : k=key(); 	

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
unsigned int  i,j;               // usefull in many time in this program  
		
unsigned      display_wave_level_timer; // used to display the wave level


unsigned char lx;                // used to display lightning
unsigned char ly;                // used to display lightning


unsigned char being_falling;
// other falling object
unsigned char obj_kernel;
unsigned char obj_ice;
unsigned char obj_x;
unsigned char obj_y;


unsigned char scroll_y;
unsigned char scroll_y_nbr;
unsigned char scroll_timer;

unsigned char read; //to read string from keyboard;

void AdvancedPrint(char x_pos,char y_pos,const char *ptr_message);

char credits_text[]="             RAIN PANIC                "
					"           INSTRUCTIONS :  DURING A STRONG THUNDERSTORM HELP MR ALDO TO CATCH ALL THE DROPS OF WATER FALLING FROM HIS HOUSE CEILING "
					"           BE CARREFULL OF ROOF TILE FALLING         "
					"    USE LEFT AND RIGHT ARROWS TO MOVE ALDO (OR O,P)"
					"    USE SPACE TO JUMP        ESC TO EXIT   "
					"                                                    "
					"  CODING BY GOYO IN C LANGUAGE WITH ORIC OSDK AND FEW INLINE ASSEMBLY CODE         ORIGINAL MUSIC : LADYWASKY      "
					"  THANKS TO THE HELP OF MANY MEMBERS OF COE FORUM : DBUG  RETORIC"
					"  LADYWASKY  DRPSY  KENNETH  ISS AND OTHERS ORICIANS ..           "
					"  HAVE FUN .....                     ";
				




 

// others object falling : ice or kernel
void manage_falling_obj()
{	
	if ((obj_kernel!=0)||(obj_ice!=0))
	{  
		if (obj_y<floor_y-3)
		{
			obj_y++;		 
			if (obj_y>ceiling_y+1)
            {          		
				*(unsigned char*)(0XBBAA+(40*(obj_y-1))+obj_x)=32;
				if (obj_kernel==1)
				{
					*(unsigned char*)(0XBBAA+(40*obj_y)+obj_x)=39;
					*(unsigned char*)(0XBBAA+(40*obj_y)+obj_x-1)=1;
				}
				if (obj_ice==1)
				{
					*(unsigned char*)(0XBBAA+(40*obj_y)+obj_x)=99;
					*(unsigned char*)(0XBBAA+(40*obj_y)+obj_x-1)=6;
				}
				*(unsigned char*)(0XBBAA+(40*obj_y)+obj_x+1)=0;   
			}		  
		}
		else
		{
			poke(0XBBAA+(40*obj_y)+obj_x,32);		
			if (obj_kernel==1)
				obj_kernel=0;
		
			if (obj_ice==1)
				obj_ice=0;
		}
		// fall on Aldo body
		if ((px-2==obj_x )&&((obj_y>=py-1)&&(obj_y<=py+3)))
		{
			if (obj_kernel==1)
			{
				if (active_sound==1)
				{ 
			       ping();
				   music_tempo=12;
				}
				obj_kernel=0;
				life++;
			}
			if (obj_ice==1)
			{
				if (active_sound==1)
				{ 
			       explode();
				   music_tempo=12;
				}
				obj_ice=0;
				being_falling=1;
			    player_falling();
			    being_falling=0;
				life--;
			}	
		   
		}		
	}
}

// scroll right 
// min y pos =1
void scroll_right(char y,char nbr)
{     
	    scroll_y_nbr=nbr;
		scroll_y=y;
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

void scroll_left(char y,char nbr)
{
        scroll_y_nbr=nbr;
		scroll_y=y;
	
		
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








void display_ceiling()
{	// this routine take ceiling_y to draw it at the y=ceiling_y
	unsigned char yy;
	yy=ceiling_y;

	//poke(0xBB80+2+((ceiling_y+1)*40),1); //red color for ceiling tile
	//poke(0xBB80+36+((ceiling_y+1)*40),4); //blue color for ceiling tile
	
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
	if (ceiling_y>6)
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
		poke(0xBB80+5+((ceiling_y)*40),22); // white blue color
		poke(0xBB80+36+((ceiling_y)*40),23); //white color
	}

	poke(0xBB80+5+((ceiling_y+1)*40),1|128); //red color for ceiling tile
	poke(0xBB80+34+((ceiling_y+1)*40),4|128); //blue color for ceiling tile
}

void main_game_loop()
{
	
	poke(0x26A,10); // 11 curseur revient ?
	
	poke(0x24F,1);
	poke(0x24E,1);
	
	do 
	{
		#ifdef DEBUG
		timer1=deek(0x276);
		#endif
	
		//poke(782,64); disabled Interruptions Keyboard Wait Timer 
        
		drop_sliding_outside();
		disp_aldo(px,py);
		// display the cat
		manage_lightning();
		manage_falling_obj();				
		manage_cat(); 
		manage_fireball(); 
		manage_rain();
		display_right_wall();
		display_left_wall();
		// display Aldo
				
		//poke(782,192);
	
	
		// water flood level
		for (i=23;i>23-waterlevel;i--)
		{
		    poke(0xBB80+(40*i)+5,22);//left 
	        poke(0xBB80+(40*i)+36,23);//right
		}
		
		if (waterlevel==4)
		{
			// noyade animation
			endgame=1; // loose
		}
		// time to keep paper & ink flash
		if (drop_floor_time>0)
		{
			if (drop_floor_time==1)
			{
				paper(7);
				ink(4);
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

			}
			shoot_tile_time--;	
		}
		if (shoot_fireball_time>0)
		{
			if (shoot_fireball_time==1)
			{
				paper(7);
				ink(4);
			}
			shoot_fireball_time--;	
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


		if (game_timer>1)
		   k=key();
		  
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
		// gotoxy(2,0);printf("IDX=%d CODE=%d TIME=%d  "  ,index_raindrop/2,codedrop,raindroptime);
		//sprintf(0xBB80+2,"IDX=%d"  ,index_raindrop/2);

		
		// manage jumping timer
		if (jump_time>0) 
		{
		   jump_time--;
		   if (jump_time==1) // player Fallout
		   {
			  if (active_sound)			  
		         SoundEffect(player_fallout_sound);
			
			  legsup=0;
		   }
		   wait=0;
		}
		if (standuptime>0)
		standuptime--;
		//gotoxy(2,25);printf("lastk=%d",lastk);
		// <space> key pressed
		if (jump_time==0)
		{	
			// KEY UP OR <SPACE>
			if ((k==32)||(k==11)) // keys 'space" or <up arrow>
			{
				if (benddown==1) // stand up from ground
				{
					standuptime=4;  // 4 game loop cycle
					
					legsup=0;
					benddown=0;	
					jump_time=0;
					wait=0; // cancel the pause
					// have to clear key buffer
					//poke(0x24E,10);
				}
				else		// else jump
				{
					if (standuptime==0)
					{
						jump_time=jump_max_time; 
						wait=0;
						if (active_sound)
						{				
							SoundEffect(player_jump_sound);
							music_tempo=2;
						}
						legsup=1;
					}
				}
			}
        }
		// # KEY DOWN : Adlo benddown     key = <down arrow>
		if ((k==10)&&(benddown==0)&&(jump_time==0))
		{	
			benddown=1;
			legsup=1;
			// faire uniqement benddown_time=1 au lieu de zero c'est tout
			// pour que Aldo reste au sol tant qu'il ne faut pas de jump ou fleche du haut
			
			// clear head zone
			gotoxy(px-1,py);printf("   ");
		}
	
		//if (benddown_time>0)
		//	benddown_time--;	
		#ifdef DEBUG
		if  (benddown==1)
		{
			gotoxy(25,0);printf("STOOP");
		}
		else
		{
			gotoxy(25,0);printf("     ");
		}
		#endif
	
		// RIGHT key pressed   'P' or <right arrow>
        if((k==9 || k=='P')&&(px<32)&&(jump_time==0))
		{
			if (benddown==1)
			{
				benddown=0;
				legsup=0;
				// clear left foot
				*(unsigned char*)(0xbb80+((py+3)*40)+px-2)=32;
			}
		    walking=1; // legs in walking mode
	     	px+=1;
			wait=0;
        }		
		// LEFT key pressed 'O' or <left arrow>
        if((k==  8 || k=='O')&&(px>7)&&(jump_time==0))
		{
		    if (benddown==1)
			{
				benddown=0;
				legsup=0;
				// clear right foot
				*(unsigned char*)(0xbb80+((py+3)*40)+px+2)=32;
			}
			
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
		if (k==76) //'l' throw a lightning
		{
			tile_x=22;
			lightning=1;
		}
		if (k==83&&lastk!=83) // touche 'S'
		{			    
			if (active_sound==1)
			{
				active_sound=0;
				gotoxy(15,0);printf("SOUND OFF");
				SoundEffect(silence_sound);
			}
			else
			{
				active_sound=1;
				gotoxy(15,0);printf("SOUND ON");
			}
			for(i=0;i<15000;i++);				
			gotoxy(15,0);printf("         ");
		}
		if (k==59&&lastk!=59) // touche 'M'
		{		
			if (active_music==1)
			{
				active_music=0;
				gotoxy(15,0);printf("MUSIC OFF");
			    SoundEffect(silence_sound);
			}
			else
			{
				
				active_music=1;
				gotoxy(15,0);printf("MUSIC ON");
				music_tempo=0;
			}
			for(i=0;i<15000;i++);				
			gotoxy(15,0);printf("         ");
			
		}

		#ifdef DEBUG
		if(k==68)  // 'D' direct to method for debugging
		{
		    endgame=2;
			armsdown=1;
			SoundEffect(silence_sound);
		    wingame();   
			k=0;	
			return;
		}
		#endif
		if(k==127)  // <back space> game pause
		{
		    SoundEffect(silence_sound);
		    sprintf(0xBB80+(40*14)+18,"PAUSE");
		    do
		    {
				k=key();
				drop_sliding_outside();   
				for(wait=0;wait<600;wait++);
			}
			while(k==127||k==0);
			k=0;
			sprintf(0xBB80+(40*14)+18,"       ");
		}
		
		#ifdef DEBUG
		if(k==49)  // do end game
		{
		  endgame=2;
		}
		if(k==67)  // 'C' for debugging ceiling falling
		{
		    wave_nbr++;  // vague des gouttes suivantes
			house_shaking();
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
			
			display_wave_level_timer=30; // durée d'affichage du numero de la vague suivante
		}
		#endif
		if (k==0) // player do nothing
		{
			player_wait++;  // wait sans rien faire;
			walking=0;
		}
		else  // if activity keep eyes open
		{
			player_wait=0.;
		}

		#ifdef DEBUG
		if (k==78) // 'N' next wave only to devlopper
		{
		
			
			drop_sliding==0;
			while ((rain[index_raindrop]!=255)&&(index_raindrop<sizeof(rain)-1))
			{
			
				index_raindrop+=2;
			}
			
			k=0;
		}
		#endif
		if (endgame==2)  // win the game
 		{
			SoundEffect(silence_sound);
			music_tempo=7;			
			armsdown=1;
		    wingame();   
			k=0;			
		}

	    if (player_wait>200)
	    	player_wait=100;
		
		// display life state :-/
		
	
		if (life>=1)
			poke(0xBF93,111 | 128);
		else
			poke(0xBF93,97);
		if (life>=2)
			poke(0xBF95,111|128);
		else
			poke(0xBF95,97);
		if (life>=3)
			poke(0xBF97,111|128);
		else
			poke(0xBF97,97);		
		if (life>=4)
			poke(0xBF99,111|128);
		else
			poke(0xBF99,97);
	
		if (life==0)  // loose Game
			endgame=1;
		
		
			
		// wave display
		if (display_wave_level_timer>0)
		{
			sprintf(0xBB80+(40*14)+16,"WAVE %d",wave_nbr);
		    if (display_wave_level_timer==1)
			{
				sprintf(0xBB80+(40*14)+16,"       ");
			}
		    display_wave_level_timer--;
		}
	
			
		game_timer++; // compteur de temps dans le jeu utile pour plusieurs raisons, nottemment déclenchement vagues de gouttages
 		display_score();
		lastk=k;  // save last key pressed
		#ifdef DEBUG
		timer2=deek(0x276);
		sprintf(0xBB80+10,"DELAY:%d  ",(timer1-timer2));
		#endif
	} while(endgame==0);
	SoundEffect(silence_sound);	
    poke(0x24E,10);	
}

void init_default_var()
{
	// initialisation des variables globales
	raindroptime=0; 			//delai d'wait entre entre 2 gouttes
	waterlevel=0; 				// niveau de l'eau au sol
	ceiling_y=def_ceiling_pos;	// default ceiling position
	endgame=0;					// flag to game end
	px=18;						// position X du joueur
	py=20;						// position Y du joueur
	armsdown=0;					// arms down =1
	legsup=0;					// to jump & benddown
	score=0;					// score du joueur
	walking=0;					// indicateur de walking du joueur ( non static)
    walking_alt=0;				// to change legs char to walking
	wait=0;					    // pour besoin de pause dans le jeu
	player_wait=0;
	catx=0;  					// position X du cat
	caty=23;           			// position Y du cat
    cat=0; 						// pour savoir si le cat est prensent ou pas
	seecat=0;
	
	fireballx=0;                // x fireball position 
	firebally=20;               // y fireball position 
	fireball=0;                 // pour savoir si le fireball est prensent ou pas
	dirfireball=0;              // direction du fireball
	fireballwait=0;             // delai du fireball qui reste sur place avant de partir
	colcattime=0;
	
	seefireball=0;
    colfireballtime=0;
	
	jump_time=0; 				// pas d'etat de saut par defaut . indicateur de saut du joueur, si en état de saut 
	benddown=0;
	standuptime=0;
	
    index_raindrop=0*2; 	    // ****** must be pair. se positionner au début du tableau des positions et timing des gouttes
	drop_catch_time=0;          //  
    shoot_cat_time=0;
	shoot_fireball_time=0;
	colcat=1;					// enable cat collision
    shoot_tile_time=0;
	game_timer=0;

	life=4;                    // nombre de life du personnage normal : 3
    tile_fall=0;  				// tuile non active par défaut
	tile_x=0;					// x tile position
	tile_y=0;					// y tile position
	obj_kernel=0;
    obj_ice=0;
	obj_y=0;
	lightning=0;	      		//        lightning appear
	lightning_time=0;			// lightning appear duration
    
	k=0;						// k = key(); contient la touche enfoncée
	game_timer=0;

	music_index=0;
	music_tempo=music_tempo_delay;
	volume=7;

	active_sound=1;
	active_music=1;
	rdindex=0;					// index initialize
	display_wave_level_timer=0; // used to display the wave level
	being_falling=0;
	altchar=1;					// used to alternate between 2 drop character
	drop_sliding=0;
	scroll_text=0;
	scroll_text_time=0;

    wave_nbr=0;				    // numéro de vague de gouttes en cours . vague de goutte=niveau

	// redef drop
	redefine_raindrop();
	
	// clear raindrop array 
	for (i=0;i<RAINDROPMAX;i++)
	{
		raindropx[i]=0;
		raindropy[i]=0;
		//raindroptimer[i]=0;
		raindropstate[i]=0;
	}
	
	// *********** for developpment -> goto define wave nbr 
	//while ((rain[index_raindrop+2]!=255)&&(index_raindrop<sizeof(rain)-1))
	//	index_raindrop+=2;
	
}
void main()
{
	unsigned char kmenu=00;
	unsigned int d1,d2;
	int timer,raintimer;
	redefine_char();
	redefine_charExt();
	volume=7; // set volume to middle
	
    init_default_var();
	// cursor hide ?
    *(unsigned char*)0x26A=10;
   
	cls();
	
	display_outside(); 

	//display_house();
	//display_floor();
	display_menu();
	
	game_timer=0;
	//SoundEffect(rain_menu_sound,volume);
	
	do 
	{
		
		scroll_text_time++;
		
		timer++;
		raintimer++;
		if (timer==400)
		{
			scroll_left(1,8);
			scroll_right(10,8);
		}
		if (timer==800)
		{
			scroll_right(1,8);
			scroll_left(10,8);
			timer=0;
		}
	
		// SIMPLE TEXT SCROLLING
		if (scroll_text_time==192)
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
	
			*(unsigned char*)(0xBB80+40+39)=credits_text[scroll_text];		
			scroll_text++;		
			if (scroll_text==sizeof(credits_text)-1)
			{
				scroll_text=0;
				scroll_text_time=0;
			}
			scroll_text_time=0;
		}
			       	
		if (raintimer==70)
		{
			drop_sliding_outside();
			raintimer=0;
		}	
			    		
	   // gotoxy(2,0);printf("ENDGAME=%d",endgame);
	    kmenu=key();
		   
		//drop_sliding++;
		#ifdef DEBUG
		if (kmenu!=0)
		{
			gotoxy(30,0);
			printf("KEY=%d ",kmenu);
		}
		#endif
		if (kmenu==83) // touche 'S'
		{		
		 if (volume<15)
				volume++;
			else
				volume=0;
		     
			
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
			    // do here GAME OVER MUSIC
				gotoxy(16,14);printf( "GAME OVER ");
				//gotoxy(16,14);printf(" SCORE:%d",score);
				for(wait=0;wait<40000;wait++);
				//poke(0xBB80+(13*40)+9,12);
				//poke(0xBB80+(13*40)+8,0);
				//poke(0xBB80+(15*40)+31,4);
				//poke(0xBB80+(15*40)+30,8);
	 
			}
			endgame=0;
			cls();
			display_menu();
			kmenu=0;
			
		}
	}
	while(kmenu!=65);
	//fin:
	//cls();   
	return;  // unreachable code here 
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


