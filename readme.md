# Dispcolor ESP-IDF Component    

Clone to `components` folder beside [GC9A01 Component](https://github.com/liyanboy74/gc9a01-esp-idf)  .

Edit `CMakeLists.txt` file and edit patch of  header `gc9a01.h` as below example:

```
set(GC9A01_Header_Dir "../gc9a01/include")
```

**Add as submodule:**

`git submodule add https://github.com/liyanboy74/dispcolor-esp-idf.git components/dispcolor`

**Example Test:**

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

