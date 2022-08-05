#include "view.hxx"
#include <iostream>

using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;
using Dims = ge211::Dims<float>;
static Color const path_color {148, 94, 0};
static Color const grass_color {0, 200, 50};
static Color const tower_color {0, 200, 200};
static Color const reg_color {255, 0, 0};
static Color const thick_color {0, 0, 255};
static Color const dart_color {0, 0, 0};
// NOTE:
    // SCREEN WIDTH: 800
    // SCREEN HEIGHT: 600


View::View(Model const& model)
        : model_(model),
          grass(ge211::Rectangle_sprite({800, 600},
                                     grass_color)),
          full_path(ge211::Rectangle_sprite({800, 50},
                                        path_color)),
          // path_1(ge211::Rectangle_sprite({380,50},
          //                            path_color)),
          // path_2(ge211::Rectangle_sprite({50,175},
          //                            path_color)),
          // path_3(ge211::Rectangle_sprite({420,50},
          //                                path_color)),
          tower(ge211::Circle_sprite(30,
                                         tower_color)),
          reg_balloon(ge211::Circle_sprite(20,
                                     reg_color)),
          thick_balloon(ge211::Circle_sprite(20,
                                     thick_color)),
          dart(ge211::Circle_sprite(5, dart_color))
{}

void
View::draw(ge211::Sprite_set& set, Position mouse_pos, Dims screen_dims)
{
    //test sprites
    set.add_sprite(grass, {0, 0}, 0);
    set.add_sprite(full_path, {0, 280}, 1);
    // set.add_sprite(path_1, {0, 175}, 1);
    // set.add_sprite(path_2, {380, 175}, 1);
    // set.add_sprite(path_3, {380, 350}, 1);
    // add_tower_sprite_(set, mouse_pos);

    // generate tower sprite(s)
    for (const auto & i : model_.tower_array_) {
        set.add_sprite(tower, i.get_posn().into<int>(), 3);
        // generate dart sprites
        for (const auto & k : i.dart_array_) {
            if (k.is_alive()) {
                set.add_sprite(dart,
                               k.get_posn
                               ().into<int>(),
                               4);
            }
        }
    }
    // generate balloon sprites
    for (const auto & j : model_.balloon_array_) {
        set.add_sprite(reg_balloon, j.get_posn().into<int>(), 3);
    }


    // generate sprite for lives remaining counter
    ge211::Posn<int> clicks_posn = {750, 5};
    if (model_.get_lives() > 0) {
        lives_sprite =
                ge211::Text_sprite(std::to_string(model_.get_lives()),
                                   {"sans.ttf", 50});
    } else {
        lives_sprite =
                ge211::Text_sprite(std::to_string(0),
                                   {"sans.ttf", 50});
    }

    set.add_sprite(lives_sprite, clicks_posn, 3);
    if (model_.winner_ != "") {
        winner_sprite = ge211::Text_sprite((model_.winner_),
                                           {"sans.ttf", 50});
        set.add_sprite(winner_sprite, {50, 5}, 3);
    }

}

void
View::add_tower_sprite_(ge211::Sprite_set set, Position posn) const
{
    set.add_sprite(tower, posn.into<int>(), 2);
}

// void
// View::set_dims(Dims screen_dimensions)
// {
//     screen_dims = screen_dimensions;
//     return;
// }
