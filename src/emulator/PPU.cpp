// #include "PPU.h
#include <SFML/Graphics.hpp>
#include <stdint.h>
#include <Emulator/PPU.h>
#include <Emulator/EmulatorUtil.h>
#include <Qt/util.h>
#include <optional>
#include <bitset>
#include <Qt/util.h>
#include <QDebug>

PPU::PPU(std::vector<uint8_t> chrrom, MirrorType mirrorType)
{

    this->chr_rom = chrrom;
    this->mirrorType = mirrorType;
    this->internalDataBuffer = 0;
    // this->reg.ppuAddr.val = 0;
    this->ppuaddr.reset();
    this->reg.ppuCtrl.val = 0;
    // this->reg..val = 0;
    this->ppustatus.reset();
    for (int i = 0; i < 2048; i++)
        this->memory[i] = 0;
    this->reg.scrollLatch = false;
    this->reg.ppumask.val = 0;
    this->scanline = 0;
    this->cycles = 0;
    this->err_string = std::nullopt;
    this->start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 255; i++)
        this->oam[i] = 0;
    for (int i = 0; i < 32; i++)
    {
        this->pallete[i] = 0;
    }
    this->oam_addr = 0;
}
PPU::PPU() {}

std::tuple<uint8_t, uint8_t, uint8_t> PPU::getColorFromByte(uint16_t byte, std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> pallete)
{
#pragma region SYS_PAL
    std::tuple<uint8_t, uint8_t, uint8_t> system_palette[64] = {
        {0x80, 0x80, 0x80},
        {0x00, 0x3D, 0xA6},
        {0x00, 0x12, 0xB0},
        {0x44, 0x00, 0x96}, //
        {0xA1, 0x00, 0x5E},
        {0xC7, 0x00, 0x28},
        {0xBA, 0x06, 0x00},
        {0x8C, 0x17, 0x00}, //
        {0x5C, 0x2F, 0x00},
        {0x10, 0x45, 0x00},
        {0x05, 0x4A, 0x00},
        {0x00, 0x47, 0x2E}, //
        {0x00, 0x41, 0x66},
        {0x00, 0x00, 0x00},
        {0x05, 0x05, 0x05},
        {0x05, 0x05, 0x05}, //
        {0xC7, 0xC7, 0xC7},
        {0x00, 0x77, 0xFF},
        {0x21, 0x55, 0xFF},
        {0x82, 0x37, 0xFA}, //
        {0xEB, 0x2F, 0xB5},
        {0xFF, 0x29, 0x50},
        {0xFF, 0x22, 0x00},
        {0xD6, 0x32, 0x00}, //
        {0xC4, 0x62, 0x00},
        {0x35, 0x80, 0x00},
        {0x05, 0x8F, 0x00},
        {0x00, 0x8A, 0x55}, //
        {0x00, 0x99, 0xCC},
        {0x21, 0x21, 0x21},
        {0x09, 0x09, 0x09},
        {0x09, 0x09, 0x09}, //
        {0xFF, 0xFF, 0xFF},
        {0x0F, 0xD7, 0xFF},
        {0x69, 0xA2, 0xFF},
        {0xD4, 0x80, 0xFF}, //
        {0xFF, 0x45, 0xF3},
        {0xFF, 0x61, 0x8B},
        {0xFF, 0x88, 0x33},
        {0xFF, 0x9C, 0x12}, //
        {0xFA, 0xBC, 0x20},
        {0x9F, 0xE3, 0x0E},
        {0x2B, 0xF0, 0x35},
        {0x0C, 0xF0, 0xA4}, //
        {0x05, 0xFB, 0xFF},
        {0x5E, 0x5E, 0x5E},
        {0x0D, 0x0D, 0x0D},
        {0x0D, 0x0D, 0x0D}, //
        {0xFF, 0xFF, 0xFF},
        {0xA6, 0xFC, 0xFF},
        {0xB3, 0xEC, 0xFF},
        {0xDA, 0xAB, 0xEB}, //
        {0xFF, 0xA8, 0xF9},
        {0xFF, 0xAB, 0xB3},
        {0xFF, 0xD2, 0xB0},
        {0xFF, 0xEF, 0xA6}, //
        {0xFF, 0xF7, 0x9C},
        {0xD7, 0xE8, 0x95},
        {0xA6, 0xED, 0xAF},
        {0xA2, 0xF2, 0xDA}, //
        {0x99, 0xFF, 0xFC},
        {0xDD, 0xDD, 0xDD},
        {0x11, 0x11, 0x11},
        {0x11, 0x11, 0x11} //
    };
#pragma endregion

    if (byte == 0)
    {
        // qInfo() << "0b00" << num_to_hexa(this->pallete[0]);
        return system_palette[this->pallete[0]];
    }
    else if (byte == 1)
    {
        // qInfo() << "0b01";
        return system_palette[std::get<0>(pallete)];
    }
    else if (byte == 2)
    {
        // qInfo() << "error";
        // qInfo() << "system pallete: " << num_to_hexa(std::get<2>(pallete));

        return system_palette[std::get<1>(pallete)];
    }
    else if (byte == 3)
    {
        // qInfo() << "error";

        return system_palette[std::get<2>(pallete)];
    }
    std::cout << "non" << std::endl;
    return system_palette[byte];
}

