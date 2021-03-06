//------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		21.04.2006
//
//	Description			:		OSGGpuBuffer.cpp
//
//-------------------------------------------------------------------------------------------------

#include "PrecompiledHeader.h"
#include "OSGGpuBuffer.h"
#include <OSGGLEXT.h>

//-------------------------------------------------------------------------------------------------

namespace OSG
{
	
	//-------------------------------------------------------------------------------------------------

	typedef ptrdiff_t	GLsizeiptrARB;
	typedef ptrdiff_t	GLintptrARB;

	typedef void 		(OSG_APIENTRY * OSGGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
	typedef void 		(OSG_APIENTRY * OSGGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage );
	typedef void 		(OSG_APIENTRY * OSGGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
	typedef void 		(OSG_APIENTRY * OSGGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
	typedef void 		(OSG_APIENTRY * OSGGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
	typedef void 		(OSG_APIENTRY * OSGGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
	typedef void 		(OSG_APIENTRY * OSGGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid** params);
	typedef void 		(OSG_APIENTRY * OSGGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
	typedef GLboolean	(OSG_APIENTRY * OSGGLISBUFFERARBPROC) (GLuint buffer);
	typedef GLvoid *	(OSG_APIENTRY * OSGGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
	typedef GLboolean	(OSG_APIENTRY * OSGGLUNMAPBUFFERARBPROC) (GLenum target);

	struct VboFunctions
	{
		VboFunctions();
		bool								initFunctionPointers( Window* window );
		void								clearFunctionPointers();

		OSGGLBINDBUFFERARBPROC				bindBuffer;
		OSGGLBUFFERDATAARBPROC				bufferData;
		OSGGLBUFFERSUBDATAARBPROC			bufferSubData;
		OSGGLDELETEBUFFERSARBPROC			deleteBuffers;
		OSGGLGENBUFFERSARBPROC				genBuffers;
		OSGGLGETBUFFERPARAMETERIVARBPROC	getBufferParameteriv;
		OSGGLGETBUFFERPOINTERVARBPROC		getBufferPointerv;
		OSGGLGETBUFFERSUBDATAARBPROC		getBufferSubData;
		OSGGLISBUFFERARBPROC				isBuffer;
		OSGGLMAPBUFFERARBPROC				mapBuffer;
		OSGGLUNMAPBUFFERARBPROC				unmapBuffer;

		static UInt32						getExtensionId();

	private:
		static UInt32						extensionId_;
		static UInt32						bindBufferId_;
		static UInt32						bufferDataId_;
		static UInt32						bufferSubDataId_;
		static UInt32						deleteBuffersId_;
		static UInt32						genBuffersId_;
		static UInt32						getBufferParameterivId_;
		static UInt32						getBufferPointervId_;
		static UInt32						getBufferSubDataId_;
		static UInt32						isBufferId_;
		static UInt32						mapBufferId_;
		static UInt32						unmapBufferId_;
	};

	//-------------------------------------------------------------------------------------------------

	UInt32 VboFunctions::extensionId_				= 0;
	UInt32 VboFunctions::bindBufferId_				= Window::invalidFunctionID;
	UInt32 VboFunctions::bufferDataId_				= Window::invalidFunctionID;
	UInt32 VboFunctions::bufferSubDataId_			= Window::invalidFunctionID;
	UInt32 VboFunctions::deleteBuffersId_			= Window::invalidFunctionID;
	UInt32 VboFunctions::genBuffersId_				= Window::invalidFunctionID;
	UInt32 VboFunctions::getBufferParameterivId_	= Window::invalidFunctionID;
	UInt32 VboFunctions::getBufferPointervId_		= Window::invalidFunctionID;
	UInt32 VboFunctions::getBufferSubDataId_		= Window::invalidFunctionID;
	UInt32 VboFunctions::isBufferId_				= Window::invalidFunctionID;
	UInt32 VboFunctions::mapBufferId_				= Window::invalidFunctionID;
	UInt32 VboFunctions::unmapBufferId_				= Window::invalidFunctionID;


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	int				g_VboFunctionRefCount = 0;
	VboFunctions*	g_VboFunctions;

	//-------------------------------------------------------------------------------------------------


	VboFunctions::VboFunctions()
	{
		// register extensions: (make this static/once only?!)
		if( extensionId_ == 0 )
		{
			extensionId_ = Window::registerExtension( "GL_ARB_vertex_buffer_object" );
			
			bindBufferId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glBindBufferARB", extensionId_ );
			bufferDataId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glBufferDataARB", extensionId_ );
			bufferSubDataId_ 		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glBufferSubDataARB", extensionId_ );
			deleteBuffersId_ 		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glDeleteBuffersARB", extensionId_ );
			genBuffersId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGenBuffersARB", extensionId_ );
			getBufferParameterivId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetBufferParameterivARB", extensionId_ );
			getBufferPointervId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetBufferPointervARB", extensionId_ );
			getBufferSubDataId_		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetBufferSubDataARB", extensionId_ );
			isBufferId_				= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glIsBufferARB", extensionId_ );
			mapBufferId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glMapBufferARB", extensionId_ );
			unmapBufferId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUnmapBufferARB", extensionId_ );
		}
		clearFunctionPointers();
	}


	//-------------------------------------------------------------------------------------------------


	bool VboFunctions::initFunctionPointers( Window* window )
	{
		assert( window );

		bindBuffer				= (OSGGLBINDBUFFERARBPROC) window->getFunction( bindBufferId_ );
		bufferData				= (OSGGLBUFFERDATAARBPROC) window->getFunction( bufferDataId_ );
		bufferSubData			= (OSGGLBUFFERSUBDATAARBPROC) window->getFunction( bufferSubDataId_ );
		deleteBuffers			= (OSGGLDELETEBUFFERSARBPROC) window->getFunction( deleteBuffersId_ );
		genBuffers				= (OSGGLGENBUFFERSARBPROC) window->getFunction( genBuffersId_ );
		getBufferParameteriv	= (OSGGLGETBUFFERPARAMETERIVARBPROC) window->getFunction( getBufferParameterivId_ );
		getBufferPointerv		= (OSGGLGETBUFFERPOINTERVARBPROC) window->getFunction( getBufferPointervId_ );
		getBufferSubData		= (OSGGLGETBUFFERSUBDATAARBPROC) window->getFunction( getBufferSubDataId_ );
		isBuffer				= (OSGGLISBUFFERARBPROC) window->getFunction( isBufferId_ );
		mapBuffer				= (OSGGLMAPBUFFERARBPROC) window->getFunction( mapBufferId_ );
		unmapBuffer				= (OSGGLUNMAPBUFFERARBPROC) window->getFunction( unmapBufferId_ );

		return ( bindBuffer != 0 ) && ( bufferData != 0 ) && ( bufferSubData != 0 ) && ( deleteBuffers != 0 ) &&
			( genBuffers != 0 ) && ( getBufferPointerv != 0 ) && ( getBufferSubData != 0 ) && ( isBuffer != 0 ) &&
			( mapBuffer != 0 ) && ( unmapBuffer != 0 );
	}


	//-------------------------------------------------------------------------------------------------


	void VboFunctions::clearFunctionPointers()
	{
		bindBuffer				= 0;
		bufferData				= 0;
		bufferSubData			= 0;
		deleteBuffers			= 0;
		genBuffers				= 0;
		getBufferParameteriv	= 0;
		getBufferPointerv		= 0;
		getBufferSubData		= 0;
		isBuffer				= 0;
		mapBuffer				= 0;
		unmapBuffer				= 0;
	}


	//-------------------------------------------------------------------------------------------------


	UInt32 VboFunctions::getExtensionId()
	{
		return extensionId_;
	}

	
	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	
	GpuBuffer::GpuBuffer()
	{
		isLocked_		= false;
		bufferId_		= 0;
		bufferSize_		= 0;
		bufferUsage_	= BufferUsage_Count;
		glUsage_		= GL_NONE;
		glType_			= GL_NONE;		

		if( g_VboFunctionRefCount++ == 0 )
		{
			g_VboFunctions = new VboFunctions;
		}
	}


	//-------------------------------------------------------------------------------------------------


	GpuBuffer::~GpuBuffer()
	{
		destroy();		
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuBuffer::create( Window* window, GLenum type, int size, BufferUsage usage )
	{
		assert( window );

		if( bufferId_ )
		{
			destroy();
		}

		if( !window->hasExtension( VboFunctions::getExtensionId() ) )
		{
			FWARNING( ( "OpenGL Shading Language is not supported, couldn't find extension 'GL_ARB_vertex_buffer_object'!\n"));			
			return false;
		}

		// Get Function Pointers from Window (GL-Context):
		if( !g_VboFunctions->initFunctionPointers( window ) )
		{
			return false;
		}

		window_ = window;

		g_VboFunctions->genBuffers( 1, &bufferId_ );		

		if( !bufferId_ )
		{
			PWARNING << "[opengl] Could not create vertex_buffer_object!\n";
			return false;
		}

		g_VboFunctions->bindBuffer( type, bufferId_ );

		switch( usage )
		{
		case BufferUsage_Static_WriteOnly:
		case BufferUsage_Static_ReadWrite:
			glUsage_ = GL_STATIC_DRAW_ARB;
			break;

		case BufferUsage_Stream_WriteOnly:
		case BufferUsage_Stream_ReadWrite:
			glUsage_ = GL_STREAM_DRAW_ARB;
			break;

		case BufferUsage_Dynamic_WriteOnly:
		case BufferUsage_Dynamic_ReadWrite:
			glUsage_ = GL_DYNAMIC_DRAW_ARB;
			break;

		default:
			PWARNING << "[opengl] Invalid usage flags!\n";
			g_VboFunctions->deleteBuffers( 1, &bufferId_ );
			bufferId_ = 0;
			return false;
		}

		g_VboFunctions->bufferData( type, size, 0, glUsage_ );

		if( glGetError() )
		{
			PWARNING << "[opengl] Could not create vertex_buffer_object!\n";

			g_VboFunctions->deleteBuffers( 1, &bufferId_ );
			bufferId_ = 0;
			return false;
		}

		glType_			= type;
		isLocked_		= false;
		bufferSize_		= size;
		bufferUsage_	= usage;

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	void GpuBuffer::destroy()
	{
		if( bufferId_ )
		{
			if( isLocked_ )
			{
				unlock();
			}
			g_VboFunctions->deleteBuffers( 1, &bufferId_ );
			bufferId_ = 0;
		}
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuBuffer::isValid() const
	{
		return bufferId_ != 0;
	}


	//-------------------------------------------------------------------------------------------------


	void GpuBuffer::activate()
	{
		g_VboFunctions->bindBuffer( glType_, bufferId_ );		
	}


	//-------------------------------------------------------------------------------------------------


	void GpuBuffer::deactivate()
	{
		g_VboFunctions->bindBuffer( glType_, 0 );
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuBuffer::uploadData( const void* sourceData, int targetOffset, int size )
	{
		// todo: implement this:
		g_VboFunctions->bindBuffer( glType_, bufferId_ );		
		g_VboFunctions->bufferSubData( glType_, targetOffset, size, sourceData );
		//g_VboFunctions->bindBuffer( glType_, 0 );
		
		return true;
	}


	//-------------------------------------------------------------------------------------------------


	void* GpuBuffer::lock( int offset, int length, BufferLockOption lockOption )
	{
		if( isLocked_ )
		{
			PWARNING << "[opengl] Could not lock an already locked VertexBuffer.\n";
			return 0;
		}

		if( !bufferId_ )
		{
			PWARNING << "[opengl] Invalid attempt to lock a uninitialized Vertex Buffer.\n";
			return 0;
		}

		if( length == 0 )
		{
			length = bufferSize_;
		}

		if( offset + length > bufferSize_ )
		{
			PWARNING << "[opengl] offset+length > size in VertexBuffer::lock()!\n";
			return 0;
		}

		g_VboFunctions->bindBuffer( GL_ARRAY_BUFFER_ARB, bufferId_ );		

		const bool bufferIsWriteOnly = 
			( bufferUsage_ == BufferUsage_Dynamic_WriteOnly ) ||
			( bufferUsage_ == BufferUsage_Stream_WriteOnly ) || 
			( bufferUsage_ == BufferUsage_Static_WriteOnly );

		GLenum accessMode = GL_READ_WRITE_ARB;

		if( bufferIsWriteOnly )
		{
			accessMode = GL_WRITE_ONLY_ARB;
		}

		if( lockOption == BufferLockOption_ReadOnly )
		{
			if( bufferIsWriteOnly )
			{
				PWARNING << "[opengl] Could not lock write-only buffer for reading!\n";
				return 0;
			}
			accessMode = GL_READ_ONLY_ARB;
		}
		else if( lockOption == BufferLockOption_Discard )
		{
			// Recreate Buffer (this will discard the data (and possibly give us a completely new one)
			g_VboFunctions->bufferData( GL_ARRAY_BUFFER_ARB, bufferSize_, 0, glUsage_ );
		}
		else if( ( lockOption == BufferLockOption_NoOverwrite ) || ( lockOption == BufferLockOption_ReadWrite ) )
		{
			// do nothing..
		}
		else
		{
			PWARNING << "[opengl] Invalid LockOption in GpuBuffer::lock()!\n";
			return 0;
		}

		void* result = g_VboFunctions->mapBuffer( GL_ARRAY_BUFFER_ARB, accessMode );

		if( !result )
		{
			PWARNING << "[opengl] could not map buffer object!\n";
			return 0;
		}

		isLocked_ = true;

		return static_cast< void* >( static_cast< char* >( result ) + offset );
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuBuffer::unlock()
	{
		if( !isLocked_ )
		{
			return true;
		}

		if( !bufferId_ )
		{
			PWARNING << "[opengl] Invalid attempt to unlock a uninitialized Vertex Buffer.\n";
			return false;
		}
		g_VboFunctions->bindBuffer( GL_ARRAY_BUFFER_ARB, bufferId_ );

		if( !g_VboFunctions->unmapBuffer( GL_ARRAY_BUFFER_ARB ) )
		{
			PWARNING << "[opengl] VertexBuffer Data corrupted.\n";
			return false;
		}

		isLocked_ = false;
		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuBuffer::isLocked() const
	{
		return isLocked_;
	}


	//-------------------------------------------------------------------------------------------------


	int GpuBuffer::getSize() const
	{
		return bufferSize_;
	}


	//-------------------------------------------------------------------------------------------------


	GLuint GpuBuffer::getBufferId() const
	{
		return bufferId_;
	}


	//-------------------------------------------------------------------------------------------------


	GLenum GpuBuffer::getBufferType() const
	{
		return glType_;
	}


	//-------------------------------------------------------------------------------------------------
	
}

//-------------------------------------------------------------------------------------------------
