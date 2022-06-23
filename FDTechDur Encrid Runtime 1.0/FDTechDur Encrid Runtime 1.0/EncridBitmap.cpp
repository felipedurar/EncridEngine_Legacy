
#define DEVELOPMENT 1.0
#include "EncridBitmap.h"
#include "EncridDevice.h"

#include <stdio.h>
#include <gl\GL.h>

#define SAIR {fclose(fp_arquivo); return -1;}
#define CTOI(C) (*(int*)&C)

int LoadBMP(char *filename, TDE_Bitmap* bmp)
{
    GLubyte *image;
    GLubyte Header[0x54];
    GLuint DataPos, imageSize;
    GLsizei Width,Height;
    int nb = 0;
    // Abre o arquivo e efetua a leitura do Header do arquivo BMP
    FILE * fp_arquivo = fopen(filename,"rb");
    if (!fp_arquivo)
    return -1;
    if (fread(Header,1,0x36,fp_arquivo)!=0x36) SAIR;
    if (Header[0]!='B' || Header[1]!='M') SAIR;
    if (CTOI(Header[0x1E])!=0) SAIR;
    if (CTOI(Header[0x1C])!=24) SAIR;
    // Recupera a informação dos atributos de
    // altura e largura da imagem
    Width = CTOI(Header[0x12]);
    Height = CTOI(Header[0x16]);
	bmp->W = Width;
	bmp->H = Height;
	//
    ( CTOI(Header[0x0A]) == 0 ) ? ( DataPos=0x36 ) : ( DataPos = CTOI(Header[0x0A]) );
    imageSize=Width*Height*3;
    // Efetura a Carga da Imagem
    image = (GLubyte *) malloc ( imageSize );
    int retorno;
    retorno = fread(image,1,imageSize,fp_arquivo);
    if (retorno !=imageSize)
    {
        free (image);
        SAIR;
    }
    // Inverte os valores de R e B
    int t, i;
    for ( i = 0; i < imageSize; i += 3 )
    {
        t = image[i];
        image[i] = image[i+2];
        image[i+2] = t;
    }
    // Tratamento da textura para o OpenGL
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // Faz a geraçao da textura na memória
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    fclose (fp_arquivo);
    free (image);
    return 1;
}

#undef SAIR
#undef CTOI

VOID TDE_Bitmap::LoadBMPFromFile(TDE_Device* dev, CHAR* fileName)
{
	//_TDE_GENTEXTURE();
	//
	Index = dev->_TexIDCounter;
	glBindTexture(GL_TEXTURE_2D, Index);
	LoadBMP(fileName, this);
	dev->_TexIDCounter++;
}