void PPU::get_chr_tile(uint16_t tile_idx, int banks, std::vector<uint8_t> &tile_list)
{

    // get chr tile
    for (int i = banks + tile_idx * 16; i <= ((banks + tile_idx * 16) + 15); i++)
    {

        tile_list.push_back(chr_rom[i]);
    }
}
std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> PPU::bg_pallete(size_t row, size_t column)
{
    size_t attr_table = row / 4 * 8 + column / 4;
    uint8_t attr_byte = memory[0x3c0 + attr_table];
    size_t column_b = column % 4 / 2;
    size_t row_b = row % 4 / 2;
    uint8_t idx = 0;
    if (row_b == 0 && column_b == 0)
    {
        idx = attr_byte & 0b11;
    }
    else if (row_b == 1 && column_b == 0)
    {
        idx = (attr_byte >> 2) & 0b11;
    }
    else if (row_b == 0 && column_b == 1)
    {
        idx = (attr_byte >> 4) & 0b11;
    }
    else
    {
        idx = (attr_byte >> 6) & 0b11;
    }
    size_t pallete_offset = 1 + idx * 4;
    // printf(" offset 1: %d \n", pallete_offset);
    // for (int i = 0; i < 32; i++)
    // {
    //     printf("%x \n", this->pallete[i]);
    // }

    return {
        this->pallete[0],
        this->pallete[pallete_offset],
        this->pallete[pallete_offset + 1],
        this->pallete[pallete_offset + 2],

    };
}

uint16_t PPU::mirror(uint16_t address)
{
    uint16_t mirrored_vram = address & 0b10111111111111;
    uint16_t m = mirrored_vram - 0x2000;
    uint16_t nameTable = m / 0x400;
    if ((mirrorType == MirrorType::VERTICAL) && (nameTable == 2 || nameTable == 3))
        m -= 0x800;
    else if ((mirrorType == MirrorType::HORIZONTAL) && (nameTable == 1 || nameTable == 2))
        m -= 0x400;
    else if (mirrorType == MirrorType::HORIZONTAL && nameTable == 3)
        m -= 0x800;
    return m;
}

