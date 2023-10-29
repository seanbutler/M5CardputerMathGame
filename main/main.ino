/**
 * @file quiz.ino
 * @author Sean Butler (butlersean@gmail.com)
 * @brief Maths Quiz Game Like Little Professor
 * @version 0.1
 * @date 2023-10-27
 *
 */

#include "M5Cardputer.h"
#include "ui_utils.hpp"

enum AppState : int {

  NONE,
  INTRO_SCREEN,
  QUIZ_MENU_SCREEN,
  DECIDE_QUESTION,
  PRESENT_MULTIPLICATION_QUESTION,
  PRESENT_DIVISION_QUESTION,
  INPUT_ANSWER, 
  CHECK_ANSWER,
  TALLY_SCREEN,
  CELEBRATE_STATE
};

AppState global_app_state = INTRO_SCREEN;
AppState global_prev_state = NONE;

static constexpr int row_height = 32;

int quiz_type;

int a, b, c;
String question = "";
String answer = "";
String choice = "";
String goal = "";
String tally = "";

String top_message = "";

static constexpr int round_length = 10;
int round_count;
int round_score;



static constexpr int NUM_PARTICLES = 64;
struct particle {
  float x, y;
  float dx, dy; 
  uint16_t c;
};

particle particle_system[NUM_PARTICLES];

void setup() {

  randomSeed(analogRead(0));

  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextColor(ui_utils::FOREGROUND_COLOUR);
  M5Cardputer.Display.setTextDatum(middle_center);
  M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
  M5Cardputer.Display.setTextSize(1);

  round_count = 0;
  round_score = 0;

  quiz_type = 1;
}

