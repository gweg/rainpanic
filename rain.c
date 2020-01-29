#include "const.h"
#include "sounds.h"
#include "levels.h"

extern unsigned char rdindex; 
extern int           raindroptime;      // to read time drop => time between two drops
extern char          altchar;			 // to alternate char for the falling rain drop
extern unsigned char raindropx[];
extern unsigned char raindropy[];
extern char          ceiling_y;         // hauteur du plafond = 8 fixe
extern unsigned char codedrop;          // to read drop x position value in Rain[] array.

extern int           index_raindrop;    // index to read from the rain[] array
extern char unsigned tile_x;            // position x of the beam
extern char unsigned tile_y;            // position y of the beam
extern char unsigned lightning;         // if lightning actived

extern unsigned char obj_kernel;
extern unsigned char obj_ice;
extern unsigned char obj_x;
extern unsigned char obj_y;
extern char          seecat;
extern char unsigned seefireball;
extern unsigned int  wait,wait2;	     // global variable to manage wait time
extern int           music_tempo;       // music temporisation 
extern int           score;		     // game score
extern char          active_sound;      // activated sound state
extern char unsigned wave_nbr;
extern char          endgame;           // loose game = 1 winned =2
extern char          px,py;             // ALDO position x & y
extern unsigned char benddown;		 // time to avoid the fireball
extern unsigned      display_wave_level_timer; // used to display the wave level
extern char 		 drop_catch_time;   // paper 6 during this time to indicate catch is successfull
extern char          drop_floor_time;   //
extern char          waterlevel;		 // level of water
extern unsigned char drop_sliding;		 // y step of vertically scrolling for the rain drop

extern unsigned char raindropstate[];

//
//   WAVES OF RAINDROP DATA 
//
// RAIN[] -> raindrop array [XPOS(drop), DELAY(between 2 drop),,,,]
// rain drop must be bewteen 6 and 32 x position (7 left---19 center -- right 31) with step of 2 )
// 255 = next wave
// 250 = cat appear on the left (cardir)
// 251 = cat appear on the right
// 252 = fireball appear on the left
// 253 = fireball appear on the right
// 200-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)
// 180-196 = Life fall 
// 150-176 = ice fall
// 0x08 or 0x09 must be in hexa don't know why. Its look lika a bug ?!
unsigned char rain[] = {   //----------------------------------------------|new wave|
// wave 1  time=3/2      x position (7 left---19 center -- right 31)
  253,3, 250,0, 19,2, 252,0, 32,2, 7,2, 253,0,	
  32,3, 19,3, 07,3,  32,3,  251,0 ,7,3, 16,3, 18,3, 20,3,
  07,3  ,9,3  ,250,0 ,26,3  ,28,3, 251,0,  32,3, 7,3, 250,0, 32,3, 
  15,3  ,17,3, 251,0 ,19,3  ,21,3, 250,0, 23,3, 
  07,3 ,9,3 ,11,3 ,251,0 ,28,3  ,32,3,  250,0, 07,3, 251,0 ,32,3, 22,3,
  251,0, 21,3,  252,0, 7,3, 250,0, 32,2, 251,0, 21,6,  // 39eme
  255,0, // wave 2 time =2
  7,3, 32,2, 32,2, 250,0, 7,2,
  13,3 ,30,2, 251,0, 07,3, 215,0, 32,3, 250,0 ,18,3, 250,0, 07,3, 32,2, 217,0, // 
  07,2, 251,0, 32,3,   250,0, 07,3, 251,0, 32,3,  250,0, 07,3, 207,0, 
  31,2, 17,3, 217,0, 32,2, 224,0, 7,1, 19,6,   
  255,0,  // wave 3 
  14,2, 251,0 ,25,2,  215,0, 250,0, 32,2, 07,2, 251,0, 32,2, // 73eme
  14,2,250,0, 16,2, 251,0, 18,2, 250,0, 20,2,217,0,14,2,218,0,32,2,
  07,2, 32,2, 251,0, 9,3, 220,0, 168,3, // ice here  reprendre ici 
  32,3, 07,3, 215,0, 31,3, 215,0, 07,3, 216,0, 30,3, 
  255,0, // wave 4
  250,0, 0x08,3,212,0, 13,2,  
  29,3, 251,0, 0x09,3,  230,0, 28,3, 251,0, 10,3, 251,0, 13,4, 230,0, 30,4,  16,4, 208,0, 
  07,4, 250,0, 32,4,   18,4, 07,4, 10,4, 251,0, 15,4, 11,4, 24,4, 
  255,0,  // wave 5 
  19,3,
  9,1, 11,1, 251,0, 13,1, 15,1, 250,0, 17,1,19,1, 
  251,0, 21,1, 23,1, 250,0, 25,1,27,4,
  250,0, 15,0, 251,0, 17,0 ,19,0, 250,0, 21,0,23,0, 25,3,
 
  7,0, 250,0, 9,0, 11,0, 250,0, 13,0 ,15,0, 17,3,251,0, 
  21,0, 23,0, 251,0, 25,0, 27,0, 29,0, 31,3, 
  7,0, 250,0, 9,0, 11,0, 251,0, 13,0 ,15,3,
  21,0, 23,0, 25,0, 27,0, 29,0, 31,3,
  7,0,9,0,11,1, 14,0,16,0,18,1, 21,0,23,0,25,1, 27,0,29,0,31,1, 
  19,2, 19,5,
  // one line
  //  [x,y] y =0 -> force to align drops in same line
 };

