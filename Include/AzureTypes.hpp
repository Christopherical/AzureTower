#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "AzureConfig.hpp"

namespace AzureTower
{
    enum Action
    {
        MoveLeft,
        MoveRight
    };

    struct Stats
    {
        int score;
    };

}