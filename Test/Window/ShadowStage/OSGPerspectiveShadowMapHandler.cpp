#include <stdlib.h>
#include <stdio.h>
#include <OSGConfig.h>
#include <OSGQuaternion.h>
#include <OSGRenderAction.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBackground.h>
#include <OSGForeground.h>
#include <OSGGrabForeground.h>
#include <OSGPolygonForeground.h>
#include <OSGPolygonBackground.h>
#include <OSGTextureGrabForeground.h>
#include <OSGFileGrabForeground.h>
#include <OSGImageForeground.h>
#include <OSGImage.h>
#include <OSGGeometry.h>
#include <OSGSimpleGeometry.h>
#include <OSGLight.h>
#include <OSGMaterialGroup.h>
#include "OSGPerspectiveShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"



#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"

#define USE_FBO_FOR_COLOR_AND_FACTOR_MAP

//--------------------------------------------------------------------
#ifndef GL_CLAMP_TO_EDGE
#   define GL_CLAMP_TO_EDGE    0x812F
#endif
#ifndef GL_CLAMP_TO_BORDER_ARB
#   define GL_CLAMP_TO_BORDER_ARB  0x812D
#endif
#ifndef GL_ARB_depth_texture
#   define GL_DEPTH_COMPONENT16_ARB    0x81A5
#   define GL_DEPTH_COMPONENT24_ARB    0x81A6
#   define GL_DEPTH_COMPONENT32_ARB    0x81A7
#   define GL_TEXTURE_DEPTH_SIZE_ARB   0x884A
#   define GL_DEPTH_TEXTURE_MODE_ARB   0x884B
#endif
#ifndef GL_ARB_shadow
#   define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#   define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#   define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E
#endif
#ifndef GL_SGIX_depth_texture
#   define GL_DEPTH_COMPONENT16_SGIX   0x81A5
#   define GL_DEPTH_COMPONENT24_SGIX   0x81A6
#   define GL_DEPTH_COMPONENT32_SGIX   0x81A7
#endif
#ifndef GL_SGIX_shadow
#   define GL_TEXTURE_COMPARE_SGIX             0x819A
#   define GL_TEXTURE_COMPARE_OPERATOR_SGIX    0x819B
#   define GL_TEXTURE_LEQUAL_R_SGIX            0x819C
#   define GL_TEXTURE_GEQUAL_R_SGIX            0x819D
#endif
#ifndef GL_DEPTH_COMPONENT_ARB
#   define GL_DEPTH_COMPONENT_ARB            0x1902
#endif
#if 1 //GL_ARB_texture_float
#	define	GL_RGBA32F_ARB                      0x8814
#	define	GL_RGB32F_ARB                       0x8815
#	define	GL_ALPHA32F_ARB                     0x8816
#	define	GL_INTENSITY32F_ARB                 0x8817
#	define	GL_LUMINANCE32F_ARB                 0x8818
#	define	GL_LUMINANCE_ALPHA32F_ARB           0x8819
#	define	GL_RGBA16F_ARB                      0x881A
#	define	GL_RGB16F_ARB                       0x881B
#	define	GL_ALPHA16F_ARB                     0x881C
#	define	GL_INTENSITY16F_ARB                 0x881D
#	define	GL_LUMINANCE16F_ARB                 0x881E
#	define	GL_LUMINANCE_ALPHA16F_ARB           0x881F
#endif

/* ----------------------- GL_EXT_framebuffer_object ----------------------- */

#ifndef GL_EXT_framebuffer_object
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_FRAMEBUFFER_STATUS_ERROR_EXT 0x8CDE
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_STENCIL_INDEX_EXT 0x8D45
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48
#define GL_STENCIL_INDEX16_EXT 0x8D49
#endif

// Some installations have a typo in the name for this one
#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#endif

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

#ifndef GL_DEPTH_COMPONENT_ARB
    #define GL_DEPTH_COMPONENT_ARB 0x1902
#endif

#ifndef GL_ARB_depth_texture
#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B
#endif

#ifndef GL_ARB_shadow
#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E
#endif


OSG_USING_NAMESPACE

static std::string _lisp_shadow_vp =
    "uniform mat4 lightPM;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow_fp =
    "uniform sampler2DShadow shadowMap;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor,mapFactor),projectiveBiased.z)).x) * intensity;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.5,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadow2_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow2_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadow3_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow3_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2DShadow shadowMap3;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform float intensity3;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "uniform float mapFactor3;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	vec4 projectiveBiased3 = vec4((projCoord3.xyz / projCoord3.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap3, vec3(vec2(projectiveBiased3.xy) * vec2(mapFactor3,mapFactor3),projectiveBiased3.z)).x) * intensity3;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";


static std::string _lisp_shadow4_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow4_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2DShadow shadowMap3;\n"
    "uniform sampler2DShadow shadowMap4;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform float intensity3;\n"
    "uniform float intensity4;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "uniform float mapFactor3;\n"
    "uniform float mapFactor4;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	vec4 projectiveBiased3 = vec4((projCoord3.xyz / projCoord3.q),1.0);\n"
    "	vec4 projectiveBiased4 = vec4((projCoord4.xyz / projCoord4.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap3, vec3(vec2(projectiveBiased3.xy) * vec2(mapFactor3,mapFactor3),projectiveBiased3.z)).x) * intensity3;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap4, vec3(vec2(projectiveBiased4.xy) * vec2(mapFactor4,mapFactor4),projectiveBiased4.z)).x) * intensity4;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadow5_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord5 = lightPM5 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  projCoord5 = bias * projCoord5;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow5_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2DShadow shadowMap3;\n"
    "uniform sampler2DShadow shadowMap4;\n"
    "uniform sampler2DShadow shadowMap5;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform float intensity3;\n"
    "uniform float intensity4;\n"
    "uniform float intensity5;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "uniform float mapFactor3;\n"
    "uniform float mapFactor4;\n"
    "uniform float mapFactor5;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	vec4 projectiveBiased3 = vec4((projCoord3.xyz / projCoord3.q),1.0);\n"
    "	vec4 projectiveBiased4 = vec4((projCoord4.xyz / projCoord4.q),1.0);\n"
    "	vec4 projectiveBiased5 = vec4((projCoord5.xyz / projCoord5.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap3, vec3(vec2(projectiveBiased3.xy) * vec2(mapFactor3,mapFactor3),projectiveBiased3.z)).x) * intensity3;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap4, vec3(vec2(projectiveBiased4.xy) * vec2(mapFactor4,mapFactor4),projectiveBiased4.z)).x) * intensity4;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap5, vec3(vec2(projectiveBiased5.xy) * vec2(mapFactor5,mapFactor5),projectiveBiased5.z)).x) * intensity5;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadow6_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "uniform mat4 lightPM6;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 projCoord6;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord5 = lightPM5 * realPos;\n"
    "  projCoord6 = lightPM6 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  projCoord5 = bias * projCoord5;\n"
    "  projCoord6 = bias * projCoord6;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow6_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2DShadow shadowMap3;\n"
    "uniform sampler2DShadow shadowMap4;\n"
    "uniform sampler2DShadow shadowMap5;\n"
    "uniform sampler2DShadow shadowMap6;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform float intensity3;\n"
    "uniform float intensity4;\n"
    "uniform float intensity5;\n"
    "uniform float intensity6;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "uniform float mapFactor3;\n"
    "uniform float mapFactor4;\n"
    "uniform float mapFactor5;\n"
    "uniform float mapFactor6;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 projCoord6;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	vec4 projectiveBiased3 = vec4((projCoord3.xyz / projCoord3.q),1.0);\n"
    "	vec4 projectiveBiased4 = vec4((projCoord4.xyz / projCoord4.q),1.0);\n"
    "	vec4 projectiveBiased5 = vec4((projCoord5.xyz / projCoord5.q),1.0);\n"
    "	vec4 projectiveBiased6 = vec4((projCoord6.xyz / projCoord6.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap3, vec3(vec2(projectiveBiased3.xy) * vec2(mapFactor3,mapFactor3),projectiveBiased3.z)).x) * intensity3;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap4, vec3(vec2(projectiveBiased4.xy) * vec2(mapFactor4,mapFactor4),projectiveBiased4.z)).x) * intensity4;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap5, vec3(vec2(projectiveBiased5.xy) * vec2(mapFactor5,mapFactor5),projectiveBiased5.z)).x) * intensity5;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap6, vec3(vec2(projectiveBiased6.xy) * vec2(mapFactor6,mapFactor6),projectiveBiased6.z)).x) * intensity6;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadow7_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "uniform mat4 lightPM6;\n"
    "uniform mat4 lightPM7;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 projCoord6;\n"
    "varying vec4 projCoord7;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord5 = lightPM5 * realPos;\n"
    "  projCoord6 = lightPM6 * realPos;\n"
    "  projCoord7 = lightPM7 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  projCoord5 = bias * projCoord5;\n"
    "  projCoord6 = bias * projCoord6;\n"
    "  projCoord7 = bias * projCoord7;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadow7_fp =
    "uniform sampler2DShadow shadowMap1;\n"
    "uniform sampler2DShadow shadowMap2;\n"
    "uniform sampler2DShadow shadowMap3;\n"
    "uniform sampler2DShadow shadowMap4;\n"
    "uniform sampler2DShadow shadowMap5;\n"
    "uniform sampler2DShadow shadowMap6;\n"
    "uniform sampler2DShadow shadowMap7;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity1;\n"
    "uniform float intensity2;\n"
    "uniform float intensity3;\n"
    "uniform float intensity4;\n"
    "uniform float intensity5;\n"
    "uniform float intensity6;\n"
    "uniform float intensity7;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float mapFactor1;\n"
    "uniform float mapFactor2;\n"
    "uniform float mapFactor3;\n"
    "uniform float mapFactor4;\n"
    "uniform float mapFactor5;\n"
    "uniform float mapFactor6;\n"
    "uniform float mapFactor7;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 projCoord4;\n"
    "varying vec4 projCoord5;\n"
    "varying vec4 projCoord6;\n"
    "varying vec4 projCoord7;\n"
    "varying vec4 texPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	vec4 projectiveBiased2 = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "	vec4 projectiveBiased3 = vec4((projCoord3.xyz / projCoord3.q),1.0);\n"
    "	vec4 projectiveBiased4 = vec4((projCoord4.xyz / projCoord4.q),1.0);\n"
    "	vec4 projectiveBiased5 = vec4((projCoord5.xyz / projCoord5.q),1.0);\n"
    "	vec4 projectiveBiased6 = vec4((projCoord6.xyz / projCoord6.q),1.0);\n"
    "	vec4 projectiveBiased7 = vec4((projCoord7.xyz / projCoord7.q),1.0);\n"
    "	float shadowed;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap1, vec3(vec2(projectiveBiased.xy) * vec2(mapFactor1,mapFactor1),projectiveBiased.z)).x) * intensity1;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap2, vec3(vec2(projectiveBiased2.xy) * vec2(mapFactor2,mapFactor2),projectiveBiased2.z)).x) * intensity2;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap3, vec3(vec2(projectiveBiased3.xy) * vec2(mapFactor3,mapFactor3),projectiveBiased3.z)).x) * intensity3;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap4, vec3(vec2(projectiveBiased4.xy) * vec2(mapFactor4,mapFactor4),projectiveBiased4.z)).x) * intensity4;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap5, vec3(vec2(projectiveBiased5.xy) * vec2(mapFactor5,mapFactor5),projectiveBiased5.z)).x) * intensity5;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap6, vec3(vec2(projectiveBiased6.xy) * vec2(mapFactor6,mapFactor6),projectiveBiased6.z)).x) * intensity6;\n"
    "	shadowed += (1.0 - shadow2D(shadowMap7, vec3(vec2(projectiveBiased7.xy) * vec2(mapFactor7,mapFactor7),projectiveBiased7.z)).x) * intensity7;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

static std::string _lisp_shadowCube_vp =
    "uniform float shadowBias;\n"
    "uniform mat4 lightPMOP;\n"
    "uniform mat4 KKtoWK;\n"
    "uniform float texFactor;\n"
    "varying vec4 texPos;\n"
    "varying vec4 realPos;\n"
    "varying vec4 realPos2;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "void main(void)\n"
    "{\n"
    "  realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  realPos2 = lightPMOP * (KKtoWK * realPos);\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _lisp_shadowCube_fp =
    "uniform sampler2DShadow shadowMap;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "\n"
    "uniform mat4 lightPMA;\n"
    "uniform mat4 lightPMB;\n"
    "uniform mat4 lightPMC;\n"
    "uniform mat4 lightPMD;\n"
    "uniform mat4 lightPME;\n"
    "uniform mat4 lightPMF;\n"
    "\n"
    "uniform float texFactor;\n"
    "\n"
    "varying vec4 texPos;\n"
    "varying vec4 realPos;\n"
    "varying vec4 realPos2;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	float xOffset = 0.0;\n"
    "	float yOffset = 0.0;\n"
    "	float maxStep = 0.25;\n"
    "\n"
    "	vec4 projCoord2;\n"
    "\n"
    "	if(abs(realPos2.x) > abs(realPos2.y) && abs(realPos2.x) > abs(realPos2.z))\n"
    "	{\n"
    "		if(realPos2.x >= 0.0) {projCoord2 = lightPME * realPos; xOffset = 0.0, yOffset = maxStep;}\n"
    "		else {projCoord2 = lightPMF * realPos; xOffset = maxStep, yOffset = maxStep;}\n"
    "	}\n"
    "	else if(abs(realPos2.y) > abs(realPos2.x) && abs(realPos2.y) > abs(realPos2.z))\n"
    "	{\n"
    "		if(realPos2.y >= 0.0) {projCoord2 = lightPMC * realPos; xOffset = 2.0 * maxStep, yOffset = 0.0;}\n"
    "		else {projCoord2 = lightPMD * realPos; xOffset = 3.0 * maxStep, yOffset = 0.0;}\n"
    "	}\n"
    "	else\n"
    "	{\n"
    "		if(realPos2.z >= 0.0) {projCoord2 = lightPMA * realPos;  xOffset = 0.0, yOffset = 0.0;}\n"
    "		else {projCoord2 = lightPMB * realPos; xOffset = maxStep, yOffset = 0.0;}\n"
    "	}\n"
    "\n"
    "	projCoord2.x *= texFactor;\n"
    "	projCoord2 = bias * projCoord2;\n"
    "\n"
    "	vec4 projectiveBiased = vec4((projCoord2.xyz / projCoord2.q),1.0);\n"
    "\n"
    "	projectiveBiased.x = projectiveBiased.x/4.0;\n"
    "	projectiveBiased.y = projectiveBiased.y/4.0;\n"
    "\n"
    "	float shadowed = 0.0;\n"
    "	shadowed = (1.0 - shadow2D(shadowMap,vec3(vec2(projectiveBiased.xy + vec2(xOffset,yOffset)),projectiveBiased.z)).x) * intensity;\n"
    "\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.5,0.0,1.0);\n"
    "}\n";

PerspectiveShadowMapHandler::PerspectiveShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
//    _fb(0),
    _fb2(0),
