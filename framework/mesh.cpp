#include "mesh.hpp"
#include "test.hpp"
#include <glm/geometric.hpp>

namespace
{
	void subdivise(std::vector<glm::vec3>& VertexData, std::size_t& VertexIndex, std::vector<GLushort>& ElementData, std::size_t I0, std::size_t J0, std::size_t K0, int Subdivise)
	{
		if(Subdivise == 0)
		{
			ElementData.push_back(I0);
			ElementData.push_back(J0);
			ElementData.push_back(K0);
		}
		else
		{
			if(VertexIndex + 3 >= VertexData.size())
				VertexData.resize(VertexData.size() * 2);

			glm::vec3 const& A0 = VertexData[I0];
			glm::vec3 const& B0 = VertexData[J0];
			glm::vec3 const& C0 = VertexData[K0];

			std::size_t const I1 = VertexIndex++;
			std::size_t const J1 = VertexIndex++;
			std::size_t const K1 = VertexIndex++;

			glm::vec3 U = (B0 + C0) * 0.5f;
			glm::vec3 V = (C0 + A0) * 0.5f;
			glm::vec3 W = (A0 + B0) * 0.5f;

			if(glm::length(U) > 0.0f)
				U = glm::normalize(U);
			if(glm::length(V) > 0.0f)
				V = glm::normalize(V);
			if(glm::length(W) > 0.0f)
				W = glm::normalize(W);

			VertexData[I1] = U;
			VertexData[J1] = V;
			VertexData[K1] = W;

			subdivise(VertexData, VertexIndex, ElementData, I0, J1, K1, Subdivise - 1);
			subdivise(VertexData, VertexIndex, ElementData, J0, K1, I1, Subdivise - 1);
			subdivise(VertexData, VertexIndex, ElementData, K0, I1, J1, Subdivise - 1);
			subdivise(VertexData, VertexIndex, ElementData, J1, I1, K1, Subdivise - 1);
		}
	}

	//http://www.3dgep.com/forward-plus/#Forward
	//http://blog.coredumping.com/subdivision-of-icosahedrons/
	//http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	void generate_sphere(std::vector<glm::vec3>& VertexData, std::vector<GLushort>& ElementData, int Subdivision)
	{
		VertexData[0] = glm::normalize(glm::vec3(+1.0f, 0.0f,-0.70710678118f));
		VertexData[1] = glm::normalize(glm::vec3(-1.0f, 0.0f,-0.70710678118f));
		VertexData[2] = glm::normalize(glm::vec3(+0.0f,+1.0f,+0.70710678118f));
		VertexData[3] = glm::normalize(glm::vec3(+0.0f,-1.0f,+0.70710678118f));

		std::size_t VertexIndex = 4;
		subdivise(VertexData, VertexIndex, ElementData, 0, 2, 3, Subdivision);
		subdivise(VertexData, VertexIndex, ElementData, 0, 3, 1, Subdivision);
		subdivise(VertexData, VertexIndex, ElementData, 1, 3, 2, Subdivision);
		subdivise(VertexData, VertexIndex, ElementData, 1, 2, 0, Subdivision);
	}

	void subdivise_icosahedron(std::vector<glm::vec3>& VertexData, glm::vec3 const& A0, glm::vec3 const& B0, glm::vec3 const& C0, int Subdivise)
	{
		if(Subdivise == 0)
		{
			VertexData.push_back(A0);
			VertexData.push_back(B0);
			VertexData.push_back(C0);
		}
		else
		{
			glm::vec3 A1 = (B0 + C0) * 0.5f;
			glm::vec3 B1 = (C0 + A0) * 0.5f;
			glm::vec3 C1 = (A0 + B0) * 0.5f;

			if(glm::length(A1) > 0.0f)
				A1 = glm::normalize(A1);
			if(glm::length(B1) > 0.0f)
				B1 = glm::normalize(B1);
			if(glm::length(C1) > 0.0f)
				C1 = glm::normalize(C1);

			subdivise_icosahedron(VertexData, A0, B1, C1, Subdivise - 1);
			subdivise_icosahedron(VertexData, B0, C1, A1, Subdivise - 1);
			subdivise_icosahedron(VertexData, C0, A1, B1, Subdivise - 1);
			subdivise_icosahedron(VertexData, B1, A1, C1, Subdivise - 1);
		}
	}
}//namespace

