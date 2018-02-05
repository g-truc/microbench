#include "test.hpp"
#include "csv.hpp"

namespace
{
	char const* VERT_SHADER_SOURCE = R"(
		#version 430 core

		out gl_PerVertex
		{
			vec4 gl_Position;
		};

		void main()
		{	
			gl_Position = gl_Position = vec4(4.f * (gl_VertexID % 2) - 1.f, 4.f * (gl_VertexID / 2) - 1.f, 0.0, 1.0);
		}
	)";

	char const* FRAG_SHADER_SOURCE = R"(
		#define FRAG_COLOR	0

#		ifndef FETCH_COUNT
#			error FETCH_COUNT must be defined to a value between 1 and 8 included
#		endif

#		ifdef SAMPLER_1D
			layout(binding = 0) uniform sampler1D Texture[FETCH_COUNT];
#		elif defined(SAMPLER_2D)
			layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];
#		elif defined(SAMPLER_3D)
			layout(binding = 0) uniform sampler3D Texture[FETCH_COUNT];
#		endif

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		void main()
		{
			Color = vec4(0);

			for(int i = 0; i < FETCH_COUNT; ++i)
			{
#				if defined(SAMPLER_1D)
					Color += texture(Texture[i], 0.0) * (float(1.0) / float(FETCH_COUNT));
#				elif defined(SAMPLER_2D)
					Color += texture(Texture[i], vec2(0.0)) * (float(1.0) / float(FETCH_COUNT));
#				elif defined(SAMPLER_3D)
					Color += texture(Texture[i], vec3(0.0)) * (float(1.0) / float(FETCH_COUNT));
#				endif
			}
		}
	)";
}//namespace

class sample_texture_fetch : public framework
{
public:
	enum format
	{
		FORMAT_R8_UNORM, FORMAT_FIRST = FORMAT_R8_UNORM,
		FORMAT_RGBA8_UNORM,
		FORMAT_RGB9E5UF,
		FORMAT_RG11B10UF,
		FORMAT_RGBA16F,
		FORMAT_RGBA32F, FORMAT_LAST = FORMAT_RGBA32F
	};

	enum
	{
		FORMAT_COUNT = FORMAT_LAST - FORMAT_FIRST + 1
	};

	char const* GetString(format Format) const
	{
		static char const* Table[]
		{
			"r8unorm",
			"rgba8unorm",
			"rgb9e5uf",
			"rg11b10uf",
			"rgba16f",
			"rgba32f"
		};
		return Table[Format];
	}

	enum filter
	{
		FILTER_NEAREST, FILTER_FIRST = FILTER_NEAREST,
		FILTER_BILINEAR,
		FILTER_TRILINEAR, FILTER_LAST = FILTER_TRILINEAR
	};

	enum
	{
		FILTER_COUNT = FILTER_LAST - FILTER_FIRST + 1
	};

	char const* GetString(filter Filter) const
	{
		static char const* Table[]
		{
			"nearest",
			"bilinear",
			"trilinear"
		};
		return Table[Filter];
	}

	enum sampler
	{
		SAMPLER_1D, SAMPLER_FIRST = SAMPLER_1D,
		SAMPLER_2D,
		SAMPLER_3D, SAMPLER_LAST = SAMPLER_3D
	};

	enum
	{
		SAMPLER_COUNT = SAMPLER_LAST - SAMPLER_FIRST + 1
	};

	char const* GetString(sampler Sampler) const
	{
		static char const* Table[]
		{
			"2D",
			"3D",
		};
		return Table[Sampler];
	}

