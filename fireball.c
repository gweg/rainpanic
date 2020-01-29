#include "sounds.h"
#include "anims.h"

extern char unsigned fireball;          // pour savoir si le fireball est prensent ou pas
extern char unsigned seefireball;
extern char unsigned fireballx;         // x fireball position 
extern char unsigned firebally;         // y fireball position 
extern char unsigned fireball;          // pour savoir si le fireball est prensent ou pas
extern char unsigned dirfireball;       // direction du fireball
extern char unsigned fireballwait;      // delai du fireball qui reste sur place avant de partir
extern char          active_sound;      // activated sound state
extern int           music_tempo;       // music temporisation 
extern char unsigned lightning;         // if lightning actived
extern char unsigned tile_x;            // position x of the beam
extern unsigned char benddown;		 // time to avoid the fireball
extern char          px,py;             // ALDO position x & y
extern char unsigned colfireballtime;   // duration of collision of Aldo with fireball enabled
extern unsigned char legsup;  			 //   legs set up
extern unsigned char armsdown;  		 // arms set down
extern char          shoot_fireball_time;
extern char          life; 			 // player lifes
extern unsigned int  i,j;               // usefull in many time in this program  


void manage_fireball()
{
	#ifdef DEBUG
	gotoxy(30,0);printf("WAT=%d",waterlevel);	
	#endif
	
	if (fireball==0) 
	{
		j=0;		 
		 if (seefireball==1) //Apparait à gauche
		 { 
		 	 //tile_x=20;
		     //lightning_time=12; // temps de durée de l'éclair  
		   
   		     fireballx=3;
			 fireball=1;
			 dirfireball=0;
			 fireballwait=10;
			 if (active_sound)
			 {
				explode();				
				music_tempo=6;
			 }
			 lightning=1;
			 tile_x=20;
		 }
		 if (seefireball==2)// apparait à droite
		 {
			
		     fireballx=34;
			 fireball=1;
			 dirfireball=1;
			 fireballwait=10;
			  if (active_sound)
			 {
				explode();				
				music_tempo=6;
			 }
			 lightning=1;
			 tile_x=20;
		 }
	}
	else // fireball ==1 , gérer la présence du fireball et son déplacement
	{
		*(unsigned char*) (0Xbb80+(firebally*40)+fireballx)=120;
		*(unsigned char*) (0Xbb80+(firebally*40)+fireballx+1)=121;
		*(unsigned char*) (0Xbb80+(firebally*40)+fireballx-1)=3;
		*(unsigned char*)(0Xbb80+(firebally*40)+fireballx+2)=32;
		
	     if (fireballwait>0) 
		     fireballwait--;
		
	     if (dirfireball==0) // de gauche à droite
		 {  
		    if (fireballwait==0)
			{
		       fireballx++;
			}
			if (fireballx==7)  // restore wall
			{
     			*(unsigned char*)(0Xbb80+(firebally*40)+1)=4;
			    *(unsigned char*)(0Xbb80+(firebally*40)+2)=109;
		     	*(unsigned char*)(0Xbb80+(firebally*40)+3)=109;
		    	*(unsigned char*)(0Xbb80+(firebally*40)+4)=126|128;
			}
			if (fireballx==37)  //arrived at position 36
			{
			  fireball=0;
			  seefireball=0;
			  // restore wall
			  *(unsigned char*)(0Xbb80+(firebally*40)+fireballx-3)=4;
		      *(unsigned char*)(0Xbb80+(firebally*40)+fireballx-2)=126|128;
			   
			  *(unsigned char*)(0Xbb80+(firebally*40)+fireballx-1)=109;
			  *(unsigned char*)(0Xbb80+(firebally*40)+fireballx)=109;
			}
		 } 
		 if (dirfireball==1) // de droite à gauche
		 {
		    if (fireballwait==0)
		       fireballx--;
			if (fireballx==31)   
 			{
			    // restore right zone
			    *(unsigned char*)(0Xbb80+(firebally*40)+34)=4;
				//*(unsigned char*)(0Xbb80+(firebally*40)+35)=126|128;
			    *(unsigned char*)(0Xbb80+(firebally*40)+36)=109;
		     	*(unsigned char*)(0Xbb80+(firebally*40)+37)=109;
			}
			if (fireballx==2)  //fireball arrived at position 1
			{
				seefireball=0;
				fireball=0;  
				// restore left zone
				*(unsigned char*)(0Xbb80+(firebally*40)+fireballx)=109;
		     	*(unsigned char*)(0Xbb80+(firebally*40)+fireballx+1)=109;
		    	//*(unsigned char*)(0Xbb80+(firebally*40)+fireballx+2)=126|128;
			}
		 }
        // handle fireball collision with Aldo
	    if (((fireballx==px)||(fireballx+1==px))&&(benddown==0)&&(colfireballtime==0))
		{
		   
		   //  faire plutot un benddown_time et pas benddown
		   
		   if (active_sound==1)
		   { 	
				SoundEffect(cat_collision_sound);				
				music_tempo=3;
   	       }
		   paper(3);
		   ink(7);		  
		 
		   benddown=1;
		   legsup=1;
		   gotoxy(px-1,py);printf("   ");
		   
		   armsdown=0;
		   shoot_fireball_time=4;
		 
		   life--;
		   colfireballtime=27;
		   unshoot();
		}
		if (colfireballtime>0)
			colfireballtime--;
    }
}
