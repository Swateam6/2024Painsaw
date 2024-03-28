/* TeensyPPM loopback test.  Create multiple input/output pins and loop back the PPM
 * signals changing polarity on each jump.
 *
 * 	Taranis Dragon-Link input         ==> INPUT:   PIN(05) FALLING	(FTM-00)
 *	INPUT:	PIN(05) FALLING (FTM-00)  ==> OUTPUT:  PIN(04) RISING	(FTM-01)
 *	OUTPUT:	PIN(04) RISING	(FTM-01)  ==> JUMPER:  PIN(25) RISING	(FTM-02)
 *	INPUT:	PIN(25) RISING	(FTM-02)  ==> OUTPUT:  PIN(23) FALLING	(FTM-00)
 *	OUTPUT:	PIN(23) FALLING	(FTM-00)  ==> JUMPER:  PIN(03) FALLING	(FTM-01)
 *	INPUT:	PIN(03) FALLING (FTM-01)  ==> OUTPUT:  PIN(32) RISING	(FTM-02)
 *	OUTPUT: PIN(32) RISING  (FTM-02)  ==> JUMPER:  PIN(10) RISING	(FTM-00)
 *	INPUT:  PIN(10) RISING  (FTM-00)
 */

#define  PPM_INPUT_05	5	// FALLING
#define  PPM_OUTPUT_04	4	// RISING
#define  PPM_INPUT_25	25	// RISING
#define  PPM_OUTPUT_23	23	// FALLING
#define  PPM_INPUT_03	3	// FALLING
#define  PPM_OUTPUT_32	32	// RISING
#define  PPM_INPUT_10	10	// RISING

#include <PPM.h>
	PPM myPPM;	/* Instance our PPM class library	*/
	int count=0;	/* Initialize our counter		*/


void setup() {
	Serial.print("TeensyPPM Library Version Number:  ");
	Serial.println(myPPM.getVersion());				/* Print Version Number					*/
	myPPM.PPMInput(FALLING, 2, PPM_INPUT_05, PPM_INPUT_03);		/* Instance pins 5 and 3 as FALLING edge input pins	*/
	myPPM.PPMInput(RISING, 2, PPM_INPUT_25, PPM_INPUT_10);		/* Instance pins 25 and 10 as RISING edge input pins	*/
	myPPM.PPMOutput(RISING, 2, PPM_OUTPUT_04, PPM_OUTPUT_32);	/* Instance pins 4 and 32 as RISING edge output pins	*/
	myPPM.PPMOutput(FALLING, 1, PPM_OUTPUT_23);			/* Instance pin 23 as FALLING edge output pin		*/

	/* Initialize PPM output on all 16 channels	*/
	myPPM.dataWrite(PPM_OUTPUT_04, 1, 1000);
	myPPM.dataWrite(PPM_OUTPUT_04, 2, 1030);
	myPPM.dataWrite(PPM_OUTPUT_04, 3, 1060);
	myPPM.dataWrite(PPM_OUTPUT_04, 4, 1090);
	myPPM.dataWrite(PPM_OUTPUT_04, 5, 1120);
	myPPM.dataWrite(PPM_OUTPUT_04, 6, 1150);
	myPPM.dataWrite(PPM_OUTPUT_04, 7, 1180);
	myPPM.dataWrite(PPM_OUTPUT_04, 8, 1210);
	myPPM.dataWrite(PPM_OUTPUT_04, 9, 1240);
	myPPM.dataWrite(PPM_OUTPUT_04, 10, 1270);
	myPPM.dataWrite(PPM_OUTPUT_04, 11, 1300);
	myPPM.dataWrite(PPM_OUTPUT_04, 12, 1330);
	myPPM.dataWrite(PPM_OUTPUT_04, 13, 1360);
	myPPM.dataWrite(PPM_OUTPUT_04, 14, 1390);
	myPPM.dataWrite(PPM_OUTPUT_04, 15, 1450);
	myPPM.dataWrite(PPM_OUTPUT_04, 16, 1500);
}

void loop() {
	int i, num;

/*	Receive input from Taranis DragonLink on PIN(03) FALLING Edge.  The
 *	DragonLink will emit 12 PPM channels.  Upon arrival, swap channels
 *	1 with channel 8, and leave all the others the same.
 *
 * 	Taranis Dragon-Link input         ==> INPUT:   PIN(05) FALLING	(FTM-00)
 *	INPUT:	PIN(05) FALLING (FTM-00)  ==> OUTPUT:  PIN(04) RISING	(FTM-01)
 */
 	num = myPPM.dataAvl(PPM_INPUT_05);
	if (num > 0) {
	for (i=1; i <= num; i++) {
		float val = myPPM.dataRead(PPM_INPUT_05, i);
		switch (i) {
			case 1:
				myPPM.dataWrite(PPM_OUTPUT_04, 8, val);
				break;

			case 8:
				myPPM.dataWrite(PPM_OUTPUT_04, 1, val);
				break;

			default:
				myPPM.dataWrite(PPM_OUTPUT_04, i, val);
				break;
			}
		}
	}

/*	Receive input from PIN(25) and output on PIN(03).  Jumper PIN(23) to PIN(03).
 *      Move all of the data from channel-N to channel N+1.
 *
 *	INPUT:	PIN(25) RISING	(FTM-02)  ==> OUTPUT:  PIN(23) FALLING	(FTM-00)
 *	OUTPUT:	PIN(23) FALLING	(FTM-00)  ==> JUMPER:  PIN(03) FALLING	(FTM-01)
 */
	num = myPPM.dataAvl(PPM_INPUT_25);
	if (num > 0) {
		for (i=1; i <= num; i++) {
			float val = myPPM.dataRead(PPM_INPUT_25, i);
			myPPM.dataWrite(PPM_OUTPUT_23, (i % 16) + 1, val);
		}
	}

/*	Receive input from PIN(03) and output on PIN(32).  Jumper PIN(32) to PIN(10).
 *      Move all of the data from channel-N to channel N+1.
 *
 *	INPUT:	PIN(03) FALLING (FTM-01)  ==> OUTPUT:  PIN(32) RISING	(FTM-02)
 *	OUTPUT: PIN(32) RISING  (FTM-02)  ==> JUMPER:  PIN(10) RISING	(FTM-00)
 */
	num = myPPM.dataAvl(PPM_INPUT_03);
	if (num > 0) {
		for (i=1; i <= num; i++) {
			float val = myPPM.dataRead(PPM_INPUT_03, i);
			myPPM.dataWrite(PPM_OUTPUT_32, (i % 16) + 1, val);
		}
	}

/*	Receive input from PIN(10) and print the results on the Serial port debug
 *	output.
 *
 *	INPUT:  PIN(10) RISING  (FTM-00)
 */
	num = myPPM.dataAvl(PPM_INPUT_10);
	if (num > 0) {
		count = count + 1;
		Serial.print(count);
		Serial.print(" :  ");
		for (i=1; i <= num; i++) {
			float val = myPPM.dataRead(PPM_INPUT_10, i);
			Serial.print(val);
			Serial.print("  ");
		}
		Serial.println();
	}
}
