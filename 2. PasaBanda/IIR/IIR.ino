
#define DEBUG_OFF  0
#define DEBUG_ON   1

#define debug DEBUG_ON

#define debug_message(fmt,...)          \
  do {              \
    if (debug)          \
       Serial.printf (fmt, ##__VA_ARGS__);     \
  } while(0)


#define DAC1 25
const int ADCPin = 34;
const byte test_pin = 32;

#define Tsample 100 //in useg

int ADCValue = 0;

hw_timer_t * timer = NULL;

double iir(int J, int K, double *h, double *j, double *w, double *z, double  x);


//#include "../coef.h"

#define M 1
double h[] = {0.05, 0, -0.05};


#define N 2
double j[] = {1.5371322893124, -0.9025};


double w[M+1];
double z[N];
double y;
int i;





void IRAM_ATTR onTimer() {      // Timer interrupt



         dacWrite(DAC1,(uint8_t)(y));
         ADCValue = analogRead(ADCPin); //3 Nibbles (12 bits ADC)
         ADCValue>>=4;                  //Keep Most significant byte
         ADCValue-=128;                 //Substract DC component

         digitalWrite(test_pin, HIGH); 
         y = iir(M, N, h, j, w, z, ADCValue); 
         digitalWrite(test_pin, LOW); 
    
         y+=128;                 //Add DC component
      

  
}







void setup() {

  Serial.begin(115200);
  delay(1000);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, Tsample, true);
  timerAlarmEnable(timer);
 
  //w = (double *) calloc(M+1, sizeof(double));
  pinMode(test_pin, OUTPUT);

 
  
}

void loop() {

 
   
 
}


/*=====================================
* fir.c - FIR filter in direct form 
*=====================================
* Usage: y = fir(M, h, w, x); 
*
* h = filter impulse response
* w = state
* x = input sample
* M = filter order
* y = Output Sample
*/
                  
                       

double iir(int J, int K, double *h, double *j, double *w, double *z, double  x) 
{   

      int i;
      double y;                             /* output sample */

       w[0] = x;                             /* read current input sample \(x\) */

       for (y=0, i=0; i<=J; i++)
              y += h[i] * w[i];              /* compute current output sample \(y\) */

       for (i = 0; i < K; i++) {            // Parte recursiva
          y += j[i] * z[i];
       }
       
       for (i=J; i>=1; i--)                  /* update states for next call */
              w[i] = w[i-1];                 /* done in reverse order */

       for (i=K; i>=1; i--)                  /* update states for next call */
              z[i] = z[i-1];                 /* done in reverse order */
              
       z[0] = y;

       return y;
}
