///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/gtx/gl_texture2d.inl
/// @date 2010-09-27 / 2013-01-13
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <gli/gli.hpp>

namespace glu{
namespace detail
{
	//GL_COMPRESSED_RED, GL_COMPRESSED_RG, GL_COMPRESSED_RGB, GL_COMPRESSED_RGBA, GL_COMPRESSED_SRGB, GL_COMPRESSED_SRGB_ALPHA, 
	//GL_SRGB, GL_SRGB8, GL_SRGB_ALPHA, or GL_SRGB8_ALPHA8
	struct texture_desc
	{
		GLint InternalFormat;
		GLint InternalFormatCompressed;
		GLint InternalFormatSRGB;
		GLint InternalFormatCompressedSRGB;
		GLenum ExternalFormat;
		GLenum ExternalFormatRev;
		GLenum Type;
	};

	//GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, and GL_BGRA.
	//GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, 
	//GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, 
	//GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, 
	//GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, 
	//GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, 
	//GL_UNSIGNED_INT_2_10_10_10_REV

#	ifndef GL_COMPRESSED_RGBA_BPTC_UNORM_ARB
#	define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#	endif

#	ifndef GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB
#	define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#	endif

#	ifndef GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB
#	define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#	endif

#	ifndef GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB
#	define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F
#	endif

