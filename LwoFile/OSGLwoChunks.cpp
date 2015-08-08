/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                Copyright (C) 2008 by the OpenSG Forum                     *
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

#include "OSGLwoChunks.h"

#include "OSGSingletonHolder.ins"

#include "OSGGroup.h"
#include "OSGGeometry.h"
#include "OSGGeoFunctions.h"
#include "OSGChunkMaterial.h"
#include "OSGTwoSidedLightingChunk.h"

#if 0
#include "OSGRangeLOD.h"
#include "OSGTransform.h"
#include "OSGTypedGeoIntegralProperty.h"

#include "OSGImageFileHandler.h"
#include "OSGSceneFileHandler.h"


#include "OSGBlendChunk.h"
#include "OSGDepthChunk.h"
#include "OSGPolygonChunk.h"
#include "OSGTextureEnvChunk.h"

#endif

#include "OSGLwoSceneFileType.h"
#include "OSGLwoDatabase.h"

OSG_BEGIN_NAMESPACE

OSG_SINGLETON_INST(LwoChunkFactoryBase, addPostFactoryExitFunction)

template class SingletonHolder<LwoChunkFactoryBase>;

//---------------------------------------------------------------------
// LwoChunkHeader
//---------------------------------------------------------------------

std::map<UInt32, UInt32> LwoChunkHeader::_mTagLengthSizeMap;

LwoChunkHeader::LwoChunkHeader(void) :
    uiChunkTag  (0),
    uiLength    (0),
    uiLengthSize(0),
    uiHeaderSize(0)
{
    if(_mTagLengthSizeMap.size() == 0)
    {
        _mTagLengthSizeMap[lwo::tag2int("FORM")] = 4;

        _mTagLengthSizeMap[lwo::tag2int("TAGS")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("LAYR")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("PNTS")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("BBOX")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("POLS")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("PTAG")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("CLIP")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("SURF")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("VMAP")] = 4;

        
        //Clip Subchunks
        _mTagLengthSizeMap[lwo::tag2int("STIL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ISEQ")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ANIM")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("XREF")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("STCC")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TIME")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CONT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("BRIT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SATR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("HUE ")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GAMM")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("NEGA")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("IFLT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("PFLT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("FLAG")] = 2;


        //Surf Subchunks
        _mTagLengthSizeMap[lwo::tag2int("COLR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("DIFF")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("LUMI")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SPEC")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("REFL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TRAN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TRNL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GLOS")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SHRP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("BUMP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SIDE")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SMAN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RFOP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RIMG")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RSAN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RIND")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TROP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TIMG")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CLRH")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CLRF")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ADTR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GLOW")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GVAL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("LINE")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ALPH")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RBLR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("BLOK")] = 2;

        //Surf block header
        _mTagLengthSizeMap[lwo::tag2int("IMAP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("PROC")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GRAD")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SHDR")] = 2;

        // Surf block subchunks
        _mTagLengthSizeMap[lwo::tag2int("CHAN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ENAB")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("OPAC")] = 2;

        // Surf block tmap subchunks
        _mTagLengthSizeMap[lwo::tag2int("TMAP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CNTR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("SIZE")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("ROTA")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("OREF")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("FALL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CSYS")] = 2;

        // Surf block imap
        _mTagLengthSizeMap[lwo::tag2int("PROJ")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("AXIS")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("IMAG")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("WRAP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("WRPW")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("WRPH")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("AAST")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("PIXB")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("STCK")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TAMP")] = 2;
        
        // Surf block proc
        _mTagLengthSizeMap[lwo::tag2int("VALU")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("FUNC")] = 2;
        
        // Surf grad block
        _mTagLengthSizeMap[lwo::tag2int("PNAM")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("INAM")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GRST")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GREN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("GRPT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("FKEY")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("IKEY")] = 2;



        // lwob

        _mTagLengthSizeMap[lwo::tag2int("SRFS")] = 4;
        _mTagLengthSizeMap[lwo::tag2int("VDIF")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("VSPC")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("CTEX")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TWRP")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TFLG")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TSIZ")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TCTR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TCLR")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("VRFL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("RFLT")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("VTRN")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TTEX")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TAAS")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TVAL")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TFP0")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("TFP1")] = 2;
        _mTagLengthSizeMap[lwo::tag2int("BTEX")] = 2;

    }

//    _mTagLengthSizeMap[lwo::tag2int("TXUV")] = 4;
//    _mTagLengthSizeMap[lwo::tag2int("_UV_")] = 2;
}

LwoChunkHeader::~LwoChunkHeader(void)
{
}

//---------------------------------------------------------------------
// LwoChunkFactoryBase
//---------------------------------------------------------------------

LwoChunkFactoryBase::RegisterChunk::RegisterChunk(CreateChunk fCreate,
                                                  UInt32      uiChunkTag)
{
    LwoChunkFactory::the()->registerChunk(fCreate, uiChunkTag);
}


LwoChunkFactoryBase::LwoChunkFactoryBase(void) :
    _mRegisteredChunks()
{
}

LwoChunkFactoryBase::~LwoChunkFactoryBase(void)
{
}

void LwoChunkFactoryBase::registerChunk(CreateChunk fHelper,
                                        UInt32      uiChunkTag)
{
    if(fHelper == NULL)
        return;

    NameChunkCreateMap::iterator mChunkIt =
        _mRegisteredChunks.find(uiChunkTag);


    if(mChunkIt == _mRegisteredChunks.end())
    {
        _mRegisteredChunks[uiChunkTag] = fHelper;

        PINFO << "Chunk registered for "
              << uiChunkTag
              << std::endl;
    }
    else
    {
        PWARNING << "Chunk already registered for %s "
                 << uiChunkTag
                 << std::endl;
    }
}

LwoChunkTransitPtr LwoChunkFactoryBase::createChunk(
    const LwoChunkHeader &oHeader)
{
    NameChunkCreateMap::iterator mChunkIt =
        _mRegisteredChunks.find(oHeader.uiChunkTag);

    LwoChunkTransitPtr returnValue(NULL);

    if(mChunkIt != _mRegisteredChunks.end())
    {
        returnValue = (*mChunkIt).second(oHeader);
    }
    else
    {
        returnValue = new LwoUnknownChunk(oHeader);
    }

    return returnValue;
}

//---------------------------------------------------------------------
// LwoChunk
//---------------------------------------------------------------------

void LwoChunk::readChar8(std::istream &is, Char8  *cVal, UInt32 iSize)
{
    is.read(cVal, iSize);
}

template<class ValueT>
UInt32 LwoChunk::readVal(std::istream &is, ValueT &val)
{
    is.read(reinterpret_cast<char *>(&val), sizeof(ValueT));

    val = osgNetToHost(val);

    return sizeof(ValueT);
}

template<>
UInt32 LwoChunk::readVal<Int8>(std::istream &is, Int8 &val)
{
    is.read(reinterpret_cast<char *>(&val), sizeof(Int8));

    return sizeof(Int8);
}

template<>
UInt32 LwoChunk::readVal<UInt8>(std::istream &is, UInt8 &val)
{
    is.read(reinterpret_cast<char *>(&val), sizeof(UInt8));

    return sizeof(UInt8);
}

template<>
UInt32 LwoChunk::readVal<Real32>(std::istream &is, Real32 &val)
{
    UInt32 uiTmp;

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val = osgNetToHostFP(uiTmp);

    return sizeof(Real32);
}

template<>
UInt32 LwoChunk::readVal<Pnt3f>(std::istream &is, Pnt3f &val)
{
#if 0
    is.read(reinterpret_cast<char *>(&(val[0])), sizeof(Real32));
    is.read(reinterpret_cast<char *>(&(val[1])), sizeof(Real32));
    is.read(reinterpret_cast<char *>(&(val[2])), sizeof(Real32));

    val[0] = osgNetToHost(val[0]);
    val[1] = osgNetToHost(val[1]);
    val[2] = osgNetToHost(val[2]);
#endif

    UInt32 uiTmp;

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[0] = osgNetToHostFP(uiTmp);

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[1] = osgNetToHostFP(uiTmp);

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[2] = osgNetToHostFP(uiTmp);

    return 3 * sizeof(Real32);
}

