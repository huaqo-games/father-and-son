#include <raylib.h>

class Player
{
private:
    Vector2 position = {0, 0};
    Vector2 spriteSize = {1.0f, 1.0f};
    Texture2D texture = {0};
    Rectangle frameRec = {0.0f, 0.0f, 0.0f, 0.0f};
    float scale = 1.0f;
    bool loaded = false;
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;  

public:
    Player() = default;

    Player(const char *path, Vector2 textureGrid)
    {
        load(path, textureGrid);
    }

    void load(const char *path, Vector2 textureGrid)
    {
        texture = LoadTexture(path);
        spriteSize.x = texture.width / textureGrid.x;
        spriteSize.y = texture.height / textureGrid.y;
        frameRec = {0.0f, 0.0f, spriteSize.x, spriteSize.y};
        scale = GetScreenHeight() / texture.height / 2.0f;
        loaded = true;
        position = {0,0};
    }

    ~Player()
    {
        if (loaded)
        {
            UnloadTexture(texture);
        }
    }

    void update()
    {
        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5) currentFrame = 0;

            frameRec.x = (float)currentFrame * spriteSize.x;
        }

        if (IsKeyDown(KEY_RIGHT)){
            position.x += 2.0f;
            frameRec.width = spriteSize.x;
        }

        if (IsKeyDown(KEY_LEFT)){
            position.x -= 2.0f;
            frameRec.width = -spriteSize.x;
        }
        if (IsKeyDown(KEY_DOWN)){
            position.y += 2.0f;
        }
        if (IsKeyDown(KEY_UP)){
            position.y -= 2.0f;
        }

        scale = GetScreenHeight() / texture.height / 2.0f;
        
    }

    void draw()
    {
        if (loaded)
        {
            Rectangle dest = {position.x, position.y, spriteSize.x * scale, spriteSize.y * scale};
            Vector2 origin = {0.0f, 0.0f};
            DrawTexturePro(texture, frameRec, dest, origin, 0.0f, WHITE);
        }
    }


    Vector2 getPosition() { return position; }
    Vector2 getSpriteSize() { return spriteSize; }
    float getScale() { return scale;}
};

///////////////////////////////////////////////////////////////////////////

class Background
{
private:
    Texture2D texture = {0};
    float scrolling = 0.0f;
    float scale = 1.0f;
    bool loaded = false;

public:
    Background() = default;

    Background(const char *path) { load(path); }

    void load(const char *path)
    {
        texture = LoadTexture(path);
        scale = GetScreenHeight() / texture.height;
        loaded = true;
    }

    ~Background()
    {
        if (loaded)
        {
            UnloadTexture(texture);
        }
    }

    void update()
    {
        // updateScrolling();
        scale = GetScreenHeight() / texture.height;

    }

    void draw()
    {
        if (!loaded)
            return;
        for (int i = -1; i <= 1; i++)
        {
            DrawTextureEx(texture, {scrolling + i * texture.width * scale, 0.0f}, 0.0f, scale, WHITE);
        }
    }
};

///////////////////////////////////////////////////////////////////////////

class CameraController
{
private:
    Camera2D camera;
    Vector2 center;
    Vector2 targetOffset;
    float transitionSpeed = 0.01f; // Speed of transition

public:

    void initialize(Vector2 position, Vector2 spriteSize)
    {
        center = {(GetScreenWidth() / 2.0f) - (spriteSize.x / 2.0f), (GetScreenHeight() / 2.0f) - (spriteSize.y / 2.0f)};
        camera = {
            .offset = center,
            .target = position,
            .rotation = 0.0f,
            .zoom = 1.0f};
        targetOffset = camera.offset;
    }

    void update(Vector2 position, Vector2 spriteSize, float scale)
    {
        if (IsKeyDown(KEY_RIGHT))
            targetOffset = {(GetScreenWidth() / 2.0f) - (spriteSize.x * scale / 2.0f) - (GetScreenWidth() / 4.0f), (GetScreenHeight() / 2.0f) - (spriteSize.y * scale / 2.0f)};
        if (IsKeyDown(KEY_LEFT))
            targetOffset = {(GetScreenWidth() / 2.0f) - (spriteSize.x * scale / 2.0f) + (GetScreenWidth() / 4.0f), (GetScreenHeight() / 2.0f) - (spriteSize.y * scale / 2.0f)};
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN))
            targetOffset = center; // Reset to center for vertical movement or no horizontal input.

        // Smooth transition using LERP
        camera.offset.x += (targetOffset.x - camera.offset.x) * transitionSpeed;
        camera.offset.y += (targetOffset.y - camera.offset.y) * transitionSpeed;

        camera.target = position;
    }

    Camera2D getCamera() const { return camera; }
};

///////////////////////////////////////////////////////////////////////////

class Game
{
private:
    Player player;
    Background background;
    Background midground;
    Background foreground;
    CameraController camera;

public:
    Game() = default;

    void initialize()
    {
        player.load("assets/character.png", {4.0f, 12.0f});
        // background.load("assets/background.png");
        // midground.load("assets/midground.png");
        // foreground.load("assets/foreground.png");
        camera.initialize(player.getPosition(),player.getSpriteSize());
    }

    void update()
    {
        player.update();
        camera.update(player.getPosition(),player.getSpriteSize(), player.getScale());
        // background.update();

    }

    void render()
    {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera.getCamera());
        background.draw();
        midground.draw();
        foreground.draw();
        player.draw();
        EndMode2D();
        drawDebug();
        EndDrawing();
    }

    void shutdown()
    {
        CloseWindow();
    }

private:

    void drawDebug (){
        DrawCircle(camera.getCamera().offset.x,camera.getCamera().offset.y,20,RED);
        DrawText(TextFormat("camera.offset.x: %f",camera.getCamera().offset.x),0,0,20,WHITE);
        DrawText(TextFormat("camera.offset.y: %f",camera.getCamera().offset.y),0,20,20,WHITE);

        DrawCircle(camera.getCamera().target.x,camera.getCamera().target.y,30,BLUE);
        DrawText(TextFormat("camera.target.x: %f",camera.getCamera().target.x),0,40,20,WHITE);
        DrawText(TextFormat("camera.target.y: %f",camera.getCamera().target.y),0,60,20,WHITE);

        DrawCircle(player.getPosition().x,player.getPosition().y,20,GREEN);
        DrawText(TextFormat("player.position.x: %f",player.getPosition().x),0,80,20,WHITE);
        DrawText(TextFormat("player.position.y: %f",player.getPosition().y),0,100,20,WHITE);
    }

};

///////////////////////////////////////////////////////////////////////////

int main()
{
    InitWindow(1280, 720, "Father and Son");
    SetTargetFPS(60);
    Game game;
    game.initialize();
    while (!WindowShouldClose())
    {
        game.update();
        game.render();
    }
    game.shutdown();
    return 0;
}
