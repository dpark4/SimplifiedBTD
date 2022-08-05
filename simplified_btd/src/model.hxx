#pragma once

#include <ge211.hxx>
#include <vector>
#include "moving_object.hxx"

class Model
{
public:
    // constructor
    Model(float i, float j);

    // HELPER FUNCTIONS:
    //calculates center position of a given object
    template<typename T> static ge211::Posn<float> get_center(T m);

    //calculates distance between two positions
    static float distance_to(ge211::Posn<float> posn1, ge211::Posn<float> posn2);

    //checks whether a given balloon has entered a given tower's
    // radius of reach.
    static bool balloon_in_radius(class Balloon b, class Tower t);

    // static ge211::Dims<float> compute_velocity(class Dart d);


    // aims the darts in a given tower's array towards the given
    // balloon.
    static void aim_dart(class Tower& t, class Balloon b);

    // runs the code for constant tower shooting (when relevant)
    void run_shooting(class Balloon& b, class Tower& t, double dt);

    //lowers a given balloon's health when hit, and destroys if necessary
    void hit_balloon(int i, class Balloon& b);

    //returns whether a given balloon and dart have collided
    static bool check_collision(class Balloon b, class Dart d);

    static bool check_hit_edge_path(class Tower t);
    // FUNCTIONS:
    // gets lives count
    int get_lives() const
    { return lives_; }

    //checks if player has won
    bool check_win();

    //checks if the round is over
    bool check_round_over();

    // runs necessary code for ending the round
    void end_round();

    // returns the round's status (active or not)
    bool check_round_status()
    { return round_active_; }

    // returns the round's status (active or not)
    bool check_game_over()
    { return game_over_; }

    //swaps the round status from active to passive or passive
    // to active
    void swap_round_status();

    // runs the round; helper for on_frame
    void run_round(double dt);

    //updates game assets on frame
    void on_frame(double dt);

    //places a tower; partner for on_mouse_down
    void place_tower(ge211::Posn<float> posn);

    //destroys assets to end the game
    void end_game();

    // PUBLIC VARIABLES:
    // holds array of balloons for each round
    std::vector<class Balloon> balloon_array_;

    // holds array of towers placed
    std::vector<class Tower> tower_array_;

    int lives_;

    unsigned int round_;

    std::string winner_;

private:

    ge211::Dims<float> dimensions_;

    bool round_active_;

    bool game_over_;

};

class Tower {
public:
    Tower(ge211::Posn<float> position, float rad, float reach);

    // GETTER FUNCTIONS:
    ge211::Posn<float> get_posn() const
    { return posn_;}

    float get_radius() const
    { return radius_;}

    float get_reach() const
    { return reach_;}

    bool get_shooting() const
    { return shooting_;}

    void set_shooting_true()
    { shooting_ = true;}

    void set_shooting_false()
    { shooting_ = false;}

    //holds array of darts for a given tower
    std::vector<class Dart> dart_array_;
private:
    ge211::Posn<float> posn_;
    float radius_;
    float reach_;
    bool shooting_;
};
