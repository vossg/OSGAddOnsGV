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
#include <OSGMatrixCameraDecorator.h>
#include <OSGLight.h>
#include <OSGMaterialGroup.h>
#include "OSGStdShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"

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

static std::string _std_shadow_vp =
    "uniform mat4 lightPM;\n"
    "uniform float texFactor;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM * realPos;\n"
    "  projCoord.x *= texFactor;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadow_fp =
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

static std::string _std_shadow2_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadow2_fp =
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

static std::string _std_shadow3_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "uniform float texFactor3;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 projCoord3;\n"
    "varying vec4 texPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  projCoord3.x *= texFactor3;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadow3_fp =
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


static std::string _std_shadow4_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "uniform float texFactor3;\n"
    "uniform float texFactor4;\n"
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
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  projCoord3.x *= texFactor3;\n"
    "  projCoord4.x *= texFactor4;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadow4_fp =
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

static std::string _std_shadow5_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "uniform float texFactor3;\n"
    "uniform float texFactor4;\n"
    "uniform float texFactor5;\n"
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
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord5 = lightPM5 * realPos;\n"
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  projCoord3.x *= texFactor3;\n"
    "  projCoord4.x *= texFactor4;\n"
    "  projCoord5.x *= texFactor5;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  projCoord2 = bias * projCoord2;\n"
    "  projCoord3 = bias * projCoord3;\n"
    "  projCoord4 = bias * projCoord4;\n"
    "  projCoord5 = bias * projCoord5;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadow5_fp =
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

static std::string _std_shadow6_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "uniform mat4 lightPM6;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "uniform float texFactor3;\n"
    "uniform float texFactor4;\n"
    "uniform float texFactor5;\n"
    "uniform float texFactor6;\n"
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
    "void main(void)\n"
    "{\n"
    "  vec4 realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM1 * realPos;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  projCoord3 = lightPM3 * realPos;\n"
    "  projCoord4 = lightPM4 * realPos;\n"
    "  projCoord5 = lightPM5 * realPos;\n"
    "  projCoord6 = lightPM6 * realPos;\n"
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  projCoord3.x *= texFactor3;\n"
    "  projCoord4.x *= texFactor4;\n"
    "  projCoord5.x *= texFactor5;\n"
    "  projCoord6.x *= texFactor6;\n"
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

static std::string _std_shadow6_fp =
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

static std::string _std_shadow7_vp =
    "uniform mat4 lightPM1;\n"
    "uniform mat4 lightPM2;\n"
    "uniform mat4 lightPM3;\n"
    "uniform mat4 lightPM4;\n"
    "uniform mat4 lightPM5;\n"
    "uniform mat4 lightPM6;\n"
    "uniform mat4 lightPM7;\n"
    "uniform float texFactor1;\n"
    "uniform float texFactor2;\n"
    "uniform float texFactor3;\n"
    "uniform float texFactor4;\n"
    "uniform float texFactor5;\n"
    "uniform float texFactor6;\n"
    "uniform float texFactor7;\n"
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
    "  projCoord.x *= texFactor1;\n"
    "  projCoord2.x *= texFactor2;\n"
    "  projCoord3.x *= texFactor3;\n"
    "  projCoord4.x *= texFactor4;\n"
    "  projCoord5.x *= texFactor5;\n"
    "  projCoord6.x *= texFactor6;\n"
    "  projCoord7.x *= texFactor7;\n"
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

static std::string _std_shadow7_fp =
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

static std::string _std_shadowCube_vp =
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
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""void main(void)\n"
    "{\n"
    "  realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  realPos2 = lightPMOP * (KKtoWK * realPos);\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _std_shadowCube_fp =
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

