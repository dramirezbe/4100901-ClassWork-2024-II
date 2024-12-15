#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "uart.h"

int main(void)
{
    configure_systick_and_start();
    configure_gpio();
    
    UART_Init(USART2); // Inicializar UART2

    UART_send_string(USART2, "Hi. Connected.....\r\n");

    // Buffer para recepción
    uint8_t rx_buffer[10];
    volatile uint8_t rx_ready = 0;  // Inicializar la bandera

    // Configurar recepción con interrupción
    UART_receive_it(USART2, rx_buffer, sizeof(rx_buffer));

    while (1) {
        // Verificar si se recibió un mensaje completo
        if (rx_ready) {
            rx_ready = 0;  // Reiniciar bandera

            // Enviar el mensaje recibido de vuelta (echo)
            UART_send_string(USART2, "Received: ");
            UART_send_string(USART2, (char *)rx_buffer);
            UART_send_string(USART2, "\r\n");

            // Configurar nuevamente la recepción
            UART_receive_it(USART2, rx_buffer, sizeof(rx_buffer));
        }

        // Parpadear el LED cada 500 ms
        if (systick_GetTick(COUNTER_A) >= 500) { 
            gpio_toggle_led();         // Toggle LED
            systick_reset(COUNTER_A);  // Reset counter A
        }

        // Enviar mensaje "Still connected..." cada 10,000 ms
        if (systick_GetTick(COUNTER_B) >= 10000) {
            UART_send_string(USART2, "Still connected...\r\n");
            systick_reset(COUNTER_B);  // Reset counter B
        }
    }
}