template<>
UInt32 LwoChunk::readVal<Color3f>(std::istream &is, Color3f &val)
{
#if 0
    is.read(reinterpret_cast<char *>(&(val[0])), sizeof(Real32));
    is.read(reinterpret_cast<char *>(&(val[1])), sizeof(Real32));
    is.read(reinterpret_cast<char *>(&(val[2])), sizeof(Real32));

    val[0] = osgNetToHost(val[0]);
    val[1] = osgNetToHost(val[1]);
    val[2] = osgNetToHost(val[2]);
#endif

    UInt32 uiTmp;

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[0] = osgNetToHostFP(uiTmp);

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[1] = osgNetToHostFP(uiTmp);

    is.read(reinterpret_cast<char *>(&uiTmp), sizeof(UInt32));

    val[2] = osgNetToHostFP(uiTmp);

    return 3 * sizeof(Real32);
}

UInt32 LwoChunk::readString(std::istream &is, 
                            std::string  &val, 
                            UInt32        uiMaxSize)
{
    static std::string tmpBuf;

    tmpBuf.resize(uiMaxSize);

    is.getline(&(tmpBuf[0]), uiMaxSize, '\0');

    UInt32 rc = is.gcount();

    tmpBuf[rc] = '\0';

    val = tmpBuf.substr(0, rc);

    if(0x0000 != (rc & 0x0001))
    {
        Char8  tmp;
            
        is.get(tmp);

        ++rc;
    }

    return rc;
}

UInt32 LwoChunk::readVarIndexVal(std::istream &is,  UInt32 &val)
{
    UInt32 returnValue = 0;

    UInt16 uiPart1     = 0;
    UInt16 uiPart2     = 0;

    returnValue += this->readVal(is, uiPart1);

    if(0xFF00 == (uiPart1 & 0xFF00))
    {
        returnValue += this->readVal(is, uiPart2);
                        
        val = (((uiPart1 & 0x00FF) << 16) | uiPart2);
    }
    else
    {
        val = uiPart1;
    }

    return returnValue;
}

LwoChunk::LwoChunk(const LwoChunkHeader &oHeader) :
     Inherited(                ),
    _uiLength (oHeader.uiLength)
{
}

LwoChunk::~LwoChunk(void)
{
}

bool LwoChunk::read(std::istream &is, LwoDatabase &oDB)
{
    static std::vector<char> tmpBuf;

    tmpBuf.resize(_uiLength);

    is.read(&(tmpBuf.front()), _uiLength);

    return is.good();
}

bool LwoChunk::handle(      std::istream   &is,
                            LwoDatabase    &oDB,
                      const LwoChunkHeader &oHeader)
{
    return false;
}

bool LwoChunk::addChild(LwoChunk *pChild)
{
    if(pChild == NULL)
        return false;

#if 0
          UInt16  uiCurrOpCode  = this  ->getOpCode(             );
    const Char8  *szCurrDesc    = this  ->findDesc (uiCurrOpCode );
          UInt16  uiChildOpCode = pChild->getOpCode(             );
    const Char8  *szChildDesc   = this  ->findDesc (uiChildOpCode);

    fprintf(stderr, "Add %hu (%p) (%s) to %hu (%p) (%s)\n",
            uiChildOpCode,
            static_cast<void *>(pChild),
            szChildDesc,
            uiCurrOpCode,
            static_cast<void *>(this),
            szCurrDesc);
#endif

    return true;
}

bool LwoChunk::isLayer(void)
{
    return false;
}

bool LwoChunk::isSubChunkContainer(void)
{
    return false;
}

NodeTransitPtr LwoChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr returnValue(NULL);

    return returnValue;
}


#if 0
const Char8 *OFRecord::findDesc(UInt16 sOpCode)
{
    OFOpCodeDesc *pDesc = aOpCodeDescs;

    while(pDesc->sOpCode != 0)
    {
        if(pDesc->sOpCode == sOpCode)
        {
            break;
        }

        ++pDesc;
    }

    return pDesc->szDesc;
}
#endif

void LwoChunk::dump(UInt32 uiIndent)
{
}

//---------------------------------------------------------------------
// LwoSubChunkContainer
//---------------------------------------------------------------------


LwoSubChunkContainer::LwoSubChunkContainer(const LwoChunkHeader &oHeader) :
     Inherited(oHeader)
{
}

LwoSubChunkContainer::~LwoSubChunkContainer(void)
{
}

bool LwoSubChunkContainer::isSubChunkContainer(void)
{
    return true;
}

//---------------------------------------------------------------------
// LwoSubChunkContainer
//---------------------------------------------------------------------


LwoBlockChunk::LwoBlockChunk(const LwoChunkHeader &oHeader) :
     Inherited    (oHeader),
    _uiReadSoFar  (0      ),
    _uiBlockLength(0      ),
    _szOrdinal    (       )
{
}

LwoBlockChunk::~LwoBlockChunk(void)
{
}

UInt32 LwoBlockChunk::handleData(      std::istream   &is,
                                       LwoDatabase    &oDB,
                                 const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = 0;

    switch(oHeader.uiChunkTag)
    {
        case LwoSurfChunk::OpCodeImap:
        case LwoSurfChunk::OpCodeProc:
        case LwoSurfChunk::OpCodeGrad:
        case LwoSurfChunk::OpCodeShdr:
        {
            fprintf(stderr, "handle imap data %d\n", _uiLength);

            uiRead += Inherited::readString(is, _szOrdinal, _uiLength);

            fprintf(stderr, "  ordinal : %s (%d)\n", 
                    _szOrdinal.c_str(),
                    uiRead);
        }
        break;

        case LwoSurfChunk::OpCodeChan:
        {
            UInt32 uiChan;

            uiRead += Inherited::readVal(is, uiChan);

            Char8 szTag[5];

            lwo::int2tag(szTag, uiChan);

            fprintf(stderr, "channel : %s\n", szTag);
        }
        break;

        case LwoSurfChunk::OpCodeEnab:
        {
            UInt16 uiEnabled;
            
            uiRead += Inherited::readVal(is, uiEnabled);

            fprintf(stderr, "enabled : %d\n", UInt32(uiEnabled));
        }
        break;

        case LwoSurfChunk::OpCodeOpac:
        {
            UInt16 uiType;
            Real32 fOpacity;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, uiType);
            uiRead += Inherited::readVal        (is, fOpacity);
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            fprintf(stderr, "opa : %d %f\n",
                    UInt32(uiType),
                    fOpacity);
        }
        break;

        case LwoSurfChunk::OpCodeNega:
        {
            UInt16 uiNega;
            
            uiRead += Inherited::readVal(is, uiNega);

            fprintf(stderr, "nega : %d\n", UInt32(uiNega));
        }
        break;

        default:
            break;
    };

    return uiRead;
}

void LwoBlockChunk::setBlockLength(UInt32 uiLength)
{
    _uiBlockLength = uiLength;
}

bool LwoBlockChunk::done(void)
{
    return _uiReadSoFar == _uiBlockLength;
}


//---------------------------------------------------------------------
// LwoHeaderChunk
//---------------------------------------------------------------------

LwoHeaderChunk::LwoHeaderChunk(const LwoChunkHeader &oHeader) :
     Inherited (oHeader),
    _vChildren (       ),
    _uiFileType(0x0000 )
{
}

LwoHeaderChunk::~LwoHeaderChunk(void)
{
    for(UInt32 i = 0; i < _vChildren.size(); ++i)
    {
        _vChildren[i] = NULL;
    }

    _vChildren.clear();
}

bool LwoHeaderChunk::read(std::istream &is, LwoDatabase &)
{
    UInt32 uiLwoTag;

    Inherited::readVal(is, uiLwoTag);

    char szTag[5];

    lwo::int2tag(szTag, uiLwoTag);

    fprintf(stderr, "got lwo tag : %s\n", szTag);

    if(uiLwoTag == lwo::tag2int("LWO2"))
    {
        _uiFileType = LwoDatabase::Lwo2File;
    }
    else if(uiLwoTag == lwo::tag2int("LWOB"))
    {
        _uiFileType = LwoDatabase::LwobFile;
    }

    return is.good() && (_uiFileType != 0x0000);
}

bool LwoHeaderChunk::addChild(LwoChunk *pChild)
{
    if(pChild == NULL)
    {
        return false;
    }

    return true;
}

UInt32 LwoHeaderChunk::getOpCode(void)
{
    return OpCode;
}

UInt32 LwoHeaderChunk::getFileType(void)
{
    return _uiFileType;
}

NodeTransitPtr LwoHeaderChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr returnValue(NULL);

