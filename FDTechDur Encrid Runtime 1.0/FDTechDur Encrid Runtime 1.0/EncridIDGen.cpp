
#define DEVELOPMENT 1.0
#include "EncridIDGen.h"

extern "C"
{
	ENCRID_API INT TDE_GetID()
	{
		IDCounter++;
		return IDCounter - 1;
	}

};

BOOL idExists(INT id, TDE_IDGEN* idg)
{
	for (INT C = 0; C < idg->_IDs.size(); C++)
	{
		if (idg->_IDs[C] == id)
		{
			return true;
		}
	}
	return false;
}

INT TDE_IDGEN::AddID(INT id)
{
	if (idExists(id, this))
	{
		return -1;
	}
	if (!idExists(id, this))
	{
		_IDs.push_back(id);
		for (int C = 0; C < _IDs.size(); C++)
		{
			if (_IDs[C] == id)
			{
				return C;
			}
		}
	}
	return -1;
}

INT TDE_IDGEN::GetID()
{
	for (INT C = 0; true; C++)
	{
		if (!idExists(C, this))
		{
			_IDs.push_back(C);
			return C;
			break;
		}
	}
	return -1;
}

BOOL TDE_IDGEN::IDExists(INT id)
{
	return idExists(id, this);
}

VOID TDE_IDGEN::RemoveID(INT id)
{
	if (idExists(id, this))
	{
		for (int C = 0; C < _IDs.size(); C++)
		{
			if (_IDs[C] == id)
			{
				_IDs[C] = -1;
				_IDs.erase(_IDs.begin() + C);
				return;
			}
		}
	}
}