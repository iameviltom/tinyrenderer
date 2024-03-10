#pragma once

#include <fstream>
#include "../Maths/Colour.h"
#include "../Renderer/ICanvas.h"

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

using namespace TV::Maths;
using TGAColor = Colour;

class TGAImage : public TV::Renderer::ICanvas
{
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool   load_rle_data(std::ifstream& in);
	bool unload_rle_data(std::ofstream& out);
public:
	enum Format {
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage& img);
	bool read_tga_file(const char* filename);
	bool write_tga_file(const char* filename, bool rle = true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	bool set(int x, int y, TGAColor c);
	~TGAImage();
	TGAImage& operator =(const TGAImage& img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char* buffer();
	void clear();

	virtual Vec2i GetSize() const override { return Vec2i(width, height); }
	virtual void SetPixel(const Vec2i& Coord, const Colour& Colour) override { set(Coord.X, Coord.Y, Colour); }
	virtual Colour GetPixel(const Vec2i& Coord) override { return get(Coord.X, Coord.Y); }
};