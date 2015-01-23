#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// 100 BRDF names.
#define MATERIAL_NUM 100
const string materials[MATERIAL_NUM] = {
  "alum-bronze",
  "alumina-oxide",
  "aluminium",
  "aventurnine",
  "beige-fabric",
  "black-fabric",
  "black-obsidian",
  "black-oxidized-steel",
  "black-phenolic",
  "black-soft-plastic",
  "blue-acrylic",
  "blue-fabric",
  "blue-metallic-paint",
  "blue-metallic-paint2",
  "blue-rubber",
  "brass",
  "cherry-235",
  "chrome-steel",
  "chrome",
  "colonial-maple-223",
  "color-changing-paint1",
  "color-changing-paint2",
  "color-changing-paint3",
  "dark-blue-paint",
  "dark-red-paint",
  "dark-specular-fabric",
  "delrin",
  "fruitwood-241",
  "gold-metallic-paint",
  "gold-metallic-paint2",
  "gold-metallic-paint3",
  "gold-paint",
  "gray-plastic",
  "grease-covered-steel",
  "green-acrylic",
  "green-fabric",
  "green-latex",
  "green-metallic-paint",
  "green-metallic-paint2",
  "green-plastic",
  "hematite",
  "ipswich-pine-221",
  "light-brown-fabric",
  "light-red-paint",
  "maroon-plastic",
  "natural-209",
  "neoprene-rubber",
  "nickel",
  "nylon",
  "orange-paint",
  "pearl-paint",
  "pickled-oak-260",
  "pink-fabric",
  "pink-fabric2",
  "pink-felt",
  "pink-jasper",
  "pink-plastic",
  "polyethylene",
  "polyurethane-foam",
  "pure-rubber",
  "purple-paint",
  "pvc",
  "red-fabric",
  "red-fabric2",
  "red-metallic-paint",
  "red-phenolic",
  "red-plastic",
  "red-specular-plastic",
  "silicon-nitrade",
  "silver-metallic-paint",
  "silver-metallic-paint2",
  "silver-paint",
  "special-walnut-224",
  "specular-black-phenolic",
  "specular-blue-phenolic",
  "specular-green-phenolic",
  "specular-maroon-phenolic",
  "specular-orange-phenolic",
  "specular-red-phenolic",
  "specular-violet-phenolic",
  "specular-white-phenolic",
  "specular-yellow-phenolic",
  "ss440",
  "steel",
  "teflon",
  "tungsten-carbide",
  "two-layer-gold",
  "two-layer-silver",
  "violet-acrylic",
  "violet-rubber",
  "white-acrylic",
  "white-diffuse-bball",
  "white-fabric",
  "white-fabric2",
  "white-marble",
  "white-paint",
  "yellow-matte-plastic",
  "yellow-paint",
  "yellow-phenolic",
  "yellow-plastic",
};

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

  // Lighting.

  // Material.
  pbrtScriptFile << endl;
  pbrtScriptFile << "AttributeBegin" << endl;
  // Randomly pick a material.
  srand(time(NULL));
  string materialName = materials[rand() % MATERIAL_NUM];
  pbrtScriptFile << "Material \"measured\" \"string filename\" \""
                 << merlFolder << "/" << materialName << ".binary\"" << endl;
  // Geometry.

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
