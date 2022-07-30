
#include <mcp_can.h>
#include <SPI.h>
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

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
  delay(100);

  unsigned char data[8] = {0};
  data[0] = 0xFF;
  data[1] = 0xFF;
  data[2] = 0xFF;
  data[3] = 0xFF;
  data[4] = 0xFF;
  data[5] = 0xFF;
  data[6] = 0xFF;
  data[7] = 0xFC;
  Serial.println("sending motor enable");
  CAN.sendMsgBuf(0x01, 0, 8, data);
  delay(100);

}

unsigned char data[8] = {0};
void loop(){
  unsigned char len = 0;
  unsigned char buf[8];
  /*
  data[0] = 0xFF;
  data[1] = 0x01;
  data[2] = 0x00;
  data[3] = 0x24;
  data[4] = 0x00;
  data[5] = 0x80;
  data[6] = 0x0;
  data[7] = int(millis()/1000)%254;
  */
  data[0] = 0x00;
  data[1] = 0x00;
  data[2] = 0x00;
  data[3] = 0x00;
  data[4] = 0x00;
  data[5] = 0x00;
  data[6] = 0x00;
  data[7] = 0x00;
  CAN.sendMsgBuf(0x01, 0, 8, data);
  /*
  Serial.print("sending motor pos: ");
  Serial.println(data[7]);
  */

  if (CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(0x01,&len, buf);
    Serial.print("len = ");
    Serial.println(len);
    for (int i=0; i<len; i++){
      Serial.print(buf[i], HEX);
      Serial.print("\t");
    }
    Serial.println();
  }
  
}