//    _rb_depth(0),
    _transforms(),
    _perspectiveLPM(),
    _perspectiveLVM(),
    _tiledeco(NULL),
    _blender(NULL),
    _matrixCam2(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowFactorMapImage2(NULL),
    _shadowCmat(NULL),
    _shadowSHL(NULL),
    _shadowCubeSHL(NULL),
    _shadowSHL2(NULL),
    _shadowSHL3(NULL),
    _shadowSHL4(NULL),
    _shadowSHL5(NULL),
    _shadowSHL6(NULL),
    _shadowSHL7(NULL),
    _firstRun(0),
    _initTexturesDone(false)
{

    _blender = BlendChunk::create();
    {
        _blender->setAlphaFunc(GL_GEQUAL);
        _blender->setAlphaValue(0.99);
    }

    _matrixCam2 = MatrixCamera::create();

#if 0
    if(_shadowVP->getParent() != NULL)
    {
        _width = _shadowVP->getPixelWidth();
        _height = _shadowVP->getPixelHeight();
    }

    if(_width == 0)
        _width = 1;
    if(_height == 0)
        _height = 1;

    if(_width > _height)
        _widthHeightPOT = osgNextPower2(_width - 1);
    else
        _widthHeightPOT = osgNextPower2(_height - 1);
#endif

    _colorMapO = TextureObjChunk::create();
    _colorMapImage = Image::create();

    _colorMapO->setImage(_colorMapImage);
    _colorMapO->setInternalFormat(GL_RGB);
    _colorMapO->setExternalFormat(GL_RGB);
    _colorMapO->setMinFilter(GL_NEAREST);
    _colorMapO->setMagFilter(GL_NEAREST);
    _colorMapO->setWrapS(GL_REPEAT);
    _colorMapO->setWrapT(GL_REPEAT);
    _colorMapO->setTarget(GL_TEXTURE_2D);

#if 0
    if(_useNPOTTextures)
    {
        _colorMapImage->set(GL_RGB, _width, _height);
    }
    else
    {
        _colorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
#endif

    _shadowFactorMapO = TextureObjChunk::create();
    _shadowFactorMapImage = Image::create();

    _shadowFactorMapO->setImage(_shadowFactorMapImage);
    _shadowFactorMapO->setInternalFormat(GL_RGB);
    _shadowFactorMapO->setExternalFormat(GL_RGB);
    _shadowFactorMapO->setMinFilter(GL_LINEAR);
    _shadowFactorMapO->setMagFilter(GL_LINEAR);
    _shadowFactorMapO->setWrapS(GL_REPEAT);
    _shadowFactorMapO->setWrapT(GL_REPEAT);
    _shadowFactorMapO->setTarget(GL_TEXTURE_2D);

#if 0
    if(_useNPOTTextures)
    {
        _shadowFactorMapImage->set(GL_RGB, _width, _height);
    }
    else
    {
        _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
#endif

    _shadowFactorMap2O = TextureObjChunk::create();
    _shadowFactorMapImage2 = Image::create();

    _shadowFactorMap2O->setImage(_shadowFactorMapImage2);
    _shadowFactorMap2O->setInternalFormat(GL_RGB);
    _shadowFactorMap2O->setExternalFormat(GL_RGB);
    _shadowFactorMap2O->setMinFilter(GL_LINEAR);
    _shadowFactorMap2O->setMagFilter(GL_LINEAR);
    _shadowFactorMap2O->setWrapS(GL_REPEAT);
    _shadowFactorMap2O->setWrapT(GL_REPEAT);
    _shadowFactorMap2O->setTarget(GL_TEXTURE_2D);

#if 0
    if(_useNPOTTextures)
    {
        _shadowFactorMapImage2->set(GL_RGB, _width, _height);
    }
    else
    {
        _shadowFactorMapImage2->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
#endif

    //SHL Chunk 1
    _shadowSHL = SHLChunk::create();
    //_shadowSHL->readVertexProgram("Perspective_Shadow.vert");
    //_shadowSHL->readFragmentProgram("Perspective_Shadow.frag");
    _shadowSHL->setVertexProgram(_lisp_shadow_vp);
    _shadowSHL->setFragmentProgram(_lisp_shadow_fp);

    _shadowSHL2 = SHLChunk::create();
    //_shadowSHL2->readVertexProgram("PSM_Shadow2.vert");
    //_shadowSHL2->readFragmentProgram("PSM_Shadow2.frag");
    _shadowSHL2->setVertexProgram(_lisp_shadow2_vp);
    _shadowSHL2->setFragmentProgram(_lisp_shadow2_fp);

    _shadowSHL3 = SHLChunk::create();
    //_shadowSHL3->readVertexProgram("PSM_Shadow3.vert");
    //_shadowSHL3->readFragmentProgram("PSM_Shadow3.frag");
    _shadowSHL3->setVertexProgram(_lisp_shadow3_vp);
    _shadowSHL3->setFragmentProgram(_lisp_shadow3_fp);

    _shadowSHL4 = SHLChunk::create();
    //_shadowSHL4->readVertexProgram("PSM_Shadow4.vert");
    //_shadowSHL4->readFragmentProgram("PSM_Shadow4.frag");
    _shadowSHL4->setVertexProgram(_lisp_shadow4_vp);
    _shadowSHL4->setFragmentProgram(_lisp_shadow4_fp);

    _shadowSHL5 = SHLChunk::create();
    //_shadowSHL5->readVertexProgram("PSM_Shadow5.vert");
    //_shadowSHL5->readFragmentProgram("PSM_Shadow5.frag");
    _shadowSHL5->setVertexProgram(_lisp_shadow5_vp);
    _shadowSHL5->setFragmentProgram(_lisp_shadow5_fp);

    _shadowSHL6 = SHLChunk::create();
    //_shadowSHL6->readVertexProgram("PSM_Shadow6.vert");
    //_shadowSHL6->readFragmentProgram("PSM_Shadow6.frag");
    _shadowSHL6->setVertexProgram(_lisp_shadow6_vp);
    _shadowSHL6->setFragmentProgram(_lisp_shadow6_fp);

    _shadowSHL7 = SHLChunk::create();
    //_shadowSHL7->readVertexProgram("PSM_Shadow7.vert");
    //_shadowSHL7->readFragmentProgram("PSM_Shadow7.frag");
    _shadowSHL7->setVertexProgram(_lisp_shadow7_vp);
    _shadowSHL7->setFragmentProgram(_lisp_shadow7_fp);

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("Perspective_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("Perspective_Shadow_combine.frag");
    _combineSHL->setVertexProgram(_shadow_combine_vp);
    _combineSHL->setFragmentProgram(_shadow_combine_fp);

    _combineDepth = DepthChunk::create();
        _combineDepth->setReadOnly(true);

    //SHL Chunk 3
    _shadowCubeSHL = SHLChunk::create();
    //_shadowCubeSHL->readVertexProgram("Std_ShadowCube.vert");
    //_shadowCubeSHL->readFragmentProgram("Std_ShadowCube.frag");
    _shadowCubeSHL->setVertexProgram(_lisp_shadowCube_vp);
    _shadowCubeSHL->setFragmentProgram(_lisp_shadowCube_fp);

    _shadowCmat = ChunkMaterial::create();

    //Combine Shader
    _combineCmat = ChunkMaterial::create();
    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMapO);
    _combineCmat->addChunk(_shadowFactorMapO);
    _combineCmat->addChunk(_combineDepth);


    _transforms[0] = Matrix(1, 0, 0, 0,
                            0, -1, 0, 0,
                            0, 0, -1, 0,
                            0, 0, 0, 1);

    _transforms[1] = Matrix(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

    _transforms[2] = Matrix(1, 0, 0, 0,
                            0, 0, 1, 0,
                            0, -1, 0, 0,
                            0, 0, 0, 1);

    _transforms[3] = Matrix(1, 0, 0, 0,
                            0, 0, -1, 0,
                            0, 1, 0, 0,
                            0, 0, 0, 1);

    _transforms[4] = Matrix(0, 0, 1, 0,
                            0, 1, 0, 0,
                            -1, 0, 0, 0,
                            0, 0, 0, 1);

    _transforms[5] = Matrix(0, 0, -1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 0, 0, 1);

}

PerspectiveShadowMapHandler::~PerspectiveShadowMapHandler(void)
{
    _tiledeco = NULL;

    _blender          = NULL;
    _shadowSHL        = NULL;
    _shadowSHL2       = NULL;
    _shadowSHL3       = NULL;
    _shadowSHL4       = NULL;
    _shadowSHL5       = NULL;
    _shadowSHL6       = NULL;
    _shadowSHL7       = NULL;
    _combineSHL       = NULL;
    _combineDepth     = NULL;
    _shadowCubeSHL    = NULL;
    _combineCmat      = NULL;
    _shadowCmat       = NULL;
    _matrixCam2       = NULL;

#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
#if 0
    if(_fb != 0)
        glDeleteFramebuffersEXT(1, &_fb);
    if(_rb_depth != 0)
        glDeleteRenderbuffersEXT(1, &_rb_depth);
#endif
#endif
    if(_fb2 != 0)
        glDeleteFramebuffersEXT(1, &_fb2);
}

/// Checks if FBO status is ok
bool PerspectiveShadowMapHandler::checkFrameBufferStatus(Window *win)
{
    GLenum  errCode, status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            FINFO(("%x: framebuffer complete!\n", status));
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            FWARNING(("%x: framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n",
                      status));
            // choose different formats
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_ATTACHMENT\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            FWARNING(("%x: framebuffer FRAMEBUFFER_MISSING_ATTACHMENT\n",
                      status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            FWARNING(("%x: framebuffer FRAMEBUFFER_DIMENSIONS\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_FORMATS\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_DRAW_BUFFER\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_READ_BUFFER\n", status));
            break;
        case GL_FRAMEBUFFER_BINDING_EXT:
            FWARNING(("%x: framebuffer BINDING_EXT\n", status));
            break;
        default:
            return false;
    }

    if((errCode = glGetError()) != GL_NO_ERROR)
    {
        const GLubyte   *errString = gluErrorString(errCode);
        FWARNING(("OpenGL Error: %s!\n", errString));
        return false;
    }
    return true;
}


bool PerspectiveShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    if(_useFBO)
    {
#if 0
        Int32   width = _shadowVP->getPixelWidth();
        Int32   height = _shadowVP->getPixelHeight();
#endif
        Int32   width  = pEnv->getPixelWidth();
        Int32   height = pEnv->getPixelHeight();

        if(width <= 0 || height <= 0)
            return false;

        if(_fb2 != 0)
            return true;

        Window *win = pEnv->getWindow();

        _width  = pEnv->getPixelWidth();
        _height = pEnv->getPixelHeight();

        _colorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage2->set(GL_RGB, _width, _height);

        commitChanges();

//        glGenFramebuffersEXT(1, &_fb);
//        glGenRenderbuffersEXT(1, &_rb_depth);

        _pFB = FrameBufferObject::create();
        
        _pFB->setSize(_width, _height);

        RenderBufferUnrecPtr pDepthRB = RenderBuffer::create();
        
        pDepthRB->setInternalFormat(GL_DEPTH_COMPONENT24);

//        win->validateGLObject(_colorMapO->getGLId(), pEnv);
//        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
//                                  GL_COLOR_ATTACHMENT0_EXT,
//                                  GL_TEXTURE_2D,
//                                  win->getGLObjectId(_colorMapO->getGLId()), 
//                                  0);

        TextureBufferUnrecPtr pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_colorMapO);

        _pFB->setColorAttachment(pTexBuffer, 0);

//        win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
//                                  GL_COLOR_ATTACHMENT1_EXT,
//                                  GL_TEXTURE_2D,
//                                  win->getGLObjectId(
//                                      _shadowFactorMapO->getGLId()), 
//                                  0);

        pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_shadowFactorMapO);

        _pFB->setColorAttachment(pTexBuffer, 1);

//        win->validateGLObject(_shadowFactorMap2O->getGLId(), pEnv);
//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
//                                  GL_COLOR_ATTACHMENT2_EXT,
//                                  GL_TEXTURE_2D,
//                                  win->getGLObjectId(
//                                      _shadowFactorMap2O->getGLId()), 
//                                  0);

        pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_shadowFactorMap2O);

        _pFB->setColorAttachment(pTexBuffer, 2);

#if 0
        //Initialize Depth Renderbuffer
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rb_depth);
        if(_useNPOTTextures)
#if 0
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB, 
                                     _shadowVP->getPixelWidth(),
                                     _shadowVP->getPixelHeight());
#endif
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB,
                                     pEnv->getPixelWidth(),
                                     pEnv->getPixelHeight());

        else
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB, 
                                     _widthHeightPOT, 
                                     _widthHeightPOT);

        //Attach Renderbuffer to Framebuffer depth Buffer
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
                                     GL_DEPTH_ATTACHMENT_EXT, 
                                     GL_RENDERBUFFER_EXT, 
                                     _rb_depth);
#endif

        _pFB->setDepthAttachment(pDepthRB);

        commitChanges();

#if 0
        win->validateGLObject(_colorMapO->getGLId(), pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
                                  GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_colorMapO->getGLId()), 0);

        win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
                                  GL_COLOR_ATTACHMENT1_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(
                                      _shadowFactorMapO->getGLId()), 
                                  0);

        win->validateGLObject(_shadowFactorMap2O->getGLId(), pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
                                  GL_COLOR_ATTACHMENT2_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(
                                      _shadowFactorMap2O->getGLId()), 0);


        bool    result = checkFrameBufferStatus(win);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#endif


        glGenFramebuffersEXT(1, &_fb2);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);

        glDrawBuffer(GL_NONE);	// no color buffer dest
        glReadBuffer(GL_NONE);	// no color buffer src

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    }
    return true;
}

void PerspectiveShadowMapHandler::reInit(DrawEnv *pEnv)
{
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
#if 0
    Int32   width = _shadowVP->getPixelWidth();
    Int32   height = _shadowVP->getPixelHeight();
#endif
    Int32   width = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    Window *win = pEnv->getWindow();

#if 0
    win->validateGLObject(_colorMapO->getGLId(), pEnv);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_colorMapO->getGLId()), 0);
    win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_shadowFactorMapO->getGLId()),
                              0);
    win->validateGLObject(_shadowFactorMap2O->getGLId(), pEnv);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_shadowFactorMap2O->getGLId()),
                              0);

    //Initialize Depth Renderbuffer
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rb_depth);
    if(_useNPOTTextures)
#if 0
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 _shadowVP->getPixelWidth(),
                                 _shadowVP->getPixelHeight());
#endif
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 pEnv->getPixelWidth(),
                                 pEnv->getPixelHeight());
    else
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 _widthHeightPOT, _widthHeightPOT);

    //Attach Renderbuffer to Framebuffer depth Buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT
                                 , _rb_depth);
#endif
#endif
}

void PerspectiveShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;

#if 0
    Int32   width = _shadowVP->getPixelWidth();
    Int32   height = _shadowVP->getPixelHeight();
