#include "CAD/Vec3d.hh"
#include "CAD/BezierPath.hh"
#include "opengl/StyledMultiShape2D.hh"
#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"
#include <vector>

using namespace std;

class BezierTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    BezierTest() : GLWin(0x000000, 0xCCCCCC, "BezierTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    //first bezier curve
    Vec3D b1(3,0,0);
    Vec3D b2(6,7,0);
    Vec3D b3(9,0,0);
    Vec3D b4(12,7,0);
    BezierPath* firstBezier = new BezierPath(b1,b2,b3,b4,c,baseGraphStyle);
    c->addLayer(firstBezier);

    //second bezier curve
    Vec3D b5(-5,4,0);
    Vec3D b6(-2,7,0);
    Vec3D b7(0,4,0);
    Vec3D b8(2,7,0);
    BezierPath* secondBezier = new BezierPath(b5,b6,b7,b8,c,baseGraphStyle);
    c->addLayer(secondBezier);

    //third bezier curve
    Vec3D b9(-3,-10,0);
    Vec3D b10(0,-3,0);
    Vec3D b11(3,-10,0);
    Vec3D b12(6,-3,0);
    BezierPath* thirdBezier = new BezierPath(b9,b10,b11,b12,c,baseGraphStyle);
    c->addLayer(thirdBezier);

  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new BezierTest(), 1000, 1000);
}
