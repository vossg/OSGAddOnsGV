//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		20.11.2004 20:10:36
//
//	Description			:		OSGGlslShader.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"
#include "OSGStatelessGlslShader.h"
#include <OSGGLEXT.h>
#include <cassert>
#include "OSGTerrainTools.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	typedef char GLcharARB;

	typedef void 		(OSG_APIENTRY * OSGGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
	typedef void 		(OSG_APIENTRY * OSGGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
	typedef GLhandleARB (OSG_APIENTRY * OSGGLCREATEPROGRAMOBJECTARBPROC) (void);
	typedef GLhandleARB (OSG_APIENTRY * OSGGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
	typedef void 		(OSG_APIENTRY * OSGGLDELETEOBJECTARBPROC) (GLhandleARB obj);
	typedef void 		(OSG_APIENTRY * OSGGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
	typedef void 		(OSG_APIENTRY * OSGGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
	typedef void 		(OSG_APIENTRY * OSGGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
	typedef GLhandleARB (OSG_APIENTRY * OSGGLGETHANDLEARBPROC) (GLenum pname);
	typedef void 		(OSG_APIENTRY * OSGGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
	typedef void 		(OSG_APIENTRY * OSGGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat* params);
	typedef void 		(OSG_APIENTRY * OSGGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
	typedef void 		(OSG_APIENTRY * OSGGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
	typedef GLint		(OSG_APIENTRY * OSGGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
	typedef void 		(OSG_APIENTRY * OSGGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat* params);
	typedef void 		(OSG_APIENTRY * OSGGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint* params);
	typedef void 		(OSG_APIENTRY * OSGGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
	typedef void 		(OSG_APIENTRY * OSGGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM1IARBPROC) (GLint location, GLint v0);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, const GLint* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, const GLint* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, const GLint* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, const GLint* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void 		(OSG_APIENTRY * OSGGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
	typedef void 		(OSG_APIENTRY * OSGGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);

	//-------------------------------------------------------------------------------------------------
	
	struct GlslFunctions
	{
		GlslFunctions();
		bool								initFunctionPointers( Window* window );
		void								clearFunctionPointers();

		OSGGLATTACHOBJECTARBPROC			attachObject;
		OSGGLCOMPILESHADERARBPROC			compileShader;
		OSGGLCREATEPROGRAMOBJECTARBPROC		createProgramObject;
		OSGGLCREATESHADEROBJECTARBPROC		createShaderObject;
		OSGGLDELETEOBJECTARBPROC			deleteObject;
		OSGGLDETACHOBJECTARBPROC			detachObject;
		OSGGLGETACTIVEUNIFORMARBPROC		getActiveUniform;
		OSGGLGETATTACHEDOBJECTSARBPROC		getAttachedObjects;
		OSGGLGETHANDLEARBPROC				getHandle;
		OSGGLGETINFOLOGARBPROC				getInfoLog;
		OSGGLGETOBJECTPARAMETERFVARBPROC	getObjectParameterfv;
		OSGGLGETOBJECTPARAMETERIVARBPROC	getObjectParameteriv;
		OSGGLGETSHADERSOURCEARBPROC			getShaderSource;
		OSGGLGETUNIFORMLOCATIONARBPROC		getUniformLocation;
		OSGGLGETUNIFORMFVARBPROC			getUniformfv;
		OSGGLGETUNIFORMIVARBPROC			getUniformiv;
		OSGGLLINKPROGRAMARBPROC				linkProgram;
		OSGGLSHADERSOURCEARBPROC			shaderSource;
		OSGGLUNIFORM1FARBPROC				uniform1f;
		OSGGLUNIFORM1FVARBPROC				uniform1fv;
		OSGGLUNIFORM1IARBPROC				uniform1i;
		OSGGLUNIFORM1IVARBPROC				uniform1iv;
		OSGGLUNIFORM2FARBPROC				uniform2f;
		OSGGLUNIFORM2FVARBPROC				uniform2fv;
		OSGGLUNIFORM2IARBPROC				uniform2i;
		OSGGLUNIFORM2IVARBPROC				uniform2iv;
		OSGGLUNIFORM3FARBPROC				uniform3f;
		OSGGLUNIFORM3FVARBPROC				uniform3fv;
		OSGGLUNIFORM3IARBPROC				uniform3i;
		OSGGLUNIFORM3IVARBPROC				uniform3iv;
		OSGGLUNIFORM4FARBPROC				uniform4f;
		OSGGLUNIFORM4FVARBPROC				uniform4fv;
		OSGGLUNIFORM4IARBPROC				uniform4i;
		OSGGLUNIFORM4IVARBPROC				uniform4iv;
		OSGGLUNIFORMMATRIX2FVARBPROC		uniformMatrix2fv;
		OSGGLUNIFORMMATRIX3FVARBPROC		uniformMatrix3fv;
		OSGGLUNIFORMMATRIX4FVARBPROC		uniformMatrix4fv;
		OSGGLUSEPROGRAMOBJECTARBPROC		useProgramObject;
		OSGGLVALIDATEPROGRAMARBPROC			validateProgram;

	private:
		static UInt32 						extensionId_;

		static UInt32						attachObjectId_;
		static UInt32						compileShaderId_;
		static UInt32						createProgramObjectId_;
		static UInt32						createShaderObjectId_;
		static UInt32						deleteObjectId_;
		static UInt32						detachObjectId_;
		static UInt32						getActiveUniformId_;
		static UInt32						getAttachedObjectsId_;
		static UInt32						getHandleId_;
		static UInt32						getInfoLogId_;
		static UInt32						getObjectParameterfvId_;
		static UInt32						getObjectParameterivId_;
		static UInt32						getShaderSourceId_;
		static UInt32						getUniformLocationId_;
		static UInt32						getUniformfvId_;
		static UInt32						getUniformivId_;
		static UInt32						linkProgramId_;
		static UInt32						shaderSourceId_;
		static UInt32						uniform1fId_;
		static UInt32						uniform1fvId_;
		static UInt32						uniform1iId_;
		static UInt32						uniform1ivId_;
		static UInt32						uniform2fId_;
		static UInt32						uniform2fvId_;
		static UInt32						uniform2iId_;
		static UInt32						uniform2ivId_;
		static UInt32						uniform3fId_;
		static UInt32						uniform3fvId_;
		static UInt32						uniform3iId_;
		static UInt32						uniform3ivId_;
		static UInt32						uniform4fId_;
		static UInt32						uniform4fvId_;
		static UInt32						uniform4iId_;
		static UInt32						uniform4ivId_;
		static UInt32						uniformMatrix2fvId_;
		static UInt32						uniformMatrix3fvId_;
		static UInt32						uniformMatrix4fvId_;
		static UInt32						useProgramObjectId_;
		static UInt32						validateProgramId_;
	};

	//-------------------------------------------------------------------------------------------------

	UInt32 GlslFunctions::extensionId_ = 0;

	UInt32 GlslFunctions::attachObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::compileShaderId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::createProgramObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::createShaderObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::deleteObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::detachObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getActiveUniformId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getAttachedObjectsId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getHandleId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getInfoLogId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getObjectParameterfvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getObjectParameterivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getShaderSourceId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getUniformLocationId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getUniformfvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::getUniformivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::linkProgramId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::shaderSourceId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform1fId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform1fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform1iId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform1ivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform2fId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform2fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform2iId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform2ivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform3fId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform3fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform3iId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform3ivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform4fId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform4fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform4iId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniform4ivId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniformMatrix2fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniformMatrix3fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::uniformMatrix4fvId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::useProgramObjectId_ = Window::invalidFunctionID;
	UInt32 GlslFunctions::validateProgramId_ = Window::invalidFunctionID;


	//-------------------------------------------------------------------------------------------------


	GlslFunctions::GlslFunctions()
	{
		// register extensions: (make this static/once only?!)
		if( extensionId_ == 0 )
		{
			extensionId_			= Window::registerExtension( "GL_ARB_shader_objects" );

			attachObjectId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glAttachObjectARB", extensionId_ );
			compileShaderId_		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glCompileShaderARB", extensionId_ );
			createProgramObjectId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glCreateProgramObjectARB", extensionId_ );
			createShaderObjectId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glCreateShaderObjectARB", extensionId_ );
			deleteObjectId_ 		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glDeleteObjectARB", extensionId_ );
			detachObjectId_ 		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glDetachObjectARB", extensionId_ );
			getActiveUniformId_		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetActiveUniformARB", extensionId_ );
			getAttachedObjectsId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetAttachedObjectsARB", extensionId_ );
			getHandleId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetHandleARB", extensionId_ );
			getInfoLogId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetInfoLogARB", extensionId_ );
			getObjectParameterfvId_ = Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetObjectParameterfvARB", extensionId_ );
			getObjectParameterivId_ = Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetObjectParameterivARB", extensionId_ );
			getShaderSourceId_		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetShaderSourceARB", extensionId_ );
			getUniformLocationId_	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetUniformLocationARB", extensionId_ );
			getUniformfvId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetUniformfvARB", extensionId_ );
			getUniformivId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glGetUniformivARB", extensionId_ );
			linkProgramId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glLinkProgramARB", extensionId_ );
			shaderSourceId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glShaderSourceARB", extensionId_ );
			uniform1fId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform1fARB", extensionId_ );
			uniform1fvId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform1fvARB", extensionId_ );
			uniform1iId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform1iARB", extensionId_ );
			uniform1ivId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform1ivARB", extensionId_ );
			uniform2fId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform2fARB", extensionId_ );
			uniform2fvId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform2fvARB", extensionId_ );
			uniform2iId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform2iARB", extensionId_ );
			uniform2ivId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform2ivARB", extensionId_ );
			uniform3fId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform3fARB", extensionId_ );
			uniform3fvId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform3fvARB", extensionId_ );
			uniform3iId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform3iARB", extensionId_ );
			uniform3ivId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform3ivARB", extensionId_ );
			uniform4fId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform4fARB", extensionId_ );
			uniform4fvId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform4fvARB", extensionId_ );
			uniform4iId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform4iARB", extensionId_ );
			uniform4ivId_			= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniform4ivARB", extensionId_ );
			uniformMatrix2fvId_ 	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniformMatrix2fvARB", extensionId_ );
			uniformMatrix3fvId_ 	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniformMatrix3fvARB", extensionId_ );
			uniformMatrix4fvId_ 	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUniformMatrix4fvARB", extensionId_ );
			useProgramObjectId_ 	= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glUseProgramObjectARB", extensionId_ );
			validateProgramId_		= Window::registerFunction( OSG_DLSYM_UNDERSCORE "glValidateProgramARB", extensionId_ );
		}
		clearFunctionPointers();
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslFunctions::initFunctionPointers( Window* window )
	{
		assert( window );

		attachObject			= (OSGGLATTACHOBJECTARBPROC) window->getFunction( attachObjectId_ );
		compileShader			= (OSGGLCOMPILESHADERARBPROC) window->getFunction( compileShaderId_ );
		createProgramObject		= (OSGGLCREATEPROGRAMOBJECTARBPROC) window->getFunction( createProgramObjectId_ );
		createShaderObject		= (OSGGLCREATESHADEROBJECTARBPROC) window->getFunction( createShaderObjectId_ );
		deleteObject			= (OSGGLDELETEOBJECTARBPROC) window->getFunction( deleteObjectId_ );
		detachObject			= (OSGGLDETACHOBJECTARBPROC) window->getFunction( detachObjectId_ );
		getActiveUniform		= (OSGGLGETACTIVEUNIFORMARBPROC) window->getFunction( getActiveUniformId_ );
		getAttachedObjects		= (OSGGLGETATTACHEDOBJECTSARBPROC) window->getFunction( getAttachedObjectsId_ );
		getHandle				= (OSGGLGETHANDLEARBPROC) window->getFunction( getHandleId_ );
		getInfoLog				= (OSGGLGETINFOLOGARBPROC) window->getFunction( getInfoLogId_ );
		getObjectParameterfv	= (OSGGLGETOBJECTPARAMETERFVARBPROC) window->getFunction( getObjectParameterfvId_ );
		getObjectParameteriv	= (OSGGLGETOBJECTPARAMETERIVARBPROC) window->getFunction( getObjectParameterivId_ );
		getShaderSource			= (OSGGLGETSHADERSOURCEARBPROC) window->getFunction( getShaderSourceId_ );
		getUniformLocation		= (OSGGLGETUNIFORMLOCATIONARBPROC) window->getFunction( getUniformLocationId_ );
		getUniformfv			= (OSGGLGETUNIFORMFVARBPROC) window->getFunction( getUniformfvId_ );
		getUniformiv			= (OSGGLGETUNIFORMIVARBPROC) window->getFunction( getUniformivId_ );
		linkProgram				= (OSGGLLINKPROGRAMARBPROC) window->getFunction( linkProgramId_ );
		shaderSource			= (OSGGLSHADERSOURCEARBPROC) window->getFunction( shaderSourceId_ );
		uniform1f				= (OSGGLUNIFORM1FARBPROC) window->getFunction( uniform1fId_ );
		uniform1fv				= (OSGGLUNIFORM1FVARBPROC) window->getFunction( uniform1fvId_ );
		uniform1i				= (OSGGLUNIFORM1IARBPROC) window->getFunction( uniform1iId_ );
		uniform1iv				= (OSGGLUNIFORM1IVARBPROC) window->getFunction( uniform1ivId_ );
		uniform2f				= (OSGGLUNIFORM2FARBPROC) window->getFunction( uniform2fId_ );
		uniform2fv				= (OSGGLUNIFORM2FVARBPROC) window->getFunction( uniform2fvId_ );
		uniform2i				= (OSGGLUNIFORM2IARBPROC) window->getFunction( uniform2iId_ );
		uniform2iv				= (OSGGLUNIFORM2IVARBPROC) window->getFunction( uniform2ivId_ );
		uniform3f				= (OSGGLUNIFORM3FARBPROC) window->getFunction( uniform3fId_ );
		uniform3fv				= (OSGGLUNIFORM3FVARBPROC) window->getFunction( uniform3fvId_ );
		uniform3i				= (OSGGLUNIFORM3IARBPROC) window->getFunction( uniform3iId_ );
		uniform3iv				= (OSGGLUNIFORM3IVARBPROC) window->getFunction( uniform3ivId_ );
		uniform4f				= (OSGGLUNIFORM4FARBPROC) window->getFunction( uniform4fId_ );
		uniform4fv				= (OSGGLUNIFORM4FVARBPROC) window->getFunction( uniform4fvId_ );
		uniform4i				= (OSGGLUNIFORM4IARBPROC) window->getFunction( uniform4iId_ );
		uniform4iv				= (OSGGLUNIFORM4IVARBPROC) window->getFunction( uniform4ivId_ );
		uniformMatrix2fv		= (OSGGLUNIFORMMATRIX2FVARBPROC) window->getFunction( uniformMatrix2fvId_ );
		uniformMatrix3fv		= (OSGGLUNIFORMMATRIX3FVARBPROC) window->getFunction( uniformMatrix3fvId_ );
		uniformMatrix4fv		= (OSGGLUNIFORMMATRIX4FVARBPROC) window->getFunction( uniformMatrix4fvId_ );
		useProgramObject		= (OSGGLUSEPROGRAMOBJECTARBPROC) window->getFunction( useProgramObjectId_ );
		validateProgram			= (OSGGLVALIDATEPROGRAMARBPROC) window->getFunction( validateProgramId_ );

		return ( attachObject != 0 ) && ( compileShader != 0 ) && ( createProgramObject != 0 ) && ( createShaderObject != 0 ) && 
			( deleteObject != 0 ) && ( detachObject != 0 ) && ( getActiveUniform != 0 ) && ( getAttachedObjects != 0 ) && 
			( getHandle != 0 ) && ( getInfoLog != 0 ) && ( getObjectParameterfv != 0 ) && ( getObjectParameteriv != 0 ) && 
			( getShaderSource != 0 ) && ( getUniformLocation != 0 ) && ( getUniformfv != 0 ) && ( getUniformiv != 0 ) && 
			( linkProgram != 0 ) && ( shaderSource != 0 ) && ( uniform1f != 0 ) && ( uniform1fv != 0 ) && ( uniform1i != 0 ) && 
			( uniform1iv != 0 ) && ( uniform2f != 0 ) && ( uniform2fv != 0 ) && ( uniform2i != 0 ) && ( uniform2iv != 0 ) && 
			( uniform3f != 0 ) && ( uniform3fv != 0 ) && ( uniform3i != 0 ) && ( uniform3iv != 0 ) && ( uniform4f != 0 ) && 
			( uniform4fv != 0 ) && ( uniform4i != 0 ) && ( uniform4iv != 0 ) && ( uniformMatrix2fv != 0 ) && ( uniformMatrix3fv != 0 ) && 
			( uniformMatrix4fv != 0 ) && ( useProgramObject != 0 ) && ( validateProgram != 0 );
	}


	//-------------------------------------------------------------------------------------------------


	void GlslFunctions::clearFunctionPointers()
	{
		attachObject = 0;
		compileShader = 0;
		createProgramObject = 0;
		createShaderObject = 0;
		deleteObject = 0;
		detachObject = 0;
		getActiveUniform = 0;
		getAttachedObjects = 0;
		getHandle = 0;
		getInfoLog = 0;
		getObjectParameterfv = 0;
		getObjectParameteriv = 0;
		getShaderSource = 0;
		getUniformLocation = 0;
		getUniformfv = 0;
		getUniformiv = 0;
		linkProgram = 0;
		shaderSource = 0;
		uniform1f = 0;
		uniform1fv = 0;
		uniform1i = 0;
		uniform1iv = 0;
		uniform2f = 0;
		uniform2fv = 0;
		uniform2i = 0;
		uniform2iv = 0;
		uniform3f = 0;
		uniform3fv = 0;
		uniform3i = 0;
		uniform3iv = 0;
		uniform4f = 0;
		uniform4fv = 0;
		uniform4i = 0;
		uniform4iv = 0;
		uniformMatrix2fv = 0;
		uniformMatrix3fv = 0;
		uniformMatrix4fv = 0;
		useProgramObject = 0;
		validateProgram = 0;
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	
	GlslShader::GlslShader()
	{
		program_		= 0;
		vertexShader_	= 0;
		fragmentShader_	= 0;

		extensionFunctions_ = new GlslFunctions();
	}


	//-------------------------------------------------------------------------------------------------


	GlslShader::~GlslShader()
	{
		delete extensionFunctions_;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::create( Window* window, const char* vertexShaderText, const char* fragmentShaderText, std::string& errorMsg )
	{
		assert( window );

		destroy();

		// Get Function Pointers from Window (GL-Context):
		if( !extensionFunctions_->initFunctionPointers( window ) )
		{
			PWARNING << "OSGGlslShader: GLSL not supported!\n";
			return false;
		}

		vertexShader_ = compileShader( GL_VERTEX_SHADER_ARB, vertexShaderText, errorMsg );
		if( !vertexShader_ )
		{
			PWARNING << "OSGGlslShader: could not compile vertex Shader : " << errorMsg << "\n";
			return false;
		}
		
		fragmentShader_ = compileShader( GL_FRAGMENT_SHADER_ARB, fragmentShaderText, errorMsg );
		if( !fragmentShader_ )
		{
			PWARNING << "OSGGlslShader: could not compile Fragment Shader : " << errorMsg << "\n";
			extensionFunctions_->deleteObject( vertexShader_ );
			return false;
		}

		program_ = extensionFunctions_->createProgramObject();

		if( vertexShader_ )
		{
			extensionFunctions_->attachObject( program_, vertexShader_ );
			extensionFunctions_->deleteObject( vertexShader_ );
		}
		if( fragmentShader_ )
		{
			extensionFunctions_->attachObject( program_, fragmentShader_ );
			extensionFunctions_->deleteObject( fragmentShader_ );
		}

		extensionFunctions_->linkProgram( program_ );
		
		GLint linked = 0;
		extensionFunctions_->getObjectParameteriv( program_, GL_OBJECT_LINK_STATUS_ARB, &linked );

		if( !linked )
		{
			//logWarning( "[opengl] Could not link GLSL Shader!\n" );
			GLint infoLength = 0;
			extensionFunctions_->getObjectParameteriv( program_, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLength );

			if( infoLength > 1 )
			{				
				errorMsg.resize( infoLength );
				extensionFunctions_->getInfoLog( program_, ( GLsizei )errorMsg.size(), 0, &errorMsg[ 0 ] );				
			}

			PWARNING << "OSGGlslShader: could not link GLSL Shader : " << errorMsg << "\n";

			extensionFunctions_->deleteObject( program_ );
			program_ = 0;
			return false;
		}

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	void GlslShader::destroy()
	{
		if( program_ )
		{
			extensionFunctions_->deleteObject( program_ );
			program_		= 0;
			vertexShader_	= 0;
			fragmentShader_	= 0;
		}
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::createFromFile( Window* window, const char* shaderFilename )
	{
		std::string shaderTextBuffer;

		if( !loadTextFile( shaderFilename, shaderTextBuffer ) )
		{
			return false;
		}

		const char* vertexShaderTag = "[VertexShader]";
		
		std::string::size_type startOfVertexShader = shaderTextBuffer.find( vertexShaderTag, 0 );

		if( startOfVertexShader == std::string::npos )
		{
			PWARNING << "[DynamicTerrain] " << shaderFilename << "(0) : error : Missing VertexShader-Tag (" << vertexShaderTag << ")\n";
			return false;
		}

		const char* fragmentShaderTag = "[FragmentShader]";

		std::string::size_type startOfFragmentShader = shaderTextBuffer.find( fragmentShaderTag, startOfVertexShader );

		if( startOfFragmentShader == std::string::npos )
		{
			PWARNING << "[DynamicTerrain] " << shaderFilename << "(0) : error : Missing FragmentShader-Tag (" << fragmentShaderTag << ")\n";
			return false;
		}

		assert( startOfFragmentShader > startOfVertexShader );

		const size_t vertexShaderSize = startOfFragmentShader - startOfVertexShader - strlen( vertexShaderTag );
		const size_t fragmentShaderSize = shaderTextBuffer.size() - startOfFragmentShader - strlen( fragmentShaderTag );

		startOfVertexShader		+= strlen( vertexShaderTag );
		startOfFragmentShader	+= strlen( fragmentShaderTag );

		std::string vertexShaderTextBuffer( shaderTextBuffer, startOfVertexShader, vertexShaderSize );
		std::string fragmentShaderTextBuffer( shaderTextBuffer, startOfFragmentShader, fragmentShaderSize );

		std::string errorMsg;

		if( !create( window, vertexShaderTextBuffer.c_str(), fragmentShaderTextBuffer.c_str(), errorMsg ) )		
		{
			PWARNING << "[DynamicTerrain] " << "(0) : error : " << errorMsg << "\n";

			return false;
		}		
		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::isValid() const
	{
		return program_ != 0;
	}


	//-------------------------------------------------------------------------------------------------


	void GlslShader::activate()
	{
		extensionFunctions_->useProgramObject( program_ );
	}


	//-------------------------------------------------------------------------------------------------


	void GlslShader::deactivate()
	{
		extensionFunctions_->useProgramObject( 0 );
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, const Pnt2f& value )
	{	
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform2fv( pos, 1, &value[ 0 ] );
		
		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, const Pnt3f& value )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform3fv( pos, 1, &value[ 0 ] );

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, const Pnt4f& value )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform4fv( pos, 1, &value[ 0 ] );

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, const Matrix4f& value, bool transpose )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniformMatrix4fv( pos, 1, transpose, value.getValues() );

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, const Color3f& value )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform3f( pos, value[ 0 ], value[ 1 ], value[ 2 ] );

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setUniform( const char* name, float value )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Uniform Shader Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform1f( pos, value );

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	bool GlslShader::setSampler( const char* name, GLint stage )
	{
		const GLint pos = extensionFunctions_->getUniformLocation( program_, name );

		if( pos < 0 )
		{
			//logWarning( "[opengl] Could not locate Shader Sampler Parameter '%s'\n", name );
			return false;
		}

		extensionFunctions_->uniform1i( pos, stage );

		return true;
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	GLhandleARB GlslShader::compileShader( GLenum type, const char* text, std::string& errorMsg )
	{
		GLhandleARB shader = 0;

		if( text )
		{
			shader = extensionFunctions_->createShaderObject( type );

			extensionFunctions_->shaderSource( shader, 1, &text, 0 );
			extensionFunctions_->compileShader( shader );

			GLint compiled = 0;
			extensionFunctions_->getObjectParameteriv( shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );

			if( !compiled )
			{
				GLint infoLength = 0;
				extensionFunctions_->getObjectParameteriv( shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLength );

				if( infoLength > 1 )
				{				
					errorMsg.resize( infoLength );
					extensionFunctions_->getInfoLog( shader, ( GLsizei )errorMsg.size(), 0, &errorMsg[ 0 ] );				
				}
				extensionFunctions_->deleteObject( shader );
				shader = 0;
			}
		}
		return shader;
	}


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------
