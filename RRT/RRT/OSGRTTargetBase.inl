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
 **     class RTTarget!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

OSG_BEGIN_NAMESPACE


//! access the type of the class
inline
OSG::FieldBundleType &RTTargetBase::getClassType(void)
{
    return _type;
}

//! access the numerical type of the class
inline
OSG::UInt32 RTTargetBase::getClassTypeId(void)
{
    return _type.getId();
}

inline
OSG::UInt16 RTTargetBase::getClassGroupId(void)
{
    return _type.getGroupId();
}

/*------------------------------ get -----------------------------------*/

//! Get the value of the RTTarget::_sfWidth field.

inline
UInt32 &RTTargetBase::editWidth(void)
{
    editSField(WidthFieldMask);

    return _sfWidth.getValue();
}

//! Get the value of the RTTarget::_sfWidth field.
inline
const UInt32 &RTTargetBase::getWidth(void) const
{
    return _sfWidth.getValue();
}

#ifdef OSG_1_GET_COMPAT
inline
UInt32              &RTTargetBase::getWidth          (void)
{
    return this->editWidth          ();
}
#endif

//! Set the value of the RTTarget::_sfWidth field.
inline
void RTTargetBase::setWidth(const UInt32 &value)
{
    editSField(WidthFieldMask);

    _sfWidth.setValue(value);
}
//! Get the value of the RTTarget::_sfHeight field.

inline
UInt32 &RTTargetBase::editHeight(void)
{
    editSField(HeightFieldMask);

    return _sfHeight.getValue();
}

//! Get the value of the RTTarget::_sfHeight field.
inline
const UInt32 &RTTargetBase::getHeight(void) const
{
    return _sfHeight.getValue();
}

#ifdef OSG_1_GET_COMPAT
inline
UInt32              &RTTargetBase::getHeight         (void)
{
    return this->editHeight         ();
}
#endif

//! Set the value of the RTTarget::_sfHeight field.
inline
void RTTargetBase::setHeight(const UInt32 &value)
{
    editSField(HeightFieldMask);

    _sfHeight.setValue(value);
}

//! create a new instance of the class
inline
RTTargetP RTTargetBase::create(void)
{
    RTTargetP fc;

    if(getClassType().getPrototype() != NULL)
    {
        fc = dynamic_cast<RTTarget::ObjPtr>(
            getClassType().getPrototype()-> shallowCopy());
    }

    return fc;
}



inline
Char8 *RTTargetBase::getClassname(void)
{
    return "RTTarget";
}

OSG_GEN_BUNDLEP(RTTarget);

OSG_END_NAMESPACE

