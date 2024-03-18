#include "Shader.h"

#include "ICanvas.h"
#include "../Maths/Geometry.h"

TV::Renderer::VertexOutput_SimpleLitDiffuse TV::Renderer::VertexOutput_SimpleLitDiffuse::Interpolate(const Vec3f& barycentricCoord, const VertexOutput_SimpleLitDiffuse& a, const VertexOutput_SimpleLitDiffuse& b, const VertexOutput_SimpleLitDiffuse& c)
{
	VertexOutput_SimpleLitDiffuse output;
	output.Position = ComputeValueFromBarycentric(barycentricCoord, a.Position, b.Position, c.Position);
	output.Normal = ComputeValueFromBarycentric(barycentricCoord, a.Normal, b.Normal, c.Normal).GetSafeNormal();
	output.TexCoord = ComputeValueFromBarycentric(barycentricCoord, a.TexCoord, b.TexCoord, c.TexCoord);
	return output;
}

TV::Renderer::VertexOutput_SimpleLitDiffuse TV::Renderer::Shader_SimpleLitDiffuse::VertexShader(const Vertex& input)
{
	VertexOutput_SimpleLitDiffuse output;

	// output position in clip space
	{
		const Vec3f worldSpacePosition = ModelMatrix.TransformPosition(input.Position);
		const Vec3f cameraSpacePosition = ViewMatrix.TransformPosition(worldSpacePosition);
		output.Position = ProjectionMatrix.TransformVector4(Vec4f(cameraSpacePosition, 1.f));
	}

	// output normal in camera space
	{
		const Vec3f worldSpaceNormal = ModelMatrix.TransformVector(input.Normal);
		output.Normal = ViewMatrix.TransformVector(worldSpaceNormal);
		output.Normal = output.Normal.GetSafeNormal();
	}

	// copy tex coord
	output.TexCoord = input.TexCoord;

	return output;
}

TV::Maths::Colour TV::Renderer::Shader_SimpleLitDiffuse::FragmentShader(const VertexOutput_SimpleLitDiffuse& input)
{
	Colour output;

	output = BaseColour;

	if (Diffuse != nullptr)
	{
		const Vec2i texSample(GetRoundToInt(input.TexCoord.X * Diffuse->GetSize().X), GetRoundToInt(input.TexCoord.Y * Diffuse->GetSize().Y));
		output = Diffuse->GetPixel(texSample);
		output.A = 255; // todo: no alpha channel in sample image, need a way to handle this
	}

	const double lightIntensity = GetDotProduct(LightDirection, input.Normal);
	output = output.Scaled(GetMax(0.0, lightIntensity));

	return output;
}
