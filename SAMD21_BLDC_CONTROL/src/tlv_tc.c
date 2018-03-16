/*
 * tlv_tc.c
 *
 * Created: 07/03/2018 14:25:19
 *  Author: Alberto
 */ 

#include "tlv_tc.h"

//! [module_inst]
struct tc_module tc4_instance;
//! [module_inst]

//! [callback_funcs]
void tlv_tc_callback_to_change_duty_cycle(
struct tc_module *const module_inst)
{
	//if(tlv_read()) tlv_reset();
	//for(int i=0; i<60000; i++);
	static uint8_t error_count = 0;
	//port_pin_toggle_output_level(PIN_PB30);
	uint8_t error = tlv_read();
	//port_pin_set_output_level(PIN_PB30, !error);
	error_count = error_count + error;
	if (error_count > 10){
		tlv_reset();
		for(int i=0; i<60000; i++){
			for(int i=0; i<10; i++);
		}
		error_count = 0;
		port_pin_set_output_level(PIN_PB30, false);
	}
	
	
}
//! [callback_funcs]

//! [setup]
void tlv_tc4_configure(void)
{
	//! [setup_config]
	struct tc_config config_tc4;
	//! [setup_config]
	//! [setup_config_defaults]
	tc_get_config_defaults(&config_tc4);
	//! [setup_config_defaults]

	config_tc4.clock_source = GCLK_GENERATOR_1;
	config_tc4.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
	//! [setup_change_config]
	config_tc4.counter_size    = TC_COUNTER_SIZE_8BIT;
	config_tc4.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config_tc4.counter_8_bit.period = 0xFF;
	config_tc4.counter_8_bit.compare_capture_channel[0] = 0x3F;
	//! [setup_change_config]

	//! [setup_change_config_pwm]
	config_tc4.pwm_channel[0].enabled = true;
	config_tc4.pwm_channel[0].pin_out = PWM4_OUT_PIN_0;
	config_tc4.pwm_channel[0].pin_mux = PWM4_OUT_MUX_0;
	//! [setup_change_config_pwm]

	//! [setup_set_config]
	tc_init(&tc4_instance, PWM4_MODULE, &config_tc4);
	//! [setup_set_config]

	//! [setup_enable]
	tc_enable(&tc4_instance);
	//! [setup_enable]
}

void tlv_tc4_configure_callbacks(void)
{
	//! [setup_register_callback]
	tc_register_callback(
	&tc4_instance,
	tlv_tc_callback_to_change_duty_cycle,
	TC_CALLBACK_CC_CHANNEL0);
	//! [setup_register_callback]

	//! [setup_enable_callback]
	tc_enable_callback(&tc4_instance, TC_CALLBACK_CC_CHANNEL0);
	//! [setup_enable_callback]
}