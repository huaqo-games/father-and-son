struct Control {
    int direction;
    float speed;
};

Control getInput() {
    int direction = 1;
    float speed = 0.0f;

    if (IsKeyDown(KEY_RIGHT)) {
        speed = 30.0f;
        direction = 1;
    } else if (IsKeyDown(KEY_LEFT)) {
        speed = 30.0f;
        direction = -1;
    } else {
        speed = 0.0f;
        direction = 0;
    }
    
    Control input;
    input.direction = direction;
    input.speed = speed;
    return input;
}