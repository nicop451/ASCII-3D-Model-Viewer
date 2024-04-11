#ifndef FILEREADING_HPP
#define FILEREADING_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "VectorMath.hpp"

using std::vector;
using std::string;
using std::cerr;
using std::fstream;
using std::istream;


namespace fileUtil {

namespace extraction
{

void obj(string directory, vector<vec3>& vertices, vector<int>& indeces, vector<vec3>& normals, vector<int>& normal_indeces)
{
   // Open File
   fstream file;
   file.open(directory, std::ios::in);
   // Check if file is open
   if(!file.is_open()) {
      cerr << "\nError: couldn't open file " << directory << "\n";
      std::exit(EXIT_FAILURE);
   } 
   // Read File
   string line;
   unsigned int lineNum = 0;
   while(std::getline(file, line)) {
      lineNum++;
      // converts line into string stream
      std::istringstream lineStream(line); 
      // Extract prefix
      string prefix;
      lineStream >> prefix;
      // Check for each prefix
      if(prefix == "v") {
         float vertX, vertY, vertZ;
         lineStream >> vertX >> vertY >> vertZ;
         vertices.push_back(vec3(vertX, vertY, vertZ));
      }
      else if(prefix == "vn") {
         float normalX, normalY, normalZ;
         lineStream >> normalX >> normalY >> normalZ;
         vec3 normal = vec3(normalX, normalY, normalZ);
         normal.display();
         normals.push_back(normal);
      }
      else if(prefix == "f") {
         // Extract Index Substrings
         string vertInfo1, vertInfo2, vertInfo3, tempForQuadCheck;
         int vertInd1, vertInd2, vertInd3, normInd1, normInd2, normInd3;
         lineStream >> vertInfo1 >> vertInfo2 >> vertInfo3;

         if(sscanf(vertInfo1.c_str(), "%i//%i", &vertInd1, &normInd1) == 2) {
            indeces.push_back(vertInd1 - 1);
            normal_indeces.push_back(normInd1 - 1);
         }
         if(sscanf(vertInfo2.c_str(), "%i//%i", &vertInd2, &normInd2) == 2) {
            indeces.push_back(vertInd2 - 1);
            normal_indeces.push_back(normInd2 - 1);
         }
         if(sscanf(vertInfo3.c_str(), "%i//%i", &vertInd3, &normInd3) == 2) {
            indeces.push_back(vertInd3 - 1);
            normal_indeces.push_back(normInd3 - 1);
         }

         // Add indeces to array

         // indeces.push_back(atoi(&vertInfo2[0]) - 1);
         // indeces.push_back(atoi(&vertInfo3[0]) - 1);
         // normal_indeces.push_back(atoi(&vertInfo1[3]) - 1);
         // normal_indeces.push_back(atoi(&vertInfo2[3]) - 1);
         // normal_indeces.push_back(atoi(&vertInfo3[3]) - 1);
         // Check If Anything Else Can Be Extracted
         if(lineStream >> tempForQuadCheck) {
            cerr << "\n\nError: Quads Present in: " << directory << " at line " << lineNum <<  "\n\n";
            std::exit(EXIT_FAILURE);
         }
      }
   }
}
}
}

#endif