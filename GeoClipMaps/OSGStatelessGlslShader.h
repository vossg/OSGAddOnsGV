/*---------------------------------------------------------------------------*\
*                                OpenSG                                     *
*                                                                           *
*                                                                           *
*               Copyright (C) 2000-2002 by the OpenSG Forum                 *
*                                                                           *
*                            www.opensg.org                                 *
*                                                                           *
*   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
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

#ifndef _OSGSTATELESSGLSLSHADER_H_
#define _OSGSTATELESSGLSLSHADER_H_

#include "OSGWindow.h"
#include <string>

OSG_BEGIN_NAMESPACE

typedef unsigned int GLhandleARB;

struct GlslFunctions;

class GlslShader
{
  public:
    GlslShader();
    ~GlslShader();
    
    bool				create(Window* window, 
                               const char* vertexShaderText, 
                               const char* fragmentShaderText, 
                               std::string& errorMsg );
    void				destroy();
    
    bool				createFromFile(Window* window, 
                                       const char* shaderFilename );
    
    bool				isValid() const;
    
    void				activate();
    void				deactivate();
    
    bool				setUniform( const char* name, const Pnt2f& value );
    bool				setUniform( const char* name, const Pnt3f& value );
    bool				setUniform( const char* name, const Pnt4f& value );
    bool				setUniform( const char* name, const Matrix4f& value, 
                                    bool transpose = false );
    bool				setUniform( const char* name, const Color3f& value );
    bool				setUniform( const char* name, float value );
    
    bool				setSampler( const char* name, GLint stage );
    
  private:
    GLhandleARB			compileShader( GLenum type, 
                                       const char* text, 
                                       std::string& errorMsg );
    
    GlslFunctions*		extensionFunctions_;
    
    GLhandleARB			vertexShader_;
    GLhandleARB			fragmentShader_;
    GLhandleARB			program_;
};

OSG_END_NAMESPACE

#endif // #ifdef  _OSGSTATELESSGLSLSHADER_H_

