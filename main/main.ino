  
#if defined ( ARDUINO )

#include <Arduino.h>

// If you use SD card, write this.
#include <SD.h>

// If you use SPIFFS, write this.
#include <SPIFFS.h>

#endif

// * The filesystem header must be included before the display library.

//----------------------------------------------------------------

// If you use ATOM Display, write this.
#include <M5AtomDisplay.h>

// If you use Module Display, write this.
#include <M5ModuleDisplay.h>

// If you use Module RCA, write this.
#include <M5ModuleRCA.h>

// If you use Unit GLASS, write this.
#include <M5UnitGLASS.h>

// If you use Unit GLASS2, write this.
#include <M5UnitGLASS2.h>

// If you use Unit OLED, write this.
#include <M5UnitOLED.h>

// If you use Unit Mini OLED, write this.
#include <M5UnitMiniOLED.h>

// If you use Unit LCD, write this.
#include <M5UnitLCD.h>

// If you use UnitRCA (for Video output), write this.
#include <M5UnitRCA.h>

// * The display header must be included before the M5Unified library.

//----------------------------------------------------------------

// Include this to enable the M5 global instance.

#include <M5Unified.h>
#include "M5Cardputer.h"
#include "M5GFX.h"

#include "glib.hpp"
#include "lgfx/v1/lgfx_fonts.hpp"


#include <random>

// ============================================================

namespace quiz {

  constexpr unsigned int size = 10;

  String op;
  int first[size];  
  int second[size];  
  int answer[size];

  int current;
  int correct;
  int guess;

  bool is_correct() {
    return answer[current] == guess;
  }
  
  void generate_mul_l1_questions()
  {
    correct = 0;
    current = 0;

    std::vector<int> nums {2, 3, 4, 5, 8, 10};
    std::random_device rd;
    std::mt19937 gen (rd());
    std::uniform_int_distribution<> dis(0, nums.size() -1);

    for (int n=0;n<size; n++){
      first[n] = nums[dis(gen)];
      second[n] = random(1, 11);
      answer[n] = first[n] * second[n];
    }
  }

  void generate_mul_l2_questions()
  {
    correct = 0;
    current = 0;

    std::vector<int> nums {6, 7, 9, 11, 12, 25};
    std::random_device rd;
    std::mt19937 gen (rd());
    std::uniform_int_distribution<> dis(0, nums.size() -1);

    for (int n=0;n<size; n++){
      first[n] = nums[dis(gen)];
      second[n] = random(1, 11);
      answer[n] = first[n] * second[n];
    }
  }

  void generate_div_questions()
  {
    correct = 0;
    current = 0;

    for (int n=0;n<size; n++){
      second[n] = random(1, 11);
      answer[n] = random(1, 11);
      first[n] = second[n] * answer[n];
    }
  }

  void generate_round_questions()
  {
    correct = 0;
    current = 0;

    for (int n=0;n<size; n++){

      int power = random(1, 4);

      if ( power == 1) {
        first[n] = random(1, 101);
        second[n] = 10;
        answer[n] = round( (float) ( first[n] / 10.0f ) ) * 10;
      }
      else if ( power == 2 )
      {
        first[n] = random(201, 1001);
        second[n] = 100;
        answer[n] = round( (float) ( first[n] / 100.0f ) ) * 100;
      }
      else
      {
        first[n] = random(201, 1001);
        second[n] = 10;
        answer[n] = round( (float) ( first[n] / 100.0f ) ) * 10;
      }
    }
  }
}


namespace fsm {

  class machine;
  class state;

  class state {
    public:
      virtual void Enter(machine * machine) = 0;
      virtual void Execute(machine* context) = 0;
      virtual void Exit(machine * machine) = 0;
  };

  class machine {
    public:
      machine()
      : current_state_(0l)
      , previous_state_(0l) 
      {
        
      }
      
      state * current_state_;
      state * previous_state_;

      void Execute(){
          if ( previous_state_ != current_state_ )
          {
            previous_state_ = current_state_;
            current_state_->Enter(this);
          }
          else
          {
            current_state_->Execute(this);
          }
      }

