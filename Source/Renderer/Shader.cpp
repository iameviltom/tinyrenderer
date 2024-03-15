#include "Shader.h"

#include "ICanvas.h"

TV::Renderer::VertexShaderOutput TV::Renderer::Shader::VertexShader(const VertexShaderInput& input, const VertexShaderConstants& constants) const
{
	VertexShaderOutput output;

	// output position in clip space
	{
		const Vec3f worldSpacePosition = constants.ModelMatrix.TransformPosition(input.Position);
		const Vec3f cameraSpacePosition = constants.ViewMatrix.TransformPosition(worldSpacePosition);
		output.Position = constants.ProjectionMatrix.TransformVector4(Vec4f(cameraSpacePosition, 1.f));
	}

	// output normal in camera space
	{
		const Vec3f worldSpaceNormal = constants.ModelMatrix.TransformVector(input.Normal);
		output.Normal = constants.ViewMatrix.TransformVector(worldSpaceNormal);
		output.Normal = output.Normal.GetSafeNormal();
	}

	// copy tex coord
	output.TexCoord = input.TexCoord;

	return output;
}

TV::Renderer::PixelShaderOutput TV::Renderer::Shader::PixelShader(const PixelShaderInput& input, const PixelShaderConstants& constants) const
{
	PixelShaderOutput output;

	output.Colour = input.Colour;

	if (constants.Diffuse != nullptr)
	{
		const Vec2i texSample(GetRoundToInt(input.TexCoord.X * constants.Diffuse->GetSize().X), GetRoundToInt(input.TexCoord.Y * constants.Diffuse->GetSize().Y));
		output.Colour = constants.Diffuse->GetPixel(texSample);
	}

	const double lightIntensity = GetDotProduct(constants.LightDirection, input.Normal);
	output.Colour = output.Colour.Scaled(GetMax(0.0, lightIntensity));

	return output;
}
