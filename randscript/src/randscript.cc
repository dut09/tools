#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// 100 BRDF names.
#include "materials.h"

struct Vector3D {
  Vector3D() : x(0.0), y(0.0), z(0.0) {}
  Vector3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
  double x, y, z;
};

void printArguments(const string objFileName, const string merlFolder,
                    const Vector3D& cameraPos, const Vector3D& lookAtPos,
                    const Vector3D& upVec, const string pbrtScriptFileName) {
  cout << "Obj file name: " << objFileName << endl;
  cout << "MERL folder: " << merlFolder << endl;
  cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ", "
       << cameraPos.z << ")" << endl;
  cout << "LootAt position: (" << lookAtPos.x << ", " << lookAtPos.y << ", "
       << lookAtPos.z << ")" << endl;
  cout << "Up vector: (" << upVec.x << ", " << upVec.y << ", " << upVec.z
       << ")" << endl;
  cout << "PBRT script file name: " << pbrtScriptFileName << endl;
}

int main(int argc, char* argv[]) {
  // Parse the arguments.
  int curArg = 1;
  // argv[0]: randscript.
  // argv[1]: the .obj filename.
  string objFileName(argv[curArg++]);

  // argv[2]: MERL folder.
  string merlFolder(argv[curArg++]);

  // argv[3, 4, 5]: camera position.
  Vector3D cameraPos;
  cameraPos.x = atof(argv[curArg++]);
  cameraPos.y = atof(argv[curArg++]);
  cameraPos.z = atof(argv[curArg++]);

  // Optional: lookAt position, up vector and pbrt script filename.
  Vector3D lookAtPos(0.0, 0.0, 0.0);
  Vector3D upVec(0.0, 0.0, 1.0);
  string pbrtScriptFileName = "randscript.pbrt";
  if (argc % 3 != 0) {
    // The last argument is script file name.
    pbrtScriptFileName = string(argv[argc - 1]);
  }
  if (argc >= 9) {
    lookAtPos.x = atof(argv[curArg++]);
    lookAtPos.y = atof(argv[curArg++]);
    lookAtPos.z = atof(argv[curArg++]);
  }
  if (argc >= 12) {
    upVec.x = atof(argv[curArg++]);
    upVec.y = atof(argv[curArg++]);
    upVec.z = atof(argv[curArg++]);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Open up the new script file.
  /////////////////////////////////////////////////////////////////////////////
  ofstream pbrtScriptFile;
  pbrtScriptFile.open(pbrtScriptFileName.c_str());

  // Film.
  pbrtScriptFile << "Film \"image\"" << endl;

  // Camera position.
  pbrtScriptFile << "LookAt " << cameraPos.x << " " << cameraPos.y << " "
                 << cameraPos.z << " " << lookAtPos.x << " " << lookAtPos.y
                 << " " << lookAtPos.z << " " << upVec.x << " " << upVec.y
                 << " " << upVec.z << endl;

  // Camera.
  pbrtScriptFile << "Camera \"perspective\" \"float fov\" [60]" << endl;

  /////////////////////////////////////////////////////////////////////////////
  // Begin the scene.
  /////////////////////////////////////////////////////////////////////////////
  pbrtScriptFile << endl;
  pbrtScriptFile << "WorldBegin" << endl << endl;

  // Initialize random seed.
  srand(time(NULL));

  // Lighting section.

  // Material section.
  pbrtScriptFile << endl;
  pbrtScriptFile << "AttributeBegin" << endl;

  // Randomly pick a material.
  string materialName = materials[rand() % MATERIAL_NUM];
  pbrtScriptFile << "Material \"measured\" \"string filename\" \""
                 << merlFolder << "/" << materialName << ".binary\"" << endl;

  // Geometry section.
  pbrtScriptFile << endl;

  // Open up obj file.
  // Assume the obj file format is:
  // v v1x v1y v1z
  // v v2x v2y v2z
  // ...
  // f f1i f1j f1k
  // f f2i f2j f2k
  // ...
  ifstream objFile;
  objFile.open(objFileName.c_str());
  pbrtScriptFile << "Shape \"trianglemesh\"" << endl;
  pbrtScriptFile << "\"point P\" [" << endl;

  // Read points from obj file.
  char ch;
  Vector3D point;
  objFile >> ch;
  while (ch == 'v') {
    objFile >> point.x >> point.y >> point.z;
    pbrtScriptFile << "  " << point.x << " " << point.y << " "
                   << point.z << endl;
    objFile >> ch;
  }
  pbrtScriptFile << "]" << endl;

  // Read faces.
  pbrtScriptFile << "\"integer indices\" [" << endl;
  while (!objFile.eof() && ch == 'f') {
    objFile >> point.x >> point.y >> point.z;
    pbrtScriptFile << "  " << (int)(point.x - 1) << " " << (int)(point.y - 1)
                   << " " << (int)(point.z - 1) << endl;
    objFile >> ch;
  }
  pbrtScriptFile << "]" << endl;
  objFile.close();
  pbrtScriptFile << "AttributeEnd" << endl;

  /////////////////////////////////////////////////////////////////////////////
  // End the scene.
  /////////////////////////////////////////////////////////////////////////////
  pbrtScriptFile << endl;
  pbrtScriptFile << "WorldEnd" << endl;

  /////////////////////////////////////////////////////////////////////////////
  // Close the script.
  /////////////////////////////////////////////////////////////////////////////
  pbrtScriptFile.close();

  return 0;
}