#if 0
    if(_vChildren.size() != 0)
    {
        returnValue = Node::create();

        returnValue->setCore(Group::create());

        NodeTransitPtr pChild;

        for(UInt32 i = 0; i < _vChildren.size(); ++i)
        {
            pChild = _vChildren[i]->convertToNode(oDB);

            if(pChild != NULL)
            {
                returnValue->addChild(pChild);
            }
            else
            {
                FFATAL(("OFHeaderRecord::convertToNode: "
                        "No child for record [%u - %s].\n",
                        _vChildren[i]->getOpCode(),
                        findDesc(_vChildren[i]->getOpCode())));
            }
        }
    }
#endif

    return returnValue;
}

void LwoHeaderChunk::dump(UInt32 uiIndent)
{
#if 0
    indentLog(uiIndent, PLOG);
    PLOG << "HeaderRecord" << std::endl;

    indentLog(uiIndent, PLOG);
    PLOG << "{" << std::endl;

    uiIndent += 2;

    _pVertexPal  ->dump(uiIndent);
    _pTexturePal ->dump(uiIndent);
    _pMaterialPal->dump(uiIndent);

    uiIndent -= 2;

    indentLog(uiIndent, PLOG);
    PLOG << "}" << std::endl;

    indentLog(uiIndent, PLOG);
    PLOG << "[" << std::endl;

    uiIndent += 2;

    for(UInt32 i = 0; i < _vChildren.size(); ++i)
    {
        _vChildren[i]->dump(uiIndent);
    }

    uiIndent -= 2;

    indentLog(uiIndent, PLOG);
    PLOG << "]" << std::endl;
#endif
}


//---------------------------------------------------------------------
// LwoUnknownChunk
//---------------------------------------------------------------------

LwoUnknownChunk::LwoUnknownChunk(const LwoChunkHeader &oHeader) :
     Inherited(oHeader           ),
    _uiOpCode (oHeader.uiChunkTag),
    _vChildren(                  )
{
}

LwoUnknownChunk::~LwoUnknownChunk(void)
{
}

bool LwoUnknownChunk::read(std::istream &is, LwoDatabase &oDB)
{
    static std::vector<char> tmpBuf;

    tmpBuf.resize(_uiLength);

    is.read(&(tmpBuf.front()), _uiLength);

    fprintf(stderr, "== unknown ==\n");

    return is.good();
}

bool LwoUnknownChunk::addChild(LwoChunk *pChild)
{
    if(pChild == NULL)
        return false;

    _vChildren.push_back(pChild);

    return true;
}

UInt32 LwoUnknownChunk::getOpCode(void)
{
    return _uiOpCode;
}

NodeTransitPtr LwoUnknownChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr returnValue = makeCoredNode<Group>();

    for(UInt32 i = 0; i < _vChildren.size(); ++i)
        returnValue->addChild(_vChildren[i]->convertToNode(oDB));

    return returnValue;
}

void LwoUnknownChunk::dump(UInt32 uiIndent)
{
    indentLog(uiIndent, PLOG);
    PLOG << "LwoUnknownChunk - " << _uiOpCode
//         << " - "                << findDesc(_sOpCode)
         << std::endl;
}

//---------------------------------------------------------------------
// LwoTagsChunk
//---------------------------------------------------------------------

LwoTagsChunk::LwoTagsChunk(const LwoChunkHeader &oHeader) :
     Inherited  (oHeader),
    _vTags      (       ),
    _mTagToIndex(       )
{
}

LwoTagsChunk::~LwoTagsChunk(void)
{
}

LwoChunkTransitPtr LwoTagsChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoTagsChunk(oHeader));
}

bool LwoTagsChunk::read(std::istream &is, LwoDatabase &oDB)
{
    static std::string tmpBuf;

    tmpBuf.resize(_uiLength);

    UInt32 uiRead = 0;

    while(uiRead < _uiLength && is.good() == true)
    {
        is.getline(&(tmpBuf[0]), _uiLength, '\0');

        UInt32 rc = is.gcount();

        tmpBuf[rc] = '\0';

        std::string tmpString = tmpBuf.substr(0, rc);

        std::map <std::string, UInt32>::iterator mIt = 
            _mTagToIndex.find(tmpString);

        if(mIt == _mTagToIndex.end())
        {
            _mTagToIndex[tmpString] = UInt32(_vTags.size());

            _vTags.push_back(tmpString);
        }
        else
        {
            fprintf(stderr, "try to use tag %s twice \n",
                    tmpString.c_str());
        }

#if 1
        fprintf(stderr, "read tag : %d (%d) | #%s# (%" PRISize ")\n",
                rc,
                uiRead,
                tmpString.c_str(),
                _vTags.size() - 1);
#endif

        if(0x0000 != (rc & 0x0001))
        {
            Char8  tmp;

            is.get(tmp);

            ++rc;
        }

        uiRead += rc;
    }

    oDB.addGlobalTags(this);

    return is.good();
}

UInt32 LwoTagsChunk::getOpCode(void)
{
    return OpCode;
}

NodeTransitPtr LwoTagsChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr returnValue(NULL);

    return returnValue;
}

Int32 LwoTagsChunk::getTagIndex(const std::string &szName)
{
    std::map <std::string, UInt32>::iterator mIt = _mTagToIndex.find(szName);

    if(mIt != _mTagToIndex.end())
    {
        return mIt->second;
    }

    return -1;
}

void LwoTagsChunk::dump(UInt32 uiIndent)
{
    indentLog(uiIndent, PLOG);
    PLOG << "LwoTagsChunk - " << OpCode
//         << " - "                << findDesc(_sOpCode)
         << std::endl;
}

LwoChunkFactoryBase::RegisterChunk LwoTagsChunk::_regHelper(
    &LwoTagsChunk::create,
     LwoTagsChunk::OpCode  );

LwoChunkFactoryBase::RegisterChunk LwoTagsChunk::_regHelperSrfs(
    &LwoTagsChunk::create,
     LwoTagsChunk::OpCodeSrfs);


//---------------------------------------------------------------------
// LwoLayrChunk
//---------------------------------------------------------------------

LwoLayrChunk::LwoLayrChunk(const LwoChunkHeader &oHeader) :
     Inherited(oHeader),
    _pPnts    (       ),
    _vPolys   (       )
{
}

LwoLayrChunk::~LwoLayrChunk(void)
{
    _pPnts     = NULL;

    _vPolys   .clear();
}

UInt32 LwoLayrChunk::uiReadLwo2Pols(      std::istream   &is,
                                    const LwoChunkHeader &oHeader)
{
    UInt32 uiRead     = 0;
    UInt32 uiType     = 0;
    UInt16 uiNumFlags = 0;
            
    uiRead += Inherited::readVal(is, uiType);

    PolySet tmpElem;

    _vPolys.push_back(tmpElem);

    switch(uiType)
    {
        case PtchTag:
        case FaceTag:
        {
            while(uiRead < oHeader.uiLength && is.good() == true)
            {
                uiRead += Inherited::readVal(is, uiNumFlags);
                
                UInt16 uiFlags   = uiNumFlags & 0xFC00;
                UInt16 uiNumVIdx = uiNumFlags & 0x03FF;
                
                std::vector <UInt32> vtmp;

                vtmp.push_back(uiFlags);
                
                _vPolys.back()._vPolys.push_back(vtmp);
#if 0
                fprintf(stderr, "Face %d (%x) %x:",
                        UInt32(uiNumVIdx),
                        UInt32(uiFlags  ),
                        UInt32(uiNumFlags));
#endif
                
                for(UInt32 i = 0; i < uiNumVIdx; ++i)
                {       
                    UInt32 uiRealIdx;
                    
                    uiRead += Inherited::readVarIndexVal(is, uiRealIdx);
                    
                    _vPolys.back()._vPolys.back().push_back(uiRealIdx);
#if 0
                    fprintf(stderr, " %d", uiRealIdx);
#endif
                }
                        
#if 0
                fprintf(stderr, "\n");
#endif
            }

            OSG_ASSERT(uiRead == oHeader.uiLength);
        }
        break;

        case CurvTag:
        case MBalTag:
        case BoneTag:
        default:
        {
            Char8 szType[5];
            
            lwo::int2tag(szType, uiType);
            
            fprintf(stderr, "poly type : %s not handled %d\n",
                    szType,
                    oHeader.uiLength);

            static std::vector<char> tmpBuf;
            
            tmpBuf.resize(oHeader.uiLength - uiRead);
            
            is.read(&(tmpBuf.front()), oHeader.uiLength - uiRead);

            uiRead = oHeader.uiLength;
        }
        break;
    }

    return uiRead;
}


