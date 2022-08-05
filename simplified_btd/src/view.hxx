#pragma once

#include "model.hxx"
#include <ge211.hxx>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<float>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<float>;

    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set, Position mouse_pos, ge211::Dims<float>
        screen_dims);

    void add_tower_sprite_(ge211::Sprite_set set, Position posn) const;
    // void set_dims(Dimensions screen_dims);

private:
    Model const& model_;
    ge211::Rectangle_sprite grass;
    ge211::Rectangle_sprite full_path;
    // ge211::Rectangle_sprite path_1;
    // ge211::Rectangle_sprite path_2;
    // ge211::Rectangle_sprite path_3;
    ge211::Circle_sprite tower;
    ge211::Circle_sprite reg_balloon;
    ge211::Circle_sprite thick_balloon;
    ge211::Circle_sprite dart;
    ge211::Text_sprite lives_sprite;
    ge211::Text_sprite winner_sprite;

};
