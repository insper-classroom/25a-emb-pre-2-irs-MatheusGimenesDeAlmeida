#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_RED = 4;
const int LED_PIN_GREEN = 6;
volatile int btn_status_red = 0;
volatile int btn_status_green = 0;


void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        if (gpio == BTN_PIN_R){
            if (btn_status_red == 3){ btn_status_red = 4;} // Segundo clique
            if (btn_status_red == 0){btn_status_red = 1;}} // Primeiro clique
        if (gpio == BTN_PIN_G){
            if (btn_status_green == 3){ btn_status_green = 4;} // Segundo clique
            if (btn_status_green == 0){btn_status_green = 1;}} // Primeiro clique
        }
    else if (events == 0x8) { // rise edge
        if (gpio == BTN_PIN_R){
            if (btn_status_red == 0){btn_status_red = 2;} // Primeira subida
            if (btn_status_red == 3){btn_status_red = 5;}}// Segunda subida
        if (gpio == BTN_PIN_G){
            if (btn_status_green == 0){btn_status_green = 2;} // Primeira subida
            if (btn_status_green == 3){btn_status_green = 5;}}// Segunda subida
        }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(LED_PIN_RED);
  gpio_init(LED_PIN_GREEN);
  gpio_set_dir(LED_PIN_RED, GPIO_OUT);
  gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
  
    // callback led r (first)
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);


  while (true) {
    if (btn_status_red == 1){ // Primeiro clique
      btn_status_red = 0;
      gpio_put(LED_PIN_RED,1);
      printf("fall 1 \n");}

    if (btn_status_red == 2){ // Primeira subida
      btn_status_red = 3;
      printf("rise 2 \n");}

    if (btn_status_red == 4){ // Segundo clique
      btn_status_red = 3;
      gpio_put(LED_PIN_RED,0);
      printf("fall 3 \n");}

    if (btn_status_red == 5){ // Segunda subida
      btn_status_red = 0;
      printf("rise 4 \n");}

      if (btn_status_green == 1){ // Primeiro clique
        btn_status_green = 0;
        printf("fall 1 \n");}
  
      if (btn_status_green == 2){ // Primeira subida
        btn_status_green = 3;
        gpio_put(LED_PIN_GREEN,1);
        printf("rise 2 \n");}
  
      if (btn_status_green == 4){ // Segundo clique
        btn_status_green = 3;
        printf("fall 3 \n");}
  
      if (btn_status_green == 5){ // Segunda subida
        btn_status_green = 0;
        gpio_put(LED_PIN_GREEN,0);
        printf("rise 4 \n");}
  }
}