#include "raylib.h"
#include "background.h"
#include "control.h"
#include "character.h"
#include "settings.h"

int main(void) {
    
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(FPS);
    
    Character character("resources/scarfy.png", charPos);
    Background background("resources/cyberpunk_street_background.png");
    Background midground("resources/cyberpunk_street_midground.png");
    Background foreground("resources/cyberpunk_street_foreground.png");
    
    while (!WindowShouldClose()) {
        
        Control input = getInput();
        
        character.Update(input.direction, input.speed);
        background.Update(input.direction, input.speed, distanceBack);
        midground.Update(input.direction, input.speed, distanceMid);
        foreground.Update(input.direction, input.speed, distanceFore);
        
        BeginDrawing();

            background.Draw();
            midground.Draw();
            foreground.Draw();
            character.Draw(input.direction);
            DrawFPS(0, 0);   
        
        EndDrawing();

    }

    CloseWindow();

}