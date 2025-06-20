#include "RedEngine/Math/Math.hpp"
#include "RedEngine/Math/Matrix.hpp"
#include "RedEngine/Math/MatrixFunctions.hpp"
#include "RedEngine/Math/Vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/core.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace red;

TEST_CASE("Orthogonal_transformation", "[MATH]")
{
    SECTION("Red")
    {
        Matrix44 m = Math::Ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);

        Vector4 v0(0.f, 0.f, 0.f, 1.f);
        Vector4 v1(800.f, 600.f, 0.f, 1.f);

        Vector4 transformedv0 = m * v0;
        Vector4 transformedv1 = m * v1;

        REQUIRE(Math::EqualsEpsilon(transformedv0.x, -1.f, 0.01f));
        REQUIRE(Math::EqualsEpsilon(transformedv0.y, -1.f, 0.01f));

        REQUIRE(Math::EqualsEpsilon(transformedv1.x, 1.f, 0.01f));
        REQUIRE(Math::EqualsEpsilon(transformedv1.y, 1.f, 0.01f));
    }

    SECTION("GLM")
    {
        glm::mat4 glmOrtho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);

        glm::vec4 v0(0.f, 0.f, 0.f, 1.f);
        glm::vec4 v1(800.f, 600.f, 0.f, 1.f);

        glm::vec4 transformedv0 = glmOrtho * v0;
        glm::vec4 transformedv1 = glmOrtho * v1;

        REQUIRE(Math::EqualsEpsilon(transformedv0.x, -1.f, 0.01f));
        REQUIRE(Math::EqualsEpsilon(transformedv0.y, -1.f, 0.01f));

        REQUIRE(Math::EqualsEpsilon(transformedv1.x, 1.f, 0.01f));
        REQUIRE(Math::EqualsEpsilon(transformedv1.y, 1.f, 0.01f));
    }
}

void ReqEquals(float r, float glm)
{
    REQUIRE(Math::EqualsEpsilon(r, glm, 0.01f));
}

void ReqEqualsPtr(float const* r, float const* glm, int size)
{
    for (int i = 0; i < size; i++)
    {
        REQUIRE(Math::EqualsEpsilon(r[i], glm[i], 0.01f));
    }
}

void GlmEquals(const Matrix44& r, const glm::mat4& g)
{
    ReqEquals(r(0, 0), g[0][0]);
    ReqEquals(r(0, 1), g[0][1]);
    ReqEquals(r(0, 2), g[0][2]);
    ReqEquals(r(0, 3), g[0][3]);
    ReqEquals(r(1, 0), g[1][0]);
    ReqEquals(r(1, 1), g[1][1]);
    ReqEquals(r(1, 2), g[1][2]);
    ReqEquals(r(1, 3), g[1][3]);
    ReqEquals(r(2, 0), g[2][0]);
    ReqEquals(r(2, 1), g[2][1]);
    ReqEquals(r(2, 2), g[2][2]);
    ReqEquals(r(2, 3), g[2][3]);
    ReqEquals(r(3, 0), g[3][0]);
    ReqEquals(r(3, 1), g[3][1]);
    ReqEquals(r(3, 2), g[3][2]);
    ReqEquals(r(3, 3), g[3][3]);

    ReqEqualsPtr(glm::value_ptr(g), r.m_data, 16);
}

void GlmEquals(const Matrix33& r, const glm::mat3& g)
{
    ReqEquals(r(0, 0), g[0][0]);
    ReqEquals(r(0, 1), g[0][1]);
    ReqEquals(r(0, 2), g[0][2]);

    ReqEquals(r(1, 0), g[1][0]);
    ReqEquals(r(1, 1), g[1][1]);
    ReqEquals(r(1, 2), g[1][2]);

    ReqEquals(r(2, 0), g[2][0]);
    ReqEquals(r(2, 1), g[2][1]);
    ReqEquals(r(2, 2), g[2][2]);

    ReqEqualsPtr(glm::value_ptr(g), r.m_data, 9);
}

TEST_CASE("Test against GLM", "[MATH]")
{
    SECTION("Matrix equals")
    {
        // clang-format off
        Matrix44 redMat = { 1.f,  5.f,  9.f,    13.f,   
                            2.f,  6.f,  10.f,   14.f,   
                            3.f,  7.f,  11.f,   15.f,   
                            4.f,  8.f,  12.f,   16.f  };
        // clang-format on

        glm::mat4 glmMat;
        glmMat[0] = glm::vec4(1.f, 5.f, 9.f, 13.f);
        glmMat[1] = glm::vec4(2.f, 6.f, 10.f, 14.f);
        glmMat[2] = glm::vec4(3.f, 7.f, 11.f, 15.f);
        glmMat[3] = glm::vec4(4.f, 8.f, 12.f, 16.f);

        GlmEquals(redMat, glmMat);
    }

    SECTION("Ortho")
    {
        Matrix44 redOrtho = Math::Ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);

        glm::mat4 glmOrtho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);

        GlmEquals(redOrtho, glmOrtho);
    }
}