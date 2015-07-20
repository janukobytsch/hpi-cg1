#pragma once

class Triangle
{
public:
    Triangle()
    {
        this->x0 = 0.0f;
        this->y0 = 0.0f;

        this->x1 = 0.0f;
        this->y1 = 0.0f;

        this->x2 = 0.0f;
        this->y2 = 0.0f;
    }

    Triangle(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2)
    {
        this->x0 = x0;
        this->y0 = y0;

        this->x1 = x1;
        this->y1 = y1;
        
        this->x2 = x2;
        this->y2 = y2;
    }

    static std::size_t memorySize()
    {
        return 6 * sizeof(float);
    }

    float x0;
    float y0;
    
    float x1;
    float y1;

    float x2;
    float y2;
};
