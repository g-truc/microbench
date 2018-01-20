#include "test.hpp"
#include "dsa.hpp"
/*
namespace
{
	std::vector<GLenum> TextureTargets;

	GLenum getTextureBinding(GLenum target)
	{
		switch(target)
		{
		case GL_TEXTURE_1D:
			return GL_TEXTURE_BINDING_1D;
		case GL_TEXTURE_1D_ARRAY:
			return GL_TEXTURE_BINDING_1D_ARRAY;
		case GL_TEXTURE_2D:
			return GL_TEXTURE_BINDING_2D;
		case GL_TEXTURE_2D_MULTISAMPLE:
			return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
		case GL_TEXTURE_2D_ARRAY:
			return GL_TEXTURE_BINDING_2D_ARRAY;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
		case GL_TEXTURE_3D:
			return GL_TEXTURE_BINDING_3D;
		case GL_TEXTURE_RECTANGLE:
			return GL_TEXTURE_BINDING_RECTANGLE;
		case GL_TEXTURE_CUBE_MAP:
			return GL_TEXTURE_BINDING_CUBE_MAP;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
		}

		assert(0);
		return 0;
	}

	GLenum getFramebufferBinding(GLenum target)
	{
		switch(target)
		{
		case GL_FRAMEBUFFER:
			return GL_FRAMEBUFFER_BINDING;
		case GL_DRAW_FRAMEBUFFER:
			return GL_DRAW_FRAMEBUFFER_BINDING;
		case GL_READ_FRAMEBUFFER:
			return GL_READ_FRAMEBUFFER_BINDING;
		}

		assert(0);
		return 0;
	}

	void blitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer,
		int srcX0, int srcY0,
		int srcX1, int srcY1,
		int dstX0, int dstY0,
		int dstX1, int dstY1,
		GLbitfield mask, GLenum filter)
	{
		GLint readRestaure = 0;
		GLint drawRestaure = 0;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readRestaure);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawRestaure);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);
		glBlitFramebuffer(
			srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1, mask, filter);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readRestaure);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawRestaure);
	}

	GLenum checkNamedFramebufferStatus(GLuint framebuffer, GLenum target)
	{
		GLint restaure = 0;
		glGetIntegerv(getFramebufferBinding(target), &restaure);
		glBindFramebuffer(target, framebuffer);
		GLenum Status = glCheckFramebufferStatus(target);
		glBindFramebuffer(target, restaure);

		return Status;
	}

	void clearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glClearNamedBufferData(GL_ARRAY_BUFFER, internalformat, format, type, data);
		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void clearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glClearNamedBufferSubData(GL_ARRAY_BUFFER, internalformat, offset, size, format, type, data);
		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void compressedTextureImage1D(GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
		glBindTexture(target, restaure);
	}

	void compressedTextureImage2D(GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
		glBindTexture(target, restaure);
	}

	void compressedTextureImage3D(GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
		glBindTexture(target, restaure);
	}

	void compressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLint border, GLenum format, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
		glBindTexture(target, restaure);
	}

	void compressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
		glBindTexture(target, restaure);
	}

	void compressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);
		glBindTexture(target, texture);
		glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
		glBindTexture(target, restaure);
	}

	void createBuffers(GLsizei n, GLuint* buffers)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glGenBuffers(n, buffers);
		for(GLsizei i = 0; i < n; ++i)
			glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void createFramebuffers(GLsizei n, GLuint* ids)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glGenFramebuffers(n, ids);
		for(GLsizei i = 0; i < n; ++i)
			glBindFramebuffer(GL_FRAMEBUFFER, ids[i]);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void createProgramPipelines(GLsizei n, GLuint* pipelines)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_PROGRAM_PIPELINE_BINDING, &restaure);

		glGenProgramPipelines(n, pipelines);
		for(GLsizei i = 0; i < n; ++i)
			glBindProgramPipeline(pipelines[i]);

		glBindProgramPipeline(restaure);
	}

	void createQueries(GLsizei n, GLuint* ids)
	{
		glCreateQueries(n, ids);
	}

	void createRenderbuffers(GLsizei n, GLuint* ids)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &restaure);

		glGenRenderbuffers(n, ids);
		for(GLsizei i = 0; i < n; ++i)
			glBindRenderbuffer(GL_RENDERBUFFER, ids[i]);

		glBindRenderbuffer(GL_RENDERBUFFER, restaure);
	}

	void createSamplers(GLsizei n, GLuint* ids)
	{
		GLint restaure = 0;
		glGetIntegeri_v(GL_SAMPLER_BINDING, 0, &restaure);

		glCreateSamplers(n, ids);
		for(GLsizei i = 0; i < n; ++i)
			glBindSampler(0, ids[i]);

		glBindSampler(0, restaure);
	}

	void createTextures(GLenum target, GLsizei n, GLuint* textures)
	{
		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glGenTextures(n, textures);

		for(GLsizei i = 0; i < n; ++i)
		{
			GLuint name = textures[i];
			if(name >= TextureTargets.size())
				TextureTargets.resize(name + 1);
			TextureTargets[name] = target;
			glBindTexture(target, name);
		}

		glBindTexture(target, restaure);
	}

	void createTransformFeedbacks(GLsizei n, GLuint* ids)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);

		glCreateTransformFeedbacks(n, ids);
		for(GLsizei i = 0; i < n; ++i)
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, ids[i]);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void getTransformFeedbackiv (GLuint xfb, GLenum pname, GLint *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glGetIntegerv(pname, param);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void getTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glGetIntegeri_v(pname, index, param);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void getTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glGetInteger64i_v(pname, index, param);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void getTransformFeedbackBooleanv(GLuint xfb, GLenum pname, GLuint index, GLboolean *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glGetBooleanv(pname, param);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void transformFeedbackBufferRange(GLuint xfb, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer, offset, size);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void transformFeedbackBufferBase(GLuint xfb, GLenum target, GLuint index, GLuint buffer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TRANSFORM_FEEDBACK_BINDING, &restaure);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, restaure);
	}

	void invalidateNamedFramebuffer(GLuint framebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments)
	{
		GLint restaure = 0;
		GLint drawRestaure = 0;
		glGetIntegerv(getFramebufferBinding(target), &restaure);
		glBindFramebuffer(target, framebuffer);
		glInvalidateFramebuffer(target, numAttachments, attachments);
		glBindFramebuffer(target, restaure);
	}

	void invalidateNamedSubFramebuffer(GLuint framebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
	{
		GLint restaure = 0;
		GLint drawRestaure = 0;
		glGetIntegerv(getFramebufferBinding(target), &restaure);
		glBindFramebuffer(target, framebuffer);
		glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
		glBindFramebuffer(target, restaure);
	}

	void createVertexArrays(GLsizei n, GLuint* ids)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);

		glCreateVertexArrays(n, ids);
		for(GLsizei i = 0; i < n; ++i)
			glBindVertexArray(ids[i]);

		glBindVertexArray(restaure);
	}

	void disableVertexArrayAttrib(GLuint vaobj, GLuint index)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glDisableVertexAttribArray(index);

		glBindVertexArray(restaure);
	}

	void enableVertexArrayAttrib(GLuint vaobj, GLuint index)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glEnableVertexAttribArray(index);

		glBindVertexArray(restaure);
	}

	void vertexArrayElementBuffer(GLuint vaobj, GLuint buffer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glBindVertexBuffer(bindingindex, buffer, offset, stride);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glVertexAttribFormat(attribindex, size, type, normalized, relativeoffset);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glVertexAttribIFormat(attribindex, size, type, relativeoffset);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glVertexAttribLFormat(attribindex, size, type, relativeoffset);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glVertexAttribBinding(attribindex, bindingindex);

		glBindVertexArray(restaure);
	}

	void vertexArrayVertexAttribDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glVertexBindingDivisor(bindingindex, divisor);

		glBindVertexArray(restaure);
	}

	void flushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glFlushMappedBufferRange(GL_ARRAY_BUFFER, offset, length);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void framebufferDrawBuffer(GLuint framebuffer, GLenum mode)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glDrawBuffer(mode);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void framebufferDrawsBuffer(GLuint framebuffer, GLsizei n, const GLenum *bufs)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glDrawBuffers(n, bufs);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void framebufferReadBuffer(GLuint framebuffer, GLenum mode)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glReadBuffer(mode);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void generateTextureMipmap(GLuint texture)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glGenerateMipmap(target);

		glBindTexture(target, restaure);
	}

	void getCompressedTextureImage(GLuint texture, GLint level, void *img)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glGetCompressedTexImage(target, level, img);

		glBindTexture(target, restaure);
	}

	void getNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGetFramebufferParameteriv(GL_FRAMEBUFFER, pname, param);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void getNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint *params)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, pname, params);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void getNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 *params)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glGetBufferParameteri64v(GL_ARRAY_BUFFER, pname, params);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void getNamedBufferPointerv(GLuint buffer, GLenum pname, void **params)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glGetBufferPointerv(GL_ARRAY_BUFFER, pname, params);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void getNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void getNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment, pname, params);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void getNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint *params)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &restaure);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, params);

		glBindRenderbuffer(GL_RENDERBUFFER, restaure);
	}

	void getTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, void *pixels)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glGetTexImage(target, level, format, type, pixels);

		glBindTexture(target, restaure);
	}

	void getTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glGetTexLevelParameterfv(target, level, pname, params);

		glBindTexture(target, restaure);
	}

	void getTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glGetTexLevelParameteriv(target, level, pname, params);

		glBindTexture(target, restaure);
	}

	void getTextureParameterfv(GLuint texture, GLenum pname, GLfloat *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterfv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void getTextureParameterIiv(GLuint texture, GLenum pname, GLint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterIiv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void getTextureParameterIuiv(GLuint texture, GLenum pname, GLuint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterIuiv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void getTextureParameteriv(GLuint texture, GLenum pname, GLint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameteriv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void getVertexArrayIntegeri_v(GLuint vaobj, GLuint index, GLenum pname, GLint *param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glGetVertexAttribIiv(index, pname, param);

		glBindVertexArray(restaure);
	}

	void getVertexArrayPointeri_v(GLuint vaobj, GLuint index, GLenum pname, void **param)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaure);
		glBindVertexArray(vaobj);
		glGetVertexAttribPointerv(index, pname, param);

		glBindVertexArray(restaure);
	}

	void* mapNamedBuffer(GLuint buffer, GLenum access)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		void* pointer = glMapBuffer(GL_ARRAY_BUFFER, access);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);

		return pointer;
	}

	void* mapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		void* pointer = glMapBufferRange(GL_ARRAY_BUFFER, offset, length, access);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);

		return pointer;
	}

	void namedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void namedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void namedBufferStorage(GLuint buffer, GLsizeiptr size, const GLvoid* data, GLbitfield flags)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferStorage(GL_ARRAY_BUFFER, size, data, flags);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void copyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
	{
		GLint restaureReadBuffer = 0;
		GLint restaureWriteBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaureReadBuffer);
		glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &restaureWriteBuffer);

		glBindBuffer(GL_ARRAY_BUFFER_BINDING, readBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER_BINDING, writeBuffer);
		glCopyBufferSubData(GL_ARRAY_BUFFER_BINDING, GL_UNIFORM_BUFFER_BINDING, readOffset, writeOffset, size);

		glBindBuffer(GL_ARRAY_BUFFER_BINDING, restaureReadBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER_BINDING, restaureWriteBuffer);
	}

	void namedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, renderbuffertarget, renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void namedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture, level);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void namedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &restaure);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture, level, layer);

		glBindFramebuffer(GL_FRAMEBUFFER, restaure);
	}

	void namedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &restaure);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, restaure);
	}

	void namedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &restaure);

		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalformat, width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, restaure);
	}

	void textureBuffer(GLuint texture, GLenum internalformat, GLuint buffer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &restaure);

		glBindTexture(GL_TEXTURE_BUFFER, texture);
		glTexBuffer(GL_TEXTURE_BUFFER, internalformat, buffer);

		glBindTexture(GL_TEXTURE_BUFFER, restaure);
	}

	void textureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_BUFFER, &restaure);

		glBindTexture(GL_TEXTURE_BUFFER, texture);
		glTexBufferRange(GL_TEXTURE_BUFFER, internalformat, buffer, offset, size);

		glBindTexture(GL_TEXTURE_BUFFER, restaure);
	}

	void textureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexStorage1D(target, levels, internalformat, width);

		glBindTexture(target, restaure);
	}

	void textureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexStorage2D(target, levels, internalformat, width, height);

		glBindTexture(target, restaure);
	}

	void textureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexStorage3D(target, levels, internalformat, width, height, depth);

		glBindTexture(target, restaure);
	}

	void textureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);

		glBindTexture(target, restaure);
	}

	void textureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);

		glBindTexture(target, restaure);
	}

	void textureParameterf(GLuint texture, GLenum pname, GLfloat param)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterf(target, pname, param);

		glBindTexture(target, restaure);
	}

	void textureParameterfv(GLuint texture, GLenum pname, const GLfloat *param)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterfv(target, pname, param);

		glBindTexture(target, restaure);
	}

	void textureParameteri(GLuint texture, GLenum pname, GLint param)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameteri(target, pname, param);

		glBindTexture(target, restaure);
	}

	void textureParameterIiv(GLuint texture, GLenum pname, const GLint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterIiv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void textureParameterIuiv(GLuint texture, GLenum pname, const GLuint *params)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameterIuiv(target, pname, params);

		glBindTexture(target, restaure);
	}

	void textureParameteriv(GLuint texture, GLenum pname, const GLint *param)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexParameteriv(target, pname, param);

		glBindTexture(target, restaure);
	}

	void textureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexSubImage1D(target, level, xoffset, width, format, type, pixels);

		glBindTexture(target, restaure);
	}

	void textureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);

		glBindTexture(target, restaure);
	}

	void textureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
	{
		GLenum target = TextureTargets[texture];

		GLint restaure = 0;
		glGetIntegerv(getTextureBinding(target), &restaure);

		glBindTexture(target, texture);
		glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);

		glBindTexture(target, restaure);
	}

	void unmapNamedBuffer(GLuint buffer)
	{
		GLint restaure = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaure);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glBindBuffer(GL_ARRAY_BUFFER, restaure);
	}

	void vertexArrayVertexAttribOffset(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)
	{
		GLint restaureVertexArray = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaureVertexArray);

		GLint restaureBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaureBuffer);

		glBindVertexArray(vaobj);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(index, size, type, normalized, stride, BUFFER_OFFSET(offset));

		glBindVertexArray(restaureVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, restaureBuffer);
	}

	void vertexArrayVertexAttribIOffset(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
	{
		GLint restaureVertexArray = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaureVertexArray);

		GLint restaureBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaureBuffer);

		glBindVertexArray(vaobj);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribIPointer(index, size, type, stride, BUFFER_OFFSET(offset));

		glBindVertexArray(restaureVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, restaureBuffer);
	}

	void vertexArrayVertexAttribLOffset(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)
	{
		GLint restaureVertexArray = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &restaureVertexArray);

		GLint restaureBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &restaureBuffer);

		glBindVertexArray(vaobj);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribLPointer(index, size, type, stride, BUFFER_OFFSET(offset));

		glBindVertexArray(restaureVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, restaureBuffer);
	}
}//namespace

PFNGLBLITNAMEDFRAMEBUFFERPROC glBlitNamedFramebuffer = 0;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUS glCheckNamedFramebufferStatus = 0;
PFNGLCLEARNAMEDBUFFERDATA glClearNamedBufferData = 0;
PFNGLCLEARNAMEDBUFFERSUBDATA glClearNamedBufferSubData = 0;
PFNGLCOMPRESSEDTEXTUREIMAGE1D glCompressedTextureImage1D = 0;
PFNGLCOMPRESSEDTEXTUREIMAGE2D glCompressedTextureImage2D = 0;
PFNGLCOMPRESSEDTEXTUREIMAGE3D glCompressedTextureImage3D = 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1D glCompressedTextureSubImage1D = 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2D glCompressedTextureSubImage2D = 0;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3D glCompressedTextureSubImage3D = 0;
PFNGLCREATEBUFFERSPROC glCreateBuffers = 0;
PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers = 0;
PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines = 0;
PFNGLCREATEQUERIESPROC glCreateQueries = 0;
PFNGLCREATERENDERBUFFERSPROC glCreateRenderbuffers = 0;
PFNGLCREATESAMPLERSPROC glCreateSamplers = 0;
PFNGLCREATETEXTURESPROC glCreateTextures = 0;
PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks = 0;
PFNGLGETTRANSFORMFEEDBACKIVPROC glGetTransformFeedbackiv = 0;
PFNGLGETTRANSFORMFEEDBACKI_VPROC glGetTransformFeedbacki_v = 0;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC glGetTransformFeedbacki64_v = 0;
PFNGLGETTRANSFORMFEEDBACKBOOLLEANVPROC glGetTransformFeedbackBooleanv = 0;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange = 0;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase = 0;
PFNGLINVALIDATENAMEDFRAMEBUFFERPROC glInvalidateNamedFramebuffer = 0;
PFNGLINVALIDATENAMEDSUBFRAMEBUFFERPROC glInvalidateNamedSubFramebuffer = 0;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays = 0;
PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib = 0;
PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib = 0;
PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer = 0;
PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer = 0;
PFNGLVERTEXARRAYVERTEXATTRIBFORMATPROC glVertexArrayVertexAttribFormat = 0;
PFNGLVERTEXARRAYVERTEXATTRIBIFORMATPROC glVertexArrayVertexAttribIFormat = 0;
PFNGLVERTEXARRAYVERTEXATTRIBLFORMATPROC glVertexArrayVertexAttribLFormat = 0;
PFNGLVERTEXARRAYVERTEXATTRIBBINDINGPROC glVertexArrayVertexAttribBinding = 0;
PFNGLVERTEXARRAYVERTEXBINDINGDIVISORPROC glVertexArrayVertexBindingDivisor = 0;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange = 0;
PFNGLFRAMEBUFFERDRAWBUFFERPROC glFramebufferDrawBuffer = 0;
PFNGLFRAMEBUFFERDRAWSBUFFERPROC glFramebufferDrawsBuffer = 0;
PFNGLFRAMEBUFFERREADBUFFERPROC glFramebufferReadBuffer = 0;
PFNGLGENERATETEXTUREMIPMAPPROC glGenerateTextureMipmap = 0;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glGetCompressedTextureImage = 0;
PFNGLNAMEDFRAMEBUFFERPARAMETERIVPROC glGetNamedFramebufferParameteriv = 0;
PFNGLNAMEDBUFFERPARAMETERIVPROC glGetNamedBufferParameteriv = 0;
PFNGLNAMEDBUFFERPARAMETERI64VPROC glGetNamedBufferParameteri64v = 0;
PFNGLNAMEDBUFFERPOINTERVPROC glGetNamedBufferPointerv = 0;
PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData = 0;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv = 0;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glGetNamedRenderbufferParameteriv = 0;
PFNGLGETTEXTUREIMAGEPROC glGetTextureImage = 0;
PFNGLGETTEXTURELEVELPARAMETERFVPROC glGetTextureLevelParameterfv = 0;
PFNGLGETTEXTURELEVELPARAMETERIVPROC glGetTextureLevelParameteriv = 0;
PFNGLGETTEXTUREPARAMETERFVPROC glGetTextureParameterfv = 0;
PFNGLGETTEXTUREPARAMETERIIVPROC glGetTextureParameterIiv = 0;
PFNGLGETTEXTUREPARAMETERIUIVPROC glGetTextureParameterIuiv = 0;
PFNGLGETTEXTUREPARAMETERIVPROC glGetTextureParameteriv = 0;
PFNGLGETVERTEXARRAYINTERGERI_VPROC glGetVertexArrayIntegeri_v = 0;
PFNGLGETVERTEXARRAYPOINTERI_VPROC glGetVertexArrayPointeri_v = 0;
PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer = 0;
PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange = 0;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData = 0;
PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData = 0;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage = 0;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData = 0;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glNamedFramebufferRenderbuffer = 0;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture = 0;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glNamedFramebufferTextureLayer = 0;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC glNamedRenderbufferStorage = 0;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glNamedRenderbufferStorageMultisample = 0;
PFNGLTEXTUREBUFFERPROC glTextureBuffer = 0;
PFNGLTEXTUREBUFFERRANGEPROC glTextureBufferRange = 0;
PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D = 0;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D = 0;
PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D = 0;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glTextureStorage2DMultisample = 0;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glTextureStorage3DMultisample = 0;
PFNGLTEXTUREPARAMETERFPROC glTextureParameterf = 0;
PFNGLTEXTUREPARAMETERFVPROC glTextureParameterfv = 0;
PFNGLTEXTUREPARAMETERIPROC glTextureParameteri = 0;
PFNGLTEXTUREPARAMETERIIVPROC glTextureParameterIiv = 0;
PFNGLTEXTUREPARAMETERIUIVPROC glTextureParameterIuiv = 0;
PFNGLTEXTUREPARAMETERIVPROC glTextureParameteriv = 0;
PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D = 0;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D = 0;
PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D = 0;
PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer = 0;
PFNGLVERTEXARRAYVERTEXATTRIBOFFSETPROC glVertexArrayVertexAttribOffset = 0;
PFNGLVERTEXARRAYVERTEXATTRIBOFFSETIPROC glVertexArrayVertexAttribIOffset = 0;
PFNGLVERTEXARRAYVERTEXATTRIBOFFSETLPROC glVertexArrayVertexAttribLOffset = 0;

void initDSA()
{
	glBlitNamedFramebuffer = ::blitNamedFramebuffer;
	glCheckNamedFramebufferStatus = ::checkNamedFramebufferStatus;
	glClearNamedBufferData = ::clearNamedBufferData;
	glClearNamedBufferSubData = ::clearNamedBufferSubData;
	glCompressedTextureImage1D = ::compressedTextureImage1D;
	glCompressedTextureImage2D = ::compressedTextureImage2D;
	glCompressedTextureImage3D = ::compressedTextureImage3D;
	glCompressedTextureSubImage1D = ::compressedTextureSubImage1D;
	glCompressedTextureSubImage2D = ::compressedTextureSubImage2D;
	glCompressedTextureSubImage3D = ::compressedTextureSubImage3D;
	glCreateBuffers = ::createBuffers;
	glCreateFramebuffers = ::createFramebuffers;
	glCreateProgramPipelines = ::createProgramPipelines;
	glCreateQueries = ::createQueries;
	glCreateRenderbuffers = ::createRenderbuffers;
	glCreateSamplers = ::createSamplers;
	glCreateTextures = ::createTextures;
	glCreateTransformFeedbacks = ::createTransformFeedbacks;
	glGetTransformFeedbackiv = ::getTransformFeedbackiv;
	glGetTransformFeedbacki_v = ::getTransformFeedbacki_v;
	glGetTransformFeedbacki64_v = ::getTransformFeedbacki64_v;
	glGetTransformFeedbackBooleanv = ::getTransformFeedbackBooleanv;
	glTransformFeedbackBufferRange = ::transformFeedbackBufferRange;
	glTransformFeedbackBufferBase = ::transformFeedbackBufferBase;
	glInvalidateNamedFramebuffer = ::invalidateNamedFramebuffer;
	glInvalidateNamedSubFramebuffer = ::invalidateNamedSubFramebuffer;
	glCreateVertexArrays = ::createVertexArrays;
	glDisableVertexArrayAttrib = ::disableVertexArrayAttrib;
	glEnableVertexArrayAttrib = ::enableVertexArrayAttrib;
	glVertexArrayElementBuffer = ::vertexArrayElementBuffer;
	glVertexArrayVertexBuffer = ::vertexArrayVertexBuffer;
	glVertexArrayVertexAttribFormat = ::vertexArrayVertexAttribFormat;
	glVertexArrayVertexAttribIFormat = ::vertexArrayVertexAttribIFormat;
	glVertexArrayVertexAttribLFormat = ::vertexArrayVertexAttribLFormat;
	glVertexArrayVertexAttribBinding = ::vertexArrayVertexAttribBinding;
	glVertexArrayVertexBindingDivisor = ::vertexArrayVertexAttribDivisor;
	glFlushMappedNamedBufferRange = ::flushMappedNamedBufferRange;
	glFramebufferDrawBuffer = ::framebufferDrawBuffer;
	glFramebufferDrawsBuffer = ::framebufferDrawsBuffer;
	glFramebufferReadBuffer = ::framebufferReadBuffer;
	glGenerateTextureMipmap = ::generateTextureMipmap;
	glGetCompressedTextureImage = ::getCompressedTextureImage;
	glGetNamedFramebufferParameteriv = ::getNamedFramebufferParameteriv;
	glGetNamedBufferParameteriv = ::getNamedBufferParameteriv;
	glGetNamedBufferParameteri64v = ::getNamedBufferParameteri64v;
	glGetNamedBufferPointerv = ::getNamedBufferPointerv;
	glGetNamedBufferSubData = ::getNamedBufferSubData;
	glGetNamedFramebufferAttachmentParameteriv = ::getNamedFramebufferAttachmentParameteriv;
	glGetNamedRenderbufferParameteriv = ::getNamedRenderbufferParameteriv;
	glGetTextureImage = ::getTextureImage;
	glGetTextureLevelParameterfv = ::getTextureLevelParameterfv;
	glGetTextureLevelParameteriv = ::getTextureLevelParameteriv;
	glGetTextureParameterfv = ::getTextureParameterfv;
	glGetTextureParameterIiv = ::getTextureParameterIiv;
	glGetTextureParameterIuiv = ::getTextureParameterIuiv;
	glGetTextureParameteriv = ::getTextureParameteriv;
	glGetVertexArrayIntegeri_v = ::getVertexArrayIntegeri_v;
	glGetVertexArrayPointeri_v = ::getVertexArrayPointeri_v;
	glMapNamedBuffer = ::mapNamedBuffer;
	glMapNamedBufferRange = ::mapNamedBufferRange;
	glNamedBufferData = ::namedBufferData;
	glNamedBufferSubData = ::namedBufferSubData;
	glNamedBufferStorage = ::namedBufferStorage;
	glCopyNamedBufferSubData = ::copyNamedBufferSubData;
	glNamedFramebufferRenderbuffer = ::namedFramebufferRenderbuffer;
	glNamedFramebufferTexture = ::namedFramebufferTexture;
	glNamedFramebufferTextureLayer = ::namedFramebufferTextureLayer;
	glNamedRenderbufferStorage = ::namedRenderbufferStorage;
	glNamedRenderbufferStorageMultisample = ::namedRenderbufferStorageMultisample;
	glTextureBuffer = ::textureBuffer;
	glTextureBufferRange = ::textureBufferRange;
	glTextureStorage1D = ::textureStorage1D;
	glTextureStorage2D = ::textureStorage2D;
	glTextureStorage3D = ::textureStorage3D;
	glTextureStorage2DMultisample = ::textureStorage2DMultisample;
	glTextureStorage3DMultisample = ::textureStorage3DMultisample;
	glTextureParameterf = ::textureParameterf;
	glTextureParameterfv = ::textureParameterfv;
	glTextureParameteri = ::textureParameteri;
	glTextureParameterIiv = ::textureParameterIiv;
	glTextureParameterIuiv = ::textureParameterIuiv;
	glTextureParameteriv = ::textureParameteriv;
	glTextureSubImage1D = ::textureSubImage1D;
	glTextureSubImage2D = ::textureSubImage2D;
	glTextureSubImage3D = ::textureSubImage3D;
	glUnmapNamedBuffer = ::unmapNamedBuffer;
	glVertexArrayVertexAttribOffset = ::vertexArrayVertexAttribOffset;
	glVertexArrayVertexAttribIOffset = ::vertexArrayVertexAttribIOffset;
	glVertexArrayVertexAttribLOffset = ::vertexArrayVertexAttribLOffset;
}
*/
