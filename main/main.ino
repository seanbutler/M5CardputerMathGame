/**
 * @file quiz.ino
 * @author Sean Butler (butlersean@gmail.com)
 * @brief Maths Quiz Game Like Little Professor
 * @version 0.1
 * @date 2023-10-27
 *
 */

#include "M5Cardputer.h"

enum AppState : int {

  NONE,
  INTRO_SCREEN,
  PRESENT_QUESTION, 
  INPUT_ANSWER, 
  CHECK_ANSWER,
  TALLY_SCREEN
};

AppState global_app_state = INTRO_SCREEN;
AppState global_prev_state = NONE;

int row_height = 32;

int a, b, c;
String question = "";
String answer = "";
String goal = "";
String tally = "";

const int round_length = 5;
int round_count;
int round_score;

static constexpr int FOREGROUND_COLOUR = GREEN;
static constexpr int BACKGROUND_COLOUR = BLACK; 

void setup() {

  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextColor(FOREGROUND_COLOUR);
  M5Cardputer.Display.setTextDatum(middle_center);
  M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
  M5Cardputer.Display.setTextSize(1);

  round_count = 0;
  round_score = 0;
}

void loop() {
    M5Cardputer.update();

    switch ( global_app_state ) {

      case INTRO_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          M5Cardputer.Display.fillRect(0, 0,
                                    M5Cardputer.Display.width(), 
                                    M5Cardputer.Display.height(),
                                    BACKGROUND_COLOUR);


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

          round_count = 0;
          round_score = 0;

          return;   
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {

            global_app_state = PRESENT_QUESTION;
            return;
          }
        }
      break;

      case PRESENT_QUESTION:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          round_count = round_count + 1;

          a = random(13);
          b = random(13);
          goal = a * b;
          
          question = a;
          question += " x ";
          question += b;
          question += " = ";
  
          M5Cardputer.Display.fillRect(0, 0,
                                  M5Cardputer.Display.width(), 
                                  M5Cardputer.Display.height(),
                                  BACKGROUND_COLOUR);


          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);


          M5Cardputer.Display.setTextDatum(middle_left);
          M5Cardputer.Display.drawString(question,
                                        24,
                                        M5Cardputer.Display.height() / 2 );

          answer = "";
          global_app_state = INPUT_ANSWER;
          return;
        }

      break;

      case INPUT_ANSWER:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
        }

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
                                          BACKGROUND_COLOUR);
  
              global_app_state = CHECK_ANSWER;
              return;
            }

            M5Cardputer.Display.fillRect( M5Cardputer.Display.width() - 80, (M5Cardputer.Display.height()/2 ) - 24, 
                                            M5Cardputer.Display.width(), (M5Cardputer.Display.height()/2 ) + 24, 
                                        BACKGROUND_COLOUR);

            M5Cardputer.Display.setTextDatum(middle_right);
            M5Cardputer.Display.drawString(answer,
                                  M5Cardputer.Display.width() - 24,
                                  M5Cardputer.Display.height() / 2 );

          }
        }
      break;
  
      case CHECK_ANSWER:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
          
          M5Cardputer.Display.fillRect(0, 0,
                                    M5Cardputer.Display.width(), 
                                    M5Cardputer.Display.height(),
                                    BACKGROUND_COLOUR);


          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);

          if ( answer.equalsIgnoreCase(goal)){
            round_score += 1;

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
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            
            if ( round_count > round_length-1) {
              global_app_state = TALLY_SCREEN;
            }
            else {
              global_app_state = PRESENT_QUESTION;
            }

            M5Cardputer.Display.fillRect(0, 0,
                                      M5Cardputer.Display.width(), 
                                      M5Cardputer.Display.height(),
                                      BACKGROUND_COLOUR);
            return;
          }
        }
      break;

      case TALLY_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
        
          M5Cardputer.Display.fillRect(0, 0,
                                    M5Cardputer.Display.width(), 
                                    M5Cardputer.Display.height(),
                                    BACKGROUND_COLOUR);


          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);

          M5Cardputer.Display.drawString("You Scored",
                                        M5Cardputer.Display.width() / 2,
                                        row_height * 1);

          M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
          M5Cardputer.Display.setTextSize(1);

          tally = "";
          tally += round_score;
          tally += " out of ";
          tally += round_length;
          
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);
          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.drawString(tally,
                                          M5Cardputer.Display.width() / 2,
                                          M5Cardputer.Display.height() / 2 );

          M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
          M5Cardputer.Display.setTextSize(1);

          M5Cardputer.Display.drawString("Press a key to go again",
                                          M5Cardputer.Display.width() / 2,
                                          M5Cardputer.Display.height() - 16 );

          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {


            global_app_state = INTRO_SCREEN;
            return;
          }
        }
      break;

    }
}
