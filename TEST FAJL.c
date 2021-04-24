#include <htc.h>
#include <delays.h>
#include <math.h>

void Send_HL_I2C(char add, char SendChar);
void Send_Gy_I2C(char add, char SendChar);
void Set_point_HL(char add);
int Read_HL_I2C(char add);
int Read_Gy_I2C(char add);

char temp = 0;
char count = 0;
char SlaveADD = 0x3C;
char SlaveADDGy = 0xD0;
char SlaveADDR = 0x3D;
char SlaveADDRGy = 0xD1;
char SlaveADDAcc =0xA6;
char SlaveADDAccR =0xA7;
char UpperByte,LowerByte;

int XPos1,XPos2,XPos,YPos1,YPos2,YPos,ZPos1,ZPos2,ZPos;
int R1,R2,R3,R4,R5,R6,R7,R8,R9,R10;
int RC_flag = 0;
int InterruptCount = 0;
int InterruptCountRX = 0;
int InterruptCountTX = 0;
int InterruptCountElse = 0;
int AutoSend = FALSE;

//////////////////////////////////////////////////////////
// 				    FUNCTION DEFINITIONS				//
//////////////////////////////////////////////////////////

void Send_Sensor_Serial(char SensIndex,char FirstChar, char SecondChar, char CoordIndex){
	
	//Send Sensor ID
	TXREG = SensIndex;
	while(TRMT==0);
	Delay10TCYx(10);
	
	Delay10TCYx(1);
	TXREG = FirstChar;
	while(TRMT==0);
	Delay10TCYx(10);
	TXREG = SecondChar;
	Delay10TCYx(10);
	while(TRMT==0);
	//Send X indentifier
	TXREG = CoordIndex;
	Delay10TCYx(10);
	while(TRMT==0);
	
}


void Send_HL_I2C(char FirstChar, char SecondChar){

	  //SEN = 1
	  unsigned int timeout=1000;

 	  SSPCON2 = 0b00000001;

	  while(SEN==1 && (--timeout > 0)){}
	 
      
	  SSPBUF = SlaveADD;
	  SSPIF = 0;


      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = FirstChar;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = SecondChar;
	  SSPIF = 0;
		
	  while(SSPIF==0 && (--timeout > 0)){}

	  SSPIF = 0;
      SSPCON2 = 4;
		  
	
	  while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

}


void Send_Acc_I2C(char FirstChar, char SecondChar){

	  //SEN = 1
	  unsigned int timeout=1000;

 	  SSPCON2 = 0b00000001;

	  while(SEN==1 && (--timeout > 0)){}
	 
      
	  SSPBUF = SlaveADDAcc;
	  SSPIF = 0;


      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = FirstChar;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = SecondChar;
	  SSPIF = 0;
		
	  while(SSPIF==0 && (--timeout > 0)){}

	  SSPIF = 0;
      SSPCON2 = 4;
		  
	
	  while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

}

void Send_Gy_I2C(char FirstChar, char SecondChar){

	  //SEN = 1
	  unsigned int timeout=1000;

 	  SSPCON2 = 0b00000001;

	  while(SEN==1 && (--timeout > 0)){}
	 
      
	  SSPBUF = SlaveADDGy;
	  SSPIF = 0;


      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = FirstChar;
	  SSPIF = 0;

	  while(SSPIF==0 && (--timeout > 0)){}

	  SSPIF = 0;
      SSPCON2 = 4;
		  
	
	  while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

}

void Set_point_HL(char add){

	  //SEN = 1
	  unsigned int timeout=1000;	

 	  SSPCON2 = 0b00000001;

	  while(SEN==1){}
	 
      
	  SSPBUF = SlaveADD;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}

	  SSPBUF = add;
	  SSPIF = 0;
		
	  while(SSPIF==0 && (--timeout > 0)){}

	  SSPIF = 0;
      SSPCON2 = 4;
		  
	
	  while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

}

int Read_HL_I2C(char addr){

	  //SEN = 1
	  unsigned int timeout=1000;
      int temp1, temp2; 

 	  SSPCON2 = 0b00000001;
   
	  while(SEN==1){}
	 
      
	  SSPBUF = SlaveADDR;
	  SSPIF = 0;

	  while(SSPIF==0 && (--timeout > 0)){}	  
	  SSPIF = 0;
	  RCEN = 1; 
	  

	  while(SSPIF==0 && (--timeout > 0)){}
      //ACKDT = 1;
      temp1 = SSPBUF;
	  ACKEN = 1;
	  SSPIF = 0;


	  while(SSPIF==0 && (--timeout > 0)){}
      RCEN = 1;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

	  ACKDT = 1;
      ACKEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  temp2 = SSPBUF;
	  SSPIF = 0;

	  PEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

      return ~(((temp1<<8) | temp2)-1);

}

