#include "Shader.h"

TV::Renderer::ShaderImplementation_Example::VertexOutput TV::Renderer::ShaderImplementation_Example::VertexShader(const IShader& shader, const Vertex& input) const
{
	VertexOutput output;

	const Vec3f worldSpacePosition = shader.ModelMatrix.TransformPosition(input.Position);
	const Vec3f cameraSpacePosition = shader.ViewMatrix.TransformPosition(worldSpacePosition);
	output.Position = shader.ProjectionMatrix.TransformVector4(Vec4f(cameraSpacePosition, 1.f));

	return output;
}

TV::Renderer::ShaderImplementation_Example::VertexOutput TV::Renderer::ShaderImplementation_Example::Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const
{
	VertexOutput output;
	output.Position = ComputeValueFromBarycentric(barycentricCoord, a.Position, b.Position, c.Position);
	return output;
}

TV::Maths::Colour TV::Renderer::ShaderImplementation_Example::FragmentShader(const IShader& shader, const VertexOutput& input) const
{
	return Colour(255, 255, 255, 255);
}

TV::Renderer::ShaderImplementation_SimpleLitDiffuse::VertexOutput TV::Renderer::ShaderImplementation_SimpleLitDiffuse::VertexShader(const IShader& shader, const Vertex& input) const
{
	VertexOutput output;

	// output position in clip space
	{
		const Vec3f worldSpacePosition = shader.ModelMatrix.TransformPosition(input.Position);
		const Vec3f cameraSpacePosition = shader.ViewMatrix.TransformPosition(worldSpacePosition);
		output.Position = shader.ProjectionMatrix.TransformVector4(Vec4f(cameraSpacePosition, 1.f));
	}

	// output normal in camera space
	{
		const Vec3f worldSpaceNormal = shader.ModelMatrix.TransformVector(input.Normal);
		output.Normal = shader.ViewMatrix.TransformVector(worldSpaceNormal);
		output.Normal = output.Normal.GetSafeNormal();
	}

	// copy tex coord
	output.TexCoord = input.TexCoord;

	return output;
}

TV::Renderer::ShaderImplementation_SimpleLitDiffuse::VertexOutput TV::Renderer::ShaderImplementation_SimpleLitDiffuse::Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const
{
	VertexOutput output;
	output.Position = ComputeValueFromBarycentric(barycentricCoord, a.Position, b.Position, c.Position);
	output.Normal = ComputeValueFromBarycentric(barycentricCoord, a.Normal, b.Normal, c.Normal).GetSafeNormal();
	output.TexCoord = ComputeValueFromBarycentric(barycentricCoord, a.TexCoord, b.TexCoord, c.TexCoord);
	return output;
}

TV::Maths::Colour TV::Renderer::ShaderImplementation_SimpleLitDiffuse::FragmentShader(const IShader& shader, const VertexOutput& input) const
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
