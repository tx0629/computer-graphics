// SphereWorld.cpp
// OpenGL SuperBible
// Demonstrates an immersive 3D environment using actors
// and a camera. This version adds lights and material properties
// and shadows.
// Program by Richard S. Wright Jr.

#include <.\GL\freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define GL_MULTISAMPLE_ARB 0x809D
#define GL_CLAMP_TO_EDGE 0x812F
//#include "gltools.h"	// OpenGL toolkit
//#include "../../shared/math3d.h"    // 3D Math Library
//#include "glframe.h"   // Frame class
#include "iostream"
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#define M3D_PI (3.14159265358979323846)
typedef float	M3DVector3f[3];
typedef float	M3DVector4f[4];
typedef float M3DMatrix44f[16];
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3);
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos);
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
    const M3DVector3f point3);
inline float m3dGetVectorLengthSquared3(const M3DVector3f u);
inline void m3dScaleVector3(M3DVector3f v, const float scale);
inline void m3dNormalizeVector3(M3DVector3f u);
inline void m3dCrossProduct3(M3DVector3f result, const M3DVector3f u, const M3DVector3f v);
inline float m3dGetVectorLength3(const M3DVector3f u);
inline void	m3dCopyVector3(M3DVector3f dst, const M3DVector3f src);
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);
GLbyte* gltLoadTGA(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);
inline void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v);
inline void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, int column);
inline void m3dNormalizeVector(M3DVector3f u);
inline float m3dGetVectorLengthSquared(const M3DVector3f u);
inline float m3dGetVectorLength(const M3DVector3f u);
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
//void Drawball(GLint nShadow);
void myKeyboard(unsigned char key, int x, int y);
int timer_flag=1;
void drawrightarm();
void drawleftarm();
void drawleftleg();
void drawrightleg();
void drawbody();
void drawball();
void OpenFile(int obj);
void Colormode(int obj);
void loadFile(std::string fileName);
void Objfile(int );
void createGLUTMenus();
void menuSelect(int option);
void Menu(int option);
void drawtest(int nShadow);
void boundboxmode(int option);
void fillmode(int option);
void drawtako();
void Drawta(GLint nShadow);
float rot = 0;
float brot = 0;
std::vector<float> v; 
std::vector<float> nv;//normalize
std::vector<std::string> f;
std::string Mode = "";
float xMin=999 , xMax = -999, yMin =999, yMax = -999, zMin =999, zMax = -999,xC,yC,zC, xNC, yNC, zNC, xNMin, yNMin, zNMin;
float  xNMax, yNMax, zNMax;
float allmax=0, allmin=999;
std::string line,userinput;
int boundbox_flag = 0;
int fix_color = 1;
float R=0.5,G=0,B=0;
int fill = 3;
float rr, rg, rb;
float min, max, r, g, b, takorot=0;
#pragma pack(1)
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
class GLFrame
{
protected:
    M3DVector3f vOrigin;	// Where am I?
    M3DVector3f vForward;	// Where am I going?
    M3DVector3f vUp;		// Which way is up?

public:
    // Default position and orientation. At the origin, looking
    // down the positive Z axis (right handed coordinate system).
    GLFrame(void) {
        // At origin
        vOrigin[0] = 0.0f; vOrigin[1] = 0.0f; vOrigin[2] = 0.0f;

        // Up is up (+Y)
        vUp[0] = 0.0f; vUp[1] = 1.0f; vUp[2] = 0.0f;

        // Forward is -Z (default OpenGL)
        vForward[0] = 0.0f; vForward[1] = 0.0f; vForward[2] = -1.0f;
    }
    inline void SetOrigin(const M3DVector3f vPoint) {
        m3dCopyVector3(vOrigin, vPoint);
    }

    inline void SetOrigin(float x, float y, float z) {
        vOrigin[0] = x; vOrigin[1] = y; vOrigin[2] = z;
    }
    void GetMatrix(M3DMatrix44f	matrix, bool bRotationOnly = false)
    {
        // Calculate the right side (x) vector, drop it right into the matrix
        M3DVector3f vXAxis;
        m3dCrossProduct(vXAxis, vUp, vForward);

        // Set matrix column does not fill in the fourth value...
        m3dSetMatrixColumn44(matrix, vXAxis, 0);
        matrix[3] = 0.0f;

        // Y Column
        m3dSetMatrixColumn44(matrix, vUp, 1);
        matrix[7] = 0.0f;

        // Z Column
        m3dSetMatrixColumn44(matrix, vForward, 2);
        matrix[11] = 0.0f;

        // Translation (already done)
        if (bRotationOnly == true)
        {
            matrix[12] = 0.0f;
            matrix[13] = 0.0f;
            matrix[14] = 0.0f;
        }
        else
            m3dSetMatrixColumn44(matrix, vOrigin, 3);

        matrix[15] = 1.0f;
    }
    void ApplyActorTransform(bool bRotationOnly = false)
    {
        M3DMatrix44f rotMat;

        GetMatrix(rotMat, bRotationOnly);

        // Apply rotation to the current matrix
        glMultMatrixf(rotMat);
    }
};
#define NUM_SPHERES      30
GLFrame    spheres[NUM_SPHERES];
GLFrame    frameCamera;

