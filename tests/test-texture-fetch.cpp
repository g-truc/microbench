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

#		if defined(MODE_TEXTURE_FETCH) || defined(MODE_TEXTURE_SAMPLE)
			layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];
#		elif defined(MODE_IMAGE_LOAD)
			layout(binding = 0, FORMAT) coherent restrict readonly uniform image2D Texture[FETCH_COUNT];
#		endif

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		void main()
		{
			Color = vec4(0);

			for(int i = 0; i < FETCH_COUNT; ++i)
			{
#				if defined(MODE_TEXTURE_SAMPLE)
					Color += texture(Texture[i], vec2(0.0));
#				elif defined(MODE_TEXTURE_FETCH)
					Color += texelFetch(Texture[i], ivec2(0, 0), 0);
#				elif defined(MODE_IMAGE_LOAD)
					Color += imageLoad(Texture[i], ivec2(0, 0));
#				endif
			}
		}
	)";
}//namespace

class sample_texture_fetch : public framework
{
public:
	enum mode
	{
		MODE_TEXTURE_SAMPLE, MODE_FIRST = MODE_TEXTURE_SAMPLE,
		MODE_TEXTURE_FETCH,
		MODE_IMAGE_LOAD, MODE_LAST = MODE_IMAGE_LOAD
	};

	enum
	{
		MODE_COUNT = MODE_FIRST - MODE_LAST + 1
	};

	enum format
	{
		FORMAT_RGBA8_UNORM, FORMAT_FIRST = FORMAT_RGBA8_UNORM,
		FORMAT_RGBA32F, FORMAT_LAST = FORMAT_RGBA32F
	};

	char const* GetString(mode Mode) const
	{
		char const* Table[]
		{
			"texture",
			"texelFetch",
			"imageLoad",
		};
		return Table[Mode];
	}

	sample_texture_fetch(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, mode Mode, int FetchCount, format Format)
		: framework(argc, argv, "texture-fetch", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, Mode(Mode)
		, FetchCount(FetchCount)
		, Format(Format)
		, PipelineName(0)
		, ProgramName(0)
		, VertexArrayName(0)
		, TimerQueryIndex(0)
		, TimerQueryCount(Frames)
		, TimerQueryCompleted(false)
	{}

private:
	csv& CSV;
	const mode Mode;
	const int FetchCount;
	const format Format;
	std::vector<GLuint> TextureName;
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

		switch (this->Mode)
		{
		case MODE_TEXTURE_SAMPLE:
			FragShaderSource += "#define MODE_TEXTURE_SAMPLE 1 \n";
			break;
		case MODE_TEXTURE_FETCH:
			FragShaderSource += "#define MODE_TEXTURE_FETCH 1 \n";
			break;
		case MODE_IMAGE_LOAD:
			FragShaderSource += "#define MODE_IMAGE_LOAD 1 \n";
			break;
		}

		switch (this->Format)
		{
		case FORMAT_RGBA8_UNORM:
			FragShaderSource += "#define FORMAT rgba8 \n";
			break;
		case FORMAT_RGBA32F:
			FragShaderSource += "#define FORMAT rgba32f \n";
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

	bool initTexture()
	{
		glm::ivec2 WindowSize(this->getWindowSize());

		this->TextureName.resize(this->FetchCount);

		glGenTextures(this->FetchCount, &TextureName[0]);

		for (int i = 0; i < this->FetchCount; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, TextureName[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			switch(this->Format)
			{
				case FORMAT_RGBA8_UNORM:
				{
					glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA8, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
					glm::vec4 const Pixel = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) / static_cast<float>(this->FetchCount);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, &Pixel[0]);
					break;
				}
				case FORMAT_RGBA32F:
				{
					glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA32F, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
					glm::vec4 const Pixel = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) / static_cast<float>(this->FetchCount);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, &Pixel[0]);
					break;
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

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

		glm::vec2 WindowSize(this->getWindowSize());
		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);

		switch (this->Mode)
		{
		case MODE_IMAGE_LOAD:
			for (int i = 0; i < this->FetchCount; ++i)
				glBindImageTexture(i, this->TextureName[i], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
			break;
		case MODE_TEXTURE_FETCH:
		case MODE_TEXTURE_SAMPLE:
			for (int i = 0; i < this->FetchCount; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, this->TextureName[i]);
			}
			break;
		}

		return Validated;
	}

	bool end()
	{
		std::string Title = this->title() + ::format(" | Fetch(%d): %s", this->FetchCount, GetString(this->Mode));

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

			fprintf(stdout, "%s | Average: %.0f us, Min: %.0f us, Max: %.0f us\n", Title.c_str(),
				static_cast<double>(TimeAvg) / 1000.0,
				static_cast<double>(TimeMin) / 1000.0,
				static_cast<double>(TimeMax) / 1000.0);

			CSV.log(::format("%s ; %d ; %s ",
				this->title(), this->FetchCount, GetString(this->Mode)).c_str(),
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
	CSV.header("texture-fetch ; FetchCount ; Mode");

	std::size_t const Frames = 1000;
	glm::uvec2 const WindowSize(2400, 1200);

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_TEXTURE_SAMPLE, i, sample_texture_fetch::FORMAT_RGBA8_UNORM);
		Error += Test();
	}

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_TEXTURE_FETCH, i, sample_texture_fetch::FORMAT_RGBA8_UNORM);
		Error += Test();
	}

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_IMAGE_LOAD, i, sample_texture_fetch::FORMAT_RGBA8_UNORM);
		Error += Test();
	}

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_TEXTURE_SAMPLE, i, sample_texture_fetch::FORMAT_RGBA32F);
		Error += Test();
	}

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_TEXTURE_FETCH, i, sample_texture_fetch::FORMAT_RGBA32F);
		Error += Test();
	}

	for (int i = 1; i <= 8; ++i)
	{
		sample_texture_fetch Test(argc, argv, CSV, WindowSize, Frames, sample_texture_fetch::MODE_IMAGE_LOAD, i, sample_texture_fetch::FORMAT_RGBA32F);
		Error += Test();
	}

	CSV.save("../log-texture-fetch.csv");

	return Error;
}

