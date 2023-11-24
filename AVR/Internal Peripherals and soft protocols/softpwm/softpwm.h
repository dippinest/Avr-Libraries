
// ===============================================================================
//
// Библиотека для реализации программной ШИМ.
//
// У данной библиотеке отсутствует файл реализации функций
// (файл с расширением *.c). Все функции являются встраиваемыми (inline)
// с целью повышения быстродействия, поскольку программная ШИМ является
// достаточно тяжёлой для процессора, так как занимает много процессорного времени
//
// -------------------------------------------------------------------------------
//
// A library for implementing software PWM.
//
// This library does not have a function implementation file
// (a file with the extension *.c). All functions are inline in order
// to increase performance, since the software PWM is quite heavy enough
// for the processor, since it takes a lot of processor time
//
// ===============================================================================


#ifndef SOFTPWM_H_
#define SOFTPWM_H_


#include <avr/io.h>

#include <stdlib.h>


// ===============================================================================


#ifndef T
#define T(P) ((uint8_t*)&P)
#endif


typedef struct
{
	uint8_t *softpwm_channel_port;
	uint8_t  softpwm_channel_pin;
	
	uint8_t  softpwm_channel_max_width;
	uint8_t  softpwm_channel_duty_cycle;
	uint8_t  softpwm_channel_duty_cycle_buf;
	
	uint8_t  softpwm_channel_counter;

} SOFTPWM_t;


// ===============================================================================


inline SOFTPWM_t SOFTPWM_Get_Channel_Object(uint8_t *port, uint8_t pin, uint8_t max_width, uint8_t duty_cycle)
{
	SOFTPWM_t pwm;
	
	pwm.softpwm_channel_port           = port;
	pwm.softpwm_channel_pin            = pin;
	
	pwm.softpwm_channel_max_width      = max_width;
	pwm.softpwm_channel_duty_cycle     = duty_cycle;
	pwm.softpwm_channel_duty_cycle_buf = duty_cycle;
	
	pwm.softpwm_channel_counter        = 0;
	
	return pwm;
}


// ===============================================================================


#define SOFTPWM_DUTY_CYCLE(T) (T.softpwm_channel_duty_cycle)


// ===============================================================================


inline void SOFTPWM_Channel_Processing(SOFTPWM_t *channel)
{
	if (channel->softpwm_channel_counter == channel->softpwm_channel_max_width)
	{
		*(channel->softpwm_channel_port) |=  (1 << channel->softpwm_channel_pin);
		
		channel->softpwm_channel_duty_cycle_buf = channel->softpwm_channel_duty_cycle;
		
		channel->softpwm_channel_counter = 0;
	}
	
	if (channel->softpwm_channel_counter >= channel->softpwm_channel_duty_cycle_buf)
	{
		*(channel->softpwm_channel_port) &= ~(1 << channel->softpwm_channel_pin);
	}
	
	++(channel->softpwm_channel_counter);
}

inline void SOFTPWM_All_Channels_Processing(SOFTPWM_t *channels, uint8_t num_of_channels)
{
	for (uint8_t i = 0; i < num_of_channels; ++i)
	{
		SOFTPWM_Channel_Processing(&(channels[i]));
	}
}


#endif

