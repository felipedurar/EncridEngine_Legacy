
#pragma once

#ifdef DEVELOPMENT
#define ENCRID_API __declspec(dllexport)
#else
#define ENCRID_API __declspec(dllimport)
#endif

#ifndef _WINDOWS_
#include <Windows.h>
#endif


#include <vector>
#include <iostream>

class TDE_GLSLManager;

#ifndef DOUBLE
#define DOUBLE double
#endif
#define SCHAR  signed char

extern "C"
{
    ENCRID_API BOOL TDE_InitOpenGLExtensions(VOID); 
    ENCRID_API BOOL TDE_HasGLSLSupport(VOID);  
    ENCRID_API BOOL TDE_HasOpenGL2Support(VOID);  
    ENCRID_API BOOL TDE_HasGeometryShaderSupport(VOID); 
    ENCRID_API BOOL TDE_HasShaderModel4(VOID); 
};

class ENCRID_API TDE_GLSLObject
{
    friend class TDE_GLSL;

public:
    TDE_GLSLObject();
    ~TDE_GLSLObject();
        
    INT load(CHAR* filename);
    VOID loadFromMemory(const CHAR* program);
       
    BOOL compile(VOID);
    CHAR* getCompilerLog(VOID); 
	INT getAttribLocation(CHAR* attribName);
    
protected:
    INT program_type;
    
    UINT ShaderObject; 
    BYTE* ShaderSource;
       
    CHAR* compiler_log;
       
    BOOL is_compiled; 
    BOOL _memalloc; 
};

class ENCRID_API TDE_VertexShader : public TDE_GLSLObject
{
public:
    TDE_VertexShader();  //!< Constructor for Vertex Shader
    ~TDE_VertexShader(); 
};

class ENCRID_API TDE_FragmentShader : public TDE_GLSLObject
{
public:
    TDE_FragmentShader(); //!< Constructor for Fragment Shader
    ~TDE_FragmentShader();
    
};

class ENCRID_API TDE_GeometryShader : public TDE_GLSLObject
{
public:
    TDE_GeometryShader(); //!< Constructor for Geometry Shader
    ~TDE_GeometryShader();
};

class ENCRID_API TDE_GLSL
{
    friend class TDE_GLSLManager;

public:
    TDE_GLSL();                  
    ~TDE_GLSL();
    VOID addShader(TDE_GLSLObject* ShaderProgram); //!< add a Vertex or Fragment Program \param ShaderProgram The shader object.
      
      //!< Returns the OpenGL Program Object (only needed if you want to control everything yourself) \return The OpenGL Program Object
    UINT GetProgramObject(){return ProgramObject;}

    BOOL link(VOID);                        //!< Link all Shaders
    CHAR* getLinkerLog(VOID);                //!< Get Linker Messages \return CHAR poINTer to linker messages. Memory of this string is available until you link again or destroy this class.

    VOID begin();                           //!< use Shader. OpenGL calls will go through vertex, geometry and/or fragment shaders.
    VOID end();                             //!< Stop using this shader. OpenGL calls will go through regular pipeline.
      
    // Geometry Shader: Input Type, Output and Number of Vertices out
    VOID SetInputPrimitiveType(INT nInputPrimitiveType);   //!< Set the input primitive type for the geometry shader
    VOID SetOutputPrimitiveType(INT nOutputPrimitiveType); //!< Set the output primitive type for the geometry shader
    VOID SetVerticesOut(INT nVerticesOut);                 //!< Set the maximal number of vertices the geometry shader can output
     
    INT GetUniformLocation(const CHAR *name);  //!< Retrieve Location (index) of a Uniform Variable
	
