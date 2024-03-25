#include "Shader_Example.h"

TV::Shaders::ShaderImplementation_Example::VertexOutput TV::Shaders::ShaderImplementation_Example::VertexShader(const IShader& shader, const Vertex& input) const
{
	VertexOutput output;

	const Vec3f worldSpacePosition = shader.ModelMatrix.TransformPosition(input.Position);
	const Vec3f cameraSpacePosition = shader.ViewMatrix.TransformPosition(worldSpacePosition);
	output.Position = shader.ProjectionMatrix.TransformVector4(Vec4f(cameraSpacePosition, 1.f));

	return output;
}

TV::Shaders::ShaderImplementation_Example::VertexOutput TV::Shaders::ShaderImplementation_Example::Interpolate(const Vec3f& barycentricCoord, const VertexOutput& a, const VertexOutput& b, const VertexOutput& c) const
{
	VertexOutput output;
	output.Position = ComputeValueFromBarycentric(barycentricCoord, a.Position, b.Position, c.Position);
	return output;
}

TV::Maths::Colour TV::Shaders::ShaderImplementation_Example::FragmentShader(const IShader& shader, const VertexOutput& input) const
{
	return Colour(255, 255, 255, 255);
}