#endif
    Int32   width  = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    //if no NPOTTextures supported, resize images
    if(!_useNPOTTextures)
    {
        if(width > height)
            _widthHeightPOT = osgNextPower2(width - 1);
        else
            _widthHeightPOT = osgNextPower2(height - 1);

        _colorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);

        _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);

        _shadowFactorMapImage2->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
}


void PerspectiveShadowMapHandler::calcPerspectiveSpot(Matrix &_LPM, 
                                                      Matrix &_LVM,
                                                      UInt32   num,
                                                      DrawEnv *pEnv)
{

    Matrix  CPM, CVM;
#if 0
    _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getParent()->getWidth(),
                                       _shadowVP->getParent()->getHeight());
    _shadowVP->getCamera()->getProjection(CPM,
                                          _shadowVP->getParent()->getWidth(),
                                          _shadowVP->getParent()->getHeight());
#endif
    pEnv->getAction()->getCamera()->getViewing(
        CVM, 
        pEnv->getWindow()->getWidth(),
        pEnv->getWindow()->getHeight());

    pEnv->getAction()->getCamera()->getProjection(
        CPM,
        pEnv->getWindow()->getWidth(),
        pEnv->getWindow()->getHeight());

    Matrix  LPM, LVM;
#if 0
    _shadowVP->_lightCameras[num]->getViewing(LVM,
                                              _shadowVP->getParent()->getWidth
                                              (),
                                              _shadowVP->getParent()->getHeight
                                              ());
    _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                 _shadowVP->getParent
                                                 ()->getWidth(),
                                                 _shadowVP->getParent
                                                 ()->getHeight());
#endif
    _shadowVP->_lightCameras[num]->getViewing(LVM,
                                              pEnv->getWindow()->getWidth
                                              (),
                                              pEnv->getWindow()->getHeight
                                              ());
    _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                 pEnv->getWindow
                                                 ()->getWidth(),
                                                 pEnv->getWindow
                                                 ()->getHeight());

    Matrix  LPVM;
    LPVM = LPM;
    LPVM.mult(LVM);

    Matrix  eyeProjView = CPM;
    eyeProjView.mult(CVM);
    Matrix  invEyeProjView = eyeProjView;
    invEyeProjView.invert();

    //Scene AABox Extrempunkte holen (Weltkoordinaten)
    Pnt3f   sceneMax = _shadowVP->getLightRoot(num)->getVolume().getMax();
    Pnt3f   sceneMin = _shadowVP->getLightRoot(num)->getVolume().getMin();

    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    //LightPos holen
    Pnt3f   lPos(0,0,0);

    bool    isDirect = false;
    bool    useStd = false;

    if(_shadowVP->_lights[num].second->getType() == DirectionalLight::getClassType())
        isDirect = true;

    if(!isDirect)
    {
        if(_shadowVP->_lights[num].second->getType() == PointLight::getClassType())
        {
            PointLight *tmpPoint;
            tmpPoint = dynamic_cast<PointLight *>(_shadowVP->_lights[num].second.get());
	
            lPos = tmpPoint->getPosition();
	
            if(tmpPoint->getBeacon() != NULL)
            {
                Matrix  m = tmpPoint->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }
        }
	
        if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType())
        {
            SpotLight *tmpSpot;
            tmpSpot = dynamic_cast<SpotLight *>(_shadowVP->_lights[num].second.get());
            lPos = tmpSpot->getPosition();
            if(tmpSpot->getBeacon() != NULL)
            {
                Matrix  m = tmpSpot->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }
        }
	
        //Wenn Lichtposition innerhalb der Scene-BB ist standard Shadow Mapping verwenden
        if(lPos[0] > sceneMin[0] && lPos[1] > sceneMin[1] &&
           lPos[2] > sceneMin[2] && lPos[0] < sceneMax[0] &&
           lPos[1] < sceneMax[1] && lPos[2] < sceneMax[2])
        {
            _LPM = LPM;
            _LVM = LVM;
            useStd = true;
        }
            //beleuchtet das Spotlight die komplette Szene? Wenn nicht -> standard Shadow Mapping
        else if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType
                () && !bbInsideFrustum(sceneMin, sceneMax, LPVM))
        {
            _LPM = LPM;
            _LVM = LVM;
            useStd = true;
        }
    }

    if(!useStd)
    {

        //Lichtrichtung im Post-Perspektivischen Raum des Lichtes
        Vec3f   lightDir(0,0,1);

        //Kamerapunkt holen
        Pnt3f   eyePos(0,0,0);
#if 0
        Matrix  m2 = _shadowVP->getCamera()->getBeacon()->getToWorld();
#endif
        Matrix  m2 = pEnv->getAction()->getCamera()->getBeacon()->getToWorld();

        m2.mult(eyePos, eyePos);
        
        LPVM.multFull(eyePos, eyePos);

        //Ist die Kamera hinter der Lichtquelle?
        bool    cBehindL = false;
        if(eyePos[2] < -1)
            cBehindL = true;
        else if(eyePos[2] > 1)
            cBehindL = true;

        if(cBehindL)
        {
            //Trick: Near = -Far / Far = Near
            //Gibt leider Probleme im Grenzbereich ...
            /*double tmpNear;
               tmpNear = _shadowVP->_lightCameras[num]->getNear();
               _shadowVP->_lightCameras[num]->setNear(-_shadowVP->_lightCameras[num]->getFar());
               _shadowVP->_lightCameras[num]->setFar(tmpNear);
               _shadowVP->_lightCameras[num]->getProjection(LPM,_shadowVP->getParent()->getWidth(),_shadowVP->getParent()->getHeight());
               LPVM = LPM;
               LPVM.mult(LVM);
               lightDir = Vec3f(0,0,-1);*/

            //Standard Shadow Mapping benutzen
            _LVM = LVM;
            _LPM = LPM;
        }
        else
        {
            //ViewDir
            Vec3f               viewDir(0,0,-1);
#if 0
            Matrix              m3 = _shadowVP->getCamera()->getBeacon
                ()->getToWorld();
#endif
            Matrix              m3 = pEnv->getAction()->getCamera()->getBeacon
                ()->getToWorld();
            m3  .mult(viewDir, viewDir);
            LPVM.mult(viewDir, viewDir);
            viewDir.normalize();

            //Body berechnen
            std::vector<Pnt3f>  points;
            calcHull2(&points, invEyeProjView, sceneMin, sceneMax, LPVM);

            //Kopie erstellen
            std::vector<Pnt3f>  pointsCopy;
            for(UInt32 i = 0;i < points.size();i++)
                pointsCopy.push_back(points[i]);

            bool                correctAngle = false;
            if((osgAbs(viewDir[0]) - osgAbs(lightDir[0])) < 0.01 &&
               (osgAbs(viewDir[1]) - osgAbs(lightDir[1])) < 0.01 &&
               (osgAbs(viewDir[2]) - osgAbs(lightDir[2])) < 0.01)
            {
                viewDir[0] += 0.01;
                correctAngle = true;
            }

            double              dotProd = viewDir.dot(lightDir);
            double              sinGamma;
            sinGamma = sqrt(1.0 - dotProd * dotProd);
            if(correctAngle)
                viewDir[0] -= 0.01;

            //up berechnen
            Vec3f               up99;
            calcUpVec2(up99, viewDir, lightDir);

            Matrix              lView22;
            MatrixLookAt(lView22, eyePos, eyePos + lightDir, up99);
            lView22.invert();

            // Punkte ins neue KKS transformieren
            for(UInt32 i = 0;i < points.size();i++)
            {
                Pnt3f   tmpPnt(points[i][0], points[i][1], points[i][2]);
                lView22.multFull(tmpPnt, tmpPnt);
                points[i] = tmpPnt;
            }

            // BBox um Punkte bestimmen
            Pnt3f               min99, max99;
            calcCubicHull2(min99, max99, &points);

            Real32              factor = 1.0 / sinGamma;

            Real32              nearDist = 1.0 - sinGamma;
            if(nearDist < 0.01)
                nearDist = 0.01;

            Real32              z_n = factor * nearDist;
            Real32              d = osgAbs(max99[1] - min99[1]);
            Real32              z_f = z_n + d * sinGamma;
            Real32              n = (z_n + sqrt(z_f * z_n)) / sinGamma;
            Real32              f = n + d;

            Pnt3f               pos99;

            pos99[0] = eyePos[0] + (up99[0] * -(n - nearDist));
            pos99[1] = eyePos[1] + (up99[1] * -(n - nearDist));
            pos99[2] = eyePos[2] + (up99[2] * -(n - nearDist));

            Matrix              lView222;
            MatrixLookAt(lView222, pos99, pos99 + lightDir, up99);
            lView222.invert();

            // Lisp-Matrix erstellen
            Matrix              lispMtx99(1,0,0,0,
                                          0,(f + n) / (f - n),0,-2 * f * n /
                                          (f - n),
                                          0,0,1,0,
                                          0,1,0,0);

            for(UInt32 i = 0;i < pointsCopy.size();i++)
            {
                Pnt3f   tmpPnt = pointsCopy[i];
                lView222 .multFull(tmpPnt, tmpPnt);
                lispMtx99.multFull(tmpPnt, tmpPnt);
                pointsCopy[i] = tmpPnt;
            }

            calcCubicHull2(min99, max99, &pointsCopy);

            Matrix              lProj;
            scaleTranslateToFit2(lProj, min99, max99);

            Matrix              rh2lh;
            rh2lh.setIdentity();
            rh2lh.setScale(1.0, 1.0, -1.0);

            lProj.mult(lispMtx99);
            lProj.multLeft(rh2lh);

            points.clear();
            pointsCopy.clear();

            _LPM = lProj;
            _LPM.mult(lView222);
            _LPM.mult(LPM);
            _LVM = LVM;
        }
    }
}

void PerspectiveShadowMapHandler::calcHull2(std::vector<Pnt3f> *points,
                                            Matrix invEyeProjMatrix, 
                                            Pnt3f sceneMin,
                                            Pnt3f sceneMax, 
                                            Matrix LPVM)
{
    //Die Funktion clippt das Kamera Frustum mit der Szenen Bounding Box und liefert eine Punktmenge zurck

    //Unit Cube
    Pnt3f   vf0(-1,-1,-1);
    Pnt3f   vf1(1,-1,-1);
    Pnt3f   vf2(1,1,-1);
    Pnt3f   vf3(-1,1,-1);
    Pnt3f   vf4(-1,-1,1);
    Pnt3f   vf5(1,-1,1);
    Pnt3f   vf6(1,1,1);
    Pnt3f   vf7(-1,1,1);

    //transform to World Coordinates
    invEyeProjMatrix.multFull(vf0, vf0);
    invEyeProjMatrix.multFull(vf1, vf1);
    invEyeProjMatrix.multFull(vf2, vf2);
    invEyeProjMatrix.multFull(vf3, vf3);
    invEyeProjMatrix.multFull(vf4, vf4);
    invEyeProjMatrix.multFull(vf5, vf5);
    invEyeProjMatrix.multFull(vf6, vf6);
    invEyeProjMatrix.multFull(vf7, vf7);

    //Scene Bounding Box Points
    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    //Build Planes of BBox
    Plane   bboxSides[6];
    //Front
    bboxSides[0] = Plane(bb0, bb1, bb2);
    //Back
    bboxSides[1] = Plane(bb7, bb6, bb5);
    //Left
    bboxSides[2] = Plane(bb0, bb3, bb7);
    //Right
    bboxSides[3] = Plane(bb2, bb1, bb5);
    //Top
    bboxSides[4] = Plane(bb6, bb7, bb3);
    //Bottom
    bboxSides[5] = Plane(bb5, bb1, bb0);

    //Build Planes of View Frustum
    //Front
    Pnt3f   vfI[6][20];
    Pnt3f   vfO[20];
    UInt32  outCount;

    //Define View Frustum as Clip Polygons
    //Front
    vfI[0][0] = vf0;
    vfI[0][1] = vf1;
    vfI[0][2] = vf2;
    vfI[0][3] = vf3;
    //Back
    vfI[1][0] = vf4;
    vfI[1][1] = vf5;
    vfI[1][2] = vf6;
    vfI[1][3] = vf7;
    //Left
    vfI[2][0] = vf0;
    vfI[2][1] = vf4;
    vfI[2][2] = vf7;
    vfI[2][3] = vf3;
    //Right
    vfI[3][0] = vf1;
    vfI[3][1] = vf5;
    vfI[3][2] = vf6;
    vfI[3][3] = vf2;
    //Top
    vfI[4][0] = vf3;
    vfI[4][1] = vf2;
    vfI[4][2] = vf6;
    vfI[4][3] = vf7;
    //Bottom
    vfI[5][0] = vf0;
    vfI[5][1] = vf1;
    vfI[5][2] = vf5;
    vfI[5][3] = vf4;

    //Punktgroesse merken
    UInt32  vfSize[6];
    for(UInt32 i = 0;i < 6;i++)
        vfSize[i] = 4;

    UInt32  sum = 0;
    Pnt3f   allPoints[100];

    // speichert die anzahl der durchgefhrten Clips
    UInt32  sumClip = 0;

    for(UInt32 i = 0;i < 6;i++)
    {
        //Kamerafrustum
        outCount = vfSize[i];

        for(UInt32 j = 0;j < 6;j++)
        {
            //BBox
            if(vfSize[i] != 0)
            {
                outCount = bboxSides[j].clip(vfI[i], vfO, vfSize[i]);
                sumClip++;
                vfSize[i] = outCount;

                for(UInt32 k = 0;k < outCount;k++)
                {
                    vfI[i][k] = vfO[k];
                }
            }
        }

        for(UInt32 k = 0;k < vfSize[i];k++)
        {
            points->push_back(vfO[k]);
        }
    }

    if(pntInFrontOf(vf0, vf1, vf2, bb0) && pntInFrontOf(vf7, vf6, vf5, bb0) &&
       pntInFrontOf(vf0, vf3, vf7, bb0) && pntInFrontOf(vf2, vf1, vf5, bb0) &&
       pntInFrontOf(vf6, vf7, vf3, bb0) && pntInFrontOf(vf5, vf1, vf0, bb0))
        points->push_back(bb0);
    if(pntInFrontOf(vf0, vf1, vf2, bb1) && pntInFrontOf(vf7, vf6, vf5, bb1) &&
       pntInFrontOf(vf0, vf3, vf7, bb1) && pntInFrontOf(vf2, vf1, vf5, bb1) &&
       pntInFrontOf(vf6, vf7, vf3, bb1) && pntInFrontOf(vf5, vf1, vf0, bb1))
        points->push_back(bb1);
    if(pntInFrontOf(vf0, vf1, vf2, bb2) && pntInFrontOf(vf7, vf6, vf5, bb2) &&
       pntInFrontOf(vf0, vf3, vf7, bb2) && pntInFrontOf(vf2, vf1, vf5, bb2) &&
       pntInFrontOf(vf6, vf7, vf3, bb2) && pntInFrontOf(vf5, vf1, vf0, bb2))
        points->push_back(bb2);
    if(pntInFrontOf(vf0, vf1, vf2, bb3) && pntInFrontOf(vf7, vf6, vf5, bb3) &&
       pntInFrontOf(vf0, vf3, vf7, bb3) && pntInFrontOf(vf2, vf1, vf5, bb3) &&
       pntInFrontOf(vf6, vf7, vf3, bb3) && pntInFrontOf(vf5, vf1, vf0, bb3))
        points->push_back(bb3);
    if(pntInFrontOf(vf0, vf1, vf2, bb4) && pntInFrontOf(vf7, vf6, vf5, bb4) &&
       pntInFrontOf(vf0, vf3, vf7, bb4) && pntInFrontOf(vf2, vf1, vf5, bb4) &&
       pntInFrontOf(vf6, vf7, vf3, bb4) && pntInFrontOf(vf5, vf1, vf0, bb4))
        points->push_back(bb4);
    if(pntInFrontOf(vf0, vf1, vf2, bb5) && pntInFrontOf(vf7, vf6, vf5, bb5) &&
       pntInFrontOf(vf0, vf3, vf7, bb5) && pntInFrontOf(vf2, vf1, vf5, bb5) &&
       pntInFrontOf(vf6, vf7, vf3, bb5) && pntInFrontOf(vf5, vf1, vf0, bb5))
        points->push_back(bb5);
    if(pntInFrontOf(vf0, vf1, vf2, bb6) && pntInFrontOf(vf7, vf6, vf5, bb6) &&
       pntInFrontOf(vf0, vf3, vf7, bb6) && pntInFrontOf(vf2, vf1, vf5, bb6) &&
       pntInFrontOf(vf6, vf7, vf3, bb6) && pntInFrontOf(vf5, vf1, vf0, bb6))
        points->push_back(bb6);
    if(pntInFrontOf(vf0, vf1, vf2, bb7) && pntInFrontOf(vf7, vf6, vf5, bb7) &&
       pntInFrontOf(vf0, vf3, vf7, bb7) && pntInFrontOf(vf2, vf1, vf5, bb7) &&
       pntInFrontOf(vf6, vf7, vf3, bb7) && pntInFrontOf(vf5, vf1, vf0, bb7))
        points->push_back(bb7);

    //Body in LK transformieren
    for(UInt32 i = 0;i < points->size();i++)
    {
        Pnt3f   tmpPnt((*points)[i]);
        LPVM.multFull(tmpPnt, tmpPnt);
        (*points)[i] = tmpPnt;
    }

    UInt32  sumPoints = points->size();
    for(UInt32 i = 0;i < sumPoints;i++)
    {
        Pnt3f   pntCut2 = (*points)[i];
        pntCut2[2] = -1.0;
        points->push_back(pntCut2);
    }
}

