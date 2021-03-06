
#define DEVELOPMENT
#include "EncridGLSL.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glew32.lib")

using namespace std;

BOOL IsUsingGLSL = false;
BOOL extensions_init = false;
BOOL bGeometryShader = false;
BOOL bGPUShader4 = false;

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Error, Warning and Info Strings
CHAR* aGLSLStrings[] = {
        "[e00] GLSL is not available!",
        "[e01] Not a valid program object!",
        "[e02] Not a valid object!",
        "[e03] Out of memory!",
        "[e04] Unknown compiler error!",
        "[e05] Linker log is not available!",
        "[e06] Compiler log is not available!",
        "[Empty]"
        };
//-----------------------------------------------------------------------------      
 
// GL ERROR CHECK
INT CheckGLError(CHAR *file, INT line)
{
	   GLenum glErr;
	   INT    retCode = 0;

	   glErr = glGetError();
	   while (glErr != GL_NO_ERROR) 
       {
	     const GLubyte* sError = gluErrorString(glErr);

		 if (sError)
			cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << endl;
		 else
		    cout << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << endl;
			
		 retCode = 1;
		 glErr = glGetError();
	   }
	   return retCode;
}
   
#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)

//----------------------------------------------------------------------------- 
extern "C" ENCRID_API BOOL TDE_InitOpenGLExtensions(VOID)
{
      if (extensions_init) return true;
      extensions_init = true;

      GLenum err = glewInit();

      if (GLEW_OK != err)
      {
         cout << "Error:" << glewGetErrorString(err) << endl;
         extensions_init = false;
         return false;
      }

      cout << "OpenGL Vendor: " << (CHAR*) glGetString(GL_VENDOR) << "\n";
      cout << "OpenGL Renderer: " << (CHAR*) glGetString(GL_RENDERER) << "\n";
      cout << "OpenGL Version: " << (CHAR*) glGetString(GL_VERSION) << "\n\n";
      //cout << "OpenGL Extensions:\n" << (CHAR*) glGetString(GL_EXTENSIONS) << "\n\n";

      TDE_HasGLSLSupport();

      return true;
}


extern "C" ENCRID_API BOOL TDE_HasGLSLSupport(VOID)
{
      bGeometryShader = TDE_HasGeometryShaderSupport();
      bGPUShader4     = TDE_HasShaderModel4();
      
      if (IsUsingGLSL) return true;  // already initialized and GLSL is available
      IsUsingGLSL = true;
     
      if (!extensions_init) TDE_InitOpenGLExtensions();  // extensions were not yet initialized!!

      
      if (GLEW_VERSION_2_0)
      {
         cout << "OpenGL 2.0 (or higher) is available!" << endl;
      }
      else if (GLEW_VERSION_1_5)
      {
         cout << "OpenGL 1.5 core functions are available" << endl;
      }
      else if (GLEW_VERSION_1_4)
      {
         cout << "OpenGL 1.4 core functions are available" << endl;
      }
      else if (GLEW_VERSION_1_3)
      {
         cout << "OpenGL 1.3 core functions are available" << endl;
      }
      else if (GLEW_VERSION_1_2)
      {
         cout << "OpenGL 1.2 core functions are available" << endl;
      }

      if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
      {
          cout << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
          IsUsingGLSL = false;
      }

      if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
      {
          cout << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
          IsUsingGLSL = false;
      }

      if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
      {
          cout << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
          IsUsingGLSL = false;
      }

      if (IsUsingGLSL)
      {
          cout << "[OK] OpenGL Shading Language is available!\n\n";
      }
      else
      {
          cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
      }   
     
      return IsUsingGLSL;
}


extern "C" ENCRID_API BOOL TDE_HasOpenGL2Support(VOID)
{
      if (!extensions_init) TDE_InitOpenGLExtensions();

      return (GLEW_VERSION_2_0 == GL_TRUE);
}   


extern "C" ENCRID_API BOOL TDE_HasGeometryShaderSupport(VOID)
{
      if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
         return false;
      
      return true;
}

