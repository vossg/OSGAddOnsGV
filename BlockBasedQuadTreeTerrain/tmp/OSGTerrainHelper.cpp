
#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include "OSGGLU.h"

#include "OSGTerrainHelper.h"
#include "OSGImageFileHandler.h"
#include "OSGDrawEnv.h"

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------



ImageBlockAccessor::ImageBlockAccessor(void) :
    _pImage(NullFC)
{
}

ImageBlockAccessor::~ImageBlockAccessor(void)
{
    OSG::subRef(_pImage);
}

bool ImageBlockAccessor::open(const std::string filename)
{
    _pImage = ImageFileHandler::the()->read(filename.c_str());

    if(_pImage != NullFC)
        _pImage->dump();

    return _pImage != NullFC;
}

Vec2i ImageBlockAccessor::getSize(void)
{
    Vec2i returnValue(_pImage->getWidth (),
                      _pImage->getHeight());

    return returnValue;
}


Image::Type ImageBlockAccessor::getType(void)
{
    return Image::Type(_pImage->getDataType());
}

Image::PixelFormat ImageBlockAccessor::getFormat(void)
{
    return Image::PixelFormat(_pImage->getPixelFormat());
}


bool ImageBlockAccessor::readBlockRGB(Vec2i  vSampleOrigin,
                                      int    iTextureSize,
                                      UInt8 *pTarget,
                                      int    iTargetSizeBytes)
{
    const UInt8 *pData = _pImage->getData();

    UInt32 destIdx = 0;

    Int32 xMin = vSampleOrigin.x();
    Int32 xMax = vSampleOrigin.x() + iTextureSize;

    Int32 yMin = vSampleOrigin.y();
    Int32 yMax = vSampleOrigin.y() + iTextureSize;

    for(UInt32 y = yMin; y < yMax; y++)
    {
        for(UInt32 x = xMin; x < xMax; x++)
        {
            for(UInt32 i = 0; i < 3; i++)
            {
                if(y >= _pImage->getHeight() ||
                   x >= _pImage->getWidth()   )
                {
                    pTarget[destIdx] = 0;
                }
                else
                {
                    pTarget[destIdx] = 
                        pData[(y * _pImage->getWidth() + x) * 3 + i];
                }

                destIdx++;
            }
        }
        
        destIdx += (iTextureSize - (xMax - xMin)) * 3;
    }
}

bool ImageBlockAccessor::readBlockA16(Vec2i   vSampleOrigin,
                                      int     iTextureSize,
                                      UInt16 *pTarget,
                                      int     iTargetSizeBytes)
{
    const UInt8 *pData = _pImage->getData();

    UInt8 *pDst = (UInt8 *) pTarget;

    UInt32 destIdx = 0;

    Int32 xMin = vSampleOrigin.x();
    Int32 xMax = vSampleOrigin.x() + iTextureSize;

    Int32 yMin = vSampleOrigin.y();
    Int32 yMax = vSampleOrigin.y() + iTextureSize;

    for(UInt32 y = yMin; y < yMax; y++)
    {
        for(UInt32 x = xMin; x < xMax; x++)
        {
            for(UInt32 i = 0; i < 2; i++)
            {
                if(y >= _pImage->getHeight() ||
                   x >= _pImage->getWidth()   )
                {
                    pDst[destIdx] = 0;
                }
                else
                {
                    pDst[destIdx] = 
                        pData[((y) * _pImage->getWidth() + x) * 2 + i];
                }

                destIdx++;
            }
        }
        
        destIdx += (iTextureSize - (xMax - xMin)) * 2;
    }
}

bool ImageBlockAccessor::readBlockA16(Vec2i   vSampleOrigin,
                                      int     iTextureSize,
                                      Int16  *pTarget,
                                      int     iTargetSizeBytes)
{
    const UInt8 *pData = _pImage->getData();

    UInt8 *pDst = (UInt8 *) pTarget;

    UInt32 destIdx = 0;

    Int32 xMin = vSampleOrigin.x();
    Int32 xMax = vSampleOrigin.x() + iTextureSize;

    Int32 yMin = vSampleOrigin.y();
    Int32 yMax = vSampleOrigin.y() + iTextureSize;

    for(UInt32 y = yMin; y < yMax; y++)
    {
        for(UInt32 x = xMin; x < xMax; x++)
        {
            for(UInt32 i = 0; i < 2; i++)
            {
                if(y >= _pImage->getHeight() ||
                   x >= _pImage->getWidth()   )
                {
                    pDst[destIdx] = 0;
                }
                else
                {
                    pDst[destIdx] = 
                        pData[((y) * _pImage->getWidth() + x) * 2 + i];
                }

                destIdx++;
            }
        }
        
        destIdx += (iTextureSize - (xMax - xMin)) * 2;
    }
}


//-----------------------------------------------------------------------------

OpenGLGpuBuffer::OpenGLGpuBuffer(void) :
    _uiBufferId(0)
{
}

