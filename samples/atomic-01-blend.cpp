#include "test.hpp"

namespace
{
	char const* VERT_SHADER_SOURCE = R"(
		#version 430 core

		#define POSITION	0
		#define COLOR		3
		#define TEXCOORD	4
		#define TRANSFORM0	1

		precision highp float;
		precision highp int;
		layout(std140, column_major) uniform;

		layout(binding = TRANSFORM0) uniform transform
		{
			mat4 MVP;
		} Transform;

		layout(location = POSITION) in vec2 Position;
		layout(location = TEXCOORD) in vec2 Texcoord;

		out block
		{
			vec2 Texcoord;
		} Out;

		out gl_PerVertex
		{
			vec4 gl_Position;
		};

		void main()
		{
			Out.Texcoord = Texcoord;
			gl_Position = Transform.MVP * vec4(Position, float(gl_InstanceID) * 0.25 - 0.5, 1.0);
		}
	)";

	char const* FRAG_SHADER_SOURCE = R"(
		#version 430 core

		#define FRAG_COLOR	0

		precision highp float;
		precision highp int;
		layout(std140, column_major) uniform;

		layout(binding = 0, offset = 0) uniform atomic_uint Atomic;

		in block
		{
			vec2 Texcoord;
		} In;

		layout(location = FRAG_COLOR, index = 0) out vec4 Color;

		void main()
		{
			uint Mask = (1 << 8) - 1;
			uint Counter = atomicCounterIncrement(Atomic);

			Color = vec4(
				((Counter & (Mask <<  0)) % 255) / 255.f,
				((Counter & (Mask <<  8)) % 255) / 255.f,
				((Counter & (Mask << 16)) % 255) / 15.f,
				0.5);
		}
	)";

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLushort);
	GLushort const ElementData[ElementCount] =
	{
		0, 1, 2, 
		2, 3, 0
	};

	namespace buffer
	{
		enum type
		{
			VERTEX,
			ELEMENT,
			TRANSFORM,
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
		framework(argc, argv, "atomic-01-blend", framework::CORE, 4, 3, glm::uvec2(1200, 1200), glm::vec2(glm::pi<float>() * 0.2f), framework::RUN_ONLY),
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

		if(VertResult == GL_FALSE)
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
		if(Result == GL_FALSE)
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
		GLint MaxVertexAtomicCounterBuffers(0);
		GLint MaxControlAtomicCounterBuffers(0);
		GLint MaxEvaluationAtomicCounterBuffers(0);
		GLint MaxGeometryAtomicCounterBuffers(0);
		GLint MaxFragmentAtomicCounterBuffers(0);
		GLint MaxCombinedAtomicCounterBuffers(0);

		glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS, &MaxVertexAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS, &MaxControlAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS, &MaxEvaluationAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS, &MaxGeometryAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS, &MaxFragmentAtomicCounterBuffers);
		glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, &MaxCombinedAtomicCounterBuffers);

		glGenBuffers(buffer::MAX, &BufferName[0]);

		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

		glBindBuffer(GL_COPY_READ_BUFFER, BufferName[buffer::CLEAR]);
		GLuint Data = 0;
		glBufferStorage(GL_COPY_READ_BUFFER, sizeof(GLuint), &Data, GL_MAP_WRITE_BIT);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);

		return true;
	}

	bool initVertexArray()
	{
		bool Validated(true);

		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
			glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), BUFFER_OFFSET(0));
			glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), BUFFER_OFFSET(sizeof(glm::vec2)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(semantic::attr::POSITION);
			glEnableVertexAttribArray(semantic::attr::TEXCOORD);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		return Validated;
	}

	bool begin()
	{
		bool Validated(true);
		Validated = Validated && this->checkExtension("GL_ARB_clear_buffer_object");

		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();

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
		glm::vec2 WindowSize(this->getWindowSize());

		// Setup blending
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
			glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(
				GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

			glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, WindowSize.x / WindowSize.y, 0.1f, 100.0f);
			glm::mat4 Model = glm::mat4(1.0f);
			glm::mat4 MVP = Projection * this->view() * Model;

			*Pointer = MVP;

			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		//glm::uint Data(0);
		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);
		//glClearBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_R8UI, 0, sizeof(glm::uint), GL_RGBA, GL_UNSIGNED_INT, &Data);

		glBindBuffer(GL_COPY_READ_BUFFER, BufferName[buffer::CLEAR]);
		glBindBuffer(GL_COPY_WRITE_BUFFER, BufferName[buffer::ATOMIC_COUNTER]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, BufferName[buffer::ATOMIC_COUNTER]);
		glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

		glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 5, 0, 0);

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

