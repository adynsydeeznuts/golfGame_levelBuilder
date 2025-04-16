#ifndef EDGE_DETECT_H
#define EDGE_DETECT_H

#include <raylib.h>
#include <vector>
#include <raymath.h>
#include <iostream>
#include <algorithm>

#include "Line.h"

using namespace std;

// Extract edges from a sprite using a basic threshold method
// Add this function to trace the outline of a shape with marching squares
vector<Line> ExtractEdgesFromSprite(Image img, int threshold) {
    vector<Vector2> boundaryPoints;
    vector<Line> edges;
    
    // Access the image data directly
    Color* pixels = LoadImageColors(img);
    int width = img.width;
    int height = img.height;
    
    // First, create a binary grid representing transparent vs. non-transparent pixels
    vector<vector<bool>> grid(height, vector<bool>(width, false));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            grid[y][x] = (pixels[idx].a > threshold);
        }
    }
    
    // Find the first boundary pixel (non-transparent with at least one transparent neighbor)
    int startX = -1, startY = -1;
    for (int y = 0; y < height && startX < 0; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x]) {  // Non-transparent pixel
                // Check 4-connected neighbors
                const int dx[4] = {1, 0, -1, 0};
                const int dy[4] = {0, 1, 0, -1};
                
                for (int i = 0; i < 4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        if (!grid[ny][nx]) {  // Transparent neighbor
                            startX = x;
                            startY = y;
                            break;
                        }
                    } else {
                        // Edge of image counts as transparent
                        startX = x;
                        startY = y;
                        break;
                    }
                }
                
                if (startX >= 0) break;
            }
        }
    }
    
    if (startX < 0) {  // No boundary found
        UnloadImageColors(pixels);
        return edges;
    }
    
    // Moore-Neighbor tracing algorithm
    int currentX = startX;
    int currentY = startY;
    int startDir = 7;  // Start checking from the top-right direction
    
    // Direction deltas (clockwise order):
    // 0=E, 1=SE, 2=S, 3=SW, 4=W, 5=NW, 6=N, 7=NE
    const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    
    vector<Vector2> contour;
    int maxIterations = width * height;  // Safety limit
    int iterations = 0;
    
    do {
        contour.push_back({(float)currentX, (float)currentY});
        
        // Find the next boundary pixel by checking neighbors in clockwise order
        int dir = (startDir + 6) % 8;  // Start from 2 positions counter-clockwise
        bool found = false;
        
        for (int i = 0; i < 8; i++) {
            int nextDir = (dir + i) % 8;
            int nextX = currentX + dx[nextDir];
            int nextY = currentY + dy[nextDir];
            
            if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height && grid[nextY][nextX]) {
                // Found a non-transparent pixel
                currentX = nextX;
                currentY = nextY;
                startDir = nextDir;
                found = true;
                break;
            }
        }
        
        if (!found) break;  // No valid neighbor found, exit
        
        iterations++;
        if (iterations > maxIterations) break;  // Safety check
        
    } while ((currentX != startX || currentY != startY) || contour.size() <= 1);
    
    UnloadImageColors(pixels);
    
    // Filter out redundant points (points in a straight line)
    vector<Vector2> filteredContour;
    if (contour.size() > 2) {
        filteredContour.push_back(contour[0]);
        Vector2 prevDir = Vector2Subtract(contour[1], contour[0]);
        
        for (size_t i = 1; i < contour.size() - 1; i++) {
            Vector2 currentDir = Vector2Subtract(contour[i+1], contour[i]);
            
            // Check if direction changed significantly
            float dotProduct = Vector2DotProduct(
                Vector2Normalize(prevDir), 
                Vector2Normalize(currentDir)
            );
            
            if (dotProduct < 0.99f) {  // Direction changed by more than ~8 degrees
                filteredContour.push_back(contour[i]);
                prevDir = currentDir;
            }
        }
        filteredContour.push_back(contour.back());
    }
    
    // Create lines from the filtered contour
    for (size_t i = 0; i < filteredContour.size() - 1; i++) {
        edges.push_back({filteredContour[i], filteredContour[i+1]});
    }
    
    // Close the loop if we have enough points
    if (filteredContour.size() > 2) {
        edges.push_back({filteredContour.back(), filteredContour[0]});
    }
    
    // Center the collision lines around (0,0) for easier rotation
    Vector2 center = {(float)img.width/2, (float)img.height/2};
    for (auto& line : edges) {
        Vector2 temp1 = Vector2Subtract(line.getPoint1(), center);
        line.setPoint1(temp1.x, temp1.y);
        
        Vector2 temp2 = Vector2Subtract(line.getPoint2(), center);
        line.setPoint2(temp2.x, temp2.y);
    }
    
    return edges;
}

#endif