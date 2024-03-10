#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"

#include <corecrt_math.h>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void DrawLine(Vec2i Start, Vec2i End, TGAImage& TargetTexture, const TGAColor& Colour)
{
	if (Start == End)
	{
		return;
	}

	Vec2i delta(End - Start);

	Vec2i absDelta = delta.Abs();
	const bool bSteep = absDelta.Y > absDelta.X;
	if (bSteep)
	{
		Start = Start.SwizzleYX();
		End = End.SwizzleYX();
		delta = delta.SwizzleYX();
	}

	// make sure we draw left to right
	if (delta.X < 0)
	{
		std::swap(Start, End);
		delta *= -1;
	}

	int deltaError = std::abs(delta.Y) * 2;
	int error = 0;

	int y = Start.Y;
	int ySign = delta.Y > 0 ? 1 : -1;

	if (bSteep)
	{
		for (int x = Start.X; x != End.X; ++x)
		{
			TargetTexture.set(y, x, Colour);

			error += deltaError;
			if (error > delta.X)
			{
				y += ySign;
				error -= delta.X * 2;
			}
		}
	}
	else
	{
		for (int x = Start.X; x != End.X; ++x)
		{
			TargetTexture.set(x, y, Colour);

			error += deltaError;
			if (error > delta.X)
			{
				y += ySign;
				error -= delta.X * 2;
			}
		}
	}
}
void DrawLine(int X0, int Y0, int X1, int Y1, TGAImage& TargetTexture, const TGAColor& Colour)
{
	DrawLine(Vec2i(X0, Y0), Vec2i(X1, Y1), TargetTexture, Colour);
}

void DrawLine(Vec3f Start, Vec3f End, TGAImage& TargetTexture, const TGAColor& Colour)
{
	DrawLine((int)Start.X, (int)Start.Y, (int)End.X, (int)End.Y, TargetTexture, Colour);
}

void DrawModelWireframe(const Model& Model, TGAImage& TargetTexture, const TGAColor& Colour)
{
	const Vec3f offset = Model.GetBoundsMin() * -1.f;
	const float scale = std::min((float)TargetTexture.get_width() / Model.GetBoundsExtents().X, (float)TargetTexture.get_height() / Model.GetBoundsExtents().Y) * 0.5f;

	for (int triIndex = 0; triIndex != Model.NumTris(); ++triIndex)
	{
		const Model::Tri& tri = Model.GetTri(triIndex);
		const Vec3f a = (Model.GetVertex(tri.VertIndex[0]) + offset) * scale;
		const Vec3f b = (Model.GetVertex(tri.VertIndex[1]) + offset) * scale;
		const Vec3f c = (Model.GetVertex(tri.VertIndex[2]) + offset) * scale;

		DrawLine(a, b, TargetTexture, Colour);
		DrawLine(a, c, TargetTexture, Colour);
		DrawLine(b, c, TargetTexture, Colour);
	}
}

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