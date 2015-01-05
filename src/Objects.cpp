/**
 * @file        Objects.cpp
 * @author      mg6 (maciej@gamrat.it)
 * @date        2014-2015
 * @copyright   GNU GPL v2
 */

#include "Objects.hpp"
#include "Map.hpp"
#include "Game.hpp"

using namespace Mario;

void Object::Kill(Object* enemy)
{
    if (enemy->state & STATE_ALIVE)
    {
        enemy->state &= ~STATE_ALIVE;
        map->game->OnKill(this, enemy);
    }
}

void Object::OnUpdate(float dt)
{
    // movement
    if (state & STATE_FALL)
        dir_y += -Gravity * dt;
    else if (dir_y != 0)
        dir_y = 0;

    pos_x += dir_x * dt;
    pos_y += dir_y * dt;

    // animation
    anim_timer += dt;

    if (anim_timer > 0.1)
    {
        anim_timer = 0;
        ++frame;
    }
}

void Enemy::OnCollision(Object* spawn)
{
    if (Player* player = dynamic_cast<Player*>(spawn))
        Kill(player);
}

void Enemy::OnUpdate(float dt)
{
    int tile_x = Map::ToTile(pos_x);
    int tile_y = Map::ToTile(pos_y);
    int tile_n = Map::ToTile(pos_x + dir_x * dt);

    // check if standing on solid tile
    if (Map::IsSolidTile(map->GetTile(tile_x, tile_y - 1)))
        // check if at edge or wall
        if (!Map::IsSolidTile(map->GetTile(tile_n, tile_y - 1)) ||
            Map::IsSolidTile(map->GetTile(tile_n, tile_y)))
            // change direction
            dir_x *= -1;

    Object::OnUpdate(dt);
}

size_t Goomba::OnDraw()
{
    if (!(state & STATE_ALIVE))
        return 2;

    return frame % 4 >= 2;
}

size_t Koopa::OnDraw()
{
    return (dir_x < 0 ? 0 : 2) + (frame % 4 >= 2);
}

size_t Spiny::OnDraw()
{
    return (dir_x < 0 ? 0 : 2) + (frame % 4 >= 2);
}