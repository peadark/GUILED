#include "mbed.h"
#include "WS2811.h"

#define X_MAX	64
#define Y_MAX	64

// per LED: 3 * 20 mA = 60mA max
// 60 LEDs: 60 * 60mA = 3600 mA max
// 120 LEDs: 7200 mA max
unsigned const nLEDs = MAX_LEDS_PER_STRIP;			// 64


uint8_t mask_1 = 1;
uint8_t mask_2 = 2;
uint8_t mask_3 = 4;
// Pins *NUMBERS* for PORTC (convenient for KL25Z freedom board)
unsigned const DATA_OUT_PIN1 = 7; 


// Address Pin locations (There is some issue with the pins where only these specific pins can be used for addressing.)
DigitalOut ADDRESS_PIN_3(D2); // AKA PTD4
DigitalOut ADDRESS_PIN_2(D3); // AKA PTA12
DigitalOut ADDRESS_PIN_1(D5); // AKA PTA5



Serial pc(USBTX, USBRX);					//  D1 (PTA2) and D0 (PTA1) on KL25Z

static void clearStrip(WS2811 &strip)
{
    unsigned nLEDs = strip.numPixels();
    for (unsigned i = 0; i < nLEDs; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
}

int main(void)
{
	myled = 1;
pc.baud(115200);							// 11520 Bytes/s -> ~0.3s for 32x32 RGB pixels
    WS2811 *lightStrip;
    // Initialize an 8-strip matrix
    // Note that these are all using the same Data Out Pin - NB This needs to be more scalable
    WS2811 lightStrip1(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip2(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip3(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip4(nLEDs, DATA_OUT_PIN1);    
    WS2811 lightStrip5(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip6(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip7(nLEDs, DATA_OUT_PIN1);
    WS2811 lightStrip8(nLEDs, DATA_OUT_PIN1);
    
    
    
    lightStrip1.begin();
    lightStrip2.begin();
    lightStrip3.begin();
    lightStrip4.begin();
    lightStrip5.begin();
    lightStrip6.begin();
    lightStrip7.begin();
    lightStrip8.begin();

    uint8_t q = 0;								// index variable
    uint8_t char_buff[10];							// UART buffer
    uint8_t curr_char = 0;							// current byte read from UART
    uint8_t	j;									// for - loop iteration variable
    
    for (;;) {
        /* I have defined a custom UART packet protocol
         * A general packet may look like: 
         * char_buff[0]		start byte: 	254
         * char_buff[1]     x-coordinate [0, 63]
         * char_buff[2]     y-coordinate [0, 63]
         * char_buff[3]     R (6-bit)	 [0, 63]
         * char_buff[4]     G (6-bit)	 [0, 63]
         * char_buff[5]     B (6-bit)	 [0, 63]
         * char_buff[6]     Delimiter: 		255
         */
        while(curr_char != 255 && q < 7)
        {
            /* If there is UART traffic */
          //  while( ! pc.readable());
            if(pc.readable())
            {
                curr_char = uint8_t(pc.getc());
                char_buff[q] = curr_char;
                q++;
            }
        }
        
        if((char_buff[0] == 254) && (q >= 5)){
            
               // This also needs to be more scalable. Considering using for - loop
            switch(char_buff[2]%Y_MAX){
            case 0:

                ADDRESS_PIN_1 =  0; //Change the value on each address pin
                ADDRESS_PIN_2 =  0;
                ADDRESS_PIN_3 =  0;
                lightStrip = &lightStrip1;
                myled = 0;
                break;
            case 1:

                ADDRESS_PIN_1 =  1; //Change the value on each address pin
                ADDRESS_PIN_2 =  0;
                ADDRESS_PIN_3 =  0;
                lightStrip = &lightStrip2;
                myled = 0;
                break;
            case 2:
                ADDRESS_PIN_1 =  0; //Change the value on each address pin
                ADDRESS_PIN_2 =  1;
                ADDRESS_PIN_3 =  0;
                lightStrip = &lightStrip3;
                myled =0;
                break;
            case 3:
                ADDRESS_PIN_1 =  1; //Change the value on each address pin
                ADDRESS_PIN_2 =  1;
                ADDRESS_PIN_3 =  0;
                lightStrip = &lightStrip4;
                myled = 0;
                break;
            case 4:
                ADDRESS_PIN_1 =  0; //Change the value on each address pin
                ADDRESS_PIN_2 =  0;
                ADDRESS_PIN_3 =  1;
                lightStrip = &lightStrip5;
                myled = 0;
                break;
            case 5:
                ADDRESS_PIN_1 =  1; //Change the value on each address pin
                ADDRESS_PIN_2 =  0;
                ADDRESS_PIN_3 =  1;
                lightStrip = &lightStrip6;
                break;
            case 6:
                ADDRESS_PIN_1 =  0; //Change the value on each address pin
                ADDRESS_PIN_2 =  1;
                ADDRESS_PIN_3 =  1;
                lightStrip = &lightStrip7;
                break;
            case 7:
                ADDRESS_PIN_1 =  1; //Change the value on each address pin
                ADDRESS_PIN_2 =  1;
                ADDRESS_PIN_3 =  1;
                lightStrip = &lightStrip8;
                break;        
            default:
                ADDRESS_PIN_1 =  1; //Change the value on each address pin
                ADDRESS_PIN_2 =  1;
                ADDRESS_PIN_3 =  1;
                lightStrip = &lightStrip8;
                q = 0;
                break;   
            }
            
           
            
            //Antonio's Original Code
   
            // 								{PIXEL_POS (X), 		R, 				G, 				B}
            (*lightStrip).setPixelColor(char_buff[1]%X_MAX, (char_buff[3]), (char_buff[4]), (char_buff[5]));
         
            (*lightStrip).show();
            
        	
            pc.putc(48+(char_buff[1]));
            
        }
        else{   
        		// This should scroll through each strip and clear them, but it doesn't.
        		for (j = 0; j < 8 ; j ++)      {  
        		
           			clearStrip(lightStrip1);
           			clearStrip(lightStrip2);
            		clearStrip(lightStrip3);
		            clearStrip(lightStrip4);
        		    clearStrip(lightStrip5);
     		    	clearStrip(lightStrip6);
           	 		clearStrip(lightStrip7);
            		clearStrip(lightStrip8);
            		
            		ADDRESS_PIN_1 = j & mask_1;
            		ADDRESS_PIN_2 = j & mask_2;
            		ADDRESS_PIN_3 = j & mask_3;
            		}
        }
        q = 0;
        curr_char = 0;
        WS2811::startDMA();

    }
}
