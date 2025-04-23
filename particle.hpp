#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "billiard.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>


  class Particle {
    private:
        sf::Vector2f position_;  // Particle position
        float angle_;            // Angle in radians
        float velocity_; 
        float last_x;
        float last_y;        // Speed of the particle
                         // Tracks the wall (upper or lower) hit on the first bounce
    
    public:
        // Constructor to initialize position, angle and velocity
        Particle(float y0, float theta0, float velocity);
    
        // Move the particle based on deltaTime
        void move(const Billiard& billiard, float deltaTime);
    
        // Getters for position and angle
        sf::Vector2f getPosition() const { return sf::Vector2f(position_.x, position_.y); }
        float getAngle() const { return angle_; }
    
        // Print the particle's state (position and angle)
        void print_state(const Billiard& billiard) const;
        void print_state_notprecise() const;
    };

#endif
