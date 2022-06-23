
#define DEVELOPMENT 1.0
#include "EncridJPEG.h"
#include "EncridDevice.h"

#include <stdio.h>
#include <gl\GL.h>

#include <jpeglib.h>
#include <jerror.h>

#pragma comment (lib, "jpeg.lib")

static int year = 0, day = 0;
int posicaoluz = 0;
int tx;
GLuint texture_id[1];
int LoadJPEG ( char *filename , TDE_JPEG* jpgc)
{
    // Contém as informações do arquivo
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    GLubyte *linha;
    // Conterá a imagem carregada
    GLubyte *image;
    // Tamanho da Imagem
    int ImageSize;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    // Abre o arquivo, lê seu cabeçalho
    // e processa a descompressão da mesma
    FILE *fd_arquivo=fopen(filename, "rb");
    jpeg_stdio_src(&cinfo, fd_arquivo);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress ( &cinfo );
    ImageSize = cinfo.image_width * cinfo.image_height * 3;
    image = (GLubyte *) malloc ( ImageSize );
    linha=image;
	//
	jpgc->W = cinfo.image_width;
	jpgc->H = cinfo.image_height;
	//
    while ( cinfo.output_scanline < cinfo.output_height )
    {
        linha = image + 3 * cinfo.image_width * cinfo.output_scanline;
        jpeg_read_scanlines ( &cinfo, &linha, 1 );
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    //Aplicação de filtros para tratamento da imagem
    //pelo OpenGL
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    // Efetua a geração da imagem na memória
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cinfo.image_width, cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    fclose (fd_arquivo);
    free (image);
    return 1;
}

VOID TDE_JPEG::LoadJPGFromFile(TDE_Device* dev, CHAR* fileName)
{
	//_TDE_GENTEXTURE();
	//
	Index = dev->_TexIDCounter;
	glBindTexture(GL_TEXTURE_2D, Index);
	LoadJPEG(fileName, this);
	dev->_TexIDCounter++;
}