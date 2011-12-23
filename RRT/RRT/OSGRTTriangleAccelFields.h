/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2003 by the OpenSG Forum                          *
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

#ifndef _OSGRTTRIANGLEACCELFIELDS_H_
#define _OSGRTTRIANGLEACCELFIELDS_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGMField.h"
#include "OSGSField.h"
#include "OSGRTTriangleAccel.h"
#include "OSGBoxVolume.h"

OSG_BEGIN_NAMESPACE

template <>
struct FieldTraits<RTTriAccelBarycentric> : 
    public FieldTraitsTemplateBase<RTTriAccelBarycentric>
{
  private:

    static  DataType                   _type;

  public:

    typedef FieldTraits<RTTriAccelBarycentric>  Self;

    enum                  { Convertible = (Self::FromStringConvertible |
                                           Self::ToStreamConvertible   )     };

    static OSG_CONTRIBRRT_DLLMAPPING
                 DataType     &getType     (void);

    static const Char8        *getPName    (void)
    {
        return NULL;
    }

    static const Char8        *getSName    (void) 
    {
        return "SFRTTriAccelBarycentric"; 
    }

    static const Char8        *getMName    (void) 
    { 
        return "MFRTTriAccelBarycentric"; 
    }

    static const RTTriAccelBarycentric getDefault  (void) 
    {
        return RTTriAccelBarycentric();   
    }


    static bool getFromCString(      RTTriAccelBarycentric  &outVal,
                               const Char8                 *&inVal)
    {
        return outVal.getFromCString(inVal);
    }


    static void putToStream(const RTTriAccelBarycentric &val,
                                  OutStream             &str)
    {
        str << "\"";
        val.putToStream(str);
        str << "\"";
    }

    static SizeT  getBinSize(const RTTriAccelBarycentric &oObject)
    {
        return oObject.getBinSize();
    }


    static SizeT  getBinSize(const RTTriAccelBarycentric *pObjectStore,
                                   SizeT                  uiNumObjects)
    {
        SizeT size = 0;

        for(SizeT i = 0; i < uiNumObjects; ++i)
        {
            size += getBinSize(pObjectStore[i]);
        }
        return size;
    }

    static void copyToBin(      BinaryDataHandler     &pMem, 
                          const RTTriAccelBarycentric &oObject)
    {
        oObject.copyToBin(pMem);

    }

    static void copyToBin(      BinaryDataHandler     &pMem, 
                          const RTTriAccelBarycentric *pObjectStore,
                                SizeT                  uiNumObjects)
    {
        for(SizeT i = 0; i < uiNumObjects; ++i)
        {
            copyToBin(pMem, pObjectStore[i]);
        }
    }

    static void copyFromBin(BinaryDataHandler     &pMem, 
                            RTTriAccelBarycentric &oObject)
    {
        oObject.copyFromBin(pMem);
    }

    static void copyFromBin(BinaryDataHandler     &pMem, 
                            RTTriAccelBarycentric *pObjectStore,
                            SizeT                  uiNumObjects)
    {
        for(SizeT i = 0; i < uiNumObjects; ++i)
        {
            copyFromBin(pMem, pObjectStore[i]);
        }
    }
};


typedef MField<RTTriAccelBarycentric> MFRTTriAccelBarycentric;


OSG_END_NAMESPACE

#endif /* _OSGMFIELD_H_ */
