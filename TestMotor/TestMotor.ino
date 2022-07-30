#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

// TODO int pin exist?
#define CAN_INT 2                              // Set INT to pin 2
MCP_CAN CAN(10);                               // Set CS to pin 10

void setup(){
  // enable Serial
  Serial.begin(115200);

  // enable CAN BUS
  // Initialize MCP2515 running at 16MHz with a baudrate of 1Mb/s and the masks and filters disabled.
  // TODO add filtering
  if(CAN.begin(MCP_ANY, CAN_1000KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  // Set operation mode to normal so the MCP2515 sends acks to received data.
  // TODO check behavior in original code
  CAN.setMode(MCP_NORMAL);

  pinMode(CAN_INT, INPUT);

  // enable motors
  int motor_can_id[N_MOTORS] = {1};
  init_motors(ids);                           // Initialize the list of motors
  // TODO
  enable_motor(&motors[0]);             // Enable first motor
  sleep(1000);

}

void loop(){
  if(!digitalRead(CAN_INT)){
    readCanbus();
  }

  // update position setpoint
  motors[0].control.p_des = 0;
  motors[0].control.kd = .5f;
  motors[0].control.kp = 2.0f;

  pack_cmd(&motors[0]);
  Serial.print("Motor target: ");
  Serial.print(motors[0].control.p_des);
  Serial.println();

}

// read from canbus
void readCanbus(){
  // Read data: len = data length, buf = data byte(s)
  CAN.readMsgBuf(&rxId, &len, rxBuf);      
  
  // TODO hardcode this
  if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
    sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
  else
    sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

  Serial.print(msgString,HEX);

  if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
    sprintf(msgString, " REMOTE REQUEST FRAME");
    Serial.print(msgString);
  } else {
    for(byte i = 0; i<len; i++){
      sprintf(msgString, " 0x%.2X", rxBuf[i]);
      Serial.print(msgString);
    }
  }
}

byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

bool sendTxMsg(MotorStruct* motor){
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  return CAN_OK = CAN.sendMsgBuf(motor->txMsg.id, 0, motor->txMsg.len, motor->txMsg.data);
}
