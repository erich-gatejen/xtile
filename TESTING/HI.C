/*

 HI demo for the XTile Graphics manager

 COPYRIGHT (C) 1992  Erich P Gatejen   All Rights Reserved


 File: HI.C    	Written for the Turbo C compiler


 This demo shows several of the features of XTile.
 The file HI.DAT must be in the same directory as hi.exe.


*/


#include<stdio.h>
#include<alloc.h>
#include<dos.h>
#include"xtile.h"


unsigned char	*Screen;		/* Input buffer for the background screen */
unsigned char	*Hello;		/* Input buffer for 'HIMOM' message	  */
unsigned char  *Mask;         /* Input buffer for 'HIMOM' mask		  */
unsigned char  *Palette;		/* Input buffer for palette data		  */


int  Init() {


	FILE	*in;

	/* Load data from "hi.dat" file into buffers	*/
	if ((in = fopen ( "hi.dat", "rb" )) == NULL) return(1);

	Screen  = farmalloc( 64000 );
	Hello   = farmalloc( 4144  );
	Mask    = farmalloc( 1036  );
	Palette = farmalloc( 768   );

	if (( Screen  == NULL) || ( Hello == NULL) || ( Mask == NULL ) ||
	    ( Palette == NULL) ) return(1);


	fread ( Screen,  sizeof( unsigned char ), 64000, in );
	fread ( Hello,   sizeof( unsigned char ), 4144,  in );
	fread ( Palette, sizeof( unsigned char ), 768,   in );
	fread ( Mask,    sizeof( unsigned char ), 1036,  in );

	fclose ( in );


	/* Set Mode X, load palette, and clear the screen			*/
	Init_Mode_X();

	Set_Pal_Color ( 218, 0, 0, 0 );

	MSD_Clear( 218 );

	Set_Pal_Block	( 0, 256, Palette );
	Set_Pal_Color  ( 218, 0, 0, 0    );

	/* Upload the 'HIMOM								*/
	MD_Upload_Tile( STORE, 4144, Hello );

	/* Load screen to PAGE1 and the BACKGROUND				*/
	Set_Display_Page( PAGE0 );
	Set_Write_Page( BACKGROUND );
	MS_Put_Tile( 0,40, 320, 200, Screen );
	Set_Write_Page( PAGE1 );
	MS_Put_Tile( 0,40, 320, 200, Screen );

     return(0);


}; /* end Init */


void DoIt() {

	int	Red, Green, Blue;

	unsigned int	Step, PageF;

	Red = 0;
	Green = 0;

     PageF = 1;       /* PageF is current displayed screen */

	/* Fade in back color */
	for ( Blue = 0; Blue < 64; Blue++ ) {
		Set_Pal_Color( 218, Red, Green, Blue );
		Red++;
		Green++;
		if( Red > 52   ) Red = 52;
		if( Green > 31 ) Green = 31;
	};


	/* Scroll to PAGE1								*/
	Screen_Scroll ( FORWARD, 120, 2 );

	Set_Write_Page( PAGE0 );
	MS_Put_Tile( 0,40, 320, 200, Screen );

	delay( 2000 );

	SD_Paste_Tile_M ( 86, 106, 148, 28, STORE, Mask );


	/* Put 'HIMOM' on screen						*/
	for ( Step = 1; Step < 14; Step++ ) {


		SD_BKG_Restore( 86, 106, 148, ( 14 - Step ));        /* Top */
		SD_BKG_Restore( 86, (120 + Step), 148,  16 - Step ); /* Bottom */

		if ( PageF == 0 ) {
			Set_Write_Page( PAGE0 );
			Set_Display_Page( PAGE1 );
			PageF = 1;
		} else {
			Set_Write_Page( PAGE1 );
			Set_Display_Page( PAGE0 );
			PageF = 0;
		};

		delay( 2 );

		SD_Paste_Tile_M ( 86, 106, 148, 28, STORE, Mask );
	};

	if ( PageF == 0 ) {
		Set_Write_Page( PAGE0 );
		Set_Display_Page( PAGE1 );
		PageF = 1;
	} else {
		Set_Write_Page( PAGE1 );
		Set_Display_Page( PAGE0 );
		PageF = 0;
	};


	delay( 2100 );

	/* Remove 'HIMOM' from screen						*/
	for ( Step = 1; Step < 14; Step++ ) {


		SD_BKG_Restore( 86, 106, 148, Step );        /* Top */
		SD_BKG_Restore( 86, (134 - Step), 148, Step ); /* Bottom */

		if ( PageF == 0 ) {
			Set_Write_Page( PAGE0 );
			Set_Display_Page( PAGE1 );
			PageF = 1;
		} else {
			Set_Write_Page( PAGE1 );
			Set_Display_Page( PAGE0 );
			PageF = 0;
		};

		delay( 4 );

	};

	SD_BKG_Restore( 86, 106, 148, 28 );        /* All */

	if ( PageF == 0 ) {
		Set_Write_Page( PAGE0 );
		Set_Display_Page( PAGE1 );
		PageF = 1;
	} else {
		Set_Write_Page( PAGE1 );
		Set_Display_Page( PAGE0 );
		PageF = 0;
	};
	SD_BKG_Restore( 86, 106, 148, 28 );        /* All */

	if ( PageF == 0 ) {
		Set_Write_Page( PAGE0 );
		Set_Display_Page( PAGE1 );
		PageF = 1;
	} else {
		Set_Write_Page( PAGE1 );
		Set_Display_Page( PAGE0 );
		PageF = 0;
	};

	Set_Display_Page( PAGE1 );
	Set_Write_Page( PAGE0 );

	MS_Set_Box ( 0,0, 320, 240, 218 );

	delay( 2500 );

	Screen_Scroll ( BACK, 120, 2 );

	Red   = 52;
	Green = 31;
	/* Fade in back color */
	for ( Blue = 63; Blue >= 0; Blue-- ) {
		Set_Pal_Color( 218, Red, Green, Blue );
		Red--;
		Green--;
		if( Red < 0   ) Red   = 0;
		if( Green < 0 ) Green = 0;
	};

	delay( 500 );


}; /* end DoIt */


void Done() {

	union REGS	in, out;

	/* Return screen to text mode				*/
	in.x.ax = 3;
	int86( 16, &in, &out );


}; /* end Done */


void main () {

	int 	Status;

	Status = Init();
	if ( Status == 0 ) DoIt();
	Done();


};