uint8_t PPU::read_PPU_data()
{
    uint16_t addr = this->ppuaddr.read_16bit();

    uint16_t c = this->ppuaddr.read_16bit() + ((reg.ppuCtrl.I) ? 32 : 1);
    this->ppuaddr.write_16bit(c);
    // printf("pallete \n");
    // this->reg.ppuAddr.val += reg.ppuCtrl.I ? 32 : 1;
    if (addr <= 0x1fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = chr_rom[addr];
        return res;
    }
    else if (addr >= 0x2000 && addr <= 0x2fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = memory[mirror(addr)];
        return res;
    }
    else if (addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c)
    {

        addr = addr - 0x10;
        return pallete[addr - 0x3f00];
    }
    return 0;
}
uint8_t PPU::read_status()
{
    uint8_t ret = this->ppustatus.read_8bit();
    // this->reg.ppuStatus.V = 0;
    this->ppustatus.set_bit(7, 0);
    this->ppuaddr.reset_latch();
    // TODO: fix
    //  reg.high_ptr = true;
    reg.scrollLatch = false;
    return ret;
}
void PPU::print_ppu_stats()
{
    printf("\n");

    printf("===== PPU ON EXIT =========== \n");
    printf("\n");

    // printf("ppu_addr:  decimal: %d hexa: 0x%x   \n", this->reg.ppuAddr.val, this->reg.ppuAddr.val);
    // printf("ppu_addr hi: decimal: %d hexa: 0x%x\n", this->reg.ppuAddr.hi, this->reg.ppuAddr.hi);
    // printf("ppu_addr lo: decimal:  %d hexa: 0x%x \n", this->reg.ppuAddr.lo, this->reg.ppuAddr.lo);
    // std::bitset<7> ppu_status(this->reg.ppuStatus.val);
    std::bitset<7> ppu_ctrl(this->reg.ppuCtrl.val);
    // std::cout << "ppu status: 0b" << ppu_status << std::endl;
    std::cout << "ppu ctrl: 0b" << ppu_ctrl << std::endl;
    printf("ppu cycles %ld \n", this->cycles);
    printf("OAM Addr hexa: 0x%x decimal: %d \n", this->oam_addr, this->oam_addr);
    printf("\n============================= \n"); //
    printf("\n");
}
void PPU::log_ppu()
{
    // std::bitset<8> ppu_status(this->reg.ppuStatus.val);
    std::bitset<8> ppu_ctrl(this->reg.ppuCtrl.val);
    std::bitset<8> ppu_mask(this->reg.ppumask.val);

    qInfo() << "===== PPU ON EXIT ===========";
    qInfo() << "";

    qInfo() << "===== OAM  ====";
    qInfo() << "oam addr: " << num_to_hexa(this->oam_addr);
    qInfo() << "=====PPU ADDR=====";
    this->ppuaddr.log();
    qInfo() << "";

    qInfo() << "===== PPU status ====";
    this->ppustatus.log();
    // qInfo() << "VBlank: " << this->reg.ppuStatus.V;
    // qInfo() << "0_hit: " << this->reg.ppuStatus.S;
    // qInfo() << "overflow: " << this->reg.ppuStatus.O;
    // qInfo() << "status: " << ppu_status.to_string();
    qInfo() << "";

    qInfo() << "===== PPU ctrl ====";
    qInfo() << "NMI enable (0: off, 1: on): " << this->reg.ppuCtrl.V;
    qInfo() << "PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins): " << this->reg.ppuCtrl.P;
    qInfo() << "sprite size (0: 8x8, 1: 8x16): " << this->reg.ppuCtrl.H;
    qInfo() << "Background patterntable (0: $0000; 1: $1000): " << this->reg.ppuCtrl.B;
    qInfo() << "Sprite patterntable (0: $0000; 1: $1000): " << this->reg.ppuCtrl.S;
    qInfo() << "increment (0: add 1 going across, 1: add 32 going down): " << this->reg.ppuCtrl.I;
    std::bitset<2> name_table_address(this->reg.ppuCtrl.N);
    qInfo() << "name table addreess: " << name_table_address.to_string();
    qInfo() << "ctrl: " << ppu_ctrl.to_string();
    qInfo() << "";
    qInfo() << "===== PPU mask ====";
    qInfo() << "Emphasize blue: " << this->reg.ppumask.B;
    qInfo() << "Emphasize green: " << this->reg.ppumask.G;
    qInfo() << "Emphasize red: " << this->reg.ppumask.R;

    qInfo() << "Enable sprite rendering: " << this->reg.ppumask.s;
    qInfo() << "Enable background rendering: " << this->reg.ppumask.b;
    qInfo() << "Show sprites in leftmost 8 pixels of screen: " << this->reg.ppumask.M;
    qInfo() << "Show background in leftmost 8 pixels of screen " << this->reg.ppumask.m;
    qInfo() << "grey scale (0: normal color, 1: grey scale): " << this->reg.ppumask.g;
    qInfo() << "ppu mask: " << ppu_mask.to_string();
    qInfo() << "";
}
void PPU::write_PPU_address(uint8_t val)
{
    this->ppuaddr.write_8bit(val);
}
void PPU::write_PPU_ctrl(uint8_t val)
{
    auto before = this->reg.ppuCtrl;

    this->reg.ppuCtrl.val = val;
    if (before.V == 0 && this->reg.ppuCtrl.V == 1 && this->ppustatus.get_bit(7) == 1)
    {
        this->reg.ppuCtrl.V = 1;
    }
}
void PPU::write_PPU_mask(uint8_t val)
{

    std::bitset<7> ppu_status2(val);
    std::cout << "mask: " << ppu_status2 << std::endl;
    std::cout << "ppu mask being written to" << std::endl;
    this->reg.ppumask.val = val;
    std::bitset<7> ppu_status(this->reg.ppumask.val);
    std::cout << "mask: " << ppu_status << std::endl;
    // exit(EXIT_FAILURE);
}
std::optional<int> PPU::write_PPU_data(uint8_t val)
{

    uint16_t addr = this->ppuaddr.read_16bit();
    if (addr == 0)
        return 1;
    // printf("%x \n", addr);

    // std::cout << addr << std::endl;
    if (addr >= 0x2000 && addr <= 0x2fff)
    {
        // uint8_t res = internalDataBuffer;
        // if (this->reg.ppumask.b == 0)
        // {
        //     // printf("is 0 \n");
        //     return 1;
        // }
        this->memory[mirror(addr)] = val;
        // std::cout << "saving to vram" << std::endl;
        // internalDataBuffer = memory[mirror(addr)];
    }
    else if (addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c)
    {
        addr = addr - 0x10;
        addr &= 0x1F;

        qInfo() << "pallete" << num_to_hexa(addr) << " val: " << num_to_hexa(val);

        this->pallete[addr] = val;
        qInfo() << "pallete written to: " << this->pallete[addr];
    }
    else if (addr >= 0x3f00 && addr <= 0x3fff)
    {
        addr &= 0x1F;
        this->pallete[addr] = val;
        qInfo() << "pallete" << num_to_hexa(addr) << " val: " << num_to_hexa(val);
    }
    else if (addr == 0x4014)
    {
        this->write_OAM_data(val);

        // printf("%x\n", addr);
    }
    else
    {
        // TODO: fails for some reason
        // std::cout << "\033[91mAttempt to write into PPU READ_ONLY_MEM\033[0m" << std::endl;
        // printf("0x%x\n", addr);
        // exit(EXIT_FAILURE);
        if (this->reg.ppumask.s != 0)
        {
            this->err_string = std::optional<std::string>{"Address 0x" + num_to_hexa(this->ppuaddr.read_16bit()) + " is a PPU read only address"};
            return {};
        }
        return 1;
    }
    uint16_t c = this->ppuaddr.read_16bit() + ((reg.ppuCtrl.I) ? 32 : 1);
    this->ppuaddr.write_16bit(c);
    // this->reg.ppuAddr.val += reg.ppuCtrl.I ? 32 : 1;
    if (this->ppuaddr.read_16bit() > 0x3fff)
    {
        // qInfo() << "addr" << num_to_hexa(this->reg.ppuAddr.val) << " val: " << val;
        uint16_t addr_value = this->ppuaddr.read_16bit() & 0b11111111111111;
        this->ppuaddr.write_16bit(addr_value);
        // this->reg.ppuAddr.val &= 0b11111111111111;
    }
    return 1;
}

