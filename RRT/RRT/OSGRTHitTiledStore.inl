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
void RTHitTiledStore<DescT>::wait(void)
{
    _pCond->acquire();
    _pCond->wait   ();
    _pCond->release();
}

template<typename DescT> inline
void RTHitTiledStore<DescT>::signal(void)
{
    _pCond->signal();
}

template<typename DescT> inline
void RTHitTiledStore<DescT>::broadcast(void)
{
    _pCond->broadcast();
}

template<typename DescT> inline
UInt32 RTHitTiledStore<DescT>::getWriteIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    returnValue = _uiCurrentHit++;

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
UInt32 RTHitTiledStore<DescT>::getReadIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    if(_uiAvailableHits != 0)
    {
        returnValue = _vAvailableHits.back();

        _vAvailableHits.pop_back();

        --_uiAvailableHits;
        ++_uiServedHits;
    }
    else
    {
        if(_uiServedHits < _uiExpectedHits)
        {
            returnValue = Waiting;
        }
    }

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
void RTHitTiledStore<DescT>::pushWriteIndex(UInt32 uiIndex)
{
    _pStoreLock->acquire();

    ++_uiAvailableHits;

    _vAvailableHits.push_back(uiIndex);

    _pStoreLock->release();
}

template<typename DescT> inline
typename RTHitTiledStore<DescT>::HitTile &
    RTHitTiledStore<DescT>::getPacket(UInt32 uiIndex)
{
    return _vHits[uiIndex];
}

template<typename DescT> inline
RTHitTiledStore<DescT>::RTHitTiledStore(void) :
     Inherited      (    ),
    _uiCurrentHit   (0   ),
    _uiAvailableHits(0   ),
    _uiServedHits   (0   ),
    _uiExpectedHits (0   ),

    _vHits          (    ),
    _vAvailableHits (    ),
    _pStoreLock     (NULL),
    _pCond          (NULL)
{
    _pStoreLock = Lock   ::get(NULL, false);
    _pCond      = CondVar::get(NULL, false);
}

template<typename DescT> inline
RTHitTiledStore<DescT>::~RTHitTiledStore(void)
{
    _pStoreLock = NULL;
    _pCond      = NULL;
}

template<typename DescT> inline
void RTHitTiledStore<DescT>::startFrame(RTTarget &pTarget,
                                        UInt32    uiNumRayPackets)
{
    if(_uiWidth != pTarget.getWidth() || _uiHeight != pTarget.getHeight())
    {
        fprintf(stderr, "Update HTS \n================================\n"); 

        Inherited::updateNumTiles(pTarget.getWidth(), 
                                  pTarget.getHeight(),
                                  HitTile::NumHPackets,
                                  HitTile::NumVPackets);


        _vHits.resize(_uiNumTiles);
    }

    _uiExpectedHits = uiNumRayPackets;

    OSG_ASSERT(_uiExpectedHits <= _uiNumTiles);

    _vAvailableHits.clear();

    _uiCurrentHit    = 0;
    _uiAvailableHits = 0;
    _uiServedHits    = 0;
}

OSG_END_NAMESPACE
