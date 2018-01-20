#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include "test.hpp"

namespace gl
{
	typedef std::uint32_t name_t;

	class noncopyable
	{
	protected:
		noncopyable() = default;
		~noncopyable() = default;

		noncopyable(noncopyable const &) = delete;
		noncopyable& operator=(noncopyable const &) = delete;
	};

	class buffer;

	class map_ptr : noncopyable
	{
		friend class buffer;

	private:
		map_ptr(name_t Name, std::size_t Offset, std::size_t Length, std::uint32_t Access);
		map_ptr(map_ptr const &);
		map_ptr& operator=(map_ptr const &);

	public:
		~map_ptr();

		void flush(std::size_t Offset, std::size_t Length) const;
		void unmap() const;

	private:
		name_t const Name;
		std::size_t const Offset;
		std::size_t const Length;
		std::uint32_t const Access;
	};

	class buffer : noncopyable
	{
	public:
		buffer(std::size_t Size, const void* Data, std::uint32_t Flags);
		~buffer();

		map_ptr const & map(GLintptr Offset, GLsizeiptr Length, GLbitfield Flags);

	private:
		name_t Name;
	};

/*
	class buffer : public noncopyable
	{
		typedef GLuint handle;

		struct mapping
		{
			std::size_t SliceIndex;
			std::uintptr_t Offset;
			std::size_t Size;
		};

	public:
		enum map_flag
		{
			MAP_READ_BIT = (1 << 0),
			MAP_WRITE_BIT = (1 << 1),
			MAP_COHERENT_BIT = (1 << 2),
			MAP_PERSISTENT_BIT = (1 << 3),
			MAP_UNSYNCHRONIZED_BIT = (1 << 4),
			MAP_EXPLICIT_FLUSH_BIT = (1 << 5),
			MAP_INVALIDATE_RANGE_BIT = (1 << 6),
			MAP_INVALIDATE_BUFFER_BIT = (1 << 7)
		};

		enum flag
		{
			READ_BIT = GL_MAP_READ_BIT,
			WRITE_BIT = GL_MAP_WRITE_BIT,
			PERSISTENT_BIT = GL_MAP_PERSISTENT_BIT,
			COHERENT_BIT = GL_MAP_COHERENT_BIT,
			SPARSE_BIT = (1 << 8)
		};

//		enum usage
//		{
//			GL_DYNAMIC_STORAGE_BIT, GL_MAP_READ_BIT GL_MAP_WRITE_BIT, GL_MAP_PERSISTENT_BIT, GL_MAP_COHERENT_BIT, and GL_CLIENT_STORAGE_BIT
//		};

		buffer(std::uint32_t Flags, std::size_t Size);
		buffer(std::uint32_t Flags, std::size_t Size, void const * Data);
		~buffer();

		void page(std::uintptr_t Offset, std::size_t Size, bool Commit);
		void* map(std::uintptr_t Offset, std::size_t Size, std::uint32_t Flags);
		void unmap(void* memory);
		void flush(std::uintptr_t Offset, std::size_t Size);
		void barrier();
		void copy(buffer const & Buffer, std::uintptr_t ReadOffset, std::uintptr_t WriteOffset, std::size_t Size);

	private:
		std::size_t const Size;
		std::uint32_t const Flags;
		handle Handle;
		std::vector<mapping> Mappings;
	};
*/
}//namespace gl
