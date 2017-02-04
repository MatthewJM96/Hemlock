#pragma once


#include <Event.hpp>
#include <ui\InputDispatcher.h>
#include <app\IApp.h>

class App : public happ::IApp {
public:
    void init();
    void operator()(h::Sender sender, hui::MouseWheelScrollEvent event);
protected:
    void prepareScreens();
private:
    void handleButtonDown(h::Sender sender, hui::MouseButtonEvent event);
    void handleMouseMovement(h::Sender sender, hui::MouseMoveEvent event) const;
    static void handleButtonUp(h::Sender sender, hui::MouseButtonEvent event);

    h::RPriorityEvent<i32>::Return rPriorityEventTest1(h::Sender sender);
    h::RPriorityEvent<i32>::Return rPriorityEventTest2(h::Sender sender);
    h::RPriorityEvent<i32>::Return rPriorityEventTest3(h::Sender sender);
    h::RPriorityEvent<i32>::Return rPriorityEventTest4(h::Sender sender);

    h::PriorityEvent<>::Return priorityEventTest1(h::Sender sender);
    h::PriorityEvent<>::Return priorityEventTest2(h::Sender sender);
    h::PriorityEvent<>::Return priorityEventTest3(h::Sender sender);
};
