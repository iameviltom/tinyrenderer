#include "Model.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace TV::Renderer;

bool Model::LoadWavefrontFile(const char* FileName)
{
	std::ifstream in;
	in.open(FileName, std::ifstream::in);
	if (in.fail())
	{
		return false;
	}

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
			for (int i = 0; i < 3; i++)
			{
				iss >> v.Raw[i];
			}
			Vertices.push_back(v);

			_Min = Min(_Min, v);
			_Max = Max(_Max, v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			// read a face
			int itrash, idx;
			iss >> trash;
			// format is e.g.
			// f 288/270/288 412/402/412 479/470/479
			// we only care about the first index in each set of 3
			Tri tri;
			for (int i = 0; i != 3; ++i)
			{
				iss >> idx >> trash >> itrash >> trash >> itrash;
				tri.VertIndex[i] = idx - 1; // in wavefront obj all indices start at 1, not zero
			}
			Tris.push_back(tri);
		}
	}

	return IsValid();
}

TV::Maths::Vec3f Model::CalculateNormal(int32 index) const
{
	const Tri& tri = GetTri(index);

	const Vec3f a = GetVertex(tri.VertIndex[0]);
	const Vec3f b = GetVertex(tri.VertIndex[1]);
	const Vec3f c = GetVertex(tri.VertIndex[2]);

	const Vec3f ba = b - a;
	const Vec3f ca = c - a;

	const Vec3f crossProduct = CrossProduct(ca, ba);
	return crossProduct.GetSafeNormal();
}
