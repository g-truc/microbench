#include "test.hpp"

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
		#version 430 core

		#define FRAG_COLOR	0

		layout(binding = 0, offset = 0) uniform atomic_uint Atomic;

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		void main()
		{
			uint Mask = (1 << 8) - 1;
			uint CounterA = atomicCounterIncrement(Atomic);
			uint CounterB = atomicCounterIncrement(Atomic);
			uint CounterC = atomicCounterIncrement(Atomic);
			uint Counter = atomicCounterIncrement(Atomic);

			Color = vec4(
				((Counter & (Mask <<  0)) % 255) / 255.f,
				((Counter & (Mask <<  8)) % 255) / 255.f,
				((Counter & (Mask << 16)) % 255) / 63.f,
				1.0 / 16.0);
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
}//namespace

class sample : public framework
{
public:
	sample(int argc, char* argv[]) :
		framework(argc, argv, "atomic-counter", framework::CORE, 4, 3, glm::uvec2(2400, 1200), glm::vec2(0), glm::vec2(0), 2, framework::RUN_ONLY),
		PipelineName(0),
		ProgramName(0),
		VertexArrayName(0)
	{}

private:
	std::array<GLuint, buffer::MAX> BufferName;
	GLuint PipelineName;
	GLuint ProgramName;
	GLuint VertexArrayName;

	bool initProgram()
	{
		GLuint VertShaderName = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertShaderName, 1, &VERT_SHADER_SOURCE, NULL);
		glCompileShader(VertShaderName);

		GLuint FragShaderName = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragShaderName, 1, &FRAG_SHADER_SOURCE, NULL);
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

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
		glBindVertexArray(0);

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();

		glm::vec2 WindowSize(this->getWindowSize());
		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

		glBindBuffer(GL_COPY_READ_BUFFER, BufferName[buffer::CLEAR]);
		glBindBuffer(GL_COPY_WRITE_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);

		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, BufferName[buffer::ATOMIC_COUNTER]);

		return Validated;
	}

	bool end()
	{
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteProgram(ProgramName);
		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return true;
	}

	bool render()
	{
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error = 0;

	sample Sample(argc, argv);
	Error += Sample();

	return Error;
}