void PerspectiveShadowMapHandler::calcBodyVec(Vec3f &dir, Pnt3f eyePos,
                                              std::vector<Pnt3f> *points)
{
    for(UInt32 i = 0;i < points->size();i++)
    {
        Vec3f   p;
        for(UInt32 j = 0;j < 3;j++)
        {
            p[j] = (*points)[i][j] - eyePos[j];
        }
        dir[0] += p[0];
        dir[1] += p[1];
        dir[2] += p[2];
    }
    dir.normalize();
}

void PerspectiveShadowMapHandler::printMatrix(Matrix m)
{
    printf("Printing Matrix...\n");
    Real32  *mPtr = m.getValues();
    printf(
        " %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f \n", mPtr[0], mPtr[1], mPtr[2], mPtr[3], mPtr[4], mPtr[5], mPtr[6], mPtr[7], mPtr[8], mPtr[9], mPtr[10], mPtr[11], mPtr[12], mPtr[13], mPtr[14], mPtr[15]);
}

void PerspectiveShadowMapHandler::printPoint(Pnt3f p)
{
    printf("Printing Point...\n");
    printf("%f, %f, %f\n", p.x(), p.y(), p.z());
}

void PerspectiveShadowMapHandler::calcUpVec2(Vec3f &up, Vec3f viewDir, Vec3f lightDir)
{
    Vec3f   left;
    left = lightDir.cross(viewDir);
    up = left.cross(lightDir);
    up.normalize();
}

bool PerspectiveShadowMapHandler::pntInFrontOf(Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p)
{
    Vec3f   dir1, dir2, testdir, norm;
    dir1 = p2 - p1;
    dir2 = p3 - p2;
    dir1.normalize();
    dir2.normalize();
    norm = dir1.cross(dir2);
    norm.normalize();
    testdir = p2 - p;
    testdir.normalize();
    if(testdir.dot(norm) < 0)
        return false;
    else
        return true;
}

void PerspectiveShadowMapHandler::calcCubicHull2(Pnt3f &min, Pnt3f &max,
                                                 std::vector<Pnt3f> *points)
{
    min = Pnt3f(1000000.0, 1000000.0, 1000000.0);
    max = Pnt3f(-1000000.0, -1000000.0, -1000000.0);

    for(UInt32 i = 1;i < points->size();i++)
    {
        for(UInt32 j = 0;j < 3;j++)
        {
            if((*points)[i][j] < min[j])
                min[j] = (*points)[i][j];
            if((*points)[i][j] > max[j])
                max[j] = (*points)[i][j];
        }
    }
}

void PerspectiveShadowMapHandler::scaleTranslateToFit2(Matrix &mat, 
                                                       const Pnt3f vMin,
                                                       const Pnt3f vMax)
{
    Real32  output[16];
    output[ 0] = 2 / (vMax[0] - vMin[0]);
    output[ 4] = 0;
    output[ 8] = 0;
    output[12] = -(vMax[0] + vMin[0]) / (vMax[0] - vMin[0]);

    output[ 1] = 0;
    output[ 5] = 2 / (vMax[1] - vMin[1]);
    output[ 9] = 0;
    output[13] = -(vMax[1] + vMin[1]) / (vMax[1] - vMin[1]);

    output[ 2] = 0;
    output[ 6] = 0;
    output[10] = 2 / (vMax[2] - vMin[2]);
    output[14] = -(vMax[2] + vMin[2]) / (vMax[2] - vMin[2]);

    output[ 3] = 0;
    output[ 7] = 0;
    output[11] = 0;
    output[15] = 1;
    mat.setValueTransposed(output[0], output[1], output[2], output[3],
                           output[4], output[5], output[6], output[7],
                           output[8], output[9], output[10], output[11],
                           output[12], output[13], output[14], output[15]);
}

bool PerspectiveShadowMapHandler::bbInsideFrustum(Pnt3f sceneMin, 
                                                  Pnt3f sceneMax,
                                                  Matrix LPVM)
{
    Matrix  iLPVM(LPVM);
    iLPVM.invert();

    LPVM.multFull(sceneMin, sceneMin);
    LPVM.multFull(sceneMax, sceneMax);

    //Scene Bounding Box Points
    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    bool    isIn[8];
    for(UInt32 i = 0;i < 8;i++)
        isIn[i] = false;

    if(bb0[0] < 1 && bb0[0] > -1 && bb0[1] < 1 && bb0[1] > -1 && bb0[2] < 1 &&
       bb0[2] > -1)
        isIn[0] = true;
    if(bb1[0] < 1 && bb1[0] > -1 && bb1[1] < 1 && bb1[1] > -1 && bb1[2] < 1 &&
       bb1[2] > -1)
        isIn[1] = true;
    if(bb2[0] < 1 && bb2[0] > -1 && bb2[1] < 1 && bb2[1] > -1 && bb2[2] < 1 &&
       bb2[2] > -1)
        isIn[2] = true;
    if(bb3[0] < 1 && bb3[0] > -1 && bb3[1] < 1 && bb3[1] > -1 && bb3[2] < 1 &&
       bb3[2] > -1)
        isIn[3] = true;
    if(bb4[0] < 1 && bb4[0] > -1 && bb4[1] < 1 && bb4[1] > -1 && bb4[2] < 1 &&
       bb4[2] > -1)
        isIn[4] = true;
    if(bb5[0] < 1 && bb5[0] > -1 && bb5[1] < 1 && bb5[1] > -1 && bb5[2] < 1 &&
       bb5[2] > -1)
        isIn[5] = true;
    if(bb6[0] < 1 && bb6[0] > -1 && bb6[1] < 1 && bb6[1] > -1 && bb6[2] < 1 &&
       bb6[2] > -1)
        isIn[6] = true;
    if(bb7[0] < 1 && bb7[0] > -1 && bb7[1] < 1 && bb7[1] > -1 && bb7[2] < 1 &&
       bb7[2] > -1)
        isIn[7] = true;

    if(isIn[0] && isIn[1] && isIn[2] && isIn[3] && isIn[4] && isIn[5] &&
       isIn[6] && isIn[7])
        return true;
    else
        return false;
}


void PerspectiveShadowMapHandler::createShadowMapsNOGLSL(DrawEnv *pEnv)
{
    OSG_ASSERT(false);
#if 0
    if(_tiledeco == NULL)
    {
        _tiledeco = TileCameraDecorator::create();
    }

    //Checking for the smallest Window-Dimension
    UInt32  minSize = _shadowVP->getPixelWidth();

    if(_shadowVP->getPixelHeight() < minSize)
        minSize = _shadowVP->getPixelHeight();

    //Checking for biggest PowerOf2 that fits in smallest Window-Dimension
    UInt32  _mapRenderSize = osgNextPower2(minSize + 1) / 2;
    if(_mapRenderSize == 0)
        _mapRenderSize = 128;

    Real32  vpTop, vpBottom, vpLeft, vpRight;

    //------Setting up Window to fit size of ShadowMap----------------

    // Saving original Viewport-Dimensions
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();

    //Temporarily switching Viewports size to size of ShadowMap | OpenSG-Level
    _shadowVP->setVPSize(0, 0, _mapRenderSize - 1, _mapRenderSize - 1);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);

    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // deactivate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            exnode->setTravMask(0);
    }

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                // we use a tiledecorator to create shadow maps with
                // a higher resolutions than the viewport or the screen.
                Matrix  LPM, LVM;// = MatrixCameraPtr::dcast(_shadowVP->_lightCameras[i])->getProjectionMatrix();
                Matrix  CPM, CVM, CVPM;

                _matrixCam2->setProjectionMatrix(_perspectiveLPM[i]);
                _matrixCam2->setModelviewMatrix(_perspectiveLVM[i]);

                _tiledeco->setDecoratee(_matrixCam2);
                _tiledeco->setFullSize(_shadowVP->getMapSize(),
                                       _shadowVP->getMapSize());

                pEnv->getAction()->setCamera(_tiledeco);

                Real32  step = (1.0 / Real32(_shadowVP->getMapSize())) *
                    Real32(_mapRenderSize);

                UInt32  ypos = 0;
                for(Real32 y = 0;y < 1.0;y += step)
                {
                    UInt32  xpos = 0;
                    for(Real32 x = 0;x < 1.0;x += step)
                    {
                        _tiledeco->setSize(x, y, x + step, y + step);

                        glClear(GL_DEPTH_BUFFER_BIT);
                        glPolygonOffset(_shadowVP->getOffFactor(),
                                        _shadowVP->getOffBias());
                        glEnable(GL_POLYGON_OFFSET_FILL);

                        _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);

                        pEnv->getWindow()->validateGLObject(
                            _shadowVP->_texChunks[i]->getGLId(),
                            pEnv);

                        glDisable(GL_POLYGON_OFFSET_FILL);

                        //----------Shadow-Texture-Parameters and Indices-------------

                        glBindTexture(GL_TEXTURE_2D,
                                      pEnv->getWindow()->getGLObjectId(
                                      _shadowVP->_texChunks[i]->getGLId()));
                        if(glGetError() != GL_NO_ERROR)
                            SWARNING << "Error on binding Texture!" << endLog;

                        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, xpos, ypos, 0, 0,
                                            _mapRenderSize, _mapRenderSize);

                        if(glGetError() != GL_NO_ERROR)
                            SWARNING << "Error on copying Texture!" << endLog;

                        glBindTexture(GL_TEXTURE_2D, 0);
                        if(glGetError() != GL_NO_ERROR)
                            SWARNING << "Error on releasing Texture!" <<
                                endLog;

                        if(glGetError() != GL_NO_ERROR)
                            SWARNING << "Error while Texture-Creation!" <<
                                endLog;

                        xpos += _mapRenderSize;
                    }
                    ypos += _mapRenderSize;
                }
            }
        }
    }

    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }

    //-------Restoring old states of Window and Viewport----------

    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
    pEnv->getAction()->setCamera(_shadowVP->getCamera());
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
#endif
}

void PerspectiveShadowMapHandler::createShadowMaps(DrawEnv *pEnv,
                                                   RenderAction *pTmpAction)
{
    if(_tiledeco == NULL)
    {
        _tiledeco = TileCameraDecorator::create();
    }

    Real32  vpTop, vpBottom, vpLeft, vpRight;

    //------Setting up Window to fit size of ShadowMap----------------
    // Saving original Viewport-Dimensions
#if 0
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();
#endif

    vpTop    = pEnv->getPixelTop();
    vpBottom = pEnv->getPixelBottom();
    vpLeft   = pEnv->getPixelLeft();
    vpRight  = pEnv->getPixelRight();

    //Checking for the smallest Window-Dimension
#if 0
    UInt32  minSize = _shadowVP->getPixelWidth();

    if(_shadowVP->getPixelHeight() < minSize)
        minSize = _shadowVP->getPixelHeight();
#endif
    UInt32  minSize = pEnv->getPixelWidth();

    if(pEnv->getPixelHeight() < minSize)
        minSize = pEnv->getPixelHeight();

    //Checking for biggest PowerOf2 that fits in smallest Window-Dimension
    UInt32  _mapRenderSize = osgNextPower2(minSize + 1) / 2;
    if(_mapRenderSize == 0)
        _mapRenderSize = 128;

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);

