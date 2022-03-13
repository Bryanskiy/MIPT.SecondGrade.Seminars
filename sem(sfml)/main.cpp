#include <cmath>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

class charge {
public:
    enum consts {
        SIZE = 5,
    };

	charge(float x, float y, float value) : pos_(x, y), value_(value), circle_ (sf::CircleShape(SIZE)) {

		if (value > 0) {
			circle_.setFillColor(sf::Color::Red);
		}
		else {
			circle_.setFillColor(sf::Color::Blue);
		}

		circle_.setPosition(pos_);
	};

    charge(const sf::Vector2f& pos, float value) : charge(pos.x, pos.y, value) {} 

    sf::Vector2f get_position() const {return pos_;}
    int get_sign() const {
        if(value_ > 0) return 1;
        else if(value_ < 0) return -1;
        else return 0; 
    }
    float get_value() const {return value_;}

    void draw(sf::RenderWindow& window) const {
    	window.draw(circle_);
    }

private:
	sf::Vector2f pos_;
	float value_;
	sf::CircleShape circle_;
};

class line {
public:
    enum consts {
        LENGTH = 10,
        HEIGHT = 1,
    };

	line(float x, float y, float angle) :
		pos_(x, y), angle_(angle) {

		rect_ = sf::RectangleShape(sf::Vector2f(LENGTH, HEIGHT));
		rect_.setFillColor(sf::Color::White);

		rect_.setOrigin({ LENGTH / 2. , HEIGHT / 2. });
		rect_.setPosition(pos_.x, pos_.y);
		rect_.setRotation(angle_ * 180/3.1415);
	};

    void draw(sf::RenderWindow& window) const {
        window.draw(rect_);
    }

private:
	sf::Vector2f pos_;
	float angle_;

	sf::RectangleShape rect_;
};

class world {
public:
    enum consts_t {
        WINDOW_HEIGHT = 800,
        WINDOW_WIDTH = 1200,
        STEP_X = 15,
        STEP_Y = 15,
    };

    world() : window_(sf::VideoMode(consts_t::WINDOW_WIDTH, consts_t::WINDOW_HEIGHT), "CHARGES") {}

    void run();
private:
    std::vector<line> field_;
    std::vector<charge> charges_;
    sf::RenderWindow window_;
private:
    void calculate_field();    
};

void world::calculate_field() {
    sf::Vector2u window_size = window_.getSize();
    for(std::size_t i = 0; i < window_size.x; i += consts_t::STEP_X) {
        for(std::size_t j = 0; j < window_size.y; j += consts_t::STEP_Y) {
            float modul;
			float x, y;
			
			sf::Vector2f Eij(0, 0);

			for (const auto& elem : charges_) {
				
				x = i - elem.get_position().x;
				y = j - elem.get_position().y;

				modul = elem.get_value() /(std::pow((x * x + y * y), 1.5));
				sf::Vector2f Eij_elem( x * modul, y * modul );
		
				Eij += Eij_elem;
			}

            if(Eij != sf::Vector2f{0.0, 0.0}) {
                sf::Vector2f absolute_pos = window_.mapPixelToCoords({i, j});
                field_.push_back({absolute_pos.x, absolute_pos.y, std::atan2(Eij.y,  Eij.x)});
            }    
        }
    } 
}

void world::run() {
    while (window_.isOpen()) {

		sf::Event event;
		while (window_.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window_.close();
            }  else if(event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2i relative_pos = sf::Mouse::getPosition(window_);
                sf::Vector2f absolute_pos = window_.mapPixelToCoords(relative_pos);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    charges_.push_back({absolute_pos, 1.0});
                } else {
                    charges_.push_back({absolute_pos, -1.0});
                }
            } 
		}

        window_.clear();
        for(const auto& charge : charges_) {
            charge.draw(window_);
        }
        
        field_.clear();
        calculate_field();
        for(const auto& line : field_) {
            line.draw(window_);
        }

		window_.display();
	}
}

int main() {
    world w;
    w.run();
}