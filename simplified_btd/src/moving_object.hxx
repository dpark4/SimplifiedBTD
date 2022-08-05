#include <ge211.hxx>

class Moving_object {
public:

    // constructor:
    Moving_object(ge211::Dims<float> vel, ge211::Posn<float> posn, float radius);

    // copy:
    Moving_object(const Moving_object& other);

    // getters:
    ge211::Posn<float> get_posn() const
    { return posn_;}

    ge211::Dims<float> get_velocity() const
    { return velocity_;}

    float get_radius() const
    { return radius_;}
    // virtual void collision();

protected:

    ge211::Dims<float> velocity_;

    ge211::Posn<float> posn_;

    float radius_;
};

class Balloon : public Moving_object {
public:

    // constructor:
    Balloon(ge211::Dims<float> vel, ge211::Posn<float> posn, float rad, int health, bool alive);

    // copy:
    Balloon(const Balloon& other);

    // getters:
    int get_health() const
    {return health_;}

    bool get_alive() const
    {return alive_;}

    //setters
    void set_health(int i)
    {health_ = i;}

    void deflate()
    {alive_ = false;}

    // helpers:
    void update_posn(ge211::Dims<float> dims);

    bool hits_end();

    Balloon next_balloon(double dt) const;

private:

    int health_;

    bool alive_;
};

class Dart : public Moving_object {
public:

    // constructor:
    Dart(ge211::Dims<float> v, ge211::Posn<float> position, float rad, bool alive);

    // copy:
    Dart(const Dart& other);

    // getters:
    bool is_alive() const
    {return alive_;}

    // setters:
    void set_alive(bool i)
    {alive_ = i;}
    // model helpers:

    void update_posn(ge211::Dims<float> dims);

    void update_vel(ge211::Dims<float> dims, float factor);

    Dart next_dart(double dt) const;

    void launch();

private:

    bool alive_;
};