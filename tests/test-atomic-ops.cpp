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

#		if defined(ATOMIC_COUNTER_ADD_ARB)
			#extension GL_ARB_shader_atomic_counter_ops : require
#		elif defined(ATOMIC_COUNTER_ADD_AMD)
			#extension GL_AMD_shader_atomic_counter_ops : require
#		endif

#		if defined(ATOMIC_COUNTER_INC) || defined(ATOMIC_COUNTER_ADD_ARB) || defined(ATOMIC_COUNTER_ADD_AMD)
			layout(binding = 0, offset = 0) uniform atomic_uint Atomic;

#		elif defined(ATOMIC_BUFFER)
			layout(binding = 0) buffer fragIndex
			{
				uint Data;
			} FragIndex;

#		elif defined(ATOMIC_IMAGE)
			layout(binding = 0, r32ui) coherent restrict uniform uimage2D Atomic;

#		else
#			error Use #define ATOMIC_COUNTER_INC, #define ATOMIC_COUNTER_ADD_ARB, #define ATOMIC_COUNTER_ADD_AMD, #define ATOMIC_BUFFER, or #define ATOMIC_IMAGE

#		endif

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		uniform int AtomicCount;

#		ifdef ATOMIC_PREDICAT
			uniform ivec2 AtomicPredicat;
#		endif

		vec4 buildColorFromIndex(uint Index, float Scale, float Alpha)
		{
			uint Mask = (1 << 8) - 1;
			return vec4(
				(((Index & (Mask <<  0)) % 255) / 255.f),
				(((Index & (Mask <<  8)) % 255) / 255.f),
				(((Index & (Mask << 16)) % 255) / 255.f) / (Scale / 8.f),
				Alpha);
		}

		in vec4 gl_FragCoord;

		void main()
		{
			uint Counter = 0;
			uint Data = 1;

#			ifdef ATOMIC_PREDICAT
				bool PerformAtomic = ((int(gl_FragCoord.x) % AtomicPredicat.x) == 0 && (int(gl_FragCoord.y) % AtomicPredicat.y) == 0);
#			else
				bool PerformAtomic = true;
#			endif

			for (int i = 0; i < AtomicCount && PerformAtomic; ++i)
			{
#				if defined(ATOMIC_COUNTER_INC)
					Counter = atomicCounterIncrement(Atomic);

#				elif defined(ATOMIC_COUNTER_ADD_ARB)
					Counter = atomicCounterAddARB(Atomic, Data);

#				elif defined(ATOMIC_COUNTER_ADD_AMD)
					Counter = atomicCounterAdd(Atomic, Data);

#				elif defined(ATOMIC_BUFFER)
					Counter = atomicAdd(FragIndex.Data, Data);

#				elif defined(ATOMIC_IMAGE)
					Counter = imageAtomicAdd(Atomic, ivec2(0, 0), Data);

#				endif

#				if defined(MEM_BARRIER_ANY)
					memoryBarrier();
#				endif
			}

#			if defined(MEM_BARRIER_ONCE)
				memoryBarrier();
#			endif

			Color = buildColorFromIndex(Counter, float(AtomicCount), 1.0f);
		}
	)";

	namespace buffer
	{
		enum type
		{
			ATOMIC_COUNTER,
			CLEAR,
			MAX
		};
	}//namespace buffer

	namespace texture
	{
		enum type
		{
			ATOMIC_COUNTER,
			CLEAR,
			MAX
		};
	}//namespace buffer
}//namespace

class sample_atomic_counter : public framework
{
public:
	enum atomic_type
	{
		ATOMIC_COUNTER_INC, ATOMIC_FIRST = ATOMIC_COUNTER_INC,
		ATOMIC_COUNTER_ADD,
		ATOMIC_BUFFER_ADD,
		ATOMIC_IMAGE_ADD, ATOMIC_LAST = ATOMIC_IMAGE_ADD
	};

	enum
	{
		ATOMIC_TYPE_COUNT = ATOMIC_LAST - ATOMIC_FIRST + 1
	};

	char const* GetString(atomic_type Type) const
	{
		char const* Table[]
		{
			"COUNTER_INC",
			"COUNTER_ADD",
			"BUFFER_ADD",
			"IMAGE_ADD"
		};

		return Table[Type];
	}

	enum barrier
	{
		BARRIER_NONE, BARRIER_FIRST = BARRIER_NONE,
		BARRIER_ONCE,
		BARRIER_ANY, BARRIER_LAST = BARRIER_ANY,
	};

