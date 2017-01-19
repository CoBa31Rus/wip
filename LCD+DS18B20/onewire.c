/*
 * 1wire.c
 *
 *  Created on: 08.11.2016
 *      Author: coba31rus
 */

#include "onewire.h"

// Устанавливает низкий уровень на шине 1-wire
inline void onewire_low() {
  ONEWIRE_DDR |= _BV(ONEWIRE_PIN_NUM);
}

// "Отпускает" шину 1-wire
inline void onewire_high() {
  ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN_NUM);
}

// Читает значение уровня на шине 1-wire
inline uint8_t onewire_level() {
  return ONEWIRE_PIN & _BV(ONEWIRE_PIN_NUM);
}

// Выдаёт импульс сброса (reset), ожидает ответный импульс присутствия.
// Если импульс присутствия получен, дожидается его завершения и возвращает 1, иначе возвращает 0
uint8_t onewire_reset()
{
  onewire_low();
  _delay_us(640); // Пауза 480..960 мкс
  onewire_high();
  _delay_us(2); // Время необходимое подтягивающему резистору, чтобы вернуть высокий уровень на шине
  // Ждём не менее 60 мс до появления импульса присутствия;
  for (uint8_t c = 80; c; c--) {
    if (!onewire_level()) {
      // Если обнаружен импульс присутствия, ждём его окончания
      //while (!onewire_level()) {} // Ждём конца сигнала присутствия
    	_delay_ms(2);
      return 1;
    }
    _delay_us(1);
  }
  return 0;
}


// Отправляет один бит
// bit отправляемое значение, 0 - ноль, любое другое значение - единица
void onewire_send_bit(uint8_t bit) {
  onewire_low();
  if (bit) {
    _delay_us(5); // Низкий импульс, от 1 до 15 мкс (с учётом времени восстановления уровня)
    onewire_high();
    _delay_us(90); // Ожидание до завершения таймслота (не менее 60 мкс)
  } else {
    _delay_us(90); // Низкий уровень на весь таймслот (не менее 60 мкс, не более 120 мкс)
    onewire_high();
    _delay_us(5); // Время восстановления высокого уровеня на шине + 1 мс (минимум)
  }
}

// Отправляет один байт, восемь подряд бит, младший бит вперёд
// b - отправляемое значение
void onewire_send(uint8_t b) {
  for (uint8_t p = 8; p; p--) {
    onewire_send_bit(b & 1);
    b >>= 1;
  }
}

// читает значение бита, передаваемое уйстройством.
// Возвращает 0 - если передан 0, отличное от нуля значение - если передана единица
uint8_t onewire_read_bit() {
  onewire_low();
  _delay_us(2); // Длительность низкого уровня, минимум 1 мкс
  onewire_high();
  _delay_us(8); // Пауза до момента сэмплирования, всего не более 15 мкс
  uint8_t r = onewire_level();
  _delay_us(80); // Ожидание до следующего тайм-слота, минимум 60 мкс с начала низкого уровня
  return r;
}

// Читает один байт, переданный устройством, младший бит вперёд, возвращает прочитанное значение
uint8_t onewire_read() {
  uint8_t r = 0;
  for (uint8_t p = 8; p; p--) {
    r >>= 1;
    if (onewire_read_bit())
      r |= 0x80;
  }
  return r;
}
/*
// Выполняет последовательность инициализации (reset + ожидает импульс присутствия)
// Если импульс присутствия получен, выполняет команду SKIP ROM
// Возвращает 1, если импульс присутствия получен, 0 - если нет
uint8_t onewire_skip() {
  if (!onewire_reset())
    return 0;
  onewire_send(0xCC);
  return 1;
}

// Выполняет последовательность инициализации (reset + ожидает импульс присутствия)
// Если импульс присутствия получен, выполняет команду READ ROM, затем читает 8-байтовый код устройства
//    и сохраняет его в буфер по указателю buf, начиная с младшего байта
// Возвращает 1, если код прочитан, 0 - если импульс присутствия не получен
uint8_t onewire_read_rom(uint8_t * buf) {
  if (!onewire_reset())
    return 0;
  onewire_send(0x33);
  for (uint8_t p = 0; p < 8; p++) {
    *(buf++) = onewire_read();
  }
  return 1;
}

// Выполняет последовательность инициализации (reset + ожидает импульс присутствия)
// Если импульс присутствия получен, выполняет команду MATCH ROM, и пересылает 8-байтовый код
//   по указателю data (младший байт вперёд)
// Возвращает 1, если импульс присутствия получен, 0 - если нет
uint8_t onewire_match(uint8_t * data) {
  if (!onewire_reset())
    return 0;
  //onewire_send(0x55);
  for (uint8_t p = 0; p < 8; p++) {
    onewire_send(*(data++));
  }
  return 1;
}
*/

int readt(void){
	char temp[2];
	int temper = 0x00;
	if (onewire_reset()){
		onewire_send(0xCC);
		onewire_send(0x44);
		delay_ms(10);
		onewire_reset();
		onewire_send(0xCC);
		onewire_send(0xBE);
		temp[0] = onewire_read();
		temp[1] = onewire_read();
		temper = (((int)temp[1] << 8)|(int)temp[0]);
		return temper;
	}
	else
		return 0x8000;
}
