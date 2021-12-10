#include<random>

#include "BgLevelGenerator.h"
#include "LevelGenerator.h"

void BgLevelGenerator::createLevel(int width, int height, bool distant, int type, Level* level)
{

    //width=2048,height=15
	BgLevelGenerator bgLevelGenerator(width, height, distant, type);
    bgLevelGenerator.createLevel(level);
}

void BgLevelGenerator::createLevel(Level* level)
{
	switch (type)
	{
	case LevelGenerator::TYPE_OVERGROUND:
	{
		int range = distant ? 4 : 6;
		int offs = distant ? 2 : 1;
		int oh = rand() % range + offs;
		int h = rand() % range + offs;
        //2<=(oh,h)<=5或1<=(oh,h)<=7
		for (int x = 0; x < width; x++)
		{
			oh = h;
			while (oh == h)
			{
				h = rand() % range + offs;
            }//使oh!=h
			for (int y = 0; y < height; y++)
			{
                int h0 = (oh < h) ? oh : h;//the smaller one
                int h1 = (oh < h) ? h : oh;//the bigger one
                if (y < h0)//[,h0)
				{
					if (distant) {
						int s = 2;
						if (y < 2) {
							s = y;
                        }//s equals the smaller one,which is 0 or 1
                        level->setBlock(x, y, (uint8_t)(4 + s * 8));//4 or 12
					}
					else {
                        level->setBlock(x, y, (uint8_t)5);
					}
				}
                else if (y == h0) {//h0
					int s = h0 == h ? 0 : 1;
					s += distant ? 2 : 0;
                    level->setBlock(x, y, (uint8_t)s);
				}
                else if (y == h1) {//h1
					int s = h0 == h ? 0 : 1;
                    s += distant ? 2 : 0;//s是2或3
                    level->setBlock(x, y, (uint8_t)(s + 16));
				}
                else {//(h0,h1) && (h1,)
					int s = y > h1 ? 1 : 0;
					if (h0 == oh)
					{
						s = 1 - s;
					}
					s += distant ? 2 : 0;
                    level->setBlock(x, y, (uint8_t)(s + 8));
				}
			}
		}
		break;
	}
    case LevelGenerator::TYPE_UNDERGROUND:
    {
        if (distant)
        {
            int tt = 0;
            for (int x = 0; x < width; x++)
            {
                if (rand() % 1000 / 1000.00 < 0.75) tt = 1 - tt;
                for (int y = 0; y < height; y++)
                {
                    int t = tt;
                    int yy = y - 2;
                    if (yy < 0 || yy > 4)
                    {
                        yy = 2;
                        t = 0;
                    }
                    level->setBlock(x, y, (uint8_t) (4 + t + (3 + yy) * 8));
                }
            }
        }
        else
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    int t = x % 2;
                    int yy = y-1;
                    if (yy < 0 || yy > 7)
                    {
                        yy = 7;
                        t = 0;
                    }
                    if (t == 0 && yy > 1 && yy < 5)
                    {
                        t = -1;
                        yy = 0;
                    }
                    level->setBlock(x, y, (uint8_t) (6 + t + (yy) * 8));
                }
            }
        }
        break;
    }
    case LevelGenerator::TYPE_CASTLE:
    {
        if (distant)
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    int t = x % 2;
                    int yy = y - 1;
                    if (yy>2 && yy<5)
                    {
                        yy = 2;
                    }
                    else if (yy>=5)
                    {
                        yy-=2;
                    }
                    if (yy < 0)
                    {
                        t = 0;
                        yy = 5;
                    }
                    else if (yy > 4)
                    {
                        t = 1;
                        yy = 5;
                    }
                    else if (t<1 && yy==3)
                    {
                        t = 0;
                        yy = 3;
                    }
                    else if (t<1 && yy>0 && yy<3)
                    {
                        t = 0;
                        yy = 2;
                    }
                    level->setBlock(x, y, (uint8_t) (1+t + (yy + 4) * 8));
                }
            }
        }
        else
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    int t = x % 3;
                    int yy = y - 1;
                    if (yy>2 && yy<5)
                    {
                        yy = 2;
                    }
                    else if (yy>=5)
                    {
                        yy-=2;
                    }
                    if (yy < 0)
                    {
                        t = 1;
                        yy = 5;
                    }
                    else if (yy > 4)
                    {
                        t = 2;
                        yy = 5;
                    }
                    else if (t<2 && yy==4)
                    {
                        t = 2;
                        yy = 4;
                    }
                    else if (t<2 && yy>0 && yy<4)
                    {
                        t = 4;
                        yy = -3;
                    }
                    level->setBlock(x, y, (uint8_t) (1 + t + (yy + 3) * 8));
                }
            }
        }
        break;
    }
	default:
		break;
	}
}

BgLevelGenerator::BgLevelGenerator(int width, int height, bool distant, int type)
{
	this->width = width;
	this->height = height;
	this->distant = distant;
	this->type = type;
}