StdShadowMapHandler::StdShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _fb(0),
    _fb2(0),
    _rb_depth(0),
    _transforms(),
    _tiledeco(NULL),
    _blender(NULL),
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
    _firstRun(1),
    _initTexturesDone(false)
{
    _blender = BlendChunk::create();
    {
        _blender->setAlphaFunc(GL_GEQUAL);
        _blender->setAlphaValue(0.99);
    }

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

    _tiledeco = NULL;

    _colorMap = TextureChunk::create();
    _colorMapImage = Image::create();

    _colorMap->setImage(_colorMapImage);
    _colorMap->setInternalFormat(GL_RGB);
    _colorMap->setExternalFormat(GL_RGB);
    _colorMap->setMinFilter(GL_NEAREST);
    _colorMap->setMagFilter(GL_NEAREST);
    _colorMap->setWrapS(GL_REPEAT);
    _colorMap->setWrapT(GL_REPEAT);
    _colorMap->setTarget(GL_TEXTURE_2D);

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

    _shadowFactorMap = TextureChunk::create();
    _shadowFactorMapImage = Image::create();

    _shadowFactorMap->setImage(_shadowFactorMapImage);
    _shadowFactorMap->setInternalFormat(GL_RGB);
    _shadowFactorMap->setExternalFormat(GL_RGB);
    _shadowFactorMap->setMinFilter(GL_LINEAR);
    _shadowFactorMap->setMagFilter(GL_LINEAR);
    _shadowFactorMap->setWrapS(GL_REPEAT);
    _shadowFactorMap->setWrapT(GL_REPEAT);
    _shadowFactorMap->setTarget(GL_TEXTURE_2D);

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

    _shadowFactorMap2 = TextureChunk::create();
    _shadowFactorMapImage2 = Image::create();

    _shadowFactorMap2->setImage(_shadowFactorMapImage2);
    _shadowFactorMap2->setInternalFormat(GL_RGB);
    _shadowFactorMap2->setExternalFormat(GL_RGB);
    _shadowFactorMap2->setMinFilter(GL_LINEAR);
    _shadowFactorMap2->setMagFilter(GL_LINEAR);
    _shadowFactorMap2->setWrapS(GL_REPEAT);
    _shadowFactorMap2->setWrapT(GL_REPEAT);
    _shadowFactorMap2->setTarget(GL_TEXTURE_2D);

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
    //_shadowSHL->readVertexProgram("Std_Shadow.vert");
    //_shadowSHL->readFragmentProgram("Std_Shadow.frag");
    _shadowSHL->setVertexProgram(_std_shadow_vp);
    _shadowSHL->setFragmentProgram(_std_shadow_fp);

    _shadowSHL2 = SHLChunk::create();
    //_shadowSHL2->readVertexProgram("Std_Shadow2.vert");
    //_shadowSHL2->readFragmentProgram("Std_Shadow2.frag");
    _shadowSHL2->setVertexProgram(_std_shadow2_vp);
    _shadowSHL2->setFragmentProgram(_std_shadow2_fp);

    _shadowSHL3 = SHLChunk::create();
    //_shadowSHL3->readVertexProgram("Std_Shadow3.vert");
    //_shadowSHL3->readFragmentProgram("Std_Shadow3.frag");
    _shadowSHL3->setVertexProgram(_std_shadow3_vp);
    _shadowSHL3->setFragmentProgram(_std_shadow3_fp);

    _shadowSHL4 = SHLChunk::create();
    //_shadowSHL4->readVertexProgram("Std_Shadow4.vert");
    //_shadowSHL4->readFragmentProgram("Std_Shadow4.frag");
    _shadowSHL4->setVertexProgram(_std_shadow4_vp);
    _shadowSHL4->setFragmentProgram(_std_shadow4_fp);

    _shadowSHL5 = SHLChunk::create();
    //_shadowSHL5->readVertexProgram("Std_Shadow5.vert");
    //_shadowSHL5->readFragmentProgram("Std_Shadow5.frag");
    _shadowSHL5->setVertexProgram(_std_shadow5_vp);
    _shadowSHL5->setFragmentProgram(_std_shadow5_fp);

    _shadowSHL6 = SHLChunk::create();
    //_shadowSHL6->readVertexProgram("Std_Shadow6.vert");
    //_shadowSHL6->readFragmentProgram("Std_Shadow6.frag");
    _shadowSHL6->setVertexProgram(_std_shadow6_vp);
    _shadowSHL6->setFragmentProgram(_std_shadow6_fp);

    _shadowSHL7 = SHLChunk::create();
    //_shadowSHL7->readVertexProgram("Std_Shadow7.vert");
    //_shadowSHL7->readFragmentProgram("Std_Shadow7.frag");
    _shadowSHL7->setVertexProgram(_std_shadow7_vp);
    _shadowSHL7->setFragmentProgram(_std_shadow7_fp);

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("Std_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("Std_Shadow_combine.frag");
    _combineSHL->setVertexProgram(_shadow_combine_vp);
    _combineSHL->setFragmentProgram(_shadow_combine_fp);

    _combineDepth = DepthChunk::create();
        _combineDepth->setReadOnly(true);

    //SHL Chunk 3
    _shadowCubeSHL = SHLChunk::create();
    //_shadowCubeSHL->readVertexProgram("Std_ShadowCube.vert");
    //_shadowCubeSHL->readFragmentProgram("Std_ShadowCube.frag");
    _shadowCubeSHL->setVertexProgram(_std_shadowCube_vp);
    _shadowCubeSHL->setFragmentProgram(_std_shadowCube_fp);

    _shadowCmat = ChunkMaterial::create();

    //Combine Shader
    _combineCmat = ChunkMaterial::create();
    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMap);
    _combineCmat->addChunk(_shadowFactorMap);
    _combineCmat->addChunk(_combineDepth);

    _transforms[0] = Matrix(1, 0, 0, 0, //andere Richtung als Orig (posZ)
                           0, -1, 0, 0,
                           0, 0, -1, 0,
                           0, 0, 0, 1);

    _transforms[1] = Matrix(1, 0, 0, 0, //original (negZ)
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

StdShadowMapHandler::~StdShadowMapHandler(void)
{
    _tiledeco = NULL;

    _blender          = NULL;
    _colorMap         = NULL;
    _shadowFactorMap  = NULL;
    _shadowFactorMap2 = NULL;
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

#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
    if(_fb != 0)
        glDeleteFramebuffersEXT(1, &_fb);
    if(_rb_depth != 0)
        glDeleteRenderbuffersEXT(1, &_rb_depth);
#endif
    if(_fb2 != 0)
        glDeleteFramebuffersEXT(1, &_fb2);
}

/// Checks if FBO status is ok
bool StdShadowMapHandler::checkFrameBufferStatus(Window *win)
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


bool StdShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    if(_useFBO)
    {
#if 0
        Int32   width  = _shadowVP->getPixelWidth();
        Int32   height = _shadowVP->getPixelHeight();
#endif
        Int32   width  = pEnv->getPixelWidth();
        Int32   height = pEnv->getPixelHeight();

        if(width <= 0 || height <= 0)
            return false;

        if(_fb2 != 0)
            return true;

        Window *win = pEnv->getWindow();
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
        _width = pEnv->getPixelWidth();
        _height = pEnv->getPixelHeight();

        _colorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage2->set(GL_RGB, _width, _height);

        commitChanges();

        glGenFramebuffersEXT(1, &_fb);
        glGenRenderbuffersEXT(1, &_rb_depth);

        win->validateGLObject(_colorMap->getGLId(),
                              pEnv);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
                                  GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_colorMap->getGLId()), 
                                  0);
        win->validateGLObject(_shadowFactorMap->getGLId(),
                             pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_shadowFactorMap->getGLId
                                                     ()), 0);
        win->validateGLObject(_shadowFactorMap2->getGLId(),
                              pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_shadowFactorMap2->getGLId
                                                     ()), 0);

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
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT
                                     , GL_RENDERBUFFER_EXT, _rb_depth);

        win->validateGLObject(_colorMap->getGLId(),
                              pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_colorMap->getGLId()), 0);
        win->validateGLObject(_shadowFactorMap->getGLId(),
                              pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_shadowFactorMap->getGLId
                                                     ()), 0);
        win->validateGLObject(_shadowFactorMap2->getGLId(),
                              pEnv);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_shadowFactorMap2->getGLId
                                                     ()), 0);

        bool    result = checkFrameBufferStatus(win);