// Light and material Data
GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

M3DMatrix44f mShadowMatrix;

#define GROUND_TEXTURE  0
#define TORUS_TEXTURE   1
#define SPHERE_TEXTURE  2
#define WU_TEXTURE  3
#define tako_TEXTURE  4
#define NUM_TEXTURES    5
GLuint  textureObjects[NUM_TEXTURES];

const char *szTextureFiles[] = {"./newback.tga", "./wood.tga", "./orb.tga","basket.tga","newtako.tga"};

        
//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
    {
    M3DVector3f vPoints[3] = {{ 0.0f, -0.4f, 0.0f },
                             { 10.0f, -0.4f, 0.0f },
                             { 5.0f, -0.4f, -5.0f }};
    int iSphere;
    int i;
    
    // Grayish background
    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);
   
    // Clear stencil buffer with zero, increment by one whenever anybody
    // draws into it. When stencil function is enabled, only write where
    // stencil value is zero. This prevents the transparent shadow from drawing
    // over itself
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);
    
    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE_ARB);
    
    // Setup light parameters
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
        
    // Calculate shadow matrix
    M3DVector4f pPlane;
    m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);
    
    // Mostly use material tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
  
    
    // Randomly place the sphere inhabitants
    for(iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
        {
        // Pick a random location between -20 and 20 at .1 increments
        spheres[iSphere].SetOrigin(((float)((rand() % 400) - 200) * 0.1f), 0.0, (float)((rand() % 400) - 200) * 0.1f);
        }
      
    // Set up texture maps
    glEnable(GL_TEXTURE_2D);
    glGenTextures(NUM_TEXTURES, textureObjects);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    
    for(i = 0; i < NUM_TEXTURES; i++)
        {
        GLbyte *pBytes;
        GLint iWidth, iHeight, iComponents;
        GLenum eFormat;
        
        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
        
        // Load this texture map
        pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

    }

////////////////////////////////////////////////////////////////////////
// Do shutdown for the rendering context
void ShutdownRC(void)
    {
    // Delete the textures
    glDeleteTextures(NUM_TEXTURES, textureObjects);
    }


///////////////////////////////////////////////////////////
// Draw the ground as a series of triangle strips
void DrawGround(void)
    {
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat iStrip, iRun;
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f);
    
    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
        {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);

            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
                {
                glTexCoord2f(s, t);
                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
                glVertex3f(iStrip, y, iRun);
                
                glTexCoord2f(s + texStep, t);
                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
                glVertex3f(iStrip + fStep, y, iRun);
                
                t += texStep;
                }
        glEnd();
        s += texStep;
        }
    }

