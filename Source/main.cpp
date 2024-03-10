#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"
#include "Renderer/Drawing.h"

using namespace TV;
using namespace Maths;
using namespace Renderer;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char** argv)
{
	const Vec2i imageSize(800, 800);
	TGAImage image(imageSize.X, imageSize.Y, TGAImage::RGB);

	Model model;
	if (model.LoadWavefrontFile("Content/african_head.obj"))
	{
		DrawModelWireframe(model, image, white);
	}
	else
	{
		DrawLine(13, 20, 80, 40, image, white);
		DrawLine(20, 13, 40, 80, image, red);
		DrawLine(80, 40, 13, 20, image, red);
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}