UInt32 LwoLayrChunk::uiReadLwobPols(      std::istream   &is,
                                    const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = 0;

    PolySet tmpElem;

    _vPolys.push_back(tmpElem);

    while(uiRead < oHeader.uiLength && is.good() == true)
    {
        UInt16 uiNumVtx    = 0;
        UInt16 uiVtxIdx    = 0;
        Int16  iVtxSurf    = 0;

        UInt16 uiNumDetVtx = 0;
        UInt16 uiDetVtxIdx = 0;

        std::vector <UInt32> vtmp;

        vtmp.push_back(uiNumVtx);

        _vPolys.back()._vPolys.push_back(vtmp);

        uiRead += Inherited::readVal(is, uiNumVtx);

#if 0
        fprintf(stderr, "%d) ", UInt32(uiNumVtx));
#endif

        for(UInt32 i = 0; i < uiNumVtx; ++i)
        {
            uiRead += Inherited::readVal(is, uiVtxIdx);

            _vPolys.back()._vPolys.back().push_back(uiVtxIdx);
            
#if 0
            fprintf(stderr, "%d ", UInt32(uiVtxIdx));
#endif
        }

#if 0
        fprintf(stderr, "\n");
#endif

        uiRead += Inherited::readVal(is, iVtxSurf);

        _vPolys.back()._vPolyTags.push_back(osgAbs(iVtxSurf) - 1);

#if 0
        fprintf(stderr, "  s: %d\n", UInt32(iVtxSurf));
#endif

        if(iVtxSurf < 0)
        {
            uiRead += Inherited::readVal(is, uiNumDetVtx);

#if 0
            fprintf(stderr, "    det : %d) ", UInt32(uiNumDetVtx));
#endif
            
            for(UInt32 i = 0; i < uiNumDetVtx; ++i)
            {
                uiRead += Inherited::readVal(is, uiDetVtxIdx);
            
#if 0
                fprintf(stderr, "%d ", UInt32(uiDetVtxIdx));
#endif
            }

#if 0
            fprintf(stderr, "\n");
#endif
        }
    }

    OSG_ASSERT(oHeader.uiLength == uiRead);

    return uiRead;
}

LwoChunkTransitPtr LwoLayrChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoLayrChunk(oHeader));
}

bool LwoLayrChunk::read(std::istream &is, LwoDatabase &oDB)
{
    static std::string tmpBuf;

    Int32 uiBuffMax = _uiLength - 16;

    OSG_ASSERT(uiBuffMax > 0);

    tmpBuf.resize(uiBuffMax);

    Int16  i16Val;
    Pnt3f  p3fVal;
    UInt32 uiRead = 0;

    uiRead += Inherited::readVal(is, i16Val);

    fprintf(stderr, "layer number : %x\n", UInt32(i16Val));
    
    uiRead += Inherited::readVal(is, i16Val);

    fprintf(stderr, "layer flags  : %x\n", UInt32(i16Val));

    uiRead += Inherited::readVal(is, p3fVal);
    
    fprintf(stderr, "layer pivot  : %f %f %f\n",
            p3fVal[0],
            p3fVal[1],
            p3fVal[2]);

    is.getline(&(tmpBuf[0]), uiBuffMax, '\0');

    UInt32 rc = is.gcount();

    tmpBuf[rc] = '\0';

    fprintf(stderr, "read layer name : %d (%d) | %s \n",
            rc,
            uiRead,
            tmpBuf.c_str());

    if(0x0000 != (rc & 0x0001))
    {
        Char8 tmp;

        is.get(tmp);

        ++rc;
    }

    uiRead += rc;

    if(uiRead < _uiLength)
    {
        OSG_ASSERT((_uiLength - uiRead) == 2);

        uiRead += Inherited::readVal(is, i16Val);

        fprintf(stderr, "layer parent  : %x\n", UInt32(i16Val));
    }

    OSG_ASSERT(uiRead == _uiLength);

    return is.good();
}

bool LwoLayrChunk::handle(      std::istream   &is,
                                LwoDatabase    &oDB,
                          const LwoChunkHeader &oHeader)
{
    bool returnValue = false;

    switch(oHeader.uiChunkTag)
    {
        case OpCodePnts:
        {
            Pnt3f  p3fVal;
            UInt32 uiRead = 0;

            UInt32 uiNumPnts = oHeader.uiLength / 12;

            if(_pPnts == NULL)
                _pPnts = GeoPnt3fProperty::create();

            fprintf(stderr, "reading %d pnts:\n", uiNumPnts);

            for(UInt32 i = 0; i < uiNumPnts; ++i)
            {
                uiRead += Inherited::readVal(is, p3fVal);
    
                _pPnts->push_back(p3fVal);
#if 0
                fprintf(stderr, "pnt[%d]  : %f %f %f\n",
                        i,
                        p3fVal[0],
                        p3fVal[1],
                        p3fVal[2]);
#endif
            }

            OSG_ASSERT(uiRead == oHeader.uiLength);

            returnValue = true;
        }
        break;

        case OpCodeBBox:
        {
            Pnt3f  p3fVal;
            UInt32 uiRead = 0;

            uiRead += Inherited::readVal(is, p3fVal);
    
            fprintf(stderr, "bbox min  : %f %f %f\n",
                    p3fVal[0],
                    p3fVal[1],
                    p3fVal[2]);
            
            
            uiRead += Inherited::readVal(is, p3fVal);
            
            fprintf(stderr, "bbox max  : %f %f %f\n",
                    p3fVal[0],
                    p3fVal[1],
                    p3fVal[2]);
            

            OSG_ASSERT(uiRead == oHeader.uiLength);

            returnValue = true;
        }
        break;

        case OpCodePols:
        {
            UInt32 uiRead = 0;

            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                uiRead += uiReadLwo2Pols(is, oHeader);
            }
            else
            {
                uiRead += uiReadLwobPols(is, oHeader);
            }

            returnValue = true;
        }
        break;

        case OpCodePTag:
        {
            UInt32 uiRead     = 0;
            UInt32 uiType     = 0;

            uiRead += Inherited::readVal(is, uiType);

            switch(uiType)
            {
                case SurfTag:
                {
                    while(uiRead < oHeader.uiLength && is.good() == true)
                    {
                        UInt32 uiRealIdx = 0;
                        UInt16 uiTag     = 0;

                        uiRead += Inherited::readVarIndexVal(is, uiRealIdx);
                        uiRead += Inherited::readVal        (is, uiTag    );

                        if(_vPolys.back()._vPolyTags.size() <= uiRealIdx)
                        {
                            _vPolys.back()._vPolyTags.resize(uiRealIdx + 1, -1);
                        }

                        _vPolys.back()._vPolyTags[uiRealIdx] = uiTag;
#if 0
                        fprintf(stderr, "(%d) : %d\n", 
                                uiRealIdx, UInt32(uiTag));
#endif
                    }
                
                    OSG_ASSERT(uiRead == oHeader.uiLength);
            
                    returnValue = true;
                }
                break;

                case PartTag:
                case SmgpTag:
                default:
                {
                    Char8 szType[5];

                    lwo::int2tag(szType, uiType);

//                    uiRead -= 4;

                    UInt32 uiToRead = oHeader.uiLength - uiRead;

                    fprintf(stderr, "ptag type : %s not handled %d %d %d\n",
                            szType,
                            oHeader.uiLength,
                            uiRead,
                            uiToRead);

                    static std::vector<char> tmpBuf;

                    tmpBuf.resize(uiToRead);

                    is.read(&(tmpBuf.front()), uiToRead);

#if 0
                    for(UInt32 i = 0; i < uiToRead; ++i)
                    {
                        fprintf(stderr, "(%d) : %c\n",
                                i, //UInt32(tmpBuf[i]),
                                tmpBuf[i]);
                    }
#endif

                    returnValue = true;
                }
                break;
            }
        }
        break;

        default:
            break;
    }

    return returnValue;
}

UInt32 LwoLayrChunk::getOpCode(void)
{
    return OpCode;
}

