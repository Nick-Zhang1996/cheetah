// execute static commands and read output from servo
// print response to terminal

#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

void setup() {
  Serial.begin(9600);
  Serial.println("CAN Write - Testing transmission of CAN Bus messages");
  delay(1000);
  
  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

void loop() 
{
tCAN message;

        message.id = 0x631; //formatted in HEX
        message.header.rtr = 0;
        message.header.length = 8; //formatted in DEC
        message.data[0] = 0x40;
	message.data[1] = 0x05;
	message.data[2] = 0x30;
	message.data[3] = 0xFF; //formatted in HEX
	message.data[4] = 0x00;
	message.data[5] = 0x40;
	message.data[6] = 0x00;
	message.data[7] = 0x00;

mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);
mcp2515_send_message(&message);

delay(1000);
  tCAN message;
if (mcp2515_check_message()) 
	{
    if (mcp2515_get_message(&message)) 
	{
        //if(message.id == 0x620 and message.data[2] == 0xFF)  //uncomment when you want to filter
             //{
               
               Serial.print("ID: ");
               Serial.print(message.id,HEX);
               Serial.print(", ");
               Serial.print("Data: ");
               Serial.print(message.header.length,DEC);
               for(int i=0;i<message.header.length;i++) 
                {	
                  Serial.print(message.data[i],HEX);
                  Serial.print(" ");
                }
               Serial.println("");
             //}
           }}

}