#endif

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
        glGenFramebuffersEXT(1, &_fb2);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);

        glDrawBuffer(GL_NONE);	// no color buffer dest
        glReadBuffer(GL_NONE);	// no color buffer src

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    }

    return true;
}

void StdShadowMapHandler::reInit(DrawEnv *pEnv)
{
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
#if 0
    Int32   width = _shadowVP->getPixelWidth();
    Int32   height = _shadowVP->getPixelHeight();
#endif
    Int32   width  = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    Window *win = pEnv->getWindow();

    win->validateGLObject(_colorMap->getGLId(), pEnv);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_colorMap->getGLId()), 0);
    win->validateGLObject(_shadowFactorMap->getGLId(), pEnv);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_shadowFactorMap->getGLId()),
                              0);
    win->validateGLObject(_shadowFactorMap2->getGLId(), pEnv);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_shadowFactorMap2->getGLId()),
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
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                 GL_RENDERBUFFER_EXT
                                 , _rb_depth);
#endif
}

void StdShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;

#if 0
    Int32   width  = _shadowVP->getPixelWidth();
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


void StdShadowMapHandler::createShadowMapsNOGLSL(DrawEnv *pEnv)
{
    OSG_ASSERT(false);

#if 0
    if(_tiledeco == NULL)
    {
        _tiledeco = TileCameraDecorator::create();
    }

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
                _tiledeco->setDecoratee(_shadowVP->_lightCameras[i]);
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

void StdShadowMapHandler::createShadowMaps(DrawEnv      *pEnv,
                                           RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createShadowMaps::start\n");

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

#if 0
    fprintf(stderr, "%p : %d %d %d %d | %d\n",
            pEnv,
            vpTop,
            vpBottom,
            vpLeft,
            vpRight,
            minSize);
#endif

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
                    _tiledeco->setDecoratee(_shadowVP->_lightCameras[i]);
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
                            _shadowVP->renderLight(pEnv->getAction(), 
                                                   _unlitMat, i);
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
                    pVP->setSize(0, 0, 
                                 _mapRenderSize - 1,
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
                                _shadowVP->renderLight(pEnv->getAction(), 
                                                       _unlitMat, i);
#endif

                                _shadowVP->renderLight(pTmpAction, 
                                                       _unlitMat, i);

                                pEnv->getWindow()->validateGLObject(
                                    _shadowVP->_texChunks[i]->getGLId(),
                                    pEnv);

                                glDisable(GL_POLYGON_OFFSET_FILL);

                                //----------Shadow-Texture-Parameters and Indices-------------

                                glBindTexture(GL_TEXTURE_2D,
                                              pEnv->getWindow
                                              ()->getGLObjectId(
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


    fprintf(stderr, "StdShadowMapHandler::createShadowMaps::end\n");
}

void StdShadowMapHandler::createShadowMapsFBO(DrawEnv      *pEnv, 
                                              RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createShadowMapsFBO start\n");

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

                    pVP->setSize(0, 0, _shadowVP->getMapSize(),
                                       _shadowVP->getMapSize());

                    pVP->activate();

#if 0
                    pEnv->getAction()->setCamera(_shadowVP->_lightCameras[i]);
                    _shadowVP->renderLight(pEnv->getAction(), _unlitMat, i);
#endif

                    pTmpAction->setCamera(_shadowVP->_lightCameras[i]);
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

    fprintf(stderr, "StdShadowMapHandler::createShadowMapsFBO end\n");
}

void StdShadowMapHandler::projectShadowMaps(DrawEnv      *pEnv, 
                                            RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::projectShadowMaps start\n");
    
    Matrix                          viewMatrix, projectionMatrix, biasMatrix;
    Vec4f                           s, t, r, q;

    biasMatrix.setIdentity();
    biasMatrix.setScale(0.5);
    biasMatrix.setTranslate(0.5, 0.5, 0.5);

#if 0
    GLint pl = _shadowVP->getPixelLeft(), 
          pr = _shadowVP->getPixelRight(), 
          pb = _shadowVP->getPixelBottom(),
          pt = _shadowVP->getPixelTop();
#endif

    GLint pl = pEnv->getPixelLeft(), 
          pr = pEnv->getPixelRight(), 
          pb = pEnv->getPixelBottom(),
          pt = pEnv->getPixelTop();

    GLint pw = pr - pl + 1, ph = pt - pb + 1;

#if 0
    bool  full = _shadowVP->isFullWindow();
#endif
    bool full = pEnv->getFull();

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
        //_shadowVP->_lights[j].second->setAmbient(0.0,0.0,0.0,1.0);
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
                _shadowVP->_lightCameras[i]->getProjection(projectionMatrix,
                                                           _shadowVP->getMapSize(), _shadowVP->getMapSize());

#if 0
                _shadowVP->_lightCameras[i]->getViewing(viewMatrix,
                                                        _shadowVP->getPixelWidth()/*_mapRenderSize*/,
                                                        _shadowVP->getPixelHeight()/*_mapRenderSize*/);
#endif
                _shadowVP->_lightCameras[i]->getViewing(viewMatrix,
                                                        pEnv->getPixelWidth()/*_mapRenderSize*/,
                                                        pEnv->getPixelHeight()/*_mapRenderSize*/);

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
                textureMatrix.mult(projectionMatrix);
                textureMatrix.mult(viewMatrix);

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

    //// switch on all transparent geos
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

    fprintf(stderr, "StdShadowMapHandler::projectShadowMaps end\n");
}

void StdShadowMapHandler::createColorMap(DrawEnv *pEnv,
                                         RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createColorMap start\n");

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

    pEnv->getWindow()->validateGLObject(_colorMap->getGLId(),
                                        pEnv);

    _shadowVP->setReadBuffer(); // set the right read buffer for the copy texture.
    glBindTexture(GL_TEXTURE_2D,
                  pEnv->getWindow()->getGLObjectId(_colorMap->getGLId()));
    //glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _shadowVP->getPixelWidth(), _shadowVP->getPixelHeight());

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

    fprintf(stderr, "StdShadowMapHandler::createColorMap end\n");
}

void StdShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
                                            RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createColorMapFBO start\n");

    Real32  vpTop, vpBottom, vpLeft, vpRight;
#if 0
    vpTop = _shadowVP->getTop();
    vpBottom = _shadowVP->getBottom();
    vpLeft = _shadowVP->getLeft();
    vpRight = _shadowVP->getRight();
    _shadowVP->setVPSize(0, 0, _shadowVP->getPixelWidth() - 1,
                         _shadowVP->getPixelHeight() - 1);
#endif

    vpTop = pEnv->getPixelTop();
    vpBottom = pEnv->getPixelBottom();
    vpLeft = pEnv->getPixelLeft();
    vpRight = pEnv->getPixelRight();

    Viewport *pVP = pTmpAction->getViewport();

    pVP->setSize(0, 0, 
                 pEnv->getPixelWidth() - 1,
                 pEnv->getPixelHeight() - 1);
    pVP->activate();

    Window  *win = pEnv->getWindow();

    GLenum  *buffers = NULL;
    buffers = new GLenum[1];
    buffers[0] = GL_COLOR_ATTACHMENT0_EXT;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);

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
#if 0
    _shadowVP->checkLightsOcclusion(pEnv->getAction());
#endif
    _shadowVP->checkLightsOcclusion(pTmpAction);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    delete[] buffers;

#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif
    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();

    fprintf(stderr, "StdShadowMapHandler::createColorMapFBO end\n");
}


void StdShadowMapHandler::createShadowFactorMap(DrawEnv      *pEnv,
                                                RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createShadowFactorMap start\n");

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
#if 0
                _shadowVP->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight());
#endif
                pEnv->getAction()->getCamera()->getViewing(
                    CVM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());

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
                _shadowCmat->addChunk(_shadowFactorMap);

#if 0
                _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
                _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

                pEnv->getWindow()->validateGLObject(_shadowFactorMap->getGLId(),
                                                    pEnv);

                glBindTexture(GL_TEXTURE_2D,
                              pEnv->getWindow()->getGLObjectId(
                              _shadowFactorMap->getGLId()));
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
    std::vector<Real32> texFactorF;
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

    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0 &&
           ((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
             _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
            !_shadowVP->_realPointLight[i]))
        {

            Real32  shadowIntensity;
            if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                   activeLights);
            else
                shadowIntensity = (_shadowVP->_lights[i].second->getShadowIntensity
                                   () / activeLights);
            shadowIntensityF.push_back(shadowIntensity);

            Matrix  LVM, LPM, CVM;

#if 0
            _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                    _shadowVP->getPixelWidth(),
                                                    _shadowVP->getPixelHeight
                                                    ());
            _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                       _shadowVP->getPixelWidth
                                                       (),
                                                       _shadowVP->getPixelHeight());
            _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getPixelWidth(),
                                               _shadowVP->getPixelHeight());
