#include "model.hxx"
#include <iostream>
// for model:
// function for screen size
// function for balloon movement
// variable for lives remaining?
// variable for round status
    // passive: between rounds
    // active: in round
// function for attempt tower place
// variable for round count and variable for game_over
    // if game_over = True, then no more towers and no more bloons

//stole this from reversi, what is it? \/
using namespace ge211;
using Position = ge211::Posn<float>;
using Velocity = ge211::Dims<float>;


// global variables for balloon health initialization
const int reg_health = 1;
const int thick_health = 2;
const int dart_rad = 5;
const float dv_factor = 150;
const int tower_reach_rad = 150;

Tower::Tower(Position posn, float rad, float reach)
    : posn_(posn),
      radius_(rad),
      reach_(reach),
      shooting_(false)
{
    Position t_center = {posn_.x + radius_ - dart_rad,
                             posn_.y + radius_ - dart_rad};
    dart_array_.push_back(Dart({0, 0}, t_center, dart_rad, false));
}

Model::Model(float i, float j)
    : lives_(5),
    round_(1),
    winner_(""),
    dimensions_({i, j}),
    round_active_(false),
    game_over_(false)
{
    int num_balloons = 0;
    float ball_rad = 20;
    int thiccness = reg_health;
    Velocity b_vel = {2, 0};
    if (round_ == 1) {
        num_balloons = 10;
    } else if (round_ == 2) {
        num_balloons = 5;
        thiccness = thick_health;
    } else if (round_ == 3) {
        num_balloons = 10;
        thiccness = thick_health;
    }
    for (int k = 0; k < num_balloons; k++) {
        float l = k;
        balloon_array_.push_back(Balloon(b_vel,
                                        {0 - ((l + 1) * 100),285},
                                         ball_rad,
                                         thiccness,
                                         true));
    }
}
template<typename T>
Position
Model::get_center(T m)
{
    Position tl = m.get_posn();
    Position center = tl.right_by(m.get_radius()).down_by(m.get_radius());
    return center;
}

float
Model::distance_to(ge211::Posn<float> posn1, ge211::Posn<float> posn2)
{
    //ge211::Posn<float> fp1 = posn1.into<float>();
    //ge211::Posn<float> fp2 = posn2.into<float>();
    float x_diff = posn2.x - posn1.x;
    float y_diff = posn2.y - posn1.y;
    float squared = (x_diff * x_diff) + (y_diff * y_diff);
    float dist = sqrtf(squared);
    return dist;
}


bool
Model::balloon_in_radius(class Balloon b, class Tower t)
{
    Position b_cent = Model::get_center(b);
    Position t_cent = t.get_posn().right_by(t.get_radius()).down_by(t
            .get_radius());
    float dist = distance_to(b_cent, t_cent);
    return dist < t.get_reach();
}

// Velocity
// Model::compute_velocity(class Dart d)
// {
//     // seconds per frame
//     float t = 0.01666666666;
//     // desired x velocity of dart
//     float k1 = 0;
//     // desired y velocity of dart
//     float k2 = 0;
//     std::cout << t;
//     k1 = (2 * t - d.get_posn().x)/t;
//     k2 = (-1 * d.get_posn().y)/t;
//     std::cout << "k1: " << k1 << "\n";
//     std::cout << "k2: " << k2 << "\n";
//     return {k1, k2};
// }

void
Model::aim_dart(class Tower& t, class Balloon b)
{
    Position b_pos = b.get_posn();
    Position t_pos = t.get_posn();
    float dist = distance_to(t_pos, b_pos);
    float vec_x = b_pos.x - t_pos.x;
    float vec_y = b_pos.y - t_pos.y;
    //unit direction vector:
    //std::cout << "dist " << float(dist) << "\n\n";
    //std::cout << "vec_x " << float(vec_x) << "\n\n";
    //std::cout << "vec_y " << float(vec_y) << "\n\n";
    ge211::Dims<float> dir = {vec_x / dist,
                    vec_y / dist};
    //std::cout << "dir " << dir << "\n\n";
    t.dart_array_.front().update_vel(dir, dv_factor);
}

// run shooting process
void
Model::run_shooting(class Balloon& b, class Tower& t,double dt)
{
    //if (balloon_in_radius(b, t) &&
    if (!t.dart_array_.front().is_alive()) {
        aim_dart(t,
                 balloon_array_.front().next_balloon(dt));
        t.dart_array_.front().launch();
        //}
    }
    t.dart_array_.front() = t.dart_array_.front().next_dart(dt);
}


bool
Model::check_collision(class Balloon b, class Dart d)
{
    Position b_center = get_center(b);
    Position d_center = get_center(d);
    float dist = distance_to(d_center, b_center);
    float rad_add = b.get_radius() + d.get_radius();
    return dist < rad_add;
}

bool
Model::check_hit_edge_path(class Tower t)
{
    if (t.get_posn().y < 280) {
        if (t.dart_array_.front().get_posn().y > 360) {
            return true;
        }
    } else if (t.get_posn().y > 330){
        if (t.dart_array_.front().get_posn().y < 250) {
            return true;
        }
    }
    return false;
}