int Read_Acc_I2C(char addr){

	  //SEN = 1
	  unsigned int timeout=1000;
      int temp1, temp2; 

 	  SSPCON2 = 0b00000001;
   
	  while(SEN==1){}
	 
      
	  SSPBUF = SlaveADDAccR;
	  SSPIF = 0;

	  while(SSPIF==0 && (--timeout > 0)){}	  
	  SSPIF = 0;
	  RCEN = 1; 
	  

	  while(SSPIF==0 && (--timeout > 0)){}
      //ACKDT = 1;
      temp1 = SSPBUF;
	  ACKEN = 1;
	  SSPIF = 0;


	  while(SSPIF==0 && (--timeout > 0)){}
      RCEN = 1;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

	  ACKDT = 1;
      ACKEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  temp2 = SSPBUF;
	  SSPIF = 0;

	  PEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

      return ((temp2<<8) | temp1);

}

int Read_Gy_I2C(char addr){

	  //SEN = 1
	  unsigned int timeout=1000;
      int temp1, temp2; 

 	  SSPCON2 = 0b00000001;
   
	  while(SEN==1){}
	 
      
	  SSPBUF = SlaveADDRGy;
	  SSPIF = 0;

	  while(SSPIF==0 && (--timeout > 0)){}	  
	  SSPIF = 0;
	  RCEN = 1; 
	  

	  while(SSPIF==0 && (--timeout > 0)){}
      //ACKDT = 1;
      temp1 = SSPBUF;
	  ACKEN = 1;
	  SSPIF = 0;


	  while(SSPIF==0 && (--timeout > 0)){}
      RCEN = 1;
	  SSPIF = 0;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

	  ACKDT = 1;
      ACKEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  temp2 = SSPBUF;
	  SSPIF = 0;

	  PEN = 1;

      while(SSPIF==0 && (--timeout > 0)){}
	  SSPIF = 0;

      return ~(((temp1<<8) | temp2)-1);

}

//////////////////////////////////////////////////////////
// 				   		 MAIN							//
//////////////////////////////////////////////////////////

