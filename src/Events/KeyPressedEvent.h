#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"
#include <SDL2/SDL.h>

class KeyPressedEvent: public Event {
    public:
        SDL_Keycode symbol;
        SDL_Event keyevent;
        KeyPressedEvent(SDL_Keycode symbol, SDL_Event keyevent): symbol(symbol), keyevent(keyevent) {}
};

#endif