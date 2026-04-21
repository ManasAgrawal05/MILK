



// /////////////////////////////////////////////////////////////////////////////

// #include <ESP32Servo.h>

// #define BAUDRATE 15600  
// #define PARAM_BUFFER_SIZE 10
// enum commandIDs{
//     CMD_START           =   0xDE,       // starts a command, 57005
//     CMD_END             =   0xBE,       // ends a command, 48879
//     CMD_EXTEND          =   0xFFFF,     // allows command to be split
//     CMD_ERROR           =   0xC0DE,     // indicates error in command
//     CMD_PING            =   1,
//     DRIVE_MOTOR         =   2,          // drive single motor (motor_id, position)
//     DRIVE_POSTION       =   3,          // drive both motors to position (position)
//     MIX_DRIVE           =   4,          // drive motors independently (left_position, right_position)
//     TURN_IN_PLACE       =   5,          // turn robot in place (turn_angle_deg)
//     DRIVE_DISTANCE      =   6           // drive both motors forward by distance in mm (distance_mm)
// };

// Servo esc;
// int escPin = 23;  // connect ESC signal wire here
// Servo esc2;
// int escPin2 = 22;

// int escledpin = 2;


// //  Uncomment the line according to your sensor type
// //AS5600L ASL;   //  use default Wire

// //uint8_t dummy[4000];


// //Servo esc;
// //int escPin = 23;  // connect ESC signal wire here
// void parse_command(int* cmd_id, int* paramsa, int* paramsb){
//     Serial.println("Waiting for command...");
//     digitalWrite(2,HIGH);
    


//     // --- wait for start byte ---
//     byte start_byte;
//     while(true){
//         if(Serial.available() >= 1){
//             Serial.readBytes(&start_byte, 1);
//             if(start_byte == CMD_START){
//                 break;
//             }
//         }
//     }

//     Serial.println("Start byte received");
//     digitalWrite(2,LOW);
    


//     // // --- read command ID ---
//     // while(Serial.available() < sizeof(int)) {} 
//     // Serial.readBytes((char*)cmd_id, sizeof(int));
//     // Serial.print("Command ID: ");
//     // Serial.println(*cmd_id);

//     // // --- read number of parameters ---
//     // while(Serial.available() < sizeof(int)) {}
//     // Serial.readBytes((char*)num_params, sizeof(int));
//     // Serial.print("Number of parameters: ");
//     // Serial.println(*num_params);

//     // // --- read parameters ---
//     // for(int i = 0; i < *num_params; i++){
//     //     while(Serial.available() < sizeof(int)) {}
//     //     Serial.readBytes((char*)&params[i], sizeof(int));
//     // }
//     // Serial.println("Parameters received");
//     // for(int i = 0; i < *num_params; i++){
//     //     Serial.print("Param ");
//     //     Serial.print(i);
//     //     Serial.print(": ");
//     //     Serial.println(params[i]);
//     // }

//     while(Serial.available() < 1) {} 
//     Serial.readBytes((uint8_t*)cmd_id, 1);
//     Serial.print("Command recieved");
//     Serial.println(*cmd_id);
    
//     while(Serial.available() < sizeof(int)) {} 
//     Serial.readBytes((char*)paramsa, sizeof(int));
//     Serial.print("Data 1");
//     Serial.println(*paramsa);
    
//     while(Serial.available() < sizeof(int)) {} 
//     Serial.readBytes((char*)paramsb, sizeof(int));
//     Serial.print("Data 2");
//     Serial.println(*paramsb);

//     // --- read end byte ---
//     byte end_byte;
//     while(Serial.available() < 1) {}  // only 1 byte now
//     Serial.readBytes(&end_byte, 1);

//     // if(end_byte != CMD_END){
//     //     *cmd_id = CMD_ERROR;
//     //     Serial.println("Error: Invalid end byte");
//     // }
//     Serial.println("End byte received");
    
// }


// TaskHandle_t serialIO;
// void serialIOCode(void * pvParameters);


// void setup()
// {
//   pinMode(2,OUTPUT);
//   //while(!Serial);
//   Serial.begin(115200);
//   Serial.println();
//   Serial.println(__FILE__);
//   //esc.attach(escPin, 1000, 2000); // min/max pulse width in µs
//   //esc.writeMicroseconds(1500);    //
//   delay(2000);
//   Serial.println("");   

//   Serial.begin(115200);
//   Serial.println();
//   Serial.println(__FILE__);

//   Serial.println();





//   //ASL.setAddress(0x38);



//   esc.attach(escPin, 1000, 2000); // min/max pulse width in µs
//   esc2.attach(escPin2, 1000, 2000);
//   esc.writeMicroseconds(1500);    // neutral
//   esc2.writeMicroseconds(1500);
//   delay(2000); 
//                 // wait for ESC to arm


//   xTaskCreatePinnedToCore(
//     serialIOCode, /* Task function. */
//     "serialIO", /* name of task. */
//     4096, /* Stack size of task */
//     NULL, /* parameter of the task */
//     1, /* priority of the task */
//     &serialIO, /* Task handle to keep track of created task */
//     0);          /* pin task to core 0 */


// }



// // accumulator
// ////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////


// // loop for serial communication later

// ////////////////////////////////////////////////////////////
// void serialIOCode( void * pvParameters ){
//   for(;;){
    

//   static bool printed = false;
//     if(!printed) {
//         Serial.println("Drivetrain Firmware Initialized");
//         printed = true;
//     }

//     int cmd_id;
    
//     int params_a;  // buffer for parameters
//     int params_b;
    

//     // begin parsing incoming commands
//     parse_command(&cmd_id, &params_a, &params_b);
//     Serial.println(params_a);
//     Serial.println(params_b);
//     if (params_a != 1500){
//         digitalWrite(2,HIGH);
//     }
//     else{
//         digitalWrite(2,LOW);
//     }
    

//     esc2.writeMicroseconds(params_a);    
//     esc.writeMicroseconds(params_b);
//     vTaskDelay(10 / portTICK_PERIOD_MS); 
//   }
// }
// /////////////////////////////////////////////////////////////



// //supposed to be some kind of writer for the code

// void loop()
// {
    

//     // int i = 200;
//     // esc2.writeMicroseconds(1500-i);    // neutral
//     // esc.writeMicroseconds(1500+i);
//     // delay(5000);

//     // esc2.writeMicroseconds(1500+i);    // neutral
//     // esc.writeMicroseconds(1500-i);
//     // delay(5000);

//     // i = 200;
//     // esc2.writeMicroseconds(1500-i);    // neutral
//     // esc.writeMicroseconds(1500+i);
//     // delay(3000);


//     // while (true) {}
    

// }
