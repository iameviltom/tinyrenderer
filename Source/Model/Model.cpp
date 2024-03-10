#include "Model.h"

#include <string>
#include <fstream>
#include <sstream>
#include "../Maths/Assert.h"

using namespace TV::Renderer;

bool Model::LoadWavefrontFile(const char* FileName)
{
	std::ifstream in;
	in.open(FileName, std::ifstream::in);
	if (in.fail())
	{
		return false;
	}

	Positions.clear();
	TexCoords.clear();
	Normals.clear();
	Tris.clear();

	_Min = Vec3f(FLT_MAX);
	_Max = Vec3f(FLT_MAX * -1.f);

	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			// read a vector
			iss >> trash;
			Vec3f v;
			for (int32 i = 0; i < 3; i++)
			{
				iss >> v.Raw[i];
			}
			Positions.push_back(v);

			_Min = Min(_Min, v);
			_Max = Max(_Max, v);
		}
		else if (!line.compare(0, 3, "vt "))
		{
			// read a tex coord
			iss >> trash;
			iss >> trash;
			Vec2f texCoord;
			for (int32 i = 0; i < 2; i++)
			{
				iss >> texCoord.Raw[i];
			}
			TexCoords.push_back(texCoord);
		}
		else if (!line.compare(0, 3, "vn "))
		{
			// read a normal
			iss >> trash;
			iss >> trash;
			Vec3f normal;
			for (int32 i = 0; i < 3; i++)
			{
				iss >> normal.Raw[i];
			}
			Normals.push_back(normal);
		}
		else if (!line.compare(0, 2, "f "))
		{
			// read a face
			iss >> trash;
			// format is e.g.
			// f 288/270/288 412/402/412 479/470/479
			TriRef tri;
			for (int32 i = 0; i != 3; ++i)
			{
				iss >> tri.Vertices[i].PosIndex >> trash >> tri.Vertices[i].TexCoordIndex >> trash >> tri.Vertices[i].NormalIndex;
				
				// in wavefront obj all indices start at 1, not zero
				--tri.Vertices[i].PosIndex;
				--tri.Vertices[i].TexCoordIndex;
				--tri.Vertices[i].NormalIndex;
			}
			Tris.push_back(tri);
		}
	}

	return IsValid();
}

TV::Maths::Vec3f Model::CalculateNormal(int32 index) const
{
	const Tri tri = GetTri(index);
	return TV::Renderer::CalculateNormal(tri.Vertices[0], tri.Vertices[1], tri.Vertices[2]);
}

TV::Renderer::Model::Tri Model::GetTri(int32 index) const
{
	Tri tri;

	const TriRef& triRef = Tris[index];
	for (int32 i = 0; i != 3; ++i)
	{
		const VertexRef& vertRef = triRef.Vertices[i];
		Vertex& vert = tri.Vertices[i];

		vert.Position = Positions[vertRef.PosIndex];
		vert.TexCoord = TexCoords[vertRef.TexCoordIndex];
		vert.Normal = Normals[vertRef.NormalIndex];
	}

	return tri;
}

TV::Maths::Vec3f TV::Renderer::CalculateNormal(const Vertex& a, const Vertex& b, const Vertex& c)
{
	const Vec3f ba = b.Position - a.Position;
	const Vec3f ca = c.Position - a.Position;

	const Vec3f crossProduct = CrossProduct(ca, ba);
	return crossProduct.GetSafeNormal();
}
