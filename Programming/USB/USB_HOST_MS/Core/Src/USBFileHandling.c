#include "USBFileHandling.h"

extern FATFS USBHFatFS;
extern FIL USBHFile;
extern char USBHPath[4];

FRESULT usb_fresult;
FILINFO usbhinfo;
UINT usb_br, usb_bw;

FATFS *pUSBHFatFS;
DWORD usb_fre_clust;
uint32_t usb_total_space, usb_free_space;

void mount_usb(void)
{
  usb_fresult = f_mount(&USBHFatFS, USBHPath, 1);
  if (usb_fresult != FR_OK){ /* Error Handling of USB Mounting*/}
  else {/* Error Handling of USB Mounting*/}
}


void unmount_usb(void)
{
  usb_fresult = f_mount(NULL, USBHPath, 1);
  if (usb_fresult != FR_OK) {/* Error Handling of USB Unmounting*/}
  else {/* Error Handling of USB Unmounting*/}
}


FRESULT usb_write_file(char *name, char *data)
{
  usb_fresult = f_stat(name, &usbhinfo);
  if (usb_fresult != FR_OK) {/* Error Handling of Existing File*/ return usb_fresult;}
  else
  {
    usb_fresult = f_open(&USBHFile, name, FA_OPEN_EXISTING | FA_WRITE);
    if (usb_fresult != FR_OK) {/* Error Handling of Openning File*/ return usb_fresult;}
    else
    {
      usb_fresult = f_write(&USBHFile, data, strlen(data), &usb_bw);
      if (usb_fresult != FR_OK) {/* Error Handling of Writing File */}
      else {/* Error Handling of Writing File*/}
      
      usb_fresult = f_close(&USBHFile);
      if (usb_fresult != FR_OK) {/* Error Handling of Closing File*/}
      else{/* Error Handling of Closing File*/}
    }
    return usb_fresult; 
  }
}


FRESULT usb_read_file(char *name)
{
  usb_fresult = f_stat(name, &usbhinfo);
  if (usb_fresult != FR_OK) {/* Error Handling of Existing File*/ return usb_fresult;}
  else
  {
    usb_fresult = f_open(&USBHFile, name, FA_READ);
    if (usb_fresult != FR_OK) {/* Error Handling of Openning File*/}
    else {/* Error Handling of Openning File*/}
    
    char *read_buffer = malloc(sizeof(f_size(&USBHFile)));
    usb_fresult = f_read(&USBHFile, read_buffer, f_size(&USBHFile), &usb_br);
    if (usb_fresult != FR_OK) {/* Error Handling of Reading File*/}
    else
    {
      // Do what ever you want with buffer
      free(read_buffer);
      
      usb_fresult = f_close(&USBHFile);
      if (usb_fresult != FR_OK) {/* Error HAndling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
    
    return usb_fresult;
  }
}


FRESULT usb_create_file(char *name)
{
  usb_fresult = f_stat(name, &usbhinfo);
  if (usb_fresult == FR_OK) {/* Error Handling of Existing File*/ return usb_fresult;}
  else
  {
    usb_fresult = f_open(&USBHFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
    if (usb_fresult != FR_OK) {/* Error Handling of Creating File*/ return usb_fresult;}
    else{/* Error Handling of Creating File*/}
    
    usb_fresult = f_close(&USBHFile);
    if (usb_fresult != FR_OK){/* Error Handling of Closing File*/}
    else{/* Error Handling of Closing File*/}
  }
  return usb_fresult;
}


FRESULT usb_update_file(char *name, char *data)
{
  usb_fresult = f_stat(name, &usbhinfo);
  if (usb_fresult != FR_OK) {/* Error Handling of Existing File*/ return usb_fresult;}
  else
  {
    usb_fresult = f_open(&USBHFile, name, FA_OPEN_APPEND | FA_WRITE);
    if (usb_fresult != FR_OK){/* Error Handling of Openning File*/ return usb_fresult;}
    else {/* Error Handling of Openning File*/}
    
    usb_fresult = f_write(&USBHFile, data, strlen(data), &usb_bw);
    if (usb_fresult != FR_OK) {/* Error Handling of Writing File*/}
    else {/* Error Handling of Writing File*/}
    
    usb_fresult = f_close(&USBHFile);
    if (usb_fresult != FR_OK) {/* Error Handling of Closing File*/}
    else {/* Error Handling of Closing File*/}
  }
  return usb_fresult;
}


FRESULT usb_remove_file(char *name)
{
  usb_fresult = f_stat(name, &usbhinfo);
  if (usb_fresult != FR_OK) {/* Error Handling of Existing File*/ return usb_fresult;}
  else 
  {
    usb_fresult = f_unlink(name);
    if (usb_fresult != FR_OK) {/* Error Handling of Removing File*/}
    else {/* Error Handling of Removing File*/}
  }
  return usb_fresult;
}


FRESULT usb_create_dir(char *name)
{
  usb_fresult = f_mkdir(name);
  if (usb_fresult != FR_OK) {/* Error Handling of Creating Directory*/}
  else {/* Error Handling of Creating Directory*/}
  return usb_fresult;
}


void check_usb_details(void)
{
  f_getfree("", &usb_fre_clust, &pUSBHFatFS);
  
  usb_total_space = (uint32_t)((pUSBHFatFS -> n_fatent - 2) * pUSBHFatFS -> csize * 0.5);
  usb_free_space = (uint32_t)(usb_fre_clust * pUSBHFatFS -> csize * 0.5);
}






