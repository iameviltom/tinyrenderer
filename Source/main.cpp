#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"
#include "Renderer/Drawing.h"

using namespace TV;
using namespace Maths;
using namespace Renderer;

const Colour white = Colour(255, 255, 255, 255);
const Colour red = Colour(255, 0, 0, 255);
const Colour green = Colour(0, 255, 0, 255);

int main(int argc, char** argv)
{
	class ScopedImage
	{
	public:
		ScopedImage(const Vec2i& size) : Image(size.X, size.Y, TGAImage::RGB) {}
		~ScopedImage()
		{
			Image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
			Image.write_tga_file("output.tga");
		}

		TGAImage Image;
	};

	if (true)
	{
		ScopedImage image(Vec2i(200));

		Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
		Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
		Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
		DrawTriangle(t0[0], t0[1], t0[2], image.Image, red);
		DrawTriangle(t1[0], t1[1], t1[2], image.Image, white);
		DrawTriangle(t2[0], t2[1], t2[2], image.Image, green);
	}
	else
	{
		Model model;
		if (model.LoadWavefrontFile("Content/african_head.obj"))
		{
			ScopedImage image(Vec2i(800));
			DrawModelWireframe(model, image.Image, white);
		}
		else
		{
			ScopedImage image(Vec2i(100));
			DrawLine(13, 20, 80, 40, image.Image, white);
			DrawLine(20, 13, 40, 80, image.Image, red);
			DrawLine(80, 40, 13, 20, image.Image, red);
		}
	}

	return 0;
}