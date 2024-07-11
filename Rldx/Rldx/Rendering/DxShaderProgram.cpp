#include "DxShaderProgram.h"


std::vector<std::string::iterator> find_all(std::string& s, char c)
{
	std::vector<std::string::iterator> res;
	for (auto p = s.begin(); p != s.end(); ++p)
		if (*p == c)
			res.push_back(p);
	return res;
}

std::vector<std::string::const_iterator> find_all2(const std::string& s, char c)
{
	std::vector<std::string::const_iterator> res;
	for (auto p = s.begin(); p != s.end(); ++p)
		if (*p == c)
			res.push_back(p);
	return res;
}

rldx::IntId rldx::DefaultShaderCreator::simpleShader = INVALID_ID;
rldx::IntId rldx::DefaultShaderCreator::noTextureShader = INVALID_ID;
