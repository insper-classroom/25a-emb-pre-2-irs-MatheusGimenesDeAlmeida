#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_RED = 4;
volatile int btn_status = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if (btn_status == 3){ // Segundo clique
      btn_status = 4;
    }
    if (btn_status == 0){ // Primeiro clique
      btn_status = 1;
    }
  } else if (events == 0x8) { // rise edge
    if (btn_status == 0){btn_status = 2;} // Primeira subida
    if (btn_status == 3){btn_status = 5;} // Segunda subida
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(LED_PIN_RED);
  gpio_set_dir(LED_PIN_RED, GPIO_OUT);
  
  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (btn_status == 1){ // Primeiro clique
      btn_status = 0;
      gpio_put(LED_PIN_RED,1);
      printf("fall 1 \n");
    }
    if (btn_status == 2){ // Primeira subida
      btn_status = 3;
      printf("rise 2 \n");
    }
    if (btn_status == 4){ // Segundo clique
      btn_status = 3;
      gpio_put(LED_PIN_RED,0);
      printf("fall 3 \n");
    }
    if (btn_status == 5){
      btn_status = 0;
      printf("rise 4 \n");
    }
  }
}