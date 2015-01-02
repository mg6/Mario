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
    Tile at = map->GetTile(Map::ToTile(pos_x), Map::ToTile(pos_y)-1);
    Tile next = map->GetTile(Map::ToTile(pos_x + dir_x * dt), Map::ToTile(pos_y)-1);

    if (Map::IsSolidTile(at) && !Map::IsSolidTile(next))
        dir_x *= -1;

    Object::OnUpdate(dt);
}