void Model::hit_balloon(const int i, Balloon &b)
{
    if (b.get_alive()) {
        b.set_health(b.get_health() - 1);
        if (b.get_health() == 0) {
            b.deflate();
            balloon_array_.erase(balloon_array_.begin() + i);
        }
    }

}

bool
Model::check_win()
{
    return round_ > 3 && lives_ > 0;
}

bool
Model::check_round_over()
{
    if (lives_ == 0) {
        return true;
    }
    if (balloon_array_.empty() && round_active_) {
        //std::cout << "swapping round status in check_round_over...\n";
        //std::cout << "swapping from " << round_active_ << "to" <<
        //!round_active_ << "\n";
        swap_round_status();
        return true;
    }
    return false;
}

void
Model::end_round()
{
    int num_balloons = 0;
    float ball_rad = 20;
    int thiccness = reg_health;
    Velocity b_vel = {2, 0};
    if (round_ >= 4 || lives_ <= 0) {
        end_game();
    } else if (round_ == 2) {
        num_balloons = 5;
        thiccness = thick_health;
    } else if (round_ == 3) {
        num_balloons = 10;
        thiccness = thick_health;
    }
    if (balloon_array_.empty() && !check_game_over()) {
        for (int k = 0; k < num_balloons; k++) {
            float l = k;
            balloon_array_.push_back(Balloon(b_vel,
                                             {0 - ((l + 1) * 100), 285},
                                             ball_rad,
                                             thiccness,
                                             true));
        }
    }
    for (auto& t : tower_array_) {
        t.dart_array_.clear();
        t.dart_array_.push_back(Dart({0, 0}, get_center(t), dart_rad, false));
    }
}

void
Model::swap_round_status()
{
    if (!check_round_status()) {
        round_active_ = true;
    } else {
        round_active_ = false;
    }
}

void
Model::run_round(double dt) {
    // code for making balloon volleys appear,
    // and activating towers (is this part necessary? they should
    // only shoot when there are balloons to shoot at)
    // code for balloon projectile collision? where?
    for (auto& b: balloon_array_) {
        b.update_posn(b.get_velocity());
        if (b.hits_end()) {
            int temp = b.get_health();
            balloon_array_.erase(balloon_array_.begin());
            lives_ -= temp;
            if (lives_ <= 0) {
                end_game();
            }
        }
        if (check_round_over() && !check_game_over()) {
            //std::cout << "calling end_round from run_round\n";
            end_round();
        }
    }

    for (auto& t: tower_array_) {
        unsigned int b_id = 0;
        for (auto& bal : balloon_array_) {
            run_shooting(bal, t,  dt);
            if (check_collision(bal, t.dart_array_.front()))
            {
                hit_balloon(b_id, bal);
                //d.set_alive(false);
                t.dart_array_.erase(t.dart_array_.begin());
                //std::cout << "Ballooon In Radius: "<< balloon_in_radius(bal,
                //                                                          //t);
                // if (balloon_in_radius(bal, t)) {
                //     t.set_shooting_true();
                // }
                t.dart_array_.push_back(Dart({0,0},
                                                 get_center(t),
                                                 dart_rad,
                                                 false));

                //std::cout << "current dart_array len: " << t
                //.dart_array_.size() << "\n";
            }
            else if (check_hit_edge_path(t)) {
                t.dart_array_.erase(t.dart_array_.begin());
                t.dart_array_.push_back(Dart({0,0},
                                             get_center(t),
                                             dart_rad,
                                             false));
            }
            b_id ++;
        }
    }
}

void
Model::on_frame(double dt)
{
    // if round is
    if (check_round_status() == 1) {
        run_round(dt);
    }
    //also check win-- if the player has won, do... something
}


void
Model::place_tower(Position posn)
{
    float rad = 30;
    //error if out of bounds
    if (posn.x > 800 || posn.x < 0 || posn.y > 600 || posn.y < 0) {
        throw Client_logic_error("Model::place_tower: out of bounds");
    } else if (!round_active_ && !game_over_) {
        //code for tower placement
        //sets position to be center of tower, not top_left
        // note: 30 is the hardcoded radius in view.
        Position new_pos = {posn.x - rad, posn.y - rad};
        // if (round_ > tower_array_.size()) {
        //     tower_array_.emplace_back(new_pos, rad, tower_reach_rad);
        // }
        tower_array_.emplace_back(new_pos, rad, tower_reach_rad);
        //std::cout << "Tower Size: " << tower_array_.size() << "\n";
        //std::cout << "round stat: " << check_round_status() << "\n";
        round_ += 1;
        //std::cout << "swapping round status in place_tower...\n";
        //std::cout << "swapping from " << round_active_ << "to" <<
                  //!round_active_ << "\n";
        swap_round_status();
        return;
    }
}


void
Model::end_game()
{
    game_over_ = true;
    std::cout << check_win();
    if (check_win()) {
        winner_ = "You Win!";
    } else if (!check_win()) {
        winner_ = "You Lose!";
    }
    std::cout << winner_;

    balloon_array_.erase (balloon_array_.begin(), balloon_array_.end());
    tower_array_.erase (tower_array_.begin(), tower_array_.end());
}


