

extern unsigned int  wait,wait2;	     // global variable to manage wait time
extern char          walking;           // manage walking
extern char          px,py;             // ALDO position x & y
extern char unsigned caty;              // y cat position 
extern char          active_sound;      // activated sound state
extern int           music_tempo;       // music temporisation 
extern unsigned int  i;


// game win animation
void wingame()
{
	  int pos_scr;
	  int i,j,x,y;
	  unsigned char kk;
	  
	  // ici bruit de pluie qui s'arrete ?
	  j=0;  
	  //scanner tout l'écran afin de faire disparaitre les gouttes
	  for (pos_scr=0xBB80;pos_scr<0xBB80+1000;pos_scr++)
	  {
		  if (j++==39) 
		  {
			for (wait=0;wait<1000;wait++);
			j=0;
		  }
		
		  drop_sliding_outside();
		  // clear the rain
		  if ((peek(pos_scr)==109)||
			  (peek(pos_scr)==108)||
			  (peek(pos_scr)==107)||
			  (peek(pos_scr)==22))
			 poke(pos_scr,32);
	  }
	  
	  *(unsigned char*)(0Xbb80+(20*40)+4)=32;
	  *(unsigned char*)(0Xbb80+(21*40)+4)=32;
	  *(unsigned char*)(0Xbb80+(22*40)+4)=32;
	  *(unsigned char*)(0Xbb80+(23*40)+4)=32;
	  // Aldo exit the house
	  for (j=0;j<31;j++)
	  {
		for (wait=0;wait<1000;wait++);
		scroll_right(1,23);
		if (walking==0)
			walking=1;
		else
			walking=0;
		disp_aldo(px,py);
	  }
	  paper(3); //yellow
	  //  the cat come to Aldo
	  for (j=3;j<px-3;j++)
	  {
		for (wait=0;wait<1500;wait++);
		*(unsigned char*)(0Xbb80+(caty*40)+j-2)=32;
		*(unsigned char*)(0Xbb80+(caty*40)+j-1)=0;
		*(unsigned char*)(0Xbb80+(caty*40)+j)=114;
		*(unsigned char*)(0Xbb80+(caty*40)+j+1)=115;
	  }
	  *(unsigned char*)(0XBBAA+(40*(caty-2))+j-3)=1; // color of kernel
		
	  // clear the hires box 
	  for (i=40;i<4480+40;i++)
		 poke(0xA01B+i,64);   // 64 = correct
	 
	  // set the text mode in hires
	  for (i=40;i<4480+40;i+=40)
		 poke(0xA01B+i,27);

	  // set the hires mode in text
	  for (i=2;i<14;i++)
		poke(0xBB80+(i*40)+2,31);

	  // set the colors of inside box
	  for (i=2;i<14;i++)
	  {
		poke(0xBB80+(i*40)+1,7);  // white before hires blox
		poke(0xBB80+(i*40)+28,4); // blue before house
	  }  

	  poke(0x24E,10);
	  // set graphicals primitives avaible in text mode
	  poke(0x213,255);  
	  // draw the sun
	  for(j=1;j<32;j++)  
	  {
		curset(90,64,1);circle(j,1);
	  }
	  poke(0xBB80+608,12); // blink before text
	  poke(0xBB80+633,8);  // no blink
	  poke(0xBB80+80+608,12); // blink before text
	  poke(0xBB80+80+633,8);  // no blink
	  AdvancedPrint(10,15,"CONGRATULATION ALDO !");
	  AdvancedPrint(5,17,"YOU R SURVIVOR OF THIS STORM !");
	  
	  // here play a victory music   ??
	  
	  j=0;

	  for (wait=0;wait<200;wait++)
	  {
		  // make sun shining
		  if (j==10) 
		  {
			 *(unsigned char*)(0XBBAA+(40*(caty-2))+px-5)=39;
			curset(90,64,1);circle(34,1);
			curset(90,64,1);circle(38,1);
			curset(90,64,1);circle(48,1);    
		  }
		  if (j++==20) 
		  {
			*(unsigned char*)(0XBBAA+(40*(caty-2))+px-5)=32;
			curset(90,64,1);circle(48,0);
			curset(90,64,1);circle(38,0);
			curset(90,64,1);circle(34,0);

			j=0;
		  }
		  kk=key();
		  if (kk==27)
			 break;
	  }
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

// player falling by collission with cat or tile
void player_falling()  
{
   
   int time;
   //colcat=0; // disable cat collision
   //colfireball=0; // disable fireball collision
   if (active_sound==1)
   {
	  shoot();
	  music_tempo=3;  
   }
	   
   for (i=0;i<5;i++)
   {
         //manage_cat();
       disp_aldo(px,py+i);
	   //plot(px,py-1+i,32);
	   //plot(px+1,py-1+i,32);
	   //plot(px-1,py-1+i,32);
	   for (time=0;time<2000;time++);
	   
	  // must manage bottom screen repaint
	
	}
    display_floor();   // bug dans cette methode voir si pbl de clc ou registre a sauver sur pile ? 
}


// house shaking bc ceiling go down
void house_shaking()  
{
    int j;
	for (j=0;j<4;j++)
	{
		scroll_right(1,23);
		for (wait=0;wait<500;wait++);
		scroll_left(1,23);
		for (wait=0;wait<500;wait++);
	}    // scroll_left()	
}



void unshoot()
{
    for (wait=0;wait<30;wait++)
	{
		// faire tourner des étoile !!
		disp_aldo(px,py);
	    for(wait2=0;wait2<4;wait2++)
		ink(1);
		drop_sliding_outside();
		ink(4);
	}
}