	sample_texture_fetch(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, int FetchCount, format Format, sampler Sampler, filter Filter, int AnisoSamples)
		: framework(argc, argv, "texture-fetch", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, FetchCount(FetchCount)
		, Format(Format)
		, Sampler(Sampler)
		, Filter(Filter)
		, AnisoSamples(AnisoSamples)
		, TextureLocation(0)
		, PipelineName(0)
		, ProgramName(0)
		, VertexArrayName(0)
		, TimerQueryIndex(0)
		, TimerQueryCount(Frames)
		, TimerQueryCompleted(false)
	{}

private:
	csv& CSV;
	const int FetchCount;
	const format Format;
	const sampler Sampler;
	const filter Filter;
	const int AnisoSamples;
	std::vector<GLuint> TextureName;
	std::vector<GLuint> BufferName;
	GLint TextureLocation;
	GLuint PipelineName;
	GLuint ProgramName;
	GLuint VertexArrayName;
	std::vector<GLuint> TimerQueryName;
	std::size_t TimerQueryIndex;
	std::size_t TimerQueryCount;
	bool TimerQueryCompleted;

	bool initProgram()
	{
		GLuint VertShaderName = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertShaderName, 1, &VERT_SHADER_SOURCE, NULL);
		glCompileShader(VertShaderName);

		std::string FragShaderSource;
		FragShaderSource += "#version 430 core \n";

		FragShaderSource += ::format("#define FETCH_COUNT %d \n", this->FetchCount);

		switch (this->Sampler)
		{
		case SAMPLER_1D:
			FragShaderSource += "#define SAMPLER_1D \n";
			break;
		case SAMPLER_2D:
			FragShaderSource += "#define SAMPLER_2D \n";
			break;
		case SAMPLER_3D:
			FragShaderSource += "#define SAMPLER_3D \n";
			break;
		default:
			assert(0);
			break;
		}

		FragShaderSource += FRAG_SHADER_SOURCE;
		char const* FragShaderSourcePointer = FragShaderSource.c_str();

		GLuint FragShaderName = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragShaderName, 1, &FragShaderSourcePointer, NULL);
		glCompileShader(FragShaderName);

		GLint VertResult = GL_FALSE;
		glGetShaderiv(VertShaderName, GL_COMPILE_STATUS, &VertResult);

		GLint FragResult = GL_FALSE;
		glGetShaderiv(FragShaderName, GL_COMPILE_STATUS, &FragResult);

		if (VertResult == GL_FALSE)
		{
			int VertInfoLogLength;
			glGetShaderiv(VertShaderName, GL_INFO_LOG_LENGTH, &VertInfoLogLength);
			if (VertInfoLogLength > 0)
			{
				std::vector<char> Buffer(VertInfoLogLength);
				glGetShaderInfoLog(VertShaderName, VertInfoLogLength, NULL, &Buffer[0]);
				fprintf(stdout, "%s\n", &Buffer[0]);
			}
		}

		if (FragResult == GL_FALSE)
		{
			int FragInfoLogLength;
			glGetShaderiv(FragShaderName, GL_INFO_LOG_LENGTH, &FragInfoLogLength);
			if (FragInfoLogLength > 0)
			{
				fprintf(stdout, "%s\n", FragShaderSourcePointer);

				std::vector<char> Buffer(FragInfoLogLength);
				glGetShaderInfoLog(FragShaderName, FragInfoLogLength, NULL, &Buffer[0]);
				fprintf(stdout, "%s\n", &Buffer[0]);
			}
		}

