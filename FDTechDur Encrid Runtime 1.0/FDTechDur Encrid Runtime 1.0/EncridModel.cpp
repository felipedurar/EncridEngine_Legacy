
#define DEVELOPMENT
#include "EncridModel.h"

       
TDE_Material::TDE_Material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t)
{
    name=na;
    alpha=al;
    ni=ni2;
    ns=n;
    dif[0]=d[0];
    dif[1]=d[1];
    dif[2]=d[2];
    //
    amb[0]=a[0];
    amb[1]=a[1];
    amb[2]=a[2];
    //
    spec[0]=s[0];
    spec[1]=s[1];
    spec[2]=s[2];
    //
    illum=i;
    texture=t;
}
       
 

TDE_Model::TDE_Model()
{
	UseGLListIs = FALSE;
}

