#include "pin.h"

//explicit Pin::Pin(GPIO_TypeDef *port_, u16 pin_) :
//		port(port_), pin(pin_) {
//}

//PinOut::PinOut() = default;

PinOut::PinOut(GPIO_TypeDef *port_, u16 pin_) :
		Pin(port_, pin_) {
}

void PinOut::set() {
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void PinOut::reset() {
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void PinOut::set_value(u8 value) {
	if (value)
		set();
	else
		reset();
}