#if 0
#endif

    Viewport *pVP = pTmpAction->getViewport();

    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // deactivate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            exnode->setTravMask(0);
    }

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                if(_shadowVP->_lights[i].second->getType() != PointLight::getClassType
                   () || !_shadowVP->_realPointLight[i])
                {
                    if(_mapRenderSize > _shadowVP->getMapSize())
                        _mapRenderSize = _shadowVP->getMapSize();

#if 0
                    _shadowVP->setVPSize(0, 0, _mapRenderSize - 1,
                                         _mapRenderSize - 1);
#endif
                    pVP->setSize(0, 0, 
                                 _mapRenderSize - 1,
                                 _mapRenderSize - 1);
                    pVP->activate();

                    // we use a tiledecorator to create shadow maps with
                    // a higher resolutions than the viewport or the screen.

                    _matrixCam2->setProjectionMatrix(_perspectiveLPM[i]);
                    _matrixCam2->setModelviewMatrix(_perspectiveLVM[i]);

                    _tiledeco->setDecoratee(_matrixCam2);
                    _tiledeco->setFullSize(_shadowVP->getMapSize(),
                                           _shadowVP->getMapSize());

#if 0
                    pEnv->getAction()->setCamera(_tiledeco);
#endif
                    pTmpAction->setCamera(_tiledeco);

                    Real32  step = (1.0 / Real32(_shadowVP->getMapSize())) *
                        Real32(_mapRenderSize);

                    UInt32  ypos = 0;
                    for(Real32 y = 0;y < 1.0;y += step)
                    {
                        UInt32  xpos = 0;
                        for(Real32 x = 0;x < 1.0;x += step)
                        {
                            _tiledeco->setSize(x, y, x + step, y + step);

                            glClear(GL_DEPTH_BUFFER_BIT);
                            glPolygonOffset(_shadowVP->getOffFactor(),
                                            _shadowVP->getOffBias());
                            glEnable(GL_POLYGON_OFFSET_FILL);

#if 0
                            _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);
#endif
                            _shadowVP->renderLight(pTmpAction, _unlitMat, i);

                            // check is this necessary.
                            pEnv->getWindow()->validateGLObject(
                                _shadowVP->_texChunks[i]->getGLId(),
                                pEnv);

                            glDisable(GL_POLYGON_OFFSET_FILL);

                            //----------Shadow-Texture-Parameters and Indices-------------

                            glBindTexture(GL_TEXTURE_2D,
                                          pEnv->getWindow()->getGLObjectId(
                                          _shadowVP->_texChunks[i]->getGLId
                                          ()));
                            if(glGetError() != GL_NO_ERROR)
                                SWARNING << "Error on binding Texture!" <<
                                    endLog;

                            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, xpos, ypos,
                                                0, 0,
                                                _mapRenderSize,
                                                _mapRenderSize);

                            if(glGetError() != GL_NO_ERROR)
                                SWARNING << "Error on copying Texture!" <<
                                    endLog;

                            glBindTexture(GL_TEXTURE_2D, 0);
                            if(glGetError() != GL_NO_ERROR)
                                SWARNING << "Error on releasing Texture!" <<
                                    endLog;

                            if(glGetError() != GL_NO_ERROR)
                                SWARNING << "Error while Texture-Creation!" <<
                                    endLog;

                            xpos += _mapRenderSize;
                        }
                        ypos += _mapRenderSize;
                    }
                }
                else
                {
                    if(_mapRenderSize > _PLMapSize)
                        _mapRenderSize = _PLMapSize;

#if 0
                    _shadowVP->setVPSize(0, 0, _mapRenderSize - 1,
                                         _mapRenderSize - 1);
#endif
                    pVP->setSize(0, 0, _mapRenderSize - 1,
                                 _mapRenderSize - 1);
                    pVP->activate();

                    MatrixCameraDecoratorUnrecPtr    deco =
                        MatrixCameraDecorator::create();

                    for(UInt32 j = 0;j < 6;j++)
                    {
                        //Offset berechnen
                        UInt32  xOffset, yOffset;
                        if(j == 0)
                        {
                            xOffset = 0;
                            yOffset = 0;
                        }
                        else if(j == 1)
                        {
                            xOffset = _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 2)
                        {
                            xOffset = 2 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 3)
                        {
                            xOffset = 3 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 4)
                        {
                            xOffset = 0;
                            yOffset = _PLMapSize;
                        }
                        else
                        {
                            xOffset = _PLMapSize;
                            yOffset = _PLMapSize;
                        }

                        deco->setDecoratee(_shadowVP->_lightCameras[i]);
                        deco->setPreProjection(_transforms[j]);

                        // we use a tiledecorator to create shadow maps with
                        // a higher resolutions than the viewport or the screen.
                        _tiledeco->setDecoratee(deco);
                        _tiledeco->setFullSize(_PLMapSize, _PLMapSize);

#if 0
                        pEnv->getAction()->setCamera(_tiledeco);
#endif
                        pTmpAction->setCamera(_tiledeco);

                        Real32  step = (1.0 / Real32(_PLMapSize)) *
                            Real32(_mapRenderSize);

                        UInt32  ypos = 0;
                        for(Real32 y = 0;y < 1.0;y += step)
                        {
                            UInt32  xpos = 0;
                            for(Real32 x = 0;x < 1.0;x += step)
                            {
                                _tiledeco->setSize(x, y, x + step, y + step);

                                glClear(GL_DEPTH_BUFFER_BIT);
                                glPolygonOffset(_shadowVP->getOffFactor(),
                                                _shadowVP->getOffBias());
                                glEnable(GL_POLYGON_OFFSET_FILL);

#if 0
                                _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);
#endif
                                _shadowVP->renderLight(pTmpAction, 
                                                       _unlitMat, i);

                                pEnv->getWindow()->validateGLObject(
                                    _shadowVP->_texChunks[i]->getGLId(),
                                    pEnv);

                                glDisable(GL_POLYGON_OFFSET_FILL);

                                //----------Shadow-Texture-Parameters and Indices-------------

                                glBindTexture(GL_TEXTURE_2D,
                                              pEnv->getWindow()->getGLObjectId(
                                              _shadowVP->_texChunks[i]->getGLId
                                              ()));
                                if(glGetError() != GL_NO_ERROR)
                                    SWARNING << "Error on binding Texture!" <<
                                        endLog;

                                glCopyTexSubImage2D(GL_TEXTURE_2D, 0,
                                                    xpos + xOffset,
                                                    ypos + yOffset, 0, 0,
                                                    _mapRenderSize,
                                                    _mapRenderSize);

                                if(glGetError() != GL_NO_ERROR)
                                    SWARNING << "Error on copying Texture!" <<
                                        endLog;

                                glBindTexture(GL_TEXTURE_2D, 0);
                                if(glGetError() != GL_NO_ERROR)
                                    SWARNING <<
                                        "Error on releasing Texture!" <<
                                        endLog;

                                if(glGetError() != GL_NO_ERROR)
                                    SWARNING << "Error while Texture-Creation!"
                                        << endLog;

                                xpos += _mapRenderSize;
                            }
                            ypos += _mapRenderSize;
                        }
                    }
                }
            }
        }
    }

    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }

    //-------Restoring old states of Window and Viewport----------

#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif
    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();

#if 0
    pEnv->getAction()->setCamera(_shadowVP->getCamera());
#endif
    pTmpAction->setCamera(pEnv->getAction()->getCamera());

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}

void PerspectiveShadowMapHandler::createShadowMapsFBO(DrawEnv *pEnv, 
                                                      RenderAction *pTmpAction)
{
    Real32  vpTop, vpBottom, vpLeft, vpRight;

    //------Setting up Window to fit size of ShadowMap----------------
    // Saving original Viewport-Dimensions
#if 0
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();
#endif
    vpTop    = pEnv->getPixelTop();
    vpBottom = pEnv->getPixelBottom();
    vpLeft   = pEnv->getPixelLeft();
    vpRight  = pEnv->getPixelRight();

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);

    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // deactivate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            exnode->setTravMask(0);
    }

#if 0
#endif

    Viewport *pVP = pTmpAction->getViewport();

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i])
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                //------Setting up Window to fit size of ShadowMap----------------

#if 0
                _shadowVP->setVPSize(0, 0,
                                     _shadowVP->_texChunks[i]->getImage
                                     ()->getWidth() - 1,
                                     _shadowVP->_texChunks[i]->getImage
                                     ()->getHeight() - 1);
#endif
                pVP->setSize(0, 0,
                             _shadowVP->_texChunks[i]->getImage
                             ()->getWidth() - 1,
                             _shadowVP->_texChunks[i]->getImage
                             ()->getHeight() - 1);
                pVP->activate();

                if(_shadowVP->_lights[i].second->getType() != PointLight::getClassType
                   () || !_shadowVP->_realPointLight[i])
                {

                    pEnv->getWindow()->validateGLObject(
                        _shadowVP->_texChunks[i]->getGLId(),
                        pEnv);

                    _matrixCam2->setProjectionMatrix(_perspectiveLPM[i]);
                    _matrixCam2->setModelviewMatrix(_perspectiveLVM[i]);

                    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);

                    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT
                                              , GL_TEXTURE_2D,
                                              pEnv->getWindow
                                              ()->getGLObjectId(
                                              _shadowVP->_texChunks[i]->getGLId
                                              ()), 0);

                    glDrawBuffer(GL_NONE);
                    glReadBuffer(GL_NONE);

                    glPolygonOffset(_shadowVP->getOffFactor(),
                                    _shadowVP->getOffBias());
                    glEnable(GL_POLYGON_OFFSET_FILL);

                    glClearColor(1.0, 1.0, 1.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
                    _shadowVP->setVPSize(0, 0, _shadowVP->getMapSize(),
                                         _shadowVP->getMapSize());
#endif

                    pVP->setSize(0, 0, 
                                 _shadowVP->getMapSize(),
                                 _shadowVP->getMapSize());
                    pVP->activate();

#if 0
                    pEnv->getAction()->setCamera(_matrixCam2);
                    _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);
#endif
                    pTmpAction->setCamera(_matrixCam2);
                    _shadowVP->renderLight(pTmpAction, _unlitMat, i);

                    glDisable(GL_POLYGON_OFFSET_FILL);

                    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

                    glClearColor(0.0, 0.0, 0.0, 1.0);

#if 0
                    pEnv->getAction()->setCamera(_shadowVP->getCamera());
#endif
                    pTmpAction->setCamera(pEnv->getAction()->getCamera());
                }
                else
                {
                    MatrixCameraDecoratorUnrecPtr    deco =
                        MatrixCameraDecorator::create();
                    pEnv->getWindow()->validateGLObject(
                        _shadowVP->_texChunks[i]->getGLId(),
                        pEnv);

                    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);

                    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT
                                              , GL_TEXTURE_2D,
                                              pEnv->getWindow
                                              ()->getGLObjectId(
                                              _shadowVP->_texChunks[i]->getGLId
                                              ()), 0);

                    glDrawBuffer(GL_NONE);
                    glReadBuffer(GL_NONE);

                    glClearColor(1.0, 1.0, 1.0, 1.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    for(UInt32 j = 0;j < 6;j++)
                    {
                        //Offset berechnen
                        UInt32  xOffset, yOffset;
                        if(j == 0)
                        {
                            xOffset = 0;
                            yOffset = 0;
                        }
                        else if(j == 1)
                        {
                            xOffset = _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 2)
                        {
                            xOffset = 2 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 3)
                        {
                            xOffset = 3 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 4)
                        {
                            xOffset = 0;
                            yOffset = _PLMapSize;
                        }
                        else
                        {
                            xOffset = _PLMapSize;
                            yOffset = _PLMapSize;
                        }

#if 0
                        _shadowVP->setVPSize(xOffset, yOffset,
                                             xOffset + _PLMapSize,
                                             yOffset + _PLMapSize);
#endif
                        pVP->setSize(xOffset, yOffset,
                                     xOffset + _PLMapSize,
                                     yOffset + _PLMapSize);
                        pVP->activate();

                        deco->setDecoratee(_shadowVP->_lightCameras[i]);
                        deco->setPreProjection(_transforms[j]);

                        glPolygonOffset(_shadowVP->getOffFactor(),
                                        _shadowVP->getOffBias());
                        glEnable(GL_POLYGON_OFFSET_FILL);

#if 0
                        pEnv->getAction()->setCamera(deco);

                        _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);
#endif
                        pTmpAction->setCamera(deco);

                        _shadowVP->renderLight(pTmpAction, _unlitMat, i);

                        glDisable(GL_POLYGON_OFFSET_FILL);
                    }
                    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
                    glClearColor(0.0, 0.0, 0.0, 1.0);
#if 0
                    pEnv->getAction()->setCamera(_shadowVP->getCamera());
#endif
                    pTmpAction->setCamera(pEnv->getAction()->getCamera());

                }
            }
        }
    }

    //-------Restoring old states of Window and Viewport----------

    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }

    //-------Restoring old states of Window and Viewport----------
#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif
    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}

void PerspectiveShadowMapHandler::projectShadowMaps(DrawEnv *pEnv, 
                                                    RenderAction *pTmpAction)
{

    Matrix                          viewMatrix, projectionMatrix, biasMatrix;
    Vec4f                           s, t, r, q;

    biasMatrix.setIdentity();
    biasMatrix.setScale(0.5);
    biasMatrix.setTranslate(0.5, 0.5, 0.5);

#if 0
    GLint                           pl = _shadowVP->getPixelLeft(), 
                                    pr = _shadowVP->getPixelRight(), 
                                    pb = _shadowVP->getPixelBottom(),
                                    pt = _shadowVP->getPixelTop();
#endif
    GLint                           pl = pEnv->getPixelLeft(), 
                                    pr = pEnv->getPixelRight(), 
                                    pb = pEnv->getPixelBottom(),
                                    pt = pEnv->getPixelTop();
    GLint                           pw = pr - pl + 1, ph = pt - pb + 1;
#if 0
    bool                            full = _shadowVP->isFullWindow();
#endif
    bool                            full = pEnv->getFull();

    glViewport(pl, pb, pw, ph);
    glScissor(pl, pb, pw, ph);

    if(!full)
        glEnable(GL_SCISSOR_TEST);

    // draw background
#if 0
    _shadowVP->getBackground()->clear(pEnv);
#endif
    pEnv->getAction()->getBackground()->clear(pEnv);

    //---Draw/Render-Pass with ambient light only & no shadows------
    std::vector<int>                light_state;
    std::vector<Color4f>            _light_specular;
    std::vector<Color4f>            _light_diffuse;
    std::vector<Color4f>            _light_ambient;

    Real32                          activeLights = 0;
    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
        activeLights = _shadowVP->_lights.size();
    else
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0 &&
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                activeLights++;
        }
    }

    for(UInt32 j = 0;j < _shadowVP->_lights.size();++j) // Switching off specular highlights
    {
        _light_specular.push_back(_shadowVP->_lights[j].second->getSpecular());
        _light_diffuse.push_back(_shadowVP->_lights[j].second->getDiffuse());
        _light_ambient.push_back(_shadowVP->_lights[j].second->getAmbient());

        Real32  shadowCol;
        if(_shadowVP->getGlobalShadowIntensity() != 0.0)
            shadowCol = ((1.0 - _shadowVP->getGlobalShadowIntensity()) *
                         (1.0 / activeLights));
        else
            shadowCol = ((1.0 - _shadowVP->_lights[j].second->getShadowIntensity()) *
                         (1.0 / activeLights));
        Color4f shadow_color = Color4f(shadowCol, shadowCol, shadowCol, 1.0);


        _shadowVP->_lights[j].second->setSpecular(0.0, 0.0, 0.0, 1.0);
        _shadowVP->_lights[j].second->setAmbient(0.0, 0.0, 0.0, 1.0);
        _shadowVP->_lights[j].second->setDiffuse(shadow_color);
    }

    // render