extern "C" ENCRID_API BOOL TDE_HasShaderModel4(VOID)
{
      if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
         return false;

      return true;
}


//----------------------------------------------------------------------------- 

// ************************************************************************
// Implementation of TDE_GLSL class
// ************************************************************************
 
TDE_GLSL::TDE_GLSL()
{
  TDE_InitOpenGLExtensions();
  ProgramObject = 0;
  linker_log = 0;
  is_linked = false; 
  _mM = false;
  _noshader = true;

  if (!IsUsingGLSL)
  {
     cout << "**ERROR: OpenGL Shading Language is NOT available!" << endl;
  }
  else
  {
      ProgramObject = glCreateProgram();
  }
      
}

//----------------------------------------------------------------------------- 

TDE_GLSL::~TDE_GLSL()
{
    if (linker_log!=0) free(linker_log);
    if (IsUsingGLSL)
    {
       for (unsigned int i=0;i<ShaderList.size();i++)
       {
            glDetachShader(ProgramObject, ShaderList[i]->ShaderObject);
            CHECK_GL_ERROR(); // if you get an error here, you deleted the Program object first and then
                           // the ShaderObject! Always delete ShaderObjects last!
            if (_mM) delete ShaderList[i]; 
       }                      

       glDeleteShader(ProgramObject);
       CHECK_GL_ERROR();
    }

}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::addShader(TDE_GLSLObject* ShaderProgram)
{
if (!IsUsingGLSL) return;

   if (ShaderProgram==0) return;

   
   if (!ShaderProgram->is_compiled)
   {
        cout << "**warning** please compile program before adding object! trying to compile now...\n";
        if (!ShaderProgram->compile())
        {
            cout << "...compile ERROR!\n";
            return;
        }
        else
        {   
            cout << "...ok!\n";
        }
   }

   ShaderList.push_back(ShaderProgram); 
   
}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::SetInputPrimitiveType(INT nInputPrimitiveType)
{
   _nInputPrimitiveType = nInputPrimitiveType;
}

VOID TDE_GLSL::SetOutputPrimitiveType(INT nOutputPrimitiveType)
{
   _nOutputPrimitiveType = nOutputPrimitiveType;
}

VOID TDE_GLSL::SetVerticesOut(INT nVerticesOut)
{
   _nVerticesOut = nVerticesOut;
}
//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::link(VOID)
{
if (!IsUsingGLSL) return false;

unsigned int i;

    if (_bUsesGeometryShader)
    {
       glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_INPUT_TYPE_EXT, _nInputPrimitiveType);
       glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_OUTPUT_TYPE_EXT, _nOutputPrimitiveType);
       glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_VERTICES_OUT_EXT, _nVerticesOut);
    }

    if (is_linked)  // already linked, detach everything first
    {
       cout << "**warning** Object is already linked, trying to link again" << endl;
       for (i=0;i<ShaderList.size();i++)
       {
            glDetachShader(ProgramObject, ShaderList[i]->ShaderObject);
            CHECK_GL_ERROR();
       }
    }
    
    for (i=0;i<ShaderList.size();i++)
    {
        glAttachShader(ProgramObject, ShaderList[i]->ShaderObject);
        CHECK_GL_ERROR();
        //cout << "attaching ProgramObj [" << i << "] @ 0x" << hex << ShaderList[i]->ProgramObject << " in ShaderObj @ 0x"  << ShaderObject << endl;
    }
    
    GLint linked; // bugfix Oct-06-2006
    glLinkProgram(ProgramObject);
    CHECK_GL_ERROR();
    glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
    CHECK_GL_ERROR();

    if (linked)
    {
        is_linked = true;
        return true;
    }
    else
    {
        cout << "**linker error**\n";
    }

return false;
}

//----------------------------------------------------------------------------- 
// Compiler Log: Ausgabe der Compiler Meldungen in String

