#pragma once


#include <Event.hpp>
#include <ui\InputDispatcher.h>
#include <app\IApp.h>

class App : public happ::IApp {
public:
    void init();
protected:
    void prepareScreens();
private:
    bool handleKeyboardButtonDown(h::Sender sender, hui::KeyboardButtonEvent event);
};
