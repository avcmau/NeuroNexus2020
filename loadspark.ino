#include "HX711.h"

#define calibration_factor 17850.0 //This value is obtained using the SparkFun_HX711_Calibration sketch


#define LOADCELL_DOUT_PIN_1  4
#define LOADCELL_SCK_PIN_1  5
#define LOADCELL_DOUT_PIN_2  6
#define LOADCELL_SCK_PIN_2  7
#define LOADCELL_DOUT_PIN_3  8
#define LOADCELL_SCK_PIN_3  9
#define LOADCELL_DOUT_PIN_4  10
#define LOADCELL_SCK_PIN_4  11


HX711 scale1;
HX711 scale2;
HX711 scale3;
HX711 scale4;

int mvm = 12;
int mvm1 = 2;

void setup() {
  Serial.begin(9600);
 // Serial.println("HX711 scale demo");
  pinMode(mvm,OUTPUT);
  pinMode(mvm1,OUTPUT);

  scale1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
  
  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  
  scale3.begin(LOADCELL_DOUT_PIN_3, LOADCELL_SCK_PIN_3);
  
  scale4.begin(LOADCELL_DOUT_PIN_4, LOADCELL_SCK_PIN_4);
  
  scale1.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
   scale2.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch

   scale3.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch

   scale4.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  
  scale1.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale2.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
 scale3.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
 scale4.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
}

void loop() {
  const float alpha = 0.5;
  double data_filtered_1[] = {0, 0};
  double data_filtered_2[] = {0, 0};
  double data_filtered_3[] = {0, 0};
  double data_filtered_4[] = {0, 0};
  const int n = 1;
  
      float a = scale1.get_units();
      float b = scale2.get_units();
      float c = scale3.get_units();
      float d = scale4.get_units();

      // Low Pass Filter
    data_filtered_1[n] =   alpha * a + (1 - alpha) * data_filtered_1[n-1];
    data_filtered_1[n-1] = data_filtered_1[n];
    data_filtered_2[n] =   alpha * b + (1 - alpha) * data_filtered_2[n-1];
    data_filtered_2[n-1] = data_filtered_2[n];
    data_filtered_3[n] = alpha * c + (1 - alpha) * data_filtered_3[n-1];
    data_filtered_3[n-1] = data_filtered_3[n];
    data_filtered_4[n] = alpha * d + (1 - alpha) * data_filtered_4[n-1];
    data_filtered_4[n-1] = data_filtered_4[n];

//    Serial.print(b);
//    Serial.print(" ");
//    Serial.println(data_filtered_2[n]);
      float top = a+b;
      float bottom = c+d;
      float right = b+d;
      float left = a+c;

        if((top - bottom>10) && (top - left>10) && (top -right>10))
      {
        Serial.println("Top");
      }
       else if(bottom - top>10 && bottom - left>10 && bottom -right>10)
      {
        Serial.println("Bottom");
      }
       else if(left - bottom>10 && left - top>10 && left -right>10)
      {
        Serial.println("Left");
      }
       else if(right - bottom>10 && right - left>10 && right -top>10)
      {
        Serial.println("Right");
      }  
      else
      {
        Serial.println("Centre");   
      }

    float F1 = data_filtered_4[n];
    float F2 = data_filtered_2[n];
    float F3 = data_filtered_1[n];
    float F4 = data_filtered_3[n];
    float L = 25.5; // in c metres (length)
    float W = 23.5;// in c metres  (width)

//*****Disk motor vibration with breaks*********//////
//    if(micros()%5==0){
//    digitalWrite(2,HIGH);
//    digitalWrite(12,HIGH);
//    }
//    else
//    {
//    digitalWrite(2,LOW);
//    digitalWrite(12,LOW);
//    }

//**************C.O.P CALCULATION******************************
if(a>2 || b>2 || c>2 || d>2){   
    
    int Xcop = ((F1+F2-F3-F4)*(L/2))/(F1+F2+F3+F4);
    int Ycop = ((F2+F3-F1-F4)*(W/2))/(F1+F2+F3+F4);
   // Serial.print("DATA,TIME,");
   // Serial.print(millis());
    //Serial.print(",");
    //Serial.print(Xcop);
    //Serial.print(",");
    //Serial.println(Ycop);
}

//*******PRINT INDIVIDUAL WEIGHTS***********************
//  Serial.print("Cell a ");
//  Serial.println(scale1.get_units(), 1); //scale.get_units() returns a float
// //Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
//  Serial.print("Cell b ");
//  Serial.println(scale2.get_units(), 1);
//  Serial.print("Cell c ");
//  Serial.println(scale3.get_units(), 1);
//  Serial.print("Cell d ");
//  Serial.println(scale4.get_units(), 1);
//  delay(1000);
}
