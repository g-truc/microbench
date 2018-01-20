#ifndef GLU_INCLUDED
#define GLU_INCLUDED

#include "texture2d.hpp"
#include "caps.hpp"

namespace glu
{

}//namespace glu

#endif//GLU_INCLUDED
/*
namespace glu 
{
	class sphere
	{
	public:
		sphere();
		sphere(glm::vec3 const & Position, float const & Radius);

		glm::vec3 position() const;
		glm::vec3 radius() const;

	private:
		glm::vec3 Position;
		float Radius;
	};

	class ray
	{
	public:
		ray();
		ray(glm::vec3 const & Position, glm::vec3 const & Direction);

	private:
		glm::vec3 Position;
		glm::vec3 Direction; // Direction is always normalized
	};

	class plane
	{
	public:
		plane(glm::vec3 const & Normal, float const & Distance);

	private:
		glm::vec3 Normal;
		float Distance;
	};

	bool operator == (plane const & p0, plane const & p1) const		{ return p0.normal == p1.normal && p0.distance == p1.distance; }
	bool operator != (plane const & p0, plane const & p1) const		{ return p0.normal != p1.normal || p0.distance != p1.distance; }

	float distance(plane const & Plane, glm::vec3 const & Point);
	float distance(glm::vec3 const & Point, plane const & Plane);

}//namespace glu
*/
