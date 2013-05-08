//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		20.11.2004 20:05:46
//
//	Description			:		OSGStatelessGlslShader.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef GLSL_SHADER_INCLUDED
#define GLSL_SHADER_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include <OSGWindow.h>
#include <string>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	typedef unsigned int GLhandleARB;

	struct GlslFunctions;

	class OSG_CONTRIBLIB_DLLMAPPING GlslShader
	{
	public:
		GlslShader();
		~GlslShader();

		bool				create( Window* window, const char* vertexShaderText, const char* fragmentShaderText, std::string& errorMsg );
		void				destroy();

		bool				createFromFile( Window* window, const char* shaderFilename );

		bool				isValid() const;

		void				activate();
		void				deactivate();

		bool				setUniform( const char* name, const Pnt2f& value );
		bool				setUniform( const char* name, const Pnt3f& value );
		bool				setUniform( const char* name, const Pnt4f& value );
		bool				setUniform( const char* name, const Matrix4f& value, bool transpose = false );
		bool				setUniform( const char* name, const Color3f& value );
		bool				setUniform( const char* name, float value );

		bool				setSampler( const char* name, GLint stage );

	private:
		GLhandleARB			compileShader( GLenum type, const char* text, std::string& errorMsg );

		GlslFunctions*		extensionFunctions_;

		GLhandleARB			vertexShader_;
		GLhandleARB			fragmentShader_;
		GLhandleARB			program_;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef GLSL_SHADER_INCLUDED

//-------------------------------------------------------------------------------------------------

