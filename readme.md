# Dispcolor Component    

#### Add as submodule:

`git submodule add https://github.com/liyanboy74/dispcolor.git components/dispcolor`

##### Select LCD Type:

Edit `#define DISPCOLOR_type` in `dispcolor.h` file , or Define `DISPCOLOR_type`=`1` or `2` or `...`

### Adapt to ESP-IDF

###### Example for GC9a01 LCD:

Clone  [GC9A01 Component](https://github.com/liyanboy74/gc9a01-esp-idf) 

Edit `CMakeLists.txt` file and edit patch of  header `gc9a01.h` as below example:

```
set(LCD_Header_Dir "../gc9a01")
```


##### Example Test:

```c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "dispcolor.h"

#define STACK_SIZE              2048

void LCD(void * arg)
{
    dispcolor_Init(240,240);

    dispcolor_FillCircleWu(120,120,100,YELLOW);
    dispcolor_Update();

    for(;;)
    {
    }
}

void app_main(void)
{
    TaskHandle_t LCDHandle;

    xTaskCreate(LCD,"Test LCD",STACK_SIZE,NULL,tskIDLE_PRIORITY,&LCDHandle);
    configASSERT(LCDHandle);
}

```
### Adapt to BMPC & GCC

Clone  [BMPC Component](https://github.com/liyanboy74/bmpc) 

You may need to edit the following functions :

```c
void dispcolor_Init(uint16_t Width, uint16_t Height);
void dispcolor_Update();
void dispcolor_SetBrightness(uint8_t Value);
void dispcolor_DrawPixel(int16_t x, int16_t y, uint16_t color);
void dispcolor_FillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
uint16_t dispcolor_GetPixel(int16_t x, int16_t y);
```
##### Example Test:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dispcolor.h"

int main(int argc, char** argv)
{
    dispcolor_Init(500,500);
    dispcolor_FillCircle(250,250,150,BLUE);
    dispcolor_Update();
}

```

