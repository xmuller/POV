#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <common/serial.h>
#include <common/timer.h>
#include <common/led_spi.h>
#include <common/encoder.h>

using namespace pov;

/**
 * 1. On récupe la position courante
 * 2. On regarde l'état théorique de la led par rapport à la position
 * 3. On update selon cet état
 */
char buf[50];

/**
 * @brief updatePovPosition
 *
 * Timer1 is reset each turn so we estimate the current position based on the time to realise
 * a turn and the current time.
 */
inline void updatePovPosition() {
  encoder::current_pov_position = timer::getCurrentTime<1>() * encoder::current_pov_speed;
}

int main()
{
    sei();
    led_spi::init();
    serial::init();
    timer::init();
    encoder::init();

//    led_spi::setAllLedsDown();
    led_spi::masterTransmit();


//    auto time_per_round = 0;
    while (1)
    {
        uint16_t time_hours_activation = encoder::time_per_round - encoder::time_per_round / 12 * (timer::getHours() + 1);
        uint16_t time_minutes_activation = encoder::time_per_round - encoder::time_per_round / 60 * (timer::getMinutes() + 15);
        uint16_t time_secondes_activation = encoder::time_per_round - encoder::time_per_round / 60 * timer::getSeconds();
        auto current_time = timer::getCurrentTime<1>();
        bool activate_hours =  current_time > time_hours_activation - 40 && current_time < time_hours_activation + 40;
        bool activate_min = current_time > time_minutes_activation - 20 && current_time < time_minutes_activation + 20;
        bool activate_sec = current_time > time_secondes_activation - 10 && current_time < time_secondes_activation + 10;



        uint16_t time_sec_bis_activation = (current_time > encoder::time_per_round/2) ? (time_secondes_activation + encoder::time_per_round/2)
                                                                                      : (time_secondes_activation - encoder::time_per_round/2);
        bool activate_sec_bis = current_time >  time_sec_bis_activation - 10 && current_time < time_sec_bis_activation + 10;
        if (activate_min || activate_hours || activate_sec || activate_sec_bis) {
//        if (false) {
          if (activate_hours)
            led_spi::leds_state = 0b1111111111;
          if (activate_min)
            led_spi::leds_state = 0b1111111111111;
          if (activate_sec)
            led_spi::leds_state = 0b1111111111111;
          if (activate_sec_bis)
            led_spi::leds_state = 0b11111;
        }
        else {
          led_spi::setAllLedsDown();
        }
        led_spi::setLedUp(15);
//        led_spi::setAllLedsDown();
        led_spi::masterTransmit();

//        _delay_ms(1);
//        led_spi::setLedUp(15);// = led_spi::leds_state | (1 << 2);

//        led_spi::masterTransmit();

//        sprintf(buf, "position %hu\n", time_hours_activation);

//        _delay_ms(1);

//        auto diff = time() - last_time;

//        serial::transmit(buf);
        //setBigNeedle();
    }
    return 0;
}
