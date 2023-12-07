#include "lgfx/v1/misc/colortype.hpp"
#pragma once

#include <vector>
#include "lgfx/v1/lgfx_fonts.hpp"
#include <M5Unified.h>
#include "M5Cardputer.h"

namespace sb {

  class Position {
    public:
      Position(int x = 0, int y = 0) : x_(x), y_(y) {}
      int x_;
      int y_;
  };

  class Size {
    public:
      Size(int w = 0, int h = 0) : w_(w), h_(h) {}
      int w_;
      int h_;
  };

  class Widget {
    public:

      void SetPosition(Position position){
        position_ = position;
      }

      virtual void Update() {
        for(auto W : children_ ) {
          W->Update();
        }
      };

      virtual void Render() {
        for(auto W : children_ ) {
          W->Render();
        }
      };

      Widget * parent_;
      std::vector<Widget *> children_;
      Position position_;
      Size size_;
  };

  class Label : public Widget {
    public:
      Label() 
      : font_(fonts::FreeSans12pt7b  )
      , alignment_(middle_center)
      , colour_(GREEN)
      {
      }

      void SetText(String text){
        text_ = text;
      }

      void SetAlignment(textdatum_t alignment){
        alignment_ = alignment;
      }

      void SetFont(lgfx::GFXfont font){
        font_ = font;
      }

      void SetColour(int colour){
        colour_  = colour;
      }


      void Render() {
        M5Cardputer.Display.setTextColor(colour_);
        M5Cardputer.Display.setTextSize(1);
        M5Cardputer.Display.setTextDatum(alignment_);
        M5Cardputer.Display.setTextFont(&font_);
        M5Cardputer.Display.drawString(text_, position_.x_, position_.y_);
      }

      void Update() {

      }

      lgfx::GFXfont font_;
      int colour_;
      String text_;
      textdatum_t alignment_;
  };





}