	enum
	{
		BARRIER_COUNT = BARRIER_LAST - BARRIER_FIRST + 1
	};

	char const* GetString(barrier Barrier) const
	{
		char const* Table[]
		{
			"NONE",
			"ONCE",
			"ANY"
		};

		return Table[Barrier];
	}

	sample_atomic_counter(int argc, char* argv[], csv& CSV, glm::uvec2 WindowSize, std::size_t Frames, atomic_type AtomicType, int AtomicOpsCount, glm::ivec2 const& AtomicOpsPredicat, barrier Barrier)
		: framework(argc, argv, "atomic-ops", framework::CORE, 4, 3, WindowSize, glm::vec2(0), glm::vec2(0), Frames, framework::RUN_ONLY)
		, CSV(CSV)
		, AtomicType(AtomicType)
		, AtomicOpsPredicat(AtomicOpsPredicat)
		, AtomicOpsCount(AtomicOpsCount)
		, Barrier(Barrier)
		, PipelineName(0)
		, ProgramName(0)
		, VertexArrayName(0)
		, TimerQueryIndex(0)
		, TimerQueryCount(Frames)
		, TimerQueryCompleted(false)
	{}

private:
	csv& CSV;
	const atomic_type AtomicType;
	const int AtomicOpsCount;
	const glm::ivec2 AtomicOpsPredicat;
	const barrier Barrier;
	std::array<GLuint, buffer::MAX> BufferName;
	std::array<GLuint, texture::MAX> TextureName;
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

		if (this->AtomicOpsPredicat.x > 1 || this->AtomicOpsPredicat.y > 1)
			FragShaderSource += "#define ATOMIC_PREDICAT 1 \n";

		switch (AtomicType)
		{
		case ATOMIC_COUNTER_INC:
			FragShaderSource += "#define ATOMIC_COUNTER_INC 1 \n";
			break;
		case ATOMIC_COUNTER_ADD:
			if (this->checkExtension("GL_ARB_shader_atomic_counter_ops"))
				FragShaderSource += "#define ATOMIC_COUNTER_ADD_ARB 1 \n";
			else if (this->checkExtension("GL_AMD_shader_atomic_counter_ops"))
				FragShaderSource += "#define ATOMIC_COUNTER_ADD_AMD 1 \n";
			break;
		case ATOMIC_BUFFER_ADD:
			FragShaderSource += "#define ATOMIC_BUFFER 1 \n";
			break;
		case ATOMIC_IMAGE_ADD:
			FragShaderSource += "#define ATOMIC_IMAGE 1 \n";
			break;
		}

		switch (Barrier)
		{
		case BARRIER_NONE:
			FragShaderSource += "#define MEM_BARRIER_NONE 1 \n";
			break;
		case BARRIER_ONCE:
			FragShaderSource += "#define MEM_BARRIER_ONCE 1 \n";
			break;
		case BARRIER_ANY:
			FragShaderSource += "#define MEM_BARRIER_ANY 1 \n";
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

		GLuint UniformAtomicCount = glGetUniformLocation(ProgramName, "AtomicCount");
		glProgramUniform1i(ProgramName, UniformAtomicCount, AtomicOpsCount);

		if (this->AtomicOpsPredicat.x > 1 || this->AtomicOpsPredicat.y > 1)
		{
			GLuint UniformAtomicPredicat = glGetUniformLocation(ProgramName, "AtomicPredicat");
			glProgramUniform2i(ProgramName, UniformAtomicPredicat, AtomicOpsPredicat.x, AtomicOpsPredicat.y);
		}

		return true;
	}

