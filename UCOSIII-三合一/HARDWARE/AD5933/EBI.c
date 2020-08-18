/*
 * EBI.cpp
 *
 * Created: 29/09/2019 22:21:40
 *  Author: ZCheng
 */ 


// include the SD library:
//#include <SPI.h>
//#include <SD.h>

#include "EBI.h"

// variables for EBI

short re;
short img;
long mag2;
float MCLK= 16.776 * pow(10, 6); // AD5933 Internal Clock Speed 16.776 MHz
float start_freq = 10000.0; // Set start freq, <= 100Khz
float incre_freq = 0.0; // Set freq increment
int numbinc = 0; // Set number of increments
unsigned char ctrl = 0x05;    // PGA = X1, 0.4 V excitation Vp-p


/************************************************************************/
/* Functions for EBI (IIC)                                              */
/************************************************************************/

void ebiInitial()
{
  //clear ctrl-reg
  writeData(CTRL_REG, 0x0);
  //reset ctrl register
  writeData(CTRL_REG2, 0x10);
  // initialization
  programReg(start_freq, incre_freq, numbinc);
}

void ebiStop()
{
  writeData(CTRL_REG, 0xA0);
  writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0xA0);
}

// we set the excitation frequency here.
void programReg(float s_f, float i_f, int n_i)
{
	// Set Range 3 (0.4V), PGA gain X1
	writeData(CTRL_REG, 0x05);

	// Set settling cycles
	writeData(NUM_SCYCLES_R1, 0x00);
	writeData(NUM_SCYCLES_R2, 0x0F);

	// Start frequency of 1kHz
	writeData(START_FREQ_R1, getFrequency(s_f, 1));
	writeData(START_FREQ_R2, getFrequency(s_f, 2));
	writeData(START_FREQ_R3, getFrequency(s_f, 3));

	// Increment by 1 kHz
	writeData(FREG_INCRE_R1, getFrequency(i_f, 1));
	writeData(FREG_INCRE_R2, getFrequency(i_f, 2));
	writeData(FREG_INCRE_R3, getFrequency(i_f, 3));

	// Points in frequency sweep (100), max 511
	writeData(NUM_INCRE_R1, (n_i & 0x001F00) >> 0x08 );
	writeData(NUM_INCRE_R2, (n_i & 0x0000FF));
}

void writeData(int addr, int data) 
{
	Wire.beginTransmission(SLAVE_ADDR);
	Wire.write(addr);
	Wire.write(data);
	Wire.endTransmission();
	//delay(1);
	delayMicroseconds(1);
}

int readData(int addr) 
{
	int data;

	Wire.beginTransmission(SLAVE_ADDR);
	Wire.write(ADDR_PTR);
	Wire.write(addr);
	Wire.endTransmission();
	//delay(1);
	delayMicroseconds(2);
	Wire.requestFrom(SLAVE_ADDR, 1);
	if (Wire.available() >= 1) {
		data = Wire.read();
	}
	else {
		data = -1;
	}
	//delay(1);
	delayMicroseconds(2);
	return data;
}

float getFrequency(float freq, int n) 
{
	long val = long((freq / (MCLK / 4)) * pow(2, 27));
	float code;

	switch (n) {
		case 1:
		code = (val & 0xFF0000) >> 0x10;
		break;
		case 2:
		code = (val & 0x00FF00) >> 0x08;
		break;
		case 3:
		code = (val & 0x0000FF);
		break;
		default:
		code = 0;
	}
	return code;
}

char measureTemperature() 
{
	char res;
	float val;
	float temperatureData;
	// Measure temperature '10010000'
	writeData(CTRL_REG, 0x90);
	delay(1); // wait for 1 s
	//Check status reg for temp measurement available
	int flag = readData(STATUS_REG) & 0x01;

	if (flag == 0x01) {
		// Temperature is available
		int temperatureData = readData(TEMP_R1) << 8;
		temperatureData |= readData(TEMP_R2);
		temperatureData &= 0x3FFF; // remove first two bits
		if (temperatureData & 0x2000 == 1) { // negative temperature
			temperatureData -= 0x4000;
		}

	   val = (temperatureData) / 32;
		temperatureData /= 32;

		//res = "T" + String(val);
		res = "T" + String(temperatureData);

		// Power Down
		writeData(CTRL_REG, 0xA0);
	}
	else {
		res = "F";
	}
return res;
}

// measure the EBI in a loop, check the value and light up the LED
// no sending data
void ebiDetectFast(long lowB, long upB )
{
	programReg(100000, 0, 0);
	
  // 1. Standby
  writeData(CTRL_REG, ctrl | 0xB0);
  // 2. Initialize sweep
  writeData(CTRL_REG, ctrl | 0x10);
  // 3. Start sweep
  writeData(CTRL_REG, ctrl | 0x20);

  while ((readData(STATUS_REG) & 0x07) < 4 )  // Check if all sweeps complete
  {
    int flag = readData(STATUS_REG) & 0x02;
    if (flag == 0x02)  // check if current sweep finish
    {
      byte R1 = readData(RE_DATA_R1);
      byte R2 = readData(RE_DATA_R2);
      re = (R1 << 8) | R2;

      R1  = readData(IMG_DATA_R1);
      R2  = readData(IMG_DATA_R2);
      img = (R1 << 8) | R2;

      mag2 = long(re)*re + long(img)*img;
 
      // lit up LED
      if ((mag2 >= lowB) && (mag2 <= upB) )
      {
        FrontGLedOn();
      }
      else
      {
        FrontGLedOff();
      }
      
      // if received command from BLE, then stop
      if(Serial.available()) {
        // IF it receive command from BLE
        String strPara = Serial.readString();
        delay(20);
        
        if(strPara.indexOf("STOP") >= 0){
          delay(1);
          // Power down
          ebiStop();
          break; // perhaps no need
        }
      }
	  
	  // if the button is pressed, stop the measurement
	  int intBtn = digitalRead(PinBtn);
	  if (intBtn == HIGH)
	  {
		  AllLedOn();
		  delay(500);
		  AllLedOff();
		  
		  ebiStop();
		  
		  break; // perhaps no need
	  }

      // repeat measurement
      if ((readData(STATUS_REG) & 0x07) < 4 ) {
        writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0x40);
      }
      
      delayMicroseconds(1);
    }
  }
}

