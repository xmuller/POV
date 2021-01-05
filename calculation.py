import math

F_CPU = 13000000
TIMER1_PRESCALER = 64
MAX_SIZE_REGISTER = 65535

TICKS_PER_TURN = 10500
TICKS_MAIN_LOOP = 23

MAX_COUNT_TIMER1 = 1 / (F_CPU / MAX_SIZE_REGISTER / TIMER1_PRESCALER)

DISTANCE_FIRST_LED = 0.015
DISTANCE_LAST_LED = 0.09


def ticksToSec(ticks):
    return ticks * MAX_COUNT_TIMER1 / MAX_SIZE_REGISTER


def ticksToLength(ticks, length):
    return (ticks / TICKS_PER_TURN) * (2 * math.pi * length)


def radToTicks(rad):
    return TICKS_PER_TURN / (math.pi * 2) * rad


def lengthToTicks(length, dist):
    return radToTicks((length / dist))


imprecision_arc_min = (
    2 * math.pi / (TICKS_PER_TURN / TICKS_MAIN_LOOP) * DISTANCE_FIRST_LED
)
imprecision_arc_max = (
    2 * math.pi / (TICKS_PER_TURN / TICKS_MAIN_LOOP) * DISTANCE_FIRST_LED
)
print(
    f"Max timer1: {MAX_COUNT_TIMER1}, resolution: {MAX_COUNT_TIMER1/MAX_SIZE_REGISTER}"
)
print("Loop time: {}".format(ticksToSec(TICKS_MAIN_LOOP)))
print(f"Imprecision dessin arc [{imprecision_arc_min}, {imprecision_arc_max}]")
print("RPM pov: {}".format((1 / (TICKS_PER_TURN / (F_CPU / TIMER1_PRESCALER)) * 60)))

print("LED1:")
print(" - CIRCUMFERENCE = {}".format(math.pi * 2 * DISTANCE_FIRST_LED))
print(
    " - speed rad/ticks = {}".format(
        (2 * math.pi * DISTANCE_FIRST_LED) / TICKS_PER_TURN
    )
)
print(
    " - distance between two loop = {}".format(
        ticksToLength(TICKS_MAIN_LOOP, DISTANCE_FIRST_LED)
    )
)
print(" - Length to tick 0.05: {}".format(lengthToTicks(0.01, DISTANCE_FIRST_LED)))

print("LED16:")
print(" - CIRCUMFERENCE = {}".format(math.pi * 2 * DISTANCE_LAST_LED))
print(
    " - speed rad/ticks = {}".format((2 * math.pi * DISTANCE_LAST_LED) / TICKS_PER_TURN)
)
print(
    " - distance between two loop = {}".format(
        ticksToLength(TICKS_MAIN_LOOP, DISTANCE_LAST_LED)
    )
)
print(" - Length to tick 0.05: {}".format(lengthToTicks(0.005, DISTANCE_LAST_LED)))
