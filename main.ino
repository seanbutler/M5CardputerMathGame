/**
 * @file quiz.ino
 * @author Sean Butler (butlersean@gmail.com)
 * @brief Maths Quiz Game Like Little Professor
 * @version 0.1
 * @date 2023-10-27
 *
 */

#include "M5Cardputer.h"

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
    M5Cardputer.Display.setTextSize(1);
}

int global_app_state = 0;
int row_height = 24;

int a, b, c;
String question = "";
String answer = "";
String goal = "";


void loop() {
    M5Cardputer.update();

    switch ( global_app_state ) {
      case 0:
        M5Cardputer.Display.setTextDatum(middle_center);
        M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
        M5Cardputer.Display.setTextSize(1);

        M5Cardputer.Display.drawString("Meadow's",
                                       M5Cardputer.Display.width() / 2,
                                       row_height * 1);
        M5Cardputer.Display.drawString("Maths Game",
                                       M5Cardputer.Display.width() / 2,
                                       row_height * 2);

        M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
        M5Cardputer.Display.setTextSize(1);

        M5Cardputer.Display.drawString("Press a key to start",
                                       M5Cardputer.Display.width() / 2,
                                      M5Cardputer.Display.height() - 16 );

        M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
        M5Cardputer.Display.setTextSize(1);

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            global_app_state = 1;
 
            M5Cardputer.Display.fillRect(0, 0,
                                      M5Cardputer.Display.width(), 
                                      M5Cardputer.Display.height(),
                                      BLACK);
            return;
          }
        }
      break;

      case 1:
        a = random(13);
        b = random(13);
        goal = a * b;
        
        question = a;
        question += " x ";
        question += b;
        question += " = ";
 
        M5Cardputer.Display.setTextDatum(middle_left);
        M5Cardputer.Display.drawString(question,
                                      24,
                                      M5Cardputer.Display.height() / 2 );

        global_app_state = 2;
        return;
      break;

      case 2:
        if (M5Cardputer.Keyboard.isChange()) {
          if (M5Cardputer.Keyboard.isPressed()) {
              Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if ( answer.length() < 3 ) {
              for (auto i : status.word) {
                  answer += i;
              }
            }

              if (status.del) {
                  answer.remove(answer.length() - 1);
              }

              if (status.enter) {

                M5Cardputer.Display.fillRect(0, 0,
                                            M5Cardputer.Display.width(), 
                                            M5Cardputer.Display.height(),
                                            BLACK);
    
                global_app_state = 3;
                return;
              }

              M5Cardputer.Display.fillRect( M5Cardputer.Display.width() - 80, (M5Cardputer.Display.height()/2 ) - 24, 
                                              M5Cardputer.Display.width(), (M5Cardputer.Display.height()/2 ) + 24, 
                                          BLACK);

              M5Cardputer.Display.setTextDatum(middle_right);
              M5Cardputer.Display.drawString(answer,
                                    M5Cardputer.Display.width() - 24,
                                    M5Cardputer.Display.height() / 2 );


          }
        }
      break;
  

      case 3:
        M5Cardputer.Display.setTextDatum(middle_center);
        M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
        M5Cardputer.Display.setTextSize(1);


        if ( answer.equalsIgnoreCase(goal)){
          M5Cardputer.Display.drawString("Correct!",
                                        M5Cardputer.Display.width() / 2,
                                        M5Cardputer.Display.height() /2 );

        }
        else
        {
          M5Cardputer.Display.drawString("Wrong!",
                                        M5Cardputer.Display.width() / 2,
                                        M5Cardputer.Display.height() /2 );
        }

        M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
        M5Cardputer.Display.setTextSize(1);

        M5Cardputer.Display.drawString("Press a key to contiune",
                                       M5Cardputer.Display.width() / 2,
                                      M5Cardputer.Display.height() - 16 );

        M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
        M5Cardputer.Display.setTextSize(1);

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            answer = "";
            global_app_state = 1;
 
            M5Cardputer.Display.fillRect(0, 0,
                                      M5Cardputer.Display.width(), 
                                      M5Cardputer.Display.height(),
                                      BLACK);
            return;
          }
        }

      break;
    }
}