CHAR* TDE_GLSL::getLinkerLog(VOID)
{    
if (!IsUsingGLSL) return aGLSLStrings[0];
 
 GLint blen = 0;    // bugfix Oct-06-2006	
 GLsizei slen = 0;  // bugfix Oct-06-2006

 if (ProgramObject==0) return aGLSLStrings[2];
 glGetProgramiv(ProgramObject, GL_INFO_LOG_LENGTH , &blen);
 CHECK_GL_ERROR();

 if (blen > 1)
 {
    if (linker_log!=0) 
    {   
        free(linker_log);
        linker_log =0;
    }
    if ((linker_log = (GLcharARB*)malloc(blen)) == NULL) 
     {
        printf("ERROR: Could not allocate compiler_log buffer\n");
        return aGLSLStrings[3];
    }

    glGetProgramInfoLog(ProgramObject, blen, &slen, linker_log);
    CHECK_GL_ERROR();
    
 }
 if (linker_log!=0)
    return (CHAR*) linker_log;    
 else
    return aGLSLStrings[5];
   
 return aGLSLStrings[4];
}

VOID TDE_GLSL::begin(VOID)
{
if (!IsUsingGLSL) return;
if (ProgramObject == 0) return;
if (!_noshader) return;

    if (is_linked)
    {
        glUseProgram(ProgramObject);
        CHECK_GL_ERROR();
    }
}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::end(VOID)
{
if (!IsUsingGLSL) return;
if (!_noshader) return;


    glUseProgram(0);
    CHECK_GL_ERROR();
}

//----------------------------------------------------------------------------- 

//#define TDE_GLSLSetUniform1f(name, v0, index) glUniform1f(GetUniformLocation(name), v0)

BOOL TDE_GLSL::setUniform1f(GLcharARB* varname, GLfloat v0, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index

    
    glUniform1f(loc, v0);
    
    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform2f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLint index)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2f(loc, v0, v1);
    
    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3f(loc, v0, v1, v2);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform4f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4f(loc, v0, v1, v2, v3);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform1i(GLcharARB* varname, GLint v0, GLint index)
{ 
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform1i(loc, v0);
    
    return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setUniform2i(GLcharARB* varname, GLint v0, GLint v1, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2i(loc, v0, v1);


    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3i(loc, v0, v1, v2);

    return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setUniform4i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4i(loc, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform1ui(GLcharARB* varname, GLuint v0, GLint index)
{ 
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform1uiEXT(loc, v0);
    
    return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setUniform2ui(GLcharARB* varname, GLuint v0, GLuint v1, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2uiEXT(loc, v0, v1);


    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3uiEXT(loc, v0, v1, v2);

    return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setUniform4ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4uiEXT(loc, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setUniform1fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform1fv(loc, count, value);

    return true;
}
BOOL TDE_GLSL::setUniform2fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform4fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform1iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform1iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform2iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform4iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform1uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform1uivEXT(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform2uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform2uivEXT(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform3uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform3uivEXT(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniform4uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!bGPUShader4) return false;
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniform4uivEXT(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniformMatrix2fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniformMatrix2fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniformMatrix3fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniformMatrix3fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

BOOL TDE_GLSL::setUniformMatrix4fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!IsUsingGLSL) return false; // GLSL not available
    if (!_noshader) return true;
    
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return false;  // can't find variable / invalid index
    
    glUniformMatrix4fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

GLint TDE_GLSL::GetUniformLocation(const GLcharARB *name)
{
	GLint loc;

	loc = glGetUniformLocation(ProgramObject, name);
	if (loc == -1) 
	{
        cout << "Error: can't find uniform variable \"" << name << "\"\n";
	}
    CHECK_GL_ERROR();
	return loc;
}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::getUniformfv(GLcharARB* varname, GLfloat* values, GLint index)
{
if (!IsUsingGLSL) return;
 
    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return;  // can't find variable / invalid index

	glGetUniformfv(ProgramObject, loc, values);
	
}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::getUniformiv(GLcharARB* varname, GLint* values, GLint index)
{
    if (!IsUsingGLSL) return;

    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return;  // can't find variable / invalid index
	
	glGetUniformiv(ProgramObject, loc, values);

}

//----------------------------------------------------------------------------- 

VOID TDE_GLSL::getUniformuiv(GLcharARB* varname, GLuint* values, GLint index)
{
    if (!IsUsingGLSL) return;

    GLint loc;
    if (varname)
       loc = GetUniformLocation(varname);
    else
       loc = index;

    if (loc==-1) 
       return;  // can't find variable / invalid index
	
	glGetUniformuivEXT(ProgramObject, loc, values);

}

//-----------------------------------------------------------------------------
VOID  TDE_GLSL::BindAttribLocation(GLint index, GLchar* name)
{
   glBindAttribLocation(ProgramObject, index, name);
}

//-----------------------------------------------------------------------------


BOOL TDE_GLSL::setVertexAttrib1f(GLuint index, GLfloat v0)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib1f(index, v0);

   return true;
}

BOOL TDE_GLSL::setVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib2f(index, v0, v1);
   
   return true;
}

BOOL TDE_GLSL::setVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;
   
    glVertexAttrib3f(index, v0, v1, v2);
    
    return true;
}

BOOL TDE_GLSL::setVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;
   
   glVertexAttrib4f(index, v0, v1, v2, v3);
   
   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib1d(GLuint index, GLdouble v0)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib1d(index, v0);

   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib2d(index, v0, v1);

   return true;
}

//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib3d(index, v0, v1, v2);

   return true;
}
//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib4d(index, v0, v1, v2, v3);

   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib1s(GLuint index, GLshort v0)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib1s(index, v0);

   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib2s(index, v0, v1);

   return true;
}

//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib3s(index, v0, v1, v2);

   return true;
}
//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib4s(index, v0, v1, v2, v3);

   return true;
}
//----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!_noshader) return true;

   glVertexAttrib4Nub(index, v0, v1, v2, v3);

   return true;
}
//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib1i(GLuint index, GLint v0)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI1iEXT(index, v0);

   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib2i(GLuint index, GLint v0, GLint v1)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI2iEXT(index, v0, v1);

   return true;
}