    // Submitting Uniform Variables. You can set varname to 0 and specifiy index retrieved with GetUniformLocation (best performance)
    BOOL setUniform1f(CHAR* varname, FLOAT v0, INT index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
    BOOL setUniform2f(CHAR* varname, FLOAT v0, FLOAT v1, INT index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
    BOOL setUniform3f(CHAR* varname, FLOAT v0, FLOAT v1, FLOAT v2, INT index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
    BOOL setUniform4f(CHAR* varname, FLOAT v0, FLOAT v1, FLOAT v2, FLOAT v3, INT index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.

    BOOL setUniform1i(CHAR* varname, INT v0, INT index = -1);  //!< Specify value of uniform INTeger variable. \param varname The name of the uniform variable.
    BOOL setUniform2i(CHAR* varname, INT v0, INT v1, INT index = -1); //!< Specify value of uniform INTeger variable. \param varname The name of the uniform variable.
    BOOL setUniform3i(CHAR* varname, INT v0, INT v1, INT v2, INT index = -1); //!< Specify value of uniform INTeger variable. \param varname The name of the uniform variable.
    BOOL setUniform4i(CHAR* varname, INT v0, INT v1, INT v2, INT v3, INT index = -1); //!< Specify value of uniform INTeger variable. \param varname The name of the uniform variable.

    // Note: unsigned INTegers require GL_EXT_gpu_shader4 (for example GeForce 8800)
    BOOL setUniform1ui(CHAR* varname, UINT v0, INT index = -1); //!< Specify value of uniform unsigned INTeger variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform2ui(CHAR* varname, UINT v0, UINT v1, INT index = -1); //!< Specify value of uniform unsigned INTeger variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform3ui(CHAR* varname, UINT v0, UINT v1, UINT v2, INT index = -1); //!< Specify value of uniform unsigned INTeger variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform4ui(CHAR* varname, UINT v0, UINT v1, UINT v2, UINT v3, INT index = -1); //!< Specify value of uniform unsigned INTeger variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.

    // Arrays
    BOOL setUniform1fv(CHAR* varname, INT count, FLOAT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform2fv(CHAR* varname, INT count, FLOAT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform3fv(CHAR* varname, INT count, FLOAT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform4fv(CHAR* varname, INT count, FLOAT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
      
    BOOL setUniform1iv(CHAR* varname, INT count, INT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform2iv(CHAR* varname, INT count, INT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform3iv(CHAR* varname, INT count, INT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
    BOOL setUniform4iv(CHAR* varname, INT count, INT *value, INT index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
      
    BOOL setUniform1uiv(CHAR* varname, INT count, UINT *value, INT index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform2uiv(CHAR* varname, INT count, UINT *value, INT index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform3uiv(CHAR* varname, INT count, UINT *value, INT index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
    BOOL setUniform4uiv(CHAR* varname, INT count, UINT *value, INT index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
	
	BOOL setUniformMatrix2fv(CHAR* varname, INT count, UCHAR transpose, FLOAT *value, INT index = -1); //!< Specify values of uniform 2x2 matrix. \param varname The name of the uniform variable.
    BOOL setUniformMatrix3fv(CHAR* varname, INT count, UCHAR transpose, FLOAT *value, INT index = -1); //!< Specify values of uniform 3x3 matrix. \param varname The name of the uniform variable.
    BOOL setUniformMatrix4fv(CHAR* varname, INT count, UCHAR transpose, FLOAT *value, INT index = -1); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.
 
    // Receive Uniform variables:
    VOID getUniformfv(CHAR* varname, FLOAT* values, INT index = -1); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
    VOID getUniformiv(CHAR* varname, INT* values, INT index = -1); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
    VOID getUniformuiv(CHAR* varname, UINT* values, INT index = -1); //!< Receive value of uniform variable. \warning Requires GL_EXT_gpu_shader4 \param varname The name of the uniform variable.

    VOID BindAttribLocation(INT index, CHAR* name);
	
    //FLOAT
    BOOL setVertexAttrib1f(UINT index, FLOAT v0); 				                         //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
    BOOL setVertexAttrib2f(UINT index, FLOAT v0, FLOAT v1); 								 //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
    BOOL setVertexAttrib3f(UINT index, FLOAT v0, FLOAT v1, FLOAT v2);					 //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
    BOOL setVertexAttrib4f(UINT index, FLOAT v0, FLOAT v1, FLOAT v2, FLOAT v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

    //DOUBLE
    BOOL setVertexAttrib1d(UINT index, DOUBLE v0);                                         //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
    BOOL setVertexAttrib2d(UINT index, DOUBLE v0, DOUBLE v1);                            //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
    BOOL setVertexAttrib3d(UINT index, DOUBLE v0, DOUBLE v1, DOUBLE v2);               //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
    BOOL setVertexAttrib4d(UINT index, DOUBLE v0, DOUBLE v1, DOUBLE v2, DOUBLE v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

    //SHORT
    BOOL setVertexAttrib1s(UINT index, SHORT v0);                                      //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
    BOOL setVertexAttrib2s(UINT index, SHORT v0, SHORT v1);                          //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
    BOOL setVertexAttrib3s(UINT index, SHORT v0, SHORT v1, SHORT v2);              //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
    BOOL setVertexAttrib4s(UINT index, SHORT v0, SHORT v1, SHORT v2, SHORT v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

    // Normalized Byte (for example for RGBA colors)
    BOOL setVertexAttribNormalizedByte(UINT index, SCHAR v0, SCHAR v1, SCHAR v2, SCHAR v3); //!< Specify value of attribute. Values will be normalized.

    //INT (Requires GL_EXT_gpu_shader4)
    BOOL setVertexAttrib1i(UINT index, INT v0); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
    BOOL setVertexAttrib2i(UINT index, INT v0, INT v1); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
    BOOL setVertexAttrib3i(UINT index, INT v0, INT v1, INT v2); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
    BOOL setVertexAttrib4i(UINT index, INT v0, INT v1, INT v2, INT v3); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.

    //UINT (Requires GL_EXT_gpu_shader4)
    BOOL setVertexAttrib1ui(UINT index, UINT v0); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4. \param v0 value of the first component
    BOOL setVertexAttrib2ui(UINT index, UINT v0, UINT v1); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.
    BOOL setVertexAttrib3ui(UINT index, UINT v0, UINT v1, UINT v2); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.
    BOOL setVertexAttrib4ui(UINT index, UINT v0, UINT v1, UINT v2, UINT v3); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.

    //! Enable this Shader:
    VOID enable(VOID) //!< Enables Shader (Shader is enabled by default)
    {
        _noshader = true;
    }

    //! Disable this Shader:
    VOID disable(VOID) //!< Disables Shader.
    {
        _noshader = false;
    } 
     
protected:
    VOID manageMemory(VOID){_mM = true;}
    VOID UsesGeometryShader(BOOL bYesNo){ _bUsesGeometryShader = bYesNo;}

private:      
    UINT ProgramObject;                      // GLProgramObject
      

    CHAR*  linker_log;
    BOOL is_linked;
    std::vector<TDE_GLSLObject*> ShaderList;       // List of all Shader Programs

    BOOL _mM;
    BOOL _noshader;

    BOOL _bUsesGeometryShader;

    INT _nInputPrimitiveType;
    INT _nOutputPrimitiveType;
    INT _nVerticesOut;
        
};
	
class ENCRID_API TDE_GLSLManager
{
public:
    TDE_GLSLManager();
    virtual ~TDE_GLSLManager();

    // Regular GLSL (Vertex+Fragment Shader)
    TDE_GLSL* loadfromFile(CHAR* vertexFile, CHAR* fragmentFile); 
    TDE_GLSL* loadfromMemory(const CHAR* vertexMem, const CHAR* fragmentMem); 
       
    // With Geometry Shader (Vertex+Geomentry+Fragment Shader)
    TDE_GLSL* loadfromFile(CHAR* vertexFile, CHAR* geometryFile, CHAR* fragmentFile);
    TDE_GLSL* loadfromMemory(const CHAR* vertexMem, const CHAR* geometryMem, const CHAR* fragmentMem);
       
    VOID SetInputPrimitiveType(INT nInputPrimitiveType); 
    VOID SetOutputPrimitiveType(INT nOutputPrimitiveType); 
    VOID SetVerticesOut(INT nVerticesOut);          
  
    BOOL free(TDE_GLSL* o); 

private:
    std::vector<TDE_GLSL*>  _shaderObjectList; 
    INT _nInputPrimitiveType;
    INT _nOutputPrimitiveType;
    INT _nVerticesOut;
};




