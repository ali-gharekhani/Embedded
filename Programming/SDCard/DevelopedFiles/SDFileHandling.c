#include "SDFileHandling.h"

extern FATFS SDFatFS;
extern FIL SDFile;
extern char SDPath[4];

FILINFO file_info;
FRESULT fresult;
UINT br, bw;

FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

void mount_sd(void)
{
  fresult = f_mount(&SDFatFS, SDPath, 1);
  if (fresult != FR_OK) {/* Error Handling of Mounting SD*/}
  else{/* Error Handling of Mounting*/}
}


void unmount_sd(void)
{
  fresult = f_mount(NULL, SDPath, 1);
  if (fresult != FR_OK) {/* Error Handling of Unmounting SD*/}
  else {/* Error Handling of Unmounting*/}
}


FRESULT sd_write_file(char *name, char *data)
{
  fresult = f_stat(name, &file_info);
  if (fresult != FR_OK){/* Error Handling of Existing File*/ return fresult;}
  else
  {
    fresult = f_open(&SDFile, name, FA_OPEN_EXISTING | FA_WRITE);
    if (fresult != FR_OK){/* Error Handling of Openning File*/; return fresult;}
    else
    {
      fresult = f_write(&SDFile, data, strlen(data), &bw);
      if (fresult != FR_OK){/* Error Handling of Writing File*/}
      else {/* Error Handling of Writing File*/}
      
      fresult = f_close(&SDFile);
      if (fresult != FR_OK) {/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
    return fresult;
  }
}


FRESULT sd_read_file(char *name)
{
  fresult = f_stat(name, &file_info);
  if (fresult != FR_OK){/* Error Handl;ing of Existing File*/ return fresult;}
  else 
  {
    fresult = f_open(&SDFile, name, FA_READ);
    if (fresult != FR_OK){/* Error Handling of Openning File*/ return fresult;}
    else 
    {
      char *read_buffer = malloc(sizeof(f_size(&SDFile)));;
      fresult = f_read(&SDFile, read_buffer, f_size(&SDFile), &br);
      if(fresult != FR_OK) {/* Error Handling of Reading File*/}
      else {/* Error Handling of Reading File*/}
      
      free(read_buffer);
      fresult = f_close(&SDFile);
      if (fresult != FR_OK){/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
    return fresult;
  }
}


FRESULT sd_create_file(char *name)
{
  fresult = f_stat(name, &file_info);
  if (fresult == FR_OK) {/* Error Handling of Existing File*/ return fresult;}
  else
  {
    fresult = f_open(&SDFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
    if (fresult != FR_OK){/* Error Handling of Creating File*/ return fresult;}
    else {/* Error Handling of Creating File*/}
    
    fresult = f_close(&SDFile);
    if (fresult != FR_OK){/* Error Handling of Closing File*/}
    else {/* Error Handling of Closing File*/}
  }
  return fresult;
}


FRESULT sd_update_file(char *name, char *data)
{
  fresult = f_stat(name, &file_info);
  if (fresult != FR_OK){/* Error Handling of Existing File*/ return fresult;}
  else
  {
    fresult = f_open(&SDFile, name, FA_OPEN_APPEND | FA_WRITE);
    if (fresult != FR_OK){/* Error Handling of Openning File*/ return fresult;}
    else
    {
      fresult = f_write(&SDFile, data, strlen(data), &bw);
      if (fresult != FR_OK){/* Error Handling of Writing File*/}
      else {/* Error Handling of Writing File*/}
      
      fresult = f_close(&SDFile);
      if (fresult != FR_OK){/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
  }
  return fresult;
}


FRESULT sd_remove_file(char *name)
{
  fresult = f_stat(name, &file_info);
  if (fresult != FR_OK){/* Error Handling of Existing File*/ return fresult;}
  else
  {
    fresult = f_unlink(name);
    if (fresult != FR_OK) {/* Error Handling of Removing File*/}
    else {/* Error Handling of Removing File*/}
  }
  return fresult;
}


FRESULT sd_create_directory(char *name)
{
  fresult = f_mkdir(name);
  if (fresult != FR_OK){/* Error Handling of Creating Directory*/}
  else {/* Error Handling of Creating Directory*/}
  
  return fresult;
}


void sd_check_space(void)
{
  f_getfree("", &fre_clust, &pfs);
  total = (uint32_t)((pfs -> n_fatent - 2) * pfs -> csize * 0.5);
  free_space = (uint32_t)(fre_clust * pfs -> csize * 0.5);
}








