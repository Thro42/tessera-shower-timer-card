#include "shower_timer.h"
#include "timer_model.h"
#include <algorithm>
#include <string>

namespace esphome::shower_timer {
using tessera::Font;
using tessera::Tone;
namespace ui = tessera::ui;

static Font fitting(const std::string &text, int width, int height, bool big = false) {
  // DIGITS does not contain a colon; VALUE is the largest complete countdown font.
  for (Font face : {Font::VALUE, Font::HEADLINE, Font::TITLE, Font::BODY_LARGE, Font::BODY}) {
    if (!big && face == Font::VALUE) continue;
    if (ui::line_height(face) <= height && ui::text_width(text, face) <= width && ui::covers(text, face)) return face;
  }
  return Font::BODY;
}

class ShowerTile : public tessera::Tile {
 public:
  explicit ShowerTile(const tessera::Plugin *plugin) : plugin_(plugin) {}

  void create(const tessera::TileContext &c) override {
    width_ = c.width;
    height_ = c.height;
    entity_ = c.entity;
    title_text_ = c.name;
    if (title_text_.empty()) title_text_ = plugin_->text("title");
    icon_ = ui::label(c.parent, Font::ICON_SMALL, theme::INK);
    ui::set_text(icon_, ui::icon(0xF09A0));  // shower, from Tessera's icon set
    title_ = ui::label(c.parent, Font::BODY, theme::INK);
    timer_ = ui::label(c.parent, Font::VALUE, theme::INK);
    status_ = ui::label(c.parent, Font::BODY, theme::INK);
    temperature_ = ui::label(c.parent, Font::BODY, theme::INK);
    lv_obj_set_style_text_align(timer_, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_align(temperature_, LV_TEXT_ALIGN_RIGHT, 0);
    paint(tessera::epoch());
  }

  void on_state(JsonObjectConst data) override {
    const char *state = data["state"] | "";
    known_ = !strcmp(state, "on") || !strcmp(state, "off");
    on_ = !strcmp(state, "on");
    auto timer = data["entities"]["timer"];
    // Tessera converts finishes_at (ISO in HA) to epoch because its name ends in _at.
    model_.update(timer["state"] | "", timer["attributes"]["finishes_at"] | 0u,
                  timer["attributes"]["remaining"] | "");
    auto temperature = data["entities"]["temperature"];
    has_temperature_ = temperature_value(temperature["state"] | "", temperature_value_);
    unit_ = temperature["attributes"]["unit_of_measurement"] | "°C";
    if (unit_ != "°C" && unit_ != "°F" && unit_ != "K") has_temperature_ = false;
    paint(tessera::epoch());
  }

  void on_tick(uint32_t epoch) override { paint(epoch); }

  void on_theme() override {
    ui::set_color(icon_, theme::INK);
    ui::set_color(title_, theme::INK);
    ui::set_color(status_, known_ ? theme::INK : theme::MUTED);
    ui::set_color(temperature_, has_temperature_ ? theme::INK : theme::MUTED);
    // The boolean, not the timer state, decides red. Reapply it on every theme change.
    ui::set_tone(timer_, on_ ? Tone::ALERT : Tone::NORMAL, theme::INK);
  }

  void on_tap() override {
    // No optimistic local flip, no direct timer or boiler command: wait for HA's state.
    if (known_ && !entity_.empty()) tessera::action("input_boolean.toggle", entity_.c_str());
  }

 private:
  void paint(uint32_t epoch) {
    if (!timer_) return;
    uint32_t left = 0;
    const std::string clock = model_.left(epoch, left) ? countdown_text(left) : "--:--";
    const std::string status = known_ ? plugin_->text(on_ ? "on" : "off") : "--";
    std::string temperature = "--°";
    if (has_temperature_) {
      char value[24];
      snprintf(value, sizeof(value), "%.1f", static_cast<double>(temperature_value_));
      if (!strcmp(plugin_->text("decimal_separator"), ",")) {
        for (char *p = value; *p; ++p) if (*p == '.') *p = ',';
      }
      temperature = std::string(value) + (unit_ == "K" ? " K" : "°");
    }
    const int gap = std::max(1, ui::px(3));
    const int body = ui::line_height(Font::BODY);
    const int row_height = std::max(body, std::min(height_ / 4, ui::line_height(Font::HEADLINE)));
    const int icon_size = ui::line_height(Font::ICON_SMALL);
    std::string title = entity_.empty() ? plugin_->text("choose_switch") : std::string(title_text_.c_str());
    int title_x = icon_size + gap;
    bool show_icon = icon_size <= row_height && ui::text_width(title, Font::BODY) <= width_ - title_x;
    if (!show_icon) title_x = 0;
    if (ui::text_width(title, Font::BODY) > width_) title = plugin_->text(entity_.empty() ? "setup" : "title_short");
    if (show_icon) lv_obj_remove_flag(icon_, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(icon_, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(icon_, icon_size, icon_size);
    lv_obj_set_pos(icon_, 0, (row_height - icon_size) / 2);
    const Font tf = fitting(title, width_ - title_x, row_height);
    ui::set_font(title_, tf);
    ui::set_text(title_, title);
    lv_obj_set_size(title_, width_ - title_x, ui::line_height(tf));
    lv_obj_set_pos(title_, title_x, (row_height - ui::line_height(tf)) / 2);

    const int half = std::max(1, (width_ - gap) / 2);
    const Font sf = fitting(status, half, row_height);
    const Font pf = fitting(temperature, half, row_height);
    ui::set_font(status_, sf);
    ui::set_font(temperature_, pf);
    ui::set_text(status_, status);
    ui::set_text(temperature_, temperature);
    lv_obj_set_size(status_, half, ui::line_height(sf));
    lv_obj_set_size(temperature_, half, ui::line_height(pf));
    lv_obj_set_pos(status_, 0, height_ - ui::line_height(sf));
    lv_obj_set_pos(temperature_, width_ - half, height_ - ui::line_height(pf));

    const int space = std::max(body, height_ - 2 * row_height - 2 * gap);
    const Font cf = fitting(clock, width_, space, true);
    ui::set_font(timer_, cf);
    ui::set_text(timer_, clock);
    lv_obj_set_size(timer_, width_, ui::line_height(cf));
    lv_obj_set_pos(timer_, 0, row_height + gap + (space - ui::line_height(cf)) / 2);
    on_theme();
  }

  const tessera::Plugin *plugin_;
  tessera::Text entity_, title_text_, unit_;
  TimerModel model_;
  int width_ = 0, height_ = 0;
  bool known_ = false, on_ = false, has_temperature_ = false;
  float temperature_value_ = 0;
  lv_obj_t *icon_{}, *title_{}, *timer_{}, *status_{}, *temperature_{};
};

void ShowerTimer::setup() { add_tile("control", [this]() { return new ShowerTile(this); }); }
}  // namespace esphome::shower_timer