//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI3iEXT(index, v0, v1, v2);

   return true;
}
//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI4iEXT(index, v0, v1, v2, v3);

   return true;
}
//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib1ui(GLuint index, GLuint v0)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI1uiEXT(index, v0);

   return true;
}

//-----------------------------------------------------------------------------

BOOL TDE_GLSL::setVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI2uiEXT(index, v0, v1);

   return true;
}

//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI3uiEXT(index, v0, v1, v2);

   return true;
}
//-----------------------------------------------------------------------------
BOOL TDE_GLSL::setVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
   if (!IsUsingGLSL) return false; // GLSL not available
   if (!bGPUShader4) return false;
   if (!_noshader) return true;

   glVertexAttribI4uiEXT(index, v0, v1, v2, v3);

   return true;
}
//-----------------------------------------------------------------------------
// ************************************************************************
// Shader Program : Manage Shader Programs (Vertex/Fragment)
// ************************************************************************
TDE_GLSLObject::TDE_GLSLObject()
{
    TDE_InitOpenGLExtensions();
    compiler_log = 0;
    is_compiled = false;
    program_type = 0;
    ShaderObject = 0;
    ShaderSource = 0;
    _memalloc = false;
}

//----------------------------------------------------------------------------- 
TDE_GLSLObject::~TDE_GLSLObject()
{
   if (compiler_log!=0) free(compiler_log);
   if (ShaderSource!=0)   
   {
        if (_memalloc)
            delete[] ShaderSource;  // free ASCII Source
   }
   
   if (is_compiled)
   { 
        glDeleteObjectARB(ShaderObject);
        CHECK_GL_ERROR();
   }
}

//----------------------------------------------------------------------------- 
unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}


