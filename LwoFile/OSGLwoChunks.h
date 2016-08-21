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

#ifndef _OSGLWORECORDS_H_
#define _OSGLWORECORDS_H_

#include "boost/function.hpp"

#include "OSGContribLwoFileDef.h"
#include "OSGBaseTypes.h"
#include "OSGBaseFunctions.h"
#include "OSGSingletonHolder.h"
#include "OSGNode.h"
#include "OSGTextureObjChunk.h"
#include "OSGMaterialChunk.h"
#include "OSGMemoryObject.h"

#include "OSGTypedGeoIntegralProperty.h"
#include "OSGTypedGeoVectorProperty.h"
#include "OSGMaterial.h"

#include <map>

OSG_BEGIN_NAMESPACE

class LwoChunk;
class LwoDatabase;

typedef TransitPtr<LwoChunk> LwoChunkTransitPtr;

/*! \brief General VRML Node Desc
*/

namespace lwo
{

#define OSG_LWO_TAG_TO_OPCODE(CTAG1, CTAG2, CTAG3, CTAG4)   \
    (UInt32(CTAG1) << 24 |                                  \
     UInt32(CTAG2) << 16 |                                  \
     UInt32(CTAG3) <<  8 |                                  \
     UInt32(CTAG4)       )

inline
UInt32 tag2int(const Char8        *szTag   )
{
    return (UInt32(szTag[0]) << 24 | 
            UInt32(szTag[1]) << 16 |
            UInt32(szTag[2]) <<  8 |
            UInt32(szTag[3])       );
}

inline
void   int2tag(      Char8        *szTagOut,
                     UInt32        uiValIn )   
{
    szTagOut[0] = UInt8(uiValIn >> 24);
    szTagOut[1] = UInt8(uiValIn >> 16);
    szTagOut[2] = UInt8(uiValIn >>  8);
    szTagOut[3] = UInt8(uiValIn      );
    szTagOut[4] = '\0';
}

}

struct LwoChunkHeader
{
    UInt32 uiChunkTag;
    UInt32 uiLength;
    UInt32 uiLengthSize;
    UInt32 uiHeaderSize;
    Char8  szTag[5];

    static std::map<UInt32, UInt32> _mTagLengthSizeMap;

    bool read(std::istream &is)
    {
        is.read(reinterpret_cast<char *>(&uiChunkTag), 4);

        if(is.good() == false)
            return false;

        uiChunkTag = osgNetToHost(uiChunkTag);

        std::map<UInt32, UInt32>::iterator mIt = 
            _mTagLengthSizeMap.find(uiChunkTag);

        uiLengthSize = 4;

        if(mIt != _mTagLengthSizeMap.end())
        {
            uiLengthSize = mIt->second;
        }
        else
        {
            char szTmpTag[5];

            lwo::int2tag(szTmpTag, uiChunkTag);

            fprintf(stderr, "unknown tag %s assume length 4\n", 
                    szTmpTag);       
        }

        uiHeaderSize = 4 + uiLengthSize;

        is.read(reinterpret_cast<char *>(&uiLength), uiLengthSize);

        if(uiLengthSize == 2)
        {
            uiLength = osgNetToHost(UInt16(uiLength));
        }
        else
        {
            uiLength = osgNetToHost(uiLength);
        }

        lwo::int2tag(szTag, uiChunkTag);

#if 1
        fprintf(stderr, "opcode %x | %s | %d, length %d\n", 
                uiChunkTag, szTag, uiLengthSize, uiLength);       
#endif

        return is.good();
    }

    LwoChunkHeader(void);
    ~LwoChunkHeader(void);

