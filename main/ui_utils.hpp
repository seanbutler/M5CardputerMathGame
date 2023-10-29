
#pragma once

namespace ui_utils {


static constexpr int AFFIRM_COLOUR = GREEN; 
static constexpr int WARN_COLOUR = ORANGE; 
static constexpr int ERROR_COLOUR = RED; 

static constexpr int FOREGROUND_COLOUR = WHITE;
static constexpr int BACKGROUND_COLOUR = BLACK; 

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
                M5Cardputer.Display.height() - 4 );
    }

    void small_top_message(String message) {

        M5Cardputer.Display.setTextDatum(top_center);

        M5Cardputer.Display.setTextFont(&fonts::FreeSans9pt7b);
        M5Cardputer.Display.setTextSize(1);

        M5Cardputer.Display.drawString(message,
                M5Cardputer.Display.width() / 2,
                4 );
    }

}
