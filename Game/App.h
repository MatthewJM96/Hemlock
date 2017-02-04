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
    void handleButtonDown(h::Sender sender, hui::MouseButtonEvent event);
    void handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) const;
    static void handleButtonUp(h::Sender sender, hui::MouseButtonEvent event);
};
