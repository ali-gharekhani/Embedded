#include "SDFileHandling.h"

extern FATFS SDFatFS;
extern FIL SDFile;
extern char SDPath[4];

FILINFO sdinfo;
FRESULT sd_fresult;
UINT sd_br, sd_bw;

FATFS *pfs;
DWORD sd_fre_clust;
uint32_t sd_total, sd_free_space;

void mount_sd(void)
{
  sd_fresult = f_mount(&SDFatFS, SDPath, 1);
  if (sd_fresult != FR_OK) {/* Error Handling of Mounting SD*/}
  else{/* Error Handling of Mounting*/}
}


void unmount_sd(void)
{
  sd_fresult = f_mount(NULL, SDPath, 1);
  if (sd_fresult != FR_OK) {/* Error Handling of Unmounting SD*/}
  else {/* Error Handling of Unmounting*/}
}


FRESULT sd_write_file(char *name, char *data)
{
  sd_fresult = f_stat(name, &sdinfo);
  if (sd_fresult != FR_OK){/* Error Handling of Existing File*/ return sd_fresult;}
  else
  {
    sd_fresult = f_open(&SDFile, name, FA_OPEN_EXISTING | FA_WRITE);
    if (sd_fresult != FR_OK){/* Error Handling of Openning File*/; return sd_fresult;}
    else
    {
      sd_fresult = f_write(&SDFile, data, strlen(data), &sd_bw);
      if (sd_fresult != FR_OK){/* Error Handling of Writing File*/}
      else {/* Error Handling of Writing File*/}
      
      sd_fresult = f_close(&SDFile);
      if (sd_fresult != FR_OK) {/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
    return sd_fresult;
  }
}


FRESULT sd_read_file(char *name)
{
  sd_fresult = f_stat(name, &sdinfo);
  if (sd_fresult != FR_OK){/* Error Handl;ing of Existing File*/ return sd_fresult;}
  else 
  {
    sd_fresult = f_open(&SDFile, name, FA_READ);
    if (sd_fresult != FR_OK){/* Error Handling of Openning File*/ return sd_fresult;}
    else 
    {
      char *read_buffer = malloc(sizeof(f_size(&SDFile)));;
      sd_fresult = f_read(&SDFile, read_buffer, f_size(&SDFile), &sd_br);
      if(sd_fresult != FR_OK) {/* Error Handling of Reading File*/}
      else {/* Error Handling of Reading File*/}
      
      free(read_buffer);
      sd_fresult = f_close(&SDFile);
      if (sd_fresult != FR_OK){/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
    return sd_fresult;
  }
}


FRESULT sd_create_file(char *name)
{
  sd_fresult = f_stat(name, &sdinfo);
  if (sd_fresult == FR_OK) {/* Error Handling of Existing File*/ return sd_fresult;}
  else
  {
    sd_fresult = f_open(&SDFile, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
    if (sd_fresult != FR_OK){/* Error Handling of Creating File*/ return sd_fresult;}
    else {/* Error Handling of Creating File*/}
    
    sd_fresult = f_close(&SDFile);
    if (sd_fresult != FR_OK){/* Error Handling of Closing File*/}
    else {/* Error Handling of Closing File*/}
  }
  return sd_fresult;
}


FRESULT sd_update_file(char *name, char *data)
{
  sd_fresult = f_stat(name, &sdinfo);
  if (sd_fresult != FR_OK){/* Error Handling of Existing File*/ return sd_fresult;}
  else
  {
    sd_fresult = f_open(&SDFile, name, FA_OPEN_APPEND | FA_WRITE);
    if (sd_fresult != FR_OK){/* Error Handling of Openning File*/ return sd_fresult;}
    else
    {
      sd_fresult = f_write(&SDFile, data, strlen(data), &sd_bw);
      if (sd_fresult != FR_OK){/* Error Handling of Writing File*/}
      else {/* Error Handling of Writing File*/}
      
      sd_fresult = f_close(&SDFile);
      if (sd_fresult != FR_OK){/* Error Handling of Closing File*/}
      else {/* Error Handling of Closing File*/}
    }
  }
  return sd_fresult;
}


FRESULT sd_remove_file(char *name)
{
  sd_fresult = f_stat(name, &sdinfo);
  if (sd_fresult != FR_OK){/* Error Handling of Existing File*/ return sd_fresult;}
  else
  {
    sd_fresult = f_unlink(name);
    if (sd_fresult != FR_OK) {/* Error Handling of Removing File*/}
    else {/* Error Handling of Removing File*/}
  }
  return sd_fresult;
}


FRESULT sd_create_directory(char *name)
{
  sd_fresult = f_mkdir(name);
  if (sd_fresult != FR_OK){/* Error Handling of Creating Directory*/}
  else {/* Error Handling of Creating Directory*/}
  
  return sd_fresult;
}


void sd_check_space(void)
{
  f_getfree("", &sd_fre_clust, &pfs);
  sd_total = (uint32_t)((pfs -> n_fatent - 2) * pfs -> csize * 0.5);
  sd_free_space = (uint32_t)(sd_fre_clust * pfs -> csize * 0.5);
}








