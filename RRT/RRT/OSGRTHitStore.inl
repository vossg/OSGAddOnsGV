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
RTHitStore<DescT>::RTHitStore(void) :
     Inherited      (    ),
    _uiNumHits      (0   ),
    _uiCurrentHit   (0   ),
    _uiAvailableHits(0   ),
    _uiServedHits   (0   ),

    _vHits          (    ),
    _vAvailableHits (    ),
    _pStoreLock     (NULL),
    _pCond          (NULL)
{
    _pStoreLock = Lock   ::get(NULL);
    _pCond      = CondVar::get(NULL);

    OSG::addRef(_pStoreLock);
    OSG::addRef(_pCond     );
}

template<typename DescT> inline
RTHitStore<DescT>::~RTHitStore(void)
{
    OSG::subRef(_pStoreLock);
    OSG::subRef(_pCond     );
}

template<typename DescT> inline
void RTHitStore<DescT>::startFrame(RTTarget &pTarget)
{
    if(_uiWidth != pTarget.getWidth() || _uiHeight != pTarget.getHeight())
    {
        fprintf(stderr, "Update HTS \n================================\n"); 

        Inherited::updateNumTiles(pTarget.getWidth(), 
                                  pTarget.getHeight(),
                                  SingleHitPacket::NumHHits,
                                  SingleHitPacket::NumVHits);


        _vHits.resize(_uiNumTiles);
    }

    _vAvailableHits.clear();

    _uiCurrentHit    = 0;
    _uiAvailableHits = 0;
    _uiServedHits    = 0;

    fprintf(stderr, "HS %d\n", _uiNumTiles);
}

template<typename DescT> inline
void RTHitStore<DescT>::wait(void)
{
    _pCond->acquire();
    _pCond->wait   ();
    _pCond->release();
}

template<typename DescT> inline
void RTHitStore<DescT>::signal(void)
{
    _pCond->signal();
}

template<typename DescT> inline
void RTHitStore<DescT>::broadcast(void)
{
    _pCond->broadcast();
}

template<typename DescT> inline
UInt32 RTHitStore<DescT>::getWriteIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    returnValue = _uiCurrentHit++;

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
UInt32 RTHitStore<DescT>::getReadIndex(void)
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
        if(_uiServedHits < _uiNumTiles)
        {
            returnValue = Waiting;
        }
    }

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
void RTHitStore<DescT>::pushWriteIndex(UInt32 uiIndex)
{
    _pStoreLock->acquire();

    ++_uiAvailableHits;

    _vAvailableHits.push_back(uiIndex);

    _pStoreLock->release();
}

template<typename DescT> inline
typename RTHitStore<DescT>::SingleHitPacket &
    RTHitStore<DescT>::getPacket(UInt32 uiIndex)
{
    return _vHits[uiIndex];
}

OSG_END_NAMESPACE
