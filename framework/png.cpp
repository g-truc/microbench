#include "png.hpp"
#include <FreeImage.h>
#include <cstdlib>

namespace
{
	static void FreeImageFree()
	{
		FreeImage_DeInitialise();
	}


	static void FreeImageInit()
	{
		static bool Init = false;

		if(!Init)
		{
			Init = true;
			FreeImage_Initialise(false);
			atexit(FreeImageFree);
		}
	}
}//namespace

/// Loading a PNG file
gli::texture load_png(char const* Filename)
{
	FreeImageInit();

	FIBITMAP * Bitmap = FreeImage_Load(FIF_PNG, Filename, 0);
	if(!Bitmap)
		return gli::texture();

	glm::uint BPP = FreeImage_GetBPP(Bitmap);
	glm::uint Width = FreeImage_GetWidth(Bitmap);
	glm::uint Height = FreeImage_GetHeight(Bitmap);

	gli::texture Texture(gli::TARGET_2D, BPP == 24 ? gli::FORMAT_RGB8_UNORM_PACK8 : gli::FORMAT_RGBA8_UNORM_PACK8, gli::texture::extent_type(Width, Height, 1), 1, 1, 1);
	memcpy(Texture.data(), FreeImage_GetBits(Bitmap), Texture.size());
	FreeImage_Unload(Bitmap);

	switch(gli::component_count(Texture.format()))
	{
	default:
		assert(0);
		break;
	case 3:
		for(std::size_t Offset = 0; Offset < Texture.size() / 3; ++Offset)
		{
			glm::u8vec3 Src = *(static_cast<glm::u8vec3 const *>(Texture.data()) + Offset);
			*(static_cast<glm::u8vec3*>(Texture.data()) + Offset) = glm::u8vec3(Src.z, Src.y, Src.x);
		}
		break;
	case 4:
		for(std::size_t Offset = 0; Offset < Texture.size() / 4; ++Offset)
		{
			glm::u8vec4 Src = *(static_cast<glm::u8vec4 const *>(Texture.data()) + Offset);
			*(static_cast<glm::u8vec4*>(Texture.data()) + Offset) = glm::u8vec4(Src.z, Src.y, Src.x, Src.w);
		}
		break;
	}

	return Texture;
}

void save_png(gli::texture const& Texture, char const* Filename)
{
	gli::texture Copy(gli::duplicate(Texture));

	switch(gli::component_count(Copy.format()))
	{
	default:
		assert(0);
		break;
	case 3:
		for(std::size_t Offset = 0; Offset < Copy.size() / 3; ++Offset)
		{
			glm::u8vec3 Src = *(static_cast<glm::u8vec3 const *>(Copy.data()) + Offset);
			*(static_cast<glm::u8vec3*>(Copy.data()) + Offset) = glm::u8vec3(Src.z, Src.y, Src.x);
		}
		break;
	case 4:
		for(std::size_t Offset = 0; Offset < Copy.size() / 4; ++Offset)
		{
			glm::u8vec4 Src = *(static_cast<glm::u8vec4 const *>(Copy.data()) + Offset);
			*(static_cast<glm::u8vec4*>(Copy.data()) + Offset) = glm::u8vec4(Src.z, Src.y, Src.x, Src.w);
		}
		break;
	}

	FreeImageInit();

	FIBITMAP* Bitmap = FreeImage_ConvertFromRawBits(
		Copy.data<BYTE>(),
		Copy.extent().x, Copy.extent().y,
		static_cast<int>(Copy.extent().x * gli::component_count(Copy.format())),
		static_cast<unsigned int>(gli::component_count(Copy.format()) * 8), 0x0000FF, 0x00FF00, 0xFF0000, false);

	BOOL Result = FreeImage_Save(FIF_PNG, Bitmap, Filename, 0);
	assert(Result);

	FreeImage_Unload(Bitmap);
}

