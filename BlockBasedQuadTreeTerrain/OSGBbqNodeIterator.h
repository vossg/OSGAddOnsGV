/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
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
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------

#ifndef _OSGBBQNODEITERATOR_H_
#define _OSGBBQNODEITERATOR_H_

//-----------------------------------------------------------------------------

#include "OSGBbqNode.h"

#include <vector>
#include <utility>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

class BbqNodeIterator
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqNodeIterator(void);
    ~BbqNodeIterator(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void   startIteration(Int32 iNodeCount);
    bool   isFinished    (void            ) const;
    Real32 getProgress   (void            ) const;
    void   step          (void            );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    BbqNodeId getCurrentNode       (void) const;
    bool      isCurrentNodeBottomUp(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void popCurrentNode(void               );
    void pushNode      (BbqNodeId iId      );
    void pushChildNodes(BbqNodeId iParentId);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef std::vector< std::pair<BbqNodeId, 
                                   bool     > > ProcessStack;
    
    Int32        _iNodeCount;
    Int32        _iProcessedNodeCount;
    ProcessStack _sProcessStack;  

    /*==========================  PRIVATE  ================================*/

  private:
};


OSG_END_NAMESPACE


#endif // #ifdef BBQNODEITERATOR_INCLUDED