      void SetCurrentState(state * new_state_){
          current_state_ = new_state_;
      }

      void ChangeState(state * new_state_){
          current_state_->Exit(this);

          previous_state_ = current_state_;
          current_state_ = new_state_;
      }
  };

}

// ============================================================

namespace app {

  namespace states {

    class intro_screen : public fsm::state {

      public:
        sb::Label title_label;
        sb::Label instruction_label;

        intro_screen() {
          Serial.printf("intro_screen ctor\r\n");
        }

        void Enter(fsm::machine * machine){
          Serial.printf("intro_screen Enter()\r\n");

          title_label.SetColour(YELLOW);
          title_label.SetText(String("Maths Game!"));
          title_label.SetFont(fonts::FreeSans18pt7b);
          title_label.SetAlignment(middle_center);
          title_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()/2));

          instruction_label.SetColour(YELLOW);
          instruction_label.SetText(String("Press Any Key"));
          instruction_label.SetFont(fonts::FreeSans9pt7b);
          instruction_label.SetAlignment(bottom_center);
          instruction_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()-4));

          title_label.Render();
          instruction_label.Render();
        }

        void Execute(fsm::machine * machine);

        void Exit(fsm::machine * machine){
          Serial.printf("intro_screen Exit()\r\n");
          M5Cardputer.Display.clearDisplay();
        }
    };

    class menu_screen : public fsm::state {
      public:
        sb::Label title_label;
        sb::Label choice1_label;
        sb::Label choice2_label;
        sb::Label choice3_label;
        sb::Label instruction_label;

        menu_screen() {
          Serial.printf("menu_screen ctor\r\n");
        }

        void Enter(fsm::machine * machine){
          Serial.printf("menu_screen Enter()\r\n");

          quiz::current = 0;
          quiz::correct = 0;
          
          title_label.SetText(String("Choose Your Quiz!"));
          title_label.SetColour(YELLOW);
          title_label.SetFont(fonts::FreeSans9pt7b);
          title_label.SetAlignment(top_center);
          title_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, 4));

          choice1_label.SetColour(RED);
          choice1_label.SetText(String("1) Easy 2) Hard Multiply"));
          choice1_label.SetFont(fonts::FreeSans9pt7b);
          choice1_label.SetAlignment(middle_center);
          choice1_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, (M5Cardputer.Display.height()/2)-24));

          choice2_label.SetColour(RED);
          choice2_label.SetText(String("D) Divide"));
          choice2_label.SetFont(fonts::FreeSans9pt7b);
          choice2_label.SetAlignment(middle_center);
          choice2_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, (M5Cardputer.Display.height()/2)+0));

          choice3_label.SetColour(RED);
          choice3_label.SetText(String("R) Round"));
          choice2_label.SetFont(fonts::FreeSans9pt7b);
          choice3_label.SetAlignment(middle_center);
          choice3_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, (M5Cardputer.Display.height()/2)+24));

          instruction_label.SetColour(YELLOW);
          instruction_label.SetText(String("Press 1, 2, D, or R"));
          instruction_label.SetFont(fonts::FreeSans9pt7b);
          instruction_label.SetAlignment(bottom_center);
          instruction_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()-4));

          title_label.Render();
          choice1_label.Render();
          choice2_label.Render();
          choice3_label.Render();
          instruction_label.Render();
        }

        void Execute(fsm::machine * machine);

        void Exit(fsm::machine * machine){
          Serial.printf("intro_screen Exit()\r\n");
          M5Cardputer.Display.clearDisplay();
        }
    };


    class question_screen : public fsm::state {
      public:
        sb::Label title_label;

        sb::Label question_label;
        sb::Label result_label;
        
        sb::Label instruction_label;

        String question;
        String answer;

        question_screen() {
          Serial.printf("question_screen ctor\r\n");
        }

        void Enter(fsm::machine * machine){
          Serial.printf("question_screen Enter()\r\n");

          title_label.SetText(String("Question ") + String(quiz::current+1) + String(" of ") + String(quiz::size));
          title_label.SetColour(YELLOW);
          title_label.SetFont(fonts::FreeSans12pt7b);
          title_label.SetAlignment(top_center);
          title_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, 8));
          title_label.Render();

          question_label.SetText(String(quiz::first[quiz::current]) + String(quiz::op) + String(quiz::second[quiz::current]) + String("="));
          question_label.SetColour(RED);
          question_label.SetFont(fonts::FreeSans18pt7b);
          question_label.SetAlignment(middle_right);
          question_label.SetPosition(sb::Position(M5Cardputer.Display.width()-64, M5Cardputer.Display.height()/2));
          question_label.Render();

          answer = "";
        }

        void Execute(fsm::machine * machine);

        void Exit(fsm::machine * machine){
          Serial.printf("question_screen Exit()\r\n");
          M5Cardputer.Display.clearDisplay();
        }
    };


    class check_answer_screen : public fsm::state {
      public:
        sb::Label title_label;
        sb::Label result_label;
        sb::Label instruction_label;

        check_answer_screen() {
          Serial.printf("check_answer_screen ctor\r\n");
        }

        void Enter(fsm::machine * machine){
          Serial.printf("check_answer_screen Enter()\r\n");

          if (quiz::is_correct())
          {
            quiz::correct++;
            title_label.SetText(String("You Got It!"));
            title_label.SetColour(GREEN);
          }
          else
          {
            title_label.SetText(String("Sorry, No"));
            title_label.SetColour(RED);
          }

          title_label.SetFont(fonts::FreeSans18pt7b);
          title_label.SetAlignment(top_center);
          title_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, 8));

          instruction_label.SetText(String("Press Any Key"));
          instruction_label.SetFont(fonts::FreeSans12pt7b);
          instruction_label.SetAlignment(bottom_center);
          instruction_label.SetColour(YELLOW);
          instruction_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()-8));

          title_label.Render();
          instruction_label.Render();

          quiz::current++;
        }

        void Execute(fsm::machine * machine);

        void Exit(fsm::machine * machine){

          Serial.printf("tally_screen Exit()\r\n");
          M5Cardputer.Display.clearDisplay();
        }
    };


    class tally_screen : public fsm::state {
      public:
        sb::Label title_label;
        sb::Label result_label;
        sb::Label instruction_label;

        tally_screen() {
          Serial.printf("tally_screen ctor\r\n");
        }

        void Enter(fsm::machine * machine){
          Serial.printf("tally_screen Enter()\r\n");

          title_label.SetText(String("You Got..."));
          title_label.SetFont(fonts::FreeSans18pt7b);
          title_label.SetAlignment(top_center);
          title_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, 8));

          result_label.SetText(String(quiz::correct) + String(" out of ") + String(quiz::size));
          result_label.SetColour(YELLOW);
          result_label.SetFont(fonts::FreeSans18pt7b);
          result_label.SetAlignment(middle_center);
          result_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()/2));

          instruction_label.SetText(String("Press Any Key"));
          instruction_label.SetFont(fonts::FreeSans12pt7b);
          instruction_label.SetAlignment(bottom_center);
          instruction_label.SetColour(YELLOW);
          instruction_label.SetPosition(sb::Position(M5Cardputer.Display.width()/2, M5Cardputer.Display.height()-8));

          title_label.Render();
          result_label.Render();
          instruction_label.Render();
        }

        void Execute(fsm::machine * machine);

        void Exit(fsm::machine * machine){
          Serial.printf("tally_screen Exit()\r\n");
          M5Cardputer.Display.clearDisplay();
        }
    };

  // ----------------------------------------------------------------------

    intro_screen          intro_screen_state;
    menu_screen           menu_screen_state;
    question_screen       question_screen_state;
    check_answer_screen     check_answer_screen_state;
    tally_screen          tally_screen_state;

  // ----------------------------------------------------------------------

    void intro_screen::Execute(fsm::machine * machine){
      // Serial.printf("intro_screen Execute()\r\n");

      // listen for key presses
      if ( M5Cardputer.Keyboard.isChange()) {
        if ( M5Cardputer.Keyboard.isPressed()) {
          // change state to next...
          machine->ChangeState(&menu_screen_state );
        }
      }
    } 

    void menu_screen::Execute(fsm::machine * machine){
      // Serial.printf("menu_screen Execute()\r\n");

      // listen for key presses              
      if ( M5Cardputer.Keyboard.isChange()) {
        if ( M5Cardputer.Keyboard.isPressed()) {
          
          if ( M5Cardputer.Keyboard.isKeyPressed('1') )
          {
            quiz::op = "x";
            quiz::generate_mul_l1_questions();
            machine->ChangeState(&question_screen_state);
          }

          if ( M5Cardputer.Keyboard.isKeyPressed('2') )
          {
            quiz::op = "x";
            quiz::generate_mul_l2_questions();
            machine->ChangeState(&question_screen_state);
          }


          if ( ( M5Cardputer.Keyboard.isKeyPressed('D') )
            || ( M5Cardputer.Keyboard.isKeyPressed('d') ) )
          {
            quiz::op = "/";
            quiz::generate_div_questions();
            machine->ChangeState(&question_screen_state);
          }            

          if ( ( M5Cardputer.Keyboard.isKeyPressed('R') )
            || ( M5Cardputer.Keyboard.isKeyPressed('r') ) )
          {
            quiz::op = "R";
            quiz::generate_round_questions();
            machine->ChangeState(&question_screen_state);
          }            


        }
      }
    }


    void question_screen::Execute(fsm::machine * machine){
      // Serial.printf("question_screen Execute()\r\n");

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
            quiz::guess = atoi(answer.c_str());
            machine->ChangeState(&check_answer_screen_state);
          }

          M5Cardputer.Display.fillRect( M5Cardputer.Display.width() - 64,  (M5Cardputer.Display.height()/2 ) - 16, 
                                        64,  32, 
                                        BLACK);

          M5Cardputer.Display.setTextDatum(middle_right);
          M5Cardputer.Display.drawString(answer,
                                M5Cardputer.Display.width() - 8,
                                M5Cardputer.Display.height() / 2 );

        }
      }    
    }

    void check_answer_screen::Execute(fsm::machine * machine){
      // Serial.printf("tally_screen Execute()\r\n");

      // listen for key presses              
      if ( M5Cardputer.Keyboard.isChange()) {
        if ( M5Cardputer.Keyboard.isPressed()) {
          if ( quiz::current < quiz::size )
          {
            machine->ChangeState(&question_screen_state);
          }
          else
          {
            machine->ChangeState(&tally_screen_state);
          }
        }
      }
    }  


    void tally_screen::Execute(fsm::machine * machine){
      // Serial.printf("tally_screen Execute()\r\n");

      // listen for key presses              
      if ( M5Cardputer.Keyboard.isChange()) {
        if ( M5Cardputer.Keyboard.isPressed()) {
          machine->ChangeState(&intro_screen_state);
        }
      }
    }  


  // ----------------------------------------------------------------------

  }

}

// ============================================================

fsm::machine app_logic;

app::states::intro_screen intro_screen_state;
app::states::menu_screen menu_screen_state;

void setup(void)
{
    M5Cardputer.begin();
    M5Cardputer.Display.setRotation(1);
    Serial.begin(115200);

    app_logic.SetCurrentState(&intro_screen_state);
}

void loop(void)
{
  M5Cardputer.update(); // needed for keyboard detection

  app_logic.Execute();
  M5.delay(1);
}

//----------------------------------------------------------------

// for ESP-IDF compat
#if !defined ( ARDUINO ) && defined ( ESP_PLATFORM )
extern "C" {
  void loopTask(void*)
  {
    setup();
    for (;;) {
      loop();
    }
    vTaskDelete(NULL);
  }

  void app_main()
  {
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, 1);
  }
}
#endif