	inline texture_desc gli2ogl_cast(gli::format const & Format)
	{
		texture_desc Cast[] = 
		{
			{GL_NONE, GL_NONE, GL_NONE,	GL_NONE, GL_NONE, GL_NONE, GL_NONE},

			//// Normalized
			//{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_BYTE},
			//{GL_RG,		GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_BYTE},
			//{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_BYTE},
			//{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_BYTE},

			//{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_SHORT},
			//{GL_RG,		GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_SHORT},
			//{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_SHORT},
			//{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_SHORT},

			//{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_INT},
			//{GL_RG,		GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_INT},
			//{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_INT},
			//{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_INT},

			// Unsigned
			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_BYTE},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_BYTE},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_BYTE},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_BYTE},

			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_SHORT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_SHORT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_SHORT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_SHORT},

			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_UNSIGNED_INT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_UNSIGNED_INT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_UNSIGNED_INT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_UNSIGNED_INT},

			// Signed
			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_BYTE},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_BYTE},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_BYTE},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_BYTE},

			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_SHORT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_SHORT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_SHORT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_SHORT},

			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_INT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_INT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_INT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_INT},

			// Float
			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_HALF_FLOAT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_HALF_FLOAT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_HALF_FLOAT},

			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_FLOAT},
			{GL_RG,			GL_COMPRESSED_RG,		GL_RG,				GL_COMPRESSED_RG,				GL_RG,			GL_RG,		GL_FLOAT},
			{GL_RGB,		GL_COMPRESSED_RGB,		GL_SRGB8,			GL_COMPRESSED_SRGB,				GL_RGB,			GL_BGR,		GL_FLOAT},
			{GL_RGBA,		GL_COMPRESSED_RGBA,		GL_SRGB8_ALPHA8,	GL_COMPRESSED_SRGB_ALPHA,		GL_RGBA,		GL_BGRA,	GL_FLOAT},

			// Packed
			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_RGB9_E5,	GL_RGB9_E5,				GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_R11F_G11F_B10F,	GL_R11F_G11F_B10F,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_RED,		GL_COMPRESSED_RED,		GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_RGBA4,		GL_RGBA4,				GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},
			{GL_RGB10_A2,	GL_RGB10_A2,			GL_RED,				GL_COMPRESSED_RED,				GL_RED,			GL_RED,		GL_HALF_FLOAT},

			// Depth
			{GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT16,	GL_DEPTH_COMPONENT,		GL_DEPTH_COMPONENT,		GL_UNSIGNED_SHORT},
			{GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT24,	GL_DEPTH_COMPONENT,		GL_DEPTH_COMPONENT,		GL_UNSIGNED_INT},
			{GL_DEPTH24_STENCIL8,	GL_DEPTH24_STENCIL8,	GL_DEPTH24_STENCIL8,	GL_DEPTH24_STENCIL8,	GL_DEPTH_COMPONENT,		GL_DEPTH_STENCIL,		GL_UNSIGNED_INT},
			{GL_DEPTH_COMPONENT32F,	GL_DEPTH_COMPONENT32F,	GL_DEPTH_COMPONENT32F,	GL_DEPTH_COMPONENT32F,	GL_DEPTH_COMPONENT,		GL_DEPTH_COMPONENT,		GL_FLOAT},
			{GL_DEPTH32F_STENCIL8,	GL_DEPTH32F_STENCIL8,	GL_DEPTH32F_STENCIL8,	GL_DEPTH32F_STENCIL8,	GL_DEPTH_COMPONENT,		GL_DEPTH_STENCIL,		GL_UNSIGNED_INT},

			// Compressed formats
			{GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,			GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,			GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,			GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RED_RGTC1,					GL_COMPRESSED_RED_RGTC1,					GL_COMPRESSED_RED_RGTC1,					GL_COMPRESSED_RED_RGTC1,					GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_SIGNED_RED_RGTC1,			GL_COMPRESSED_SIGNED_RED_RGTC1,				GL_COMPRESSED_SIGNED_RED_RGTC1,				GL_COMPRESSED_SIGNED_RED_RGTC1,				GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RG_RGTC2,					GL_COMPRESSED_RG_RGTC2,						GL_COMPRESSED_RG_RGTC2,						GL_COMPRESSED_RG_RGTC2,						GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_SIGNED_RG_RGTC2,				GL_COMPRESSED_SIGNED_RG_RGTC2,				GL_COMPRESSED_SIGNED_RG_RGTC2,				GL_COMPRESSED_SIGNED_RG_RGTC2,				GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,	GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,	GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,	GL_NONE, GL_NONE, GL_NONE},
			{GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,			GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,			GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB,	GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB,	GL_NONE, GL_NONE, GL_NONE},
		};

		return Cast[Format];
	}

}//namespace detail

	inline GLuint create_texture_2d(char const * Filename)
	{
		gli::texture2D Texture(gli::load_dds(Filename));
		if(Texture.empty())
			return 0;

		GLint Alignment(0);
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &Alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLuint Name(0);
		glGenTextures(1, &Name);
		glTextureStorage2DEXT(Name,
			GL_TEXTURE_2D,
			static_cast<GLint>(Texture.levels()),
			static_cast<GLenum>(gli::internal_format(Texture.format())),
			static_cast<GLsizei>(Texture.dimensions().x),
			static_cast<GLsizei>(Texture.dimensions().y));

		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.levels() > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

		if(!gli::is_compressed(Texture.format()))
		{
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glTextureSubImage2DEXT(Name,
					GL_TEXTURE_2D, 
					static_cast<GLint>(Level), 
					0, 0,
					static_cast<GLsizei>(Texture[Level].dimensions().x), 
					static_cast<GLsizei>(Texture[Level].dimensions().y), 
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLenum>(gli::type_format(Texture.format())),
					Texture[Level].data());
			}
		}
		else
		{
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glCompressedTextureSubImage2DEXT(Name,
					GL_TEXTURE_2D,
					static_cast<GLint>(Level),
					0, 0,
					static_cast<GLsizei>(Texture[Level].dimensions().x), 
					static_cast<GLsizei>(Texture[Level].dimensions().y), 
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLsizei>(Texture[Level].size()), 
					Texture[Level].data());
			}
		}

		// Restaure previous states
		glPixelStorei(GL_UNPACK_ALIGNMENT, Alignment);

		return Name;
	}

	inline GLuint create_texture_2d_array(char const * Filename)
	{
		gli::texture2DArray Texture(gli::load_dds(Filename));
		if(Texture.empty())
			return 0;

		GLint Alignment(0);
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &Alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		GLuint Name(0);
		glGenTextures(1, &Name);
		glTextureStorage3DEXT(Name,
			GL_TEXTURE_2D_ARRAY,
			static_cast<GLint>(Texture.levels()),
			static_cast<GLenum>(gli::internal_format(Texture.format())),
			static_cast<GLsizei>(Texture.dimensions().x),
			static_cast<GLsizei>(Texture.dimensions().y),
			static_cast<GLsizei>(Texture.layers()));

		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, Texture.levels() > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

		if(!gli::is_compressed(Texture.format()))
		{
			for(gli::texture2D::size_type Layer(0); Layer < Texture.layers(); ++Layer)
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glTextureSubImage3DEXT(Name,
					GL_TEXTURE_2D_ARRAY,
					static_cast<GLint>(Level),
					0, 0, static_cast<GLint>(Layer),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().x),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().y),
					1,
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLenum>(gli::type_format(Texture.format())),
					Texture[Layer][Level].data());
			}
		}
		else
		{
			for(gli::texture2D::size_type Layer(0); Layer < Texture.layers(); ++Layer)
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glCompressedTextureSubImage3DEXT(Name,
					GL_TEXTURE_2D_ARRAY,
					static_cast<GLint>(Level),
					0, 0, static_cast<GLint>(Layer),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().x),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().y),
					1,
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLsizei>(Texture[Layer][Level].size()), 
					Texture[Layer][Level].data());
			}
		}

		// Restaure previous states
		glPixelStorei(GL_UNPACK_ALIGNMENT, Alignment);

		return Name;
	}

	inline GLuint create_texture_2d(gli::storage const & Storage)
	{
		gli::texture2D Texture(Storage);
		if(Texture.empty())
			return 0;

		GLuint Name(0);
		glGenTextures(1, &Name);
		glTextureStorage2DEXT(Name,
			GL_TEXTURE_2D,
			static_cast<GLint>(Texture.levels()),
			static_cast<GLenum>(gli::internal_format(Texture.format())),
			static_cast<GLsizei>(Texture.dimensions().x),
			static_cast<GLsizei>(Texture.dimensions().y));

		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.levels() > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

		if(!gli::is_compressed(Texture.format()))
		{
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glTextureSubImage2DEXT(Name,
					GL_TEXTURE_2D, 
					static_cast<GLint>(Level), 
					0, 0,
					static_cast<GLsizei>(Texture[Level].dimensions().x), 
					static_cast<GLsizei>(Texture[Level].dimensions().y), 
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLenum>(gli::type_format(Texture.format())),
					Texture[Level].data());
			}
		}
		else
		{
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glCompressedTextureSubImage2DEXT(Name,
					GL_TEXTURE_2D,
					static_cast<GLint>(Level),
					0, 0,
					static_cast<GLsizei>(Texture[Level].dimensions().x), 
					static_cast<GLsizei>(Texture[Level].dimensions().y), 
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLsizei>(Texture[Level].size()), 
					Texture[Level].data());
			}
		}

		return Name;
	}

	inline GLuint create_texture_2d_array(gli::storage const & Storage)
	{
		gli::texture2DArray Texture(Storage);
		if(Texture.empty())
			return 0;

		GLuint Name(0);
		glGenTextures(1, &Name);
		glTextureStorage3DEXT(Name,
			GL_TEXTURE_2D_ARRAY,
			static_cast<GLint>(Texture.levels()),
			static_cast<GLenum>(gli::internal_format(Texture.format())),
			static_cast<GLsizei>(Texture.dimensions().x),
			static_cast<GLsizei>(Texture.dimensions().y),
			static_cast<GLsizei>(Texture.layers()));

		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, Texture.levels() > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTextureParameteriEXT(Name, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

		if(!gli::is_compressed(Texture.format()))
		{
			for(gli::texture2D::size_type Layer(0); Layer < Texture.layers(); ++Layer)
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glTextureSubImage3DEXT(Name,
					GL_TEXTURE_2D_ARRAY,
					static_cast<GLint>(Level),
					0, 0, static_cast<GLint>(Layer),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().x),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().y),
					1,
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLenum>(gli::type_format(Texture.format())),
					Texture[Layer][Level].data());
			}
		}
		else
		{
			for(gli::texture2D::size_type Layer(0); Layer < Texture.layers(); ++Layer)
			for(gli::texture2D::size_type Level(0); Level < Texture.levels(); ++Level)
			{
				glCompressedTextureSubImage3DEXT(Name,
					GL_TEXTURE_2D_ARRAY,
					static_cast<GLint>(Level),
					0, 0, static_cast<GLint>(Layer),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().x),
					static_cast<GLsizei>(Texture[Layer][Level].dimensions().y),
					1,
					static_cast<GLenum>(gli::external_format(Texture.format())),
					static_cast<GLsizei>(Texture[Layer][Level].size()), 
					Texture[Layer][Level].data());
			}
		}

		return Name;
	}

	class scopedPixelStore
	{
	public:
		scopedPixelStore(GLint Alignment) :
			SavedAlignment(0)
		{
			glGetIntegerv(GL_UNPACK_ALIGNMENT, &this->SavedAlignment);
			glPixelStorei(GL_UNPACK_ALIGNMENT, Alignment);
		}

		~scopedPixelStore()
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, this->SavedAlignment);
		}

	private:
		GLint SavedAlignment;
	};

	inline GLuint create_texture(char const * Filename)
	{
		gli::storage Storage(gli::load_dds(Filename));

		scopedPixelStore PixelStore(1);

		if(Storage.layers() > 1)
			return create_texture_2d_array(Storage);
		else
			return create_texture_2d(Storage);
	}

}//namespace gli
