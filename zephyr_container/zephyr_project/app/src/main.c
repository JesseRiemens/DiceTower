/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <string.h>

#include <app_version.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#define STACK_SIZE 1024

// Thread declarations
K_THREAD_STACK_DEFINE(encoder_thread_stack_area, STACK_SIZE);
K_THREAD_STACK_DEFINE(lcd_thread_stack_area, STACK_SIZE);

struct k_thread encoder_thread;
struct k_thread lcd_thread;

void encoder_entry(void*, void*, void*);
void lcd_entry(void*, void*, void*);

// GPIO callback
void gpio_callback(const struct device *dev, struct gpio_callback *cb,
                   uint32_t pins);

int main(void) {
  printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

  // Create threads
  k_tid_t encoder_tid = k_thread_create(
      &encoder_thread, encoder_thread_stack_area, STACK_SIZE, encoder_entry,
      NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

  k_tid_t lcd_tid =
      k_thread_create(&lcd_thread, lcd_thread_stack_area, STACK_SIZE, lcd_entry,
                      NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

  // Add callback to GPIO
  const struct device *gpio_dev = device_get_binding("GPIO_0");
  if (!gpio_dev) {
    printk("Cannot find GPIO_0!\n");
    return -ENODEV;
  }

  static struct gpio_callback gpio_cb;
  gpio_init_callback(&gpio_cb, gpio_callback, BIT(0));
  gpio_add_callback(gpio_dev, &gpio_cb);
}

// Incremental encoder thread>
void encoder_entry(void* a, void* b, void* c) {
  while (1) {
    k_msleep(1234);
    printk("Encoder thread\n");
  }
}

// LCD thread
void lcd_entry(void* a, void* b, void* c) {
  while (1) {
    k_msleep(4321);
    printk("LCD thread\n");
  }
}

// GPIO callback
void gpio_callback(const struct device *dev, struct gpio_callback *cb,
                   uint32_t pins) {
  printk("GPIO callback\n");
}
