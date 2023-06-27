
#include <Model/Setting.h>

FontSetting::FontSetting(QStringList families, int fontStyle, int fontSize) {
    this->families = families;
    this->fontStyle = fontStyle;
    this->fontSize = fontSize;
}

FontSetting::FontSetting() {

}

FontSetting::~FontSetting() = default;
