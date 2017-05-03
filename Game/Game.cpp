#include "stdafx.h"

#include <Hemlock.hpp>

#include "App.h"

int main(int argc, char* argv[]) {
    h::init();
    
    App app;
    app.run();

    h::dispose();

    return 0;
}
