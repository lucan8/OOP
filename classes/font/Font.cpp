#include "Font.h"
#include <fstream>
#include <filesystem>
#include "../exceptions/FileOpenException.h"
#include "../constants/Constants.h"

Font :: Font(const std :: string& file_path) {
    //Allocating memory for the baked characters
    uint16_t nr_char = Constants :: getVal("NR_FONT_CHARS");
    this->cdata = std :: make_unique<stbtt_bakedchar>(nr_char);

    //Allocating memory for the font's bitmap (for the resolution, width and height are the same)
    uint16_t bitmap_width = Constants :: getVal("FONT_BITMAP_WIDTH");
    unique_ptr<uint8_t> bitmap(new uint8_t[bitmap_width * bitmap_width]);

    //Baking the font
    stbtt_BakeFontBitmap((uint8_t*)readFile(file_path).c_str(), 0, 24, bitmap.get(),
                         bitmap_width, bitmap_width,' ', nr_char, cdata.get());
                         
    //Creating the texture for that font
    this->texture = Textures(bitmap.get(), bitmap_width, bitmap_width);
}