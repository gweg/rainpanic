
extern char          ceiling_y;         // hauteur du plafond = 8 fixe
extern unsigned int  i;
extern int           score;

	
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
void display_house()
{
	display_ceiling();
}			
void display_floor()
{
    char i;
	for (i=24;i<28;i++)
		AdvancedPrint(2,i,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		//poke(0xBB80+1+(i*40),5);
	
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
	// clear background
	for (i=16;i<28;i++)
	{
		AdvancedPrint(2,i,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
		poke(0xBB80+1+(i*40),4);
	}
	
	// screen of menu page
	AdvancedPrint(2,2,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2,3,"mmmmmmm     mmmm    mmm mm mmmm mmmmmm");
	AdvancedPrint(2,4,"mmmmmm! !!!! mm !!!! m!mm!  mm! mmmmmm");
	AdvancedPrint(2,5,"mmmmmm! mmm! m! mmm! mm m! ! m! mmmmmm");
	AdvancedPrint(2,6,"mmmmmm!     mm!      m! m! m! ! mmmmmm");
	AdvancedPrint(2,7,"mmmmmm! !!! mm! !!!! m! m! mm!  mmmmmm");
	AdvancedPrint(2,8,"mmmmmm! mm!! m! mmm! m! m! mmm! mmmmmm");
	AdvancedPrint(2,9,"mmmmmm!mmmm!mm!mmmm!mm!mm!mmmm!mmmmmmm");
	AdvancedPrint(2,10,"mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
	AdvancedPrint(2,11, "mmm     mmmm    mmm mmmm mm mmm    mmm");
	AdvancedPrint(2,12,"mm! !!!! mm !!!! m!  mm! m!mmm !!!! mm");
	AdvancedPrint(2,13,"mm! mmm! m! mmm! m! ! m! mm m! mmm!mmm");
	AdvancedPrint(2,14,"mm!     mm!      m! m! ! m! m! mmmmmmm");
	AdvancedPrint(2,15,"mm! !!!mmm! !!!! m! mm!  m! m! mmmm mm");
	AdvancedPrint(2,16,"mm! mmmmmm! mmm! m! mmm! m! mm!    mmm");
	AdvancedPrint(2,17,"mm!mmmmmmm!mmmm!mm!mmmm!mm!mmmm!!!mmmm");
	AdvancedPrint(6,21,"\23\4DIRECTION : ARROWS KEY < > \4\26");
	AdvancedPrint(13,23,"\23\4SPACE TO JUMP\4\26");
	AdvancedPrint(10,25,"\23\4PRESS SPACE TO PLAY\4\26");
    // text color   // 12 = blink

	color_inverse_menu();
	
	poke(0xBB80+(25*40)+9,12);
	poke(0xBB80+(25*40)+31,8);
	 
	 
	// to debbug
	// gotoxy(20,0);printf("CODE=%d IDX=%d",rain[index_raindrop],index_raindrop);
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

void display_score()
{
   sprintf(0xBB80+(40*26)+20,"\23\4DROPS \27\1%d\27\4",score);   	
  
}