#if 0
    pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
    pTmpAction->apply(_shadowVP->getSceneRoot());

    // get all lights
    for(UInt32 j = 0;j < _shadowVP->_lights.size();++j) // Switching on specular highlights
    {
        Real32  shadowCol;
        if(_shadowVP->getGlobalShadowIntensity() != 0.0)
            shadowCol = ((1.0 - _shadowVP->getGlobalShadowIntensity()) *
                         (1.0 / activeLights));
        else
            shadowCol = ((1.0 - _shadowVP->_lights[j].second->getShadowIntensity()) *
                         (1.0 / activeLights));
        Color4f shadow_color = Color4f(shadowCol, shadowCol, shadowCol, 1.0);

        Color4f lightDiff;
        lightDiff[0] = _light_diffuse[j][0] - shadow_color[0];
        lightDiff[1] = _light_diffuse[j][1] - shadow_color[1];
        lightDiff[2] = _light_diffuse[j][2] - shadow_color[2];
        lightDiff[3] = 1.0;

        _shadowVP->_lights[j].second->setSpecular(_light_specular[j]);
        _shadowVP->_lights[j].second->setDiffuse(lightDiff);
        _shadowVP->_lights[j].second->setAmbient(_light_ambient[j]);
    }
    for(UInt32 j = 0;j < _shadowVP->_lights.size();++j)
    {
        light_state.push_back(_shadowVP->_lights[j].second->getOn());
        _shadowVP->_lights[j].second->setOn(false);
    }

    {
        _blender->setSrcFactor(GL_ONE);
        _blender->setDestFactor(GL_ONE);
        _blender->setAlphaFunc(GL_GEQUAL);
        _blender->setAlphaValue(0.99);
    }

    // switch off all transparent geos
    for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
        _shadowVP->_transparent[t]->setTravMask(0);

    _shadowVP->_offset->setOffsetBias(-1);
    _shadowVP->_offset->setOffsetFactor(0);
    _shadowVP->_offset->setOffsetFill(true);

    glDepthFunc(GL_LESS);

    //---Render-Pass with Shadow-----------------------------
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                glBindTexture(GL_TEXTURE_2D,
                              pEnv->getWindow()->getGLObjectId(
                              _shadowVP->_texChunks[i]->getGLId()));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                                GL_COMPARE_R_TO_TEXTURE_ARB);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,
                                GL_LEQUAL);
                glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB,
                                GL_LUMINANCE);
                glBindTexture(GL_TEXTURE_2D, 0);

                Matrix  textureMatrix = biasMatrix;
                textureMatrix.mult(_perspectiveLPM[i]);
                textureMatrix.mult(_perspectiveLVM[i]);

                textureMatrix.transpose();
                Vec4f   ps = textureMatrix[0];
                Vec4f   pt = textureMatrix[1];
                Vec4f   pr = textureMatrix[2];
                Vec4f   pq = textureMatrix[3];

                _shadowVP->_texGen->setGenFuncSPlane(ps);
                _shadowVP->_texGen->setGenFuncTPlane(pt);
                _shadowVP->_texGen->setGenFuncRPlane(pr);
                _shadowVP->_texGen->setGenFuncQPlane(pq);

                _shadowVP->_lights[i].second->setOn(true);

                _shadowVP->_texChunks[i]->activate(pEnv, 3);

                _shadowVP->_texGen->activate(pEnv, 3);

                _blender->activate(pEnv, 0);

                _shadowVP->_offset->activate(pEnv, 0);

#if 0
                pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
                pTmpAction->apply(_shadowVP->getSceneRoot());

                _shadowVP->_offset->deactivate(pEnv, 0);

                _blender->deactivate(pEnv, 0);

                _shadowVP->_texGen->deactivate(pEnv, 3);

                _shadowVP->_texChunks[i]->deactivate(pEnv, 3);

                _shadowVP->_lights[i].second->setOn(false);

                // increase offset for next light
                _shadowVP->_offset->setOffsetBias(
                    _shadowVP->_offset->getOffsetBias() - 1);
            }
        }
    }

    glDepthFunc(GL_LEQUAL);

    // switch on all transparent geos
    for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
        _shadowVP->_transparent[t]->setTravMask(TypeTraits<UInt32>::BitsSet);

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i) // Switching on ambient
    {
        _shadowVP->_lights[i].second->setAmbient(_light_ambient[i]);
    }

    for(UInt32 j = 0;j < _shadowVP->_lights.size();++j)
    {
        _shadowVP->_lights[j].second->setOn(light_state[j]);
    }

    // get all lights
    for(UInt32 j = 0;j < _shadowVP->_lights.size();++j) // Switching on specular highlights
    {
        _shadowVP->_lights[j].second->setSpecular(_light_specular[j]);
        _shadowVP->_lights[j].second->setDiffuse(_light_diffuse[j]);
        _shadowVP->_lights[j].second->setAmbient(_light_ambient[j]);
    }
}

void PerspectiveShadowMapHandler::createColorMap(DrawEnv *pEnv,
                                                 RenderAction *pTmpAction)
{
#if 0
    if(_shadowVP->isFullWindow())
    {
        _shadowVP->getBackground()->clear(pEnv);
    }
#endif
    if(pEnv->getFull())
    {
        pEnv->getAction()->getBackground()->clear(pEnv);
    }
    else
    {
        // HACK but we need this for a correct clear.
#if 0
        GLint   pl = _shadowVP->getPixelLeft(), 
                pr = _shadowVP->getPixelRight(),
                pb = _shadowVP->getPixelBottom(),
                pt = _shadowVP->getPixelTop();
#endif
        GLint   pl = pEnv->getPixelLeft(), 
                pr = pEnv->getPixelRight(),
                pb = pEnv->getPixelBottom(),
                pt = pEnv->getPixelTop();
        GLint   pw = pr - pl + 1, ph = pt - pb + 1;
        glViewport(pl, pb, pw, ph);
        glScissor(pl, pb, pw, ph);
        glEnable(GL_SCISSOR_TEST);
#if 0
        _shadowVP->getBackground()->clear(pEnv);
#endif
        pEnv->getAction()->getBackground()->clear(pEnv);
        glDisable(GL_SCISSOR_TEST);
    }

#if 0
    pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
    pTmpAction->apply(_shadowVP->getSceneRoot());

    // disable occluded lights.
#if 0
    _shadowVP->checkLightsOcclusion(pEnv->getAction());
#endif
    _shadowVP->checkLightsOcclusion(pTmpAction);

    pEnv->getWindow()->validateGLObject(_colorMapO->getGLId(), pEnv);

    _shadowVP->setReadBuffer(); // set the right read buffer for the copy texture.
    glBindTexture(GL_TEXTURE_2D,
                  pEnv->getWindow()->getGLObjectId(_colorMapO->getGLId()));
#if 0
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _shadowVP->getPixelLeft(),
                        _shadowVP->getPixelBottom(),
                        _shadowVP->getPixelWidth(),
                        _shadowVP->getPixelHeight());
#endif
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                        pEnv->getPixelLeft(),
                        pEnv->getPixelBottom(),
                        pEnv->getPixelWidth(),
                        pEnv->getPixelHeight());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void PerspectiveShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
                                                    RenderAction *pTmpAction)
{
    Real32  vpTop, vpBottom, vpLeft, vpRight;
#if 0
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();
#endif
    vpTop    = pEnv->getPixelTop();
    vpBottom = pEnv->getPixelBottom();
    vpLeft   = pEnv->getPixelLeft();
    vpRight  = pEnv->getPixelRight();

#if 0
    _shadowVP->setVPSize(0, 0, _shadowVP->getPixelWidth() - 1,
                         _shadowVP->getPixelHeight() - 1);
#endif

    Viewport *pVP = pTmpAction->getViewport();

    pVP->setSize(0, 0, 
                 pEnv->getPixelWidth() - 1,
                 pEnv->getPixelHeight() - 1);
    pVP->activate();
    
    Window  *win = pEnv->getWindow();

    GLenum  *buffers = NULL;
    buffers = new GLenum[1];
    buffers[0] = GL_COLOR_ATTACHMENT0_EXT;

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);

    _pFB->activate(pEnv);
    glDrawBuffer(*buffers);

#if 0
    GLint   pw = _shadowVP->getPixelWidth();
    GLint   ph = _shadowVP->getPixelHeight();
#endif

    GLint   pw = pEnv->getPixelWidth();
    GLint   ph = pEnv->getPixelHeight();

    glViewport(0, 0, pw, ph);
    glScissor(0, 0, pw, ph);
    glEnable(GL_SCISSOR_TEST);
#if 0
    _shadowVP->getBackground()->clear(pEnv);
#endif
    pEnv->getAction()->getBackground()->clear(pEnv);

    glDisable(GL_SCISSOR_TEST);

#if 0
    pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
    pTmpAction->apply(_shadowVP->getSceneRoot());

    // disable occluded lights.
    _shadowVP->checkLightsOcclusion(pEnv->getAction());

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    _pFB->deactivate(pEnv);

    delete[] buffers;
#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif
    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();
}

void PerspectiveShadowMapHandler::createShadowFactorMap(DrawEnv *pEnv,
                                                        RenderAction *pTmpAction)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if(_firstRun)
    {
#if 0
        if(_shadowVP->isFullWindow())
#endif
        if(pEnv->getFull())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            // HACK but we need this for a correct clear.
#if 0
            GLint   pl = _shadowVP->getPixelLeft(), 
                    pr = _shadowVP->getPixelRight(),
                    pb = _shadowVP->getPixelBottom(),
                    pt = _shadowVP->getPixelTop();
#endif
            GLint   pl = pEnv->getPixelLeft(), 
                    pr = pEnv->getPixelRight(),
                    pb = pEnv->getPixelBottom(),
                    pt = pEnv->getPixelTop();
            GLint   pw = pr - pl + 1, ph = pt - pb + 1;
            glViewport(pl, pb, pw, ph);
            glScissor(pl, pb, pw, ph);
            glEnable(GL_SCISSOR_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_SCISSOR_TEST);
        }
    }

    //Finde alle aktiven Lichtquellen
    Real32              activeLights = 0;
    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0)
                activeLights++;
        }
    }
    else
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0 &&
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                activeLights++;
        }
    }

    //Zuerst alle echte Pointlights
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               _shadowVP->_realPointLight[i])
            {
                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                else
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);

                Matrix  LVM, LPM, CVM;
#if 0
                _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                        _shadowVP->getPixelWidth(), _shadowVP->getPixelHeight());
                _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                           _shadowVP->getPixelWidth(), _shadowVP->getPixelHeight());
                _shadowVP->getCamera()->getViewing(CVM,
                                                   _shadowVP->getPixelWidth(),
                                                   _shadowVP->getPixelHeight
                                                   ());
#endif
                _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                        pEnv->getPixelWidth(),
                                                        pEnv->getPixelHeight());
                _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                           pEnv->getPixelWidth(), 
                                                           pEnv->getPixelHeight());
                pEnv->getAction()->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight
                                                   ());
                Matrix  iCVM = CVM;
                iCVM.invert();
				
                Real32  texFactor;
                if(_shadowVP->_lights[i].second->getType() == PointLight::getClassType
                   () || _shadowVP->_lights[i].second->getType() ==
                   SpotLight::getClassType())
                    texFactor = Real32(_width) / Real32(_height);
                else
                    texFactor = 1.0;

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);

                Real32  xFactor = 1.0;
                Real32  yFactor = 1.0;
	
                if(!_useNPOTTextures)
                {
                    xFactor = Real32(_width) / Real32(_widthHeightPOT);
                    yFactor = Real32(_height) / Real32(_widthHeightPOT);
                }

#if 0
                Matrix  m = pEnv->getAction()->getCamera()->getBeacon()->getToWorld();
#endif
                Matrix  m = pTmpAction->getCamera()->getBeacon()->getToWorld();

                Matrix  shadowMatrixOP = LVM;
                shadowMatrix.mult(iCVM);

                Matrix  shadowMatrixA = LPM;
                shadowMatrixA.mult(_transforms[0]);
                shadowMatrixA.mult(LVM);
                shadowMatrixA.mult(iCVM);

                Matrix  shadowMatrixB = LPM;
                shadowMatrixB.mult(_transforms[1]);
                shadowMatrixB.mult(LVM);
                shadowMatrixB.mult(iCVM);

                Matrix  shadowMatrixC = LPM;
                shadowMatrixC.mult(_transforms[2]);
                shadowMatrixC.mult(LVM);
                shadowMatrixC.mult(iCVM);

                Matrix  shadowMatrixD = LPM;
                shadowMatrixD.mult(_transforms[3]);
                shadowMatrixD.mult(LVM);
                shadowMatrixD.mult(iCVM);

                Matrix  shadowMatrixE = LPM;
                shadowMatrixE.mult(_transforms[4]);
                shadowMatrixE.mult(LVM);
                shadowMatrixE.mult(iCVM);

                Matrix  shadowMatrixF = LPM;
                shadowMatrixF.mult(_transforms[5]);
                shadowMatrixF.mult(LVM);
                shadowMatrixF.mult(iCVM);

                _shadowCubeSHL->addUniformVariable("shadowMap", 0);
                _shadowCubeSHL->addUniformVariable("oldFactorMap", 1);
                _shadowCubeSHL->addUniformVariable("firstRun", _firstRun);
                _shadowCubeSHL->addUniformVariable("intensity",
                                                    shadowIntensity);
                _shadowCubeSHL->addUniformVariable("texFactor", texFactor);
                _shadowCubeSHL->addUniformVariable("lightPMA", shadowMatrixA);
                _shadowCubeSHL->addUniformVariable("lightPMB", shadowMatrixB);
                _shadowCubeSHL->addUniformVariable("lightPMC", shadowMatrixC);
                _shadowCubeSHL->addUniformVariable("lightPMD", shadowMatrixD);
                _shadowCubeSHL->addUniformVariable("lightPME", shadowMatrixE);
                _shadowCubeSHL->addUniformVariable("lightPMF", shadowMatrixF);
                _shadowCubeSHL->addUniformVariable("lightPMOP",
                                                    shadowMatrixOP);
                _shadowCubeSHL->addUniformVariable("KKtoWK", m);
                _shadowCubeSHL->addUniformVariable("xFactor",
                                                    Real32(xFactor));
                _shadowCubeSHL->addUniformVariable("yFactor",
                                                    Real32(yFactor));

                _shadowCmat->clearChunks();
                _shadowCmat->addChunk(_shadowCubeSHL);
                _shadowCmat->addChunk(_shadowVP->_texChunks[i]);
                _shadowCmat->addChunk(_shadowFactorMapO);

#if 0
                _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
                _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

                pEnv->getWindow()->validateGLObject(_shadowFactorMapO->getGLId(),
                                                    pEnv);

                glBindTexture(GL_TEXTURE_2D,
                              pEnv->getWindow()->getGLObjectId(
                              _shadowFactorMapO->getGLId()));
#if 0
                glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                    _shadowVP->getPixelLeft(),
                                    _shadowVP->getPixelBottom(),
                                    _shadowVP->getPixelWidth(),
                                    _shadowVP->getPixelHeight());