bool OpenGLGpuBuffer::create(GLenum type, 
                             int size, 
                             BufferUsage usage )
{
    if(_uiBufferId != 0)
    {
        destroy();
    }
    

    glGenBuffers(1, &_uiBufferId);

    if(_uiBufferId == 0)
    {
        PWARNING << "[opengl] Could not create vertex_buffer_object!\n";
        return false;
    }

    glBindBuffer(type, _uiBufferId);

    switch( usage )
    {
		case BufferUsage_Static_WriteOnly:
		case BufferUsage_Static_ReadWrite:
			_eGLUsage = GL_STATIC_DRAW_ARB;
			break;
            
		case BufferUsage_Stream_WriteOnly:
		case BufferUsage_Stream_ReadWrite:
			_eGLUsage = GL_STREAM_DRAW_ARB;
			break;

		case BufferUsage_Dynamic_WriteOnly:
		case BufferUsage_Dynamic_ReadWrite:
			_eGLUsage = GL_DYNAMIC_DRAW_ARB;
			break;

		default:
			PWARNING << "[opengl] Invalid usage flags!\n";
			glDeleteBuffers( 1, &_uiBufferId );
			_uiBufferId = 0;
			return false;
    }

    glBufferData(type, size, 0, _eGLUsage);
#if 0
    g_VboFunctions->bufferData( type, size, 0, glUsage_ );
    
    if( glGetError() )
    {
        PWARNING << "[opengl] Could not create vertex_buffer_object!\n";
        
        g_VboFunctions->deleteBuffers( 1, &bufferId_ );
        bufferId_ = 0;
        return false;
    }
#endif

    _eGLType      = type;
    _iBufferSize  = size;
    _eBufferUsage = usage;

    return true;
}


bool OpenGLGpuBuffer::uploadData(const void* sourceData, 
                                 int targetOffset, 
                                 int size )
{
    glBindBuffer(_eGLType, _uiBufferId);		
    glBufferSubData(_eGLType, targetOffset, size, sourceData);
}

GLuint OpenGLGpuBuffer::getBufferId() const
{
    return _uiBufferId;
}

void OpenGLGpuBuffer::destroy(void)
{
    if(_uiBufferId != 0 )
    {
        glDeleteBuffers(1, &_uiBufferId);
        _uiBufferId = 0;
    }
}


OpenGLShader::OpenGLShader(void) :
    _pShader(NullFC)
{
}

bool OpenGLShader::loadVtxFromFile(Char8 *szFilename)
{
    if(_pShader == NullFC)
    {
        _pShader = SHLChunk::create();
    }

    return _pShader->readVertexProgram(szFilename);
}

bool OpenGLShader::loadFrgFromFile(Char8 *szFilename)
{
    if(_pShader == NullFC)
    {
        _pShader = SHLChunk::create();
    }

    return _pShader->readFragmentProgram(szFilename);
}

void OpenGLShader::activate(DrawEnv *pEnv)
{
    _pShader->activate(pEnv);
}

void OpenGLShader::deactivate(DrawEnv *pEnv)
{
    _pShader->deactivate(pEnv);
}

void OpenGLShader::update(DrawEnv *pEnv)
{
    _pShader->updateParameters(pEnv->getWindow(),
                               _pShader->getParameters(),
                               false, true, true);
}

void OpenGLShader::setSampler(Char8 *szName, Int32 iVal)
{
    _pShader->setUniformParameter(szName, iVal);
}

void OpenGLShader::setUniform(Char8 *szName, Real32 rVal)
{
    _pShader->setUniformParameter(szName, rVal);
}

void OpenGLShader::setUniform(Char8 *szName, Vec2f  vVal)
{
    _pShader->setUniformParameter(szName, vVal);
}


OpenGLTexture::OpenGLTexture(void) :
    _pTexture(NullFC),
    _bCompressed(false)
{
}

bool OpenGLTexture::loadFromFile(Char8 *szFilename)
{
    return true;
}

bool OpenGLTexture::create2D(Image::PixelFormat pf, 
                             int iWidth, 
                             int iHeight, 
                             int unknown)
{
    if(_pTexture == NullFC)
    {
        ImagePtr pImg = Image          ::create();

        pImg->set(pf, iWidth, iHeight);

        _pTexture = TextureObjChunk::create();
        
        _pTexture->setImage(pImg);

        if(pf == Image::OSG_RGB_DXT1)
        {
            _bCompressed = true;
        }
    }

    return true;
}

bool OpenGLTexture::upload2DRect( int iX, 
                                  int iY,
                                  const void *pData,
                                  unsigned int iSize)
{
    //    glTexSubImage2D

    if(_bCompressed == true)
    {
        glCompressedTexSubImage2DARB(GL_TEXTURE_2D, 0, iX, iY, 128, 128, 
                                     //vSize[0], vSize[1], 
                                     GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 
                                     iSize, pData);
    }
    else
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, iX, iY, 128, 128, 
                        //vSize[0], vSize[1], 
                        GL_RGB, GL_UNSIGNED_BYTE, pData);
    }

    return true;
}

void OpenGLTexture::disable(GLenum eTexUnit, DrawEnv *pEnv)
{
    if(_pTexture != NullFC)
        _pTexture->deactivate(pEnv, eTexUnit - GL_TEXTURE0);
}

void OpenGLTexture::enable(GLenum eTexUnit, DrawEnv *pEnv)
{
    if(_pTexture != NullFC)
        _pTexture->activate(pEnv, eTexUnit - GL_TEXTURE0);
}

void OpenGLTexture::destroy(void)
{
}


OSG_END_NAMESPACE
