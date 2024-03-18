#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"
#include "Renderer/Drawing.h"
#include "Renderer/DepthBuffer.h"
#include "Renderer/Shader.h"

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

	Model model;
	if (model.LoadWavefrontFile("Content/african_head.obj"))
	{
		ScopedImage image(Vec2i(600, 800));
		DepthBuffer depthBuffer(image.Image.GetSize());

		RenderContext context;
		context.Canvas = &image.Image;
		context.DepthBuffer = &depthBuffer;

		Shader_SimpleLitDiffuse shader;

		// build camera matrix
		const Vec3f cameraPos(1.f, 1.f, 3.f);
		const Matrix4x4f cameraMtx = Matrix4x4f::MakeLookAt(cameraPos, Vec3f(), Vec3f::UpVector);
		shader.ViewMatrix = cameraMtx.GetInverse();

		// build projection matrix
		constexpr float nearclip = 0.1f;
		constexpr float farclip = 1000.f;
		constexpr bool bOrthographic = false;
		if (bOrthographic)
		{
			constexpr float orthoWidth = 2.f;
			shader.ProjectionMatrix = Matrix4x4f::MakeOrthographicProjection(orthoWidth, image.Image.GetAspectRatio(), nearclip, farclip);
		}
		else
		{
			constexpr float verticalFieldOfView = GetRadiansFromDegrees(30.f);
			shader.ProjectionMatrix = Matrix4x4f::MakePerspectiveProjection(verticalFieldOfView, image.Image.GetAspectRatio(), nearclip, farclip);
		}

		TGAImage diffuse;
		const bool bDiffuseValid = diffuse.read_tga_file("Content/african_head_diffuse.tga");
		diffuse.flip_vertically();
		if (bDiffuseValid)
		{
			shader.Diffuse = &diffuse;
		}

		shader.BaseColour = white;

		const Vec3f lightDir(1.f, 1.f, 1.f);
		shader.LightDirection = shader.ViewMatrix.TransformVector(lightDir);

		DrawModel(model, context, shader);
		DrawModelWireframe(model, context, shader, white);
	}

	return 0;
}