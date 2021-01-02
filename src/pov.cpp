#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <common/serial.h>
#include <common/timer.h>
#include <common/led_spi.h>
#include <common/encoder.h>

using namespace pov;

namespace settings {
static constexpr uint16_t RADIUS_MICRO_METER = 0.09e4;
static constexpr uint16_t PIx2 = 3.1415e4 * 2;
}

/**
 * 1. On récupe la position courante
 * 2. On regarde l'état théorique de la led par rapport à la position
 * 3. On update selon cet état
 */
using namespace settings;
//char buf[50];

/**
 * @brief updatePovPosition
 *
 * Timer1 is reset each turn so we estimate the current position based on the time to realise
 * a turn and the current time.
 */
inline void updatePovPosition() {
  auto time_distance_from_origine = timer::getCurrentTime<1>();
  uint32_t speed = RADIUS_MICRO_METER * PIx2 / encoder::time_per_round;
  encoder::current_pov_position = time_distance_from_origine * speed;
}

int main()
{
    led_spi::init();
    serial::init();
    timer::init();
    encoder::init();
    sei();

//    auto time_per_round = 0;
    while (1)
    {
        updatePovPosition();
        if (encoder::current_pov_position > 0 && encoder::current_pov_position < 100 )
          led_spi::setAllLedsUp();
        else
          led_spi::setAllLedsDown();
        led_spi::masterTransmit();


//        sprintf(buf, "position %lu\n", encoder::current_pov_position);

//        _delay_us(500);

//        auto diff = time() - last_time;

//        serial::transmit(buf);
        //setBigNeedle();
    }
    return 0;
}