//----------------------------------------------------------------------------- 
INT TDE_GLSLObject::load(CHAR* filename)
{
   ifstream file;
	file.open(filename, ios::in);
   if(!file) return -1;
    
   unsigned long len = getFileLength(file);
    
   if (len==0) return -2;   // "Empty File" 
    
   if (ShaderSource!=0)    // there is already a source loaded, free it!
   {
      if (_memalloc)
      delete[] ShaderSource;
   }
    
   ShaderSource = (GLubyte*) new CHAR[len+1];
   if (ShaderSource == 0) return -3;   // can't reserve memory
   _memalloc = true;
     
   ShaderSource[len] = 0;  // len isn't always strlen cause some CHARacters are stripped in ascii read...
                           // it is important to 0-terminate the real length later, len is just max possible value...
   unsigned int i=0;
   while (file.good())
   {
       ShaderSource[i] = file.get();       // get CHARacter from file.
       if (!file.eof())
        i++;
   }
    
   ShaderSource[i] = 0;  // 0 terminate it.
    
   file.close();
      
   return 0;
}

//----------------------------------------------------------------------------- 
VOID TDE_GLSLObject::loadFromMemory(const CHAR* program)
{
    if (ShaderSource!=0)    // there is already a source loaded, free it!
    {
        if (_memalloc)
        delete[] ShaderSource;
    }
   _memalloc = false;
   ShaderSource = (GLubyte*) program;
      
}


// ----------------------------------------------------------------------------
// Compiler Log: Ausgabe der Compiler Meldungen in String
CHAR* TDE_GLSLObject::getCompilerLog(VOID)
{    
if (!IsUsingGLSL) return aGLSLStrings[0];
 
 GLint blen = 0;	
 GLsizei slen = 0;	


 if (ShaderObject==0) return aGLSLStrings[1]; // not a valid program object

 glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH , &blen);
 CHECK_GL_ERROR();

 if (blen > 1)
 {
    if (compiler_log!=0) 
    {   
        free(compiler_log);
        compiler_log =0;
    }
    if ((compiler_log = (GLcharARB*)malloc(blen)) == NULL) 
     {
        printf("ERROR: Could not allocate compiler_log buffer\n");
        return aGLSLStrings[3];
    }

     glGetInfoLogARB(ShaderObject, blen, &slen, compiler_log);
     CHECK_GL_ERROR();
     //cout << "compiler_log: \n", compiler_log);     
 }
 if (compiler_log!=0)
    return (CHAR*) compiler_log;
 else
   return aGLSLStrings[6];       

 return aGLSLStrings[4];
}

// ----------------------------------------------------------------------------
BOOL TDE_GLSLObject::compile(VOID)
{
if (!IsUsingGLSL) return false;

is_compiled = false;

GLint compiled = 0;

  if (ShaderSource==0) return false;

  GLint	length = (GLint) strlen((const CHAR*)ShaderSource);
  glShaderSourceARB(ShaderObject, 1, (const GLcharARB **)&ShaderSource, &length);
  CHECK_GL_ERROR();

  glCompileShaderARB(ShaderObject); 
  CHECK_GL_ERROR();
  glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
  CHECK_GL_ERROR();

  if (compiled) is_compiled=true;
 
return is_compiled;
}

// ----------------------------------------------------------------------------
GLint TDE_GLSLObject::getAttribLocation(CHAR* attribName)
{
   return glGetAttribLocationARB(ShaderObject, attribName);
}

// ----------------------------------------------------------------------------
TDE_VertexShader::TDE_VertexShader()
{
  program_type = 1; 
   if (IsUsingGLSL)
   {
       ShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
       CHECK_GL_ERROR();
   }
}

// ----------------------------------------------------
TDE_VertexShader::~TDE_VertexShader()
{
}
// ----------------------------------------------------

TDE_FragmentShader::TDE_FragmentShader()
{
    program_type = 2;
    if (IsUsingGLSL)
    {
        ShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB); 
        CHECK_GL_ERROR();
    }
}

// ----------------------------------------------------

TDE_FragmentShader::~TDE_FragmentShader()
{
}


// ----------------------------------------------------

TDE_GeometryShader::TDE_GeometryShader()
{
  program_type = 3; 
   if (IsUsingGLSL && bGeometryShader)
   {
       ShaderObject = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_EXT);
       CHECK_GL_ERROR();
   }
}

// ----------------------------------------------------

