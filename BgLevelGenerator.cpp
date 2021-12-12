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
        int h0 = rand() % range + offs;//第一个小场景像素个数
        int h1 = rand() % range + offs;//第二个小小场景像素个数
        //2<=(oh,h)<=5或1<=(oh,h)<=6
        for (int x = 0; x < width; x++)//竖直渲染
		{
            h0 = h1;//保留其中一个
            while (h0 == h1)
			{
                h1 = rand() % range + offs;
            }//使h0!=h1
            int min_h = (h0 < h1) ? h0 : h1;//the smaller one
            int max_h = (h0 < h1) ? h1 : h0;//the bigger one
			for (int y = 0; y < height; y++)
			{
                if (y < min_h)//天空或空白(透明)像素块
				{
                    if (distant) {//天空
                        int s = y > 2 ? 2 : y;//s是0,1,2
                        level->setBlock(x, y, (uint8_t)(4 + s * 8));//4,12,20
					}
					else {
                        level->setBlock(x, y, (uint8_t)5);//空白(透明)
					}
				}
                else if (y == min_h) {//第一个蘑菇头的像素块
                    int s = min_h == h1 ? 0 : 1;
					s += distant ? 2 : 0;
                    level->setBlock(x, y, (uint8_t)s);
				}
                else if (y == max_h) {//第二个蘑菇头的像素块
                    int s = min_h == h1 ? 0 : 1;
                    s += distant ? 2 : 0;//s是2或3
                    level->setBlock(x, y, (uint8_t)(s + 16));
				}
                else {//蘑菇体
                    int s = y > max_h ? 1 : 0;
                    if (min_h == h0)
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
    this->distant = distant;//远景还是前景.远景是蓝色的天空和柱子,近景是黄色的柱子
	this->type = type;
}