#endif
            _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                    pEnv->getPixelWidth(),
                                                    pEnv->getPixelHeight
                                                    ());
            _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                       pEnv->getPixelWidth(),
                                                       pEnv->getPixelHeight());
            pTmpAction->getCamera()->getViewing(CVM, 
                                                pEnv->getPixelWidth(),
                                                pEnv->getPixelHeight());

            Matrix  iCVM = CVM;
            iCVM.invert();

            Real32  texFactor;
            if(_shadowVP->_lights[i].second->getType() == PointLight::getClassType
               () || _shadowVP->_lights[i].second->getType() ==
               SpotLight::getClassType())
                texFactor = Real32(_width) / Real32(_height);
            else
                texFactor = 1.0;

            texFactorF.push_back(texFactor);

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
                _shadowCmat->addChunk(_shadowFactorMap);

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 7)]);
                _shadowSHL->addUniformVariable("texFactor", texFactorF[0 +
                                                (i * 7)]);
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL2->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("texFactor2", texFactorF[1 +
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL3->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("texFactor3", texFactorF[2 +
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL4->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor4", texFactorF[3 +
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL5->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor5", texFactorF[4 +
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL6->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor5", texFactorF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor6", texFactorF[5 +
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
                _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL7->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor5", texFactorF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor6", texFactorF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor7", texFactorF[6 +
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

            pEnv->getWindow()->validateGLObject(_shadowFactorMap->getGLId(),
                                                pEnv);

            glBindTexture(GL_TEXTURE_2D,
                          pEnv->getWindow()->getGLObjectId(
                          _shadowFactorMap->getGLId()));
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

    shadowIntensityF.clear();
    texFactorF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();

    fprintf(stderr, "StdShadowMapHandler::createShadowFactorMap end\n");
}

void StdShadowMapHandler::createShadowFactorMapFBO(DrawEnv *pEnv,
                                                  RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::createShadowFactorMapFBO start\n");

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

    {
        //clear all ShadowFactorMaps
        GLenum  *buffers = NULL;
        buffers = new GLenum[1];
        buffers[0] = GL_COLOR_ATTACHMENT1_EXT;

        //Setup FBO
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
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
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
        glDrawBuffer(*buffers);

        checkFrameBufferStatus(pEnv->getWindow());

        // ACHTUNG der fbo kann nur 0,w,0,h rendern
        // damit es auch mit mehreren viewports klappt ...
        glViewport(0, 0, pw, ph);
        glScissor(0, 0, pw, ph);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);

        delete[] buffers;
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
                if(_activeFactorMap == 0)
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

                GLenum  *buffers = NULL;
                buffers = new GLenum[1];
                if(_activeFactorMap == 0)
                    buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
                else
                    buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

                //Setup FBO
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);

                glDrawBuffer(*buffers);

#if 0
                _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
                _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

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
    std::vector<Real32> texFactorF;
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
        if(_shadowVP->_lightStates[i] != 0 &&
           ((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
             _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
            !_shadowVP->_realPointLight[i]))
        {

            Real32  shadowIntensity;
            if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                   activeLights);
            else
                shadowIntensity = (_shadowVP->_lights[i].second->getShadowIntensity
                                   () / activeLights);
            shadowIntensityF.push_back(shadowIntensity);

            Matrix  LVM, LPM, CVM;
#if 0
            _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                    _shadowVP->getPixelWidth(),
                                                    _shadowVP->getPixelHeight
                                                    ());
            _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                       _shadowVP->getPixelWidth
                                                       (),
                                                       _shadowVP->getPixelHeight());
            _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getPixelWidth(),
                                               _shadowVP->getPixelHeight());
#endif
            _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                    pEnv->getPixelWidth(),
                                                    pEnv->getPixelHeight
                                                    ());
            _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                       pEnv->getPixelWidth
                                                       (),
                                                       pEnv->getPixelHeight());
            pTmpAction->getCamera()->getViewing(CVM, 
                                               pEnv->getPixelWidth(),
                                               pEnv->getPixelHeight());
            Matrix  iCVM = CVM;
            iCVM.invert();

            Real32  texFactor;
            if(_shadowVP->_lights[i].second->getType() == PointLight::getClassType
               () || _shadowVP->_lights[i].second->getType() ==
               SpotLight::getClassType())
                texFactor = Real32(_width) / Real32(_height);
            else
                texFactor = 1.0;

            texFactorF.push_back(texFactor);

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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 7)]);
                _shadowSHL->addUniformVariable("texFactor", texFactorF[0 +
                                                (i * 7)]);
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL2->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("texFactor2", texFactorF[1 +
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL3->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("texFactor3", texFactorF[2 +
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL4->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("texFactor4", texFactorF[3 +
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL5->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("texFactor5", texFactorF[4 +
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL6->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor5", texFactorF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("texFactor6", texFactorF[5 +
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
                    _shadowCmat->addChunk(_shadowFactorMap2);
                else
                    _shadowCmat->addChunk(_shadowFactorMap);

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
                _shadowSHL7->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor5", texFactorF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor6", texFactorF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("texFactor7", texFactorF[6 +
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


            //Setup FBO
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
            glDrawBuffer(*buffers);

#if 0
            _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, i);
#endif
            _shadowVP->renderLight(pTmpAction, _shadowCmat, i);

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

            _firstRun = 0;
            if(_activeFactorMap == 0)
                _activeFactorMap = 1;
            else
                _activeFactorMap = 0;
        }

        delete[] buffers;
    }
    shadowIntensityF.clear();
    texFactorF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();

#if 0
    _shadowVP->setVPSize(vpLeft, vpBottom, vpRight, vpTop);
#endif
    pVP->setSize(vpLeft, vpBottom, vpRight, vpTop);
    pVP->activate();

    fprintf(stderr, "StdShadowMapHandler::createShadowFactorMapFBO end\n");
}


void StdShadowMapHandler::render(DrawEnv      *pEnv, 
                                 RenderAction *pTmpAction)
{
    fprintf(stderr, "StdShadowMapHandler::render start\n");

    Window           *win     = pEnv->getWindow();
    RenderActionBase *pAction = pEnv->getAction();

    initialize(win);

    if(!_useShadowExt)
    {
#if 0
        _shadowVP->Viewport::render(pAction);
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
                          win->getGLObjectId(
                          _shadowVP->_texChunks[i]->getGLId()));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                            GL_COMPARE_R_TO_TEXTURE_ARB);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,
                            GL_LEQUAL);
            glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB,
                            GL_LUMINANCE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if((_shadowVP->getMapSize() / 4) > _maxPLMapSize)
            _PLMapSize = _maxPLMapSize;
        else
            _PLMapSize = _shadowVP->getMapSize() / 4;

        if(!_useGLSL)
        {

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

            if(!_shadowVP->_lights.empty() || !_shadowVP->_lightCameras.empty
               ())
            {
                projectShadowMaps(pEnv, pTmpAction);
            }
            else
            {
                FDEBUG(("Rendering without Shadows\n"));
#if 0
                _shadowVP->Viewport::render(pAction);
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
                _width = pEnv->getPixelWidth();
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


            if(_shadowVP->getMapAutoUpdate())
            {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                if(_useFBO && _useNPOTTextures)
                    createColorMapFBO(pEnv, pTmpAction);
                else
#endif
                    createColorMap(pEnv, pTmpAction);

                //deactivate transparent Nodes
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                    _shadowVP->_transparent[t]->setTravMask(0);

                if(_useFBO)
                    createShadowMapsFBO(pEnv, pTmpAction);
                else
                    createShadowMaps(pEnv, pTmpAction);

                // switch on all transparent geos
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                    _shadowVP->_transparent[t]->setTravMask(TypeTraits<UInt32>::BitsSet);

#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                if(_useFBO && _useNPOTTextures)
                    createShadowFactorMapFBO(pEnv, pTmpAction);
                else
#endif
                    createShadowFactorMap(pEnv, pTmpAction);
            }
            else
            {
                if(_shadowVP->_trigger_update)
                {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                    if(_useFBO && _useNPOTTextures)
                        createColorMapFBO(pEnv, pTmpAction);
                    else
#endif
                        createColorMap(pEnv, pTmpAction);

                    //deactivate transparent Nodes
                    for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                        _shadowVP->_transparent[t]->setTravMask(0);

                    if(_useFBO)
                        createShadowMapsFBO(pEnv, pTmpAction);
                    else
                        createShadowMaps(pEnv, pTmpAction);

                    // switch on all transparent geos
                    for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                        _shadowVP->_transparent[t]->setTravMask(TypeTraits<UInt32>::BitsSet);

#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                    if(_useFBO && _useNPOTTextures)
                        createShadowFactorMapFBO(pEnv, pTmpAction);
                    else
#endif
                        createShadowFactorMap(pEnv, pTmpAction);
                    _shadowVP->_trigger_update = false;
                }
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
    }

    fprintf(stderr, "StdShadowMapHandler::render end\n");
}