NodeTransitPtr LwoLayrChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr                returnValue(NULL);
    std::vector<GeometryUnrecPtr> vGeos;

    GeometryUnrecPtr              pCurrGeo    = NULL;
    GeoUInt8PropertyUnrecPtr      pCurrTypes  = NULL;
    GeoUInt32PropertyUnrecPtr     pCurrLength = NULL;
    GeoUInt32PropertyUnrecPtr     pCurrIdx    = NULL;

    for(UInt32 k = 0; k < _vPolys.size(); ++k)
    {
        for(UInt32 i = 0; i < _vPolys[k]._vPolys.size(); ++i)
        {
            if(_vPolys[k]._vPolyTags[i] < 0)
                continue;

            if(Int32(vGeos.size()) <= _vPolys[k]._vPolyTags[i])
            {
                vGeos.resize(_vPolys[k]._vPolyTags[i] + 1, NULL);
            }

            pCurrGeo = vGeos[_vPolys[k]._vPolyTags[i]];

            if(pCurrGeo == NULL)
            {
                pCurrGeo = Geometry::create();

                pCurrGeo->setProperty(_pPnts, Geometry::PositionsIndex);

                pCurrTypes  = GeoUInt8Property ::create();
                pCurrLength = GeoUInt32Property::create();
                pCurrIdx    = GeoUInt32Property::create();

                pCurrGeo->setIndex  (pCurrIdx, Geometry::PositionsIndex);
                pCurrGeo->setTypes  (pCurrTypes                        );
                pCurrGeo->setLengths(pCurrLength                       );

                vGeos[_vPolys[k]._vPolyTags[i]] = pCurrGeo;

                LwoSurfChunk *pSurf = oDB.getSurf(_vPolys[k]._vPolyTags[i]);

                if(pSurf != NULL)
                {
                    fprintf(stderr, "use surface %p %s (%d) | %d\n",
                            static_cast<void *>(pSurf),
                            pSurf->getName().c_str(),
                            _vPolys[k]._vPolyTags[i],
                            UInt32(pSurf->getSidedness()));

                    Material *pMat = pSurf->convertToMaterial(oDB);
                
                    pCurrGeo->setMaterial(pMat);
                }
                else
                {
                    fprintf(stderr, "no surface found %d \n",
                            _vPolys[k]._vPolyTags[i]);
                }
            }
            else
            {
                pCurrTypes  = 
                    dynamic_cast<GeoUInt8Property  *>(pCurrGeo->getTypes());

                pCurrLength = 
                    dynamic_cast<GeoUInt32Property *>(pCurrGeo->getLengths());

                pCurrIdx    = 
                    dynamic_cast<GeoUInt32Property *>(
                        pCurrGeo->getIndex(Geometry::PositionsIndex));
            }

            for(UInt32 j = 1; j < _vPolys[k]._vPolys[i].size(); ++j)
            {
                pCurrIdx->push_back(_vPolys[k]._vPolys[i][j]);
            }

            pCurrTypes ->push_back(GL_POLYGON                              );
            pCurrLength->push_back(UInt32(_vPolys[k]._vPolys[i].size()) - 1);
        }
    }

    returnValue = Node::create();
    
    returnValue->setCore(Group::create());

    for(UInt32 i = 0; i < vGeos.size(); ++i)
    {
        if(vGeos[i] != NULL)
        {
            NodeUnrecPtr pNode = Node::create();

            calcVertexNormals(vGeos[i]);

            pNode->setCore(vGeos[i]);

            returnValue->addChild(pNode);
        }
    }

    return returnValue;
}

bool LwoLayrChunk::isLayer(void)
{
    return true;
}

void LwoLayrChunk::dump(UInt32 uiIndent)
{
    indentLog(uiIndent, PLOG);
    PLOG << "LwoLayrChunk - " << OpCode
//         << " - "                << findDesc(_sOpCode)
         << std::endl;
}

LwoChunkFactoryBase::RegisterChunk LwoLayrChunk::_regHelper(
    &LwoLayrChunk::create,
     LwoLayrChunk::OpCode  );


//---------------------------------------------------------------------
// LwoClipChunk
//---------------------------------------------------------------------

LwoClipChunk::LwoClipChunk(const LwoChunkHeader &oHeader) :
    Inherited(oHeader)
{
}

LwoClipChunk::~LwoClipChunk(void)
{
}

LwoChunkTransitPtr LwoClipChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoClipChunk(oHeader));
}

bool LwoClipChunk::read(std::istream &is, LwoDatabase &oDB)
{
    Int32  i32Val;
    UInt32 uiRead = 0;

    uiRead += Inherited::readVal(is, i32Val);

#if 0
    fprintf(stderr, "clip index : %x\n", UInt32(i32Val));
#endif
    
    return is.good();
}

bool LwoClipChunk::handle(      std::istream   &is,
                                LwoDatabase    &oDB,
                          const LwoChunkHeader &oHeader)
{
    bool returnValue = false;

    switch(oHeader.uiChunkTag)
    {
        case OpCodeStil:
        {
            static std::string tmpBuf;

            tmpBuf.resize(oHeader.uiLength);

            is.getline(&(tmpBuf[0]), oHeader.uiLength, '\0');

            UInt32 rc = is.gcount();

            tmpBuf[rc] = '\0';

#if 0
            fprintf(stderr, "read still fname : %d | %s \n",
                    rc,
                    tmpBuf.c_str());
#endif

            if(0x0000 != (rc & 0x0001))
            {
                Char8 tmp;
                
                is.get(tmp);
                
                ++rc;
            }
            
            OSG_ASSERT(rc == oHeader.uiLength);

            returnValue = true;
        }
        break;

        case OpCodeIseq:
        case OpCodeAnim:
        case OpCodeXref:
        case OpCodeStcc:
        case OpCodeTime:
        case OpCodeCont:
        case OpCodeBrit:
        case OpCodeSatr:
        case OpCodeHue:
        case OpCodeGamm:
        case OpCodeNega:
        case OpCodeIflt:
        case OpCodePflt:
        {
            static std::vector<char> tmpBuf;

            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled in %s clip ==\n",
                    oHeader.szTag);

            returnValue = true;
        }
        break;
        
        default:
            break;
    };

    return returnValue;
}


UInt32 LwoClipChunk::getOpCode(void)
{
    return OpCode;
}

NodeTransitPtr LwoClipChunk::convertToNode(LwoDatabase &oDB)
{
    NodeTransitPtr returnValue(NULL);

    return returnValue;
}

void LwoClipChunk::dump(UInt32 uiIndent)
{
    indentLog(uiIndent, PLOG);
    PLOG << "LwoClipChunk - " << OpCode
//         << " - "                << findDesc(_sOpCode)
         << std::endl;
}

LwoChunkFactoryBase::RegisterChunk LwoClipChunk::_regHelper(
    &LwoClipChunk::create,
     LwoClipChunk::OpCode  );



//---------------------------------------------------------------------
// LwoSurfChunk
//---------------------------------------------------------------------

LwoSurfChunk::LwoSurfChunk(const LwoChunkHeader &oHeader) :
     Inherited         (oHeader),

    _uiReadSoFar       (      0),

    _szName            (       ),
    _szSource          (       ),

    _usSidedness       (1      ),

    _oBaseColor        (0.1f, 
                        0.1f, 
                        0.1f   ),

    _fDiffuseFact      (0.f    ),
    _fLuminosityFact   (0.f    ),
    _fSpecularFact     (0.f    ),
    _fReflectionFact   (0.f    ),
    _fTransparentFact  (0.f    ),
    _fTranslucentFact  (0.f    ),
     
    _fMaxSmoothingAngle(0.f    ),

    _pMat              (NULL   ),
     
    _pActiveBlock      (NULL   ),
    _vBlocks           (       )
{
}

LwoSurfChunk::~LwoSurfChunk(void)
{
    _pMat = NULL;
}

LwoChunkTransitPtr LwoSurfChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoSurfChunk(oHeader));
}

bool LwoSurfChunk::read(std::istream &is, LwoDatabase &oDB)
{
    _uiReadSoFar = 0;
    
    LwoChunkHeader::_mTagLengthSizeMap[lwo::tag2int("VMAP")] = 2;

    static std::string tmpBuf;

    tmpBuf.resize(_uiLength);

    is.getline(&(tmpBuf[0]), _uiLength, '\0');

    UInt32 rc = is.gcount();

    tmpBuf[rc] = '\0';

    _szName = tmpBuf.substr(0, rc);

#if 1
    fprintf(stderr, "read surf name : %d | %s (%" PRISize ")\n",
            rc,
            _szName.c_str(),
            _szName.size());
#endif

    if(0x0000 != (rc & 0x0001))
    {
        Char8 tmp;

        is.get(tmp);

        ++rc;
    }

    _uiReadSoFar += rc;

    if(oDB.getFileType() == LwoDatabase::Lwo2File)
    {
        is.getline(&(tmpBuf[0]), _uiLength, '\0');

        rc = is.gcount();

        tmpBuf[rc] = '\0';

        _szSource = tmpBuf.substr(0, rc);

#if 0
        fprintf(stderr, "read surf source : %d | %s \n",
                rc,
                tmpBuf.c_str());
#endif

        if(0x0000 != (rc & 0x0001))
        {
            Char8 tmp;
            
            is.get(tmp);
            
            ++rc;
        }
   
        _uiReadSoFar += rc;
    }

    oDB.addSurf(this);

    return is.good();
}

