#include "controller.hxx"
#include <iostream>

Controller::Controller(float width, float height)
        : model_(width, height),
        view_(model_)
{ }

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}

void
Controller::draw(ge211::Sprite_set& set)
{
    // view_.set_dims(get_window().get_dimensions());
    view_.draw(set, mouse_pos, get_window().get_dimensions().into<float>());
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> mouse)
{
    if (!model_.check_round_status() && model_.round_ <= 3) {
        if (mouse.y <= 250 || mouse.y >= 350) {
            try {
                //std::cout << "placing a tower now!\n";
                model_.place_tower(mouse.into<float>());
                return;
            } catch (...) {
                return;
            }
        } else {
            return;
        }
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> mouse)
{
    mouse_pos = mouse.into<float>();
    return;
}