bool PPU::tick(uint8_t clock_cycles)
{
    this->cycles += clock_cycles;
    // qInfo() << "ppu cycles: " << this->cycles;

    if (this->cycles >= 341)
    {
        this->scanline += 1;
        this->cycles -= 341;
        // qInfo() << "scanline: " << this->scanline;
        // frame change

        if (scanline == 241)
        {
            this->ppustatus.set_bit(7, 1);
            // reg.ppuStatus.V = 1;
            qInfo() << "vblank";
            if (this->reg.ppuCtrl.V == 1)
            {
                // printf("NMI?");

                // std::bitset<8> ppu_status(this->reg.ppuCtrl.val);
                // std::cout << "after NMI ctrl: " << ppu_status << std::endl;
                return true;
            }
        }

        // ppu reser. this is when it finishes clearing the screen with pizles
        if (scanline >= 262)
        {

            this->scanline = 0;
            this->ppustatus.set_bit(7, 1);
            // this->reg.ppuStatus.V = 0;
            qInfo() << "reset";
            return true;
        }
    }
    return false;
}

bool PPU::NMI_interrupt(uint8_t clock_cycles)
{
    if (this->scanline == 241)
    {
        this->ppustatus.set_bit(7, 1);
        if (this->reg.ppuCtrl.V == 1)
        {

            // printf("%d \n",)
            // std::cout << "NMI should be firing" << std::endl;

            return true;
        }
    }
    // std::cout << "n"
    return false;
}

