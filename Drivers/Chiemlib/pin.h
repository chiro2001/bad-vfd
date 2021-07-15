/*
 * pin.h
 *
 *  Created on: Jul 2, 2021
 *      Author: Chiro
 */

#ifndef CHIEMLIB_PIN_H_
#define CHIEMLIB_PIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"

#ifndef u8
typedef uint8_t u8;
#endif

#ifndef u16
typedef uint16_t u16;
#endif

#ifndef u32
typedef uint32_t u32;
#endif

#ifdef __cplusplus
class Pin {
public:
	GPIO_TypeDef *port = nullptr;
	u16 pin = 0;
	Pin() = default;
	explicit Pin(GPIO_TypeDef *port_, u16 pin_) :
			port(port_), pin(pin_) {
	}
};

class PinOut: public Pin {
public:
	PinOut() = default;
	PinOut(GPIO_TypeDef *port_, u16 pin_);

	void set();

	void reset();

	void set_value(u8 value);
};
#endif

#ifdef __cplusplus
}
#endif

#endif /* CHIEMLIB_PIN_H_ */