  protected:
};

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoChunkFactoryBase
{
    /*==========================  PUBLIC  =================================*/

  public :

    typedef boost::function<
        LwoChunkTransitPtr (const LwoChunkHeader &)> CreateChunk;

    class OSG_CONTRIBLWOFILE_DLLMAPPING RegisterChunk
    {
      public:

        RegisterChunk(CreateChunk fCreate,
                      UInt32      uiChunkTag);
    };

    /*---------------------------------------------------------------------*/
    /*! \name                   Class Get                                  */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Helper                                    */
    /*! \{                                                                 */


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Field                                    */
    /*! \{                                                                 */

    void registerChunk(CreateChunk fRecord,
                       UInt32      uiChunkTag);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Prototypes                                 */
    /*! \{                                                                 */

    LwoChunkTransitPtr createChunk(const LwoChunkHeader &oHeader);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Node                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      FieldValue                              */
    /*! \{                                                                 */


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    template <class SingletonT>
    friend class SingletonHolder;

    typedef std::map<UInt32,
                     CreateChunk> NameChunkCreateMap;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    LwoChunkFactoryBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~LwoChunkFactoryBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    NameChunkCreateMap _mRegisteredChunks;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    LwoChunkFactoryBase(const LwoChunkFactoryBase &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const LwoChunkFactoryBase &source);
};

#if defined(WIN32)
OSG_CONTRIBLWOFILE_EXPIMP_TMPL 
template class 
OSG_CONTRIBLWOFILE_DLLMAPPING SingletonHolder<LwoChunkFactoryBase>;
#endif

