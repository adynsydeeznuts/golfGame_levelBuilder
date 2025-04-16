#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Line.h"
#include "EdgeDetection.h"

using namespace std;

extern std::vector<Line> ExtractEdgesFromSprite(Image img, int threshold);

class Obstacle {
private:
    Texture2D texture;                  // The sprite image
    Vector2 position;                   // Position (center of sprite)
    float rotation;                     // Rotation in degrees
    float scale;                        // Scale factor
    vector<Line> collisionLines;  // Collision lines in local space
    
public:
    // Default constructor
    Obstacle() : texture{0}, position{0, 0}, rotation{0}, scale{1.0f} {}
    
    // Constructor with parameters
    Obstacle(const char* fileName, Vector2 pos, float rot = 0.0f, float sc = 1.0f) {
        const Image img = LoadImage(fileName);
        texture = LoadTexture(fileName);
        position = pos;
        rotation = rot;
        scale = sc;
        
        // Extract collision lines
        
        collisionLines = ExtractEdgesFromSprite(img, 0);
        UnloadImage(img);
    }

    // Getters and setters
    Texture2D GetTexture() const { return texture; }
    Vector2 GetPosition() const { return position; }    
    float GetRotation() const { return rotation; }
    float GetScale() const { return scale; }

    vector<Line> GetCollisionLines() const {
        vector<Line> worldLines;
        worldLines.reserve(collisionLines.size());
        
        for (const auto& localLine : collisionLines) {
            worldLines.push_back(TransformLine(localLine));
        }
        
        return worldLines;
    }
    
    void SetTexture(Texture2D tex) { texture = tex; }
    void SetPosition(Vector2 pos) { position = pos; }
    void SetRotation(float rot) { rotation = rot; }
    void SetScale(float sc) { scale = sc; }

    void SetCollisionLines(const vector<Line>& lines) {
        collisionLines = lines;
    }
    
    // Destructor to clean up resources
    ~Obstacle() {
        UnloadTexture(texture);
    }
    
    // Transform a collision line from local to world space
    Line TransformLine(const Line& line) const {
        Line transformed(line.getPoint1(), line.getPoint2());
        transformed.setPoint1(TransformPoint(line.getPoint1()).x, TransformPoint(line.getPoint1()).y);
        transformed.setPoint2(TransformPoint(line.getPoint2()).x, TransformPoint(line.getPoint2()).y);
        return transformed;
    }
    
    // Transform a point from local to world space
    Vector2 TransformPoint(Vector2 point) const {
        // Scale
        Vector2 scaled = {point.x * scale, point.y * scale};
        
        // Rotate
        float rad = rotation * DEG2RAD;
        float cosRot = cosf(rad);
        float sinRot = sinf(rad);
        Vector2 rotated = {
            scaled.x * cosRot - scaled.y * sinRot,
            scaled.x * sinRot + scaled.y * cosRot
        };
        
        // Translate
        return Vector2Add(rotated, position);
    }

    
    
    // Draw the obstacle
    void Draw() const {
        // Draw the sprite
        DrawTexturePro(
            texture,
            (Rectangle){0, 0, (float)texture.width, (float)texture.height},
            (Rectangle){
                position.x, 
                position.y, 
                texture.width * scale, 
                texture.height * scale
            },
            (Vector2){texture.width * scale / 2, texture.height * scale / 2},
            rotation,
            WHITE
        );
    }
    
    // Draw collision lines (for debugging)
    void DrawCollisionLines() const {
        for (const auto& localLine : collisionLines) {
            Line worldLine = TransformLine(localLine);
            DrawLineEx(worldLine.getPoint1(), worldLine.getPoint2(), 1.0f, RED);
        }
    }
};