bool LwoSurfChunk::handle(      std::istream   &is,
                                LwoDatabase    &oDB,
                          const LwoChunkHeader &oHeader)
{
    bool returnValue = false;

#if 1
    fprintf(stderr, "surf handle enter %d (%d)\n",
            _uiReadSoFar,
            _uiLength);
#endif

    UInt32 uiRead = 0;

    if(_pActiveBlock != NULL)
    {
        uiRead += _pActiveBlock->handleData(is, oDB, oHeader);

        fprintf(stderr, "block finished %d %d\n", 
                uiRead,
                UInt32(_pActiveBlock->done()));

        if(_pActiveBlock->done() == true)
        {
            _pActiveBlock = NULL;
        }

        if(uiRead != 0 || oHeader.uiChunkTag == OpCodeTmap)
        {
            returnValue = true;
        }
    }

    switch(oHeader.uiChunkTag)
    {
        case OpCodeColr:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, _oBaseColor);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope);
            }
            else
            {
                UInt32 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }
           
            returnValue = true;
        }
        break;

        case OpCodeDiff:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, _fDiffuseFact);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope  );
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeLumi:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;
            
                uiRead += Inherited::readVal        (is, _fLuminosityFact);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope     );
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeSpec:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, _fSpecularFact);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope   );
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeRefl:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, _fReflectionFact);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope     );
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeTran:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, _fTransparentFact);
                uiRead += Inherited::readVarIndexVal(is,  uiEnvelope      );
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeTrnl:
        {
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, _fTranslucentFact);
            uiRead += Inherited::readVarIndexVal(is,  uiEnvelope      );

            returnValue = true;
        }
        break;

        case OpCodeGlos:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                Real32 fVal;
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, fVal      );
                uiRead += Inherited::readVarIndexVal(is, uiEnvelope);
            }
            else
            {
                UInt16 uiTmp;

                uiRead += Inherited::readVal(is, uiTmp);
            }

            returnValue = true;
        }
        break;

        case OpCodeShrp:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeBump:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeSide:
        {
            uiRead += Inherited::readVal(is, _usSidedness);
            
            returnValue = true;
        }
        break;

        case OpCodeSman:
        {
            uiRead += Inherited::readVal(is, _fMaxSmoothingAngle);

            returnValue = true;
        }
        break;

        case OpCodeRfop:
        {
            UInt16 uiVal;

            uiRead += Inherited::readVal(is, uiVal);
            
            returnValue = true;
        }
        break;

        case OpCodeRimg:
        {
            UInt32 uiVal;

            uiRead += Inherited::readVarIndexVal(is, uiVal);
            
            returnValue = true;
        }
        break;

        case OpCodeRsan:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeRind:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                Real32 fVal;
                UInt32 uiEnvelope;

                uiRead += Inherited::readVal        (is, fVal      );
                uiRead += Inherited::readVarIndexVal(is, uiEnvelope);
            }
            else
            {
                Real32 fVal;

                uiRead += Inherited::readVal(is, fVal);
            }

            returnValue = true;
        }
        break;

        case OpCodeTrop:
        {
            UInt16 uiVal;
            
            uiRead += Inherited::readVal(is, uiVal);
            
            returnValue = true;
        }
        break;

        case OpCodeTimg:
        {
            if(oDB.getFileType() == LwoDatabase::Lwo2File)
            {
                UInt32 uiVal;

                uiRead += Inherited::readVarIndexVal(is, uiVal);
            }
            else
            {
                std::string szTmp;

                uiRead += Inherited::readString(is, szTmp, oHeader.uiLength);
            }

            returnValue = true;
        }
        break;

        case OpCodeClrh:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeClrf:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeAdtr:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeGlow:
        {
            UInt16 uiType;
            Real32 fIntensity;
            UInt32 uiIntensityEnvelope;
            Real32 fSize;
            UInt32 uiSizeEnvelope;

            uiRead += Inherited::readVal        (is, uiType             );
            uiRead += Inherited::readVal        (is, fIntensity         );
            uiRead += Inherited::readVarIndexVal(is, uiIntensityEnvelope);
            uiRead += Inherited::readVal        (is, fSize              );
            uiRead += Inherited::readVarIndexVal(is, uiSizeEnvelope     );

            returnValue = true;
        }
        break;

        case OpCodeGval:
        {
            Real32 fVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fVal      );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            returnValue = true;
        }
        break;

        case OpCodeLine:
        {
            static std::vector<char> tmpBuf;

            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled %s in surf ==\n",
                    oHeader.szTag);

            uiRead += oHeader.uiLength;

            returnValue = true;
        }
        break;

        case OpCodeAlph:
        {
            UInt16 uiMode;
            Real32 fVal;

            uiRead += Inherited::readVal(is, uiMode);
            uiRead += Inherited::readVal(is, fVal  );

            returnValue = true;
        }
        break;

        case OpCodeBlok:
        {
//            uiRead += oHeader.uiHeaderSize;

            uiRead += oHeader.uiHeaderSize;

            LwoChunkHeader oRHeader;

            oRHeader.read(is);

            _pActiveBlock = dynamic_pointer_cast<LwoBlockChunk>(
                LwoChunkFactory::the()->createChunk(oRHeader));

            _pActiveBlock->setBlockLength(oHeader.uiLength);

            OSG_ASSERT(_pActiveBlock != NULL);

            _vBlocks.push_back(_pActiveBlock);

            uiRead += _pActiveBlock->handleData(is, oDB, oRHeader);

            if(_pActiveBlock->done() == true)
            {
                _pActiveBlock = NULL;
            }

            fprintf(stderr, "block finished %d\n", 
                    UInt32(_pActiveBlock->done()));
#if 0
            static std::string tmpBuf;

            tmpBuf.resize(_uiLength);

            is.getline(&(tmpBuf[0]), _uiLength, '\0');

            UInt32 uiRead = is.gcount();

            tmpBuf[uiRead] = '\0';

            if(0x0000 != (uiRead & 0x0001))
            {
                Char8  tmp;

                is.get(tmp);
                
                ++uiRead;
            }
#endif
            returnValue = true;
        }
        break;

        default:
            break;
    };

    if(uiRead != 0 || (oHeader.uiChunkTag == OpCodeTmap))
    {
        _uiReadSoFar += uiRead;
        _uiReadSoFar += oHeader.uiHeaderSize;
    }
    else
    {
        fprintf(stderr, "else %d %d | %d %d\n",
                _uiReadSoFar,
                _uiLength,
                oHeader.uiLength,
                oHeader.uiHeaderSize);
                
        if(_uiReadSoFar < _uiLength)
        {
            _uiReadSoFar += oHeader.uiLength;
            _uiReadSoFar += oHeader.uiHeaderSize;
        }
    }

#if 1
    fprintf(stderr, "surf handle leave %d (%d)\n",
            _uiReadSoFar,
            _uiLength);
#endif

    if(_uiReadSoFar == _uiLength)
    {
        LwoChunkHeader::_mTagLengthSizeMap[lwo::tag2int("VMAP")] = 4;
    }

    return returnValue;
}

UInt32 LwoSurfChunk::getOpCode(void)
{
    return OpCode;
}

Material *LwoSurfChunk::convertToMaterial(LwoDatabase &oDB)
{
    if(_pMat == NULL)
    {
        ChunkMaterialUnrecPtr pMat = ChunkMaterial::create();

        MaterialChunkUnrecPtr pMatC = MaterialChunk::create();

        Color4f oCol;

        oCol[0] = _oBaseColor[0] * _fDiffuseFact;
        oCol[1] = _oBaseColor[1] * _fDiffuseFact;
        oCol[2] = _oBaseColor[2] * _fDiffuseFact;
        oCol[3] = 1.f;

        pMatC->setDiffuse(oCol);

        pMat->addChunk(pMatC);

        if(_usSidedness == 3)
        {
            TwoSidedLightingChunkUnrecPtr pTwoSL = 
                TwoSidedLightingChunk::create();

            pMat->addChunk(pTwoSL);
        }


        _pMat = pMat;
    }

    return _pMat;
}

