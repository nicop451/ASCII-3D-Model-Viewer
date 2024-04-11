#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include "VectorMath.hpp"
#include "ObjectClass.hpp"
#include "Objects.hpp"
using std::cout;
using std::vector;

const float PI = 3.141592;
const int FRAMES = 1000;
const float FRAME_DELAY = 1;

char brightLevels[] = {'.',',','"','=','t','#','@'};

// got this from the internet
void clear_screen() {
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD coordScreen = { 0, 0 };
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   DWORD dwConSize;

   GetConsoleScreenBufferInfo(hConsole, &csbi);
   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

   FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
   GetConsoleScreenBufferInfo(hConsole, &csbi);
   FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
   SetConsoleCursorPosition(hConsole, coordScreen);
}

struct AsciiRenderer {
   int rows, columns, arraySize; // Grid Diemensions

   char clearChar = ' ';
   char faceChar = ' ';
   char drawChar = '#';
   char* pixelArray;

   
   
   // Projection Settings
   float VIEW_PLANE_DIST = 160; // Distance from focal point the plane where vertices are projected
   vec2 VIEW_ANGLE = vec2(PI / 4, PI / 4); // Angle above horizontal for viewing frustum angle
   vec2 VIEW_PLANE_SCALE = vec2(VIEW_PLANE_DIST * tan(VIEW_ANGLE.x), VIEW_PLANE_DIST * tan(VIEW_ANGLE.y));
   vec3 CAMERA_NORMAL = vec3(0, 0, 1);
   vec3 CAMERA_POS = vec3(0, 0, 0);
   vec3 LIGHT_DIR = vec3(0.1, -1, 0);


   AsciiRenderer(int _rows, int _columns) : rows(_rows), columns(_columns) {
      arraySize = rows * columns;
      pixelArray = new char[arraySize];
      this->clear();
   }

   void set(int x, int y, char c) {
      x += columns/2;
      y += rows/2;

      if(x > columns - 1 || y > rows - 1 || x < 0 || y < 0) {
         return;
      }

      pixelArray[(y * columns) + x] = c;
   }

   void line(float x1, float y1, float x2, float y2, char c) {

      float deltaX = x2 - x1;
      float deltaY = y2 - y1;
      
      if(deltaX == 0) { // To Avoid Undefined behavior
         deltaX = 0.00001;
      }

      float slope = deltaY / deltaX;
      float dist = mag(vec2(deltaX, deltaY));
      float stepSize = 1;
      float amountofSteps = dist / stepSize;

      float xStep = deltaX * (stepSize/amountofSteps);

      for(int i = 0; i < amountofSteps; i++) {
         float x = xStep * i;
         float y = x * slope;
         set(x1 + x, y1 + y, c);
      }
      // set(x1, y1, drawChar);
      // set(x2, y2, drawChar);  
   }

   void RenderVertices(vector<vec3>& vertices, vector<int>& indeces, vector<vec3>& normals, vector<int>& normalIndeces) {
      // Project Vertices
      vector<vec2> projectedVertexArray;
      for(int i = 0; i < vertices.size(); i++) {
         // Project
         vec3 vert = vertices[i];
         vec2 ratio = vec2(vert.x/vert.z, vert.y/vert.z);
         vec2 ratioAtVP = vec2(ratio.x * VIEW_PLANE_DIST, ratio.y * VIEW_PLANE_DIST);
         projectedVertexArray.push_back(ratioAtVP);
      }
      // Connect 2d vertices with lines according to index array
      for(int i = 0; i < indeces.size(); i += 3) {
         vec2 vert1 = projectedVertexArray[indeces[i]];
         vec2 vert2 = projectedVertexArray[indeces[i + 1]];
         vec2 vert3 = projectedVertexArray[indeces[i + 2]];

         vec3 vertex1 = vertices[indeces[i]];
         vec3 vertex2 = vertices[indeces[i + 1]];
         vec3 vertex3 = vertices[indeces[i + 2]];


         
         



         vec3 relaCameraNormal = CAMERA_POS - midpoint(vertex1, vertex2, vertex3);
         relaCameraNormal.normalize();
         // relaCameraNormal.display();


         vec3 currNormal = normals[normalIndeces[i]];
         bool isFacing = dot(relaCameraNormal, currNormal) > 0;
         if(!isFacing) {
            continue;// Dont Draw This Triangle
         }                  


         float lightDot = dot(LIGHT_DIR, currNormal) * 4;
         int brightIndex = lightDot + 2;
         char brightCharacter = brightLevels[brightIndex];



         // Calculate steps across shortest line
         float stepSize = 0.5;
         
         vec2 smallLineVector = vert2 - vert1;
         float smallLineDist = mag(smallLineVector);
         float stepRatio_S = stepSize / smallLineDist;
         vec2 stepVector_S = smallLineVector * stepRatio_S;

         int amountOfSteps = smallLineDist / stepSize;

         vec2 otherLineVector = vert3 - vert1;
         float otherLineDist = mag(otherLineVector);
         float ratio_O = (otherLineDist / amountOfSteps) / otherLineDist;
         vec2 stepVector_O = otherLineVector * ratio_O;


         for(int n = 0; n < amountOfSteps; n++) {
            vec2 point1 = (stepVector_S * n) + vert1;
            vec2 point2 = (stepVector_O * n) + vert1;
            line(point1.x, point1.y, point2.x, point2.y, brightCharacter);
         }
         cout << "\033[97m";
         // line(vert1.x, vert1.y, vert2.x, vert2.y, drawChar);
         // line(vert2.x, vert2.y, vert3.x, vert3.y, drawChar);
         // line(vert3.x, vert3.y, vert1.x, vert1.y, drawChar);

      }
   }

   void display() {
      // Organize Array into string for faster rendering
      std::string outputString;
      for(int i = 0; i < arraySize; i++) {
         outputString.push_back(pixelArray[i]);
         outputString.push_back(' ');

         if(i % columns == 0) {
            outputString += "\n";
         }
      }
      // Print Pixels
      cout << outputString;
      cout << "\n" << std::flush;
   }

   void clear() {
      for(int i = 0; i < arraySize; i++) {
         pixelArray[i] = clearChar;
      }
   }
};

int main() {
   AsciiRenderer* asciiRenderer = new AsciiRenderer(65 * 2, 100 * 2.3);

   Object* cube3 = new Object(vec3(0, 0, 150), vec3(40000, 40000, 40000), vec3(0, 0, PI), "ObjectFiles/Cube.obj");
   
   // Animation Loop
   float n = 0;
   for(int i = 0; i < FRAMES; i++) {
      // Grid Rendering -----------------
      asciiRenderer->clear();

      cube3->transform(vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0.06, 0));
      asciiRenderer->RenderVertices(cube3->vertices, cube3->indeces, cube3->normals, cube3->normalIndeces);

      // asciiRenderer->VIEW_PLANE_DIST -=  1;
   

      // Frame Delay and Then Clear Screen
      Sleep(FRAME_DELAY);
      clear_screen();
      asciiRenderer->display();
      n++;
      // --------------------------------
   }
   return 0;
}