	bool initBuffer()
	{
		glGenBuffers(buffer::MAX, &BufferName[0]);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);
		glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_MAP_WRITE_BIT);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

		glBindBuffer(GL_COPY_READ_BUFFER, BufferName[buffer::CLEAR]);
		GLuint Data = 0;
		glBufferStorage(GL_COPY_READ_BUFFER, sizeof(GLuint), &Data, GL_MAP_WRITE_BIT);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);

		return true;
	}

	bool initTexture()
	{
		glm::ivec2 WindowSize(this->getWindowSize());

		glGenTextures(texture::MAX, &TextureName[0]);

		glBindTexture(GL_TEXTURE_2D, TextureName[texture::ATOMIC_COUNTER]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_R32UI, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, TextureName[texture::CLEAR]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_R32UI, GLsizei(WindowSize.x), GLsizei(WindowSize.y));
		GLuint Pixels = 0;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &Pixels);

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

		if(Validated)
			Validated = initBuffer();
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

		if (this->AtomicType != ATOMIC_IMAGE_ADD)
		{
			glBindBuffer(GL_COPY_READ_BUFFER, BufferName[buffer::CLEAR]);
			glBindBuffer(GL_COPY_WRITE_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);
		}

		switch (this->AtomicType)
		{
		case ATOMIC_BUFFER_ADD:
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->BufferName[buffer::ATOMIC_COUNTER]);
			break;

		case ATOMIC_COUNTER_ADD:
		case ATOMIC_COUNTER_INC:
			glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, this->BufferName[buffer::ATOMIC_COUNTER]);
			break;

		case ATOMIC_IMAGE_ADD:
			glBindImageTexture(semantic::image::DIFFUSE, this->TextureName[texture::ATOMIC_COUNTER], 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
			break;

		default:
			return false;
		}

		return Validated;
	}

	bool end()
	{
		std::string Title = this->title() + format(" | Atomic(%d): %s - barrier: %s - predicate: %d, %d", this->AtomicOpsCount, GetString(this->AtomicType), GetString(this->Barrier), this->AtomicOpsPredicat.x, this->AtomicOpsPredicat.y);

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

			CSV.log(format("atomic-counter ; %d ; %s ; %s ; %d ; %d ", 
				this->AtomicOpsCount, GetString(this->AtomicType),
				GetString(this->Barrier),
				this->AtomicOpsPredicat.x, this->AtomicOpsPredicat.y).c_str(),
				static_cast<double>(TimeAvg) / 1000.0, static_cast<double>(TimeMin) / 1000.0, static_cast<double>(TimeMax) / 1000.0);
		}
		else
			fprintf(stdout, "%s | Not enough frames to report rendering time\n", Title.c_str());

		glDeleteQueries(static_cast<GLsizei>(TimerQueryName.size()), &TimerQueryName[0]);
		glDeleteTextures(texture::MAX, &TextureName[0]);
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteProgram(ProgramName);
		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return true;
	}

	bool render()
	{
		glBeginQuery(GL_TIME_ELAPSED, this->TimerQueryName[this->TimerQueryIndex]);

		if (this->AtomicType == ATOMIC_IMAGE_ADD)
			glCopyImageSubData(
				this->TextureName[texture::CLEAR], GL_TEXTURE_2D, 0, 0, 0, 0,
				this->TextureName[texture::ATOMIC_COUNTER], GL_TEXTURE_2D, 0, 0, 0, 0,
				1, 1, 1);
		else
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

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
	CSV.header("atomic-counter ; AtomicOpsCount ; AtomicType ; BarrierMode ; AtomicOpsPredicat.x ; AtomicOpsPredicat.y");

	std::size_t const Frames = 1000;
	glm::uvec2 const WindowSize(2400, 1200);

	//for (int BarrierMode = 0; BarrierMode < sample_atomic_counter::BARRIER_COUNT; ++BarrierMode)
	int BarrierMode = sample_atomic_counter::BARRIER_ONCE;
	for (int AtomicMode = 0; AtomicMode < sample_atomic_counter::ATOMIC_TYPE_COUNT; ++AtomicMode)
	for (int AtomicCount = 1; AtomicCount <= 8; ++AtomicCount)
	{
		sample_atomic_counter Test(
			argc, argv, CSV, WindowSize, Frames,
			static_cast<sample_atomic_counter::atomic_type>(AtomicMode), AtomicCount, glm::ivec2(1, 1), static_cast<sample_atomic_counter::barrier>(BarrierMode));
		Error += Test();
	}

/* FULL
	for (int BarrierMode = 0; BarrierMode < sample_atomic_counter::BARRIER_COUNT; ++BarrierMode)
	for (int AtomicMode = 0; AtomicMode < sample_atomic_counter::ATOMIC_TYPE_COUNT; ++AtomicMode)
	for (int AtomicCount = 1; AtomicCount <= 8; ++AtomicCount)
	for (int AtomicPredicatY = 1; AtomicPredicatY <= 4; ++AtomicPredicatY)
	for (int AtomicPredicatX = 1; AtomicPredicatX <= 4; ++AtomicPredicatX)
	{
		sample_atomic_counter Test(
			argc, argv, CSV, WindowSize, Frames,
			static_cast<sample_atomic_counter::atomic_type>(AtomicMode), AtomicCount, glm::ivec2(AtomicPredicatX, AtomicPredicatY), static_cast<sample_atomic_counter::barrier>(BarrierMode));
		Error += Test();
	}
*/
	CSV.save("../log.csv");

	return Error;
}