#endif
                glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                    pEnv->getPixelLeft(),
                                    pEnv->getPixelBottom(),
                                    pEnv->getPixelWidth(),
                                    pEnv->getPixelHeight());
                glBindTexture(GL_TEXTURE_2D, 0);
                _firstRun = 0;
            }
        }
    }

    std::vector<Real32> shadowIntensityF;
    std::vector<Real32> mapFactorF;
    std::vector<Matrix> shadowMatrixF;

    UInt32              lightCounter = 0;

    Real32              xFactor = 1.0;
    Real32              yFactor = 1.0;
	
    if(!_useNPOTTextures)
    {
        xFactor = Real32(_width) / Real32(_widthHeightPOT);
        yFactor = Real32(_height) / Real32(_widthHeightPOT);
    }

    //Jetzt alle normalen Lichtquellen
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               !_shadowVP->_realPointLight[i])
            {

                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                else
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);
                shadowIntensityF.push_back(shadowIntensity);
				
                Matrix  LVM, LPM, CVM;
                LVM = _perspectiveLVM[i];
                LPM = _perspectiveLPM[i];
#if 0
                _shadowVP->getCamera()->getViewing(CVM,
                                                   _shadowVP->getPixelWidth(),
                                                   _shadowVP->getPixelHeight
                                                   ());
#endif
                pTmpAction->getCamera()->getViewing(CVM,
                                                    pEnv->getPixelWidth(),
                                                    pEnv->getPixelHeight
                                                   ());
                Matrix  iCVM = CVM;
                iCVM.invert();

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);
                shadowMatrixF.push_back(shadowMatrix);

                Real32  mapFactor;
                mapFactor = Real32(_shadowVP->getMapSize()) /
                    Real32(_shadowVP->_shadowImages[i]->getWidth());
                mapFactorF.push_back(mapFactor);
                lightCounter++;
            }
        }
    }

    if(lightCounter != 0)
    {
        UInt32  renderTimes = 1;
        if(lightCounter > 7)
            renderTimes = UInt32(ceil(Real32(lightCounter) / 7.0f));

        for(UInt32 i = 0;i < renderTimes;i++)
        {

            UInt32  lightOffset = lightCounter - (i * 7);
		
            //clear chunk and add Textures
            _shadowCmat->clearChunks();
	
            UInt32  lightNum = 0;
            for(UInt32 j = 0;j < _shadowVP->_lights.size();j++)
            {
                if(_shadowVP->_lightStates[j] != 0)
                {
                    if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                        _shadowVP->_lights[j].second->getShadowIntensity() != 0.0) &&
                       !_shadowVP->_realPointLight[j])
                    {
                        if(lightNum >= (i * 7) && lightNum < ((i + 1) * 7))
                        {
                            _shadowCmat->addChunk(_shadowVP->_texChunks[j]);
                        }
                        lightNum++;
                    }
                }
            }

            if(lightOffset == 1)
            {
                _shadowCmat->addChunk(_shadowSHL);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 7)]);
                _shadowSHL->addUniformVariable("lightPM", shadowMatrixF[0 +
                                                (i * 7)]);
                _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL->addUniformVariable("mapFactor",
                                                Real32(mapFactorF[0 +
                                                       (i * 7)]));
            }	

            else if(lightOffset == 2)
            {
                _shadowCmat->addChunk(_shadowSHL2);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL2->addUniformVariable("oldFactorMap", 2);
                _shadowSHL2->addUniformVariable("shadowMap1", 0);
                _shadowSHL2->addUniformVariable("shadowMap2", 1);
                _shadowSHL2->addUniformVariable("firstRun", _firstRun);
                _shadowSHL2->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL2->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL2->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL2->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
            }

            else if(lightOffset == 3)
            {
                _shadowCmat->addChunk(_shadowSHL3);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL3->addUniformVariable("oldFactorMap", 3);
                _shadowSHL3->addUniformVariable("shadowMap1", 0);
                _shadowSHL3->addUniformVariable("shadowMap2", 1);
                _shadowSHL3->addUniformVariable("shadowMap3", 2);
                _shadowSHL3->addUniformVariable("firstRun", _firstRun);
                _shadowSHL3->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL3->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL3->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 4)
            {
                _shadowCmat->addChunk(_shadowSHL4);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL4->addUniformVariable("oldFactorMap", 4);
                _shadowSHL4->addUniformVariable("shadowMap1", 0);
                _shadowSHL4->addUniformVariable("shadowMap2", 1);
                _shadowSHL4->addUniformVariable("shadowMap3", 2);
                _shadowSHL4->addUniformVariable("shadowMap4", 3);
                _shadowSHL4->addUniformVariable("firstRun", _firstRun);
                _shadowSHL4->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL4->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL4->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 5)
            {
                _shadowCmat->addChunk(_shadowSHL5);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL5->addUniformVariable("oldFactorMap", 5);
                _shadowSHL5->addUniformVariable("shadowMap1", 0);
                _shadowSHL5->addUniformVariable("shadowMap2", 1);
                _shadowSHL5->addUniformVariable("shadowMap3", 2);
                _shadowSHL5->addUniformVariable("shadowMap4", 3);
                _shadowSHL5->addUniformVariable("shadowMap5", 4);
                _shadowSHL5->addUniformVariable("firstRun", _firstRun);
                _shadowSHL5->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL5->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL5->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 6)
            {
                _shadowCmat->addChunk(_shadowSHL6);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL6->addUniformVariable("oldFactorMap", 6);
                _shadowSHL6->addUniformVariable("shadowMap1", 0);
                _shadowSHL6->addUniformVariable("shadowMap2", 1);
                _shadowSHL6->addUniformVariable("shadowMap3", 2);
                _shadowSHL6->addUniformVariable("shadowMap4", 3);
                _shadowSHL6->addUniformVariable("shadowMap5", 4);
                _shadowSHL6->addUniformVariable("shadowMap6", 5);
                _shadowSHL6->addUniformVariable("firstRun", _firstRun);
                _shadowSHL6->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL6->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL6->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
            }

            else
            {
                _shadowCmat->addChunk(_shadowSHL7);
                _shadowCmat->addChunk(_shadowFactorMapO);

                _shadowSHL7->addUniformVariable("oldFactorMap", 7);
                _shadowSHL7->addUniformVariable("shadowMap1", 0);
                _shadowSHL7->addUniformVariable("shadowMap2", 1);
                _shadowSHL7->addUniformVariable("shadowMap3", 2);
                _shadowSHL7->addUniformVariable("shadowMap4", 3);
                _shadowSHL7->addUniformVariable("shadowMap5", 4);
                _shadowSHL7->addUniformVariable("shadowMap6", 5);
                _shadowSHL7->addUniformVariable("shadowMap7", 6);
                _shadowSHL7->addUniformVariable("firstRun", _firstRun);
                _shadowSHL7->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity7",
                                                 shadowIntensityF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM7", shadowMatrixF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL7->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL7->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor7",
                                                 Real32(mapFactorF[6 + (i *
                                                                        7)]));
            }


#if 0
            _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
            _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

            pEnv->getWindow()->validateGLObject(_shadowFactorMapO->getGLId(),
                                                pEnv);

            glBindTexture(GL_TEXTURE_2D,
                          pEnv->getWindow()->getGLObjectId(
                          _shadowFactorMapO->getGLId()));
#if 0
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                _shadowVP->getPixelLeft(),
                                _shadowVP->getPixelBottom(),
                                _shadowVP->getPixelWidth(),
                                _shadowVP->getPixelHeight());
#endif
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                                pEnv->getPixelLeft(),
                                pEnv->getPixelBottom(),
                                pEnv->getPixelWidth(),
                                pEnv->getPixelHeight());
            glBindTexture(GL_TEXTURE_2D, 0);

            _firstRun = 0;
        }
    }
    _firstRun = 0;
    shadowIntensityF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();

}

void PerspectiveShadowMapHandler::createShadowFactorMapFBO(
    DrawEnv *pEnv,
    RenderAction *pTmpAction)
{
    Real32              vpTop, vpBottom, vpLeft, vpRight;

#if 0
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();
#endif

    vpTop    = pEnv->getPixelTop();
    vpBottom = pEnv->getPixelBottom();
    vpLeft   = pEnv->getPixelLeft();
    vpRight  = pEnv->getPixelRight();

#if 0
    _shadowVP->setVPSize(0, 0, _shadowVP->getPixelWidth() - 1,
                         _shadowVP->getPixelHeight() - 1);
#endif
    Viewport *pVP = pTmpAction->getViewport();

    pVP->setSize(0, 0, 
                 pEnv->getPixelWidth() - 1,
                 pEnv->getPixelHeight() - 1);

    pVP->activate();

    _activeFactorMap = 0;
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //Finde alle aktiven Lichtquellen
    Real32              activeLights = 0;
    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0)
                activeLights++;
        }
    }
    else
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0 &&
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                activeLights++;
        }
    }

#if 0
    {
        //clear all ShadowFactorMaps
        GLenum  *buffers = NULL;
        buffers = new GLenum[1];
        buffers[0] = GL_COLOR_ATTACHMENT1_EXT;

        //Setup FBO
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
        _pFB->activate(pEnv);

        glDrawBuffer(*buffers);
		
        //clear all ShadowFactorMaps
        // ACHTUNG der fbo kann nur 0,w,0,h rendern
        // damit es auch mit mehreren viewports klappt ...
#if 0
        GLint   pw = _shadowVP->getPixelWidth();
        GLint   ph = _shadowVP->getPixelHeight();
#endif
        GLint   pw = pEnv->getPixelWidth();
        GLint   ph = pEnv->getPixelHeight();

        glViewport(0, 0, pw, ph);
        glScissor(0, 0, pw, ph);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        delete[] buffers;

        buffers = new GLenum[1];
        buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

        //Setup FBO
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
        glDrawBuffer(*buffers);
		
        // ACHTUNG der fbo kann nur 0,w,0,h rendern
        // damit es auch mit mehreren viewports klappt ...
        glViewport(0, 0, pw, ph);
        glScissor(0, 0, pw, ph);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        _pFB->deactivate(pEnv);

        delete[] buffers;
    }
#endif

    bool bCA1Cleared = false;
    bool bCA2Cleared = false;

    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    //Zuerst alle echte Pointlights
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               _shadowVP->_realPointLight[i])
            {
                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                {
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                }
                else
                {
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);
                }

                Matrix  LVM, LPM, CVM;
#if 0
                _shadowVP->_lightCameras[i]->getViewing(
                    LVM,
                    _shadowVP->getPixelWidth(), 
                    _shadowVP->getPixelHeight());

                _shadowVP->_lightCameras[i]->getProjection(
                    LPM,
                    _shadowVP->getPixelWidth(), 
                    _shadowVP->getPixelHeight());

                _shadowVP->getCamera()->getViewing(
                    CVM,
                    _shadowVP->getPixelWidth(),
                    _shadowVP->getPixelHeight());
#endif

                _shadowVP->_lightCameras[i]->getViewing(
                    LVM,
                    pEnv->getPixelWidth(), 
                    pEnv->getPixelHeight());

                _shadowVP->_lightCameras[i]->getProjection(
                    LPM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());

                pTmpAction->getCamera()->getViewing(
                    CVM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());

                Matrix  iCVM = CVM;
                iCVM.invert();
				
                Real32  texFactor;
                if(_shadowVP->_lights[i].second->getType() == 
                                               PointLight::getClassType() || 
                   _shadowVP->_lights[i].second->getType() ==
                                               SpotLight::getClassType()   )
                {
                    texFactor = Real32(_width) / Real32(_height);
                }
                else
                {
                    texFactor = 1.0;
                }

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);

                Real32  xFactor = 1.0;
                Real32  yFactor = 1.0;
	
                if(!_useNPOTTextures)
                {
                    xFactor = Real32(_width) / Real32(_widthHeightPOT);
                    yFactor = Real32(_height) / Real32(_widthHeightPOT);
                }

#if 0
                Matrix  m = 
                    pEnv->getAction()->getCamera()->getBeacon()->getToWorld();
#endif
                Matrix  m = pTmpAction->getCamera()->getBeacon()->getToWorld();

                Matrix  shadowMatrixOP = LVM;
                shadowMatrix.mult(iCVM);

                Matrix  shadowMatrixA = LPM;
                shadowMatrixA.mult(_transforms[0]);
                shadowMatrixA.mult(LVM);
                shadowMatrixA.mult(iCVM);

                Matrix  shadowMatrixB = LPM;
                shadowMatrixB.mult(_transforms[1]);
                shadowMatrixB.mult(LVM);
                shadowMatrixB.mult(iCVM);

                Matrix  shadowMatrixC = LPM;
                shadowMatrixC.mult(_transforms[2]);
                shadowMatrixC.mult(LVM);
                shadowMatrixC.mult(iCVM);

                Matrix  shadowMatrixD = LPM;
                shadowMatrixD.mult(_transforms[3]);
                shadowMatrixD.mult(LVM);
                shadowMatrixD.mult(iCVM);

                Matrix  shadowMatrixE = LPM;
                shadowMatrixE.mult(_transforms[4]);
                shadowMatrixE.mult(LVM);
                shadowMatrixE.mult(iCVM);

                Matrix  shadowMatrixF = LPM;
                shadowMatrixF.mult(_transforms[5]);
                shadowMatrixF.mult(LVM);
                shadowMatrixF.mult(iCVM);

                _shadowCubeSHL->addUniformVariable("shadowMap", 0);
                _shadowCubeSHL->addUniformVariable("oldFactorMap", 1);
                _shadowCubeSHL->addUniformVariable("firstRun", _firstRun);
                _shadowCubeSHL->addUniformVariable("intensity",
                                                    shadowIntensity);
                _shadowCubeSHL->addUniformVariable("texFactor", texFactor);
                _shadowCubeSHL->addUniformVariable("lightPMA", shadowMatrixA);
                _shadowCubeSHL->addUniformVariable("lightPMB", shadowMatrixB);
                _shadowCubeSHL->addUniformVariable("lightPMC", shadowMatrixC);
                _shadowCubeSHL->addUniformVariable("lightPMD", shadowMatrixD);
                _shadowCubeSHL->addUniformVariable("lightPME", shadowMatrixE);
                _shadowCubeSHL->addUniformVariable("lightPMF", shadowMatrixF);
                _shadowCubeSHL->addUniformVariable("lightPMOP",
                                                    shadowMatrixOP);
                _shadowCubeSHL->addUniformVariable("KKtoWK", m);
                _shadowCubeSHL->addUniformVariable("xFactor",
                                                    Real32(xFactor));
                _shadowCubeSHL->addUniformVariable("yFactor",
                                                    Real32(yFactor));

                _shadowCmat->clearChunks();
                _shadowCmat->addChunk(_shadowCubeSHL);
                _shadowCmat->addChunk(_shadowVP->_texChunks[i]);

                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                GLenum  *buffers = NULL;
                buffers = new GLenum[1];
                if(_activeFactorMap == 0)
                    buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
                else
                    buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

#if 0
                //Setup FBO
                //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
                _pFB->activate(pEnv);

                glDrawBuffer(*buffers);

#if 0
                _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
                _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

                //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
                _pFB->deactivate(pEnv);
