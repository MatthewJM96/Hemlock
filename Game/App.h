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
    void handleMouseButtonDown(h::Sender sender, hui::MouseButtonEvent event);
    void handleKeyboardButtonDown(h::Sender sender, hui::KeyboardButtonEvent event);
    void handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) const;
    static void handleMouseButtonUp(h::Sender sender, hui::MouseButtonEvent event);
};
