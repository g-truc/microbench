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
/*
		#pragma optionNV(fastmath off)
		#pragma optionNV(fastprecision off)
		#pragma optionNV(unroll none)
*/
		layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];

		in vec4 gl_FragCoord;

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		vec2 alus(vec2 Coord)
		{
			vec4 Tmp = vec4(Coord, Coord);
			for(int i = 0; i < 100; ++i)
			{
				Tmp *= vec4(1.000001, 1.000002, 1.000003, 1.000004);
				Tmp -= vec4(0.000001, 0.000002, 0.000003, 0.000004);
				Tmp /= vec4(1.000001, 1.000002, 1.000003, 1.000004);
				Tmp += vec4(0.000001, 0.000002, 0.000003, 0.000004);
			}
			return (Tmp.xy + Tmp.zw) * 0.5;
		}

		void main()
		{
			vec2 Coord = gl_FragCoord.xy * NORMALIZE_COORD;

#			ifdef DEPENDENT_FETCH
				for(int i = 0; i < FETCH_COUNT; ++i)
				{
					Coord = texture(Texture[i], Coord).xy + 0.000001;
					Coord = alus(Coord);
				}
				Color = vec4(Coord * 1.000001, 0.0, 1.0);
#			else
				vec2 Temp = vec2(0);
				for(int i = 0; i < FETCH_COUNT; ++i)
				{
					Temp = texture(Texture[i], Coord).xy + Temp;
					Temp = alus(Temp);
				}
				Color = vec4(Temp * (1.0 / float(FETCH_COUNT)), 0.0, 1.0);
#			endif
		}
	)";
}//namespace

class sample_dependent_fetch : public framework
{
public:
	enum format
	{
		FORMAT_RGBA8_UNORM, FORMAT_FIRST = FORMAT_RGBA8_UNORM,
		FORMAT_RGB9E5UF,
		FORMAT_RG11B10UF,
		FORMAT_RGBA16F,
		FORMAT_RGBA32F, FORMAT_LAST = FORMAT_RGBA32F
	};

	enum
	{
		FORMAT_COUNT = FORMAT_LAST - FORMAT_FIRST + 1
	};

	sample_dependent_fetch(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, int FetchCount, format Format, bool DependentFetch, std::size_t TextureSize)
		: framework(argc, argv, "dependent-fetch", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, FetchCount(FetchCount)
		, Format(Format)
		, DependentFetch(DependentFetch)
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
	bool const DependentFetch;
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

	GLenum const GetInternalFormat(format Format) const
	{
		static GLenum const Table[] = {
			GL_RGBA8,				// FORMAT_RGBA8_UNORM
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

		if (this->DependentFetch)
			FragShaderSource += "#define DEPENDENT_FETCH 1 \n";

		FragShaderSource += ::format("#define FETCH_COUNT %d \n", this->FetchCount);

		FragShaderSource += ::format("#define NORMALIZE_COORD vec2(%f, %f) \n", 1.0f / static_cast<float>(this->getWindowSize().x), 1.0f / static_cast<float>(this->getWindowSize().y));

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
		this->TextureName.resize(this->FetchCount);
		glGenTextures(this->FetchCount, &TextureName[0]);

		std::vector<glm::vec2> TextureData;
		TextureData.resize(this->TextureSize * this->TextureSize);

		for (GLsizei y = 0; y < TextureSize; ++y)
		for (GLsizei x = 0; x < TextureSize; ++x)
			TextureData[x + y * TextureSize] = (glm::vec2(x, y) / float(TextureSize - 1));

		for (int i = 0; i < this->FetchCount; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, TextureName[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexStorage2D(GL_TEXTURE_2D, 1, GetInternalFormat(this->Format), this->TextureSize, this->TextureSize);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->TextureSize, this->TextureSize, GL_RG, GL_FLOAT, &TextureData[0][0]);
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
				glBindTexture(GL_TEXTURE_2D, this->TextureName[i]);
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

			CSV.log(::format("%s ; %d ; %s ; %s ",
				this->title(), this->FetchCount, this->DependentFetch ? "true" : "false", GetString(this->Format)).c_str(),
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
	CSV.header("dependent-fetch ; FetchCount ; Filter ; Format ; TextureSize");

	std::size_t const Frames = 0;
	glm::uvec2 const WindowSize(1024, 1024);

	for (int FetchCount = 8; FetchCount <= 8; FetchCount <<= 1)
	{
		sample_dependent_fetch Test(argc, argv, CSV, WindowSize, Frames,
			FetchCount, sample_dependent_fetch::FORMAT_RGBA8_UNORM, true, 2048);
		Error += Test();
	}

	CSV.save("../log-dependent-fetch.csv");

	return Error;
}

