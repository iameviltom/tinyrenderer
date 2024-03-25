#include "Image/TgaImage.h"

#include "Maths/Vec2.h"
#include "Model/Model.h"
#include "Renderer/DepthBuffer.h"
#include "Renderer/Shader.h"
#include "Shaders/Shader_SimpleLitDiffuse.h"

using namespace TV;
using namespace Maths;
using namespace Renderer;

constexpr Colour white = Colour(255, 255, 255, 255);
constexpr Colour red = Colour(255, 0, 0, 255);
constexpr Colour green = Colour(0, 255, 0, 255);

int main(int argc, char** argv)
{
	Model model;
	if (model.LoadWavefrontFile("Content/african_head.obj"))
	{
		constexpr int32 imageHeight = 800;
		constexpr int32 imageWidth = 600;
		constexpr float aspectRatio = imageWidth / (float)imageHeight;

		TV::Shaders::Shader_SimpleLitDiffuse shader;

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
			shader.ProjectionMatrix = Matrix4x4f::MakeOrthographicProjection(orthoWidth, aspectRatio, nearclip, farclip);
		}
		else
		{
			constexpr float verticalFieldOfView = GetRadiansFromDegrees(30.f);
			shader.ProjectionMatrix = Matrix4x4f::MakePerspectiveProjection(verticalFieldOfView, aspectRatio, nearclip, farclip);
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

		{
			TGAImage image(imageWidth, imageHeight, TGAImage::RGB);
			DepthBuffer depthBuffer(image.GetSize());

			RenderContext context;
			context.Canvas = &image;
			context.DepthBuffer = &depthBuffer;

			shader.DrawModel(model, context);

			image.flip_vertically();
			image.write_tga_file("output.tga");
		}

		{
			TGAImage image(imageWidth, imageHeight, TGAImage::RGB);

			RenderContext context;
			context.Canvas = &image;

			shader.DrawModelWireframe(model, context, white);

			image.flip_vertically();
			image.write_tga_file("output_wireframe.tga");
		}
	}

	return 0;
}