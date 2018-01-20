#pragma once
/*
#include "framework.hpp"

void initDSA();

typedef void (GLAPIENTRY * PFNGLBLITNAMEDFRAMEBUFFERPROC) (GLuint src, GLuint dst, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern PFNGLBLITNAMEDFRAMEBUFFERPROC glBlitNamedFramebuffer;

typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUS) (GLuint framebuffer, GLenum target);
extern PFNGLCHECKNAMEDFRAMEBUFFERSTATUS glCheckNamedFramebufferStatus;

typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERDATA) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data);
extern PFNGLCLEARNAMEDBUFFERDATA glClearNamedBufferData;

typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERSUBDATA) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data);
extern PFNGLCLEARNAMEDBUFFERSUBDATA glClearNamedBufferSubData;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1D) (GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTUREIMAGE1D glCompressedTextureImage1D;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2D) (GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTUREIMAGE2D glCompressedTextureImage2D;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3D) (GLuint texture, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTUREIMAGE3D glCompressedTextureImage3D;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1D) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLint border, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1D glCompressedTextureSubImage1D;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2D) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2D glCompressedTextureSubImage2D;

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3D) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3D glCompressedTextureSubImage3D;

typedef void (GLAPIENTRY * PFNGLCREATEBUFFERSPROC) (GLsizei n, GLuint* buffers);
extern PFNGLCREATEBUFFERSPROC glCreateBuffers;

typedef void (GLAPIENTRY * PFNGLCREATEFRAMEBUFFERSPROC) (GLsizei n, GLuint* ids);
extern PFNGLCREATEFRAMEBUFFERSPROC glCreateFramebuffers;

typedef void (GLAPIENTRY * PFNGLCREATEPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
extern PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines;

typedef void (GLAPIENTRY * PFNGLCREATEQUERIESPROC) (GLsizei n, GLuint* queries);
extern PFNGLCREATEQUERIESPROC glCreateQueries;

typedef void (GLAPIENTRY * PFNGLCREATERENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
extern PFNGLCREATERENDERBUFFERSPROC glCreateRenderbuffers;

typedef void (GLAPIENTRY * PFNGLCREATESAMPLERSPROC) (GLsizei n, GLuint* samplers);
extern PFNGLCREATESAMPLERSPROC glCreateSamplers;

typedef void (GLAPIENTRY * PFNGLCREATETEXTURESPROC) (GLenum target, GLsizei n, GLuint* samplers);
extern PFNGLCREATETEXTURESPROC glCreateTextures;

typedef void (GLAPIENTRY * PFNGLCREATETRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
extern PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks;

typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKIVPROC) (GLuint xfb, GLenum pname, GLint *param);
extern PFNGLGETTRANSFORMFEEDBACKIVPROC glGetTransformFeedbackiv;

typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKI_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint *param);
extern PFNGLGETTRANSFORMFEEDBACKI_VPROC glGetTransformFeedbacki_v;

typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKI64_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
extern PFNGLGETTRANSFORMFEEDBACKI64_VPROC glGetTransformFeedbacki64_v;

typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKBOOLLEANVPROC) (GLuint xfb, GLenum pname, GLuint index, GLboolean *param);
extern PFNGLGETTRANSFORMFEEDBACKBOOLLEANVPROC glGetTransformFeedbackBooleanv;

typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) (GLuint xfb, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange;

typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) (GLuint xfb, GLenum target, GLuint index, GLuint buffer);
extern PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase;

typedef void (GLAPIENTRY * PFNGLINVALIDATENAMEDFRAMEBUFFERPROC) (GLuint framebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments);
extern PFNGLINVALIDATENAMEDFRAMEBUFFERPROC glInvalidateNamedFramebuffer;

typedef void (GLAPIENTRY * PFNGLINVALIDATENAMEDSUBFRAMEBUFFERPROC) (GLuint framebuffer, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLINVALIDATENAMEDSUBFRAMEBUFFERPROC glInvalidateNamedSubFramebuffer;

typedef void (GLAPIENTRY * PFNGLCREATEVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;

typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
extern PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib;

typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYELEMENTBUFFERPROC) (GLuint vaobj, GLuint buffer);
extern PFNGLVERTEXARRAYELEMENTBUFFERPROC glVertexArrayElementBuffer;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXBUFFERPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern PFNGLVERTEXARRAYVERTEXBUFFERPROC glVertexArrayVertexBuffer;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern PFNGLVERTEXARRAYVERTEXATTRIBFORMATPROC glVertexArrayVertexAttribFormat;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFNGLVERTEXARRAYVERTEXATTRIBIFORMATPROC glVertexArrayVertexAttribIFormat;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBLFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern PFNGLVERTEXARRAYVERTEXATTRIBLFORMATPROC glVertexArrayVertexAttribLFormat;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBBINDINGPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
extern PFNGLVERTEXARRAYVERTEXATTRIBBINDINGPROC glVertexArrayVertexAttribBinding;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXBINDINGDIVISORPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);
extern PFNGLVERTEXARRAYVERTEXBINDINGDIVISORPROC glVertexArrayVertexBindingDivisor;

typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange;

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERPROC) (GLuint framebuffer, GLenum mode);
extern PFNGLFRAMEBUFFERDRAWBUFFERPROC glFramebufferDrawBuffer;

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWSBUFFERPROC) (GLuint framebuffer, GLsizei n, const GLenum *bufs);
extern PFNGLFRAMEBUFFERDRAWSBUFFERPROC glFramebufferDrawsBuffer;

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFERPROC) (GLuint framebuffer, GLenum mode);
extern PFNGLFRAMEBUFFERREADBUFFERPROC glFramebufferReadBuffer;

typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPPROC) (GLuint texture);
//extern PFNGLGENERATETEXTUREMIPMAPPROC glGenerateTextureMipmap;

typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) (GLuint texture, GLint level, void *img);
extern PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC glGetCompressedTextureImage;

typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERPARAMETERIVPROC) (GLuint framebuffer, GLenum pname, GLint *param);
extern PFNGLNAMEDFRAMEBUFFERPARAMETERIVPROC glGetNamedFramebufferParameteriv;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERPARAMETERIVPROC) (GLuint buffer, GLenum pname, GLint *params);
extern PFNGLNAMEDBUFFERPARAMETERIVPROC glGetNamedBufferParameteriv;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERPARAMETERI64VPROC) (GLuint buffer, GLenum pname, GLint64 *params);
extern PFNGLNAMEDBUFFERPARAMETERI64VPROC glGetNamedBufferParameteri64v;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERPOINTERVPROC) (GLuint buffer, GLenum pname, void **params);
extern PFNGLNAMEDBUFFERPOINTERVPROC glGetNamedBufferPointerv;

typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
extern PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData;

typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
extern PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv;

typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) (GLuint renderbuffer, GLenum pname, GLint *params);
extern PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC glGetNamedRenderbufferParameteriv;

typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, void *pixels);
extern PFNGLGETTEXTUREIMAGEPROC glGetTextureImage;

typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVPROC) (GLuint texture, GLint level, GLenum pname, GLfloat *params);
extern PFNGLGETTEXTURELEVELPARAMETERFVPROC glGetTextureLevelParameterfv;

typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVPROC) (GLuint texture, GLint level, GLenum pname, GLint *params);
extern PFNGLGETTEXTURELEVELPARAMETERIVPROC glGetTextureLevelParameteriv;

typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, GLfloat *params);
extern PFNGLGETTEXTUREPARAMETERFVPROC glGetTextureParameterfv;

typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, GLint *params);
extern PFNGLGETTEXTUREPARAMETERIIVPROC glGetTextureParameterIiv;

typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, GLuint *params);
extern PFNGLGETTEXTUREPARAMETERIUIVPROC glGetTextureParameterIuiv;

typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, GLint *params);
extern PFNGLGETTEXTUREPARAMETERIVPROC glGetTextureParameteriv;

typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTERGERI_VPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint *param);
extern PFNGLGETVERTEXARRAYINTERGERI_VPROC glGetVertexArrayIntegeri_v;

typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VPROC) (GLuint vaobj, GLuint index, GLenum pname, void **param);
extern PFNGLGETVERTEXARRAYPOINTERI_VPROC glGetVertexArrayPointeri_v;

typedef void* (GLAPIENTRY * PFNGLMAPNAMEDBUFFERPROC) (GLuint buffer, GLenum access);
extern PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer;

typedef void* (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
extern PFNGLNAMEDBUFFERDATAPROC glNamedBufferData;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
extern PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData;

typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSTORAGEPROC) (GLuint buffer, GLsizeiptr size, const GLvoid* data, GLbitfield flags);
extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage;

typedef void (GLAPIENTRY * PFNGLCOPYNAMEDBUFFERSUBDATAPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData;

typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC glNamedFramebufferRenderbuffer;

typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC glNamedFramebufferTexture;

typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC glNamedFramebufferTextureLayer;

typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLNAMEDRENDERBUFFERSTORAGEPROC glNamedRenderbufferStorage;

typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC glNamedRenderbufferStorageMultisample;

typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERPROC) (GLuint texture, GLenum internalformat, GLuint buffer);
extern PFNGLTEXTUREBUFFERPROC glTextureBuffer;

typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERRANGEPROC) (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern PFNGLTEXTUREBUFFERRANGEPROC glTextureBufferRange;

typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE1DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
extern PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D;

typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;

typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D;

typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC glTextureStorage2DMultisample;

typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC glTextureStorage3DMultisample;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFPROC) (GLuint texture, GLenum pname, GLfloat param);
extern PFNGLTEXTUREPARAMETERFPROC glTextureParameterf;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, const GLfloat *param);
extern PFNGLTEXTUREPARAMETERFVPROC glTextureParameterfv;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIPROC) (GLuint texture, GLenum pname, GLint param);
extern PFNGLTEXTUREPARAMETERIPROC glTextureParameteri;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, const GLint *params);
extern PFNGLTEXTUREPARAMETERIIVPROC glTextureParameterIiv;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, const GLuint *params);
extern PFNGLTEXTUREPARAMETERIUIVPROC glTextureParameterIuiv;

typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, const GLint *param);
extern PFNGLTEXTUREPARAMETERIVPROC glTextureParameteriv;

typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D;

typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;

typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
extern PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D;

typedef void (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFERPROC) (GLuint buffer);
extern PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
extern PFNGLVERTEXARRAYVERTEXATTRIBOFFSETPROC glVertexArrayVertexAttribOffset;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETIPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFNGLVERTEXARRAYVERTEXATTRIBOFFSETIPROC glVertexArrayVertexAttribIOffset;

typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETLPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
extern PFNGLVERTEXARRAYVERTEXATTRIBOFFSETLPROC glVertexArrayVertexAttribLOffset;
*/
