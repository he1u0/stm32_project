#include "sys.h"
#include "led.h"

int main()
{
    delay_init();
    LED_Init();
    while (1)
    {
        LED0 = LED_ON;
        delay_ms(1000);
        LED0 = LED_OFF;
        delay_ms(1000);
    }
    return 0;
}