#endif

                a->pushPartition((RenderPartition::CopyWindow      |
                                  RenderPartition::CopyViewing     |
                                  RenderPartition::CopyProjection  |
                                  RenderPartition::CopyFrustum     |
                                  RenderPartition::CopyNearFar     |
                                  RenderPartition::CopyViewportSize),
                                 RenderPartition::StateSorting);
                {
                    RenderPartition *pPart = a->getActivePartition();

                    pPart->setRenderTarget(_pFB);
                    pPart->setDrawBuffer  (*buffers);

                    Node *light  = _shadowVP->_lights[i].first;
                    Node *parent =  light->getParent();

                    if(parent != NULL)
                    {
                        a->pushMatrix(parent->getToWorld());
                    }

                    if(_activeFactorMap == 0 && bCA1Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA1Cleared = true;
                    }
                    else if(bCA2Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA2Cleared = true;
                    }
                             
                    a->overrideMaterial(_shadowCmat, a->getActNode());
                    a->recurse(light);
                    a->overrideMaterial( NULL,       a->getActNode());

                    if(parent != NULL)
                    {
                        a->popMatrix();
                    }
                }
                a->popPartition();

                delete[] buffers;
                _firstRun = 0;
                if(_activeFactorMap == 0)
                    _activeFactorMap = 1;
                else
                    _activeFactorMap = 0;
            }
        }
    }

    std::vector<Real32> shadowIntensityF;
    std::vector<Real32> mapFactorF;
    std::vector<Matrix> shadowMatrixF;

    UInt32              lightCounter = 0;

    Real32              xFactor = 1.0;
    Real32              yFactor = 1.0;
	
    if(!_useNPOTTextures)
    {
        xFactor = Real32(_width) / Real32(_widthHeightPOT);
        yFactor = Real32(_height) / Real32(_widthHeightPOT);
    }

    //Jetzt alle normalen Lichtquellen
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               !_shadowVP->_realPointLight[i])
            {

                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                else
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);
                shadowIntensityF.push_back(shadowIntensity);
				
                Matrix  LVM, LPM, CVM;
                LVM = _perspectiveLVM[i];
                LPM = _perspectiveLPM[i];
#if 0
                _shadowVP->getCamera()->getViewing(CVM,
                                                   _shadowVP->getPixelWidth(),
                                                   _shadowVP->getPixelHeight
                                                   ());
#endif
                pTmpAction->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight
                                                   ());
                Matrix  iCVM = CVM;
                iCVM.invert();

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);
                shadowMatrixF.push_back(shadowMatrix);

                Real32  mapFactor;
                mapFactor = Real32(_shadowVP->getMapSize()) /
                    Real32(_shadowVP->_shadowImages[i]->getWidth());
                mapFactorF.push_back(mapFactor);
                lightCounter++;
            }
        }
    }

    if(lightCounter != 0)
    {
        GLenum  *buffers = NULL;
        buffers = new GLenum[1];
        if(_activeFactorMap == 0)
            buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
        else
            buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

        UInt32  renderTimes = 1;
        if(lightCounter > 7)
            renderTimes = UInt32(ceil(Real32(lightCounter) / 7.0f));

        for(UInt32 i = 0;i < renderTimes;i++)
        {

            UInt32  lightOffset = lightCounter - (i * 7);
		
            //clear chunk and add Textures
            _shadowCmat->clearChunks();
	
            UInt32  lightNum = 0;
            for(UInt32 j = 0;j < _shadowVP->_lights.size();j++)
            {
                if(_shadowVP->_lightStates[j] != 0)
                {
                    if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                        _shadowVP->_lights[j].second->getShadowIntensity() != 0.0) &&
                       !_shadowVP->_realPointLight[j])
                    {
                        if(lightNum >= (i * 7) && lightNum < ((i + 1) * 7))
                        {
                            _shadowCmat->addChunk(_shadowVP->_texChunks[j]);
                        }
                        lightNum++;
                    }
                }
            }

            if(lightOffset == 1)
            {
                _shadowCmat->addChunk(_shadowSHL);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 7)]);
                _shadowSHL->addUniformVariable("lightPM", shadowMatrixF[0 +
                                                (i * 7)]);
                _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL->addUniformVariable("mapFactor",
                                                Real32(mapFactorF[0 +
                                                       (i * 7)]));
            }	

            else if(lightOffset == 2)
            {
                _shadowCmat->addChunk(_shadowSHL2);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL2->addUniformVariable("oldFactorMap", 2);
                _shadowSHL2->addUniformVariable("shadowMap1", 0);
                _shadowSHL2->addUniformVariable("shadowMap2", 1);
                _shadowSHL2->addUniformVariable("firstRun", _firstRun);
                _shadowSHL2->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL2->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL2->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL2->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
            }

            else if(lightOffset == 3)
            {
                _shadowCmat->addChunk(_shadowSHL3);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL3->addUniformVariable("oldFactorMap", 3);
                _shadowSHL3->addUniformVariable("shadowMap1", 0);
                _shadowSHL3->addUniformVariable("shadowMap2", 1);
                _shadowSHL3->addUniformVariable("shadowMap3", 2);
                _shadowSHL3->addUniformVariable("firstRun", _firstRun);
                _shadowSHL3->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL3->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL3->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 4)
            {
                _shadowCmat->addChunk(_shadowSHL4);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL4->addUniformVariable("oldFactorMap", 4);
                _shadowSHL4->addUniformVariable("shadowMap1", 0);
                _shadowSHL4->addUniformVariable("shadowMap2", 1);
                _shadowSHL4->addUniformVariable("shadowMap3", 2);
                _shadowSHL4->addUniformVariable("shadowMap4", 3);
                _shadowSHL4->addUniformVariable("firstRun", _firstRun);
                _shadowSHL4->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL4->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL4->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 5)
            {
                _shadowCmat->addChunk(_shadowSHL5);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL5->addUniformVariable("oldFactorMap", 5);
                _shadowSHL5->addUniformVariable("shadowMap1", 0);
                _shadowSHL5->addUniformVariable("shadowMap2", 1);
                _shadowSHL5->addUniformVariable("shadowMap3", 2);
                _shadowSHL5->addUniformVariable("shadowMap4", 3);
                _shadowSHL5->addUniformVariable("shadowMap5", 4);
                _shadowSHL5->addUniformVariable("firstRun", _firstRun);
                _shadowSHL5->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL5->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL5->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 6)
            {
                _shadowCmat->addChunk(_shadowSHL6);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL6->addUniformVariable("oldFactorMap", 6);
                _shadowSHL6->addUniformVariable("shadowMap1", 0);
                _shadowSHL6->addUniformVariable("shadowMap2", 1);
                _shadowSHL6->addUniformVariable("shadowMap3", 2);
                _shadowSHL6->addUniformVariable("shadowMap4", 3);
                _shadowSHL6->addUniformVariable("shadowMap5", 4);
                _shadowSHL6->addUniformVariable("shadowMap6", 5);
                _shadowSHL6->addUniformVariable("firstRun", _firstRun);
                _shadowSHL6->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL6->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL6->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
            }

            else
            {
                _shadowCmat->addChunk(_shadowSHL7);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL7->addUniformVariable("oldFactorMap", 7);
                _shadowSHL7->addUniformVariable("shadowMap1", 0);
                _shadowSHL7->addUniformVariable("shadowMap2", 1);
                _shadowSHL7->addUniformVariable("shadowMap3", 2);
                _shadowSHL7->addUniformVariable("shadowMap4", 3);
                _shadowSHL7->addUniformVariable("shadowMap5", 4);
                _shadowSHL7->addUniformVariable("shadowMap6", 5);
                _shadowSHL7->addUniformVariable("shadowMap7", 6);
                _shadowSHL7->addUniformVariable("firstRun", _firstRun);
                _shadowSHL7->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity7",
                                                 shadowIntensityF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM7", shadowMatrixF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL7->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL7->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor7",
                                                 Real32(mapFactorF[6 + (i *
                                                                        7)]));
            }


#if 0
            //Setup FBO
            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
            _pFB->activate(pEnv);

            glDrawBuffer(*buffers);

#if 0
            _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
            _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

            //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            _pFB->deactivate(pEnv);
#endif

            a->pushPartition((RenderPartition::CopyWindow      |
                              RenderPartition::CopyViewing     |
                              RenderPartition::CopyProjection  |
                              RenderPartition::CopyFrustum     |
                              RenderPartition::CopyNearFar     |
                              RenderPartition::CopyViewportSize),
                             RenderPartition::StateSorting);
            {
                RenderPartition *pPart = a->getActivePartition();
                
                pPart->setRenderTarget(_pFB);
                pPart->setDrawBuffer  (*buffers);
                
                Node *light  = _shadowVP->_lights[i].first;
                Node *parent =  light->getParent();
                
                if(parent != NULL)
                {
                    a->pushMatrix(parent->getToWorld());
                }
                
                if(_activeFactorMap == 0 && bCA1Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA1Cleared = true;
                }
                else if(bCA2Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA2Cleared = true;
                }

                a->overrideMaterial(_shadowCmat, a->getActNode());
                a->recurse(light);
                a->overrideMaterial( NULL,       a->getActNode());

                if(parent != NULL)
                {
                    a->popMatrix();
                }
            }
            a->popPartition();

            _firstRun = 0;
            if(_activeFactorMap == 0)
                _activeFactorMap = 1;
            else
                _activeFactorMap = 0;
        }

        delete[] buffers;
    }
    _firstRun = 0;
    shadowIntensityF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();

#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif

    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();
}


void PerspectiveShadowMapHandler::render(DrawEnv      *pEnv,
                                         RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    if(!_useShadowExt)
    {
#if 0
        _shadowVP->Viewport::render(pEnv->getAction());
#endif
    }
    else
    {

        glPushAttrib(GL_ENABLE_BIT);

        if(!_initTexturesDone)
            initTextures(pEnv);

        if(_useFBO)
        {
            if(!initFBO(pEnv))
                printf("ERROR with FBOBJECT\n");
        }


        GLfloat globalAmbient[] =
        {
            0.0, 0.0, 0.0, 1.0
        };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            glBindTexture(GL_TEXTURE_2D,
                          pEnv->getWindow()->getGLObjectId(
                          _shadowVP->_texChunks[i]->getGLId()));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                            GL_COMPARE_R_TO_TEXTURE_ARB);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,
                            GL_LEQUAL);
            glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB,
                            GL_LUMINANCE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if(_shadowVP->getMapSize() / 4 > _maxPLMapSize)
            _PLMapSize = _maxPLMapSize;
        else
            _PLMapSize = _shadowVP->getMapSize() / 4;

        if(!_useGLSL)
        {
            for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
            {
                if(_shadowVP->_lightStates[i] != 0 &&
                   (_shadowVP->_lights[i].second->getShadowIntensity() != 0.0 ||
                    _shadowVP->getGlobalShadowIntensity() != 0.0))
                {
                    Matrix  _LPM, _LVM;
                    calcPerspectiveSpot(_LPM, _LVM, i, pEnv);
                    _perspectiveLPM.push_back(_LPM);
                    _perspectiveLVM.push_back(_LVM);
					
                }
                else
                {
                    Matrix  _LPM, _LVM;
                    _LPM.setIdentity();
                    _LVM.setIdentity();
                    _perspectiveLPM.push_back(_LPM);
                    _perspectiveLVM.push_back(_LVM);
                }
            }
	
            if(_shadowVP->getMapAutoUpdate())
            {
                createShadowMapsNOGLSL(pEnv);
            }
            else
            {
                if(_shadowVP->_trigger_update)
                {
                    createShadowMapsNOGLSL(pEnv);
                    _shadowVP->_trigger_update = false;
                }
            }
	
            if(!_shadowVP->_lights.empty() && !_shadowVP->_lightCameras.empty
               ())
            {
                projectShadowMaps(pEnv, pTmpAction);
            }
            else
            {
                FDEBUG(("Rendering without Shadows\n"));
#if 0
                _shadowVP->Viewport::render(pEnv->getAction());
#endif
            }
        }
        else
        {
            _firstRun = 1;
			
#if 0
            if(_shadowVP->getPixelWidth() != _width ||
               _shadowVP->getPixelHeight() != _height)
#endif
            if(pEnv->getPixelWidth() != _width ||
               pEnv->getPixelHeight() != _height)
            {
#if 0
                _width = _shadowVP->getPixelWidth();
                _height = _shadowVP->getPixelHeight();
#endif
                _width  = pEnv->getPixelWidth();
                _height = pEnv->getPixelHeight();

                if(_useNPOTTextures)
                {
                    _colorMapImage->set(GL_RGB, _width, _height);

                    _shadowFactorMapImage->set(GL_RGB, _width, _height);

                    _shadowFactorMapImage2->set(GL_RGB, _width, _height);
		
                    reInit(pEnv);
                }
                else
                {
                    if(_width > _height)
                        _widthHeightPOT = osgNextPower2(_width - 1);
                    else
                        _widthHeightPOT = osgNextPower2(_height - 1);
	
                    _colorMapImage->set(GL_RGB, _widthHeightPOT,
                                        _widthHeightPOT);
	
                    _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT,
                                               _widthHeightPOT);

                    _shadowFactorMapImage2->set(GL_RGB, _widthHeightPOT,
                                                _widthHeightPOT);
                }
            }

            //Matrizen fr alle Lichter berechnen
            for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
            {
                if(_shadowVP->_lightStates[i] != 0 &&
                   (_shadowVP->_lights[i].second->getShadowIntensity() != 0.0 ||
                    _shadowVP->getGlobalShadowIntensity() != 0.0))
                {
                    Matrix  _LPM, _LVM;
                    calcPerspectiveSpot(_LPM, _LVM, i, pEnv);
                    _perspectiveLPM.push_back(_LPM);
                    _perspectiveLVM.push_back(_LVM);
					
                }
                else
                {
                    Matrix  _LPM, _LVM;
                    _LPM.setIdentity();
                    _LVM.setIdentity();
                    _perspectiveLPM.push_back(_LPM);
                    _perspectiveLVM.push_back(_LVM);
                }
            }

            if(_shadowVP->getMapAutoUpdate() == true ||
               _shadowVP->_trigger_update    == true  )
            {
                if(_useFBO && _useNPOTTextures)
                {
                    createColorMapFBO(pEnv, pTmpAction);
                }
                else
                {
                    createColorMap(pEnv, pTmpAction);
                }


                //deactivate transparent Nodes
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                {
                    _shadowVP->_transparent[t]->setTravMask(0);
                }


                if(_useFBO)
                {
                    createShadowMapsFBO(pEnv, pTmpAction);
                }
                else
                {
                    createShadowMaps(pEnv, pTmpAction);
                }


                // switch on all transparent geos
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                {
                    _shadowVP->_transparent[t]->setTravMask(
                        TypeTraits<UInt32>::BitsSet);
                }


                if(_useFBO && _useNPOTTextures)
                {
                    createShadowFactorMapFBO(pEnv, pTmpAction);
                }
                else
                {
                    createShadowFactorMap(pEnv, pTmpAction);
                }

                _shadowVP->_trigger_update = false;
            }

            setupDrawCombineMap2(pEnv->getAction());
        }

        glPopAttrib();

#if 0
        // render the foregrounds.
        for(UInt16 i = 0;i < _shadowVP->getMFForegrounds()->size();++i)
        {
            _shadowVP->getForegrounds(i)->draw(pEnv, _shadowVP);
        }
#endif
        _perspectiveLPM.clear();
        _perspectiveLVM.clear();
    }

}
