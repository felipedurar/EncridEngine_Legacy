
#define DEVELOPMENT
#include "EncridPhysics.h"
#include "EncridGame.h"
#include "EncridImage.h"

TDE_GO_PHYSICS_VALUES::TDE_GO_PHYSICS_VALUES()
{
	_PENABLE = FALSE;
	_YForce = 0.000000f;
	_XForce = 0.000000f;
	_PMASS = 0.00000f;
	_YMAXF = 20.000000f;
	_YMINF = -20.000000f;
	_XMAXF = 20.000000f;
	_XMINF = -20.000000f;
	_XFS = 0.000000f;
	_YFS = 0.000000f;
	_XFSR = 0.500000f;
	_YFSR = 0.500000f;
	// Last Pos
	_XLP = 0.000000f;
	_YLP = 0.000000f;
	// LAST DIR
	_XLD = 0;
	_YLD = 0;
	// BackUp X and Y
	_BX = 0.000000f;
	_BY = 0.000000f;
	// Ratio X and Y
	_RX = 0.000000f;
	_RY = 0.000000f;

}

// GOPysics

TDE_GOPhysics::TDE_GOPhysics(TDE_GameObject* gop)
{
	go = gop;
}

VOID TDE_GOPhysics::EnablePhysics()
{
	go->GOPhysicsVals->_PENABLE = TRUE;
}

VOID TDE_GOPhysics::SetYGravity(FLOAT force, FLOAT maxForce)
{
	go->GOPhysicsVals->_YFS = force;
	go->GOPhysicsVals->_YMAXF = maxForce;
}

VOID TDE_GOPhysics::DoPhysics()
{
	if (go->GOPhysicsVals->_PENABLE == FALSE) return;
	// Stabilization Force
	if (go->GOPhysicsVals->_XForce > go->GOPhysicsVals->_XFS)
	{
		if (go->GOPhysicsVals->_XForce - go->GOPhysicsVals->_XFS < go->GOPhysicsVals->_XFSR)
		{
			go->GOPhysicsVals->_XForce = go->GOPhysicsVals->_XFS;
		}
		else
		{
			if (go->GOPhysicsVals->_XForce > go->GOPhysicsVals->_XFS) go->GOPhysicsVals->_XForce -= go->GOPhysicsVals->_XFSR;
		}
	}
	if (go->GOPhysicsVals->_XForce < go->GOPhysicsVals->_XFS)
	{
		if (go->GOPhysicsVals->_XFS - go->GOPhysicsVals->_XForce < go->GOPhysicsVals->_XFSR)
		{
			go->GOPhysicsVals->_XForce = go->GOPhysicsVals->_XFS;
		}
		else
		{
            if (go->GOPhysicsVals->_XForce < go->GOPhysicsVals->_XFS) go->GOPhysicsVals->_XForce += go->GOPhysicsVals->_XFSR;
		}
	}
	//
	if (go->GOPhysicsVals->_YForce > go->GOPhysicsVals->_YFS)
	{
		if (go->GOPhysicsVals->_YForce - go->GOPhysicsVals->_YFS < go->GOPhysicsVals->_YFSR)
		{
			go->GOPhysicsVals->_YForce = go->GOPhysicsVals->_YFSR;
		}
		else
		{
			if (go->GOPhysicsVals->_YForce > go->GOPhysicsVals->_YFS) go->GOPhysicsVals->_YForce -= go->GOPhysicsVals->_YFSR;
		}
	}
	if (go->GOPhysicsVals->_YForce < go->GOPhysicsVals->_YFS)
	{
		if (go->GOPhysicsVals->_YFS - go->GOPhysicsVals->_YForce < go->GOPhysicsVals->_YFSR)
		{
			go->GOPhysicsVals->_YForce = go->GOPhysicsVals->_YFSR;
		}
		else
		{
			if (go->GOPhysicsVals->_YForce < go->GOPhysicsVals->_YFS) go->GOPhysicsVals->_YForce += go->GOPhysicsVals->_YFSR;
		}
	}
	// Max & min
	if (go->GOPhysicsVals->_XForce > go->GOPhysicsVals->_XMAXF) go->GOPhysicsVals->_XForce = go->GOPhysicsVals->_XMAXF;
	if (go->GOPhysicsVals->_XForce < go->GOPhysicsVals->_XMINF) go->GOPhysicsVals->_XForce = go->GOPhysicsVals->_XMINF;
	//
	if (go->GOPhysicsVals->_YForce > go->GOPhysicsVals->_YMAXF) go->GOPhysicsVals->_YForce = go->GOPhysicsVals->_YMAXF;
	if (go->GOPhysicsVals->_YForce < go->GOPhysicsVals->_YMINF) go->GOPhysicsVals->_YForce = go->GOPhysicsVals->_YMINF;
	// Apply Forces
	go->SetPosition(go->GetX() + go->GOPhysicsVals->_XForce, go->GetY() + go->GOPhysicsVals->_YForce);
}

