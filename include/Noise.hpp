#pragma once

#include "TextureManager.hpp"

namespace method {

/**
 * \brief Generates an RGB8 Texture with white noise in all channels.
 *
 * \param width The width of the texture.
 * \param height The height of the texture.
 * \param band The minimum value of each pixel in the texture.
 */
Texture white_noise(int width, int height, int band);

}
