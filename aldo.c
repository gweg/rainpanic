#include "const.h"

extern unsigned char benddown;		 // time to avoid the fireball
extern unsigned char armsdown;  		 // arms set down
extern char unsigned player_wait;       // close eyes afeter no activity
extern unsigned char jump_time;       // gestion du saut du héro 
extern unsigned char legsup;  			 //   legs set up
extern char          walking;           // manage walking
extern char          px;
extern char          endgame;           // loose game = 1 winned =2
extern unsigned char being_falling;

void disp_aldo(char x,char y)
{
	unsigned char aldoy;
	aldoy=y;
	if (benddown==1)
		aldoy=y+1;

	// arms up
    if (armsdown==0)
	{
		// sceau
		*(unsigned char*)(0xbb80+(aldoy*40)+x-1)=91;
		*(unsigned char*)(0xbb80+(aldoy*40)+x)=92;
		*(unsigned char*)(0xbb80+(aldoy*40)+x+1)=93;
	// blancs
		*(unsigned char*)(0xbb80+(aldoy*40)+x-2)=5;
		//poke (0xbb80+(aldoy*40)+x-2,5);
		*(unsigned char*)(0xbb80+(aldoy*40)+x+2)=0;
		
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x-1)=94;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x+1)=96;
	// blancs
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x-2)=1;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x+2)=0;
	}
	else  // arms down Aldo Win
	{
		*(unsigned char*)(0xbb80+((aldoy)*40)+x-1)=0;
		*(unsigned char*)(0xbb80+((aldoy)*40)+x+1)=0;
	    *(unsigned char*)(0xbb80+(aldoy*40)+x-2)=5;
		
		*(unsigned char*)(0xbb80+(aldoy*40)+x)=34;
		*(unsigned char*)(0xbb80+(aldoy*40)+x-2)=5;
		*(unsigned char*)(0xbb80+(aldoy*40)+x+2)=0;
	    *(unsigned char*)(0xbb80+((aldoy+1)*40)+x-1)=35;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x)=95;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x+1)=36;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x-2)=1;
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x+2)=0;
	}
	
    // head smile after 100 loop

	if (player_wait>64)  
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x)=106;
	else  // close eyes after 100 main loop cycle
	{	
		*(unsigned char*)(0xbb80+((aldoy+1)*40)+x)=95;	
	}

    if (armsdown==0)
	{
		if (jump_time<2&&benddown==0)
		{
		// ventre en mode non saut	
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x-1)=1;
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x)=98;
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+1)=32;		
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x-2)=1;	
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+2)=0;
		}
		else // ventre en mode saut
		{
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x)=98;
		}
	}
	else // arms up
	{
	    *(unsigned char*)(0xbb80+((aldoy+2)*40)+x-2)=1;
	    *(unsigned char*)(0xbb80+((aldoy+2)*40)+x-1)=37;
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+1)=38;
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x)=98;
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+2)=0;
	}
    // blancs
//	poke (0xbb80+((y+2)*40)+x-2,0);
//	poke (0xbb80+((y+2)*40)+x+2,0);
    // blancs pour effacer les jambes du saut
//	poke (0xbb80+((y+2)*40)+x-3,0);
//	poke (0xbb80+((y+2)*40)+x+3,0);
	
    // getion du saut pour mettre les jambes en l'air
	// legs up
	if (legsup==1)
	{
	    // mettre les jambes sur les cotés 2xcar de chaque coté
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x-1)=117;
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x-2)=116;
        if (px<6)  // redessine le mur à gauche
	    {
	        *(unsigned char*)(0xbb80+((aldoy+2)*40)+x-3)=126;
	    }
	    else
	    {
	       *(unsigned char*)(0xbb80+((aldoy+2)*40)+x-3)=2;
		}
		//poke (0xbb80+((aldoy+2)*40)+x,127);
		*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+1)=118;   
     	*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+2)=119; 
		
		if (px<32)  // redessine le mur à gauche
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+3)=0;
		
		
		if (jump_time==jump_max_time&&benddown==0) 
		{
			// blancs à la place des jambes du bas
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x-1)=32;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x)=32;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x+1)=32;
		}
	}
	else // manage legs on ground 
	{
		if (walking==1)  // walking mode
		{
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x-1)=100;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x)=101;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x+1)=102;
		}
		else
		{
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x-1)=103;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x)=104;
			*(unsigned char*)(0xbb80+((aldoy+3)*40)+x+1)=105;
		}
		if ((px>7)&&(endgame==0)) // redraw wall on the left
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x-3)=32;
	
		
		if ((px>30)&&(endgame==0)) // redraw wall on the right
		{
				*(unsigned char*)(0xbb80+((aldoy+3)*40)+34)=4;
				*(unsigned char*)(0xbb80+((aldoy+3)*40)+35)=126|128; 
		 		*(unsigned char*)(0xbb80+((aldoy+2)*40)+34)=4;
			  	*(unsigned char*)(0xbb80+((aldoy+2)*40)+35)=126|128; 
				*(unsigned char*)(0xbb80+((aldoy+1)*40)+34)=4;
				*(unsigned char*)(0xbb80+((aldoy+1)*40)+35)=126|128; 
				*(unsigned char*)(0xbb80+((aldoy)*40)+34)=4;
				*(unsigned char*)(0xbb80+((aldoy)*40)+35)=126|128; 
		}
		else
		{
			*(unsigned char*)(0xbb80+((aldoy+2)*40)+x+3)=32; 
		}
	}
	if (benddown==0)
	{
		// blank char next the legs
		*(unsigned char*)(0xbb80+((aldoy+3)*40)+x-2)=2;
		*(unsigned char*)(0xbb80+((aldoy+3)*40)+x+2)=0;
	}
	
    if (being_falling==1)
	{
		*(unsigned char*)(0xbb80+((aldoy-1)*40)+x-1)=32;
		*(unsigned char*)(0xbb80+((aldoy-1)*40)+x)=32;
        *(unsigned char*)(0xbb80+((aldoy-1)*40)+x+1)=32;
	}
	
}