TDE_GeometryShader::~TDE_GeometryShader()
{
}

// ----------------------------------------------------------------------------
// ShaderManager: Easy use of (multiple) Shaders

TDE_GLSLManager::TDE_GLSLManager()
{
   TDE_InitOpenGLExtensions();
   _nInputPrimitiveType = GL_TRIANGLES;
   _nOutputPrimitiveType = GL_TRIANGLE_STRIP;
   _nVerticesOut = 3;
   
}

TDE_GLSLManager::~TDE_GLSLManager()
{
   // free objects
   vector<TDE_GLSL*>::iterator  i=_shaderObjectList.begin();
   while (i!=_shaderObjectList.end()) 
   {
        //TDE_GLSL* o = *i;
        i=_shaderObjectList.erase(i);
        //delete o;
   }
}

// ----------------------------------------------------------------------------

VOID TDE_GLSLManager::SetInputPrimitiveType(INT nInputPrimitiveType)
{
      _nInputPrimitiveType = nInputPrimitiveType;

}

VOID TDE_GLSLManager::SetOutputPrimitiveType(INT nOutputPrimitiveType)
{
      _nOutputPrimitiveType = nOutputPrimitiveType;

}

VOID TDE_GLSLManager::SetVerticesOut(INT nVerticesOut)
{
   _nVerticesOut = nVerticesOut;
}

// ----------------------------------------------------------------------------
TDE_GLSL* TDE_GLSLManager::loadfromFile(CHAR* vertexFile, CHAR* fragmentFile) 
{
   TDE_GLSL* o = new TDE_GLSL();
   o->UsesGeometryShader(false);
  
   TDE_VertexShader* tVertexShader = new TDE_VertexShader;
   TDE_FragmentShader* tFragmentShader = new TDE_FragmentShader;

    // load vertex program
   if (vertexFile!=0)
   if (tVertexShader->load(vertexFile) != 0)
   { 
     cout << "error: can't load vertex shader!\n"; 
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
   }

  
  // Load fragment program
  if (fragmentFile!=0)
  if (tFragmentShader->load(fragmentFile) != 0)
  {
     cout << "error: can't load fragment shader!\n";
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }
    
  // Compile vertex program
  if (vertexFile!=0)
  if (!tVertexShader->compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->getCompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      return 0;
  }
  cout << "***GLSL Compiler Log (Vertex Shader):\n";
  cout << tVertexShader->getCompilerLog() << "\n";
   
  // Compile fragment program 
  if (fragmentFile!=0)
  if (!tFragmentShader->compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->getCompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
     
  }
  cout << "***GLSL Compiler Log (Fragment Shader):\n";
  cout << tFragmentShader->getCompilerLog() << "\n";
   
  // Add to object    
  if (vertexFile!=0) o->addShader(tVertexShader);
  if (fragmentFile!=0) o->addShader(tFragmentShader); 
  
  // link 
  if (!o->link())
  {
     cout << "**LINKER ERROR\n";
     cout << o->getLinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }
  cout << "***GLSL Linker Log:\n";
  cout << o->getLinkerLog() << endl;
  
  _shaderObjectList.push_back(o);
  o->manageMemory();

   return o;
}


