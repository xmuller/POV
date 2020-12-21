#pragma once

void init_timer();

int getHours();
int getMinutes();
int getSeconds();

int readTCNT0();

unsigned short time_sec();
unsigned int time_milli();
unsigned long time_micro();