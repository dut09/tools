#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

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
  cout << "Camera position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")" << endl;
  cout << "LootAt position: (" << lookAtPos.x << ", " << lookAtPos.y << ", " << lookAtPos.z << ")" << endl;
  cout << "Up vector: (" << upVec.x << ", " << upVec.y << ", " << upVec.z << ")" << endl;
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

  // Checkout the arguments are parsed correctly.
  printArguments(objFileName, merlFolder, cameraPos, lookAtPos, upVec, pbrtScriptFileName);
  return 0;
}
