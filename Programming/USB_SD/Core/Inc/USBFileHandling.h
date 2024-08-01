#ifndef USBFILEHANDLING_H_
#define USBFILEHANDLING_H_

#include "fatfs.h"
#include "main.h"
#include "string.h"

void mount_usb(void);
void unmount_usb(void);
FRESULT usb_write_file(char *name, char *data);
FRESULT usb_read_file(char *name);
FRESULT usb_create_file(char *name);
FRESULT usb_update_file(char *name, char *data);
FRESULT usb_remove_file(char *name);
FRESULT usb_create_dir(char *name);
void check_usb_details(void);

#endif 