#pragma once

#include <ui/InputDispatcher.h>
#include <app/IScreen.h>

class MainMenuScreen : public happ::IScreen {
public:
    void init(const char* name) override;
    void dispose() override;

    void update(TimeData time) override;
    void draw(TimeData time) override;

private:
};