const std::string &LwoSurfChunk::getName(void)
{
    return _szName;
}

UInt16 LwoSurfChunk::getSidedness(void)
{
    return _usSidedness;
}

void LwoSurfChunk::dump(UInt32 uiIndent)
{
    indentLog(uiIndent, PLOG);
    PLOG << "LwoSurfChunk - " << OpCode
//         << " - "                << findDesc(_sOpCode)
         << std::endl;
}

LwoChunkFactoryBase::RegisterChunk LwoSurfChunk::_regHelper(
    &LwoSurfChunk::create,
     LwoSurfChunk::OpCode  );


//---------------------------------------------------------------------
// LwoImapChunk
//---------------------------------------------------------------------

LwoImapBlockChunk::LwoImapBlockChunk(const LwoChunkHeader &oHeader) :
    Inherited(oHeader)
{
}

LwoImapBlockChunk::~LwoImapBlockChunk(void)
{
}

LwoChunkTransitPtr LwoImapBlockChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoImapBlockChunk(oHeader));
}

UInt32 LwoImapBlockChunk::getOpCode(void)
{
    return OpCode;
}

UInt32 LwoImapBlockChunk::handleData(      std::istream   &is,
                                           LwoDatabase    &oDB,
                                     const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = Inherited::handleData(is, oDB, oHeader);

    switch(oHeader.uiChunkTag)
    {
        case LwoSurfChunk::OpCodeProj:
        {
            UInt16 uiProjectionMode;

            uiRead += Inherited::readVal(is, uiProjectionMode);

            fprintf(stderr, "proj : %d\n", UInt32(uiProjectionMode));
        }
        break;

        case LwoSurfChunk::OpCodeAxis:
        {
            UInt16 uiAxis;

            uiRead += Inherited::readVal(is, uiAxis);

            fprintf(stderr, "axis : %d\n", UInt32(uiAxis));
        }
        break;

        case LwoSurfChunk::OpCodeImag:
        {
            UInt32 uiClip;

            uiRead += Inherited::readVarIndexVal(is, uiClip);

            fprintf(stderr, "clip : %d\n", uiClip);
        }
        break;

        case LwoSurfChunk::OpCodeWrap:
        {
            UInt16 uiWrapW;
            UInt16 uiWrapH;

            uiRead += Inherited::readVal(is, uiWrapW);
            uiRead += Inherited::readVal(is, uiWrapH);

            fprintf(stderr, "wrap w : %d\n", uiWrapW);
            fprintf(stderr, "wrap h : %d\n", uiWrapH);
        }
        break;

        case LwoSurfChunk::OpCodeWrpW:
        {
            Real32 fWrapRepeatW;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fWrapRepeatW);
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope  );

            fprintf(stderr, "wrap repeat w : %f\n", fWrapRepeatW);
        }
        break;

        case LwoSurfChunk::OpCodeWrpH:
        {
            Real32 fWrapRepeatH;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, fWrapRepeatH);
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope  );

            fprintf(stderr, "wrap repeat h : %f\n", fWrapRepeatH);
        }
        break;

        case LwoSurfChunk::OpCodeAast:
        {
            UInt16 uiAntiAliasFlags;
            Real32 fAntiAliasStrength;

            uiRead += Inherited::readVal(is, uiAntiAliasFlags  );
            uiRead += Inherited::readVal(is, fAntiAliasStrength);

            fprintf(stderr, "aa : %d %f\n",
                    UInt32(uiAntiAliasFlags),
                    fAntiAliasStrength      );
        }
        break;

        case LwoSurfChunk::OpCodePixb:
        {
            UInt16 uiPixelBlend;

            uiRead += Inherited::readVal(is, uiPixelBlend);

            fprintf(stderr, "pixel blend : %d\n", UInt32(uiPixelBlend));
        }
        break;


        case LwoSurfChunk::OpCodeTmap:
        {
            _uiReadSoFar += oHeader.uiHeaderSize;            
        };
        break;


        case LwoSurfChunk::OpCodeCntr:
        {
            Pnt3f  uiVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, uiVal     );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            fprintf(stderr, "center %f %f %f\n",
                    uiVal[0],
                    uiVal[1],
                    uiVal[2]);
        };
        break;

        case LwoSurfChunk::OpCodeSize:
        {
            Pnt3f  uiVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, uiVal     );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            fprintf(stderr, "size %f %f %f\n",
                    uiVal[0],
                    uiVal[1],
                    uiVal[2]);
        };
        break;

        case LwoSurfChunk::OpCodeRota:
        {
            Pnt3f  uiVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, uiVal     );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            fprintf(stderr, "rota %f %f %f\n",
                    uiVal[0],
                    uiVal[1],
                    uiVal[2]);
        };
        break;

        case LwoSurfChunk::OpCodeOref:
        {
            std::string szVal;

            uiRead += Inherited::readString(is, szVal, oHeader.uiLength);

            fprintf(stderr, "oref %s\n",
                    szVal.c_str());
        };
        break;

        case LwoSurfChunk::OpCodeFall:
        {
            UInt16 uiType;
            Pnt3f  uiVal;
            UInt32 uiEnvelope;

            uiRead += Inherited::readVal        (is, uiType    );
            uiRead += Inherited::readVal        (is, uiVal     );
            uiRead += Inherited::readVarIndexVal(is, uiEnvelope);

            fprintf(stderr, "type %d | vec %f %f %f\n",
                    UInt32(uiType),
                    uiVal[0],
                    uiVal[1],
                    uiVal[2]);
        };
        break;

        case LwoSurfChunk::OpCodeCsys:
        {
            UInt16 uiVal;

            uiRead += Inherited::readVal(is, uiVal     );

            fprintf(stderr, "csys %d\n", UInt32(uiVal));
        };
        break;


//        case LwoSurfChunk::OpCodeProj:
//        case LwoSurfChunk::OpCodeAxis:
//        case LwoSurfChunk::OpCodeImag:
//        case LwoSurfChunk::OpCodeWrap:
//        case LwoSurfChunk::OpCodeWrpW:
//        case LwoSurfChunk::OpCodeWrpH:
        case LwoSurfChunk::OpCodeVmap:
//        case LwoSurfChunk::OpCodeAast:
//        case LwoSurfChunk::OpCodePixb:
        case LwoSurfChunk::OpCodeStck:
        case LwoSurfChunk::OpCodeTamp:
            
        case LwoSurfChunk::OpCodeValu:
        case LwoSurfChunk::OpCodeFunc:
            
        case LwoSurfChunk::OpCodePnam:
        case LwoSurfChunk::OpCodeInam:
        case LwoSurfChunk::OpCodeGrst:
        case LwoSurfChunk::OpCodeGren:
        case LwoSurfChunk::OpCodeGrpt:
        case LwoSurfChunk::OpCodeFkey:
        case LwoSurfChunk::OpCodeIkey:
        {
            static std::vector<char> tmpBuf;
            
            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled %s block in surfblock ==\n",
                    oHeader.szTag);

            uiRead = oHeader.uiLength;
        }
        break;

        default:
            break;
    };
    
    _uiReadSoFar += uiRead;

    if(uiRead != 0)
    {
        _uiReadSoFar += oHeader.uiHeaderSize;
    }
       

    fprintf(stderr, "== unknown %d (%d | %d)==\n", uiRead, 
            _uiReadSoFar, _uiBlockLength);

    return uiRead;
}

LwoChunkFactoryBase::RegisterChunk LwoImapBlockChunk::_regHelper(
    &LwoImapBlockChunk::create,
     LwoImapBlockChunk::OpCode  );


//---------------------------------------------------------------------
// LwoProcChunk
//---------------------------------------------------------------------

LwoProcBlockChunk::LwoProcBlockChunk(const LwoChunkHeader &oHeader) :
    Inherited(oHeader)
{
}

LwoProcBlockChunk::~LwoProcBlockChunk(void)
{
}

LwoChunkTransitPtr LwoProcBlockChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoProcBlockChunk(oHeader));
}

UInt32 LwoProcBlockChunk::getOpCode(void)
{
    return OpCode;
}