///////////////////////////////////////////////////////////////////////
// Draw random inhabitants and the rotating torus/sphere duo
void DrawInhabitants(GLint nShadow)
    {
    static GLfloat yRot = 0.0f;         // Rotation angle for animation
    GLint i;

    if(nShadow == 0)
        {
        yRot += 0.5f;
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
    else
        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color
  
        
    // Draw the randomly located spheres
   
 

    glPushMatrix();
        glTranslatef(0.5f, 0.0f, -2.5f);
        glScaled(0.5, 0.5, 0.5);
         
        if(nShadow == 0)
            {
            // Torus alone will be specular
            glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
            }
        
        //glRotatef(yRot/15, 0.0f, 1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
        glPushMatrix();
        glScaled(0.15, 0.15, 0.15);
        //glutSolidCube(1);
        drawbody();
        glPopMatrix();
        
        glPushMatrix();
            //neck
            glTranslatef(0.0f, 0.2f, 0.0f);
            glRotatef(-30, 0.0f, 0.0f, 1.0f);//neckrotate
            gltDrawSphere(0.05f, 21, 11);//neck

            //head
            
            glPushMatrix();
            glTranslatef(0.0f, 0.15f, 0.0f);
                glScaled(0.2, 0.2, 0.2);//head
                glutSolidCube(1);
             glPopMatrix();
        glPopMatrix();
       
        drawrightarm();
        drawleftarm();
        drawleftleg();
        drawrightleg();
        drawball();
       drawtako();
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    glPopMatrix();
    }
void drawrightarm() {
    rot += 0.5;
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, 0.0f);
    //shoulder
    gltDrawSphere(0.05f, 21, 11);
    glRotatef(90, 0.0f, 0.0f, 1.0f);


    glTranslatef(0, -0.05f, 0);
    glPushMatrix();
    glScaled(.03f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0, -0.15f, 0);

    //glRotatef(cos(rot * 0.01) * 20 + 20, 0.0f, 0.0f, 1.0f);
    glRotatef(sin(rot*0.005) * 30 + 20, 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.05f, 0);


    glPushMatrix();
    glScaled(.03f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -0.15f, 0);
    glRotatef(-30, 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.05f, 0);

    glPushMatrix();
    glScaled(.05f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}
void drawleftarm() {
    rot += 0.5;
    glPushMatrix();
    glTranslatef(-0.2f, 0.15f, 0.0f);
    //shoulder
    gltDrawSphere(0.05f, 21, 11);
    glRotatef(-rot/25, 1.0f, 0.0f, 0.0f);


    glTranslatef(0, -0.05f, 0);
    glPushMatrix();
    glScaled(.03f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0, -0.15f, 0);

    glRotatef(-(cos(rot * 0.01) * 20 + 20), 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.05f, 0);


    glPushMatrix();
    glScaled(.03f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -0.15f, 0);
    glRotatef(-sin(rot), 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.05f, 0);

    glPushMatrix();
    glScaled(.05f, .2f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}
void drawleftleg() {
    rot += 0.5;
    glPushMatrix();
    glTranslatef(-0.1f, -0.2f, 0.0f);
    //shoulder
    gltDrawSphere(0.05f, 21, 11);
    //glRotatef(-rot / 25, 0.0f, 1.0f, 0.0f);


    glTranslatef(0, -0.03f, 0);
    glPushMatrix();
    glScaled(.03f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0, -0.09f, 0);

   // glRotatef(-(cos(rot * 0.01) * 20 + 20), 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.03f, 0);


    glPushMatrix();
    glScaled(.03f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -0.09f, 0);
    //glRotatef(-sin(rot), 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.09f, 0);

    glPushMatrix();
    glScaled(.05f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}
void drawrightleg() {
    rot += 0.5;
    glPushMatrix();
    glTranslatef(0.1f, -0.2f, 0.0f);
    //shoulder
    gltDrawSphere(0.05f, 21, 11);
    //glRotatef(-rot / 10, 0.0f, 1.0f, 0.0f);


    glTranslatef(0, -0.03f, 0);
    glPushMatrix();
    glScaled(.03f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0, -0.09f, 0);

    //glRotatef(-(cos(rot * 0.01) * 20 + 20), 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.03f, 0);


    glPushMatrix();
    glScaled(.03f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();
    glTranslatef(0, -0.09f, 0);
   // glRotatef(-sin(rot)*5, 0.0f, 0.0f, 1.0f);
    gltDrawSphere(0.05f, 21, 11);
    glTranslatef(0, -0.09f, 0);

    glPushMatrix();
    glScaled(.05f, .1f, .03f);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}
void drawbody() {
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); 
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); 
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); 
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); 
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    // back face
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // top face
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

    // bottom face
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // right face
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // left face
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
}
void drawball() {
    brot += 0.5;
    glBindTexture(GL_TEXTURE_2D, textureObjects[WU_TEXTURE]);
    glPushMatrix();
    glRotatef(sin(brot/50)*30+5 , 0.0f, 0.0f, 0.5f);
    glTranslatef(0.6f, -0.2f, 0.0f);
    gltDrawSphere(0.14f, 21, 11);
    glPopMatrix();
}
void drawtako() {
    //brot += 0.5;
    takorot += 0.5;
    glBindTexture(GL_TEXTURE_2D, textureObjects[tako_TEXTURE]);
    glPushMatrix();
    glRotatef(90, 1.0f, 0.0f, 0.0f);

    //glRotatef(sin(takorot /50)*20+20, 1.0f,0.0f, 0.0f);
    glScaled(.3f, .3f, .3f);
    glTranslatef(-9.5f, -1.5f, 0.0f);
    glTranslatef(-sin(takorot / 60) * 1.3 + 0.8, 0.0f, 0.0f);

   // glTranslatef(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // top face
    glBindTexture(GL_TEXTURE_2D, textureObjects[tako_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

    // bottom face
    glBindTexture(GL_TEXTURE_2D, textureObjects[tako_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    // right face
    glBindTexture(GL_TEXTURE_2D, textureObjects[tako_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // left face
    glBindTexture(GL_TEXTURE_2D, textureObjects[tako_TEXTURE]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}
        
// Called to draw scene
void RenderScene(void)
    {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
    glPushMatrix();
       // frameCamera.ApplyCameraTransform();
        
        // Position light before any other transformations
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
        
        // Draw the ground
        glColor3f(1.0f, 1.0f, 1.0f);
        DrawGround();
        
        // Draw shadows first
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_STENCIL_TEST);
        glPushMatrix();
            glMultMatrixf(mShadowMatrix);
           DrawInhabitants(1);
           
        drawtest(1);
        //Drawta(1);
        //Drawball(1);
        glPopMatrix();
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        
        // Draw inhabitants normally
        DrawInhabitants(0);

    glPopMatrix();

    drawtest(0);

    

    // Do the buffer Swap
    glutSwapBuffers();
    }



// Respond to arrow keys by moving the camera frame of reference
/*void SpecialKeys(int key, int x, int y)
    {
    if(key == GLUT_KEY_UP)
        frameCamera.MoveForward(0.1f);

    if(key == GLUT_KEY_DOWN)
		frameCamera.MoveForward(-0.1f);

    if(key == GLUT_KEY_LEFT)
		frameCamera.RotateLocalY(0.1f);
	   
    if(key == GLUT_KEY_RIGHT)
        frameCamera.RotateLocalY(-0.1f);
                        
    // Refresh the Window
    glutPostRedisplay();
    }*/

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
    {
    if (timer_flag==0)
    {
        return;

    }
    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(3,TimerFunction, 1);
    }

void ChangeSize(int w, int h)
    {
    GLfloat fAspect;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
        
    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set the clipping volume
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
    }

int main(int argc, char* argv[])
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800,600);
    glutCreateWindow("OpenGL SphereWorld Demo + Texture Maps");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    createGLUTMenus();
   // glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(myKeyboard);
    
    SetupRC();
    glutTimerFunc(33, TimerFunction, 1);

    glutMainLoop();
    
    ShutdownRC();

    return 0;
    }
void myKeyboard(unsigned char key, int x, int y) {
    switch (key) {
     case 'p':
         if (timer_flag==0) {
             timer_flag = 1;
             glutTimerFunc(3, TimerFunction, timer_flag);
             std::cout << timer_flag << std::endl;
         }
         else
         {
             timer_flag = 0;
             std::cout << timer_flag << std::endl;
         }
         break;
    }
}
void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3) {
    // Get two vectors... do the cross product
    M3DVector3f v1, v2;

    // V1 = p3 - p1
    v1[0] = p3[0] - p1[0];
    v1[1] = p3[1] - p1[1];
    v1[2] = p3[2] - p1[2];

    // V2 = P2 - p1
    v2[0] = p2[0] - p1[0];
    v2[1] = p2[1] - p1[1];
    v2[2] = p2[2] - p1[2];

    // Unit normal to plane - Not sure which is the best way here
    m3dCrossProduct3(planeEq, v1, v2);
    m3dNormalizeVector3(planeEq);
    // Back substitute to get D
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}
void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos)
{
    // These just make the code below easier to read. They will be 
    // removed by the optimizer.	
    float a = planeEq[0];
    float b = planeEq[1];
    float c = planeEq[2];
    float d = planeEq[3];

    float dx = -vLightPos[0];
    float dy = -vLightPos[1];
    float dz = -vLightPos[2];

    // Now build the projection matrix
    proj[0] = b * dy + c * dz;
    proj[1] = -a * dy;
    proj[2] = -a * dz;
    proj[3] = 0.0;

    proj[4] = -b * dx;
    proj[5] = a * dx + c * dz;
    proj[6] = -b * dz;
    proj[7] = 0.0;

    proj[8] = -c * dx;
    proj[9] = -c * dy;
    proj[10] = a * dx + b * dy;
    proj[11] = 0.0;

    proj[12] = -d * dx;
    proj[13] = -d * dy;
    proj[14] = -d * dz;
    proj[15] = a * dx + b * dy + c * dz;
    // Shadow matrix ready
}
inline void m3dCrossProduct3(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
{
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = -u[0] * v[2] + v[0] * u[2];
    result[2] = u[0] * v[1] - v[0] * u[1];
}
inline void m3dNormalizeVector3(M3DVector3f u)
{
    m3dScaleVector3(u, 1.0f / m3dGetVectorLength3(u));
}
inline void m3dScaleVector3(M3DVector3f v, const float scale)
{
    v[0] *= scale; v[1] *= scale; v[2] *= scale;
}
inline float m3dGetVectorLength3(const M3DVector3f u)
{
    return sqrtf(m3dGetVectorLengthSquared3(u));
}
inline float m3dGetVectorLengthSquared3(const M3DVector3f u)
{
    return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}
void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
    const M3DVector3f point3)
{
    M3DVector3f v1, v2;		// Temporary vectors

    // Calculate two vectors from the three points. Assumes counter clockwise
    // winding!
    v1[0] = point1[0] - point2[0];
    v1[1] = point1[1] - point2[1];
    v1[2] = point1[2] - point2[2];

    v2[0] = point2[0] - point3[0];
    v2[1] = point2[1] - point3[1];
    v2[2] = point2[2] - point3[2];

    // Take the cross product of the two vectors to get
    // the normal vector.
    m3dCrossProduct3(result, v1, v2);
}
void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks)
{
    GLfloat drho = (GLfloat)(3.141592653589) / (GLfloat)iStacks;
    GLfloat dtheta = 2.0f * (GLfloat)(3.141592653589) / (GLfloat)iSlices;
    GLfloat ds = 1.0f / (GLfloat)iSlices;
    GLfloat dt = 1.0f / (GLfloat)iStacks;
    GLfloat t = 1.0f;
    GLfloat s = 0.0f;
    GLint i, j;     // Looping variables

    for (i = 0; i < iStacks; i++)
    {
        GLfloat rho = (GLfloat)i * drho;
        GLfloat srho = (GLfloat)(sin(rho));
        GLfloat crho = (GLfloat)(cos(rho));
        GLfloat srhodrho = (GLfloat)(sin(rho + drho));
        GLfloat crhodrho = (GLfloat)(cos(rho + drho));

        // Many sources of OpenGL sphere drawing code uses a triangle fan
        // for the caps of the sphere. This however introduces texturing 
        // artifacts at the poles on some OpenGL implementations
        glBegin(GL_TRIANGLE_STRIP);
        s = 0.0f;
        for (j = 0; j <= iSlices; j++)
        {
            GLfloat theta = (j == iSlices) ? 0.0f : j * dtheta;
            GLfloat stheta = (GLfloat)(-sin(theta));
            GLfloat ctheta = (GLfloat)(cos(theta));

            GLfloat x = stheta * srho;
            GLfloat y = ctheta * srho;
            GLfloat z = crho;

            glTexCoord2f(s, t);
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;
            glTexCoord2f(s, t - dt);
            s += ds;
            glNormal3f(x, y, z);
            glVertex3f(x * fRadius, y * fRadius, z * fRadius);
        }
        glEnd();

        t -= dt;
    }
}
GLbyte* gltLoadTGA(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
{
    FILE* pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte* pBits = NULL;          // Pointer to bits

    // Default/Failed values
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;

    // Attempt to open the file
    pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
    {
        printf("File not found");
        return NULL;
    }
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

    // Do byte swap for big vs little endian
#ifdef __APPLE__
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
    LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
    LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
    LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
    LITTLE_ENDIAN_WORD(&tgaHeader.width);
    LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


    // Get width, height, and depth of texture
    * iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;

    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;

    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

    // Allocate memory and check for success
    pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    if (pBits == NULL)
        return NULL;

    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if (fread(pBits, lImageSize, 1, pFile) != 1)
    {
        free(pBits);
        return NULL;
    }

    // Set OpenGL format expected
    switch (sDepth)
    {
    case 3:     // Most likely case
        *eFormat = GL_BGR_EXT;
        *iComponents = GL_RGB8;
        break;
    case 4:
        *eFormat = GL_BGRA_EXT;
        *iComponents = GL_RGBA8;
        break;
    case 1:
        *eFormat = GL_LUMINANCE;
        *iComponents = GL_LUMINANCE8;
        break;
    };


    // Done with File
    fclose(pFile);

    // Return pointer to image data
    return pBits;
}
inline void	m3dCopyVector3(M3DVector3f dst, const M3DVector3f src) { memcpy(dst, src, sizeof(M3DVector3f)); }
inline void m3dCrossProduct(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
{
    result[0] = u[1] * v[2] - v[1] * u[2];
    result[1] = -u[0] * v[2] + v[0] * u[2];
    result[2] = u[0] * v[1] - v[0] * u[1];
}
inline void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, int column)
{
    memcpy(dst + (4 * column), src, sizeof(float) * 4);
}
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
{
    M3DVector3f vNormal;
    double majorStep = 2.0f * M3D_PI / numMajor;
    double minorStep = 2.0f * M3D_PI / numMinor;
    int i, j;

    for (i = 0; i < numMajor; ++i)
    {
        double a0 = i * majorStep;
        double a1 = a0 + majorStep;
        GLfloat x0 = (GLfloat)cos(a0);
        GLfloat y0 = (GLfloat)sin(a0);
        GLfloat x1 = (GLfloat)cos(a1);
        GLfloat y1 = (GLfloat)sin(a1);

        glBegin(GL_TRIANGLE_STRIP);
        for (j = 0; j <= numMinor; ++j)
        {
            double b = j * minorStep;
            GLfloat c = (GLfloat)cos(b);
            GLfloat r = minorRadius * c + majorRadius;
            GLfloat z = minorRadius * (GLfloat)sin(b);

            // First point
            glTexCoord2f((float)(i) / (float)(numMajor), (float)(j) / (float)(numMinor));
            vNormal[0] = x0 * c;
            vNormal[1] = y0 * c;
            vNormal[2] = z / minorRadius;
            m3dNormalizeVector(vNormal);
            glNormal3fv(vNormal);
            glVertex3f(x0 * r, y0 * r, z);

            glTexCoord2f((float)(i + 1) / (float)(numMajor), (float)(j) / (float)(numMinor));
            vNormal[0] = x1 * c;
            vNormal[1] = y1 * c;
            vNormal[2] = z / minorRadius;
            m3dNormalizeVector(vNormal);
            glNormal3fv(vNormal);
            glVertex3f(x1 * r, y1 * r, z);
        }
        glEnd();
    }
}
inline void m3dNormalizeVector(M3DVector3f u)
{
    m3dScaleVector3(u, 1.0f / m3dGetVectorLength(u));
}
inline float m3dGetVectorLength(const M3DVector3f u)
{
    return float(sqrt(double(m3dGetVectorLengthSquared(u))));
}
inline float m3dGetVectorLengthSquared(const M3DVector3f u)
{
    return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
}
void createGLUTMenus() {
    int Color_mode, Open_File;

    Open_File = glutCreateMenu(OpenFile);
    glutAddMenuEntry("gound", 1);
    glutAddMenuEntry("octahedron", 2);
    glutAddMenuEntry("teapot", 3);
    glutAddMenuEntry("teddy", 4);
    glutAddMenuEntry("userinput", 5);
    Color_mode = glutCreateMenu(Colormode);
    glutAddMenuEntry("fix_color", 1);
    glutAddMenuEntry("random", 2);
    int fill_mode;
    fill_mode = glutCreateMenu(fillmode);
    glutAddMenuEntry("point", 0);
    glutAddMenuEntry("line", 1);
    glutAddMenuEntry("fill", 2);
    int boundbox_mode;
    boundbox_mode = glutCreateMenu(boundboxmode);
    glutAddMenuEntry("on", 1);
    glutAddMenuEntry("off", 2);
    glutCreateMenu(Menu);
    glutAddSubMenu("OpenFile", Open_File);
    glutAddSubMenu("ColorMode", Color_mode);
    glutAddSubMenu("FillMode", fill_mode);
    glutAddSubMenu("BoundingMode", boundbox_mode);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void boundboxmode(int option) {
    switch (option)
    {
    case 1:
        boundbox_flag = 1;
        glutPostRedisplay();
        break;
    case 2:
        boundbox_flag = 0;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
void fillmode(int option)
{
    switch (option)
    {
    case 0:
        fill = 1;

        glutPostRedisplay();
        break;
    case 1:
        fill = 2;

        glutPostRedisplay();
        break;
    case 2:
        fill = 3;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
void Menu(int option) {
}
void Colormode(int obj) {
    std::string FileName = "";
    switch (obj) {
    case 1:
        fix_color = 1;
        R = 0.5;
        G = 0;
        B = 0;
        glutPostRedisplay();
        break;
    case 2:
        fix_color = 0;
        srand((unsigned)time(NULL));
         min = 0;
         max = 1.0;
         r = (max - min) * rand() / (RAND_MAX + 1.0) + min;
         g = (max - min) * rand() / (RAND_MAX + 1.0) + min;
         b = (max - min) * rand() / (RAND_MAX + 1.0) + min;

        rr = r;
        rg = g;
        rb = b;
        glutPostRedisplay();
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void OpenFile(int obj) {
    std::string FileName = "";
    switch (obj) {
    case 1:

        FileName = "gourd.obj";

        break;
    case 2:
        std::cout << "load octahedron" << std::endl;
        FileName = "octahedron.obj";
        break;
    case 3:

        FileName = "teapot.obj";
        break;
    case 4:

        FileName = "teddy.obj";
        break;
    case 5:
        std::cout << "Please input file name " <<  std::endl;
        std::cin >> userinput;
        std::cout << "input is " << userinput << std::endl;
        FileName = userinput;
        break;
    default:
        break;
    }
    loadFile(FileName);
    glutPostRedisplay();
}
void loadFile(std::string fileName) {
    v.clear();
    f.clear();
    xMin = 999, xMax = -999, yMin = 999, yMax = -999, zMin = 999, zMax = -999;
    allmax=0, allmin=999;
    std::fstream source;
    source.open(fileName, std::ios::in);
    if (source.is_open() == true) {
        Mode = "lines";
        while (getline(source, line)) {
            if (line.substr(0, 2) == "v ") {
                std::istringstream iss(line);
                std::string token;
                while (getline(iss, token, ' '))
                {
                    //                    cout<<"token="<<token<<endl;
                    if (token != "v") {
                        std::stringstream ss;
                        float temp;
                        ss << token;
                        ss >> temp;
                        v.push_back(temp);
                        nv.push_back(temp);
                    }
                }
            }
            else if (line.substr(0, 2) == "f ") {
                f.push_back(line.substr(2));
            }
        }
        //cout << "get " << fileName << endl;
        for (int i = 0; i < v.size(); i += 3) {
            if (v[i] > xMax) {
                xMax = v[i];
            }
            if (v[i] < xMin) {
                xMin = v[i];
            }
            if (v[i + 1] > yMax) {
                yMax = v[i + 1];
            }
            if (v[i + 1] < yMin) {
                yMin = v[i + 1];
            }
            if (v[i + 2] > zMax) {
                zMax = v[i + 2];
            }
            if (v[i + 2] < zMin) {
                zMin = v[i + 2];
            }
        }
        if (xMax > allmax) {
            allmax = xMax;
        }
        if (yMax > allmax) {
            allmax = yMax;
        }
        if (zMax > allmax) {
            allmax = zMax;
        }
        if (xMin < allmin) {
            allmin = xMin;
        }
        if (yMin < allmin) {
            allmin = yMin;
        }
        if (zMin < allmin) {
            allmin = zMin;
        }
        xC = (xMin + xMax) / 2;
        xNC = (xC - allmin) / (allmax - allmin)*2-1;
        yC = (yMin + yMax) / 2;
        yNC = (yC - allmin) / (allmax - allmin)*2-1;
        zC = (zMin + zMax) / 2;
        zNC = (zC - allmin) / (allmax - allmin)*2-1;
        xNMin= (xMin -allmin) / (allmax - allmin) * 2 - 1;
        yNMin = (yMin - allmin) / (allmax - allmin) * 2 - 1;
        zNMin = (zMin - allmin) / (allmax - allmin) * 2 - 1;
        xNMax=(xMax-allmin)/ (allmax - allmin) * 2 - 1;
        yNMax = (yMax - allmin) / (allmax - allmin) * 2 - 1;
        zNMax = (zMax - allmin) / (allmax - allmin) * 2 - 1;
    }
    else {
        std::cout << "can't open file!" << std::endl;
    }
    source.close();
}
void Objfile(int nShadow) {
    float temparray[3][3];

    for (int i = 0; i < f.size(); i++) {
        std::vector<int> f1;
        f1.clear();
        std::istringstream iss(f[i]);
        std::string token;
        while (getline(iss, token, ' ')) {
            std::stringstream ss;
            int temp;
            ss << token;
            ss >> temp;
            f1.push_back(temp);

            //cout << temp << " ";
        }

        //glTranslatef(0.0f, 0.2f, -2.5f);
        //glColor3f(0.5, 0.5, 0.5);
        //glutSolidCube(0.1);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_NORMALIZE);
        
        if (fill == 1) {
            glBegin(GL_POINTS);
        }
        else if (fill == 2) {
            glBegin(GL_LINES);
        }
        else if (fill == 3) {
            glBegin(GL_TRIANGLES);
        }
        
        
        //glColor3f(R,G, B);
        int k = 0;
        while  ( k < f1.size()) {
            //                    cout<<i<<"="<<f[i]<<" ";
            M3DVector3f normal;
            for (int j = 0; j < 3; j++) {
                int px = (f1[k] - 1) * 3;
                int py = (f1[k] - 1) * 3 + 1;
                int pz = (f1[k] - 1) * 3 + 2;
                temparray[j][0] = (v[px]-allmin)/(allmax-allmin)*2-1-xNC;
                temparray[j][1] = (v[py] - allmin) / (allmax - allmin) * 2 - 1 - yNC;
                temparray[j][2] = (v[pz] - allmin) / (allmax - allmin) * 2 - 1-zNC;
                //temparray[j][0] = (v[px]);
                //temparray[j][1] = (v[py]);
                //temparray[j][2] = (v[pz]);
                k++;
            }
            m3dFindNormal(
               normal, 
               temparray[0],
               temparray[1],
               temparray[2]
           );
            glNormal3fv(normal);
            if (fix_color == 0) {
                
                for (int j = 0; j < 3; j++) {
                    R = rr + ((temparray[j][0] - xNMin) / (xNMax - xNMin));
                    G = rg + ((temparray[j][1] - yNMin) / (yNMax - yNMin));
                    B = rb + ((temparray[j][2] - zNMin) / (zNMax - zNMin));
                    if (nShadow == 0) {

                        glColor4f(R, G, B, 1.0f);
                    }
                    else {
                        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color*/

                    }
                    /*   R = r;
                       G = g;
                       B = b;*/
                    glVertex3f(temparray[j][0], temparray[j][1], temparray[j][2]);

                }
                
            }

            
            
            else
            {
                if (nShadow == 0) {

                    glColor4f(R, G, B, 1.0f);
                }
                else {
                    glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color*/

                }
                for (int j = 0; j < 3; j++) {
                    glVertex3f(temparray[j][0], temparray[j][1], temparray[j][2]);
                }
            }
    
        }
        glEnd();
        glEnable(GL_TEXTURE_2D);
        //glutPostRedisplay();
        f1.clear();
    }
}
void drawtest(int nShadow) {
    static GLfloat objRot = 0.0f;         // Rotation angle for animation
    GLint i;
    objRot += 0.5f;
    /*if (nShadow == 0)
    {
       
        glColor4f(R,G, B, 1.0f);
    }
    else
        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color*/


    // Draw the randomly located spheres

    glPushMatrix();
  
    glTranslatef(0.0f, 0.0f, -2.5f);
    

    if (nShadow == 0)
    {
        // Torus alone will be specular
        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    }

    
    //head

    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glScaled(0.2, 0.2, 0.2);//head
    
    //glutSolidCube(1);
    glRotatef(objRot , 0.0f, 1.0f, 0.0f);
    Objfile(nShadow);
    //drawtako();
    glPopMatrix();
    if (boundbox_flag == 1) {
        glPushMatrix();
        glTranslatef(0.0f, 0.15f, 0.0f);
        float scx, scy, scz;
        scx = (xMax - xMin) / (allmax - allmin);
        scy = (yMax - yMin) / (allmax - allmin);
        scz = (zMax - zMin) / (allmax - allmin);

        glScaled(scx, scy, scz);
        glRotatef(objRot, 0.0f, 1.0f, 0.0f);
        glutWireCube(0.5);
        glPopMatrix();
    }
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    glPopMatrix();

}
//void Drawta(GLint nShadow) {
//    static GLfloat yRot = 0.0f;         // Rotation angle for animation
//    GLint i;
//
//    if (nShadow == 0)
//    {
//        yRot += 0.5f;
//        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    }
//    else
//        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color
//
//
//    // Draw the randomly located spheres
//
//
//
//    glPushMatrix();
//    glTranslatef(0.0f, 0.0f, -2.5f);
//
//    if (nShadow == 0)
//    {
//        // Torus alone will be specular
//        glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
//    }
//
//    //glRotatef(yRot/15, 0.0f, 1.0f, 0.0f);
//    glBindTexture(GL_TEXTURE_2D, textureObjects[WU_TEXTURE]);
//    glPushMatrix();
//    glScaled(0.3f, 0.3f, 0.3f);
//   // gltDrawSphere(0.05f, 21, 11);
//    //drawbody();
//    drawtako();
//    glPopMatrix();
//    glPopMatrix();
//}