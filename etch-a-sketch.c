#include "address_map_arm.h"
#include "defines.h"
#include <stdio.h>

volatile int pixel_buffer_start; // global variable, to draw 
void clear_screen ();
void disable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_GIC(void);
void config_KEYs(void);
void enable_A9_interrupts(void);
void pushbutton_ISR(void);
void config_interrupt(int, int);
void plot_pixel(int x, int y, short int line_color);
void drawBackground();
void drawWelcome();
void drawStar();
void drawLotus();
void drawSpiral();
void drawFace();
void drawHelp();
int x=160;
int y=120;
short int colour = 0x000000;//black


int main(void) {
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	
	drawWelcome();
	
	disable_A9_interrupts(); // disable interrupts in the A9 processor
	set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
	config_GIC(); // configure the general interrupt controller
	config_KEYs(); // configure pushbutton KEYs to generate interrupts
	enable_A9_interrupts(); // enable interrupts in the A9 processor
	while (1); // wait for an interrupt

}
void drawHelp(){
	extern short Help [240][320];
    volatile short * pix = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pix + (j<<0) + (i<<9)) = Help[i][j];  
    // while (1);
    return 0;
	
	
}
void drawFace(){
	extern short Face [240][320];
    volatile short * pix = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pix + (j<<0) + (i<<9)) = Face[i][j];  
    // while (1);
    return 0;
	
}
void drawSpiral(){
	extern short Spiral [240][320];
    volatile short * pix = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pix + (j<<0) + (i<<9)) = Spiral[i][j];  
    // while (1);
    return 0;


}
void drawLotus(){
	extern short Lotus [240][320];
    volatile short * pix = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pix + (j<<0) + (i<<9)) = Lotus[i][j];  
    // while (1);
    return 0;
	
	
}
void drawStar() {
	extern short Star [240][320];
    volatile short * pix = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pix + (j<<0) + (i<<9)) = Star[i][j];
   
    // while (1);
    return 0;
}

void drawBackground(){
	extern short Background [240][320];
    volatile short * pixelbuf = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pixelbuf + (j<<0) + (i<<9)) = Background[i][j];
   
	//while (1);
    return 0;
	
}

void drawWelcome(){
	extern short Welcome [240][320];
    volatile short * pixel = 0xc8000000;
    int i, j;
    for (i=0; i<240; i++)
        for (j=0; j<320; j++)
        *(pixel + (j<<0) + (i<<9)) = Welcome[i][j];
   
    // while (1);
    return 0;
}


/* setup the KEY interrupts in the FPGA */
void config_KEYs() {
volatile int * PS2_ptr = (int *) PS2_BASE; // pushbutton KEY base address
*(PS2_ptr + 1) = 0x00000001; // enable interrupts for the two KEYs
}