VOID TDE_GOPhysics::SetMass(INT m)
{
	go->GOPhysicsVals->_PMASS = m / 100.000000f;
}

VOID TDE_GOPhysics::ApplyYForce(FLOAT yf)
{
	go->GOPhysicsVals->_YForce += yf;
}

VOID TDE_GOPhysics::ApplyXForce(FLOAT xf)
{
	go->GOPhysicsVals->_XForce += xf;
}

VOID TDE_GOPhysics::SetXForce(FLOAT xf)
{
	go->GOPhysicsVals->_XForce = xf;
}

VOID TDE_GOPhysics::SetYForce(FLOAT yf)
{
	go->GOPhysicsVals->_YForce = yf;
}

VOID TDE_GOPhysics::XForceReductionRatio(FLOAT xs)
{
	go->GOPhysicsVals->_XFSR = xs / 100.000000f;
}
VOID TDE_GOPhysics::YForceReductionRatio(FLOAT ys)
{
	go->GOPhysicsVals->_YFSR = ys / 100.000000f;
}

BOOL TDE_GOPhysics::Colision(TDE_GOPhysics* goPhy)
{
	if ((INT)goPhy->go->GetX() + goPhy->go->Sprite->W > (INT)go->GetX())
	{
		if ((INT)goPhy->go->GetX() < (INT)go->GetX() + go->Sprite->W)
		{
			if ((INT)goPhy->go->GetY() + goPhy->go->Sprite->H > (INT)go->GetY())
			{
				if ((INT)goPhy->go->GetY() < (INT)go->GetY() + go->Sprite->H)
				{
					return true;
				}
			}
		}
	}
	return false;
}

BOOL TDE_GOPhysics::ColisionVari(TDE_GOPhysics* goPhy, FLOAT vari)
{
	if ((INT)(goPhy->go->GetX() - vari) + (goPhy->go->Sprite->W + vari) > (INT)go->GetX())
	{
		if ((INT)(goPhy->go->GetX() - vari) < (INT)go->GetX() + go->Sprite->W)
		{
			if ((INT)(goPhy->go->GetY() - vari) + (goPhy->go->Sprite->H + vari) > (INT)go->GetY())
			{
				if ((INT)(goPhy->go->GetY() - vari) < (INT)go->GetY() + go->Sprite->H)
				{
					return true;
				}
			}
		}
	}
	return false;
}

TDE_Side::TDE_Side()
{
	xs = 0;
	ys = 0;
}

TDE_Side TDE_Side::GetSideColision(TDE_GameObject* go1, TDE_GameObject* go2)
{
	float player_bottom = go1->GetY() + go1->Sprite->H;
	float tiles_bottom = go2->GetY() + go2->Sprite->H;
	float player_right = go1->GetX() + go1->Sprite->W;
	float tiles_right = go2->GetX() + go2->Sprite->W;
	//
    float b_collision = tiles_bottom - go1->GetY();
    float t_collision = player_bottom - go2->GetY();
    float l_collision = player_right - go2->GetX();
	float r_collision = tiles_right - go1->GetX();
	//
	xs = 0;
	ys = 0;
	//
    if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision )
    {                           
        ys = 1;
    }
    if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision)                        
    {
        ys = -1;
    }
    if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision)
    {
        xs = 1;
    }
    if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision )
    {
        xs = -1;
    }
	return *this;
}

