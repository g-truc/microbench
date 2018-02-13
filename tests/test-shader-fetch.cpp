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
#		ifdef MODE_BINDLESS_IMAGE_LOAD
			#extension GL_ARB_bindless_texture : require
#		endif

		#define FRAG_COLOR	0

#		ifndef FETCH_COUNT
#			error FETCH_COUNT must be defined to a value between 1 and 8 included
#		endif

#		if defined(MODE_TEXTURE_FETCH) || defined(MODE_TEXTURE_SAMPLE)
			layout(binding = 0) uniform sampler2D Texture[FETCH_COUNT];
#		elif defined(MODE_IMAGE_LOAD)
			layout(binding = 0, FORMAT) restrict readonly uniform image2D Texture[FETCH_COUNT];
#		elif defined(MODE_BINDLESS_IMAGE_LOAD)
			layout(bindless_image, FORMAT) restrict readonly uniform image2D Texture[FETCH_COUNT];
#		elif defined(MODE_UNIFORM_BUFFER) || defined(MODE_SHADER_BUFFER)
#			ifdef MODE_UNIFORM_BUFFER
#				define QUALIFIER uniform
#			elif MODE_SHADER_BUFFER
#				define QUALIFIER buffer
#			endif

			layout(binding = 0) QUALIFIER bufferFetch
			{
#				if defined(FORMAT_RGBA32F)
					vec4 Color;
#				else
					uint Color;
#				endif
			} Buffer[FETCH_COUNT];
#		endif

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		vec3 correctSrgbToRgb(in vec3 ColorSRGB, in float Gamma)
		{
			return mix(
				pow((ColorSRGB + 0.055) * 0.94786729857819905213270142180095, vec3(Gamma)),
				ColorSRGB * 0.07739938080495356037151702786378,
				lessThanEqual(ColorSRGB, vec3(0.04045)));
		}

		vec3 correctSrgbToRgb(in vec3 ColorSRGB)
		{
			return correctSrgbToRgb(ColorSRGB, 2.4);
		}

		vec4 correctSrgbToRgb(in vec4 ColorSRGB, in float Gamma)
		{
			return vec4(correctSrgbToRgb(ColorSRGB.rgb, Gamma), ColorSRGB.a);
		}

		vec4 correctSrgbToRgb(in vec4 ColorSRGB)
		{
			return vec4(correctSrgbToRgb(ColorSRGB.rgb, 2.4), ColorSRGB.a);
		}

		vec4 gammaSrgbToRgb(in vec4 ColorSRGB)
		{
			return vec4(pow(ColorSRGB.rgb, vec3(2.2)), ColorSRGB.a);
		}

		vec3 taylorSrgbToRgb(in vec3 ColorSRGB)
		{
			return ColorSRGB * (ColorSRGB * (ColorSRGB * 0.305306011 + 0.682171111) + 0.012522878);
		}

		vec4 taylorSrgbToRgb(in vec4 ColorSRGB)
		{
			return vec4(taylorSrgbToRgb(ColorSRGB.rgb), ColorSRGB.a);
		}

		void main()
		{
			Color = vec4(0);

			for(int i = 0; i < FETCH_COUNT; ++i)
			{
				vec4 Texel;

#				if defined(MODE_TEXTURE_SAMPLE)
					Texel = texture(Texture[i], vec2(0.0));
#				elif defined(MODE_TEXTURE_FETCH)
					Texel = texelFetch(Texture[i], ivec2(0, 0), 0);
#				elif defined(MODE_IMAGE_LOAD) || defined(MODE_BINDLESS_IMAGE_LOAD)
					Texel = imageLoad(Texture[i], ivec2(0, 0));
#				elif defined(MODE_UNIFORM_BUFFER) || defined(MODE_SHADER_BUFFER)
#					if defined(FORMAT_RGBA32F)
						Texel = Buffer[i].Color;
#					else
						Texel = unpackUnorm4x8(Buffer[i].Color);
#					endif
#				endif

#				if defined(SRGB_CORRECT)
					Texel = correctSrgbToRgb(Texel);
#				elif defined(SRGB_GAMMA)
					Texel = gammaSrgbToRgb(Texel);
#				elif defined(SRGB_TAYLOR)
					Texel = taylorSrgbToRgb(Texel);
#				endif

				Color += Texel;
			}
		}
	)";
}//namespace

