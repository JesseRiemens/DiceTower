/*
 * Copyright (c) 2023 Jamie McCrae
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/auxdisplay.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(auxdisplay_sample, LOG_LEVEL_DBG);

int main(void) {
  int rc;
  const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(auxdisplay_0));
  uint8_t data[6] = "Hello";

  if (!device_is_ready(dev)) {
    LOG_ERR("Auxdisplay device is not ready.");
    return 0;
  }

  rc = auxdisplay_cursor_set_enabled(dev, false);

  if (rc != 0) {
    LOG_ERR("Failed to enable cursor: %d", rc);
  }

  // snprintk(data, sizeof(data), "Hello world from %s", CONFIG_BOARD);
  while (true) {

    rc = auxdisplay_write(dev, data, strlen(data));

    if (rc != 0) {
      LOG_ERR("Failed to write data: %d", rc);
    }
    LOG_INF("Sent data: %s", data);

    // rc =
    //     auxdisplay_cursor_position_set(dev, AUXDISPLAY_POSITION_ABSOLUTE, 0,
    //     0);
    // if (rc != 0) {
    //   LOG_ERR("Failed to set cursor position: %d", rc);
    // }

    k_sleep(K_MSEC(1000));
  }

  LOG_INF("Sample application has completed.");
  return 0;
}