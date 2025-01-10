#include "raylib.h"
#include "cmath"

int screenWidth = 1280;
int screenHeight = 720;
const char *title = "Father and son";
int FPS = 60;
float distanceBack = 40.0f;
float distanceMid = 20.0f;
float distanceFore = 10.0f;
float charSpriteHeight = 256.0f;
float bottomSpace = 64.0f;
Vector2 charPos = {screenWidth / 2.0f - 128.0f, screenHeight - (charSpriteHeight + bottomSpace)};
int fontSize = 20;


Color backgroundTint = {171, 157, 146, 100}; 
Color midgroundTint = {171, 157, 146, 130}; 
Color foregroundTint = {171, 157, 146, 150}; 
Color characterTint = {171, 157, 146, 100}; 




typedef struct {
    const char *text;
    int next; 
} Dialog;

Dialog dialogs[] = {
    {"Son: Father?", 1},
    {"Father: Everything ok?", 2},
    {"Son: When do we reach the coast?", 3},
    {"Father: It will take some time.", 4},
    {"Son: I am tired.", -1}
};

const char * continueDialog = "[Press SPACE to continue]";

struct Control {
    int direction;
    float speed;
};

Control getInput() {
    int direction = 1;
    float speed = 0.0f;

    if (IsKeyDown(KEY_RIGHT)) {
        speed = 7.5f;
        direction = 1;
    } else if (IsKeyDown(KEY_LEFT)) {
        speed = 7.5f;
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
            sprite = LoadTexture(filePath);
            position = pos;
            frameRec = { 0.0f, 0.0f, (float)sprite.width / 6, (float)sprite.height };
            framesCounter = 0;
            currentFrame = 0;
            framesSpeed = 0;
        }

        ~Character() {
            UnloadTexture(sprite);
        }

        void Update(int direction, float speed) {
            if (direction != 0) {
                framesSpeed = 30;
            } 
            else {
                framesSpeed = 0;
                currentFrame = 4;
            }
            
            framesCounter++;
            
            if (framesCounter >= (480 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 5)
                    currentFrame = 0;

                frameRec.x = (float)currentFrame * (float)sprite.width / 6;
            }
        }

        void Draw(int direction) {
            Rectangle sourceRec = frameRec;

            if (direction == -1) {
                sourceRec.width *= -1;
            }

            DrawTextureRec(sprite, sourceRec, position, WHITE);
        }
};

class Background { 
    private:
        Texture2D texture;
        float scrolling = 0.0f;
        float scale;

    public:
        Background(const char* filePath, float initialScale = 1.0f) {
            texture = LoadTexture(filePath);
            scale = initialScale;
        }

        ~Background() {
            UnloadTexture(texture);
        }

        Texture2D GetTexture() const {
            return texture;
        }

        void Update(int direction, float speed, float distance) {
            scrolling -= direction * (speed / distance);
            float textureWidth = texture.width * scale;

            if (scrolling <= -textureWidth) {
                scrolling += textureWidth;
            }
            else if (scrolling >= textureWidth) {
                scrolling -= textureWidth;
            }
        }

        void Draw() {
            float textureWidth = texture.width * scale;

            DrawTextureEx(texture, {scrolling - textureWidth, 0}, 0.0f, scale, WHITE);
            DrawTextureEx(texture, {scrolling, 0}, 0.0f, scale, WHITE);
            DrawTextureEx(texture, {scrolling + textureWidth, 0}, 0.0f, scale, WHITE);
        }

        void SetScale(float newScale) {
            scale = newScale;
        }
};

void AdjustBackgroundScale(Background& bg) {
    Texture2D texture = bg.GetTexture();
    float scaleX = (float)GetScreenWidth() / texture.width;
    float scaleY = (float)GetScreenHeight() / texture.height;
    bg.SetScale(fmaxf(scaleX, scaleY));
}

int main(void) {
    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(FPS);

    // if (!IsWindowFullscreen()) {
    //     ToggleFullscreen();
    // }

    InitAudioDevice();
    Music music = LoadMusicStream("assets/soundtrack.wav");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.0f);
    
    Character character("assets/father-and-son.png", charPos);
    Background background("assets/background.png");
    Background midground("assets/midground.png");
    Background foreground("assets/foreground.png");

    AdjustBackgroundScale(background);
    AdjustBackgroundScale(midground);
    AdjustBackgroundScale(foreground);

    int currentDialog = 0;  
    bool dialogActive = true;

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

        Control input = getInput();

        character.Update(input.direction, input.speed);
        background.Update(input.direction, input.speed, distanceBack);
        midground.Update(input.direction, input.speed, distanceMid);
        foreground.Update(input.direction, input.speed, distanceFore);


        if (dialogActive && IsKeyPressed(KEY_SPACE)) {
            if (dialogs[currentDialog].next == -1) {
                dialogActive = false; 
            } else {
                currentDialog = dialogs[currentDialog].next;
            }
        }

        int textWidthDialog = MeasureText(dialogs[currentDialog].text, fontSize);
        int textWidthContinueDialog = MeasureText(continueDialog, fontSize);


        BeginDrawing();

            background.Draw();
            DrawRectangle(0, 0, screenWidth, screenHeight, backgroundTint); 
            midground.Draw();
            DrawRectangle(0, 0, screenWidth, screenHeight, midgroundTint); 
            foreground.Draw();
            DrawRectangle(0, 0, screenWidth, screenHeight, foregroundTint); 
            
            character.Draw(input.direction);
            DrawRectangle(0, 0, screenWidth, screenHeight, characterTint); 
            
            DrawText(dialogs[currentDialog].text, (screenWidth / 2) - (textWidthDialog / 2) , screenHeight / 2, fontSize, BLACK);
            DrawText(continueDialog, (screenWidth / 2) - (textWidthContinueDialog / 2), screenHeight - 50, fontSize, DARKGRAY);
            
            DrawFPS(0, 0);

        EndDrawing();
    }

    UnloadMusicStream(music);

    CloseWindow();
}
