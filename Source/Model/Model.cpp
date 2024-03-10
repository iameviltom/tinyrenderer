#include "Model.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

bool Model::LoadWavefrontFile(const char* FileName)
{
	std::ifstream in;
	in.open(FileName, std::ifstream::in);
	if (in.fail())
	{
		return false;
	}

	Min = Vec3f(FLT_MAX);
	Max = Vec3f(FLT_MAX * -1.f);

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

			Min = Vec3f::Min(Min, v);
			Max = Vec3f::Max(Max, v);
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