TDE_GlobalPhysics::TDE_GlobalPhysics()
{
	CollisionPrecision = 50;
}

VOID TDE_GlobalPhysics::DoPhysics(const vector<TDE_GameObject*>& GOs)
{
	// Do Physics
	for (INT C = 0; C < GOs.size(); C++)
	{
		GOs[C]->GOPhysics->DoPhysics();
	}
	// 
	for (INT C = 0; C < GOs.size(); C++)
	{
		// Ratio
		FLOAT prec = CollisionPrecision * 1.000000f;
		GOs[C]->GOPhysicsVals->_RX = (GOs[C]->GetX() - GOs[C]->GOPhysicsVals->_XLP) / prec;
		GOs[C]->GOPhysicsVals->_RY = (GOs[C]->GetY() - GOs[C]->GOPhysicsVals->_YLP) / prec;
		// BackUps
		GOs[C]->GOPhysicsVals->_BX = GOs[C]->GetX();
		GOs[C]->GOPhysicsVals->_BY = GOs[C]->GetY();
		// Old Pos = New Pos
		GOs[C]->_SetPosNC(GOs[C]->GOPhysicsVals->_XLP, GOs[C]->GOPhysicsVals->_YLP);
	}
	for (INT C = 0; C < CollisionPrecision; C++)
	{
		for (INT D = 0; D < GOs.size(); D++)
		{
			GOs[D]->SetPosition(GOs[D]->GetX() + GOs[D]->GOPhysicsVals->_RX, GOs[D]->GetY() + GOs[D]->GOPhysicsVals->_RY);
		}
		//
	    SolveCollision(GOs);
	}


}
//
VOID TDE_GlobalPhysics::SolveCollision(const vector<TDE_GameObject*>& GOs)
{
	for (INT C = 0; C < GOs.size(); C++)
	{
		if (GOs[C] != NULL)
		{
		    	for (INT D = 0; D < GOs.size(); D++)
				{
					if (C != D)
					{
						if (GOs[D] != NULL)
						{
							//
							if (GOs[C]->Fixed == false)
							{
								//GOs[C]->_SetPosNC(GOs[C]->GetX() + xr, GOs[C]->GetY() + yr);
								if (GOs[C]->GOPhysics->Colision(GOs[D]->GOPhysics))
								{
									TDE_Side sd;
									sd.GetSideColision(GOs[C], GOs[D]);
									WCHAR txt[80];
									swprintf(txt, L"%d - %d", sd.xs, sd.ys);
									//MessageBox(NULL, txt, L"INF", MB_OK);
									//
									int counter = 0;
									while (GOs[C]->GOPhysics->Colision(GOs[D]->GOPhysics)) // && counter < 10000
									{
										counter++;
										//
										if (sd.ys == 1)
										{
											GOs[C]->GOPhysics->SetYForce(0.0f);
											GOs[C]->_SetPosNC(GOs[C]->GetX(), GOs[C]->GetY() - 0.1f);
										}
										else if (sd.ys == -1)
										{
											GOs[C]->GOPhysics->SetYForce(0.0f);
											GOs[C]->_SetPosNC(GOs[C]->GetX(), GOs[C]->GetY() + 0.1f);
										}
										//
										if (sd.xs == 1)
										{
											GOs[C]->GOPhysics->SetXForce(0.0f);
											GOs[C]->_SetPosNC(GOs[C]->GetX() - 0.1f, GOs[C]->GetY());
											//MessageBox(NULL, txt, L"INF", MB_OK);
										}
										else if (sd.xs == -1)
										{
											GOs[C]->GOPhysics->SetXForce(0.0f);
											GOs[C]->_SetPosNC(GOs[C]->GetX() + 0.1f, GOs[C]->GetY());
											//MessageBox(NULL, txt, L"INF", MB_OK);
										}
									}
								}
								//GOs[C]->GOPhysics->DoPhysics();
							}
						}
						//
					}
				}
			//GOs[C]->GOPhysics->DoPhysics();
		}
	}


}

