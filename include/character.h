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
            if (framesCounter >= (120 / framesSpeed))
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
                sourceRec.width *= -1;  // flip horizontally
            }

            DrawTextureRec(sprite, sourceRec, position, WHITE);
        }
};