// Define the IRQ exception handler
void __attribute__((interrupt)) __cs3_isr_irq(void) {
// Read the ICCIAR from the CPU Interface in the GIC
int interrupt_ID = *((int *)0xFFFEC10C);
if (interrupt_ID == 79) // check if interrupt is from the KEYs
pushbutton_ISR();
else
while (1); // if unexpected, then stay here
// Write to the End of Interrupt Register (ICCEOIR)
*((int *)0xFFFEC110) = interrupt_ID;
}
// Define the remaining exception handlers
void __attribute__((interrupt)) __cs3_reset(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_undef(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_swi(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_pabort(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_dabort(void) {
while (1);
}
void __attribute__((interrupt)) __cs3_isr_fiq(void) {
while (1);
}
// Turn off interrupts in the ARM processor
void disable_A9_interrupts(void) {
int status = 0b11010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

//Initialize the banked stack pointer register for IRQ mode
void set_A9_IRQ_stack(void) {
int stack, mode;
stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
/* change processor to IRQ mode with interrupts disabled */
mode = 0b11010010;
asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
/* set banked stack pointer */
asm("mov sp, %[ps]" : : [ps] "r"(stack));
/* go back to SVC mode before executing subroutine return! */
mode = 0b11010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}
/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void) {
int status = 0b01010011;
asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}
/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void) {
	config_interrupt (79, 1); // configure the FPGA KEYs interrupt (73)
	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all
	// priorities
	*((int *) 0xFFFEC104) = 0xFFFF;
	// Set CPU Interface Control Register (ICCICR). Enable signaling of
	// interrupts
	*((int *) 0xFFFEC100) = 1;
	// Configure the Distributor Control Register (ICDDCR) to send pending
	// interrupts to CPUs
	*((int *) 0xFFFED000) = 1;
}
/*
* Configure Set Enable Registers (ICDISERn) and Interrupt Processor Target
* Registers (ICDIPTRn). The default (reset) values are used for other registers
* in the GIC.
*/
void config_interrupt(int N, int CPU_target) {
	int reg_offset, index, value, address;
	/* Configure the Interrupt Set-Enable Registers (ICDISERn).
	* reg_offset = (integer_div(N / 32) * 4
	* value = 1 << (N mod 32) */
	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;
	/* Now that we know the register address and value, set the appropriate bit */
	*(int *)address |= value;

	/* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	* reg_offset = integer_div(N / 4) * 4
	* index = N mod 4 */
	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;
	/* Now that we know the register address and value, write to (only) the
	* appropriate byte */
	*(char *)address = (char)CPU_target;
}
void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void clear_screen (){
	int y,x;
	for(x=0;x<320;x++){
		for(y=0;y<240;y++){
			plot_pixel(x,y,0);
		}
	}
}
void pushbutton_ISR(void) {

	volatile int * PS2_ptr = (int *)PS2_BASE;
	volatile int * LED_ptr = (int *)LEDR_BASE;	//Points to LED Address 
	unsigned char byte0 = 0;
	
    
	int PS2_data = *(PS2_ptr);
	int RVALID = PS2_data & 0x8000;
	int i = 0;
        
	unsigned char key [4] = {0,0,0,0};
	int readInterruptReg;
    

	//Read Interrupt Register
	 readInterruptReg = *(PS2_ptr + 1 );  //added
	  
	//Clear Interrupt 
	*(PS2_ptr+1) = readInterruptReg; //added 
	

	int LED_1 = 0x1; // pattern for LED lights
	int LED_2 = 0x2;
	int LED_3 = 0x3;
	int LED_4 = 0x4; 
	int ERROR = 0x5;
	
	//reset 
	//*(PS2_ptr) = 0xFF; //CAUSES ERROR 
	*(LED_ptr) = 0x40;
	


	//if( (RVALID >> 32) == 1){ //when RVALID is 1, there is data 
	if(RVALID != 0){
               
			byte0 = (PS2_data & 0xFF);//data in LSB	
			*(LED_ptr) = 0x20;
		
           if(byte0 == 0x32) //B, Blue
        	colour=0x0000FF;
        
           if(byte0 == 0x4D) //P, Purple
        	colour=0xF81F;
            
           if(byte0 == 0x2D) //R, Red
        	colour=0xF800;
            
		   if(byte0 == 0x34) //G, Green 
        	colour=0x07E0; 
            
           if(byte0 == 0x35) //Y, Yellow
        	colour=0xFFFF00;
        
			if(byte0 == 0x1D){  //UP, W
				*(LED_ptr) = 0x01; //led 0
				key[0] = 1;
		
				y=y--;
				plot_pixel(x,y,colour);
			}
			if(byte0 == 0x1B){ //DOWN, S
				*(LED_ptr) = 0x02;  //led 2
				key[1] = 1;
				
				y=y++;
				plot_pixel(x,y,colour);
			}
		
			if(byte0 == 0x1C){ //LEFT, A
				*(LED_ptr) = 0x04;  //led 0 & 1 
				key[2] = 1;
				
				 x= x--;
				plot_pixel(x,y,colour);
			
			}
			if(byte0 == 0x23){ //RIGHT, D
				*(LED_ptr) = 0x08; // led 3
				key[3] = 1;
				
				x= x++;
				plot_pixel(x,y,colour);
			}
            if(byte0 == 0x29){  //Clears Screen, SpaceBar 
                //clear_screen(x,y,0x00); 
			 	drawBackground();
				colour = 0x000000;
                x=160; //reintialize 
                y=120; 
            }   
			if(byte0 == 0x33){ //Help Screen, H
				drawHelp();
			}
			
			
			
			/*Draw Stencils*/
			if(byte0 == 0x16){ //Spiral, Level 1 
			drawSpiral();
			 x = 118;
			 y = 162;
            }
			if(byte0 == 0x1E){ //Star, Level 2 
			 drawStar();
			 x = 133;
			 y = 126;
			 
			}
			if(byte0 == 0x26){ //Lotus, Level 3 
			 drawLotus();
			 x = 171;
			 y = 133;
			}
			if(byte0 == 0x25){//Face  ,Level 4
			 drawFace();
			 x = 164;
			 y = 75;
			}
			
			if(byte0 == 0xF0) {
				// going to check for break;
				*(LED_ptr) = 0x00;
				for(i = 0; i < 32; ++i)
					*(LED_ptr) ^= 0x80;
				switch (PS2_data & 0xFF) {
					case 0x1D:
						key[0] = 0;
						break;
					case 0x1B:
						key[1] = 0;
						break;
					case 0x1C:
						key[2]=0;
						break;
					case 0x23:
						key[3]=0;
						break;
					default:
						break;
				}	
			}



					
	} 
	return;
}





