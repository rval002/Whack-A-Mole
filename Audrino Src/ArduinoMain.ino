////////////////////////// arduino////////////////////////////////////



#include <SPI.h>
#include <LedControl.h>

char buff [255];
volatile byte indx;
volatile boolean process;
char a;
char b;
int c;
int score;
int DIN = 6;
int CS =  5;
int CLK = 4;

LedControl lc=LedControl(DIN,CLK,CS,0);

 
void setup (void) {
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt

   setup1();
     
}

void setup1(void) {
// Ledstuff
   lc.shutdown(0,false);       
   lc.setIntensity(0,15);      //Adjust the brightness maximum is 15
   lc.clearDisplay(0);  

}
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
   
   if (indx < sizeof(buff)) {
      buff[indx++] = c; // save data in the next index in the array buff
      if (c == '\n') { 
//        a = buff[indx-3];

//        b = buff[indx];
        
        buff[indx - 1] = 0; // replace newline ('\n') with end of string (0)
        a = sizeof(buff[indx -1]);
        process = true;
      }
   }   
}
 
void loop (void) {
 


    byte *arrayofNum[10];
    // numbers 1 -9
    byte d0[8]=   {0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C};
    byte d1[8]=   {0x18,0x38,0x48,0x08,0x08,0x08,0x08,0x7E};
    
    byte d2[8]= {0x3C,0x42,0x44,0x08,0x10,0x20,0x40,0x7E};
    byte d3[8]=   {0x38,0x04,0x04,0x38,0x04,0x04,0x38,0x00};
    
    byte d4[8]= {0x44,0x44,0x44,0x44,0x7C,0x04,0x04,0x04};
    byte d5[8]= {0x00,0x3C,0x20,0x20,0x38,0x04,0x04,0x38};
   
    
    byte d6[8]= {0x00,0x38,0x44,0x40,0x78,0x44,0x44,0x38};
    byte d7[8]= {0x00,0x3E,0x02,0x04,0x08,0x08,0x08,0x08};
    
   
    byte d8[8]= {0x00,0x3C,0x42,0x42,0x3C,0x42,0x42,0x3C};
    byte d9[8]= {0x3C,0x42,0x42,0x42,0x3E,0x02,0x04,0x38};

    //Win flash
    byte w1[8]= {0xAB,0xD5,0xAB,0xD5,0xAB,0xD5,0xAB,0xD5};
    byte w2[8]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    byte w3[8]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    arrayofNum[0] = d0;
    arrayofNum[1] = d1;
    arrayofNum[2] = d2;
    arrayofNum[3] = d3;
    arrayofNum[4] = d4;
    arrayofNum[5] = d5;
    arrayofNum[6] = d6;
    arrayofNum[7] = d7;
    arrayofNum[8] = d8;
    arrayofNum[9] = d9;


if (score < 10){
    delay(100);

    printByte(arrayofNum[score]); 
    delay(100);
      

  }
  else {
    delay(50);
    printByte(w1);
    delay(100);
    printByte(w2);
    delay(100);
    printByte(w3);
    delay(100);
  }

   if (process) {
    score = 1 + score;
      process = false; //reset the process
      Serial.println("score");  
      Serial.println(score);
//      Serial.println (buff); //print the array on serial monitor
      indx= 0; //reset button to zero
   }
//   Serial.println("outside loop");
//   Serial.println(score);


}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}


///////////////////////////////////////////////////////////////////
