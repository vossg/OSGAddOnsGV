//------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		21.04.2006
//
//	Description			:		OSGGpuBuffer.h
//
//-------------------------------------------------------------------------------------------------

#ifndef __GPUBUFFER_H__
#define __GPUBUFFER_H__

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include <OSGWindow.h>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	enum BufferUsage
	{
		BufferUsage_Static_WriteOnly,
		BufferUsage_Static_ReadWrite,
		BufferUsage_Stream_WriteOnly,
		BufferUsage_Stream_ReadWrite,
		BufferUsage_Dynamic_WriteOnly,
		BufferUsage_Dynamic_ReadWrite,
		BufferUsage_Count
	};

	enum BufferLockOption
	{
		BufferLockOption_ReadWrite,
		BufferLockOption_Discard,
		BufferLockOption_ReadOnly,
		BufferLockOption_NoOverwrite,
		BufferLockOption_Count
	};


	//-------------------------------------------------------------------------------------------------

	struct VboFunctions;

	class OSG_CONTRIBLIB_DLLMAPPING GpuBuffer
	{
	public:
		GpuBuffer();
		virtual				~GpuBuffer();

		bool				create( Window* window, GLenum type, int size, BufferUsage usage );
		void				destroy();

		bool				isValid() const;

		bool				uploadData( const void* sourceData, int targetOffset, int size );

		void*				lock( int offset, int length, BufferLockOption lockOption );
		bool				unlock();

		bool				isLocked() const;

		int					getSize() const;

		GLuint				getBufferId() const;
		GLenum				getBufferType() const;

		void				activate();
		void				deactivate();

	private:
		Window*				window_;
		GLenum				glType_;
		GLenum				glUsage_;
		GLuint				bufferId_;
		int					bufferSize_;
		BufferUsage			bufferUsage_;
		bool				isLocked_;
	};

	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif

//-------------------------------------------------------------------------------------------------
