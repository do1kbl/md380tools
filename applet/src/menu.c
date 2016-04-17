/*! \file menu.c
  \brief Menu hooks and extensions.
*/

#include <stdio.h>
#include <string.h>

#include "printf.h"
#include "dmesg.h"
#include "md380.h"
#include "version.h"
#include "config.h"
#include "os.h"


/* This hooks a function that is called a lot during menu processing.
   Its exact purpose is unknown, but I'm working on that.
 */
void *main_menu_hook(void *menu){
  void *menustruct;
  
//  printf("main_menu() ");
//  printhex(menu,32);
//  printf("\n");
  
  
  switch(* ((int*)menu)){
  case 0x0b:
    //printf("Exiting menu.\n");
    break;
  case 0x24:
    //Third word of the parameter is a structure with
    //more entries.
    menustruct=*((int*)menu + 2);
    printf("Menu struct: @0x%08x\n",
	   menustruct);
    printf("Item %5d/%5d selected. %s\n",
	   (int) *((unsigned short*) (menustruct+0x42)),
	   (int) *((unsigned short*)menustruct),
	   "test");
    
    
    
    //printhex(*((int*) menu+2),128);
    //printf("\n");
    
    /*

Main menu:
Menu struct: @0x20001398
06000000 Total Entries
02000000 Selected Page Index
04000000 4a000000 00000000 91000000 8c0f0d08 00000000 3280ff00
1414ff00 c0c0c000 c0c0c000 00000000 ffffff00 ffffff00 80808000 00000500
                                                                   \--/
                                                                Selected item
00000000 4a001600 00436f6e 74616374 73006361 27001600 00536361 6e001600
                    \--Contacts begins here.
0a000c00 27001600 005a6f6e 65006c20 0a000c00 0b006573 49001600

Contacts Menu, last Entry:
Menu struct: @0x20001390
e4020000 Total Entries
e0020000 Selected Page Index
         04000000 9c000000 00000000 91000000 8c0f0d08 00000000 3280ff00
1414ff00 c0c0c000 c0c0c000 00000000 ffffff00 ffffff00 80808000 0000e302
                                                                   \--/
                                                                Selected item
00000000 7d001600 00547269 2d537461 74652028 4c322900 41001600 004c6f63
                    \--First contact entry starts here.
616c2039 00436f6e 09000b00 3b001600 00444d52 204e4100 09000b00
     */
    break;
  default:
    //do nothing
    break;
  }
  return main_menu(menu);
}


static void do_jump(uint32_t entrypoint) {
  asm volatile(
               "bx     %0      \n"
                : : "r" (entrypoint) : );
  // just to keep noreturn happy
  for (;;) ;
}
                                                                        

void Create_Menu_Utilies_hook(void) {

  uint8_t * program_radio_unprohibited = (uint8_t *) (0x2001d030 + 4);
  // 0x08012740      dff8f80a       ldr.w r0, [pc, 0xaf8]       ; [0x801323c:4]=0x2001d030
  // 0x08012744      0079           ldrb r0, [r0, 4]   
  // 0x08012746      c0f38000       ubfx r0, r0, 2, 1 
  // 0x0801274a      c007           lsls r0, r0, 0x1f
  // 0x0801274c      02d4           bmi 0x8012754               ;[2] ; Is Menu ProgramRadio allowedly  
  if ( (* program_radio_unprohibited & 0x4) == 0x4 ) {
    printf("program_radio_unprohibited ");  
  } else {
    printf("program_radio_prohibited ");
  }
}  
  

void Create_Menu_Utilies_hook_end(void) {
  do_jump(0x08012786+1);
}



void F_249_Create_MenuEntry_hook(int a, void * b , void * c, void  * d, int e, int f ,int g) {
  printf("0x%x Text: 0x%x GreenKey 0x%x RedKey 0x%x 0x%x 0x%x 0x%x\n", a,b,c,d,e,f,g);
  printf("b: ");
  printhex2(b,10);
  printf("\n");
//  printf("%x %\n", * (unsigned int*) 0x20019df0);
  F_249_Create_MenuEntry(a,b,c,d,e,f,g);
}
