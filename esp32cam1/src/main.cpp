#include <Arduino.h>
#include "esp_camera.h"
#include <TFT_eSPI.h>
//ESP32-CAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#include <EEPROM.h>
#include "camera_pins.h"
TFT_eSPI tft = TFT_eSPI(); 

#define SCREEN_WIDTH 160     //定义显示屏幕的像素
#define SCREEN_HEIGHT 120 
 

static camera_config_t camera_config = {
  .pin_pwdn = PWDN_GPIO_NUM,
  .pin_reset = RESET_GPIO_NUM,
  .pin_xclk = XCLK_GPIO_NUM, 
  .pin_sscb_sda = SIOD_GPIO_NUM,  
  .pin_sscb_scl = SIOC_GPIO_NUM,
     
  .pin_d7 = Y9_GPIO_NUM,
  .pin_d6 = Y8_GPIO_NUM,
  .pin_d5 = Y7_GPIO_NUM, 
  .pin_d4 = Y6_GPIO_NUM,
  .pin_d3 = Y5_GPIO_NUM,
  .pin_d2 = Y4_GPIO_NUM,  
  .pin_d1 = Y3_GPIO_NUM,
  .pin_d0 = Y2_GPIO_NUM,
  .pin_vsync = VSYNC_GPIO_NUM,
  .pin_href = HREF_GPIO_NUM,
  .pin_pclk = PCLK_GPIO_NUM,

  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  
  .pixel_format = PIXFORMAT_RGB565,
  .frame_size = FRAMESIZE_QQVGA,
  .jpeg_quality = 12,
  .fb_count = 1,
};

esp_err_t camera_init(){
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        return err;
    }
    sensor_t * s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV2640_PID) {
        s->set_vflip(s, 1);//flip it back
        s->set_brightness(s, 1);//up the blightness just a bit
        s->set_contrast(s, 1);
        // s->set_vflip(s, 1);
        // s->set_hmirror(s, 1);
    }
 
    return ESP_OK;
}
 camera_fb_t *fb ;

void  display(void *param_type)
{
 uint16_t i=0;
  
   while(1)
   {
camera_fb_t *fc=fb;
  for(uint8_t y = 0; y <SCREEN_HEIGHT; y ++)
    {
      for(uint8_t x = 0; x <SCREEN_WIDTH; x ++)
      {
        //RGB565格式 120*160
         tft.drawPixel(x,y,((fc->buf[2*i])<<8)|(fc->buf[2*i+1]));
        i++;
       }
    }
   
   }
   
     /* code */
}


void camera_show(void )
{
  
    /* code */
    uint16_t i=0;
    camera_fb_t *fb = esp_camera_fb_get();
 
    for(uint8_t y = 0; y <SCREEN_HEIGHT; y ++)
    {
      for(uint8_t x = 0; x <SCREEN_WIDTH; x ++)
      {
        //RGB565格式 120*160
         tft.drawPixel(x,y,((fb->buf[2*i])<<8)|(fb->buf[2*i+1]));
        i++;
       }
    }
    
     esp_camera_fb_return(fb);
   

}

uint16_t sys_time;
void setup(void)
 {

//  xTaskCreatePinnedToCore(camera_show,    //具体实现的函数
//                             "APP_CPU_LOOP",  //任务名称
//                             8192,       //堆栈大小
//                             NULL,       //输入参数
//                             1,          //任务优先级
//                             NULL,       //
//                             0          //核心  0\1
//                             );

// xTaskCreatePinnedToCore( display,    //具体实现的函数
//                             "APP_CPU_LOOP",  //任务名称
//                             8192*120*160,       //堆栈大小
//                             NULL,       //输入参数
//                             1,          //任务优先级
//                             NULL,       //
//                             1          //核心  0\1
//                             );

      Serial.begin(115200);
      camera_init();
      tft.begin();
      tft.initDMA();
      tft.setRotation(1);//横屏0 1 2 3
    
    
      if(psramFound())
      {
        camera_config.frame_size = FRAMESIZE_UXGA;
        camera_config.jpeg_quality = 10;
        camera_config.fb_count = 2;
      } else {
        camera_config.frame_size = FRAMESIZE_SVGA;
        camera_config.jpeg_quality = 12;
        camera_config.fb_count = 1;
      }
      tft.setCursor(0, 0);
      tft.setTextColor(0xF800);
      tft.setTextSize(1);
      tft.setTextWrap(true);
      
      // tft.fillScreen(TFT_BLACK);//黑色
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      tft.init(240);
      tft.fillScreen(0xFFFF);
  }

void loop() 
{
 sys_time = millis();
  camera_show();
  //display( );
  sys_time = millis() - sys_time; 
 // tft.setCursor(10, 10);
 // tft.print(1000.0/(long)sys_time);
  Serial.printf("www");
  Serial.println(1000.0/(long)sys_time);
  
}