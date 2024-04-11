#ifndef OBJECTCLASS_HPP
#define OBJECTCLASS_HPP
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "FileFunctions.hpp"
#include <string>
#include "VectorMath.hpp"

using std::cout;
using std::string;
using std::vector;

class Object
{
   public:
   vec3 pos = vec3(0, 0, 0); 
   vec3 scale = vec3(1, 1, 1);
   vec3 rotation = vec3(0, 0, 0);

   vector<vec3> vertices;
   vector<vec3> localVertexPositions; 
   vector<int> indeces;
   vector<vec3> normals; 
   vector<int> normalIndeces;

   Object(vec3 _pos, vec3 _scale, vec3 _rotation, string _filePath) {
      fileUtil::extraction::obj(_filePath, localVertexPositions, indeces, normals, normalIndeces);
      vertices = localVertexPositions;

      transform(_pos, _scale, _rotation);         
   }

   void transform(vec3 _pos, vec3 _scale, vec3 _rotation) {

      for(int j = 0; j < normals.size(); j++)
      {
         normals[j].rotate(_rotation, vec3(0, 0, 0));
      }
      for(int i = 0; i < localVertexPositions.size(); i++)
      {
         vec3& currLocalVertexPos = localVertexPositions[i];

         pos = pos + _pos;

         currLocalVertexPos =  currLocalVertexPos * _scale;

         currLocalVertexPos.rotate(_rotation, vec3(0, 0, 0));

         vertices[i] = pos + currLocalVertexPos;

      }
   }

   void displayObjectData() {
      cout << "\n-----------------------------------------------\n";
      cout << "Vertices: \n";
      for(int i = 0; i < localVertexPositions.size(); i++) {
         cout << "v ";
         localVertexPositions[i].display();
      }
      cout << "Vertex Indeces: \n";
      for(int i = 0; i < indeces.size(); i+=3) {
         cout << "vi " << indeces[i] << " " << indeces[i+1] << " " << indeces[i+2] << "\n";

      }
      cout << "Normals: \n";
      for(int i = 0; i < normals.size(); i++) {
         cout << "vn "; 
         normals[i].display();
      }
      cout << "Normals Indeces: \n";
      for(int i = 0; i < normalIndeces.size(); i+=3) {
         cout << "ni " << normalIndeces[i] << " " << normalIndeces[i+1] << " " << normalIndeces[i+2] << "\n";
      } 
      cout << "\n-----------------------------------------------\n";
   }
};

#endif