// raindrop managment
//

void manage_rain()
{

//	if (raindroptime>0)
//	   raindroptime--; // espace de temps entre les gouttes: temps en therme de cycle
	//gotoxy(5,5);printf("TIME=%d  ",raindroptime);
	for (rdindex=0;rdindex<RAINDROPMAX;rdindex++)
	{	
		// activate a new drop
		// here we find a free place in the raindrop to create a new drop . array if raindroptime=0
		if (raindropstate[rdindex]==0 && raindroptime<1)
		{
			raindropstate[rdindex]=1; // cellule de la goutte devient occupée
			
			if (altchar==0)
				raindropy[rdindex]=ceiling_y+1;
			else
				raindropy[rdindex]=ceiling_y+2;		

			// 1rst code read from the rain array
			codedrop=rain[index_raindrop];			
			// initialiser la position x de la nouvelle goute depuis le tableau rain[]
			raindropx[rdindex]=codedrop;
			if (codedrop<150)
				poke(0XBB80+(40*(ceiling_y+1))+codedrop,122);
				
			// delai entre 2 gouttes
			//décompte lors du déplacement de la goutte
			// à lire dans le tableau des vagues
			// second code read from the rain array	
			// raindroptime = wait delay to read next rain drop
			raindroptime=rain[index_raindrop+1];
			
			// specials code :		
			if (codedrop>149)
			{
				raindropstate[rdindex]=0;
				// lightning
				if (codedrop>199&&codedrop<227) 
				{
					tile_x=codedrop-200;
				    // get codedrop for X position of lightening
					lightning=1;
				}
				// kernel (life) 180-196 
				if (codedrop>179&&codedrop<197)
				{
					obj_x=codedrop-180;
				    obj_y=ceiling_y-3;
					// get codedrop for X position of lightening
					obj_kernel=1;	
				}
				// 150-226 = lightning appear and use this code to define x position of lighting (Xpox = code-200 + 7)
				// ice 150-176
				if (codedrop>149&&codedrop<178)
				{
					obj_x=codedrop-150;
					obj_y=ceiling_y-3;
				    // get codedrop for X position of lightening
					obj_ice=1;
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
				if (codedrop==252)
				{
					if (seefireball==0)
						seefireball=1;
				}
				if (codedrop==253)
				{
					if (seefireball==0)
						seefireball=2;
				}
				
				// new wave of drops, ceiling fall one step
				if (codedrop==255)  // si la position de x=255 c'est une nouvelle vague de gouttes
				{
				

					if (wave_nbr>0)
					{
						ceiling_y++; // le plafond s'affesse d'une ligne
						house_shaking();
						if (active_sound==1)
						{ 
							explode();
							music_tempo=12;
						}
						display_ceiling(); // affichage du plafond
					}					
					wave_nbr++;  // vague des gouttes suivantes
					//raindroptimer[rdindex]=wait_fall_raindrop; //	
					if (ceiling_y==py) // si le plafond est au niveau de la tete du hero
					{	
						if (active_sound=1)
						{
							explode();
							music_tempo=12;
						}
						// fin partie
						endgame=1;
					}	
					display_wave_level_timer=30; // durée d'affichage du numero de la vague suivante
				}
			}
		//	raindroptimer[rdindex]=wait_fall_raindrop; // wait_fall_raindrop
			// increment the raindrop array index
			if (index_raindrop<sizeof(rain))
				index_raindrop+=2;
			else
			   endgame=2;  // end of game Win 

		} //if (raindropstate[rdindex]==0 && raindroptime<1)					
		if (endgame>0) break;
	} // end for (rdindex=0	
	
	if (endgame>0) 
		return;
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
			   ((py-2+benddown<raindropy[rdindex])&&(py+1+benddown>raindropy[rdindex])))
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
					music_tempo=2;							
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
	   // reserve time to time compentation with the previous loop
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
			{
				altchar=1;
			    if (raindroptime>0)
					raindroptime--; // espace de temps entre les gouttes: temps en therme de cycle			  
			}
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
