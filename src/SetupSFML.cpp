#include <SFML/Graphics.hpp>
using namespace std;

sf::Color getColorFromByte(uint8_t byte)
{
    switch (byte)
    {
    case 0:
        return sf::Color::Black;
    case 1:
        return sf::Color::White;
    case 2:
        return sf::Color(128, 128, 128); // Grey
    case 9:
        return sf::Color(128, 128, 128); // Grey
    case 3:
        return sf::Color::Red;
    case 10:
        return sf::Color::Red;
    case 4:
        return sf::Color::Green;
    case 11:
        return sf::Color::Green;
    case 5:
        return sf::Color::Blue;
    case 12:
        return sf::Color::Blue;
    case 6:
        return sf::Color::Magenta;
    case 13:
        return sf::Color::Magenta;
    case 7:
        return sf::Color::Yellow;
    case 14:
        return sf::Color::Yellow;
    default:
        return sf::Color::Cyan;
    }
}
bool update_texture(uint8_t pixels[32 * 32 * 4])
{
    // // cout << "test: " << static_cast<int>(color.r) << endl;
    // int frame_idx = 0;
    bool update = false;

    // for (int i = 0x0200; i < 0x0600; i++)
    // {
    //     sf::Color c = getColorFromByte(read_8bit(i));
    //     if (pixels[frame_idx] != static_cast<uint8_t>(c.r)         // m
    //         || pixels[frame_idx + 1] != static_cast<uint8_t>(c.g)  // s
    //         || pixels[frame_idx + 2] != static_cast<uint8_t>(c.b)) // s
    //     {
    //         pixels[frame_idx] = static_cast<uint8_t>(c.r);
    //         pixels[frame_idx + 1] = static_cast<uint8_t>(c.g);
    //         pixels[frame_idx + 2] = static_cast<uint8_t>(c.b);
    //         pixels[frame_idx + 3] = 255;

    //         update = true;
    //     }
    //     frame_idx += 4;
    // }
    return update;
}
void runsfml()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "test window");
    window.setFramerateLimit(144);
    sf::Texture texture;

    texture.create(32, 32); // Creating a texture of size 32x32
    // uint8_t pixels[32 * 32 * 4]; // RGBA (4 bytes per pixel)
    uint8_t pixels[32 * 32 * 4];
    // Fill the pixel data with a single red pixel

    float scaleX = window.getSize().x / static_cast<float>(texture.getSize().x);
    float scaleY = window.getSize().y / static_cast<float>(texture.getSize().y);
    sf::Sprite sprite(texture);

    sprite.setScale(scaleX, scaleY);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        // write_8bit(0xfe, ((uint8_t)rand() % 16 + 1));
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        // {
        //     write_8bit(0xff, 0x77);
        // }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        // {
        //     write_8bit(0xff, 0x61);
        // }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        // {
        //     write_8bit(0xff, 0x73);
        // }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        // {
        //     write_8bit(0xff, 0x64);
        // }
        // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        // {
        //     exit(EXIT_SUCCESS);
        // }
        update_texture(pixels);
        texture.update(pixels);
        window.clear(); // Change this to the desired color
        window.draw(sprite);
        window.display();
    }
}
