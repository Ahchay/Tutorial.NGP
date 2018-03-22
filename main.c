#include "ngpc.h"
#include "carthdr.h" // TODO: edit game name in carthdr.h
#include "library.h"
// #include "your tileset here"
#include "tutorial.h"
#include "tutorial.c"
#include "map.c"

void main()
{
	u8 Forever;
	u8 GameOver;
	u8 LevelOn;
	u8 iLoop;
	u8 iEnemyLoop;
	u8 CurrentLevel;
	u16 CurrentTile;
	u8 CurrentPalette;
	u8 xDistance;
	u8 yDistance;
	u8 SkipTurn;
	u8 Lives;
	u8 DeadEnemies;
	u8 Score;
	T_PLAYERSHIP tpsMyShip;
	T_PLAYERSHIP tpsEnemyShip[3];
	T_PLAYERSHIP tpsBullet;

	InitNGPC();
	SysSetSystemFont();

	InstallTileSet(tutorial, sizeof(tutorial)/2);

	ClearScreen(SCR_1_PLANE);
	ClearScreen(SCR_2_PLANE);
	SetBackgroundColour(RGB(0, 0, 0));

	//Setup tile palettes
	SetPalette(SCR_1_PLANE, 0, 0, 0, 0, RGB(15,15,15));
	SetPalette(SCR_2_PLANE, 0, 0, RGB(15,0,0), RGB(0,15,0), RGB(0,0,15));

	//Setup Sprite Palettes
	SetPalette(SPRITE_PLANE, 0, 0, RGB(8,3,10), RGB(2,10,2), RGB(3,9,9));
	SetPalette(SPRITE_PLANE, 1, 0, RGB(2,14,5), RGB(17,10,2), RGB(2,2,14));

	Forever=1;

	while (Forever)
	{

		PrintString(SCR_1_PLANE, 0, 0, 0, "PRESS 'OPT' TO PLAY");

		//Loop until player presses the "B" button.
		while (!(JOYPAD & J_OPTION));

		ClearScreen(SCR_1_PLANE);

		CurrentLevel=1;

		GameOver=0;
		Lives=3;
		Score=0;

		while(Forever && !GameOver) // never fall out of main!!!
		{

			//Test GameOver conditions
			if (Lives==0)
				GameOver=1;
			else
			{
				if (tpsMyShip.Flags)
				{
					CurrentLevel++;
					if (CurrentLevel>1)
						CurrentLevel=0;
				}
				else
				{
					Lives--;
				}
			}

			DrawLevel(CurrentLevel);

			//Setup players sprite
			tpsMyShip.SpriteID=0;
			tpsMyShip.zPosition=24;
			tpsMyShip.xPosition=LevelData[CurrentLevel].PlayerX;
			tpsMyShip.yPosition=LevelData[CurrentLevel].PlayerY;
			tpsMyShip.Direction=DirRight;
			tpsMyShip.Flags=1;
			tpsMyShip.Palette=0;
			tpsMyShip.TileNumber=ShipSprite;
			SetSprite(tpsMyShip.SpriteID, tpsMyShip.TileNumber, 0, tpsMyShip.xPosition, tpsMyShip.yPosition, tpsMyShip.Palette);

			//Setup bullet sprite
			tpsBullet.SpriteID=4;
			tpsBullet.zPosition=24;
			tpsBullet.xPosition=0;
			tpsBullet.yPosition=0;
			tpsBullet.Direction=0;
			tpsBullet.Flags=0;
			tpsBullet.Palette=0;
			tpsBullet.TileNumber=BulletSprite;
			SetSprite(tpsBullet.SpriteID, tpsBullet.TileNumber, 0, tpsBullet.xPosition, tpsBullet.yPosition, tpsBullet.Palette);

			//Setup enemy sprite
			for (iEnemyLoop=0;iEnemyLoop<3;iEnemyLoop++)
			{
				tpsEnemyShip[iEnemyLoop].SpriteID=1+iEnemyLoop;
				tpsEnemyShip[iEnemyLoop].zPosition=24;
				tpsEnemyShip[iEnemyLoop].xPosition=LevelData[CurrentLevel].EnemyX;
				tpsEnemyShip[iEnemyLoop].yPosition=LevelData[CurrentLevel].EnemyY;
				tpsEnemyShip[iEnemyLoop].Direction=DirRight;
				tpsEnemyShip[iEnemyLoop].Flags=1;
				tpsEnemyShip[iEnemyLoop].Palette=0;
				tpsEnemyShip[iEnemyLoop].TileNumber=EnemySprite+iEnemyLoop;
				SetSprite(tpsEnemyShip[iEnemyLoop].SpriteID, tpsEnemyShip[iEnemyLoop].TileNumber, 0, tpsEnemyShip[iEnemyLoop].xPosition, tpsEnemyShip[iEnemyLoop].yPosition, tpsEnemyShip[iEnemyLoop].Palette);
			}

			LevelOn=1;
			DeadEnemies=0;
			SkipTurn=0;

			while (Forever && !GameOver && LevelOn)
			{
				PrintString(SCR_2_PLANE, 0, 0, 0, "Score:");
				PrintDecimal(SCR_2_PLANE, 0, 6, 0, Score, 3);
				PrintString(SCR_2_PLANE, 0, 12, 0, "Lives:");
				PrintDecimal(SCR_2_PLANE, 0, 18, 0, Lives, 2);
				//Move enemy
				//Pattern mode.
				//Beastie Direction determines which way we move the pointer...
				if (tpsEnemyShip[0].Flags)
				{
					if (tpsEnemyShip[0].Direction==DirLeft)
					{
						//Move left
						tpsEnemyShip[0].xPosition--;
						if (tpsEnemyShip[0].xPosition==16)
							tpsEnemyShip[0].Direction=DirRight;
					}
					else
					{
						tpsEnemyShip[0].xPosition++;
						if (tpsEnemyShip[0].xPosition==127)
							tpsEnemyShip[0].Direction=DirLeft;
					}
					//yPosition comes from the sine table
					tpsEnemyShip[0].yPosition=SineTable[tpsEnemyShip[0].xPosition];
				}

				//Rule mode
				if (tpsEnemyShip[1].Flags)
				{
					CurrentTile=0;
					CurrentPalette=0;
					if (tpsEnemyShip[1].Direction==DirLeft)
					{
						tpsEnemyShip[1].xPosition--;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsEnemyShip[1].xPosition)>>3, (tpsEnemyShip[1].yPosition+4)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsEnemyShip[1].xPosition++;
							tpsEnemyShip[1].Direction=DirUp;
						}
					}
					else if (tpsEnemyShip[1].Direction==DirRight)
					{
						tpsEnemyShip[1].xPosition++;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsEnemyShip[1].xPosition+7)>>3, (tpsEnemyShip[1].yPosition+4)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsEnemyShip[1].xPosition--;
							tpsEnemyShip[1].Direction=DirDown;
						}
					}
					else if (tpsEnemyShip[1].Direction==DirDown)
					{
						tpsEnemyShip[1].yPosition++;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsEnemyShip[1].xPosition+4)>>3, (tpsEnemyShip[1].yPosition+7)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsEnemyShip[1].yPosition--;
							tpsEnemyShip[1].Direction=DirLeft;
						}
					}
					else if (tpsEnemyShip[1].Direction==DirUp)
					{
						tpsEnemyShip[1].yPosition--;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsEnemyShip[1].xPosition+4)>>3, (tpsEnemyShip[1].yPosition)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsEnemyShip[1].yPosition++;
							tpsEnemyShip[1].Direction=DirRight;
						}
					}
				}

				//Enemy 3 - Environmental movement
				//Calculate any collisions with other enemy sprites
				if (tpsEnemyShip[2].Flags)
				{
					if (!SkipTurn)
					{
						for (iEnemyLoop=0;iEnemyLoop<=2;iEnemyLoop++)
						{
							if (iEnemyLoop!=2)
							{
								if (tpsEnemyShip[iEnemyLoop].Flags)
								{
									if (SpriteCollision(tpsEnemyShip[iEnemyLoop], tpsEnemyShip[2]))
									{
										SkipTurn=1;
										//Run away!
										if (tpsEnemyShip[iEnemyLoop].xPosition<=tpsEnemyShip[2].xPosition)
											tpsEnemyShip[2].xPosition++;
										else
											tpsEnemyShip[2].xPosition--;
										if (tpsEnemyShip[iEnemyLoop].yPosition<=tpsEnemyShip[2].yPosition)
											tpsEnemyShip[2].yPosition++;
										else
											tpsEnemyShip[2].yPosition--;
									}
								}
							}
						}
						//Calculate the relative distance along the x & y axis to the player
						if (!SkipTurn)
						{
							if (tpsMyShip.xPosition<=tpsEnemyShip[2].xPosition)
								xDistance=tpsEnemyShip[2].xPosition-tpsMyShip.xPosition;
							else
								xDistance=tpsMyShip.xPosition-tpsEnemyShip[2].xPosition;
							if (tpsMyShip.yPosition<=tpsEnemyShip[2].yPosition)
								yDistance=tpsEnemyShip[2].yPosition-tpsMyShip.yPosition;
							else
								yDistance=tpsMyShip.yPosition-tpsEnemyShip[2].yPosition;

							//Which is the greater?
							if (xDistance>=yDistance)
							{
								//Move along the x axis
								if (tpsMyShip.xPosition<=tpsEnemyShip[2].xPosition)
								{
									tpsEnemyShip[2].xPosition--;
									tpsEnemyShip[2].Direction=DirLeft;
								}
								else
								{
									tpsEnemyShip[2].xPosition++;
									tpsEnemyShip[2].Direction=DirRight;
								}
							}
							else
							{
								//Move along the y axis
								if (tpsMyShip.yPosition<=tpsEnemyShip[2].yPosition)
								{
									tpsEnemyShip[2].yPosition--;
									tpsEnemyShip[2].Direction=DirUp;
								}
								else
								{
									tpsEnemyShip[2].yPosition++;
									tpsEnemyShip[2].Direction=DirDown;
								}
							}

							SkipTurn=1;
						}
					}
					else
					{
						SkipTurn=0;
					}
				}

				//Draw the enemy sprites
				for (iEnemyLoop=0;iEnemyLoop<3;iEnemyLoop++)
				{
					if (tpsEnemyShip[iEnemyLoop].Flags)
					{
						CopySpriteTile((u16 *) enemysprite, EnemySprite+iEnemyLoop, tpsEnemyShip[iEnemyLoop].Direction);
						SetSpritePosition(tpsEnemyShip[iEnemyLoop].SpriteID, tpsEnemyShip[iEnemyLoop].xPosition, tpsEnemyShip[iEnemyLoop].yPosition);
					}
				}

				CurrentTile=0;
				CurrentPalette=0;
				//Test Left/Right axis
				if (JOYPAD & J_LEFT)
				{
					tpsMyShip.xPosition--;
					tpsMyShip.Direction=DirLeft;
					//Test point is the middle left
					GetTile(SCR_2_PLANE, &CurrentPalette, (tpsMyShip.xPosition)>>3, (tpsMyShip.yPosition+4)>>3, &CurrentTile);
					if (CurrentTile!=TileNull)
						tpsMyShip.xPosition++;

				}
				else if (JOYPAD & J_RIGHT)
				{
					tpsMyShip.xPosition++;
					tpsMyShip.Direction=DirRight;
					//Test point is middle right
					GetTile(SCR_2_PLANE, &CurrentPalette, (tpsMyShip.xPosition+7)>>3, (tpsMyShip.yPosition+4)>>3, &CurrentTile);
					if (CurrentTile!=TileNull)
						tpsMyShip.xPosition--;
				}
				//Test Up/Down axis
				if (JOYPAD & J_UP)
				{
					tpsMyShip.yPosition--;
					tpsMyShip.Direction=DirUp;
					//Test point is top middle
					GetTile(SCR_2_PLANE, &CurrentPalette, (tpsMyShip.xPosition+4)>>3, (tpsMyShip.yPosition)>>3, &CurrentTile);
					if (CurrentTile!=TileNull)
						tpsMyShip.yPosition++;
				}
				else if (JOYPAD & J_DOWN)
				{
					tpsMyShip.yPosition++;
					tpsMyShip.Direction=DirDown;
					//Test point is bottom middle
					GetTile(SCR_2_PLANE, &CurrentPalette, (tpsMyShip.xPosition+4)>>3, (tpsMyShip.yPosition+7)>>3, &CurrentTile);
					if (CurrentTile!=TileNull)
						tpsMyShip.yPosition--;
				}
				//Fire!
				if ((JOYPAD & J_A) && (!tpsBullet.Flags))
				{
					tpsBullet.xPosition=tpsMyShip.xPosition;
					tpsBullet.yPosition=tpsMyShip.yPosition;
					tpsBullet.Direction=tpsMyShip.Direction;
					tpsBullet.Flags=1;
				}
				//Draw player's ship
				CopySpriteTile((u16 *) shipsprite, ShipSprite, tpsMyShip.Direction);
				SetSpritePosition(tpsMyShip.SpriteID, tpsMyShip.xPosition, tpsMyShip.yPosition);

				for (iEnemyLoop=0;iEnemyLoop<3;iEnemyLoop++)
				{
					if (tpsEnemyShip[iEnemyLoop].Flags)
					{
						if (SpriteCollision(tpsMyShip, tpsEnemyShip[iEnemyLoop]))
							tpsMyShip.Flags=0;
					}
				}

				//Move the bullet!
				//Rule mode
				if (tpsBullet.Flags)
				{
					CurrentTile=0;
					CurrentPalette=0;
					if (tpsBullet.Direction==DirLeft)
					{
						tpsBullet.xPosition-=2;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsBullet.xPosition)>>3, (tpsBullet.yPosition+4)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsBullet.Flags=0;
						}
					}
					else if (tpsBullet.Direction==DirRight)
					{
						tpsBullet.xPosition+=2;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsBullet.xPosition+7)>>3, (tpsBullet.yPosition+4)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsBullet.Flags=0;
						}
					}
					else if (tpsBullet.Direction==DirDown)
					{
						tpsBullet.yPosition+=2;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsBullet.xPosition+4)>>3, (tpsBullet.yPosition+7)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsBullet.Flags=0;
						}
					}
					else if (tpsBullet.Direction==DirUp)
					{
						tpsBullet.yPosition-=2;
						GetTile(SCR_2_PLANE, &CurrentPalette, (tpsBullet.xPosition+4)>>3, (tpsBullet.yPosition)>>3, &CurrentTile);
						if (CurrentTile!=TileNull)
						{
							tpsBullet.Flags=0;
						}
					}
				}
				//Draw Bullet
				CopySpriteTile((u16 *) bulletsprite, BulletSprite, tpsBullet.Flags);
				SetSpritePosition(tpsBullet.SpriteID, tpsBullet.xPosition, tpsBullet.yPosition);

				//Check for collisions with the enemy ships
				for (iEnemyLoop=0;iEnemyLoop<3;iEnemyLoop++)
				{
					if (tpsEnemyShip[iEnemyLoop].Flags && tpsBullet.Flags)
					{
						if (SpriteCollision(tpsBullet, tpsEnemyShip[iEnemyLoop]))
						{
							tpsEnemyShip[iEnemyLoop].Flags=0;
							tpsBullet.Flags=0;
							Score++;
							DeadEnemies++;
						}
					}
				}

				//Check for end of level conditions
				if (!tpsMyShip.Flags || DeadEnemies==3)
					LevelOn=0;

				Sleep(2);
			}
		}
	}

}


