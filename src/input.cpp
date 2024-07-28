enum class KeyState{KeyDown, KeyUp, KeyRelease, KeyHold};
KeyState key_list[255];
KeyState mouse_button_list[32];
int mouse_x, mouse_y;

void pool_events(SDL_Event* ev, bool& app_running){
    while(SDL_PollEvent(ev) != 0){
        if(ev->type == SDL_QUIT){
            app_running = false;
        }

        switch(ev->type){
            case SDL_KEYDOWN:{
                u32 key_id = ev->key.keysym.sym;
                if(key_id < 255){
                    key_list[key_id] =  (key_list[key_id] == KeyState::KeyHold) ? KeyState::KeyHold : KeyState::KeyDown;
                }
                break;

            }
            case SDL_KEYUP:{
                u32 key_id = ev->key.keysym.sym;
                if(key_id < 255){
                    key_list[key_id] = KeyState::KeyUp;
                }
                break;
            }
            
            case SDL_MOUSEBUTTONDOWN: {
                u32 key_id = ev->button.button;
                if(key_id < 32){
                    mouse_button_list[key_id] =  (mouse_button_list[key_id] == KeyState::KeyHold) ? KeyState::KeyHold : KeyState::KeyDown;
                }
                

                }break;
            case SDL_MOUSEBUTTONUP: {
                u32 key_id = ev->button.button;

                if(key_id < 32){
                    mouse_button_list[key_id] = KeyState::KeyUp;
                }
                }break;
        }
    }
    
    SDL_GetMouseState(&mouse_x, &mouse_y);
}

IS_KEY_DOWN(is_key_down){
    return key_list[key_code] == KeyState::KeyDown;
}

IS_KEY_UP(is_key_up){
    return key_list[key_code] == KeyState::KeyUp;
}

IS_KEY_PRESS(is_key_press){
    return (key_list[key_code] == KeyState::KeyDown || key_list[key_code] == KeyState::KeyHold) ? true : false;
}

IS_MOUSE_BUTTON_DOWN(is_mouse_button_down){
    return mouse_button_list[key_code] == KeyState::KeyDown;
}

IS_MOUSE_BUTTON_UP(is_mouse_button_up){
    return mouse_button_list[key_code] == KeyState::KeyUp;
}

IS_MOUSE_BUTTON_PRESS(is_mouse_button_press){
    return (mouse_button_list[key_code] == KeyState::KeyDown || mouse_button_list[key_code] == KeyState::KeyHold) ? true : false;
}

void reset_input(){
    for(int i = 0; i < 255; i++){
        if(key_list[i] == KeyState::KeyDown)
            key_list[i] = KeyState::KeyHold;
        if(key_list[i] == KeyState::KeyUp)
            key_list[i] = KeyState::KeyRelease;
    }

    for(int i = 0; i < 32; i++){
        if(mouse_button_list[i] == KeyState::KeyDown)
            mouse_button_list[i] = KeyState::KeyHold;
        if(mouse_button_list[i] == KeyState::KeyUp)
            mouse_button_list[i] = KeyState::KeyRelease;
    }
}
