/*
 * vim:ts=4:sw=4:expandtab
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

#include <linux/input.h>

int main(int argc, char *argv[]) {
  if (getuid() != 0) {
    errx(EXIT_FAILURE, "run as root");
  }
  if (argc != 2) {
    errx(EXIT_FAILURE, "syntax: %s /dev/input/eventN", argv[0]);
  }
  int fd = open(argv[1], O_RDWR);
  if (fd < 0) {
    err(EXIT_FAILURE, "open(%s)", argv[1]);
  }

  if (ioctl(fd, EVIOCGRAB, 1) < 0) {
    err(EXIT_FAILURE, "Grabbing evdev keyboard device failed");
  }
  
  struct input_event ev;
  ssize_t size;
  struct input_event on = {
    .type = EV_LED,
    .value = MSC_PULSELED,
    .code = LED_CAPSL,
  };
  struct input_event off = {
    .type = EV_LED,
    .value = !MSC_PULSELED,
    .code = LED_CAPSL,
  };
  while (true) {
    size = read(fd, &ev, sizeof(struct input_event));
    if (size < sizeof(struct input_event)) {
      errx(EXIT_FAILURE, "short read: got %" PRIi32 ", want %" PRIu32, (int32_t)size, (uint32_t)sizeof(struct input_event));
    }
    if (ev.type != EV_KEY) {
      continue; // not EV_KEY
    }
    if (ev.code != 58 /* MSG_CAPSLOCK */) {
      continue; // not caps lock
    }
    if (ev.value != 1) {
      continue; // depressed
    }
    // turn on LED when caps lock gets pressed
    write(fd, &on, sizeof(struct input_event));
    printf("MSG_CAPSLOCK processed\n");
    write(fd, &off, sizeof(struct input_event));    
  }
}
