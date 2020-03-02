#ifndef RES_TEXTURE_H
#define RES_TEXTURE_H

#include <iostream>

namespace Resources 
{
	enum RTextureMapType
	{
		NONE = 0,
		DIFFUSE_MAP = 1,
		SPECULAR_MAP = 2,
		EMISSIVE_MAP = 4,
		HEIGHT_MAP = 5,
		NORMAL_MAP = 6,
		ALPHA_MAP = 7
	};

	class RTexture
	{
	public:
		RTexture();
		~RTexture();

		unsigned int GetId() { return this->_id; }
		unsigned int GetWidth() { return this->_width; }
		unsigned int GetHeight() { return this->_height; }
		unsigned char* GetTextureData() { return this->_data; }

	private:
		std::string filename;
		std::string _type = "";
		unsigned int _id = 0;
		unsigned int _width = 0;
		unsigned int _height = 0;

		unsigned char* _data;
		unsigned char* _hash;
	};
}

#endif