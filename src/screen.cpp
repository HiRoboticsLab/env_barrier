#include "screen.h"

Screen::Screen() {
  display = Adafruit_SSD1306(128, 64, &Wire);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
}

void Screen::title() {
  display.drawBitmap(26, 0, chs_zhi, 16, 16, 1);
  display.drawBitmap(46, 0, chs_neng, 16, 16, 1);
  display.drawBitmap(66, 0, chs_dao, 16, 16, 1);
  display.drawBitmap(86, 0, chs_zha, 16, 16, 1);
  display.display();
}

void Screen::write(int16_t x, int16_t y, String str) {
  //   display.setTextSize(1, 2);  //设置字体大小
  display.setTextSize(1);  //设置字体大小
  display.setTextColor(WHITE);
  display.setCursor(x, y);  //设置显示位置
  display.println(str);     //输出字符
  display.display();
}

void Screen::menu() {
  write(4, 36, "  id :");
  write(4, 50, "state:");
}

void Screen::setId(String arg) {
  //涂改带
  display.writeFillRect(50, 36, 64, 8, BLACK);
  write(50, 36, arg);
}

void Screen::setState(String arg) {
  //涂改带
  display.writeFillRect(50, 50, 64, 8, BLACK);
  write(50, 50, arg);
}