//Jumping Light 

#define NUM_LIGHTS 6

//Set up pins
int input1 = 2;
int input2 = 4;
int led1 = 3;
int led2 = 5;
int led3 = 6;
int led4 = 9;
int led5 = 10;
int led6 = 11;

unsigned long debounce_time = 500;  //Time between jumps to activate

unsigned long turn_on_time = 1024; // Time to fade on Light
unsigned long turn_off_time = 4864; // Time to fade off light

unsigned long rampUpStep = turn_on_time / 256;
unsigned long rampDownStep = turn_off_time / 256; 

// Arrays for storing 
int lights[NUM_LIGHTS];
int light_vals[NUM_LIGHTS];
int light_rampUp[NUM_LIGHTS];
int light_rampDown[NUM_LIGHTS];
unsigned long time[NUM_LIGHTS];


int light_in_use1;
int light_in_use2;

unsigned long last_jump1;
unsigned long last_jump2;
unsigned long dt;


bool serial_debug = true;    

void setup(){
  
  Serial.begin(9600);                   

  lights[0] = led1;
  lights[1] = led2;
  lights[2] = led3;
  lights[3] = led4;
  lights[4] = led5;
  lights[5] = led6;


  pinMode(input1, INPUT_PULLUP);
  pinMode(input2, INPUT_PULLUP);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  
  analogWrite(led1,0);
  analogWrite(led2,0);
  analogWrite(led3,0);
  analogWrite(led4,0);
  analogWrite(led5,0);
  analogWrite(led6,0);

  //Initialise arrays

  for(int i = 0; i < 6; i++){
    light_vals[i] = 0;
    light_rampUp[i] = 0;
    light_rampDown[i] = 1;
    time[i] = millis();

  }

  last_jump1 = millis();
  last_jump2 = millis();

  //indicate which light to use first
  light_in_use1 = 2;
  light_in_use2 = 5;


}

void loop(){
  
  // read first jump switch and check debounce  
  if (digitalRead(input1)==LOW && millis() - last_jump1 > debounce_time)
  {
    int prevLight = light_in_use1;

    light_rampDown[light_in_use1]  = 1;

    light_in_use1++;
    if(light_in_use1 > 2)
      light_in_use1 = 0;

    light_rampUp[light_in_use1] = 1;
    light_rampDown[light_in_use1] = 0;

    if(serial_debug){
      Serial.print(light_vals[prevLight]);
      Serial.print("\t1\t");
      Serial.print(lights[light_in_use1]);
      Serial.print("\t");
      Serial.println(millis());
    }

    last_jump1 = millis();   

  }
  

  if (digitalRead(input2)==LOW && millis() - last_jump2 > debounce_time)
  {
    int prevLight = light_in_use2;

    light_rampDown[light_in_use2]  = 1;

    light_in_use2++;
    if(light_in_use2 > 5)
      light_in_use2 = 3;

    light_rampUp[light_in_use2] = 1;
    light_rampDown[light_in_use2] = 0;

    if(serial_debug){
      Serial.print(light_vals[prevLight]);
      Serial.print("\t2\t");
      Serial.print(lights[light_in_use2]);
      Serial.print("\t");
      Serial.println(millis());  
    }
    
    last_jump2 = millis();   
  }


  for(int i=0; i < NUM_LIGHTS; i++){
  
    dt = millis() - time[i];

    if(light_rampUp[i] && dt >= rampUpStep){
        light_vals[i] += 1;
        time[i] = millis();
    }

    if(!light_rampUp[i] && dt >= rampDownStep){
      light_vals[i] -= 1;
      if(light_rampDown[i] == 1){
        light_vals[i] -= 5;
      }
      time[i] = millis();
    }

    if(light_vals[i] > 254){
      light_vals[i] = 255;
      light_rampUp[i] = 0;

    }


    if(light_vals[i] < 1){
      light_vals[i] = 0;
      //light_rampDown[0] = 0;
     
    }

    analogWrite(lights[i], light_vals[i]);  
  }
  

}

   
 
