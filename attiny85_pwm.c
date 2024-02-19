//WinAvr compatibility
//PWM of ATTiny85, test for PWM to other waveforms
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>
#define NOINIT __attribute__ ((section (".noinit")))

int Wave NOINIT;
unsigned int Freq NOINIT;
int8_t Sinewave[256] NOINIT;
volatile unsigned int Acc, Jump;

void SetupPWM() {

   PLLCSR = 1<<PCKE | 1<<PLLE;
  TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS00); 
    DDRB |= (1 << DDB0);
}

void CalculateSine () {
  int X=0, Y=8180;
int i;
   for (i=0; i<256; i++) {
    X = X + (Y*4)/163;
    Y = Y - (X*4)/163;
    Sinewave[i] = X>>6;
  }
}
 
void Sine () {
  Acc = Acc + Jump;
  OCR1A = Sinewave[Acc>>8] + 128;
}

typedef void (*wavefun_t)();
wavefun_t Waves[1] = {Sine};	
wavefun_t Wavefun;


ISR(TIMER0_COMPA_vect) {
    Wavefun(); 
}

int main() {
	SetupPWM();
	CalculateSine();
	Wave = 0;
	Freq = 100;
	Wavefun = Waves[Wave];
	Jump = Freq * 4;
	sei(); 
while (1) {

}

return 0;
}

/*void setup() {
  Wire.begin();
  // Is it a power-on reset?
    Wave = 0; Freq = 100;     // Start with 100Hz Sine
    CalculateSine();
    InitDisplay();
    ClearDisplay();

  else Wave = (Wave+1) % nWaves;
  Wavefun = Waves[Wave];
  MCUSR = 0;
  SetupDDS();
  SetupRotaryEncoder();
  Jump = Freq*4;
  PlotFreq(Freq, 1, 7);
  PlotIcon(Wave, 1, 0);
}
 
// Everything done by interrupts
void loop() {
}*/