namespace glf
{
	void generate_icosahedron(std::vector<glm::vec3>& VertexData, int Subdivision)
	{
/*
		glm::vec3 const& A = glm::normalize(glm::vec3(+1.0f, 0.0f,-0.70710678118f));
		glm::vec3 const& B = glm::normalize(glm::vec3(-1.0f, 0.0f,-0.70710678118f));
		glm::vec3 const& C = glm::normalize(glm::vec3(+0.0f,+1.0f,+0.70710678118f));
		glm::vec3 const& D = glm::normalize(glm::vec3(+0.0f,-1.0f,+0.70710678118f));

		subdivise_array(VertexData, A, C, D, Subdivision);
		subdivise_array(VertexData, A, D, B, Subdivision);
		subdivise_array(VertexData, B, D, C, Subdivision);
		subdivise_array(VertexData, B, C, A, Subdivision);
*/

		//The golden ratio
		float t = (1 +sqrt(5))/ 2;
		float size = 1.0f;

		glm::vec3 const A = glm::normalize(glm::vec3(-size, t * size, 0.0f));	// 0
		glm::vec3 const B = glm::normalize(glm::vec3(+size, t * size, 0.0f));	// 1
		glm::vec3 const C = glm::normalize(glm::vec3(-size,-t * size, 0.0f));	// 2
		glm::vec3 const D = glm::normalize(glm::vec3(+size,-t * size, 0.0f));	// 3

		glm::vec3 const E = glm::normalize(glm::vec3(0.0f,-size, t * size));	// 4
		glm::vec3 const F = glm::normalize(glm::vec3(0.0f, size, t * size));	// 5
		glm::vec3 const G = glm::normalize(glm::vec3(0.0f,-size,-t * size));	// 6
		glm::vec3 const H = glm::normalize(glm::vec3(0.0f, size,-t * size));	// 7

		glm::vec3 const I = glm::normalize(glm::vec3( t * size, 0.0f,-size));	// 8
		glm::vec3 const J = glm::normalize(glm::vec3( t * size, 0.0f, size));	// 9
		glm::vec3 const K = glm::normalize(glm::vec3(-t * size, 0.0f,-size));	// 10
		glm::vec3 const L = glm::normalize(glm::vec3(-t * size, 0.0f, size));	// 11

		subdivise_icosahedron(VertexData, A, L, F, Subdivision);
		subdivise_icosahedron(VertexData, A, F, B, Subdivision);
		subdivise_icosahedron(VertexData, A, B, H, Subdivision);
		subdivise_icosahedron(VertexData, A, H, K, Subdivision);
		subdivise_icosahedron(VertexData, A, K, L, Subdivision);

		subdivise_icosahedron(VertexData, B, F, J, Subdivision);
		subdivise_icosahedron(VertexData, F, L, E, Subdivision);
		subdivise_icosahedron(VertexData, L, K, C, Subdivision);
		subdivise_icosahedron(VertexData, K, H, G, Subdivision);
		subdivise_icosahedron(VertexData, H, B, I, Subdivision);

		subdivise_icosahedron(VertexData, D, J, E, Subdivision);
		subdivise_icosahedron(VertexData, D, E, C, Subdivision);
		subdivise_icosahedron(VertexData, D, C, G, Subdivision);
		subdivise_icosahedron(VertexData, D, G, I, Subdivision);
		subdivise_icosahedron(VertexData, D, I, J, Subdivision);

		subdivise_icosahedron(VertexData, E, J, F, Subdivision);
		subdivise_icosahedron(VertexData, C, E, L, Subdivision);
		subdivise_icosahedron(VertexData, G, C, K, Subdivision);
		subdivise_icosahedron(VertexData, I, G, H, Subdivision);
		subdivise_icosahedron(VertexData, J, I, B, Subdivision);
	}
}//namespace glf
