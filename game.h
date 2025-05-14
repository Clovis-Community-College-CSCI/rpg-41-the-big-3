#pragma once
#include "actor.h"
#include "inventory.h"
#include "map.h"
#include <vector>
#include <algorithm>

void startGame(Hero* player, InventoryBST& inventory);
void startCombat(Hero* player, Monster* monster, WINDOW* win);