void loop() {
    M5Cardputer.update();

    switch ( global_app_state ) {

      case INTRO_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          ui_utils::clear_screen();
          ui_utils::large_middle_message("Maths Game");
          ui_utils::small_bottom_message("Press Any Key");

          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);

          round_count = 0;
          round_score = 0;
          
          return;   
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {

            global_app_state = QUIZ_MENU_SCREEN;
            
            return;
          }
        }
      break;



      case QUIZ_MENU_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          ui_utils::clear_screen();

          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);

          M5Cardputer.Display.drawString("Please Choose",
                                        M5Cardputer.Display.width() / 2,
                                        row_height * 1);


          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
          M5Cardputer.Display.setTextSize(1);

          M5Cardputer.Display.drawString("Press M for Multiplication",
                                        M5Cardputer.Display.width() / 2,
                                        (row_height * 2) + ( 0.5 * row_height * 1) );

          M5Cardputer.Display.drawString("Press D for Division",
                                        M5Cardputer.Display.width() / 2,
                                        (row_height * 2) + ( 0.5 * row_height * 2) );

          M5Cardputer.Display.drawString("Press R for Random",
                                        M5Cardputer.Display.width() / 2,
                                        (row_height * 2) + ( 0.5 * row_height * 3) );
         
          return;   
        }

        if (M5Cardputer.Keyboard.isChange()) 
        {
          if (M5Cardputer.Keyboard.isPressed()) 
          {
            if ( ( M5Cardputer.Keyboard.isKeyPressed('M') )
              || ( M5Cardputer.Keyboard.isKeyPressed('m') ) )
            {
              quiz_type = 0;
              global_app_state = DECIDE_QUESTION;
              return;
            }

            if ( ( M5Cardputer.Keyboard.isKeyPressed('D') )
              || ( M5Cardputer.Keyboard.isKeyPressed('d') ) )
            {
              quiz_type = 1;
              global_app_state = DECIDE_QUESTION;
              return;
            }

            if ( ( M5Cardputer.Keyboard.isKeyPressed('R') ) 
              || ( M5Cardputer.Keyboard.isKeyPressed('r') ) )
            {
              quiz_type = 2;
              global_app_state = DECIDE_QUESTION;
              return;
            }
          }
        }
      break;


      case DECIDE_QUESTION:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          switch (quiz_type)
          {
            case 0:
              global_app_state = PRESENT_MULTIPLICATION_QUESTION;
            break;

            case 1:
              global_app_state = PRESENT_DIVISION_QUESTION;
            break;

            case 2:
                if ( random(0, 2) ) {
                  global_app_state = PRESENT_MULTIPLICATION_QUESTION;
                }
                else {
                  global_app_state = PRESENT_DIVISION_QUESTION;
                }
            break;
          }
        }

      break;

      case PRESENT_MULTIPLICATION_QUESTION:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          round_count = round_count + 1;

          a = random(1, 13);
          b = random(1, 13);
          goal = a * b;
          
          question = a;
          question += " x ";
          question += b;
          question += " = ";
  
          ui_utils::clear_screen();

          String message = "Question ";
          message += round_count;
          message += " of ";
          message += round_length;

          ui_utils::small_top_message(message);

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

      case PRESENT_DIVISION_QUESTION:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;

          round_count = round_count + 1;

          a = random(1, 13);
          c = random(1, 13);
          b = c * a;
          goal = c;

          question = b;
          question += " / ";
          question += a;
          question += " = ";
  
          ui_utils::clear_screen();

          String message = "Question ";
          message += round_count;
          message += " of ";
          message += round_length;

          ui_utils::small_top_message(message);

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
              global_app_state = CHECK_ANSWER;
              return;
            }

            M5Cardputer.Display.fillRect( M5Cardputer.Display.width() - 82,  (M5Cardputer.Display.height()/2 ) - 16, 
                                          80,  32, 
                                        ui_utils::BACKGROUND_COLOUR);

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
          
          ui_utils::clear_screen();

          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);
          String response;
          
          if ( answer.equalsIgnoreCase(goal)){
            round_score += 1;
            M5Cardputer.Display.setTextColor(ui_utils::AFFIRM_COLOUR);

            response = "Yes, Correct!";
          }
          else
          {
            M5Cardputer.Display.setTextColor(ui_utils::ERROR_COLOUR);
            response = "Sorry, No.";
          }

          ui_utils::large_middle_message(response);

          M5Cardputer.Display.setTextColor(ui_utils::FOREGROUND_COLOUR);

          ui_utils::small_bottom_message("Press a Key to Continue");
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            
            if ( round_count > round_length-1) {
              global_app_state = TALLY_SCREEN;
            }
            else {
              global_app_state = DECIDE_QUESTION;
            }

            return;
          }
        }
      break;

      case TALLY_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
        
          ui_utils::clear_screen();

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

          ui_utils::small_bottom_message("Again? Press Any Key");

          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            if ( round_score >= ( round_length - 1))
              global_app_state = CELEBRATE_STATE;
            else
              global_app_state = INTRO_SCREEN;

            return;
          }
        }
      break;

      case CELEBRATE_STATE:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
        
          // ui_utils::clear_screen();

          for (int n=0;n<NUM_PARTICLES;n++){
            particle_system[n].x = M5Cardputer.Display.width() / 2;
            particle_system[n].y = M5Cardputer.Display.height() / 2;

            float d1 = random(0, 10);
            float d2 = random(0, 100);
            float d3 = d1 + ( d2 / 100.0);

            particle_system[n].dx = 5 - d3;

            d1 = random(0, 4);
            d2 = random(0, 100);
            d3 = d1 + ( d2 / 100.0);

            particle_system[n].dy = -d3;
            particle_system[n].c = rand();
          }
        }

        ui_utils::clear_screen();
        
        for (int n=0;n<NUM_PARTICLES;n++){

          particle_system[n].dx *= 0.96;
          particle_system[n].x += particle_system[n].dx;

          particle_system[n].dy += 0.05;
          particle_system[n].y += particle_system[n].dy;
        }

        for (int n=0;n<NUM_PARTICLES;n++){
          M5Cardputer.Display.fillCircle(
                    particle_system[n].x, particle_system[n].y, 
                    4, 
                    particle_system[n].c);
        }

        ui_utils::small_bottom_message("Press Any Key");
        delay(6);

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            global_app_state = INTRO_SCREEN;
            return;
          }
        }
      break;
    }
}
