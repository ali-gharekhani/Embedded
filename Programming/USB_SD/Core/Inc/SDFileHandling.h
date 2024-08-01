#ifndef SDFILEHANDLINH_H
#define SDFILEHANDLING_H

#include "main.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"


void mount_sd(void);
void unmount_sd(void);
FRESULT sd_write_file(char *name, char *data);
FRESULT sd_read_file(char *name);
FRESULT sd_create_file(char *name);
FRESULT sd_update_file(char *name, char *data);
FRESULT sd_remove_file(char *name);
FRESULT sd_create_directory(char *name);
void sd_check_space(void);


#endif