/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

/*****************************************************************************\
 *****************************************************************************
 **                                                                         **
 **                  This file is automatically generated.                  **
 **                                                                         **
 **          Any changes made to this file WILL be lost when it is          **
 **           regenerated, which can become necessary at any time.          **
 **                                                                         **
 **     Do not change this file, changes should be done in the derived      **
 **     class RTTextureTarget!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

OSG_BEGIN_NAMESPACE


//! access the type of the class
inline
OSG::FieldBundleType &RTTextureTargetBase::getClassType(void)
{
    return _type;
}

//! access the numerical type of the class
inline
OSG::UInt32 RTTextureTargetBase::getClassTypeId(void)
{
    return _type.getId();
}

inline
OSG::UInt16 RTTextureTargetBase::getClassGroupId(void)
{
    return _type.getGroupId();
}

/*------------------------------ get -----------------------------------*/


//! Get the value of the RTTextureTarget::_sfTexObjChunk field.
inline
TextureObjChunkPtrConst RTTextureTargetBase::getTexObjChunk(void) const
{
    return _sfTexObjChunk.getValue();
}

//! Set the value of the RTTextureTarget::_sfTexObjChunk field.
inline
void RTTextureTargetBase::setTexObjChunk(TextureObjChunkPtrConstArg value)
{
    editSField(TexObjChunkFieldMask);

    setRefd(_sfTexObjChunk.getValue(), value);

}

//! create a new instance of the class
inline
RTTextureTargetP RTTextureTargetBase::create(void)
{
    RTTextureTargetP fc;

    if(getClassType().getPrototype() != NULL)
    {
        fc = dynamic_cast<RTTextureTarget::ObjPtr>(
            getClassType().getPrototype()-> shallowCopy());
    }

    return fc;
}



inline
Char8 *RTTextureTargetBase::getClassname(void)
{
    return "RTTextureTarget";
}

OSG_GEN_BUNDLEP(RTTextureTarget);

OSG_END_NAMESPACE

