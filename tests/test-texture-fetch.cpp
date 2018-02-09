#include "test.hpp"
#include "csv.hpp"
#include <array>

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
#		elif defined(SAMPLER_1D_ARRAY)
			layout(binding = 0) uniform sampler1DArray Texture[FETCH_COUNT];
#		elif defined(SAMPLER_2D)
			layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];
#		elif defined(SAMPLER_2D_ARRAY)
			layout(binding = 0) uniform sampler2DArray Texture[FETCH_COUNT];
#		elif defined(SAMPLER_3D)
			layout(binding = 0) uniform sampler3D Texture[FETCH_COUNT];
#		elif defined(SAMPLER_CUBE)
			layout(binding = 0) uniform samplerCube Texture[FETCH_COUNT];
#		elif defined(SAMPLER_CUBE_ARRAY)
			layout(binding = 0) uniform samplerCubeArray Texture[FETCH_COUNT];
#		endif

		in vec4 gl_FragCoord;

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		void main()
		{
			Color = vec4(0);
			vec2 Coord = gl_FragCoord.xy * NORMALIZE_COORD;

			for(int i = 0; i < FETCH_COUNT; ++i)
			{
#				if defined(SAMPLER_1D)
					Color += texture(Texture[i], Coord.x) * (float(1.0) / float(FETCH_COUNT));
#				elif defined(SAMPLER_2D) || defined(SAMPLER_1D_ARRAY)
					Color += texture(Texture[i], Coord) * (float(1.0) / float(FETCH_COUNT));
#				elif defined(SAMPLER_2D_ARRAY) || defined(SAMPLER_3D) || defined(SAMPLER_CUBE)
					Color += texture(Texture[i], vec3(Coord, 0.0)) * (float(1.0) / float(FETCH_COUNT));
#				elif defined(SAMPLER_CUBE_ARRAY)
					Color += texture(Texture[i], vec4(Coord, 0.0, 0.0)) * (float(1.0) / float(FETCH_COUNT));
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
		FORMAT_RGBA8_UNORM, FORMAT_FIRST = FORMAT_RGBA8_UNORM,
		FORMAT_RGBA8_SRGB,
		FORMAT_RGB9E5UF,
		FORMAT_RG11B10UF,
		FORMAT_RGBA16F,
		FORMAT_RGBA32F, FORMAT_LAST = FORMAT_RGBA32F
	};

	enum
	{
		FORMAT_COUNT = FORMAT_LAST - FORMAT_FIRST + 1
	};

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
		SAMPLER_1D_ARRAY,
		SAMPLER_2D,
		SAMPLER_2D_ARRAY,
		SAMPLER_3D,
		SAMPLER_CUBE,
		SAMPLER_CUBE_ARRAY, SAMPLER_LAST = SAMPLER_CUBE_ARRAY
	};

	enum
	{
		SAMPLER_COUNT = SAMPLER_LAST - SAMPLER_FIRST + 1
	};

	sample_texture_fetch(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, int FetchCount, format Format, sampler Sampler, filter Filter, int AnisoSamples, size_t TextureSize)
		: framework(argc, argv, "texture-fetch", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, FetchCount(FetchCount)
		, Format(Format)
		, Sampler(Sampler)
		, Filter(Filter)
		, AnisoSamples(AnisoSamples)
		, TextureSize(static_cast<GLsizei>(TextureSize))
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
	int const FetchCount;
	format const Format;
	sampler const Sampler;
	filter const Filter;
	int const AnisoSamples;
	GLsizei const TextureSize;
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

	char const* GetString(sampler Sampler) const
	{
		static char const* Table[]
		{
			"1D",
			"1D Array",
			"2D",
			"2D Array",
			"3D",
			"Cube",
			"Cube Array",
		};
		static_assert((sizeof(Table) / sizeof(Table[0])) == SAMPLER_COUNT, "Invalid table size");

		return Table[Sampler];
	}

	std::string const GetSamplerDefine(sampler Sampler) const
	{
		static char const* Table[] = {
			"SAMPLER_1D",				// SAMPLER_1D
			"SAMPLER_1D_ARRAY",			// SAMPLER_1D_ARRAY
			"SAMPLER_2D",				// SAMPLER_2D
			"SAMPLER_2D_ARRAY",			// SAMPLER_2D_ARRAY
			"SAMPLER_3D",				// SAMPLER_3D
			"SAMPLER_CUBE",				// SAMPLER_CUBE
			"SAMPLER_CUBE_ARRAY"		// SAMPLER_CUBE_ARRAY
		};
		static_assert((sizeof(Table) / sizeof(Table[0])) == SAMPLER_COUNT, "Invalid table size");

		return std::string("#define ") + Table[Sampler] + " \n";
	}

	GLenum const GetTarget(sampler Sampler) const
	{
		static GLenum const Table[] = {
			GL_TEXTURE_1D,				// SAMPLER_1D
			GL_TEXTURE_1D_ARRAY,		// SAMPLER_1D_ARRAY
			GL_TEXTURE_2D,				// SAMPLER_2D
			GL_TEXTURE_2D_ARRAY,		// SAMPLER_2D_ARRAY
			GL_TEXTURE_3D,				// SAMPLER_3D
			GL_TEXTURE_CUBE_MAP,		// SAMPLER_CUBE
			GL_TEXTURE_CUBE_MAP_ARRAY,	// SAMPLER_CUBE_ARRAY
		};
		static_assert((sizeof(Table) / sizeof(Table[0])) == SAMPLER_COUNT, "Invalid table size");

		return Table[Sampler];
	}

	GLenum const GetInternalFormat(format Format) const
	{
		static GLenum const Table[] = {
			GL_RGBA8,				// FORMAT_RGBA8_UNORM
			GL_SRGB8_ALPHA8,		// FORMAT_RGBA8_SRGB
			GL_RGB9_E5,				// FORMAT_RGB9E5UF
			GL_R11F_G11F_B10F,		// FORMAT_RG11B10UF
			GL_RGBA16F,				// FORMAT_RGBA16F
			GL_RGBA32F				// FORMAT_RGBA32F
		};
		static_assert((sizeof(Table) / sizeof(Table[0])) == FORMAT_COUNT, "Invalid table size");

		return Table[Format];
	}

	char const* GetString(format Format) const
	{
		static char const* Table[]
		{
			"rgba8unorm",			// FORMAT_RGBA8_UNORM
			"rgba8srgb",			// FORMAT_RGBA8_SRGB
			"rgb9e5uf",				// FORMAT_RGB9E5UF
			"rg11b10uf",			// FORMAT_RG11B10UF
			"rgba16f",				// FORMAT_RGBA16F
			"rgba32f"				// FORMAT_RGBA32F
		};
		static_assert((sizeof(Table) / sizeof(Table[0])) == FORMAT_COUNT, "Invalid table size");

		return Table[Format];
	}

	bool initProgram()
	{
		GLuint VertShaderName = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertShaderName, 1, &VERT_SHADER_SOURCE, NULL);
		glCompileShader(VertShaderName);

		std::string FragShaderSource;
		FragShaderSource += "#version 430 core \n";

		FragShaderSource += ::format("#define FETCH_COUNT %d \n", this->FetchCount);

		FragShaderSource += ::format("#define NORMALIZE_COORD vec2(%f, %f) \n", 1.0f / static_cast<float>(this->getWindowSize().x), 1.0f / static_cast<float>(this->getWindowSize().y));

		FragShaderSource += GetSamplerDefine(this->Sampler);

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

	bool initTexture()
	{
		glm::ivec2 const WindowSize(this->getWindowSize());
		GLsizei const TextureLevels = gli::levels(TextureSize);

		this->TextureName.resize(this->FetchCount);

		glGenTextures(this->FetchCount, &TextureName[0]);

		GLenum const Target = GetTarget(this->Sampler);

		for (int i = 0; i < this->FetchCount; ++i)
		{
			glBindTexture(Target, TextureName[i]);
			glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, TextureLevels - 1);
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

			glm::vec4 Pixel = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
			if (this->Format == FORMAT_RGBA8_SRGB)
				Pixel = glm::convertLinearToSRGB(Pixel);
			std::vector<glm::vec4> Pixels;

			GLenum const InternalFormat = GetInternalFormat(this->Format);

			switch(this->Sampler)
			{
			case SAMPLER_1D:
				glTexStorage1D(Target, TextureLevels, InternalFormat, TextureSize);
				break;
			case SAMPLER_1D_ARRAY:
			case SAMPLER_2D:
				glTexStorage2D(Target, TextureLevels, InternalFormat, TextureSize, TextureSize);
				break;
			case SAMPLER_2D_ARRAY:
			case SAMPLER_3D:
				glTexStorage3D(Target, TextureLevels, InternalFormat, TextureSize, TextureSize, TextureSize);
				break;
			case SAMPLER_CUBE:
				glTexStorage2D(Target, TextureLevels, InternalFormat, TextureSize, TextureSize);
				break;
			case SAMPLER_CUBE_ARRAY:
				break;
			default:
				assert(0);
				break;
			}

			for (GLsizei Level = 0; Level < TextureLevels; ++Level)
				glClearTexImage(TextureName[i], Level, GL_RGBA, GL_FLOAT, &Pixel[0]);
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

			CSV.log(::format("%s ; %d ; %s ; %s ; %s ; %d ",
				this->title(), this->FetchCount, GetString(this->Sampler), GetString(this->Filter), GetString(this->Format), TextureSize).c_str(),
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
	CSV.header("texture-fetch ; FetchCount ; Filter ; Format ; TextureSize");

	std::size_t const Frames = 1000;
	glm::uvec2 const WindowSize(1024, 1024);

	for (int FetchCount = 4; FetchCount <= 32; FetchCount <<= 1)
	for (int TextureSize = 1; TextureSize <= 4096; TextureSize <<= 1)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames,
			FetchCount, sample_texture_fetch::FORMAT_RGBA8_UNORM, sample_texture_fetch::SAMPLER_2D, sample_texture_fetch::FILTER_TRILINEAR, 1, TextureSize);
		Error += Test();
	}

/*
	sample_texture_fetch::sampler Samplers[] =
	{
		sample_texture_fetch::SAMPLER_2D,
		sample_texture_fetch::SAMPLER_3D
	};

	for (int FilterIndex = 0; FilterIndex < sample_texture_fetch::FILTER_COUNT; ++FilterIndex)
	for (int SamplerIndex = 0; SamplerIndex < 2; ++SamplerIndex)
	for (int FetchCount = 4; FetchCount <= 32; FetchCount <<= 1)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames,
			FetchCount, sample_texture_fetch::FORMAT_RGBA32F, Samplers[SamplerIndex], static_cast<sample_texture_fetch::filter>(FilterIndex), 1, 1024);
		Error += Test();
	}
*/
/*
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, 0,
			16, sample_texture_fetch::FORMAT_RGBA8_SRGB, sample_texture_fetch::SAMPLER_CUBE, sample_texture_fetch::FILTER_TRILINEAR, 16);
		Error += Test();
	}

	for (int SamplerIndex = 0; SamplerIndex < sample_texture_fetch::SAMPLER_COUNT; ++SamplerIndex)
	for (int FilterIndex = 0; FilterIndex < sample_texture_fetch::FILTER_COUNT; ++FilterIndex)
	for (int FormatIndex = 0; FormatIndex < sample_texture_fetch::FORMAT_COUNT; ++FormatIndex)
	for (int FetchCount = 1; FetchCount <= 32; FetchCount <<= 1)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames,
			FetchCount, static_cast<sample_texture_fetch::format>(FormatIndex), static_cast<sample_texture_fetch::sampler>(SamplerIndex), static_cast<sample_texture_fetch::filter>(FilterIndex), 1);
		Error += Test();
	}
*/
	CSV.save("../log-texture-fetch.csv");

	return Error;
}

