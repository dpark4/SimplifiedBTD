#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller(float width, float height);
    void on_frame(double dt);

protected:
    void draw(ge211::Sprite_set& set) override;
    void on_mouse_down(ge211::Mouse_button m, ge211::Posn<int> p) override;
    void on_mouse_move(ge211::Posn<int> mouse) override;

private:
    Model model_;
    View view_;
    ge211::Posn<float> mouse_pos {0, 0};
};
