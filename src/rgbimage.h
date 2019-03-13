#ifndef __SimpleRayTracer__rgbimage__
#define __SimpleRayTracer__rgbimage__

#include <iostream>
#include <assert.h>
#include "Matrix.h"

#define HEADERSIZE 14
#define INFOSIZE 40

class Color;

typedef struct t_BITMAPFILEHEADER {
	uint16_t   bfType;			// 'BM' 
	uint32_t   bfSize;			// Größe des .bmp file
	uint32_t   bfReserved1;		// 0
	uint32_t   bfOffBits;		// Offset
} BMPFILEHEADER;

typedef struct t_BITMAPINFOHEADER {
	uint32_t  biSize;            // Größe Infoheader
	int32_t   biWidth;           // image-width
	int32_t   biHeight;          // image-height
	uint16_t  biPlanes;          // bitplanes
	uint16_t  biBitCount;        // Farbtiefe
	uint32_t  biCompression;     // Kompression
	uint32_t  biSizeImage;       // Bildgröße in Byte
	int32_t   biXPelsPerMeter;   // Horizontale Auflösung des Ausgabegerätes
	int32_t   biYPelsPerMeter;   // Vertikale Auflösung des Ausgabegerätes
	uint32_t  biClrUsed;         // Farbtabelle
	uint32_t  biClrImportant;    // Alle verwendeten Farben
} BMPINFOHEADER;

class RGBImage {
public:
	RGBImage(unsigned int Width, unsigned Height);
	~RGBImage();
	void setPixelColor(unsigned int x, unsigned int y, const Color& c);
	const Color& getPixelColor(unsigned int x, unsigned int y) const;
	bool saveToDisk(const char* Filename);
	unsigned int width() const;
	unsigned int height() const;

	static unsigned char convertColorChannel(float f);
	static RGBImage& SobelFilter(RGBImage& dst, const RGBImage& src, float factor = 1.0f);
	static RGBImage& GaussFilter(RGBImage& dst, const RGBImage& src);

protected:
	Color* m_Image;
	unsigned int m_Height;
	unsigned int m_Width;

};

#endif /* defined(__SimpleRayTracer__rgbimage__) */