class sample_shader_fetch : public framework
{
public:
	enum mode
	{
		MODE_TEXTURE_SAMPLE, MODE_FIRST = MODE_TEXTURE_SAMPLE,
		MODE_TEXTURE_FETCH,
		MODE_IMAGE_LOAD,
		MODE_BINDLESS_IMAGE_LOAD,
		MODE_BUFFER_UNIFORM,
		MODE_BUFFER_SHADER, MODE_LAST = MODE_BUFFER_SHADER
	};

	enum
	{
		MODE_COUNT = MODE_LAST - MODE_FIRST + 1
	};

	char const* GetString(mode Mode) const
	{
		static char const* Table[]
		{
			"texture",
			"texelFetch",
			"imageLoad",
			"bindless imageLoad",
			"uniform buffer",
			"shader buffer"
		};
		return Table[Mode];
	}

	enum format
	{
		FORMAT_RGBA8_UNORM, FORMAT_FIRST = FORMAT_RGBA8_UNORM,
		FORMAT_RGBA8_SRGB,
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
			"rgba8unorm",
			"rgba8srgb",
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

	enum color
	{
		COLOR_ORANGE, COLOR_FIRST = COLOR_ORANGE,
		COLOR_WHITE,
		COLOR_BLACK, COLOR_LAST = COLOR_BLACK
	};

	enum
	{
		COLOR_COUNT = COLOR_LAST - COLOR_FIRST + 1
	};

	char const* GetString(color Color) const
	{
		static char const* Table[]
		{
			"orange",
			"white",
			"black"
		};
		return Table[Color];
	}

	enum srgb
	{
		SRGB_LINEAR, SRGB_FIRST = SRGB_LINEAR,
		SRGB_CORRECT,
		SRGB_GAMMA,
		SRGB_TAYLOR, SRGB_LAST = SRGB_TAYLOR
	};

	enum
	{
		SRGB_COUNT = SRGB_LAST - SRGB_FIRST + 1
	};

	char const* GetString(srgb SRGB) const
	{
		static char const* Table[]
		{
			"srgb linear",
			"srgb correct",
			"srgb gamma",
			"srgb taylor"
		};
		return Table[SRGB];
	}

	sample_shader_fetch(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, mode Mode, int FetchCount, format Format, srgb SRGB, color Color, filter Filter)
		: framework(argc, argv, "shader-fetch", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, Mode(Mode)
		, FetchCount(FetchCount)
		, Format(Format)
		, SRGB(SRGB)
		, Color(Color)
		, Filter(Filter)
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
	const mode Mode;
	const int FetchCount;
	const format Format;
	const srgb SRGB;
	const color Color;
	const filter Filter;
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
		case MODE_BINDLESS_IMAGE_LOAD:
			FragShaderSource += "#define MODE_BINDLESS_IMAGE_LOAD 1 \n";
			break;
		case MODE_BUFFER_UNIFORM:
			FragShaderSource += "#define MODE_UNIFORM_BUFFER 1 \n";
		case MODE_BUFFER_SHADER:
			FragShaderSource += "#define MODE_SHADER_BUFFER 1 \n";
			break;
		}

		if (this->Mode == MODE_IMAGE_LOAD || this->Mode == MODE_BINDLESS_IMAGE_LOAD || this->Mode == MODE_BUFFER_UNIFORM || this->Mode == MODE_BUFFER_SHADER)
		{
			switch(this->SRGB)
			{
			case SRGB_CORRECT:
				FragShaderSource += "#define SRGB_CORRECT 1 \n";
				break;
			case SRGB_GAMMA:
				FragShaderSource += "#define SRGB_GAMMA 1 \n";
				break;
			case SRGB_TAYLOR:
				FragShaderSource += "#define SRGB_TAYLOR 1 \n";
				break;
			}
		}

