#include <Arduino.h>
#include <Arduino_OV767X.h>

OV767X camera;

// Выделяем память под ВЕСЬ кадр (160 * 120)
// Используем ключевое слово static, чтобы массив лежал в области данных, а не в
// стеке
static uint16_t frameBuffer[160 * 120];

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  if (!camera.begin(QQVGA, RGB565, 1)) {
    Serial.println("ERR: Camera failed");
    while (1)
      ;
  }
  Serial.println("SUCCESS: Camera is ready.");
}

void loop() {
  Serial.println(">>> Requesting frame...");

  // Теперь у нас буфер нужного размера!
  camera.readFrame((uint8_t *)frameBuffer);

  Serial.println(">>> Frame captured successfully!");

  // Выведем один пиксель из самого центра для проверки
  uint16_t centerPixel = frameBuffer[160 * 60 + 80];
  Serial.print("Center pixel color: 0x");
  Serial.println(centerPixel, HEX);

  // Вычисляем яркость (простое усреднение для начала)
  // RGB565 -> Brightness
  uint16_t r = (centerPixel >> 11) & 0x1F; // Извлекаем 5 бит красного
  uint16_t g = (centerPixel >> 5) & 0x3F;  // Извлекаем 6 бит зеленого
  uint16_t b = centerPixel & 0x1F;         // Извлекаем 5 бит синего

  // Приводим к общему знаменателю (0-255)
  float brightness = (r * 8 + g * 4 + b * 8) / 3.0;

  Serial.print("Current Brightness: ");
  Serial.println(brightness);

  if (brightness < 50) {
    Serial.println("Status: TOO DARK");
  } else {
    Serial.println("Status: LIGHT OK");
  }

  delay(2000);
}