#include "VectorMath.hpp"

// 2D VECTOR
vec2::vec2(float _x, float _y) : x(_x), y(_y) {}

vec2 vec2::operator+(const vec2& other) {
   return vec2(x + other.x, y + other.y);
}
vec2 vec2::operator-(const vec2& other) {
   return vec2(x - other.x, y - other.y);
}
vec2 vec2::operator*(const float other) {
   return vec2(x * other, y * other);
}
vec2 vec2::operator/(const float other) {
   return vec2(x / other, y / other);
}
void vec2::rotate(vec2 deltaRadians, vec2 origin) {
   vec2 relativePos = vec2(x, y) - origin;
   float r = mag(vec2(relativePos.x, relativePos.y));
   float theta = std::atan2(relativePos.y, relativePos.x);
   x = r * cos(theta + deltaRadians.x) + origin.x;
   y = r * sin(theta + deltaRadians.x) + origin.y;
}
void vec2::normalize() {
   float magnitude = mag(vec2(x, y));
   x /= magnitude;
   y /= magnitude;
}
void vec2::display() {
   cout << "(" << x << ", " << y << ")\n";
}


// 3D VECTOR
vec3::vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

vec3 vec3::operator+(const vec3& other) {
   return vec3(x + other.x, y + other.y, z + other.z);
}
vec3 vec3::operator-(const vec3& other) {
   return vec3(x - other.x, y - other.y, z - other.z);
}
void vec3::rotate(vec3 deltaRadians, vec3 origin) {
   // About X axis
   vec3 relativePos = vec3(x, y, z) - origin;
   float rx = mag(vec2(relativePos.z, relativePos.y));
   float thetax = std::atan2(relativePos.y, relativePos.z);
   z = rx * cos(thetax + deltaRadians.x) + origin.z;
   y = rx * sin(thetax + deltaRadians.x) + origin.y;
   // About Y axis
   relativePos = vec3(x, y, z) - origin;
   float ry = mag(vec2(relativePos.x, relativePos.z));
   float thetay = std::atan2(relativePos.z, relativePos.x);
   x = ry * cos(thetay + deltaRadians.y) + origin.x;
   z = ry * sin(thetay + deltaRadians.y) + origin.z;
   // About Z axis
   relativePos = vec3(x, y, z) - origin;
   float rz = mag(vec2(relativePos.x, relativePos.y));
   float thetaz = std::atan2(relativePos.y, relativePos.x);
   x = rz * cos(thetaz + deltaRadians.z) + origin.x;
   y = rz * sin(thetaz + deltaRadians.z) + origin.y;
}
vec3 vec3::operator*(const vec3& other) {
   return vec3(x * other.x, y * other.y, z * other.z);
}
void vec3::normalize() {
   float magnitude = mag(vec3(x, y, z));
   x /= magnitude;
   y /= magnitude;
   z /= magnitude;
}
void vec3::display() {
   cout << "(" << x << ", " << y << ", " << z << ")\n";
}


// General Vector Math
float sqr(float num) {
   return num * num;
}
float mag(vec3 vec) {
   return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);  
}
float mag(vec2 vec) {
   return sqrt(vec.x * vec.x + vec.y * vec.y);
}
float dot(vec3 v1, vec3 v2) {
   // I dont think this is mathematically correct way 
   float perpDist = sqrt(sqr(v1.x) + sqr(v1.y) + sqr(v1.z) + sqr(v2.x) + sqr(v2.y) + sqr(v2.z));
   float currDist = mag(v1-v2);
   return perpDist - currDist;
}
float dot(vec2 v1, vec2 v2) {
   // I dont think this is mathematically correct way 
   float perpDist = sqrt(sqr(v1.x) + sqr(v1.y) + sqr(v2.x) + sqr(v2.y));
   float currDist = mag(v1-v2);
   return perpDist - currDist;
}

vec3 midpoint(vec3& v1, vec3& v2, vec3& v3) {
   return vec3((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3, (v1.z + v2.z + v3.z) / 3);
}