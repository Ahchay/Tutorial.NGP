typedef struct TPlayerShip
{
	u8 SpriteID;
	u8 zPosition;
	u8 xPosition;
	u8 yPosition;
	u8 Direction;
	u8 Flags;
	u8 Palette;
	u8 TileNumber;
} T_PLAYERSHIP;

typedef struct MGameLevel
{
	u8 PlayerX;
	u8 PlayerY;
	u8 EnemyX;
	u8 EnemyY;
	u8 Background[18][18];
} M_GAMELEVEL;

const unsigned char DirRight=0;
const unsigned char DirUp=1;
const unsigned char DirLeft=2;
const unsigned char DirDown=3;