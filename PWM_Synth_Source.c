#include <htc.h>
#include <delays.h>
#include <math.h>

void Send_HL_I2C(char add, char SendChar);
void Send_Gy_I2C(char add, char SendChar);
void Set_point_HL(char add);
int Read_HL_I2C(char add);
int Read_Gy_I2C(char add);

char FreqWave = 0;
char FreqTim = 0;
char count = 0;
char lastRecievedID;
char Phase = 0;
char PhaseShift = 0;
char SizeData = 49;
char DataNotLoaded= FALSE;
char counterData = 0;
char MaxWaveSize = 100;


//GenericWaveForm initizalized to SineWave
char WaveForm[50] = {0x10,0x11,0x13,0x15,0x17,0x19,0x1a,0x1b,
0x1d,0x1e,0x1e,0x1f,0x1f,0x1f,0x1f,0x1e,
0x1e,0x1d,0x1b,0x1a,0x19,0x17,0x15,0x13,
0x11,0x10,0xe,0xc,0xa,0x8,0x6,0x5,0x4,0x2,
0x1,0x1,0x0,0x0,0x0,0x0,0x1,0x1,0x2,0x4,
0x5,0x6,0x8,0xa,0xc,0xe};

//20 Sample Sine
//char WaveForm[100] = {15,20,24,27,29,30,29,27,24,20,15,10, 6, 3, 1, 0, 1, 3, 6,10};


//////////////////////////////////////////////////////////
// 				    FUNCTION DEFINITIONS				//
//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
// 				   		 MAIN							//
//////////////////////////////////////////////////////////

int main(){

 TRISC = TRISC & 0b11110000;
 
 SPBRG = 0x1F;

 //CONFIGURE INTERUPUTS
 INTCON = 0b11100001;
 //PIE1 = 0b00101000; enabled SSPIF
 PIE1 = 0b00100000;
 SSPIF = 0;

 TRISC = 0b11111000;
 LATC = 0b00011000;

 //Config UART
 RCSTA = 0x90;
 TXSTA = 0x20;

 //PR2 = 0x36;
 PR2 = 0x1F;
 CCPR2L = 0x15;
 T2CON = 0b00000100;
 CCP1CON = 0b00001100;

T0CON = 0b11000011;
TMR0IE = 1;

 while(1){
   
   Phase = (FreqWave+PhaseShift)%SizeData;

   CCPR1L = WaveForm[Phase];

 }

}


//////////////////////////////////////////////////////////
// 				   	  INTERRUPTS						//
//////////////////////////////////////////////////////////



//Rx INTERUPT FUNCTION
void interrupt ISR(){


   //UART RECIEVE INTERRUPT
   if(RCIF){	 
      //Read the byte
     if(RCREG > 250){
       lastRecievedID = RCREG;
       //SET FLAG FOR DATA RECIEVE
       if(RCREG == 253) DataNotLoaded = TRUE;   
     } else if (lastRecievedID == 251){
	       FreqTim = 256 - RCREG +2;
     } else if (lastRecievedID == 252){
       PhaseShift = RCREG;
     } else if ((lastRecievedID == 253) && DataNotLoaded){
	   //LAST BYTE NOT RECIEVED - LOAD DATA
       if(counterData == 0){
         SizeData = RCREG;
	     counterData++;
       } else if(counterData-1 < SizeData){
         WaveForm [counterData-1] = RCREG;
		 counterData++;
       } else {
         DataNotLoaded = FALSE;
         counterData = 0;
       }
     }

     RCIF = 0;
   } else if(TMR0IF){

       if(FreqWave <= SizeData-1){
	     FreqWave = FreqWave + 1;
       } else { 
         FreqWave = 0;
       }
	   TMR0 = FreqTim;
	   TMR0IF = 0;
   }

}