//Jumping Light 

#define NUM_LIGHTS 3

//Set up pins
int input1 = 2;
int led1 = 3;
int led2 = 5;
int led3 = 6;

unsigned long debounce_time = 500;  //Time between jumps to activate

unsigned long turn_on_time = 1024; // Time to fade on Light
unsigned long turn_off_time = 10240; // Time to fade off light

unsigned long rampUpStep = turn_on_time / 256;
unsigned long rampDownStep = turn_off_time / 256; 

// Arrays for storing 
int lights[NUM_LIGHTS];
int light_vals[NUM_LIGHTS];
int light_rampUp[NUM_LIGHTS];
int light_rampDown[NUM_LIGHTS];
unsigned long time[NUM_LIGHTS];


int state;

unsigned long last_jump1;
unsigned long last_jump2;
unsigned long dt;

bool pressed = false;

bool serial_debug = true;    


void setup(){
  
  Serial.begin(9600);                   

  lights[0] = led1;
  lights[1] = led2;
  lights[2] = led3;


  pinMode(input1, INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  
  analogWrite(led1,0);
  analogWrite(led2,0);
  analogWrite(led3,0);

  //Initialise arrays

  for(int i = 0; i < NUM_LIGHTS; i++){
    light_vals[i] = 0;
    light_rampUp[i] = 0;
    light_rampDown[i] = 0;
    time[i] = millis();

  }

  last_jump1 = millis() - debounce_time;

  //indicate which light to use first
  state = 0;


}

void loop(){
  
  // read first jump switch and check debounce  
  if (digitalRead(input1)==HIGH && millis() - last_jump1 > debounce_time && !pressed)
  {

      pressed = true;

      if(state >= NUM_LIGHTS){
        //state = 0;
        for(int i =0; i < NUM_LIGHTS; i++){
          light_rampDown[i] = 1;

        }
      }


      if(state == 2){
        light_vals[0] = 254;
        light_vals[1] = 254;
        light_vals[2] = 254;
        state = 0 ;
      }
      
      else if(state == 1){
        light_vals[0] = 254;
        light_vals[1] = 254;
        light_vals[2] = 0;

        state = 2;
      }

      else if(state == 0){
        light_vals[0] = 254;
        light_vals[1] = 0;
        light_vals[2] = 0;

        state = 1;
      }
      
  
    

    if(serial_debug){
      Serial.print(state);
      Serial.print("\t");
      Serial.print(light_vals[state]);
      Serial.print("\t");
      Serial.println(millis());
    }

    last_jump1 = millis();   

  }
  
  if(digitalRead(input1)==LOW){
    pressed = false;
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
      //light_rampUp[i] = 0;

    }


    if(light_vals[i] < 1){
      light_vals[i] = 0;
      light_rampDown[i] = 0;
     
    }
    if(light_vals[0] == 0){
      state = 0;
    }

    analogWrite(lights[i], light_vals[i]);  
  }
  

}

   
 
