#include "Framework.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>

void generatePlatforms(int a[][2], float gap, int Width, int Height, int wPlatform)
{
    srand(time(NULL));
    a[1][0] = Width / 2;
    a[1][1] = Height - 60;
    for (int i = 2; i <= 6; i++)
    {
        if (a[i - 1][0] > Width / 2)
            a[i][0] = rand() % (Width / 2 - wPlatform);
        else
            a[i][0] = rand() % (Width / 2 - wPlatform) + Width / 2;
        a[i][1] = (gap * i) - gap;
    }
}

void numberToDigits(int score, int dig[50], int &cnt)
{
    int aux = score;
    if (aux != 0)
    {
        while (aux)
        {
            dig[cnt] = aux % 10;
            aux /= 10;
            cnt++;
        }
    }
    else
    {
        dig[cnt] = 0;
        cnt++;
    }
}

struct Bullet
{
    float x, y;
    float dy;
    float dx;
    bool isActive;
    float mouseX;
};

struct Enemy
{
    int x, y;
    int plt;
};

struct Spring
{
    int x, y;
    int plt;
};

struct TempPlat
{
    int x, y;
    float startTime;
};

void renderBullets(std::vector<Bullet> &bullets, float dt, Sprite *bulletspr)
{
    for (int i = 0; i < bullets.size(); i++)
    {
        Bullet &bullet = bullets[i];
        if (bullet.isActive)
        {
            float angle = atan2(bullet.y, bullet.x - bullet.mouseX);
            bullet.y += bullet.dy * dt * sin(angle);
            bullet.x += bullet.dx * dt * cos(angle);
            drawSprite(bulletspr, bullet.x, bullet.y);
            if (bullet.y < 0)
            {
                bullet.isActive = false;
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }
    }
}

void bulletDestroysEnemy(std::vector<Bullet> &bullets, std::vector<Enemy> &enemies, int wEnemy, int hEnemy, int wBullet)
{
    for (int i = 0; i < bullets.size(); i++)
    {
        for (int j = 0; j < enemies.size(); j++)
        {
            if (bullets[i].x >= enemies[j].x && bullets[i].x - wBullet <= enemies[j].x + wEnemy && bullets[i].y >= enemies[j].y && bullets[i].y <= enemies[j].y + hEnemy)
            {
                enemies.erase(enemies.begin() + j);
                bullets.erase(bullets.begin() + i);
            }
        }
    }
}

void playerInteractWithEnemy(std::vector<Enemy> &enemies, float playerx, float playery, int wPlayer, int hPlayer, int wEnemy, int hEnemy, float dy, bool &play)
{
    for (int i = 0; i < enemies.size(); i++)
    {
        if (floor(playerx) >= enemies[i].x - wPlayer && floor(playerx) <= enemies[i].x + wEnemy)
        {
            if ((floor(playery + hPlayer) == enemies[i].y + 3 || floor(playery + hPlayer) == enemies[i].y + 1 || floor(playery + hPlayer) == enemies[i].y + 2 || floor(playery + hPlayer) == enemies[i].y + 5 || floor(playery + hPlayer) == enemies[i].y + 4 || floor(playery + hPlayer) == enemies[i].y + 6) && dy > 0)
            {
                enemies.erase(enemies.begin() + i);
            }
            else if ((floor(playery + hPlayer) == enemies[i].y + hEnemy + 3 || floor(playery + hPlayer) == enemies[i].y + hEnemy + 1 || floor(playery + hPlayer) == enemies[i].y + hEnemy + 2 || floor(playery + hPlayer) == enemies[i].y + hEnemy + 5 || floor(playery + hPlayer) == enemies[i].y + hEnemy + 4 || floor(playery + hPlayer) == enemies[i].y + hEnemy + 6) && dy < 0)
            {
                play = false;
            }
        }
    }
}

void playerInteractWithSpring(std::vector<Spring> &springs, float playerx, float playery, int wPlayer, int hPlayer, int wEnemy, int hEnemy, float dy, bool &springed, float &spring, float &startSpring)
{
    for (int i = 0; i < springs.size(); i++)
    {
        if (floor(playerx) >= springs[i].x - wPlayer && floor(playerx) <= springs[i].x + wEnemy)
        {
            if ((floor(playery + hPlayer) == springs[i].y + 3 || floor(playery + hPlayer) == springs[i].y + 1 || floor(playery + hPlayer) == springs[i].y + 2 || floor(playery + hPlayer) == springs[i].y + 5 || floor(playery + hPlayer) == springs[i].y + 4 || floor(playery + hPlayer) == springs[i].y + 6) && dy > 0)
            {
                springs.erase(springs.begin() + i);
                springed = true;
                spring = 1.5;
                startSpring = getTickCount();
            }
        }
    }
}

/* Test Framework realization */
class MyFramework : public Framework
{

private:
    bool started = false;
    bool play = true;
    bool facingRight = true;
    bool facingLeft = false;
    bool facingUp = false;
    bool springed = false;
    bool isMovingRight = false;
    bool isMovingLeft = false;
    bool goodForAgain = false;
    bool goodForStartGame = false;
    Sprite *lookRight = nullptr;
    Sprite *lookLeft = nullptr;
    Sprite *lookUp = nullptr;
    Sprite *background = nullptr;
    Sprite *block = nullptr;
    Sprite *backgroundlost = nullptr;
    Sprite *finish = nullptr;
    Sprite *playagain = nullptr;
    Sprite *digits[10];
    Sprite *bulletspr = nullptr;
    Sprite *nose = nullptr;
    Sprite *enemy = nullptr;
    Sprite *topscore = nullptr;
    Sprite *scoreWord = nullptr;
    Sprite *springBoots = nullptr;
    Sprite *platformTemp = nullptr;
    Sprite *playStart = nullptr;
    float playerx;
    float playery;
    float mousex, mousey;
    float dy = 0, grav = 500;
    float gap;
    int wPlatform, hPlatform, hPlayer, wPlayer, wButtonAgain, hButtonAgain;
    int a[8][2];
    int Width, Height;
    float dt;
    float spring;
    float lastTick;
    float startSpring;
    float stopSpring;
    int score = 0;
    int scorepx = 0;
    int dig1[20], dig2[20];
    int aux, cnt1 = 0, cnt2 = 0;
    int lifes = 5;
    int wEnemy, hEnemy, wBullet, hBullet, wDigit, hDigit, wSpring, hSpring, wPlayStart, hPlayStart;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    std::vector<Spring> springs;
    std::vector<TempPlat> tmplats;

public:
    virtual void PreInit(int &width, int &height, bool &fullscreen)
    {
        std::cout << "Enter Window Width and Height:";
        std::cin >> width;
        std::cin >> height;
        fullscreen = false;
    }

    virtual bool Init()
    {
        getScreenSize(Width, Height);
        playerx = Width / 2;
        playery = Height / 2;
        digits[0] = createSprite("assets/0.png");
        digits[1] = createSprite("assets/1.png");
        digits[2] = createSprite("assets/2.png");
        digits[3] = createSprite("assets/3.png");
        digits[4] = createSprite("assets/4.png");
        digits[5] = createSprite("assets/5.png");
        digits[6] = createSprite("assets/6.png");
        digits[7] = createSprite("assets/7.png");
        digits[8] = createSprite("assets/8.png");
        digits[9] = createSprite("assets/9.png");
        bulletspr = createSprite("assets/bullet.png");
        topscore = createSprite("assets/top_score.png");
        scoreWord = createSprite("assets/score.png");
        lookRight = createSprite("assets/blue-lik-right.png");
        lookLeft = createSprite("assets/blue-lik-left.png");
        lookUp = createSprite("assets/blue-lik-puca.png");
        nose = createSprite("assets/lik-njuska.png");
        block = createSprite("assets/block1.png");
        background = createSprite("assets/bck@2x.png");
        springBoots = createSprite("assets/spring.png");
        enemy = createSprite("assets/enemy.png");
        platformTemp = createSprite("assets/tmpPlat.png");
        playStart = createSprite("assets/play-on@2x.png");
        setSpriteSize(background, Width, Height);
        setSpriteSize(topscore, Width, Height / 15);
        finish = createSprite("assets/awesome.png");
        playagain = createSprite("assets/playagain.png");
        getSpriteSize(playagain, wButtonAgain, hButtonAgain);
        getSpriteSize(lookRight, wPlayer, hPlayer);
        getSpriteSize(block, wPlatform, hPlatform);
        getSpriteSize(enemy, wEnemy, hEnemy);
        getSpriteSize(bulletspr, wBullet, hBullet);
        getSpriteSize(digits[9], wDigit, hDigit);
        getSpriteSize(springBoots, wSpring, hSpring);
        getSpriteSize(playStart, wPlayStart, hPlayStart);
        int platformCount = 6;
        gap = (float)(Height / platformCount);
        generatePlatforms(a, gap, Width, Height, wPlatform);
        dt = 0;
        lastTick = getTickCount();
        lifes = 5;
        spring = 1;
        return true;
    }

    virtual void Close()
    {
    }

    virtual bool Tick()
    {
        if (started)
        {
            if (play == true)
            {
                dt = (getTickCount() - lastTick) / 1000.0f;
                if (dt > 0.015)
                    dt = 0.015;
                lastTick = getTickCount();
                drawSprite(background, 0, 0);
                drawSprite(topscore, 0, 0);
                drawSprite(scoreWord, 0, 0);
                for (int i = 1; i <= 6; i++)
                    drawSprite(block, a[i][0], a[i][1]);

                // score and scorepx using digits sprites
                //  score is number of platforms jumped
                //  scorepx is number of pixels jumped
                cnt1 = 0;
                numberToDigits(score, dig1, cnt1);

                for (int i = cnt1 - 1; i >= 0; i--)
                {
                    drawSprite(digits[dig1[i]], Width / 3 - i * 25, 10);
                }

                cnt2 = 0;
                numberToDigits(scorepx, dig2, cnt2);

                for (int i = cnt2 - 1; i >= 0; i--)
                {
                    drawSprite(digits[dig2[i]], Width - i * 25 - (wDigit + wDigit / 2), 10);
                }

                for (int i = 0; i < enemies.size(); i++)
                {
                    drawSprite(enemy, enemies[i].x, enemies[i].y);
                }

                for (int i = 0; i < springs.size(); i++)
                {
                    drawSprite(springBoots, springs[i].x, springs[i].y);
                }

                for (int i = 0; i < tmplats.size(); i++)
                {
                    drawSprite(platformTemp, tmplats[i].x, tmplats[i].y);
                }

                // hitting the platform
                // player jumps , and score is increased
                for (int i = 1; i <= 6; i++)
                {
                    if ((floor(playery + hPlayer) == a[i][1] + 3 || floor(playery + hPlayer) == a[i][1] + 1 || floor(playery + hPlayer) == a[i][1] + 2 || floor(playery + hPlayer) == a[i][1] + 5 || floor(playery + hPlayer) == a[i][1] + 4 || floor(playery + hPlayer) == a[i][1] + 6) && floor(playerx) >= a[i][0] - wPlayer + 25 && floor(playerx) <= a[i][0] + wPlatform - 20 && dy > 0)
                    {
                        dy = -(800) * spring;
                        score += 1;
                        scorepx += Height - a[i][1];
                    }
                }

                // jump from temporary platforms
                for (int i = 0; i < tmplats.size(); i++)
                {
                    if ((floor(playery + hPlayer) == tmplats[i].y + 3 || floor(playery + hPlayer) == tmplats[i].y + 1 || floor(playery + hPlayer) == tmplats[i].y + 2 || floor(playery + hPlayer) == tmplats[i].y + 5 || floor(playery + hPlayer) == tmplats[i].y + 4 || floor(playery + hPlayer) == tmplats[i].y + 6) && floor(playerx) >= tmplats[i].x - wPlayer + 25 && floor(playerx) <= tmplats[i].x + wPlatform - 20 && dy > 0)
                    {
                        dy = -(800) * spring;
                        score += 1;
                        scorepx += Height - a[i][1];
                    }
                }

                dy += grav * dt;
                playery += dy * dt;

                // some calculations for the jumping of the doodle, so it could work properly
                // on multiple devices
                if (dt < 0.002)
                {
                    if (dy * 0.002 > -0.7 && dy < 0)
                    {
                        dy = -dy;
                    }
                }
                else if (dt > 0.004)
                {
                    if (dy * 0.004 > -0.7 && dy < 0)
                    {
                        dy = -dy;
                    }
                }
                else
                {
                    if (dy * dt > -0.7 && dy < 0)
                    {
                        dy = -dy;
                    }
                }

                // TASK 2)
                //  spring boots ability last for 20 seconds, after that spring is set to 1,
                //  the jump speed being multiplied by spring it will come back to normal after setting it to 1
                //  and it will speed up when the spring boots is on
                if (springed == true)
                {
                    if ((getTickCount() - startSpring) / 1000 > 20)
                    {
                        springed = false;
                        spring = 1;
                        stopSpring = getTickCount();
                    }
                }

                // the algorithm for keeping the player at max the center of the screen
                // and making it seem like he is going up, and platforms down
                if (playery < Height / 2)
                {
                    // keeping player at max the center of the screen
                    playery = Height / 2;
                    for (int i = 1; i <= 6; i++)
                    {
                        // moving the platforms down
                        a[i][1] -= dy * dt;
                        if (a[i][1] > Height)
                        {
                            // erase the enemy from the paltform that got off the screen
                            for (int j = 0; j < enemies.size(); j++)
                                if (enemies[j].plt == i)
                                    enemies.erase(enemies.begin() + j);
                            // erase the spring from the paltform that got off the screen
                            for (int j = 0; j < springs.size(); j++)
                                if (springs[j].plt == i)
                                    springs.erase(springs.begin() + j);

                            score += 1;
                            a[i][1] = 0;
                            if (a[i - 1][0] > Width / 2)
                                a[i][0] = rand() % (Width / 2 - wPlatform);
                            else
                                a[i][0] = rand() % (Width / 2 - wPlatform) + Width / 2;

                            // TASK 3)
                            // random temporary 3 seconds PLATFORM
                            // it will be spawned randomly, it has color BLUE
                            int randPlatTemp = rand() % 999;
                            // 1% of time a platform will be created
                            if (randPlatTemp < 50)
                            {
                                TempPlat newPlatform = {rand() % (Width - wPlatform), a[i][1] + hPlatform + rand() % ((int)gap - 2 * hPlatform), getTickCount()};
                                tmplats.push_back(newPlatform);
                            }
                            int random = rand() % 999;
                            // 6% of time an enemy will be created
                            if (random < 60)
                            {
                                Enemy newEnemy = {a[i][0] + wPlatform / 2 - wEnemy / 2, a[i][1] - hEnemy, i};
                                enemies.push_back(newEnemy);
                            }

                            if ((getTickCount() - startSpring) / 1000 > 20 && (getTickCount() - stopSpring) / 1000 > 10)
                            {
                                int randomSpring = rand() % 999;
                                // 1% of time spring boots will be created
                                if (randomSpring < 60 && random > 60)
                                {
                                    Spring newSpring = {a[i][0] + wPlatform / 2 - wSpring / 2, a[i][1] - hSpring, i};
                                    springs.push_back(newSpring);
                                }
                            }
                        }
                        // enemy moves down with the platform it belongs to
                        for (int j = 0; j < enemies.size(); j++)
                            enemies[j].y = a[enemies[j].plt][1] - hEnemy;
                        // spring moves down with the platform it belongs to
                        for (int j = 0; j < springs.size(); j++)
                            springs[j].y = a[springs[j].plt][1] - hSpring;
                    }
                    // temp platforms move down with the platforms
                    for (int i = 0; i < tmplats.size(); i++)
                        tmplats[i].y -= dy * dt;
                }

                renderBullets(bullets, dt, bulletspr);

                bulletDestroysEnemy(bullets, enemies, wEnemy, hEnemy, wBullet);

                playerInteractWithEnemy(enemies, playerx, playery, wPlayer, hPlayer, wEnemy, hEnemy, dy, play);

                playerInteractWithSpring(springs, playerx, playery, wPlayer, hPlayer, wSpring, hSpring, dy, springed, spring, startSpring);

                // remove temp platforms out of the screen
                for (int i = 0; i < tmplats.size(); i++)
                {
                    if (tmplats[i].y > Height)
                        tmplats.erase(tmplats.begin() + i);
                    else if ((getTickCount() - tmplats[i].startTime) / 1000 > 3)
                        tmplats.erase(tmplats.begin() + i);
                }

                // if player is out of the screen then he appears on the other side
                if (playerx + wPlayer > Width)
                    playerx = 0;
                else if (playerx + wPlayer < 0)
                    playerx = Width - wPlayer;

                // left right speed
                if (isMovingRight)
                {
                    playerx += 300 * dt;
                }
                if (isMovingLeft)
                {
                    playerx -= 300 * dt;
                }

                // if shooting then doodle has the nose up
                if (!bullets.size())
                {
                    if (facingRight)
                        drawSprite(lookRight, playerx, playery);
                    else if (facingLeft)
                        drawSprite(lookLeft, playerx, playery);
                }
                // else he looks normal
                else
                {
                    drawSprite(lookUp, playerx, playery);
                    drawSprite(nose, playerx + wPlayer / 2.5, playery);
                }

                // TASK 1) each player has 5 lifes, if it falls down, it loses one life
                //  and it gets spawned on the lowest platform
                if (playery > Height - hPlayer)
                {
                    lifes--;
                    for (int i = 1; i <= 6; i++)
                    {
                        if (a[i][1] < Height && a[i][1] > Height - (Height / 4))
                        {
                            playery = a[i][1] - 2 * hPlayer;
                            playerx = a[i][0] + wPlatform / 2 - wPlayer / 2;
                        }
                    }
                }
                // if player loses all lifes, he lost, and after that he can play again
                if (lifes == 0)
                    play = false;
            }
            if (play == false)
            {
                // draw playagain screen
                drawSprite(background, 0, 0);
                drawSprite(topscore, 0, 0);
                drawSprite(scoreWord, 0, 0);
                drawSprite(finish, Width * 3 / 4, Height / 3);
                drawSprite(playagain, Width / 3, Height / 2);
                cnt1 = 0;
                numberToDigits(score, dig1, cnt1);

                for (int i = cnt1 - 1; i >= 0; i--)
                {
                    drawSprite(digits[dig1[i]], Width / 3 - i * 25, 10);
                }

                cnt2 = 0;
                numberToDigits(scorepx, dig2, cnt2);

                for (int i = cnt2 - 1; i >= 0; i--)
                {
                    drawSprite(digits[dig2[i]], Width - i * 25 - (wDigit + wDigit / 2), 10);
                }
            }
        }
        else
        {
            drawSprite(background, 0, 0);
            drawSprite(playStart, Width / 2 - wPlayStart / 2, Height / 2 - hPlayStart / 2);
        }
        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
    {
        if (x >= Width / 3 && x <= Width / 3 + wButtonAgain && y >= Height / 2 && y <= Height / 2 + hButtonAgain && play == false)
            goodForAgain = true;
        else
            goodForAgain = false;
        if (x >= Width / 2 - wPlayStart / 2 && x <= Width / 2 + wPlayStart && y >= Height / 2 - hPlayStart / 2 && y <= Height / 2 + hPlayStart / 2 && started == false)
            goodForStartGame = true;
        mousex = x;
        mousey = y;
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
    {
        if (button == FRMouseButton::LEFT && goodForStartGame == true && started == false)
            started = true;
        // play again
        if (button == FRMouseButton::LEFT && goodForAgain == true && play == false)
        {
            play = true;
            playerx = Width / 2;
            playery = Height / 2;
            bullets.clear();
            enemies.clear();
            springs.clear();
            tmplats.clear();
            dy = 0;
            grav = 500;
            score = 0;
            scorepx = 0;
            int platformCount = 6;
            float gap = (float)(Height / platformCount);
            generatePlatforms(a, gap, Width, Height, wPlatform);
            lifes = 5;
            spring = 1;
            springed = false;
        }
        // shoota
        if (button == FRMouseButton::LEFT && isReleased == true)
        {
            Bullet newBullet = {playerx, playery, -700, -700, true, mousex};
            bullets.push_back(newBullet);
        }
    }

    virtual void onKeyPressed(FRKey k)
    {
        if (k == FRKey::RIGHT)
        {
            facingRight = true;
            facingLeft = false;
            isMovingRight = true;
        }
        else if (k == FRKey::LEFT)
        {
            facingRight = false;
            facingLeft = true;
            isMovingLeft = true;
        }
    }

    virtual void onKeyReleased(FRKey k)
    {
        if (k == FRKey::RIGHT)
            isMovingRight = false;
        else if (k == FRKey::LEFT)
            isMovingLeft = false;
    }

    virtual const char *GetTitle() override
    {
        return "Bitica Project";
    }
};

int main(int argc, char *argv[])
{
    return run(new MyFramework);
}