TDE_GLSL* TDE_GLSLManager::loadfromFile(CHAR* vertexFile, CHAR* geometryFile, CHAR* fragmentFile)
{
   TDE_GLSL* o = new TDE_GLSL();
   o->UsesGeometryShader(true);
   o->SetInputPrimitiveType(_nInputPrimitiveType);
   o->SetOutputPrimitiveType(_nOutputPrimitiveType);
   o->SetVerticesOut(_nVerticesOut);
  
   TDE_VertexShader* tVertexShader = new TDE_VertexShader;
   TDE_FragmentShader* tFragmentShader = new TDE_FragmentShader;
   TDE_GeometryShader* tGeometryShader = new TDE_GeometryShader;

    // load vertex program
   if (vertexFile!=0)
      if (tVertexShader->load(vertexFile) != 0)
      { 
        cout << "error: can't load vertex shader!\n"; 
        delete o;
        delete tVertexShader;
        delete tFragmentShader;
        delete tGeometryShader;
        return 0;
      }

  // Load geometry program
  if (geometryFile!=0)
     if (tGeometryShader->load(geometryFile) != 0)
     {
        cout << "error: can't load geometry shader!\n";
        delete o;
        delete tVertexShader;
        delete tFragmentShader;
        delete tGeometryShader;
        return 0;
     }
  
  // Load fragment program
  if (fragmentFile!=0)
     if (tFragmentShader->load(fragmentFile) != 0)
     {
        cout << "error: can't load fragment shader!\n";
        delete o;
        delete tVertexShader;
        delete tFragmentShader;
        delete tGeometryShader;
        return 0;
     }
    
  // Compile vertex program
  if (vertexFile!=0)
     if (!tVertexShader->compile())
     {
         cout << "***COMPILER ERROR (Vertex Shader):\n";
         cout << tVertexShader->getCompilerLog() << endl;
         delete o;
         delete tVertexShader;
         delete tFragmentShader;
         delete tGeometryShader;
         return 0;
     }
  cout << "***GLSL Compiler Log (Vertex Shader):\n";
  cout << tVertexShader->getCompilerLog() << "\n";

  // Compile geometry program 
  if (geometryFile!=0)
  {
     
     if (!tGeometryShader->compile())
     {
        cout << "***COMPILER ERROR (Geometry Shader):\n";
        cout << tGeometryShader->getCompilerLog() << endl;
        
        delete o;
        delete tVertexShader;
        delete tFragmentShader;
        delete tGeometryShader;
        return 0;
        
     }
  }
  cout << "***GLSL Compiler Log (Geometry Shader):\n";
  cout << tGeometryShader->getCompilerLog() << "\n";
   
  // Compile fragment program 
  if (fragmentFile!=0)
     if (!tFragmentShader->compile())
     {
        cout << "***COMPILER ERROR (Fragment Shader):\n";
        cout << tFragmentShader->getCompilerLog() << endl;
        
        delete o;
        delete tVertexShader;
        delete tFragmentShader;
        delete tGeometryShader;
        return 0;
        
     }
  cout << "***GLSL Compiler Log (Fragment Shader):\n";
  cout << tFragmentShader->getCompilerLog() << "\n";
   
  // Add to object    
  if (vertexFile!=0) o->addShader(tVertexShader);
  if (geometryFile!=0) o->addShader(tGeometryShader);
  if (fragmentFile!=0) o->addShader(tFragmentShader); 
  
  // link 
  if (!o->link())
  {
     cout << "**LINKER ERROR\n";
     cout << o->getLinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     delete tGeometryShader;
     return 0;
  }
  cout << "***GLSL Linker Log:\n";
  cout << o->getLinkerLog() << endl;
  
  _shaderObjectList.push_back(o);
  o->manageMemory();

   return o;
   return 0;
}
// ----------------------------------------------------------------------------

TDE_GLSL* TDE_GLSLManager::loadfromMemory(const CHAR* vertexMem, const CHAR* fragmentMem)
{
  TDE_GLSL* o = new TDE_GLSL();
  o->UsesGeometryShader(false);
  
  TDE_VertexShader* tVertexShader = new TDE_VertexShader;
  TDE_FragmentShader* tFragmentShader = new TDE_FragmentShader;

  // get vertex program
  if (vertexMem!=0)
     tVertexShader->loadFromMemory(vertexMem);
  
  // get fragment program
  if (fragmentMem!=0)
     tFragmentShader->loadFromMemory(fragmentMem);
 
  // Compile vertex program
  if (vertexMem!=0)
  if (!tVertexShader->compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->getCompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      return 0;
  }
  cout << "***GLSL Compiler Log (Vertex Shader):\n";
  cout << tVertexShader->getCompilerLog() << "\n";
   
  // Compile fragment program 
  if (fragmentMem!=0)
  if (!tFragmentShader->compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->getCompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;   
  }
  cout << "***GLSL Compiler Log (Fragment Shader):\n";
  cout << tFragmentShader->getCompilerLog() << "\n";
   
  // Add to object    
  if (vertexMem!=0) o->addShader(tVertexShader);
  if (fragmentMem!=0) o->addShader(tFragmentShader); 
  
  // link 
  if (!o->link())
  {
     cout << "**LINKER ERROR\n";
     cout << o->getLinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     return 0;
  }
  cout << "***GLSL Linker Log:\n";
  cout << o->getLinkerLog() << endl;

  _shaderObjectList.push_back(o);
  o->manageMemory();

  return o;
}

