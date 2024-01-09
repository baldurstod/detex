#include "emscripten.h"
#include "detex.h"
#include <stdlib.h>
#include <math.h>


int result[2];

EMSCRIPTEN_KEEPALIVE
uint8_t* create_buffer(int size) {
	return malloc(size * sizeof(uint8_t));
}

EMSCRIPTEN_KEEPALIVE
void destroy_buffer(uint8_t* p) {
	free(p);
}

EMSCRIPTEN_KEEPALIVE
uint32_t decode_BC1(uint8_t *data, int width, int height) {
	detexTexture texture;

	texture.format = DETEX_TEXTURE_FORMAT_BC1;
	texture.data = data;
	texture.width = width;
	texture.height = height;
	texture.width_in_blocks = ceil(width / 4);
	texture.height_in_blocks = ceil(height / 4);

	uint32_t pixel_format = DETEX_PIXEL_FORMAT_RGBA8;
	size_t size = texture.width * texture.height * detexGetPixelSize(pixel_format);
	uint8_t *pixel_buffer = (uint8_t *)malloc(size);
	uint32_t r = detexDecompressTextureLinear(&texture, pixel_buffer, pixel_format);


	result[0] = (int)pixel_buffer;
	result[1] = size;
	return r;
}

EMSCRIPTEN_KEEPALIVE
uint32_t decode_BC2(uint8_t *data, int width, int height) {
	detexTexture texture;

	texture.format = DETEX_TEXTURE_FORMAT_BC2;
	texture.data = data;
	texture.width = width;
	texture.height = height;
	texture.width_in_blocks = ceil(width / 4);
	texture.height_in_blocks = ceil(height / 4);

	uint32_t pixel_format = DETEX_PIXEL_FORMAT_RGBA8;
	size_t size = texture.width * texture.height * detexGetPixelSize(pixel_format);
	uint8_t *pixel_buffer = (uint8_t *)malloc(size);
	uint32_t r = detexDecompressTextureLinear(&texture, pixel_buffer, pixel_format);


	result[0] = (int)pixel_buffer;
	result[1] = size;
	return r;
}

EMSCRIPTEN_KEEPALIVE
uint32_t decode_BC3(uint8_t *data, int width, int height) {
	detexTexture texture;

	texture.format = DETEX_TEXTURE_FORMAT_BC3;
	texture.data = data;
	texture.width = width;
	texture.height = height;
	texture.width_in_blocks = ceil(width / 4);
	texture.height_in_blocks = ceil(height / 4);

	uint32_t pixel_format = DETEX_PIXEL_FORMAT_RGBA8;
	size_t size = texture.width * texture.height * detexGetPixelSize(pixel_format);
	uint8_t *pixel_buffer = (uint8_t *)malloc(size);
	uint32_t r = detexDecompressTextureLinear(&texture, pixel_buffer, pixel_format);


	result[0] = (int)pixel_buffer;
	result[1] = size;
	return r;
}

EMSCRIPTEN_KEEPALIVE
uint32_t decode_BC4(uint8_t *data, int width, int height) {
	detexTexture texture;

	texture.format = DETEX_TEXTURE_FORMAT_RGTC1;
	texture.data = data;
	texture.width = width;
	texture.height = height;
	texture.width_in_blocks = ceil(width / 4);
	texture.height_in_blocks = ceil(height / 4);

	uint32_t pixel_format = DETEX_PIXEL_FORMAT_RGBA8;
	size_t size = texture.width * texture.height * detexGetPixelSize(pixel_format);
	uint8_t *pixel_buffer = (uint8_t *)malloc(size);
	uint32_t r = detexDecompressTextureLinear(&texture, pixel_buffer, pixel_format);


	result[0] = (int)pixel_buffer;
	result[1] = size;
	return r;
}

EMSCRIPTEN_KEEPALIVE
uint32_t decode_BC7(uint8_t *data, int width, int height) {
	detexTexture texture;

	texture.format = DETEX_TEXTURE_FORMAT_BPTC;
	texture.data = data;
	texture.width = width;
	texture.height = height;
	texture.width_in_blocks = ceil(width / 4);
	texture.height_in_blocks = ceil(height / 4);

	uint32_t pixel_format = DETEX_PIXEL_FORMAT_RGBA8;
	size_t size = texture.width * texture.height * detexGetPixelSize(pixel_format);
	uint8_t *pixel_buffer = (uint8_t *)malloc(size);
	uint32_t r = detexDecompressTextureLinear(&texture, pixel_buffer, pixel_format);


	result[0] = (int)pixel_buffer;
	result[1] = size;
	return r;
}

EMSCRIPTEN_KEEPALIVE
int get_result_pointer() {
  return result[0];
}

EMSCRIPTEN_KEEPALIVE
int get_result_size() {
  return result[1];
}

EMSCRIPTEN_KEEPALIVE
int get_detex_error_message_pointer() {
	return (int)detexGetErrorMessage();
}

/*
 * Decode texture function (linear). Decode an entire texture into a single
 * image buffer, with pixels stored row-by-row, converting into the given pixel
 * format.
 */
/*
typedef struct {
	uint32_t format;
	uint8_t *data;
	int width;
	int height;
	int width_in_blocks;
	int height_in_blocks;
} detexTexture;
*/
/*
bool detexDecompressTextureLinear(const detexTexture *texture,
uint8_t * DETEX_RESTRICT pixel_buffer, uint32_t pixel_format) {
	uint8_t block_buffer[DETEX_MAX_BLOCK_SIZE];
	if (!detexFormatIsCompressed(texture->format)) {
		return detexConvertPixels(texture->data, texture->width * texture->height,
			detexGetPixelFormat(texture->format), pixel_buffer, pixel_format);
	}
	const uint8_t *data = texture->data;
	int pixel_size = detexGetPixelSize(pixel_format);
	bool result = true;
	for (int y = 0; y < texture->height_in_blocks; y++) {
		int nu_rows;
		if (y * 4 + 3 >= texture->height)
			nu_rows = texture->height - y * 4;
		else
			nu_rows = 4;
		for (int x = 0; x < texture->width_in_blocks; x++) {
			bool r = detexDecompressBlock(data, texture->format,
				DETEX_MODE_MASK_ALL, 0, block_buffer, pixel_format);
			uint32_t block_size = detexGetPixelSize(pixel_format) * 16;
			if (!r) {
				result = false;
				memset(block_buffer, 0, block_size);
			}
			uint8_t *pixelp = pixel_buffer +
				y * 4 * texture->width * pixel_size +
				+ x * 4 * pixel_size;
			int nu_columns;
			if (x * 4 + 3  >= texture->width)
				nu_columns = texture->width - x * 4;
			else
				nu_columns = 4;
			for (int row = 0; row < nu_rows; row++)
				memcpy(pixelp + row * texture->width * pixel_size,
					block_buffer + row * 4 * pixel_size,
					nu_columns * pixel_size);
			data += detexGetCompressedBlockSize(texture->format);
		}
	}
	return result;
}*/