int main(){

 int T = 0;

 TRISD = 0x00;
 SPBRG = 0x1F;

 //CONFIGURE INTERUPUTS
 INTCON = 0b11100001;
 //PIE1 = 0b00101000; enabled SSPIF
 PIE1 = 0b00100000;
 SSPIF =  0;

 TRISC = 0b11111000;
 LATC = 0b00011000;
 SSPSTATbits.SMP = 1;
 SSPSTATbits.CKE = 1;
 SSPCON1 = 0b00101000; 
 
//High speed mode 400KHZ
//SSPADD = 0x0C;

//Low Speed mode 100 KHz
 SSPADD = 0x30;

 //Config UART
 RCSTA = 0x90;
 TXSTA = 0x20;

 Delay10TCYx(1);
 temp = 0xFF;
 PIE1bits.TXIE = 0;
 T0CON = 0b0000000;

 Send_Acc_I2C(0x31,0x01);
 Send_Acc_I2C(0x2D,0x08);

 //Set gyro sensitivity to +-2000deg/s
 Send_Gy_I2C(0x16,0x18);

//Configure Magnetometer
Send_HL_I2C(0x00,0x70);
Send_HL_I2C(0x01,0xA0);
Send_HL_I2C(0x02,0x00);

 while(1){
  
  
    if(AutoSend){
		
    
	//READ HL
	Set_point_HL(0x03); 
    XPos = Read_HL_I2C(0x3C);
	Set_point_HL(0x05);
    YPos = Read_HL_I2C(0x3C);
	Set_point_HL(0x07);
    ZPos = Read_HL_I2C(0x3C);
    
	//SEND HL X-COORD
	UpperByte = XPos>>8;
	LowerByte = XPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x58);
    
	Delay10TCYx(10);
	
	//SEND HL Y-COORD	
	UpperByte = YPos>>8;
	LowerByte = YPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x59);
    
	Delay10TCYx(10);
	
	//SEND HL Z-COORD
	UpperByte = ZPos>>8;
	LowerByte = ZPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x5A);
    
	Delay10TCYx(200);
	
    //INITIALIZE GYRO
    Send_Gy_I2C(0x00,0x00);
	Send_Gy_I2C(0x1F,0x20);
    
	Delay1KTCYx(1);
    
	//READ GYRO
	XPos = Read_Gy_I2C(0x3C);
	YPos = Read_Gy_I2C(0x3C);
	ZPos = Read_Gy_I2C(0x3C);

	//SEND GYRO X-COORD
	UpperByte = XPos>>8;
	LowerByte = XPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x58);
    
	Delay10TCYx(10);
	
	//SEND GYRO Y-COORD	
	UpperByte = YPos>>8;
	LowerByte = YPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x59);
    
	Delay10TCYx(10);
	
	//SEND GYRO Z-COORD
	UpperByte = ZPos>>8;
	LowerByte = ZPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x5A);
    
	Delay10TCYx(200);

	//CONFIGURE ACC.
    Send_Acc_I2C(0x32,0x32);
    
	Delay1KTCYx(1);
    //READ ACC. DATA
	XPos = Read_Acc_I2C(0x3C);
	YPos = Read_Acc_I2C(0x3C);
	ZPos = Read_Acc_I2C(0x3C);

	//SEND ACC. X-COORD
	UpperByte = XPos>>8;
	LowerByte = XPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x58);
    
	Delay10TCYx(10);
	
	//SEND ACC. Y-COORD	
	UpperByte = YPos>>8;
	LowerByte = YPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x59);
    
	Delay10TCYx(10);
	
	//SEND ACC. Z-COORD
	UpperByte = ZPos>>8;
	LowerByte = ZPos;
	
	Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x5A);
    
	Delay10TCYx(200);
	
   } else if(RC_flag!=0) {

	if(temp==0x30){

        Delay1KTCYx(1);
        //Set_point_HL(0x00);
        //%R1 = Read_HL_I2C(0x3C);
	    //Set_point_HL(0x02);
	    //R2 = Read_HL_I2C(0x3C);
		Set_point_HL(0x03);
	    XPos = Read_HL_I2C(0x3C);
		Set_point_HL(0x05);
        YPos = Read_HL_I2C(0x3C);
		Set_point_HL(0x07);
	    ZPos = Read_HL_I2C(0x3C);
		//Set_point_HL(0x0B);
	    //R6 = Read_HL_I2C(0x3C);
		//Set_point_HL(0x0D);
	    //R7 = Read_HL_I2C(0x3C);
		//Set_point_HL(0x0F);
        //R8 = Read_HL_I2C(0x3C);
		//Set_point_HL(0x10);
	    //R9 = Read_HL_I2C(0x3C);
		//Set_point_HL(0x12);
	    //R10 = Read_HL_I2C(0x3C);



	  } else if(temp==0x31) {
        
	    Delay1KTCYx(1);
        Send_Gy_I2C(0x1D,0x1D);
	    XPos = Read_Gy_I2C(0x3C);
        Send_Gy_I2C(0x1F,0x1F);
	    YPos = Read_Gy_I2C(0x3C);
        Send_Gy_I2C(0x21,0x21);
	    ZPos = Read_Gy_I2C(0x3C);
        //Send_Gy_I2C(0x1F,0x1F);
	    //YPos2 = Read_Gy_I2C(0x3C);
        //Send_Gy_I2C(0x20,0x20);
	    //ZPos1 = Read_Gy_I2C(0x3C);
        //Send_Gy_I2C(0x21,0x21);
	    //ZPos2 = Read_Gy_I2C(0x3C);


	} else if(temp==0x32) {

        Send_Acc_I2C(0x32,0x32);
		Delay1KTCYx(1);
	    XPos = Read_Acc_I2C(0x3C);

        Send_Acc_I2C(0x34,0x34);
		Delay1KTCYx(1);
	    YPos = Read_Acc_I2C(0x3C);

        Send_Acc_I2C(0x36,0x36);
		Delay1KTCYx(1);
	    ZPos = Read_Acc_I2C(0x3C);

	} 

	    UpperByte = XPos>>8;
	    LowerByte = XPos;
		
	    Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x58);
        
	    Delay10TCYx(100);
	
		UpperByte = YPos>>8;
	    LowerByte = YPos;
		
	    Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x59);
        
	    Delay10TCYx(100);

		UpperByte = ZPos>>8;
	    LowerByte = ZPos;
		
	    Send_Sensor_Serial(temp, UpperByte, LowerByte, 0x5A);
        
	    Delay10TCYx(100);

   	    RC_flag=0;

   }

 }

}


//////////////////////////////////////////////////////////
// 				   	  INTERRUPTS						//
//////////////////////////////////////////////////////////



//Rx INTERUPT FUNCTION
void interrupt ISR(){

    InterruptCount++;

   //UART RECIEVE INTERRUPT
   if(RCIF){

	 InterruptCountRX++; 

     //Read the byte
     temp = RCREG;
	 RC_flag = 1;
     RCIF = 0;

   } else if(TXIF){
	 PIE1bits.TXIE = 0;
	 InterruptCountTX++;
   } else {
	 InterruptCountElse++;
	 PIE1 = 0;
   }

}