		switch (this->Format)
		{
		case FORMAT_RGBA8_UNORM:
		case FORMAT_RGBA8_SRGB:
			FragShaderSource += "#define FORMAT rgba8 \n";
			break;
		case FORMAT_RGBA32F:
			FragShaderSource += "#define FORMAT_RGBA32F \n";
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

		if (this->Mode == MODE_BINDLESS_IMAGE_LOAD)
			this->TextureLocation = glGetUniformLocation(this->ProgramName, "Texture[0]");

		glDeleteShader(VertShaderName);
		glDeleteShader(FragShaderName);

		glGenProgramPipelines(1, &PipelineName);
		glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);

		return true;
	}

	glm::vec4 GetColor()
	{
		switch (this->Color)
		{
		default:
		case COLOR_ORANGE:
			return glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) / static_cast<float>(this->FetchCount);
		case COLOR_WHITE:
			return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) / static_cast<float>(this->FetchCount);
		case COLOR_BLACK:
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) / static_cast<float>(this->FetchCount);
		}
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

			switch(Filter)
			{
			case FILTER_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case FILTER_BILINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case FILTER_TRILINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}

			glm::vec4 Pixel32f = this->GetColor();

			if (this->SRGB != SRGB_LINEAR)
				Pixel32f = glm::convertLinearToSRGB(Pixel32f);

			glm::uint Pixel8UNorm = glm::packUnorm4x8(Pixel32f);

			switch(this->Format)
			{
			case FORMAT_RGBA8_UNORM:
				glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA8, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Pixel8UNorm);
				break;
			case FORMAT_RGBA8_SRGB:
				glTexStorage2D(GL_TEXTURE_2D, GLint(1), this->Mode == MODE_IMAGE_LOAD || this->Mode == MODE_BINDLESS_IMAGE_LOAD ? GL_RGBA8 : GL_SRGB8_ALPHA8, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &Pixel8UNorm);
				break;
			case FORMAT_RGBA32F:
				glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA32F, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, &Pixel32f[0]);
				break;
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	bool initBuffer()
	{
		glm::vec4 Pixel32f = GetColor();

		if (this->SRGB != SRGB_LINEAR)
			Pixel32f = glm::convertLinearToSRGB(Pixel32f);

		glm::uint Pixel8UNorm = glm::packUnorm4x8(Pixel32f);

		this->BufferName.resize(this->FetchCount);
		glGenBuffers(this->FetchCount, &this->BufferName[0]);
		for (int i = 0; i < this->FetchCount; ++i)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, this->BufferName[i]);
			if (this->Format == FORMAT_RGBA32F)
				glBufferStorage(GL_UNIFORM_BUFFER, sizeof(Pixel32f), &Pixel32f[0], 0);
			else
				glBufferStorage(GL_UNIFORM_BUFFER, sizeof(Pixel8UNorm), &Pixel8UNorm, 0);
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

		if (Validated && this->Mode == MODE_BINDLESS_IMAGE_LOAD)
			Validated = Validated && this->checkExtension("GL_ARB_bindless_texture");

		if (Validated)
			Validated = initBuffer();
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

			GLenum InternalFormat = GL_NONE;

			switch (this->Format)
			{
			case FORMAT_RGBA8_UNORM:
			case FORMAT_RGBA8_SRGB:
				InternalFormat = GL_RGBA8;
				break;
			case FORMAT_RGBA32F:
				InternalFormat = GL_RGBA32F;
				break;
			}

			switch (this->Mode)
			{
				case MODE_BINDLESS_IMAGE_LOAD:
				{
					for (int i = 0; i < this->FetchCount; ++i)
					{
						GLuint64 Handle = glGetImageHandleARB(this->TextureName[i], 0, GL_FALSE, 0, InternalFormat);
						glMakeImageHandleResidentARB(Handle, GL_READ_ONLY);
						glProgramUniformHandleui64ARB(this->ProgramName, this->TextureLocation + i, Handle);
					}
				}
				break;
				case MODE_IMAGE_LOAD:
				{
					for (int i = 0; i < this->FetchCount; ++i)
						glBindImageTexture(i, this->TextureName[i], 0, GL_FALSE, 0, GL_READ_ONLY, InternalFormat);
				}
				break;
				case MODE_TEXTURE_FETCH:
				case MODE_TEXTURE_SAMPLE:
				{
					for (int i = 0; i < this->FetchCount; ++i)
					{
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, this->TextureName[i]);
					}
				}
				break;
				case MODE_BUFFER_UNIFORM:
				{
					for (int i = 0; i < this->FetchCount; ++i)
					{
						glBindBufferBase(GL_UNIFORM_BUFFER, i, this->BufferName[i]);
					}
				}
				break;
				case MODE_BUFFER_SHADER:
				{
					for (int i = 0; i < this->FetchCount; ++i)
					{
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, this->BufferName[i]);
					}
				}
				break;
			}
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

			fprintf(stdout, "%s | %d %s %s | Average: %.0f us, Min: %.0f us, Max: %.0f us\n", Title.c_str(),
				this->FetchCount, GetString(this->Mode), GetString(this->Format),
				static_cast<double>(TimeAvg) / 1000.0,
				static_cast<double>(TimeMin) / 1000.0,
				static_cast<double>(TimeMax) / 1000.0);

			CSV.log(::format("%s ; %d ; %s ; %s ; %s",
				this->title(), this->FetchCount, GetString(this->Mode), GetString(this->Format), GetString(this->SRGB)).c_str(),
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

	// warmup
	{
		csv CSV;
		sample_shader_fetch Test(argc, argv, CSV, glm::uvec2(1920, 1080), 100,
			sample_shader_fetch::MODE_BUFFER_UNIFORM, 8, sample_shader_fetch::FORMAT_RGBA32F, sample_shader_fetch::SRGB_CORRECT, sample_shader_fetch::COLOR_ORANGE, sample_shader_fetch::FILTER_NEAREST);
		Error += Test();
	}

	csv CSV;
	CSV.header("texture-fetch ; FetchCount ; Mode ; Format ; SRGB");

	std::size_t const Frames = 200;
	glm::uvec2 const WindowSize(2560, 1440);

	for (int SRGB = 0; SRGB < sample_shader_fetch::SRGB_COUNT; ++SRGB)
	for (int FetchCount = 1; FetchCount <= 8; ++FetchCount)
	{
		sample_shader_fetch Test(argc, argv, CSV, WindowSize, Frames,
			sample_shader_fetch::MODE_BUFFER_UNIFORM, FetchCount, sample_shader_fetch::FORMAT_RGBA32F, static_cast<sample_shader_fetch::srgb>(SRGB), sample_shader_fetch::COLOR_ORANGE, sample_shader_fetch::FILTER_NEAREST);
		Error += Test();
	}

/*
	for (int Mode = 0; Mode < sample_shader_fetch::MODE_COUNT; ++Mode)
	for (int Format = 0; Format < sample_shader_fetch::FORMAT_COUNT; ++Format)
	for (int SRGB = 0; SRGB < sample_shader_fetch::SRGB_COUNT; ++SRGB)
	for (int FetchCount = 1; FetchCount <= 8; ++FetchCount)
	{
		sample_shader_fetch Test(argc, argv, CSV, WindowSize, Frames,
			static_cast<sample_shader_fetch::mode>(Mode), FetchCount, static_cast<sample_shader_fetch::format>(Format), static_cast<sample_shader_fetch::srgb>(SRGB), sample_shader_fetch::COLOR_ORANGE, sample_shader_fetch::FILTER_NEAREST);
		Error += Test();
	}
*/
	CSV.save("../log-shader-fetch.csv");

	return Error;
}

