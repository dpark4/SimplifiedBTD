#include "moving_object.hxx"
#include <iostream>
using Position = ge211::Posn<float>;
using Velocity = ge211::Dims<float>;

Moving_object::Moving_object(Velocity vel, Position posn, float radius)
    : velocity_(vel),
    posn_(posn),
    radius_(radius)
{}

Moving_object::Moving_object(const Moving_object& other)
    : Moving_object(other.get_velocity(), other.get_posn(), other.get_radius())
{}

Balloon::Balloon(Velocity v, Position posn, float rad, int health, bool alive)
    : Moving_object(v, posn, rad),
    health_(health),
    alive_(alive)
{}

Balloon::Balloon(const Balloon& other)
    : Balloon(other.get_velocity(), other.get_posn(),
    other.get_radius(), other.get_health(), other.get_alive())
{}

Dart::Dart(Velocity v, Position posn, float rad, bool alive)
    : Moving_object(v, posn, alive),
    alive_(alive)
{}

Dart::Dart(const Dart& other)
    : Dart(other.get_velocity(),
           other.get_posn(),
           other.get_radius(),
           other.is_alive())
{}

void
Balloon::update_posn(ge211::Dims<float> dims)
{
    posn_ += dims;
}

bool
Balloon::hits_end()
{
    return get_posn().x >= 800;
}

Balloon
Balloon::next_balloon(double dt) const
{
    Balloon result(*this);
    result.update_posn(result.get_velocity() * dt);
    return result;
}

void
Dart::update_posn(ge211::Dims<float> dims)
{
    posn_ += dims;
}

void
Dart::update_vel(ge211::Dims<float> dims, float factor)
{
    //std::cout << "updating dart vel...\n";
    //std::cout << "dims width: " << dims.width << "\n";
    //std::cout << "factor: " << factor << "\n";
    //std::cout << "dims height: " << dims.height << "\n";
    velocity_ = {dims.width * factor, dims.height * factor};
    float v_mag = sqrtf((velocity_.width * velocity_.width) +
                                (velocity_.height * velocity_.height));
    //std::cout << "velocity: " << velocity_ << "\n";
    //std::cout << "velocity magnitude: " << v_mag << "\n";
}

Dart
Dart::next_dart(double dt) const
{
    Dart result(*this);
    result.update_posn(result.get_velocity() * dt);
    return result;
}

void
Dart::launch()
{
    alive_ = true;
}