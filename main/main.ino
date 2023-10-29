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
  TALLY_SCREEN,
  CELEBRATE_STATE
};

AppState global_app_state = INTRO_SCREEN;
AppState global_prev_state = NONE;

static constexpr int row_height = 32;

int a, b, c;
String question = "";
String answer = "";
String goal = "";
String tally = "";

String top_message = "";

static constexpr int round_length = 10;
int round_count;
int round_score;

static constexpr int FOREGROUND_COLOUR = RED;
static constexpr int BACKGROUND_COLOUR = BLACK; 

static constexpr int NUM_PARTICLES = 64;
struct particle {
  float x, y;
  float dx, dy; 
  uint16_t c;
};

particle particle_system[NUM_PARTICLES];


void clear_screen() {

  M5Cardputer.Display.fillRect(0, 0,
        M5Cardputer.Display.width(), 
        M5Cardputer.Display.height(),
        BACKGROUND_COLOUR);

}

void large_middle_message(String message) {

  M5Cardputer.Display.setTextDatum(middle_center);
  M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
  M5Cardputer.Display.setTextSize(1);

  M5Cardputer.Display.drawString(message,
                              M5Cardputer.Display.width() / 2,
                              M5Cardputer.Display.height() / 2);
}

void small_bottom_message(String message) {

  M5Cardputer.Display.setTextDatum(bottom_center);

  M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
  M5Cardputer.Display.setTextSize(1);

  M5Cardputer.Display.drawString(message,
                                  M5Cardputer.Display.width() / 2,
                                  M5Cardputer.Display.height() - 3 );
}

void small_top_message(String message) {

  M5Cardputer.Display.setTextDatum(top_center);

  M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
  M5Cardputer.Display.setTextSize(1);

  M5Cardputer.Display.drawString(message,
                                M5Cardputer.Display.width() / 2,
                                3 );
}



void setup() {

  randomSeed(analogRead(0));


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

          clear_screen();

          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);

          M5Cardputer.Display.drawString("Meadow's",
                                        M5Cardputer.Display.width() / 2,
                                        row_height * 1);

          M5Cardputer.Display.drawString("Maths Game",
                                        M5Cardputer.Display.width() / 2,
                                        row_height * 2);


          small_bottom_message("Press Any Key");

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

          a = random(1, 13);
          b = random(1, 13);
          goal = a * b;
          
          question = a;
          question += " x ";
          question += b;
          question += " = ";
  
          clear_screen();

          String top_message = "Question ";
          top_message += round_count;
          top_message += " out of ";
          top_message += round_length;

          small_top_message(top_message);

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
          
          clear_screen();

          M5Cardputer.Display.setTextDatum(middle_center);
          M5Cardputer.Display.setTextFont(&fonts::FreeSans18pt7b);
          M5Cardputer.Display.setTextSize(1);
          String response;
          
          if ( answer.equalsIgnoreCase(goal)){
            round_score += 1;

            response = "Yes, Correct!";
          }
          else
          {
            response = "Sorry, No.";
          }

          large_middle_message(response);          
          small_bottom_message("Press a Key to Continue");
        }

        if (M5Cardputer.Keyboard.isChange()) {
          if ( M5Cardputer.Keyboard.isPressed()) {
            
            if ( round_count > round_length-1) {
              global_app_state = TALLY_SCREEN;
            }
            else {
              global_app_state = PRESENT_QUESTION;
            }

            return;
          }
        }
      break;

      case TALLY_SCREEN:
        if ( global_prev_state != global_app_state ) {
          global_prev_state = global_app_state;
        
          clear_screen();


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

          small_bottom_message("Again? Press Any Key");

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
        
          // clear_screen();

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

        clear_screen();
        
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

        small_bottom_message("Press Any Key");
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
