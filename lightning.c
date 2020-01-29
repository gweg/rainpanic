
#include "const.h"

extern char unsigned tile_x;            // position x of the beam
extern char unsigned tile_y;            // position y of the beam
extern char unsigned tile_fall;         // if tile fall
extern char unsigned lightning;         // if lightning actived
extern char          ceiling_y;         // hauteur du plafond = 8 fixe
extern int  unsigned lightning_time;	 // duration of lightning
extern char          active_sound;      // activated sound state
extern int           music_tempo;       // music temporisation 
extern unsigned char ly;                // used to display lightning
extern char          px,py;             // ALDO position x & y
extern char          shoot_tile_time;   //
extern unsigned char legsup;  			 //   legs set up

extern unsigned char benddown;		 // time to avoid the fireball
extern char          life; 			 // player lifes


// manage lightning 
// tile_x = lightning x
//
void manage_lightning()
{ 
	
    if (tile_fall==0)
    {
		if (lightning==1)
		{
		   // afficher foudre
		   tile_fall=1;
		
		   tile_y=ceiling_y-3;
		   
		   lightning_time=12; // temps de durée de l'éclair  
		   paper(0);
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
		    // display the lightning
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
            // clear ligthning		
			for (ly=0;ly<ceiling_y-1;ly++)
			{
			    // efface éclair avec gouttes
				if (ly<def_ceiling_pos)
				{
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=109;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=109;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=109;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x+2)=109;	
				}
				else
				{
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=32;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=32;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=32;
					*(unsigned char*)(0XBBAA+(40*ly)+tile_x+2)=32;	
				}
			}
			if (ly<def_ceiling_pos)
				*(unsigned char*)(0XBBAA+((40*ly+2))+tile_x-3)=109;
			else
				*(unsigned char*)(0XBBAA+((40*ly+2))+tile_x-3)=32;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x-1)=109;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x)=109;
			//*(unsigned char*)(0XBBAA+(40*ly)+tile_x+1)=109;
		    lightning_time=0;
			paper(7);
		}
	 } 
	 if (tile_fall==1)  
	 {
	   // roof tile fall on the player
		if ((px-2==tile_x )&&((tile_y>=py-1)&&(tile_y<=py+3)))
		{
			poke(0XBBAA+(40*tile_y)+tile_x,32);		
			tile_fall=0; // 	    
			shoot_tile_time=3; // 4 main program cycles for show color indacte correct catching
			lightning=0;
			lightning_time=0;
			paper(3);
			if (active_sound==1)
			{ 
				shoot();
				music_tempo=7;
			}
			benddown=1;
		    legsup=1;
			// clear head zone
		    gotoxy(px-1,py);printf("   ");
		   
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
		}
		else
		{
			lightning=0;
		    // display falling tile
			tile_fall=0;
			poke(0XBBAA+(40*tile_y)+tile_x,32);	  
		}
	}
}
