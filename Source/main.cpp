#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"
#include "Renderer/Drawing.h"
#include "Renderer/DepthBuffer.h"

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
		Model model;
		if (model.LoadWavefrontFile("Content/african_head.obj"))
		{
			ScopedImage image(Vec2i(800));
			DepthBuffer depthBuffer(image.Image.GetSize());
			const Vec3f lightDir(0, 0, 1);

			TGAImage diffuse;
			const bool bDiffuseValid = diffuse.read_tga_file("Content/african_head_diffuse.tga");
			diffuse.flip_vertically();

			// build model matrix (hard-coded for now)
			const Matrix4x4f modelMtx; // identity for now

			// build camera matrix (hard-coded for now)
			const Matrix4x4f cameraMtx = Matrix4x4f::MakeTranslation(Vec3f(0.f, 0.f, 3.f));
			const Matrix4x4f invCameraMtx = cameraMtx.GetInverse();

			const Matrix4x4f modelViewMatrix = invCameraMtx * modelMtx;

			// build projection matrix (hard-coded for now)
			constexpr float cameraDistance = 3.f;
			Matrix4x4f projectionMtx;
			projectionMtx.M32 = -1.f / cameraDistance;

			const Vec3f cameraSpaceLightDir = invCameraMtx.TransformVector(lightDir);

			DrawModel(modelViewMatrix, projectionMtx, model, bDiffuseValid ? &diffuse : nullptr, image.Image, &depthBuffer, cameraSpaceLightDir);
		}
		else
		{
			ScopedImage image(Vec2i(200));

			Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
			Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
			Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
			DrawTriangle(t0[0], t0[1], t0[2], image.Image, red);
			DrawTriangle(t1[0], t1[1], t1[2], image.Image, white);
			DrawTriangle(t2[0], t2[1], t2[2], image.Image, green);
		}
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