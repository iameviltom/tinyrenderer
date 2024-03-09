#include "Image/TgaImage.h"

#include "Maths/IntVector2.h"

#include <corecrt_math.h>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void DrawLine(IntVector2 Start, IntVector2 End, TGAImage& TargetTexture, const TGAColor& Colour)
{
	if (Start == End)
	{
		return;
	}

	IntVector2 delta(End - Start);
	IntVector2 absDelta = delta.Abs();
	const bool bSteep = absDelta.Y > absDelta.X;

	if (bSteep)
	{
		Start = Start.SwizzleYX();
		End = End.SwizzleYX();
		delta = delta.SwizzleYX();
		//absDelta = absDelta.SwizzleYX();
	}

	// make sure we draw left to right
	if (delta.X < 0)
	{
		std::swap(Start, End);
		delta *= -1;
	}

	float gradient = delta.Y / (float)delta.X;
	float intercept = Start.Y - gradient * Start.X;

	for (int x = Start.X; x != End.X; ++x)
	{
		int y = (int)std::round(gradient * x + intercept);

		if (bSteep)
		{
			TargetTexture.set(y, x, Colour);
		}
		else
		{
			TargetTexture.set(x, y, Colour);
		}
	}

/*
	IntVector2 delta(End - Start);
	delta = delta.Abs();
	delta.Y *= -1;

	IntVector2 sign;
	sign.X = (Start.X > End.X) ? -1 : 1;
	sign.Y = (Start.Y > End.Y) ? -1 : 1;

	int error = delta.X + delta.Y;

	IntVector2 Point(Start);
	while (true)
	{
		TargetTexture.set(Point.X, Point.Y, Colour);

		if (Point == End)
		{
			break;
		}

		int error2 = 2 * error;
		if (error2 >= delta.Y)
		{
			if (Point.X == End.X)
			{
				break;
			}
			error += delta.Y;
			Point.X += sign.X;
		}
		if (error2 <= delta.X)
		{
			if (Point.Y == End.Y)
			{
				break;
			}
			error += delta.X;
			Point.Y += sign.Y;
		}
	}
	*/
}
void DrawLine(int X0, int Y0, int X1, int Y1, TGAImage& TargetTexture, const TGAColor& Colour)
{
	DrawLine(IntVector2(X0, Y0), IntVector2(X1, Y1), TargetTexture, Colour);
}

int main(int argc, char** argv)
{
	TGAImage image(100, 100, TGAImage::RGB);

	DrawLine(13, 20, 80, 40, image, white);
	DrawLine(20, 13, 40, 80, image, red);
	DrawLine(80, 40, 13, 20, image, red);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}