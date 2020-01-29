#include "sounds.h"
#include "anims.h"

unsigned char catfish[]={
00,48,11,05,31,39,11,16, // 112 left catfish
16,49,59,63,62,63,59,33, // 113 left catfish (queue)
02,35,55,63,31,63,55,33, // 114 right fish
00,03,52,40,62,57,52,02  // 115 right fish
};
unsigned char catcat[]={
04,28,31,15,03,03,02,06, // 112 right cat  
01,01,62,60,60,12,38,42, // 113 right cat
16,16,15,07,07,06,12,10, // 114 left cat
04,07,63,62,56,24,40,44 // 115 left cat}
};

extern char unsigned cat;               // pour savoir si le cat est prensent ou pas
extern unsigned int  i,j;
extern char          waterlevel;		 // level of water
extern char          seecat;            // if cat is visible  
extern char unsigned catx;              // x cat position 
extern char unsigned caty;              // y cat position 
extern char          dircat;            // direction du cat
extern char unsigned catwait;           // delai du cat qui reste sur place avant de partir
extern char          active_sound;      // activated sound state

extern int           music_tempo;       // music temporisation 
extern unsigned char jump_time;       // gestion du saut du héro 
extern unsigned char colcattime;	     // duration of collision of Aldo with the cat enabled
extern char          px,py;             // ALDO position x & y
extern unsigned char being_falling;
extern unsigned char armsdown;  		 // arms set down
extern unsigned char benddown;		 // time to avoid the fireball
extern unsigned char legsup;  			 //   legs set up
extern char          life; 			 // player lifes
extern char 		  colcat;
extern char 		  shoot_cat_time;    // paper cyan during this time to indicate shoot with the cat

void manage_cat()
{
	#ifdef DEBUG
	gotoxy(30,0);printf("WA LVL=%d",waterlevel);	
	#endif
	
	if (cat==0) 
	{
		j=0;
		
		if (waterlevel>0)
		{  // redefine chars for catfish apparence
		    for (i=46976;i<46976+32;i++)
			*(unsigned char*)i=catfish[j++];
		
		}
		else
		{  // redefine chars for cat apparence
			for (i=46976;i<46976+32;i++)
			*(unsigned char*)i=catcat[j++];
		}
		 
		if (seecat==1) //appear on left
		{ 
   		     catx=3;
			 cat=1;
			 dircat=0;
			 catwait=10;
			 if (active_sound)
			{
				SoundEffect(cat_at_door_sound);				
				music_tempo=2;
			}
		}
		if (seecat==2)// appear on right
		{
			
		    catx=34;
			cat=1;
			dircat=1;
			catwait=10;
			if (active_sound)
			{
				SoundEffect(cat_at_door_sound);				
				music_tempo=2;
			}
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
			if (catx==7)  // restore left zone
			{
     			*(unsigned char*)(0Xbb80+(caty*40)+1)=4;
			    *(unsigned char*)(0Xbb80+(caty*40)+2)=109;
		     	*(unsigned char*)(0Xbb80+(caty*40)+3)=109;
		    	//*(unsigned char*)(0Xbb80+(caty*40)+4)=126|128;
			}
			if (catx==37)  // resore right zone
			{
			   // effacement du cat par la pluie et mur
			  cat=0;
			 
			  seecat=0;
			  *(unsigned char*)(0Xbb80+(caty*40)+catx-3)=4;
		      //*(unsigned char*)(0Xbb80+(caty*40)+catx-2)=126|128;
			   
			  *(unsigned char*)(0Xbb80+(caty*40)+catx-1)=109;
			  *(unsigned char*)(0Xbb80+(caty*40)+catx)=109;
			}
		 } 
		 if (dircat==1) // de droite à gauche
		 {
		    if (catwait==0)
			{
		       catx--;
			}
			if (catx==31)   
 			{
			    // restore right wall
			    *(unsigned char*)(0Xbb80+(caty*40)+34)=4;
				*(unsigned char*)(0Xbb80+(caty*40)+35)=126|128;
			    *(unsigned char*)(0Xbb80+(caty*40)+36)=109;
		     	*(unsigned char*)(0Xbb80+(caty*40)+37)=109;
			}
			if (catx==2)  //arrivé du cat à la position 1
			{
				seecat=0;
			 // effacement du cat par la pluie et mur
				cat=0;  
				// restore left wall
				*(unsigned char*)(0Xbb80+(caty*40)+catx)=109;
		     	*(unsigned char*)(0Xbb80+(caty*40)+catx+1)=109;
		    	*(unsigned char*)(0Xbb80+(caty*40)+catx+2)=126|128;
		
			}
		 }
        // handle cat collision with Aldo
	    if (((catx==px)||(catx+1==px))&&(jump_time<3)&&(colcattime==0))
		{
		
		   if (active_sound==1)
		   { 	
				SoundEffect(cat_collision_sound);				
				music_tempo=3;
   	       }
		   if (dircat==1)
				catx-=2;
		   if (dircat==2)
				catx+=3;
		   paper(3);
		   ink(7);		  
		   being_falling=1;
		   armsdown=1;
		   
		   //player_falling();
		   benddown=1;
		   legsup=1;
		   gotoxy(px-1,py);printf("   ");
			
		   armsdown=0;
		   being_falling=0,
		   life--;
		   colcat=1; //enable cat collision
		   shoot_cat_time=3; // keepn 3 cycle time tp show cyan color paper indicate correct catching
		   colcattime=27;
		   unshoot();
		}
		if (colcattime>0)
		colcattime--;
    }
}
