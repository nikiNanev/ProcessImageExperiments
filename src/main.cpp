// main.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Place this file in your project folder

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

struct Image {
    int width = 0;
    int height = 0;
    int channels = 0;           // original channels in file (1–4)
    std::vector<unsigned char> data;

    Image() = default;

    // Load image with desired number of channels (0 = keep original)
    explicit Image(const std::string& filename, int desired_channels = 4)
    {
        unsigned char* loaded_data = stbi_load(
            filename.c_str(),
            &width,
            &height,
            &channels,
            desired_channels
        );

        if (!loaded_data) {
            throw std::runtime_error("Failed to load image: " +
                filename + " - " + stbi_failure_reason());
        }

        // desired_channels is 0 if we kept original, otherwise it's the forced value
        int actual_channels = desired_channels != 0 ? desired_channels : channels;

        size_t size = static_cast<size_t>(width) * height * actual_channels;
        data.assign(loaded_data, loaded_data + size);

        // Free the memory allocated by stb_image
        stbi_image_free(loaded_data);

        // Update channels to reflect what we actually have
        channels = actual_channels;

        std::cout << "Loaded: " << filename << "\n"
                  << "  Size: " << width << " x " << height << "\n"
                  << "  Channels: " << channels << " ("
                  << (channels == 1 ? "Gray" :
                      channels == 2 ? "Gray+Alpha" :
                      channels == 3 ? "RGB" : "RGBA") << ")\n";
    }

    // Helper to get pixel (x,y) as unsigned char pointer (top-left origin)
    const unsigned char* getPixel(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return nullptr;
        return &data[(y * width + x) * channels];
    }
};

int main()
{
    try {
        // Load as RGBA (4 channels) — most common for OpenGL, graphics, etc.
        Image img("../assets/texture.png", 4);

        // Example: read pixel at (100, 50)
        if (auto pixel = img.getPixel(100, 50)) {
            unsigned char r = pixel[0];
            unsigned char g = pixel[1];
            unsigned char b = pixel[2];
            unsigned char a = pixel[3];
            std::cout << "Pixel(100,50) = RGBA("
                      << (int)r << ", " << (int)g << ", "
                      << (int)b << ", " << (int)a << ")\n";
        }

        // The image data is automatically managed in img.data (std::vector)
        // No manual free needed!
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}