void PPU::draw_background(std::vector<uint8_t> &rgb_ds, int banks, std::tuple<size_t, size_t> res)
{

    for (int ppu_idx = 0; ppu_idx < 0x3c0; ppu_idx++)
    {

        uint16_t tile = this->memory[(ppu_idx)]; /// name tables

        int idx = ppu_idx % 32;
        int idy = ppu_idx / 32;
        auto bgpallete = this->bg_pallete(idx, idy);
        std::vector<uint8_t>
            tile_list;
        this->get_chr_tile(tile, banks, tile_list);
        qDebug() << tile;
        // std::vector<uint8_t> tile_list = this->get_chr_tile(tile, banks);

        // for (int i = banks + tile * 16; i <= ((banks + tile * 16) + 15); i++)
        // {

        //     tile_list.push_back(chr_rom[i]);
        // }
        for (int y = 0; y < 8; y++)
        {
            uint8_t upper = tile_list[y];
            uint8_t lower = tile_list[y + 8];
            for (int x = 7; x >= 0; x--)
            {
                uint16_t value = (1 & upper) << 1 | (1 & lower);
                upper >>= 1;
                lower >>= 1;

                auto rgb = getColorFromByte((this->reg.ppumask.b == 1 ? value : 0), bgpallete);
                // if (value == 0)
                //     continue;
                // sf::Color rgb = getColorFromByte(value);
                int tile_x = idx * 8 + x;
                int tile_y = idy * 8 + y;
                // printf("tile_x %d  tile_y: %d \n", tile_x, tile_y);

                int b = (tile_y) * 4 * std::get<0>(res) + (tile_x) * 4;

                rgb_ds[b] = std::get<0>(rgb);
                rgb_ds[b + 1] = std::get<1>(rgb);
                rgb_ds[b + 2] = std::get<2>(rgb);
                rgb_ds[b + 3] = 0xff;
            }
            // printf("=========\n");
        }
        // printf("\n ");
    }
}
void PPU::draw_sprites(std::vector<uint8_t> &rgb_ds, int banks, std::tuple<size_t, size_t> res)
{

    for (int ppu_idx = 255; ppu_idx >= 0; ppu_idx -= 4)
    {
        union Attribute_byte
        {
            struct
            {
                unsigned pallete : 2;
                unsigned padding : 4;
                // unsigned priority : 1;

                unsigned flip_x : 1;
                unsigned flip_y : 1;
            };
            uint8_t val;
        };
        Attribute_byte attribbyte;
        int idy = this->oam[ppu_idx - 3];
        // attribbyte.val = this->oam[ppu_idx - 1];
        attribbyte.val = this->oam[ppu_idx - 1];
        uint16_t tile = this->oam[ppu_idx - 2];
        int idx = this->oam[ppu_idx];
        // std::cout << "a" << std::endl;
        // Attribute_byte b = this->oam[ppu_idx - 1];

        // if (idy != 0)
        // printf(" idx: %x  idy: %x \n", idx, idy);
        // DEBUG STUFF
        // if (idy == 0x94)
        // printf("x: %x  y: %x \n tile: %x tle: %x\n", idx, idy, tile, idk);
        // printf(" tile:%d \n", tile);
        //  uint16_t tile = this->memory[ppu_idx];
        //  int idx = ppu_idx % 32;
        //  int idy = ppu_idx / 32;

        banks = this->reg.ppuCtrl.B ? 0x1000 : 0;
        std::vector<uint8_t> tile_list;
        auto pallete_idx = attribbyte.pallete;
        // printf("%x \n", attribbyte.pallete);
        size_t pallete_offset = 0x11 + (pallete_idx * 4);
        std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> sprite_palletes = {
            0,
            this->pallete[pallete_offset],
            this->pallete[pallete_offset + 1],
            this->pallete[pallete_offset + 2],

        };
        // printf("offset: %d \n", pallete_offset);
        qDebug() << "sprite pallete 1: " << num_to_hexa(std::get<0>(sprite_palletes)) << "2: " << num_to_hexa(std::get<1>(sprite_palletes)) << "3: " << num_to_hexa(std::get<2>(sprite_palletes));
        // for (int i = 0; i < 32; i++)
        //     printf("pallete: 0x%x \n", this->pallete[i]);

        // for (int i = 0; i < 255; i++)
        //     printf("oam: %x \n", this->oam[i]);

        std::bitset<2>
            u(attribbyte.pallete);
        std::bitset<8>
            attri(attribbyte.val);
        // qInfo() << "pallete: " << u.to_string();
        // qInfo() << "y: " << idy;
        // qInfo() << "attributes: " << attri.to_string() << attribbyte.val << num_to_hexa(attribbyte.val);
        // qDebug() << "tile: " << num_to_hexa(tile);
        // qInfo() << "x: " << idx;
        // qInfo()
        // printf("tile: %x \n", tile);

        qDebug()
            << "index: " << ppu_idx;

        this->get_chr_tile(tile, banks, tile_list);
        for (int y = 0; y < 8; y++)
        {
            uint8_t upper = tile_list[y];
            uint8_t lower = tile_list[y + 8];
            for (int x = 7; x >= 0; x--)
            {
                uint16_t value = (1 & upper) << 1 | (1 & lower);
                upper >>= 1;
                lower >>= 1;

                if (value == 0)
                    continue;
                auto rgb = getColorFromByte(value, sprite_palletes);
                // std::cout << rgb << std::endl;
                int tile_x = 0;
                if (attribbyte.flip_x)
                    tile_x = idx + 7 - x;
                else
                    tile_x = idx + x;

                int tile_y = 0;
                if (attribbyte.flip_y)
                    tile_y = idy + 7 - y;
                else
                    tile_y = idy + y;
                // printf("tile_x %d  tile_y: %d \n", tile_x, tile_y);
                int b = (tile_y) * 4 * std::get<0>(res) + (tile_x) * 4;

                rgb_ds[b] = std::get<0>(rgb);
                rgb_ds[b + 1] = std::get<1>(rgb);
                rgb_ds[b + 2] = std::get<2>(rgb);
                rgb_ds[b + 3] = 0xff;

                // printf("combined %d  \n", b);
                // printf("%d \n", rgb_ds.size());
            }
            // printf("=========\n");
        }
        // printf("reset loop\n");s
    }
}
/**
 * @brief gets u a vector of bytes that represent the Texture map warning. assumes you are RGBA so its res * 4
 *
 * @param res
 * @return std::vector<uint8_t>
 */
