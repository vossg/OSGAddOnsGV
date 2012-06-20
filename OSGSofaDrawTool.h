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

#ifndef _OSGSOFADRAWTOOL_H_
#define _OSGSOFADRAWTOOL_H_

//#include "initPlugin.h"
#include <sofa/core/visual/DrawTool.h>
#include <sofa/defaulttype/Vec.h>

#include "OSGContribSOFADef.h"

BEGIN_SOFA_CORE_VISUAL_NAMESPACE
      
class OSG_CONTRIBSOFA_DLLMAPPING OSGDrawTool : public DrawTool 
{

  public:

    typedef sofa::defaulttype::Vec4f       Vec4f;
    typedef sofa::defaulttype::Vec3f       Vec3f;
    typedef sofa::defaulttype::Vector3     Vector3;
    typedef sofa::defaulttype::Vec<3, int> Vec3i;
    typedef sofa::defaulttype::Vec<2, int> Vec2i;
    typedef sofa::defaulttype::Quaternion  Quaternion;


    OSGDrawTool(void);
    virtual ~OSGDrawTool(void);    
     
    virtual void drawPoints       (const std::vector<Vector3> &points, 
                                         float                 size,  
                                   const Vec4f                 colour         );
    
    virtual void drawLines        (const std::vector<Vector3> &points, 
                                         float                 size, 
                                   const Vec4f                 colour         );
    virtual void drawLines        (const std::vector<Vector3> &points, 
                                   const std::vector<Vec2i  > &index, 
                                         float                 size, 
                                   const Vec4f                 colour         );
    
    virtual void drawTriangle(const Vector3&, const Vector3&, const Vector3&, const Vector3&);

    virtual void drawTriangles    (const std::vector<Vector3> &points, 
                                   const Vec4f                 colour         );
    virtual void drawTriangles    (const std::vector<Vector3> &points, 
                                   const Vector3               normal, 
                                   const Vec4f                 colour         );
    virtual void drawTriangles    (const std::vector<Vector3> &points, 
                                   const std::vector<Vec3i  > &index, 
                                   const std::vector<Vector3> &normal,
                                   const Vec4f                 colour         );
    virtual void drawTriangles    (const std::vector<Vector3> &points,
                                   const std::vector<Vector3> &normal,
                                   const std::vector<Vec4f  > &colour         );

    virtual void drawTriangleStrip(const std::vector<Vector3> &points,
                                   const std::vector<Vector3> &normal,
                                   const Vec4f                colour          );
    
    virtual void drawTriangleFan  (const std::vector<Vector3> &points,
                                   const std::vector<Vector3> &normal,
                                   const Vec4f                 colour         );
    
    virtual void drawFrame        (const Vector3              &position, 
                                   const Quaternion           &orientation, 
                                   const Vec3f                &size           );

    virtual void drawSpheres      (const std::vector<Vector3> &points, 
                                   const std::vector<float  > &radius, 
                                   const Vec4f                 colour         );
    virtual void drawSpheres      (const std::vector<Vector3> &points, 
                                         float                 radius, 
                                   const Vec4f                 colour         );
    
    virtual void drawCone         (const Vector3              &p1, 
                                   const Vector3              &p2, 
                                         float                 radius1, 
                                         float                 radius2, 
                                   const Vec4f                 colour, 
                                         int                   subd   = 16    );
    
    virtual void drawCube         (const float                &radius, 
                                   const Vec4f                &colour, 
                                   const int                  &subd   = 16    );
    
    virtual void drawCylinder     (const Vector3              &p1, 
                                   const Vector3              &p2, 
                                         float                 radius, 
                                   const Vec4f                 colour,  
                                         int                   subd   = 16    );
    
    virtual void drawArrow        (const Vector3              &p1, 
                                   const Vector3              &p2, 
                                         float                 radius, 
                                   const Vec4f                 colour,  
                                         int                   subd   = 16    );

    virtual void drawPlus         (const float                &radius, 
                                   const Vec4f                &colour, 
                                   const int                  &subd   = 16    );

    virtual void drawPoint        (const Vector3              &p, 
                                   const Vec4f                &c);
    virtual void drawPoint        (const Vector3              &p, 
                                   const Vector3              &n, 
                                   const Vec4f                &c);
   
    virtual void drawTriangle     (const Vector3              &p1,
                                   const Vector3              &p2,
                                   const Vector3              &p3,
                                   const Vector3              &normal, 
                                   const Vec4f                &c);
    virtual void drawTriangle     (const Vector3              &p1,
                                   const Vector3              &p2,
                                   const Vector3              &p3,
                                   const Vector3              &normal,
                                   const Vec4f                &c1, 
                                   const Vec4f                &c2, 
                                   const Vec4f                &c3             );

    virtual void drawSphere       (const Vector3              &p, 
                                         float                 radius         );
   
    virtual void clear            (      void                                 );
   
    virtual void setMaterial      (const Vec4f                &colour, 
                                         std::string           name = 
                                                                 std::string());
   
    virtual void resetMaterial    (const Vec4f                &colour, 
                                         std::string           name =
                                                                 std::string());

    virtual void pushMatrix       (      void                                 );
    virtual void popMatrix        (      void                                 );
    virtual void multMatrix       (      float                *glTransform    );
    virtual void scale            (      float                 s              );

  protected:

    bool _bLightEnabled;
    int  _iPolygonMode;      // 0: no cull, 
                             // 1 front (CULL_CLOCKWISE), 
                             // 2 back (CULL_ANTICLOCKWISE)
    bool _bWireFrameEnabled;

    
  public:

    // getter & setter
    virtual void setLightingEnabled (bool bIsEnabled);
            bool getLightEnabled    (void           );
    
    virtual void setPolygonMode     (int  iMode, 
                                     bool bWireframe);
            int  getPolygonMode     (void           );
    
            bool getWireFrameEnabled(void           );

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGDrawTool(const OSGDrawTool &source);
    void operator =(const OSGDrawTool &source);
};

END_SOFA_CORE_VISUAL_NAMESPACE

#include "OSGSofaDrawTool.inl"

#endif // _OSGSOFADRAWTOOL_H_