typedef SingletonHolder<LwoChunkFactoryBase> LwoChunkFactory;


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoChunk : public MemoryObject
{
  protected:

    typedef MemoryObject Inherited;

    /*---------------------------------------------------------------------*/

    UInt32 _uiLength;

    /*---------------------------------------------------------------------*/

    void readChar8(std::istream &is, Char8  *cVal, UInt32 uiSize);

    /*---------------------------------------------------------------------*/

    template<class ValueT>
    UInt32 readVal        (std::istream &is,  
                           ValueT       &val      );

    UInt32 readVarIndexVal(std::istream &is,  
                           UInt32       &val      );

    UInt32 readString     (std::istream &is, 
                           std::string  &val, 
                           UInt32        uiMaxSize);

    /*---------------------------------------------------------------------*/

//    const Char8 *findDesc(UInt16 sOpCode);

    /*---------------------------------------------------------------------*/

             LwoChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoChunk(void);

  public:

    /*---------------------------------------------------------------------*/

    virtual bool read  (      std::istream   &is,
                              LwoDatabase    &oDB    );

    virtual bool handle(      std::istream   &is,
                              LwoDatabase    &oDB,
                        const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual bool   addChild           (LwoChunk *pChild);
    virtual UInt32 getOpCode          (void            ) = 0;
    virtual bool   isLayer            (void            );
    virtual bool   isSubChunkContainer(void            );

    /*---------------------------------------------------------------------*/

    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};

typedef RefCountPtr<LwoChunk, MemObjRefCountPolicy> LwoChunkRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoSubChunkContainer : public LwoChunk
{
  protected:

    typedef LwoChunk Inherited;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

             LwoSubChunkContainer(const LwoChunkHeader &oHeader);

    virtual ~LwoSubChunkContainer(void);

  public:

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    virtual bool isSubChunkContainer(void);

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/
};

typedef RefCountPtr<LwoSubChunkContainer, 
                    MemObjRefCountPolicy> LwoSubChunkContainerRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoBlockChunk : public LwoChunk
{
  protected:

    typedef LwoChunk Inherited;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    UInt32      _uiReadSoFar;
    UInt32      _uiBlockLength;

    std::string _szOrdinal;

    /*---------------------------------------------------------------------*/

             LwoBlockChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoBlockChunk(void);

  public:

    /*---------------------------------------------------------------------*/

    void setBlockLength(UInt32 uiLength);

    /*---------------------------------------------------------------------*/

    virtual bool done(void);

    /*---------------------------------------------------------------------*/

    virtual UInt32 handleData(      std::istream   &is,
                                    LwoDatabase    &oDB,
                              const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/
};

typedef RefCountPtr<LwoBlockChunk, 
                    MemObjRefCountPolicy> LwoBlockChunkRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoHeaderChunk : public LwoChunk
{
    typedef LwoChunk Inherited;

    friend class LwoDatabase;

    /*---------------------------------------------------------------------*/

    std::vector<LwoChunkRCPtr> _vChildren;
    UInt32                     _uiFileType;

    /*---------------------------------------------------------------------*/

    LwoHeaderChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoHeaderChunk(void);

  public:

    static const UInt32 OpCode = 1;

    /*---------------------------------------------------------------------*/

    virtual bool read(std::istream &is, LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual bool addChild (LwoChunk *pChild);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode  (void);
            UInt32 getFileType(void);

    /*---------------------------------------------------------------------*/

    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);

    /*---------------------------------------------------------------------*/
};

typedef RefCountPtr<LwoHeaderChunk,
                    MemObjRefCountPolicy> LwoHeaderChunkRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoUnknownChunk : public LwoChunk
{
  protected:

    typedef LwoChunk Inherited;

    /*---------------------------------------------------------------------*/

    UInt32                     _uiOpCode;
    std::vector<LwoChunkRCPtr> _vChildren;

    /*---------------------------------------------------------------------*/

    LwoUnknownChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoUnknownChunk(void);

    /*---------------------------------------------------------------------*/

    friend class LwoChunkFactoryBase;

  public:

    virtual bool read(std::istream &is, LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual bool addChild(LwoChunk *pChild);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode(void);

    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoTagsChunk : public LwoChunk
{
  protected:

    typedef LwoChunk Inherited;

    /*---------------------------------------------------------------------*/

    static const UInt32 OpCode     = OSG_LWO_TAG_TO_OPCODE('T','A','G','S');
    static const UInt32 OpCodeSrfs = OSG_LWO_TAG_TO_OPCODE('S','R','F','S');

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;
    static LwoChunkFactoryBase::RegisterChunk _regHelperSrfs;

    /*---------------------------------------------------------------------*/

    LwoTagsChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoTagsChunk(void);

    /*---------------------------------------------------------------------*/

    friend class LwoChunkFactoryBase;

    std::vector<std::string        > _vTags;
    std::map   <std::string, UInt32> _mTagToIndex;

  public:

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual bool read(std::istream &is, LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual UInt32         getOpCode    (void            );
    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/
 
    Int32 getTagIndex(const std::string &szName);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};

typedef RefCountPtr<LwoTagsChunk,
                    MemObjRefCountPolicy> LwoTagsChunkRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class LwoSurfChunk;

typedef RefCountPtr<LwoSurfChunk,
                    MemObjRefCountPolicy> LwoSurfChunkRCPtr;

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoLayrChunk : public LwoChunk
{
  protected:

    typedef LwoChunk Inherited;

    /*---------------------------------------------------------------------*/

    static const UInt32 OpCode     = OSG_LWO_TAG_TO_OPCODE('L','A','Y','R');

    static const UInt32 OpCodePnts = OSG_LWO_TAG_TO_OPCODE('P','N','T','S');
    static const UInt32 OpCodeBBox = OSG_LWO_TAG_TO_OPCODE('B','B','O','X');
    static const UInt32 OpCodePols = OSG_LWO_TAG_TO_OPCODE('P','O','L','S');
    static const UInt32 OpCodePTag = OSG_LWO_TAG_TO_OPCODE('P','T','A','G');


    static const UInt32 FaceTag    = OSG_LWO_TAG_TO_OPCODE('F','A','C','E');
    static const UInt32 CurvTag    = OSG_LWO_TAG_TO_OPCODE('C','U','R','V');
    static const UInt32 PtchTag    = OSG_LWO_TAG_TO_OPCODE('P','T','C','H');
    static const UInt32 MBalTag    = OSG_LWO_TAG_TO_OPCODE('M','B','A','L');
    static const UInt32 BoneTag    = OSG_LWO_TAG_TO_OPCODE('B','O','N','E');

    static const UInt32 SurfTag    = OSG_LWO_TAG_TO_OPCODE('S','U','R','F');
    static const UInt32 PartTag    = OSG_LWO_TAG_TO_OPCODE('P','A','R','T');
    static const UInt32 SmgpTag    = OSG_LWO_TAG_TO_OPCODE('S','M','G','P');

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    LwoLayrChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoLayrChunk(void);

    /*---------------------------------------------------------------------*/

    struct PolySet
    {
        std::vector< std::vector <UInt32> > _vPolys;
        std::vector<               Int32  > _vPolyTags;  

        PolySet(void) : _vPolys(), _vPolyTags() {}
    };

    GeoPnt3fPropertyUnrecPtr _pPnts;
    std::vector<PolySet>     _vPolys;           

    /*---------------------------------------------------------------------*/

    UInt32 uiReadLwo2Pols(      std::istream   &is,
                          const LwoChunkHeader &oHeader);

    UInt32 uiReadLwobPols(      std::istream   &is,
                          const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    friend class LwoChunkFactoryBase;

  public:

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual bool read  (      std::istream   &is, 
                              LwoDatabase    &oDB);
    virtual bool handle(      std::istream   &is,
                              LwoDatabase    &oDB,
                        const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32         getOpCode    (void            );
    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);
    virtual bool           isLayer      (void            );

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};

typedef RefCountPtr<LwoLayrChunk, MemObjRefCountPolicy> LwoLayrChunkRCPtr;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoClipChunk : public LwoSubChunkContainer
{
  protected:

    typedef LwoSubChunkContainer Inherited;

    /*---------------------------------------------------------------------*/

    static const UInt32 OpCode     = OSG_LWO_TAG_TO_OPCODE('C','L','I','P');

    static const UInt32 OpCodeStil = OSG_LWO_TAG_TO_OPCODE('S','T','I','L');
    static const UInt32 OpCodeIseq = OSG_LWO_TAG_TO_OPCODE('I','S','E','Q');
    static const UInt32 OpCodeAnim = OSG_LWO_TAG_TO_OPCODE('A','N','I','M');
    static const UInt32 OpCodeXref = OSG_LWO_TAG_TO_OPCODE('X','R','E','F');
    static const UInt32 OpCodeStcc = OSG_LWO_TAG_TO_OPCODE('S','T','C','C');
    static const UInt32 OpCodeTime = OSG_LWO_TAG_TO_OPCODE('T','I','M','E');
    static const UInt32 OpCodeCont = OSG_LWO_TAG_TO_OPCODE('C','O','N','T');
    static const UInt32 OpCodeBrit = OSG_LWO_TAG_TO_OPCODE('B','R','I','T');
    static const UInt32 OpCodeSatr = OSG_LWO_TAG_TO_OPCODE('S','A','T','R');
    static const UInt32 OpCodeHue  = OSG_LWO_TAG_TO_OPCODE('H','U','E',' ');
    static const UInt32 OpCodeGamm = OSG_LWO_TAG_TO_OPCODE('G','A','M','M');
    static const UInt32 OpCodeNega = OSG_LWO_TAG_TO_OPCODE('N','E','G','A');
    static const UInt32 OpCodeIflt = OSG_LWO_TAG_TO_OPCODE('I','F','L','T');
    static const UInt32 OpCodePflt = OSG_LWO_TAG_TO_OPCODE('P','F','L','T');

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    LwoClipChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoClipChunk(void);

    /*---------------------------------------------------------------------*/

    friend class LwoChunkFactoryBase;

  public:

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual bool read  (      std::istream   &is, 
                              LwoDatabase    &oDB    );

    virtual bool handle(      std::istream   &is,
                              LwoDatabase    &oDB,
                        const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32         getOpCode    (void            );
    virtual NodeTransitPtr convertToNode(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoSurfChunk : public LwoSubChunkContainer
{
  protected:

    typedef LwoSubChunkContainer Inherited;

  public:

    /*---------------------------------------------------------------------*/

    static const UInt32 OpCode     = OSG_LWO_TAG_TO_OPCODE('S','U','R','F');

    static const UInt32 OpCodeColr = OSG_LWO_TAG_TO_OPCODE('C','O','L','R');
    static const UInt32 OpCodeDiff = OSG_LWO_TAG_TO_OPCODE('D','I','F','F');
    static const UInt32 OpCodeLumi = OSG_LWO_TAG_TO_OPCODE('L','U','M','I');
    static const UInt32 OpCodeSpec = OSG_LWO_TAG_TO_OPCODE('S','P','E','C');
    static const UInt32 OpCodeRefl = OSG_LWO_TAG_TO_OPCODE('R','E','F','L');
    static const UInt32 OpCodeTran = OSG_LWO_TAG_TO_OPCODE('T','R','A','N');
    static const UInt32 OpCodeTrnl = OSG_LWO_TAG_TO_OPCODE('T','R','N','L');
    static const UInt32 OpCodeGlos = OSG_LWO_TAG_TO_OPCODE('G','L','O','S');
    static const UInt32 OpCodeShrp = OSG_LWO_TAG_TO_OPCODE('S','H','R','P');
    static const UInt32 OpCodeBump = OSG_LWO_TAG_TO_OPCODE('B','U','M','P');
    static const UInt32 OpCodeSide = OSG_LWO_TAG_TO_OPCODE('S','I','D','E');
    static const UInt32 OpCodeSman = OSG_LWO_TAG_TO_OPCODE('S','M','A','N');
    static const UInt32 OpCodeRfop = OSG_LWO_TAG_TO_OPCODE('R','F','O','P');
    static const UInt32 OpCodeRimg = OSG_LWO_TAG_TO_OPCODE('R','I','M','G');
    static const UInt32 OpCodeRsan = OSG_LWO_TAG_TO_OPCODE('R','S','A','N');
    static const UInt32 OpCodeRind = OSG_LWO_TAG_TO_OPCODE('R','I','N','D');
    static const UInt32 OpCodeTrop = OSG_LWO_TAG_TO_OPCODE('T','R','O','P');
    static const UInt32 OpCodeTimg = OSG_LWO_TAG_TO_OPCODE('T','I','M','G');
    static const UInt32 OpCodeClrh = OSG_LWO_TAG_TO_OPCODE('C','L','R','H');
    static const UInt32 OpCodeClrf = OSG_LWO_TAG_TO_OPCODE('C','L','R','F');
    static const UInt32 OpCodeAdtr = OSG_LWO_TAG_TO_OPCODE('A','D','T','R');
    static const UInt32 OpCodeGlow = OSG_LWO_TAG_TO_OPCODE('G','L','O','W');
    static const UInt32 OpCodeGval = OSG_LWO_TAG_TO_OPCODE('G','V','A','L');
    static const UInt32 OpCodeLine = OSG_LWO_TAG_TO_OPCODE('L','I','N','E');
    static const UInt32 OpCodeAlph = OSG_LWO_TAG_TO_OPCODE('A','L','P','H');


    static const UInt32 OpCodeBlok = OSG_LWO_TAG_TO_OPCODE('B','L','O','K');

    //Surf block header
    static const UInt32 OpCodeImap = OSG_LWO_TAG_TO_OPCODE('I','M','A','P');
    static const UInt32 OpCodeProc = OSG_LWO_TAG_TO_OPCODE('P','R','O','C');
    static const UInt32 OpCodeGrad = OSG_LWO_TAG_TO_OPCODE('G','R','A','D');
    static const UInt32 OpCodeShdr = OSG_LWO_TAG_TO_OPCODE('S','H','D','R');

    // Surf block subchunks
    static const UInt32 OpCodeChan = OSG_LWO_TAG_TO_OPCODE('C','H','A','N');
    static const UInt32 OpCodeEnab = OSG_LWO_TAG_TO_OPCODE('E','N','A','B');
    static const UInt32 OpCodeOpac = OSG_LWO_TAG_TO_OPCODE('O','P','A','C');
    static const UInt32 OpCodeNega = OSG_LWO_TAG_TO_OPCODE('N','E','G','A');

    // Surf block tmap subchunks
    static const UInt32 OpCodeTmap = OSG_LWO_TAG_TO_OPCODE('T','M','A','P');
    static const UInt32 OpCodeCntr = OSG_LWO_TAG_TO_OPCODE('C','N','T','R');
    static const UInt32 OpCodeSize = OSG_LWO_TAG_TO_OPCODE('S','I','Z','E');
    static const UInt32 OpCodeRota = OSG_LWO_TAG_TO_OPCODE('R','O','T','A');
    static const UInt32 OpCodeOref = OSG_LWO_TAG_TO_OPCODE('O','R','E','F');
    static const UInt32 OpCodeFall = OSG_LWO_TAG_TO_OPCODE('F','A','L','L');
    static const UInt32 OpCodeCsys = OSG_LWO_TAG_TO_OPCODE('C','S','Y','S');

    // Surf block imap
    static const UInt32 OpCodeProj = OSG_LWO_TAG_TO_OPCODE('P','R','O','J');
    static const UInt32 OpCodeAxis = OSG_LWO_TAG_TO_OPCODE('A','X','I','S');
    static const UInt32 OpCodeImag = OSG_LWO_TAG_TO_OPCODE('I','M','A','G');
    static const UInt32 OpCodeWrap = OSG_LWO_TAG_TO_OPCODE('W','R','A','P');
    static const UInt32 OpCodeWrpW = OSG_LWO_TAG_TO_OPCODE('W','R','P','W');
    static const UInt32 OpCodeWrpH = OSG_LWO_TAG_TO_OPCODE('W','R','P','H');
    static const UInt32 OpCodeVmap = OSG_LWO_TAG_TO_OPCODE('V','M','A','P');
    static const UInt32 OpCodeAast = OSG_LWO_TAG_TO_OPCODE('A','A','S','T');
    static const UInt32 OpCodePixb = OSG_LWO_TAG_TO_OPCODE('P','I','X','B');
    static const UInt32 OpCodeStck = OSG_LWO_TAG_TO_OPCODE('S','T','C','K');
    static const UInt32 OpCodeTamp = OSG_LWO_TAG_TO_OPCODE('T','A','M','P');

    // Surf block proc
    static const UInt32 OpCodeValu = OSG_LWO_TAG_TO_OPCODE('V','A','L','U');
    static const UInt32 OpCodeFunc = OSG_LWO_TAG_TO_OPCODE('F','U','N','C');

    // Surf grad block
    static const UInt32 OpCodePnam = OSG_LWO_TAG_TO_OPCODE('P','N','A','M');
    static const UInt32 OpCodeInam = OSG_LWO_TAG_TO_OPCODE('I','N','A','M');
    static const UInt32 OpCodeGrst = OSG_LWO_TAG_TO_OPCODE('G','R','S','T');
    static const UInt32 OpCodeGren = OSG_LWO_TAG_TO_OPCODE('G','R','E','N');
    static const UInt32 OpCodeGrpt = OSG_LWO_TAG_TO_OPCODE('G','R','P','T');
    static const UInt32 OpCodeFkey = OSG_LWO_TAG_TO_OPCODE('F','K','E','Y');
    static const UInt32 OpCodeIkey = OSG_LWO_TAG_TO_OPCODE('I','K','E','Y');

  protected:
    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    LwoSurfChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoSurfChunk(void);

    /*---------------------------------------------------------------------*/

    UInt32                          _uiReadSoFar;

    std::string                     _szName;
    std::string                     _szSource;

    UInt16                          _usSidedness;
    Color3f                         _oBaseColor;

    Real32                          _fDiffuseFact;
    Real32                          _fLuminosityFact;
    Real32                          _fSpecularFact;
    Real32                          _fReflectionFact;
    Real32                          _fTransparentFact;
    Real32                          _fTranslucentFact;

    Real32                          _fMaxSmoothingAngle;

    MaterialUnrecPtr                _pMat;


    LwoBlockChunkRCPtr              _pActiveBlock;

    std::vector<LwoBlockChunkRCPtr> _vBlocks;

    /*---------------------------------------------------------------------*/

    friend class LwoChunkFactoryBase;

  public:

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual bool read(        std::istream   &is, 
                              LwoDatabase    &oDB    );
    virtual bool handle(      std::istream   &is,
                              LwoDatabase    &oDB,
                        const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32    getOpCode        (void            );
    virtual Material *convertToMaterial(LwoDatabase &oDB);

    /*---------------------------------------------------------------------*/

    const std::string &getName(void);

    /*---------------------------------------------------------------------*/

    UInt16 getSidedness(void);

    /*---------------------------------------------------------------------*/

    virtual void dump(UInt32 uiIndent);
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoImapBlockChunk : public LwoBlockChunk
{
  protected:

    typedef LwoBlockChunk Inherited;

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

             LwoImapBlockChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoImapBlockChunk(void);

  public:

    static const UInt32 OpCode = OSG_LWO_TAG_TO_OPCODE('I','M','A','P');

    /*---------------------------------------------------------------------*/

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode(void);

    /*---------------------------------------------------------------------*/

    virtual UInt32 handleData(      std::istream   &is,
                                    LwoDatabase    &oDB,
                              const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoProcBlockChunk : public LwoBlockChunk
{
  protected:

    typedef LwoBlockChunk Inherited;

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

             LwoProcBlockChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoProcBlockChunk(void);

  public:

    static const UInt32 OpCode = OSG_LWO_TAG_TO_OPCODE('P','R','O','C');

    /*---------------------------------------------------------------------*/

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode(void);

    /*---------------------------------------------------------------------*/

    virtual UInt32 handleData(      std::istream   &is,
                                    LwoDatabase    &oDB,
                              const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoGradBlockChunk : public LwoBlockChunk
{
  protected:

    typedef LwoBlockChunk Inherited;

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

             LwoGradBlockChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoGradBlockChunk(void);

  public:

    static const UInt32 OpCode = OSG_LWO_TAG_TO_OPCODE('G','R','A','D');

    /*---------------------------------------------------------------------*/

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode(void);

    /*---------------------------------------------------------------------*/

    virtual UInt32 handleData(      std::istream   &is,
                                    LwoDatabase    &oDB,
                              const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/
};


//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoShdrBlockChunk : public LwoBlockChunk
{
  protected:

    typedef LwoBlockChunk Inherited;

    /*---------------------------------------------------------------------*/

    static LwoChunkFactoryBase::RegisterChunk _regHelper;

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/

             LwoShdrBlockChunk(const LwoChunkHeader &oHeader);

    virtual ~LwoShdrBlockChunk(void);

  public:

    static const UInt32 OpCode     = OSG_LWO_TAG_TO_OPCODE('S','H','D','R');

    /*---------------------------------------------------------------------*/

    static LwoChunkTransitPtr create(const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/

    virtual UInt32 getOpCode(void);

    /*---------------------------------------------------------------------*/

    virtual UInt32 handleData(      std::istream   &is,
                                    LwoDatabase    &oDB,
                              const LwoChunkHeader &oHeader);

    /*---------------------------------------------------------------------*/
};

OSG_END_NAMESPACE

#endif
