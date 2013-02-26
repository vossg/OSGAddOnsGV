/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2012 by the OpenSG Forum                    *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk            'at' opensg.org                                  * 
 *          gerrit.voss     'at' vossg.org                                   *
 *          carsten_neumann 'at' gmx.net                                     *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
//#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include <sofa/helper/gl/template.h>
#include "OSGSofaDrawTool.h"
//#include <sofa/core/visual/DrawTool.h>

#include <sofa/helper/system/gl.h>
#include <sofa/helper/system/glut.h>
#include <sofa/helper/gl/Axis.h>

BEGIN_SOFA_CORE_VISUAL_NAMESPACE

using namespace sofa::defaulttype;
using namespace sofa::helper::gl;

OSGDrawTool::OSGDrawTool(void) :
     DrawTool         (     ),
    _bLightEnabled    (false),
    _iPolygonMode     (1    ),
    _bWireFrameEnabled(false)
{
}
  
//////////////////////////////////////////////////////////////////////////////
  
OSGDrawTool::~OSGDrawTool(void)
{

}

//////////////////////////////////////////////////////////////////////////////
  
void OSGDrawTool::drawPoints(const std::vector<Vector3> &points, 
                                   float                 size, 
                             const Vec4f                 colour = 
                                  Vec4f(1.0f, 1.0f, 1.0f, 1.0f))
{
    return;

    setMaterial(colour);

    glPointSize(size);
    glDisable(GL_LIGHTING);

    glBegin(GL_POINTS);
    {
        for(unsigned int i = 0; i < points.size(); ++i)
        {
            drawPoint(points[i], colour);
        } 
    }
    glEnd();

    if(getLightEnabled() == true) 
        glEnable(GL_LIGHTING);

    resetMaterial(colour);

    glPointSize(1);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawPoints(const std::vector< Vec3d > &points, 
				float size, 
				std::vector< Vec4f > colour)
{
	return;
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawLines(const std::vector<Vector3> &points, 
                                  float                 size, 
                            const Vec4f                 colour)
{
    return;

    setMaterial(colour);

    glLineWidth(size);    
    glDisable(GL_LIGHTING);    

    glBegin(GL_LINES);
    {
        for(unsigned int i = 0; i < points.size() / 2; ++i)
        {       
            drawPoint(points[2 * i    ], colour);
            drawPoint(points[2 * i + 1], colour);
        }       
    }
    glEnd();

    if(getLightEnabled() == true) 
        glEnable(GL_LIGHTING);

    resetMaterial(colour);

    glLineWidth(1);
}
  
//////////////////////////////////////////////////////////////////////////////
  
void OSGDrawTool::drawLines(const std::vector<Vector3> &points, 
                            const std::vector<Vec2i  > &index, 
                                  float                 size, 
                            const Vec4f                 colour = 
                                Vec4f(1.0f, 1.0f, 1.0f, 1.0f))
{
    return;
 
    setMaterial(colour);

    glLineWidth(size);
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    {
        for (unsigned int i = 0; i < index.size(); ++i)
        {          
            drawPoint(points[index[i][0]], colour);
            drawPoint(points[index[i][1]], colour);
        }       
    }
    glEnd();

    if(getLightEnabled() == true) 
        glEnable(GL_LIGHTING);

    resetMaterial(colour);

    glLineWidth(1);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangle(
        const Vector3& A,
        const Vector3& B,
        const Vector3& C,
        const Vector3& colour)
{
    return;
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangles(const std::vector<Vector3> &points, 
                                const Vec4f                 colour)
{ 
    return;

    setMaterial(colour);

    glBegin(GL_TRIANGLES);
    {
        for(unsigned int i = 0; i < points.size() / 3; ++i)
        {
            const Vector3 &a = points[3 *i +0];
            const Vector3 &b = points[3 *i +1];
            const Vector3 &c = points[3 *i +2];

            Vector3 n = cross((b - a), (c - a));

            n.normalize();

            drawTriangle(a, b, c, n, colour);
        }
    }
    glEnd();

    resetMaterial(colour);
}
  
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangles(const std::vector<Vector3> &points, 
                                const Vector3               normal, 
                                const Vec4f                 colour)
{
    return;

    setMaterial(colour);

    glBegin(GL_TRIANGLES);
    {
        for(unsigned int i = 0; i < points.size() / 3; ++i)
        {
            drawTriangle(points[3 * i + 0],
                         points[3 * i + 1],
                         points[3 * i + 2], 
                         normal, 
                         colour           );
        }
    }
    glEnd();

    resetMaterial(colour);
}
  
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangles(const std::vector<Vector3> &points, 
                                const std::vector<Vec3i  > &index, 
                                const std::vector<Vector3> &normal, 
                                const Vec4f                 colour =
                                    Vec4f(1.0f, 1.0f, 1.0f, 1.0f))
{ 
    return;

    setMaterial(colour);

    glBegin(GL_TRIANGLES);
    {
        for(unsigned int i = 0; i < index.size(); ++i)
        {         
            drawTriangle(points[index[i][0]],
                         points[index[i][1]],
                         points[index[i][2]],
                         normal[i],
                         colour             );
        }       
    }
    glEnd();

    resetMaterial(colour);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangles(const std::vector<Vector3> &points,
                                const std::vector<Vector3> &normal, 
                                const std::vector<Vec4f  > &colour)
{
    return;

    const unsigned int nbTriangles = points.size() / 3;

    bool computeNormals = (normal.size() != nbTriangles);

    if(nbTriangles == 0) 
        return;
//    setMaterial(colour);

    glBegin(GL_TRIANGLES);
    {
        for(unsigned int i = 0; i < nbTriangles; ++i)
        {
            if(!computeNormals)
            {
                drawTriangle(points[3 * i + 0],
                             points[3 * i + 1],
                             points[3 * i + 2],
                             normal[    i    ],
                             colour[3 * i + 0],
                             colour[3 * i + 1],
                             colour[3 * i + 2]);
            }
            else
            {
                const Vector3 &a = points[3 * i + 0];
                const Vector3 &b = points[3 * i + 1];
                const Vector3 &c = points[3 * i + 2];

                Vector3 n = cross((b - a), (c - a));

                n.normalize();

                drawPoint(a, n, colour[3 * i + 0]);
                drawPoint(b, n, colour[3 * i + 1]);
                drawPoint(c, n, colour[3 * i + 2]);

            }
        }
    }
    glEnd();

//    resetMaterial(colour);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangleStrip(const std::vector<Vector3> &points,
                                    const std::vector<Vector3> &normal,
                                    const Vec4f                 colour)
{
    return;

    setMaterial(colour);

    glBegin(GL_TRIANGLE_STRIP);
    {
        for(unsigned int i = 0; i < normal.size(); ++i)
        {        
            glNormalT(normal[i]); 

            glVertexNv<3>(points[2 * i    ].ptr());
            glVertexNv<3>(points[2 * i + 1].ptr());
        }       
    }
    glEnd();

    resetMaterial(colour);
}
    
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangleFan(const std::vector<Vector3> &points,
                                  const std::vector<Vector3> &normal,
                                  const Vec4f                 colour)
{
    return;

    if(points.size() < 3) 
        return;

    setMaterial(colour);

    glBegin(GL_TRIANGLE_FAN);
    {
        glNormalT(normal[0]);

        glVertexNv<3>(points[0].ptr());
        glVertexNv<3>(points[1].ptr());
        glVertexNv<3>(points[2].ptr());

        for (unsigned int i = 3; i < points.size(); ++i)
        {
            glNormalT(normal[i]);

            glVertexNv<3>(points[i].ptr());
        }
    }
    glEnd();

    resetMaterial(colour);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawFrame(const Vector3    &position, 
                            const Quaternion &orientation, 
                            const Vec3f      &size       )
{
    return;

    setPolygonMode(0, false);

    helper::gl::Axis::draw(position, orientation, size);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawFrame(	const Vector3 &position, 
				const Quaternion &orientation, 
				const Vec3f &size, 
				const Vec4f &colour )
{
	return;
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawSpheres(const std::vector<Vector3> &points, 
                                    float                 radius, 
                              const Vec4f                 colour)
{
    return;

    setMaterial(colour);

    for(unsigned int i = 0; i < points.size(); ++i)
        drawSphere(points[i], radius);

    resetMaterial(colour);
}
    
//////////////////////////////////////////////////////////////////////////////
    
void OSGDrawTool::drawSpheres(const std::vector<Vector3> &points, 
                              const std::vector<float>   &radius, 
                              const Vec4f                 colour)
{      
    return;

    setMaterial(colour);

    for(unsigned int i = 0; i < points.size(); ++i)
        drawSphere(points[i], radius[i]);
    
    resetMaterial(colour);
}
    
///////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawCone(const Vector3 &p1, 
                           const Vector3 &p2, 
                                 float    radius1, 
                                 float    radius2, 
                           const Vec4f    colour, 
                                 int      subd   )
{
    return;

    Vector3 tmp = p2 - p1;

    setMaterial(colour);
    /* create Vectors p and q, co-planar with the cylinder's cross-sectional
       disk 
     */ 

    Vector3 p = tmp;

    if(fabs(p[0]) + fabs(p[1]) < 0.00001 * tmp.norm())
    {
        p[0] += 1.0;
    }
    else
    {
        p[2] += 1.0;
    }

    Vector3 q;

    q = p.cross(tmp);
    p = tmp.cross(q);

    /* do the normalization outside the segment loop */
    p.normalize();
    q.normalize();
      
    int i2;
    float theta, st, ct;

    /* build the cylinder from rectangular subd */
    std::vector<Vector3> points;
    std::vector<Vec4i  > indices;
    std::vector<Vector3> normals;

    std::vector<Vector3> pointsCloseCylinder1;
    std::vector<Vector3> normalsCloseCylinder1;
    std::vector<Vector3> pointsCloseCylinder2;
    std::vector<Vector3> normalsCloseCylinder2;

    Vector3 dir=p1 - p2; 
    
    dir.normalize();

    pointsCloseCylinder1 .push_back( p1 );
    normalsCloseCylinder1.push_back( dir);
    pointsCloseCylinder2 .push_back( p2 );
    normalsCloseCylinder2.push_back(-dir);


    for(i2 = 0 ;i2 <= subd ;i2++)
    {
        /* sweep out a circle */
        theta =  i2 * 2.0 * 3.14 / subd;

        st = sin(theta);
        ct = cos(theta);

        /* construct normal */
        tmp = p * ct + q * st;

        /* set the normal for the two subseqent points */
        normals.push_back(tmp);

        /* point on disk 1 */
        Vector3 w(p1);

        w += tmp * radius1;

        points.push_back(w);

        pointsCloseCylinder1 .push_back(w  );
        normalsCloseCylinder1.push_back(dir);

        /* point on disk 2 */
        w = p2;

        w += tmp * radius2;

        points.push_back(w);          

        pointsCloseCylinder2 .push_back( w  );
        normalsCloseCylinder2.push_back(-dir);
    }

    pointsCloseCylinder1 .push_back(pointsCloseCylinder1 [1]);
    normalsCloseCylinder1.push_back(normalsCloseCylinder1[1]);
    pointsCloseCylinder2 .push_back(pointsCloseCylinder2 [1]);
    normalsCloseCylinder2.push_back(normalsCloseCylinder2[1]);


    drawTriangleStrip(points, normals,colour);

    if(radius1 > 0) 
    {
        drawTriangleFan(pointsCloseCylinder1, 
                        normalsCloseCylinder1,
                        colour               );
    }

    if(radius2 > 0)
    {
        drawTriangleFan(pointsCloseCylinder2, 
                        normalsCloseCylinder2,
                        colour               );
    }

    resetMaterial(colour);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawCube(const float &radius, 
                           const Vec4f &colour, 
                           const int   &subd  )
{
    return;

    // X Axis
    drawCylinder(Vector3(-1.0, -1.0, -1.0), 
                 Vector3( 1.0, -1.0, -1.0), radius, colour, subd);
    drawCylinder(Vector3(-1.0,  1.0, -1.0), 
                 Vector3( 1.0,  1.0, -1.0), radius, colour, subd);
    drawCylinder(Vector3(-1.0, -1.0,  1.0), 
                 Vector3( 1.0, -1.0,  1.0), radius, colour, subd);
    drawCylinder(Vector3(-1.0,  1.0,  1.0), 
                 Vector3( 1.0,  1.0,  1.0), radius, colour, subd);

    // Y Axis
    drawCylinder(Vector3(-1.0, -1.0, -1.0), 
                 Vector3(-1.0,  1.0, -1.0), radius, colour, subd);
    drawCylinder(Vector3(-1.0, -1.0,  1.0), 
                 Vector3(-1.0,  1.0,  1.0), radius, colour, subd);
    drawCylinder(Vector3( 1.0, -1.0, -1.0), 
                 Vector3( 1.0,  1.0, -1.0), radius, colour, subd);
    drawCylinder(Vector3( 1.0, -1.0,  1.0), 
                 Vector3( 1.0,  1.0,  1.0), radius, colour, subd);

    // Z Axis
    drawCylinder(Vector3(-1.0, -1.0, -1.0), 
                 Vector3(-1.0, -1.0,  1.0), radius, colour, subd);
    drawCylinder(Vector3(-1.0,  1.0, -1.0), 
                 Vector3(-1.0,  1.0,  1.0), radius, colour, subd);
    drawCylinder(Vector3( 1.0, -1.0, -1.0), 
                 Vector3( 1.0, -1.0,  1.0), radius, colour, subd);
    drawCylinder(Vector3( 1.0,  1.0, -1.0), 
                 Vector3( 1.0,  1.0,  1.0), radius, colour, subd);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawCylinder(const Vector3 &p1, 
                               const Vector3 &p2, 
                                     float    radius, 
                               const Vec4f    colour, 
                                     int      subd  )
{      
    return;

    drawCone(p1, p2, radius, radius, colour, subd);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawCapsule(	const Vector3& p1, 
				const Vector3 &p2, 
				float radius, 
				const Vec4f colour,  
				int subd)
{
	return;
}
    
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawArrow(const Vector3 &p1, 
                            const Vector3 &p2, 
                                  float    radius, 
                            const Vec4f    colour,  
                                  int      subd  )
{
    return;

    Vector3 p3 = p1 * 0.2 + p2 * 0.8;

    drawCylinder(p1, p3, radius,          colour, subd);
    drawCone    (p3, p2, radius * 2.5, 0, colour, subd);
}
  
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawPlus(const float &radius, 
                           const Vec4f &colour, 
                           const int   &subd  )
{
    return;

    drawCylinder(Vector3(-1.0,  0.0,  0.0), 
                 Vector3( 1.0,  0.0,  0.0), radius, colour, subd);
    drawCylinder(Vector3( 0.0, -1.0,  0.0), 
                 Vector3( 0.0,  1.0,  0.0), radius, colour, subd);
    drawCylinder(Vector3( 0.0,  0.0, -1.0), 
                 Vector3( 0.0,  0.0,  1.0), radius, colour, subd);
}
    
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawPoint(const Vector3 &p, const Vec4f &c)
{
    return;

    glColor4f(c[0], c[1], c[2], c[3]);

    glVertexNv<3>(p.ptr());
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawPoint(const Vector3 &p, 
                            const Vector3 &n, 
                            const Vec4f   &c)
{ 
    return;

    glColor4f(c[0], c[1], c[2], c[3]);

    glNormalT(n);

    glVertexNv<3>(p.ptr());
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangle(const Vector3 &p1,
                               const Vector3 &p2,
                               const Vector3 &p3,
                               const Vector3 &normal,
                               const Vec4f   &c1, 
                               const Vec4f   &c2, 
                               const Vec4f   &c3    )
{      
    return;

    glNormalT(normal);

    glColor4fv(c1.ptr());
    glVertexNv<3>(p1.ptr());

    glColor4fv(c2.ptr());
    glVertexNv<3>(p2.ptr());

    glColor4fv(c3.ptr());
    glVertexNv<3>(p3.ptr());
}
  
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawTriangle(const Vector3 &p1, 
                               const Vector3 &p2, 
                               const Vector3 &p3,
                               const Vector3 &normal, 
                               const Vec4f   &c     )
{ 
    return;

    glNormalT(normal); 

    glColor4fv(c.ptr());

    glVertexNv<3>(p1.ptr());
    glVertexNv<3>(p2.ptr());
    glVertexNv<3>(p3.ptr());
}
  
//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawSphere(const Vector3 &p, float radius)
{ 
    return;

    std::cerr << "OSG2_DrawTool : drawSphere " << std::endl;

    glPushMatrix();

    glTranslated(p[0], p[1], p[2]);

    glutSolidSphere(radius, 32, 16);

    glPopMatrix();
}



//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::setMaterial(const Vec4f &colour, std::string)
{
    return;

    glColor4f(colour[0], colour[1], colour[2], colour[3]);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, &colour[0]);

    static const float emissive[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  emissive);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 20);

    if(colour[3] < 1)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(0);
    }
    else
    {
        glDisable(GL_BLEND);
        glDepthMask(1);
    }
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::resetMaterial(const Vec4f &colour, std::string)
{
    return;

    if(colour[3] < 1)
    {
        glDisable(GL_BLEND);
        glDepthMask(1);
    }
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::drawBoundingBox( 	const Vector3 &min, 
			const Vector3 &max )
{
//TODO
	return;
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::writeOverlayText( 	int x, 
					int y, 
					unsigned fontSize, 
					const Vec4f &color, 
					const char* text )
{
//TODO
	return;
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::clear(void)
{
   
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::pushMatrix(void)
{
    return;

    glPushMatrix();
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::popMatrix(void)
{
    return;

    glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::multMatrix(float *glTransform)
{
    return;

    glMultMatrix(glTransform);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::scale(float s)
{
    return;

    glScale(s, s, s);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::setLightingEnabled(bool bIsEnabled)
{
    return;

    _bLightEnabled = bIsEnabled;

    if(this->getLightEnabled() == true) 
        glEnable(GL_LIGHTING);
    else 
        glDisable(GL_LIGHTING);
}

//////////////////////////////////////////////////////////////////////////////

void OSGDrawTool::setPolygonMode(int iMode, bool bWireframe)
{
    return;

    _iPolygonMode      = iMode;
    _bWireFrameEnabled = bWireframe;

    if(_iPolygonMode == 0) 
    {
        if(_bWireFrameEnabled == true) 
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if(_iPolygonMode == 1)
    {
        if(_bWireFrameEnabled == true) 
            glPolygonMode(GL_FRONT, GL_LINE);
        else
            glPolygonMode(GL_FRONT, GL_FILL);
    }
    else if(_iPolygonMode == 2)
    {
        if(_bWireFrameEnabled == true) 
            glPolygonMode(GL_BACK, GL_LINE);
        else
            glPolygonMode(GL_BACK, GL_FILL);
    }
}

END_SOFA_CORE_VISUAL_NAMESPACE
