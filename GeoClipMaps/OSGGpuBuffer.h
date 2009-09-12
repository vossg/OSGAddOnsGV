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

#ifndef _OSGGPUBUFFER_H_
#define _OSGGPUBUFFER_H_

#include "OSGWindow.h"

OSG_BEGIN_NAMESPACE

enum BufferUsage
{
    BufferUsage_Static_WriteOnly,
    BufferUsage_Static_ReadWrite,
    BufferUsage_Stream_WriteOnly,
    BufferUsage_Stream_ReadWrite,
    BufferUsage_Dynamic_WriteOnly,
    BufferUsage_Dynamic_ReadWrite,
    BufferUsage_Count
};

enum BufferLockOption
{
    BufferLockOption_ReadWrite,
    BufferLockOption_Discard,
    BufferLockOption_ReadOnly,
    BufferLockOption_NoOverwrite,
    BufferLockOption_Count
};


struct VboFunctions;

class GpuBuffer
{
  public:
    GpuBuffer();
    virtual				~GpuBuffer();
    
    bool				create(Window* window, 
                               GLenum type, 
                               int size, 
                               BufferUsage usage );
    void				destroy();
    
    bool				isValid() const;
    
    bool				uploadData(const void* sourceData, 
                                   int targetOffset, 
                                   int size );
    
    void*				lock(int offset, 
                             int length, 
                             BufferLockOption lockOption );
    bool				unlock();
    
    bool				isLocked() const;
    
    int					getSize() const;
    
    GLuint				getBufferId() const;
    GLenum				getBufferType() const;
    
    void				activate();
    void				deactivate();
    
  private:
    Window*				window_;
    GLenum				glType_;
    GLenum				glUsage_;
    GLuint				bufferId_;
    int					bufferSize_;
    BufferUsage			bufferUsage_;
    bool				isLocked_;
};

OSG_END_NAMESPACE

#endif /* _OSGGPUBUFFER_H_ */