// measure the EBI continuously and send the value via BLE
// it also checks if the value is within the threshold, then lights on the LED
void ebiSendContinue(long lgUB, long lgLB)
{	
  // 1. Standby
  writeData(CTRL_REG, ctrl | 0xB0);
  // 2. Initialize sweep
  writeData(CTRL_REG, ctrl | 0x10);
  // 3. Start sweep
  writeData(CTRL_REG, ctrl | 0x20);

  while ((readData(STATUS_REG) & 0x07) < 4 )  // Check if all sweeps complete
  {
    int flag = readData(STATUS_REG) & 0x02;
    if (flag == 0x02)  // check if current sweep finish
    {
      byte R1 = readData(RE_DATA_R1);
      byte R2 = readData(RE_DATA_R2);
      re = (R1 << 8) | R2;

      R1  = readData(IMG_DATA_R1);
      R2  = readData(IMG_DATA_R2);
      img = (R1 << 8) | R2;
	  
	  // get the impedance value
	  long mag2 = long(re)*re + long(img)*img;
	  
	  // lit up LED
	  if ((mag2 <= lgLB) && (mag2 >= lgUB) )
	  {
		  FrontGLedOn();
		  //digitalWrite(PinLedBloodX,HIGH);
	  }
	  else
	  {
		  FrontGLedOff();
		  //digitalWrite(PinLedBloodX,LOW);
	  }

      Serial.print(re);
      Serial.print('\t');
      Serial.println(img);
      
      delay(1);
      
      // if received command, then stop
      if(Serial.available()) {
        // IF it receive command from BLE
        String strPara = Serial.readString();
        delay(10);
        
        if(strPara.indexOf("STOP") >= 0){
          delay(1);
          // Power down
          ebiStop();
          break;
        }
      }

      // repeat measurement
      if ((readData(STATUS_REG) & 0x07) < 4 ) {
        writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0x40);
      }
    }
  }
}

// sampling multiple frequencies
String ebiMulSamp()
{
	String strRes="";
	//strRes.reserve(100);
  // set the start frequency, the number of increment and the frequency increment  
  // write to the registers
  programReg(10000.0,10000.0,9);
  
  // 1. Standby
  writeData(CTRL_REG, ctrl | 0xB0);
  // 2. Initialize sweep
  writeData(CTRL_REG, ctrl | 0x10);
  // 3. Start sweep
  writeData(CTRL_REG, ctrl | 0x20);
  
  while ((readData(STATUS_REG) & 0x07) < 4 ) { 
    // Check if all sweeps complete
    int flag = readData(STATUS_REG) & 0x02;
	
    if (flag == 0x02)
    {  // check if current sweep finish
      byte R1 = readData(RE_DATA_R1);
      byte R2 = readData(RE_DATA_R2);
      re = (R1 << 8) | R2;

      R1  = readData(IMG_DATA_R1);
      R2  = readData(IMG_DATA_R2);
      img = (R1 << 8) | R2;

      //strRes = strRes + String(re) + ";" + String(img) + '\t';
	  strRes = strRes + "12345;67890" + '\t';
	  delay(60);

      // Increment frequency
      if ((readData(STATUS_REG) & 0x07) < 4 ) {
        writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0x30);
      }
    }
  }
  delayMicroseconds(1);

  // Power down
  writeData(CTRL_REG, 0xA0);
  writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0xA0);
  
  return strRes;
}

// Sampling once with a single given frequency
void ebiOnce(short *real, short *image)
{	
  // 1. Standby
  writeData(CTRL_REG, ctrl | 0xB0);
  // 2. Initialize sweep
  writeData(CTRL_REG, ctrl | 0x10);
  // 3. Start sweep
  writeData(CTRL_REG, ctrl | 0x20);

  while ((readData(STATUS_REG) & 0x07) < 4 ) { // Check if all sweeps complete

    int flag = readData(STATUS_REG) & 0x02;
    if (flag == 0x02) 
    {  // check if current sweep finish
      byte R1 = readData(RE_DATA_R1);
      byte R2 = readData(RE_DATA_R2);
      *real = (R1 << 8) | R2;

      R1  = readData(IMG_DATA_R1);
      R2  = readData(IMG_DATA_R2);
      *image = (R1 << 8) | R2;

      //float t_re = abs(re);
      //float t_im = abs(img);
      //mag = sqrt(t_re * t_re + t_im * t_im);
      // phase = atan(double(img)/double(re));
      // phase = (180.0/3.1415926)*phase;  //convert phase angle to degrees

      // Phase Calibration
      // sys_phase = 118;
      // phase = phase - sys_phase;

      //impedance = 1/(gain*mag);

      //res = String(freq/1000)+","+String(re)+","+String(img)+","+String(timenow-time0)+'\n';
      //res = String(re)+","+String(img);

      // Increment frequency
      if ((readData(STATUS_REG) & 0x07) < 4 ) {
        writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0x30);
        //i++;
      }
    }
  }
  delayMicroseconds(1);

  // Power down
  writeData(CTRL_REG, 0xA0);
  writeData(CTRL_REG, (readData(CTRL_REG) & 0x07) | 0xA0);
  
  //return res;	
}
