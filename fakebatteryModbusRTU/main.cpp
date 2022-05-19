// #include <Arduino.h>
#include <ModbusRTU.h>
#define SLAVE_ID 15 //ID that BYD battery pack uses
ModbusRTU mb;

#define RX2_PIN	33
#define TX2_PIN	23
#define SERIAL2BAUDRATE 9600		//9600/4800/2400 BPS ok; if >=19200 fails with Checksum error!
#define SERIAL2BITS	SERIAL_8E1	//8bits,even parity, 1 stop bit
//#define SERIAL2BITS	SERIAL_8N1	//no parity


// =========================================
// ===  Modbus RTU Setup and Callbacks  ====
// =========================================


// *** Holding Registers ***		16 bit	RW	t4
uint16_t callback_getHR(TRegister* reg, uint16_t val) {
	int addr=int(reg->address.address);
	Serial.printf("@mb_callback_GET_HR Reg_Addr:%d Reg_Val:%d\n",int(reg->address.address), int(reg->value));
	return val;
}


uint16_t callback_setHR(TRegister* reg, uint16_t val) {
	int addr=int(reg->address.address);
	Serial.printf("@mb_callback_SET_HR Reg_Addr:%d Reg_OldVal:%d -> Reg_NewVal_new:%d\n",int(reg->address.address), int(reg->value), val );

	if (addr==401)
		val=255;	//<- always returns 255. (Overrides mb write)
	//The master gets an error: "Write output (holding) register failed: Slave device or server failure"

	return val;
}


void load_dummy_reg300s() {
	uint16_t val[]={0,0,128,450,10948,492,0,2503,0,0,2056,0,10,30,0,0,16,22595,0,0,13,52064,50,9900};
	for (int i=0; i<24; i++)
		mb.Hreg(300+i,val[i]);
}


void setup_Modbus_Registers() {
	Serial2.begin(SERIAL2BAUDRATE, SERIAL2BITS, RX2_PIN, TX2_PIN);
  mb.begin(&Serial2);
  mb.slave(SLAVE_ID);

	//t4 - ** RW ** - 16-bit output (holding) register data type (default)
	mb.addHreg(300,0,24);	// add 24 HR, start at 300, init with 0	
	mb.addHreg(1000,0,100);	// add 100 HR, start at 1000, init with 0	
	mb.addHreg(401,255,1);	// add 1 HR, start at 401, init with 255	

	load_dummy_reg300s();

	/* **Question **
	DOES REGISTER 401 need to be reset to 255 after every write? 
	I assume this register is used to receive commands to be executed. 
	Once the commands are complete, the register should be reset to 255 (?)
	*/

	mb.onGetHreg(300, callback_getHR, 24);	//on_get 300..323 execute callback
	mb.onGetHreg(1000, callback_getHR, 100);	//on_get 1000..1099 execute callback
	mb.onGetHreg(401, callback_getHR, 1);	//on_get 401 execute callback
	mb.onSetHreg(401, callback_setHR, 1);	//on_set 401 execute callback
}



// ================================
// ===  Setup and Main Loop    ====
// ================================

void setup() {
	Serial.begin(115200); 
	Serial.printf("\n");
	Serial.printf("Starting...");

	setup_Modbus_Registers();
}


void loop() {
	mb.task();	//modbus task update
}


/*
test commands (from a linux computer with a usb->ttl serial + rs485 transceiver)
read device with: 
-a15 slaveID=15, 
-t4 holding resister (t4), 
-r300 start reference=300, 
-0 First reference is 0 (PDU addressing) :
-1 Poll only once only

READ 24regs start@300:
mbpoll -b 9600 /dev/ttyUSB1 -a15 -t4 -r300 -0 -c24 

READ 1reg start@303:
mbpoll -b 9600 /dev/ttyUSB1 -a15 -t4 -r303 -0 -1 

WRITE val=179 (1reg) start@reg303:
mbpoll -b 9600 /dev/ttyUSB1 -a15 -t4 -r303 -0 179 
*/