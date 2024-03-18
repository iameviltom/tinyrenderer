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

	std::vector<Vec3f> positions;
	std::vector<Vec2f> texCoords;
	std::vector<Vec3f> normals;

	struct VertexRef
	{
		int32 PosIndex = 0;
		int32 TexCoordIndex = 0;
		int32 NormalIndex = 0;

		bool operator == (const VertexRef& other) const
		{
			return PosIndex == other.PosIndex && TexCoordIndex == other.TexCoordIndex && NormalIndex == other.NormalIndex;
		}
	};
	struct TriRef
	{
		VertexRef Vertices[3];
	};
	std::vector<TriRef> tris;

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
			positions.push_back(v);

			_Min = GetMin(_Min, v);
			_Max = GetMax(_Max, v);
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
			texCoords.push_back(texCoord);
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
			normals.push_back(normal);
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
			tris.push_back(tri);
		}
	}

	// now convert to more standard format with vertex data grouped together
	std::vector<VertexRef> uniqueVertices;
	for (const TriRef& triRef : tris)
	{
		Tri tri;

		for (int32 triPointIndex = 0; triPointIndex != 3; ++triPointIndex)
		{
			const VertexRef& vertRef = triRef.Vertices[triPointIndex];

			// check if vertex already exists
			int32 index = -1;
			for (int32 existingIndex = 0; existingIndex != uniqueVertices.size(); ++existingIndex)
			{
				if (uniqueVertices[existingIndex] == vertRef)
				{
					index = existingIndex;
					break;
				}
			}
			if (index == -1)
			{
				index = (int32)uniqueVertices.size();
				uniqueVertices.push_back(vertRef);
			}

			tri.VertexIndex[triPointIndex] = index;
		}

		Triangles.push_back(tri);
	}
	for (const VertexRef& uniqueVertex : uniqueVertices)
	{
		Vertex vertex;
		vertex.Position = positions[uniqueVertex.PosIndex];
		vertex.TexCoord = texCoords[uniqueVertex.TexCoordIndex];
		vertex.Normal = normals[uniqueVertex.NormalIndex];
		Vertices.push_back(vertex);
	}

	return IsValid();
}

TV::Maths::Vec3f Model::CalculateNormal(int32 triIndex) const
{
	const Tri& tri = GetTri(triIndex);
	return TV::Renderer::CalculateNormal(GetVertex(tri.VertexIndex[0]), GetVertex(tri.VertexIndex[1]), GetVertex(tri.VertexIndex[2]));
}