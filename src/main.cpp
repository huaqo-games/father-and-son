#include "raylib.h"
#include <cmath>   // Changed to <cmath> for consistency

// Screen settings
int screenWidth = 1280;
int screenHeight = 720;
const char *title = "Father and son";
int FPS = 60;

// Parallax distances
float distanceBack = 40.0f;
float distanceMid  = 20.0f;
float distanceFore = 10.0f;

// Character positioning
float charSpriteHeight = 256.0f;
float bottomSpace       = 64.0f;
Vector2 charPos         = {
    static_cast<float>(screenWidth) / 2.0f - 128.0f,
    static_cast<float>(screenHeight) - (256.0f + 64.0f)
};

// Dialog & font
int fontSize = 20;
int steps    = 0;

// Tints
Color backgroundTint = {171, 157, 146, 100}; 
Color midgroundTint  = {171, 157, 146, 130}; 
Color foregroundTint = {171, 157, 146, 150}; 
Color characterTint  = {171, 157, 146, 100}; 

// Dialog struct and list
typedef struct {
    const char *text;
    int next; 
} Dialog;

Dialog dialogs[] = {
    {"Son: Father?",               1},
    {"Father: Everything ok?",     2},
    {"Son: When do we reach the coast?", 3},
    {"Father: It will take some time.",  4},
    {"Son: I am tired.",          -1}
};

const char *continueDialog = "[Press SPACE to continue]";

// Simple control structure for movement
struct Control {
    int direction;
    float speed;
};

// Get input function
Control getInput() {
    int direction = 1;
    float speed   = 0.0f;

    if (IsKeyDown(KEY_RIGHT)) {
        speed     = 7.5f;
        direction = 1;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        speed     = 7.5f;
        direction = -1;
    }
    else {
        speed     = 0.0f;
        direction = 1;
    }
    
    Control input;
    input.direction = direction;
    input.speed     = speed;
    return input;
}

// Character class
class Character {
private:
    Texture2D sprite;
    Rectangle frameRec;
    Vector2 position;
    int framesCounter;
    int currentFrame;
    int framesSpeed;

public:
    Character(const char* filePath, Vector2 pos) {
        sprite        = LoadTexture(filePath);
        position      = pos;
        frameRec      = {0.0f, 0.0f, static_cast<float>(sprite.width) / 6, static_cast<float>(sprite.height)};
        framesCounter = 0;
        currentFrame  = 0;
        framesSpeed   = 0;
    }

    ~Character() {
        UnloadTexture(sprite);
    }

    void Update(int direction, float speed) {
        if (direction != 0) {
            framesSpeed = 30;
        }
        else {
            framesSpeed  = 0;
            currentFrame = 4; // Idle frame
        }
        
        framesCounter++;

        // Advance frame if enough time (frames) has passed
        if (framesSpeed > 0 && framesCounter >= (480 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame > 5) currentFrame = 0;
            frameRec.x = static_cast<float>(currentFrame) * (sprite.width / 6.0f);
        }
    }

    void Draw(int direction) {
        Rectangle sourceRec = frameRec;
        // Flip horizontally if moving left
        if (direction == -1) {
            sourceRec.width *= -1;
        }
        DrawTextureRec(sprite, sourceRec, position, WHITE);
    }
};

// Background class
class Background { 
private:
    Texture2D texture;
    float scrolling = 0.0f;
    float scale;

public:
    Background(const char* filePath, float initialScale = 1.0f) {
        texture = LoadTexture(filePath);
        scale   = initialScale;
    }

    ~Background() {
        UnloadTexture(texture);
    }

    Texture2D GetTexture() const {
        return texture;
    }

    void Update(int direction, float speed, float distance) {
        scrolling -= direction * (speed / distance);
        float textureWidth = static_cast<float>(texture.width) * scale;

        // Wrap scrolling
        if (scrolling <= -textureWidth) {
            scrolling += textureWidth;
        }
        else if (scrolling >= textureWidth) {
            scrolling -= textureWidth;
        }
    }

    void Draw() {
        float textureWidth = static_cast<float>(texture.width) * scale;

        DrawTextureEx(texture, {scrolling - textureWidth, 0}, 0.0f, scale, WHITE);
        DrawTextureEx(texture, {scrolling,               0}, 0.0f, scale, WHITE);
        DrawTextureEx(texture, {scrolling + textureWidth,0}, 0.0f, scale, WHITE);
    }

    void SetScale(float newScale) {
        scale = newScale;
    }
};

// Helper to scale backgrounds to fill the screen
void AdjustBackgroundScale(Background& bg) {
    Texture2D texture = bg.GetTexture();
    float scaleX      = static_cast<float>(GetScreenWidth())  / texture.width;
    float scaleY      = static_cast<float>(GetScreenHeight()) / texture.height;
    bg.SetScale(fmaxf(scaleX, scaleY));
}

