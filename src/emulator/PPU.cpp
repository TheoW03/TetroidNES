#include "PPU.h"
#include <SFML/Graphics.hpp>
// sf::RenderWindow Render_window;

PPU::PPU(vector<uint8_t> chr_rom)
{
    this->chr_rom = chr_rom;
    // sf::RenderWindow renderWindow(sf::VideoMode(800, 600), "test window");
    // this->window = renderWindow;
    // Render_window = renderWindow(sf::VideoMode(800, 600), "test window");
}

PPU::PPU()
{
}
