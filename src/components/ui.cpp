#include "../../include/components/ui.h"

UI::UI(){}

UI::UI(Rectangle bounds, Color color, int layer, int luaCallbackRef, const std::string& label, Vector2 labelOffset, int fontSize, Color textColor)
: bounds(bounds), color(color), layer(layer), luaCallbackRef(luaCallbackRef), label(label), labelOffset(labelOffset), fontSize(fontSize), textColor(textColor){}