UInt32 LwoProcBlockChunk::handleData(      std::istream   &is,
                                           LwoDatabase    &oDB,
                                     const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = Inherited::handleData(is, oDB, oHeader);

    switch(oHeader.uiChunkTag)
    {
        case LwoSurfChunk::OpCodeTmap:
        case LwoSurfChunk::OpCodeCntr:
        case LwoSurfChunk::OpCodeSize:
        case LwoSurfChunk::OpCodeRota:
        case LwoSurfChunk::OpCodeOref:
        case LwoSurfChunk::OpCodeFall:
        case LwoSurfChunk::OpCodeCsys:

        case LwoSurfChunk::OpCodeProj:
        case LwoSurfChunk::OpCodeAxis:
        case LwoSurfChunk::OpCodeImag:
        case LwoSurfChunk::OpCodeWrap:
        case LwoSurfChunk::OpCodeWrpW:
        case LwoSurfChunk::OpCodeWrpH:
        case LwoSurfChunk::OpCodeVmap:
        case LwoSurfChunk::OpCodeAast:
        case LwoSurfChunk::OpCodePixb:
        case LwoSurfChunk::OpCodeStck:
        case LwoSurfChunk::OpCodeTamp:
            
        case LwoSurfChunk::OpCodeValu:
        case LwoSurfChunk::OpCodeFunc:
            
        case LwoSurfChunk::OpCodePnam:
        case LwoSurfChunk::OpCodeInam:
        case LwoSurfChunk::OpCodeGrst:
        case LwoSurfChunk::OpCodeGren:
        case LwoSurfChunk::OpCodeGrpt:
        case LwoSurfChunk::OpCodeFkey:
        case LwoSurfChunk::OpCodeIkey:
        {
            static std::vector<char> tmpBuf;
            
            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled %s block in surfblock ==\n",
                    oHeader.szTag);

            uiRead = oHeader.uiLength;
        }
        break;

        default:
            break;
    };
    
    _uiReadSoFar += uiRead;

    if(uiRead != 0)
    {
        _uiReadSoFar += oHeader.uiHeaderSize;
    }
       

    fprintf(stderr, "== unknown %d (%d | %d)==\n", uiRead, 
            _uiReadSoFar, _uiBlockLength);

    return uiRead;
}

LwoChunkFactoryBase::RegisterChunk LwoProcBlockChunk::_regHelper(
    &LwoProcBlockChunk::create,
     LwoProcBlockChunk::OpCode  );


//---------------------------------------------------------------------
// LwoGradChunk
//---------------------------------------------------------------------

LwoGradBlockChunk::LwoGradBlockChunk(const LwoChunkHeader &oHeader) :
    Inherited(oHeader)
{
}

LwoGradBlockChunk::~LwoGradBlockChunk(void)
{
}

LwoChunkTransitPtr LwoGradBlockChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoGradBlockChunk(oHeader));
}

UInt32 LwoGradBlockChunk::getOpCode(void)
{
    return OpCode;
}

UInt32 LwoGradBlockChunk::handleData(      std::istream   &is,
                                           LwoDatabase    &oDB,
                                     const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = Inherited::handleData(is, oDB, oHeader);

    switch(oHeader.uiChunkTag)
    {
        case LwoSurfChunk::OpCodeTmap:
        case LwoSurfChunk::OpCodeCntr:
        case LwoSurfChunk::OpCodeSize:
        case LwoSurfChunk::OpCodeRota:
        case LwoSurfChunk::OpCodeOref:
        case LwoSurfChunk::OpCodeFall:
        case LwoSurfChunk::OpCodeCsys:

        case LwoSurfChunk::OpCodeProj:
        case LwoSurfChunk::OpCodeAxis:
        case LwoSurfChunk::OpCodeImag:
        case LwoSurfChunk::OpCodeWrap:
        case LwoSurfChunk::OpCodeWrpW:
        case LwoSurfChunk::OpCodeWrpH:
        case LwoSurfChunk::OpCodeVmap:
        case LwoSurfChunk::OpCodeAast:
        case LwoSurfChunk::OpCodePixb:
        case LwoSurfChunk::OpCodeStck:
        case LwoSurfChunk::OpCodeTamp:
            
        case LwoSurfChunk::OpCodeValu:
        case LwoSurfChunk::OpCodeFunc:
            
        case LwoSurfChunk::OpCodePnam:
        case LwoSurfChunk::OpCodeInam:
        case LwoSurfChunk::OpCodeGrst:
        case LwoSurfChunk::OpCodeGren:
        case LwoSurfChunk::OpCodeGrpt:
        case LwoSurfChunk::OpCodeFkey:
        case LwoSurfChunk::OpCodeIkey:
        {
            static std::vector<char> tmpBuf;
            
            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled %s block in surfblock ==\n",
                    oHeader.szTag);

            uiRead = oHeader.uiLength;
        }
        break;

        default:
            break;
    };
    
    _uiReadSoFar += uiRead;

    if(uiRead != 0)
    {
        _uiReadSoFar += oHeader.uiHeaderSize;
    }
       

    fprintf(stderr, "== unknown %d (%d | %d)==\n", uiRead, 
            _uiReadSoFar, _uiBlockLength);

    return uiRead;
}

LwoChunkFactoryBase::RegisterChunk LwoGradBlockChunk::_regHelper(
    &LwoGradBlockChunk::create,
     LwoGradBlockChunk::OpCode  );


//---------------------------------------------------------------------
// LwoShdrChunk
//---------------------------------------------------------------------

LwoShdrBlockChunk::LwoShdrBlockChunk(const LwoChunkHeader &oHeader) :
    Inherited(oHeader)
{
}

LwoShdrBlockChunk::~LwoShdrBlockChunk(void)
{
}

LwoChunkTransitPtr LwoShdrBlockChunk::create(const LwoChunkHeader &oHeader)
{
    return LwoChunkTransitPtr(new LwoShdrBlockChunk(oHeader));
}

UInt32 LwoShdrBlockChunk::getOpCode(void)
{
    return OpCode;
}

UInt32 LwoShdrBlockChunk::handleData(      std::istream   &is,
                                           LwoDatabase    &oDB,
                                     const LwoChunkHeader &oHeader)
{
    UInt32 uiRead = Inherited::handleData(is, oDB, oHeader);

    switch(oHeader.uiChunkTag)
    {
            
        case LwoSurfChunk::OpCodeTmap:
        case LwoSurfChunk::OpCodeCntr:
        case LwoSurfChunk::OpCodeSize:
        case LwoSurfChunk::OpCodeRota:
        case LwoSurfChunk::OpCodeOref:
        case LwoSurfChunk::OpCodeFall:
        case LwoSurfChunk::OpCodeCsys:

        case LwoSurfChunk::OpCodeProj:
        case LwoSurfChunk::OpCodeAxis:
        case LwoSurfChunk::OpCodeImag:
        case LwoSurfChunk::OpCodeWrap:
        case LwoSurfChunk::OpCodeWrpW:
        case LwoSurfChunk::OpCodeWrpH:
        case LwoSurfChunk::OpCodeVmap:
        case LwoSurfChunk::OpCodeAast:
        case LwoSurfChunk::OpCodePixb:
        case LwoSurfChunk::OpCodeStck:
        case LwoSurfChunk::OpCodeTamp:
            
        case LwoSurfChunk::OpCodeValu:
        case LwoSurfChunk::OpCodeFunc:
            
        case LwoSurfChunk::OpCodePnam:
        case LwoSurfChunk::OpCodeInam:
        case LwoSurfChunk::OpCodeGrst:
        case LwoSurfChunk::OpCodeGren:
        case LwoSurfChunk::OpCodeGrpt:
        case LwoSurfChunk::OpCodeFkey:
        case LwoSurfChunk::OpCodeIkey:
        {
            static std::vector<char> tmpBuf;
            
            tmpBuf.resize(oHeader.uiLength);

            is.read(&(tmpBuf.front()), oHeader.uiLength);

            fprintf(stderr, "== unhandled %s block in surfblock ==\n",
                    oHeader.szTag);

            uiRead = oHeader.uiLength;
        }
        break;

        default:
            break;
    };
    
    _uiReadSoFar += uiRead;

    if(uiRead != 0)
    {
        _uiReadSoFar += oHeader.uiHeaderSize;
    }
       

    fprintf(stderr, "== unknown %d (%d | %d)==\n", uiRead, 
            _uiReadSoFar, _uiBlockLength);

    return uiRead;
}

LwoChunkFactoryBase::RegisterChunk LwoShdrBlockChunk::_regHelper(
    &LwoShdrBlockChunk::create,
     LwoShdrBlockChunk::OpCode  );



OSG_END_NAMESPACE
