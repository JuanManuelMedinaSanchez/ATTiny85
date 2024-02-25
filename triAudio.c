/*Audio generation.
Triangular - Sine Signal ATinny85
Juan Medina -> juansth@gmail.com*/

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t counter = 0;
volatile uint8_t direction = 1; // Dirección de la onda triangular

void setupTriangularWave() {
    // Configurar el pin PB0 (OC0A) como salida
    DDRB |= (1 << DDB0);

    // Configurar Timer/Counter0 para generar una señal triangular en PB0 (OC0A)
    TCCR0A = (1 << COM0A1) | (1 << WGM00); // Modo PWM no inverso, fase correcta
    TCCR0B = (1 << CS00);  // Sin preescalador
    TIMSK |= (1 << OCIE0A); // Habilitar interrupción de comparación A
    OCR0A = 0; // Inicializar el valor de comparación
}

int main() {
    setupTriangularWave();

    sei(); // Habilitar interrupciones globales

    while (1) {} // Bucle infinito

    return 0;
}

ISR(TIMER0_COMPA_vect) {
    if (direction == 1) {
        // Incrementar el contador
        counter++;
        if (counter == 255) {
            // Cambiar la dirección cuando alcanza el límite superior
            direction = -1;
        }
    } else {
        // Decrementar el contador
        counter--;
        if (counter == 0) {
            // Cambiar la dirección cuando alcanza el límite inferior
            direction = 1;
        }
    }
    
    // Establecer el valor del registro de comparación para controlar la salida
    OCR0A = counter;
}