int main(void) {
    // Initialize window
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(FPS);

    // Load the custom radial-light shader
    Shader radialLightShader = LoadShader(NULL, "shaders/radial_light.fs");
    int lightPosLoc  = GetShaderLocation(radialLightShader, "lightPos");
    int radiusLoc    = GetShaderLocation(radialLightShader, "radius");
    int intensityLoc = GetShaderLocation(radialLightShader, "intensity");

    // Example values
    float torchRadius    = 1200.0f;   
    float torchIntensity = 1.0f; 

    // Create a render texture for the entire screen
    RenderTexture2D mainTarget = LoadRenderTexture(screenWidth, screenHeight);

    // Audio setup
    InitAudioDevice();
    Music music = LoadMusicStream("assets/soundtrack.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.0f);

    // Create character and backgrounds
    Character character("assets/father-and-son.png", charPos);
    Background background("assets/background.png");
    Background midground("assets/midground.png");
    Background foreground("assets/foreground.png");

    // Scale backgrounds
    AdjustBackgroundScale(background);
    AdjustBackgroundScale(midground);
    AdjustBackgroundScale(foreground);

    // Dialog variables
    int currentDialog = 0;  
    bool dialogActive = true;

    // Main loop
    while (!WindowShouldClose()) {
        // Update music
        UpdateMusicStream(music);

        // Get input
        Control input = getInput();
        
        // Simple step counter
        if (input.direction == 1 && input.speed == 7.5f) {
            steps++;
        }

        // Decrease torch radius slightly every frame
        if (torchRadius > 1.0f) {
            torchRadius -= 1.0f;
            torchIntensity -= 0.001f;
        }
        else {
            torchRadius = 0.01f;
            torchIntensity = 0.001f;

        }

        // Update character & backgrounds
        character.Update(input.direction, input.speed);
        background.Update(input.direction, input.speed, distanceBack);
        midground.Update(input.direction, input.speed, distanceMid);
        foreground.Update(input.direction, input.speed, distanceFore);

        // 1) Draw entire scene to the offscreen texture
        BeginTextureMode(mainTarget);
            BeginDrawing();
                ClearBackground(BLACK);
                
                // Draw backgrounds with tints
                background.Draw();
                DrawRectangle(0, 0, screenWidth, screenHeight, backgroundTint);

                midground.Draw();
                DrawRectangle(0, 0, screenWidth, screenHeight, midgroundTint);

                foreground.Draw();
                DrawRectangle(0, 0, screenWidth, screenHeight, foregroundTint);

                // Draw character with an overall tint
                character.Draw(input.direction);
                DrawRectangle(0, 0, screenWidth, screenHeight, characterTint);
            EndDrawing();
        EndTextureMode();

        // 2) Draw the offscreen texture with the radial-light shader
        BeginDrawing();
            ClearBackground(BLACK);

            // Torch position offset 
            Vector2 torchPos;
            if (input.direction == 1) {
                torchPos = {charPos.x + 219.0f, charPos.y - 120.0f};
            } else {
                // If facing left, offset differently
                torchPos = {charPos.x + (256.0f - 219.0f), charPos.y - 120.0f};
            }

            // Pass uniforms to shader
            SetShaderValue(radialLightShader, lightPosLoc,  &torchPos,       SHADER_UNIFORM_VEC2);
            SetShaderValue(radialLightShader, radiusLoc,    &torchRadius,    SHADER_UNIFORM_FLOAT);
            SetShaderValue(radialLightShader, intensityLoc, &torchIntensity, SHADER_UNIFORM_FLOAT);

            BeginShaderMode(radialLightShader);
                DrawTextureRec(
                    mainTarget.texture,
                    Rectangle{
                        0.0f,
                        0.0f,
                        static_cast<float>(mainTarget.texture.width),
                        static_cast<float>(-mainTarget.texture.height)
                    },
                    Vector2{0.0f, 0.0f},
                    WHITE
                );
            EndShaderMode();

            // Dialog handling
            if (dialogActive && IsKeyPressed(KEY_SPACE)) {
                if (dialogs[currentDialog].next == -1) {
                    dialogActive = false;
                } else {
                    currentDialog = dialogs[currentDialog].next;
                }
            }

            int textWidthDialog         = MeasureText(dialogs[currentDialog].text, fontSize);
            int textWidthContinueDialog = MeasureText(continueDialog, fontSize);

            // Only show dialog text if the torch has almost gone out
            if (torchRadius < 1.0f) {
                DrawText(
                    dialogs[currentDialog].text,
                    (screenWidth / 2) - (textWidthDialog / 2),
                    screenHeight / 2,
                    fontSize,
                    WHITE
                );
                DrawText(
                    continueDialog,
                    (screenWidth / 2) - (textWidthContinueDialog / 2),
                    screenHeight - 50,
                    fontSize,
                    DARKGRAY
                );
            }

            // Optional debug: DrawFPS(10, 10)
        EndDrawing();
    }

    // Unload resources
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