std::vector<uint8_t> PPU::render_texture(std::tuple<size_t, size_t> res)
{
    int banks = this->reg.ppuCtrl.B ? 0x1000 : 0;
    std::vector<uint8_t> rgb_ds;
    rgb_ds.resize(std::get<0>(res) * std::get<1>(res) * 4);

    if (this->chr_rom.size() == 0)
        return rgb_ds;
    draw_background(rgb_ds, banks, res);
    draw_sprites(rgb_ds, banks, res);
    return rgb_ds;
}
uint8_t PPU::read_OAM_data()
{

    return oam[oam_addr];
}
// oam data. set at 0x2004
void PPU::write_OAM_data(uint8_t val)
{
    // cant access the OAM if the ctrl S bit is not set

    this->oam[this->oam_addr] = val;
    this->oam_addr++;

    // for (int i = 0; i < 255; i++)
    // {
    //     qDebug() << "oam " << this->oam[i];
    // }
    if (oam_addr > 255)
    {
        qDebug() << "oam is 0";
        qDebug() << "oam: " << num_to_hexa(this->oam_addr);

        this->oam_addr = 0;
    }
    // oam_addr += (oam_addr + 1) % 256;
    // printf("%x \n", oam_addr);
    // std::cout << oam_addr << std::endl;
}

void PPU::write_OAM_address(uint8_t val)
{
    // std::cout << "write to oam addr" << std::endl;

    this->oam_addr = val;
}
