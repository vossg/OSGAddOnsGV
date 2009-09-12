/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2003 by the OpenSG Forum                   *
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

OSG_BEGIN_NAMESPACE

template<typename DescT> inline
UInt32 RTPrimaryRayStore<DescT>::nextIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    if(_uiNumRays != 0)
    {
        _uiNumRays--;

        returnValue = _uiCurrentRay++;
    }

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
typename RTPrimaryRayStore<DescT>::SingleRayPacket &
    RTPrimaryRayStore<DescT>::getRayPacket(UInt32 uiRayIndex)
{
    return _vRays[uiRayIndex];
}


template<typename DescT> inline
RTPrimaryRayStore<DescT>::RTPrimaryRayStore(void) :
     Inherited   (    ),
    _uiNumRays   (   0),
    _uiCurrentRay(   0),

    _vRays       (    ),
    _pStoreLock  (NULL)
{
    _pStoreLock = Lock::get(NULL);

    OSG::addRef(_pStoreLock);
}

template<typename DescT> inline
RTPrimaryRayStore<DescT>::~RTPrimaryRayStore(void)
{
    OSG::subRef(_pStoreLock);
}

template<typename DescT> inline
void RTPrimaryRayStore<DescT>::startFrame(Camera &pCam, RTTarget &pTarget)
{
    _uiNumRays    = pTarget.getWidth() * pTarget.getHeight();
    _uiCurrentRay = 0;

    _vRays.resize(_uiNumRays);

    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    PerspectiveCamera *pPCam = dynamic_cast<PerspectiveCamera *>(&pCam);

    Matrix mCam;

    pPCam->getBeacon()->getToWorld(mCam);

    Real32 rVOff = atan(pPCam->getFov() / 2.f);
    Real32 rHOff = (rVOff * uiHeight) / uiWidth;

    Vec3f vRight(mCam[0][0], mCam[0][1], mCam[0][2]);
    Vec3f vUp   (mCam[1][0], mCam[1][1], mCam[1][2]);
    Vec3f vDir  (0.f, 0.f, -1.f);

    mCam.mult(vDir);

    vRight *= rVOff;
    vUp    *= rHOff;

//    vRight.setValues(0.f, 0.f, 0.f);
//    vUp.setValues   (0.f, 0.f, 0.f);

    Vec3f vTopLeft = vDir - vRight + vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    for(UInt32 i = 0; i < uiHeight; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiWidth; ++j)
        {
#if 0
            fprintf(stderr, "%d %d\n", j, i);
            fprintf(stderr, "%f %f %f\n", mCam[3][0], mCam[3][1], mCam[3][2]);
            fprintf(stderr, "%f %f %f\n", vCurrV[0], vCurrV[1], vCurrV[2]);
#endif

            SingleRayPacket &rayPacket = _vRays[i * uiWidth + j];

            rayPacket.setOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);
            rayPacket.setDirection(vCurrV);
            rayPacket.normalizeDirection();

            rayPacket.setXY(j, i);

            vCurrV += vRight;
        }
        
        vCurrH -= vUp;
    }

    fprintf(stderr, "RS %d\n", _uiNumRays);
}

OSG_END_NAMESPACE