		ProgramName = glCreateProgram();
		glProgramParameteri(ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(ProgramName, VertShaderName);
		glAttachShader(ProgramName, FragShaderName);
		glLinkProgram(ProgramName);

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			int InfoLogLength;
			glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0)
			{
				fprintf(stdout, "%s\n", FragShaderSourcePointer);

				std::vector<char> Buffer(glm::max(InfoLogLength, int(1)));
				glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
				fprintf(stdout, "%s\n", &Buffer[0]);
				return false;
			}
		}

		glDeleteShader(VertShaderName);
		glDeleteShader(FragShaderName);

		glGenProgramPipelines(1, &PipelineName);
		glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);

		return true;
	}

	GLenum GetTarget(sampler Sampler)
	{
		switch(this->Sampler)
		{
		case SAMPLER_1D:
			return GL_TEXTURE_1D;
		case SAMPLER_2D:
			return GL_TEXTURE_2D;
		case SAMPLER_3D:
			return GL_TEXTURE_3D;
		default:
			assert(0);
			return GL_NONE;
		}
	}

	bool initTexture()
	{
		glm::ivec2 WindowSize(this->getWindowSize());

		this->TextureName.resize(this->FetchCount);

		glGenTextures(this->FetchCount, &TextureName[0]);

		GLenum const Target = GetTarget(this->Sampler);

		for (int i = 0; i < this->FetchCount; ++i)
		{
			glBindTexture(Target, TextureName[i]);
			glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(Target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(Target, GL_TEXTURE_MAX_ANISOTROPY, this->AnisoSamples);

			switch(Filter)
			{
			case FILTER_NEAREST:
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case FILTER_BILINEAR:
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case FILTER_TRILINEAR:
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			default:
				assert(0);
				break;
			}

			glm::dvec4 const Pixel_f64vec4 = glm::dvec4(1.0f, 0.5f, 0.0f, 1.0f);
			glm::vec4 const Pixel_f32vec4 = glm::vec4(Pixel_f64vec4);
			glm::uint const Packed_u8vec4 = glm::packUnorm4x8(Pixel_f32vec4);
			glm::uint const Packed_rgb9e5uf = glm::packF3x9_E1x5(glm::vec3(Pixel_f32vec4));
			glm::uint const Packed_rg11b10uf = glm::packF2x11_1x10(glm::vec3(Pixel_f32vec4));
			glm::u8vec4 const Pixel_u8vec4 = glm::u8vec4(Pixel_f32vec4 * float(std::numeric_limits<std::uint8_t>::max()));
			glm::u32vec4 const Pixel_u32vec4 = glm::u32vec4(Pixel_f64vec4 * double(std::numeric_limits<std::uint32_t>::max()));

			switch(this->Sampler)
			{
			case SAMPLER_1D:
				switch(this->Format)
				{
				case FORMAT_R8_UNORM:
					glTexStorage1D(Target, GLint(1), GL_R8, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RED, GL_UNSIGNED_BYTE, &Pixel_u8vec4[0]);
					break;
				case FORMAT_RGBA8_UNORM:
					glTexStorage1D(Target, GLint(1), GL_RGBA8, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Packed_u8vec4);
					break;
				case FORMAT_RGB9E5UF:
					glTexStorage1D(Target, GLint(1), GL_RGB9_E5, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, &Packed_rgb9e5uf);
					break;
				case FORMAT_RG11B10UF:
					glTexStorage1D(Target, GLint(1), GL_R11F_G11F_B10F, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, &Packed_rg11b10uf);
					break;
				case FORMAT_RGBA16F:
					glTexStorage1D(Target, GLint(1), GL_RGBA16F, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				case FORMAT_RGBA32F:
					glTexStorage1D(Target, GLint(1), GL_RGBA32F, 1);
					glTexSubImage1D(Target, 0, 0, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				default:
					assert(0);
					break;
				}
				break;
			case SAMPLER_2D:
				switch(this->Format)
				{
				case FORMAT_R8_UNORM:
					glTexStorage2D(Target, GLint(1), GL_R8, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &Pixel_u8vec4[0]);
					break;
				case FORMAT_RGBA8_UNORM:
					glTexStorage2D(Target, GLint(1), GL_RGBA8, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Packed_u8vec4);
					break;
				case FORMAT_RGB9E5UF:
					glTexStorage2D(Target, GLint(1), GL_RGB9_E5, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, &Packed_rgb9e5uf);
					break;
				case FORMAT_RG11B10UF:
					glTexStorage2D(Target, GLint(1), GL_R11F_G11F_B10F, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, &Packed_rg11b10uf);
					break;
				case FORMAT_RGBA16F:
					glTexStorage2D(Target, GLint(1), GL_RGBA16F, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				case FORMAT_RGBA32F:
					glTexStorage2D(Target, GLint(1), GL_RGBA32F, 1, 1);
					glTexSubImage2D(Target, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				default:
					assert(0);
					break;
				}
				break;
			case SAMPLER_3D:
				switch(this->Format)
				{
				case FORMAT_R8_UNORM:
					glTexStorage3D(Target, GLint(1), GL_R8, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &Pixel_u8vec4[0]);
					break;
				case FORMAT_RGBA8_UNORM:
					glTexStorage3D(Target, GLint(1), GL_RGBA8, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Packed_u8vec4);
					break;
				case FORMAT_RGB9E5UF:
					glTexStorage3D(Target, GLint(1), GL_RGB9_E5, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RGB, GL_UNSIGNED_INT_5_9_9_9_REV, &Packed_rgb9e5uf);
					break;
				case FORMAT_RG11B10UF:
					glTexStorage3D(Target, GLint(1), GL_R11F_G11F_B10F, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV, &Packed_rg11b10uf);
					break;
				case FORMAT_RGBA16F:
					glTexStorage3D(Target, GLint(1), GL_RGBA16F, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				case FORMAT_RGBA32F:
					glTexStorage3D(Target, GLint(1), GL_RGBA32F, 1, 1, 1);
					glTexSubImage3D(Target, 0, 0, 0, 0, 1, 1, 1, GL_RGBA, GL_FLOAT, &Pixel_f32vec4[0]);
					break;
				default:
					assert(0);
					break;
				}
				break;
			default:
				assert(0);
				break;
			}
		}

		return true;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
		glBindVertexArray(0);

		return true;
	}

	bool initQuery()
	{
		this->TimerQueryName.resize(this->TimerQueryCount > 0 ? this->TimerQueryCount : 1000);

		glGenQueries(static_cast<GLsizei>(TimerQueryName.size()), &TimerQueryName[0]);

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if (Validated)
			Validated = initTexture();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();
		if (Validated)
			Validated = initQuery();

		if (Validated)
		{
			glm::vec2 WindowSize(this->getWindowSize());
			glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

			glBindProgramPipeline(PipelineName);
			glBindVertexArray(VertexArrayName);

			for (int i = 0; i < this->FetchCount; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GetTarget(this->Sampler), this->TextureName[i]);
			}
		}

		return Validated;
	}

	bool end()
	{
		std::string Title = this->title() + ::format(" | Fetch(%d)", this->FetchCount);

		if (this->TimerQueryCompleted)
		{
			GLuint64 TimeMin = ~0;
			GLuint64 TimeMax = 0;
			GLuint64 TimeSum = 0;
			for (std::size_t i = 0; i < this->TimerQueryName.size(); ++i)
			{
				GLuint64 Time = 0;
				glGetQueryObjectui64v(this->TimerQueryName[i], GL_QUERY_RESULT, &Time);
				TimeMin = std::min(TimeMin, Time);
				TimeMax = std::max(TimeMax, Time);
				TimeSum += Time;
			}

			GLuint64 const TimeAvg = TimeSum /= static_cast<GLuint64>(this->TimerQueryName.size());

			fprintf(stdout, "%s | %d %s | Average: %.0f us, Min: %.0f us, Max: %.0f us\n", Title.c_str(),
				this->FetchCount, GetString(this->Format),
				static_cast<double>(TimeAvg) / 1000.0,
				static_cast<double>(TimeMin) / 1000.0,
				static_cast<double>(TimeMax) / 1000.0);

			CSV.log(::format("%s ; %d ; %s ; %s",
				this->title(), this->FetchCount, GetString(this->Filter), GetString(this->Format)).c_str(),
				static_cast<double>(TimeAvg) / 1000.0, static_cast<double>(TimeMin) / 1000.0, static_cast<double>(TimeMax) / 1000.0);
		}
		else
			fprintf(stdout, "%s | Not enough frames to report rendering time\n", Title.c_str());

		glDeleteQueries(static_cast<GLsizei>(TimerQueryName.size()), &TimerQueryName[0]);
		glDeleteTextures(this->FetchCount, &TextureName[0]);
		glDeleteProgram(ProgramName);
		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return true;
	}

	bool render()
	{
		glBeginQuery(GL_TIME_ELAPSED, this->TimerQueryName[this->TimerQueryIndex]);
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);
		glEndQuery(GL_TIME_ELAPSED);

		++this->TimerQueryIndex;
		if (this->TimerQueryIndex >= this->TimerQueryName.size())
		{
			this->TimerQueryIndex = 0;
			this->TimerQueryCompleted = true;
		}

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error = 0;

	csv CSV;
	CSV.header("texture-fetch ; FetchCount ; Filter ; Format");

	std::size_t const Frames = 1000;
	glm::uvec2 const WindowSize(2400, 1200);

	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, 0,
			16, sample_texture_fetch::FORMAT_RGBA16F, sample_texture_fetch::SAMPLER_3D, sample_texture_fetch::FILTER_TRILINEAR, 16);
		Error += Test();
	}

	sample_texture_fetch::format const Formats[] = {
		sample_texture_fetch::FORMAT_RGBA8_UNORM,
		sample_texture_fetch::FORMAT_RG11B10UF,
		sample_texture_fetch::FORMAT_RGBA16F,
		sample_texture_fetch::FORMAT_RGBA32F
	};
	int const FormatCount = sizeof(Formats) / sizeof(Formats[0]);

	for (int Filter = 0; Filter < sample_texture_fetch::FILTER_COUNT; ++Filter)
	for (int FormatIndex = 0; FormatIndex < FormatCount; ++FormatIndex)
	for (int FetchCount = 1; FetchCount <= 32; FetchCount <<= 1)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames,
			FetchCount, Formats[FormatIndex], sample_texture_fetch::SAMPLER_2D, static_cast<sample_texture_fetch::filter>(Filter), 1);
		Error += Test();
	}

	CSV.save("../log-texture-fetch.csv");

	return Error;
}

