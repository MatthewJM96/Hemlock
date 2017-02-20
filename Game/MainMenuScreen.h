#pragma once

#include <ui\InputDispatcher.h>
#include <app\IScreen.h>

class MainMenuScreen : public happ::IScreen {
public:
    void init(char* name);
    void dispose();

    void update(TimeData time);
    void draw(TimeData time);

private:
};
