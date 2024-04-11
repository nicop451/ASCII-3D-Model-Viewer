#ifndef VECTORMATH_HPP
#define VECTORMATH_HPP

#include <iostream>
#include <cmath>

using std::cout;

struct vec2 {
   float x, y;

   vec2(float _x, float _y);

   vec2 operator+(const vec2& other);
   vec2 operator-(const vec2& other);
   vec2 operator*(const float other);
   vec2 operator/(const float other);
   void normalize();
   void rotate(vec2 deltaRadians, vec2 origin);
   void display();
};
struct vec3 {
   float x, y, z;

   vec3(float _x, float _y, float _z);

   vec3 operator+(const vec3& other);
   vec3 operator-(const vec3& other);
   vec3 operator*(const vec3& other);
   void normalize();
   void rotate(vec3 deltaRadians, vec3 origin);
   void display();
};

float sqr(float num);
float mag(vec3 vec);
float mag(vec2 vec);
float dot(vec3 v1, vec3 v2);
float dot(vec2 v1, vec2 v2);
vec3 midpoint(vec3& v1, vec3& v2, vec3& v3);

#endif