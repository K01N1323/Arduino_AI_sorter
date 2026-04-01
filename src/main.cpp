#include <Arduino.h>
#include <Arduino_OV767X.h>

OV767X camera;

static uint16_t Buffer[160 * 120];

void setup() {
  Serial.begin(115200);

  while (!Serial)
    ;

  if (!camera.begin(QQVGA, RGB565, 1)) {
    Serial.println("Инициация камеры ......");
    while (1)
      ;
  }

  Serial.println("Все необходимые объекты инициализированны. Переходим к "
                 "выполнению алгоритма......");
}

void loop() {
  Serial.println("Делаем фото....");
  camera.readFrame((uint8_t *)Buffer);
  Serial.println("Фото сделано");

  long sumX = 0;
  long sumY = 0;
  int count = 0;

  static float FilteredX = 80;
  static float FilteredY = 60;
  float alfa = 0.2;

  Serial.println("Все параметры инициализированы");

  for (int y = 0; y < 120; y++) {
    for (int x = 0; x < 160; x++) {
      uint16_t pixel = Buffer[160 * y + x];

      int r = ((pixel >> 11) & 0x1F) * 8;
      int g = ((pixel >> 5) & 0x3F) * 4;
      int b = (pixel & 0x1F) * 8;

      if ((r > 1.2 * g) && (r > 1.2 * b) && (r > 100)) {
        count++;
        sumX += x;
        sumY += y;
      }
    }
  }

  if (count < 5) {
    Serial.println("Красного объекта нет в кадре.....");
  } else {

    int avgX = sumX / count;
    int avgY = sumY / count;

    FilteredX = avgX * alfa + (1 - alfa) * FilteredX;
    FilteredY = avgY * alfa + (1 - alfa) * FilteredY;

    Serial.print("Filtered X: ");
    Serial.print((int)FilteredX);
    Serial.print(" | Filtered Y: ");
    Serial.println((int)FilteredY);
  }

  delay(2000);
}