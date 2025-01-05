class Background { 
    private:
        Texture2D texture;
        float scrolling = 0.0f;

    public:
        Background(const char* filePath) {
            texture = LoadTexture(filePath);
        }

        ~Background() {
            UnloadTexture(texture);
        }

        void Update(int direction, float speed, float distance){
            scrolling -= direction * (speed / distance);
            if (scrolling <= -texture.width * 2) {
                scrolling += texture.width * 2;
            }
            else if (scrolling >= texture.width * 2) {
                scrolling -= texture.width * 2;
            }

        }

        void Draw() {
            DrawTextureEx(texture, {scrolling - texture.width * 2, 0}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(texture, {scrolling, 0}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(texture, {scrolling + texture.width * 2, 0}, 0.0f, 2.0f, WHITE);
        }
};