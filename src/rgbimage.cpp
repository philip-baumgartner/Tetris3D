#include "rgbimage.h"
#include "color.h"
#include "assert.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height) {
	this->m_Width = Width;
	this->m_Height = Height;
	this->m_Image = new Color[this->m_Width * this->m_Height];
}

RGBImage::~RGBImage() {
	delete m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color &c) {
	if (x >= 0 && x < this->m_Width && y >= 0 && y < this->m_Height) {
		m_Image[x * m_Height + y] = c;
	}
}

const Color &RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
	// Exception?
	if (x >= 0 && x < this->m_Width && y >= 0 && y < this->m_Height) {
		return m_Image[x * m_Height + y];
	}
	return Color(0, 0, 0);
}

bool RGBImage::saveToDisk(const char* Filename) {
	BMPFILEHEADER bmfh;
	BMPINFOHEADER info;

	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfSize = HEADERSIZE + INFOSIZE + m_Width * 3 * m_Height;
	bmfh.bfReserved1 = 0;
	bmfh.bfOffBits = HEADERSIZE + INFOSIZE;

	info.biSize = INFOSIZE;
	info.biWidth = this->m_Width;
	info.biHeight = this->m_Height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0x0ec4;
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	FILE *f;
	f = fopen(Filename, "wb");

	if (f != NULL) {
		fwrite(&bmfh.bfType, 1, sizeof(uint16_t), f);
		fwrite(&bmfh.bfSize, 1, sizeof(uint32_t), f);
		fwrite(&bmfh.bfReserved1, 1, sizeof(uint32_t), f);
		fwrite(&bmfh.bfOffBits, 1, sizeof(uint32_t), f);
		fwrite(&info, 1, sizeof(BMPINFOHEADER), f);

		for (int i = this->height() - 1; i >= 0; i--) {
			for (unsigned int j = 0; j < this->width(); j++) {

				Color tmp = this->getPixelColor(j, i);

				unsigned char r = convertColorChannel(tmp.R);
				unsigned char g = convertColorChannel(tmp.G);
				unsigned char b = convertColorChannel(tmp.B);

				fwrite(&b, 1, sizeof(unsigned char), f);
				fwrite(&g, 1, sizeof(unsigned char), f);
				fwrite(&r, 1, sizeof(unsigned char), f);
			}
		}
		fclose(f);
		return true;
	}
	else {
		std::cerr << "Could not open file: " << Filename << std::endl;
		fclose(f);
		return false;
	}
}

unsigned int RGBImage::width() const {
	return m_Width;
}

unsigned int RGBImage::height() const {
	return m_Height;
}

unsigned char RGBImage::convertColorChannel(float f) {
	if (f < 0) {
		return 0;
	}
	else if (f > 1) {
		return 255;
	}
	else {
		return f * 255;
	}
}

RGBImage& RGBImage::SobelFilter(RGBImage& dst, const RGBImage& src, float factor) {
	assert(dst.height() == src.height() && dst.width() == src.width());

	float k1[3][3] ={	{ 1,	0.,		-1 },
						{ 2,	0.,		-2 },
						{ 1,	0.,		-1 }	};
	float k2[3][3] ={	{ 1,	2,		1  },
						{ 0.,	0.,		0. },
						{ -1,	-2,		-1 }	};

	float u, v, s;

	for (unsigned int y = 0; y < src.height(); y++) {
		for (unsigned int x = 0; x < src.width(); x++) {
			u = src.getPixelColor(x - 1, y - 1).R * k1[0][0]	+ src.getPixelColor(x, y - 1).R * k1[0][1]		+ src.getPixelColor(x + 1, y - 1).R * k1[0][2] +
				src.getPixelColor(x - 1, y).R * k1[1][0]		+ src.getPixelColor(x, y).R * k1[1][1]			+ src.getPixelColor(x + 1, y).R * k1[1][2] +
				src.getPixelColor(x - 1, y + 1).R * k1[2][0]	+ src.getPixelColor(x, y + 1).R * k1[2][1]		+ src.getPixelColor(x + 1, y + 1).R * k1[2][2];

			v = src.getPixelColor(x - 1, y - 1).R * k2[0][0]	+ src.getPixelColor(x, y - 1).R * k2[0][1]		+ src.getPixelColor(x + 1, y - 1).R * k2[0][2] +
				src.getPixelColor(x - 1, y).R * k2[1][0]		+ src.getPixelColor(x, y).R * k2[1][1]			+ src.getPixelColor(x + 1, y).R * k2[1][2] +
				src.getPixelColor(x - 1, y + 1).R * k2[2][0]	+ src.getPixelColor(x, y + 1).R * k2[2][1]		+ src.getPixelColor(x + 1, y + 1).R * k2[2][2];

			s = sqrt(pow(u, 2) + pow(v, 2)) * factor;
			dst.setPixelColor(x, y, Color(s, s, s));
		}
	}

	return dst;
}

RGBImage& RGBImage::GaussFilter(RGBImage& dst, const RGBImage& src) {
	assert(dst.height() == src.height() && dst.width() == src.width());

	float k[] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006 };
	Color c(0, 0, 0);

	for (unsigned int y = 0; y < src.height(); y++) {
		for (unsigned int x = 0; x < src.width(); x++) {
			for (unsigned int i = 0; i < 7; i++) {
				c += src.getPixelColor(x - i - 3, y) * k[i];
			}
			dst.setPixelColor(x, y, c);
			c.R, c.G, c.B = 0;
		}
	}

	for (unsigned int y = 0; y < src.height(); y++) {
		for (unsigned int x = 0; x < src.width(); x++) {
			for (unsigned int i = 0; i < 7; i++) {
				c += dst.getPixelColor(x, y - 1 - 3) * k[i];
			}
			dst.setPixelColor(x, y, c);
			c.R, c.G, c.B = 0;
		}
	}

	return dst;
}