TDE_GLSL* TDE_GLSLManager::loadfromMemory(const CHAR* vertexMem, const CHAR* geometryMem, const CHAR* fragmentMem)
{
  TDE_GLSL* o = new TDE_GLSL();
  o->UsesGeometryShader(true);
  o->SetInputPrimitiveType(_nInputPrimitiveType);
  o->SetOutputPrimitiveType(_nOutputPrimitiveType);
  o->SetVerticesOut(_nVerticesOut);
  
  TDE_VertexShader* tVertexShader = new TDE_VertexShader;
  TDE_FragmentShader* tFragmentShader = new TDE_FragmentShader;
  TDE_GeometryShader* tGeometryShader = new TDE_GeometryShader;

  // get vertex program
  if (vertexMem!=0)
     tVertexShader->loadFromMemory(vertexMem);
  
  // get fragment program
  if (fragmentMem!=0)
     tFragmentShader->loadFromMemory(fragmentMem);

    // get fragment program
  if (geometryMem!=0)
     tGeometryShader->loadFromMemory(geometryMem);
 
  // Compile vertex program
  if (vertexMem!=0)
  if (!tVertexShader->compile())
  {
      cout << "***COMPILER ERROR (Vertex Shader):\n";
      cout << tVertexShader->getCompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      delete tGeometryShader;
      return 0;
  }
  cout << "***GLSL Compiler Log (Vertex Shader):\n";
  cout << tVertexShader->getCompilerLog() << "\n";

  // Compile geometry program
  if (geometryMem!=0)
  if (!tGeometryShader->compile())
  {
      cout << "***COMPILER ERROR (Geometry Shader):\n";
      cout << tGeometryShader->getCompilerLog() << endl;
      delete o;
      delete tVertexShader;
      delete tFragmentShader;
      delete tGeometryShader;
      return 0;
  }
  cout << "***GLSL Compiler Log (Geometry Shader):\n";
  cout << tVertexShader->getCompilerLog() << "\n";
   
  // Compile fragment program 
  if (fragmentMem!=0)
  if (!tFragmentShader->compile())
  {
     cout << "***COMPILER ERROR (Fragment Shader):\n";
     cout << tFragmentShader->getCompilerLog() << endl;
     
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     delete tGeometryShader;
     return 0;   
  }
  cout << "***GLSL Compiler Log (Fragment Shader):\n";
  cout << tFragmentShader->getCompilerLog() << "\n";
   
  // Add to object    
  if (vertexMem!=0) o->addShader(tVertexShader);
  if (geometryMem!=0) o->addShader(tGeometryShader);
  if (fragmentMem!=0) o->addShader(tFragmentShader); 

  // link 
  if (!o->link())
  {
     cout << "**LINKER ERROR\n";
     cout << o->getLinkerLog() << endl;
     delete o;
     delete tVertexShader;
     delete tFragmentShader;
     delete tGeometryShader;
     return 0;
  }
  cout << "***GLSL Linker Log:\n";
  cout << o->getLinkerLog() << endl;

  _shaderObjectList.push_back(o);
  o->manageMemory();

   return o;
}

// ----------------------------------------------------------------------------

 BOOL  TDE_GLSLManager::free(TDE_GLSL* o)
 {
   vector<TDE_GLSL*>::iterator  i=_shaderObjectList.begin();
   while (i!=_shaderObjectList.end()) 
   {
        if ((*i)==o)
        {
            _shaderObjectList.erase(i);
            delete o;
            return true;
        }
        i++;
   }   
   return false;
 }


