#ifndef POS_CLASS_H
#define POS_CLASS_H
#include <glad/glad.h>


struct Position2D{
	GLfloat x, y;
	Position2D operator +(Position2D& rpos){
        return Position2D{x+rpos.x, y+rpos.y};
    }
    Position2D operator -(Position2D& rpos){
        return Position2D{x-rpos.x, y-rpos.y};
    }
    Position2D operator *(Position2D& rpos){
        return Position2D{x*rpos.x, y*rpos.y};
    }
    Position2D operator /(Position2D& rpos){
        return Position2D{x/rpos.x, y/rpos.y};
    }
	
	
	


    Position2D operator +(Position2D&& rpos){
        return Position2D{x+rpos.x, y+rpos.y};
    }
    Position2D operator -(Position2D&& rpos){
        return Position2D{x-rpos.x, y-rpos.y};
    }
    Position2D operator *(Position2D&& rpos){
        return Position2D{x*rpos.x, y*rpos.y};
    }
    Position2D operator /(Position2D&& rpos){
        return Position2D{x/rpos.x, y/rpos.y};
    }


    void operator +=(Position2D& rpos){
        x += rpos.x;
        y += rpos.y;
    }
    void operator -=(Position2D& rpos){
        x -= rpos.x;
        y -= rpos.y;
    }
    void operator *=(Position2D& rpos){
        x *= rpos.x;
        y *= rpos.y;
    }
    void operator /=(Position2D& rpos){
        x /= rpos.x;
        y /= rpos.y;
    }
    
    void operator +=(Position2D&& rpos){
        x += rpos.x;
        y += rpos.y;
    }
    void operator -=(Position2D&& rpos){
        x -= rpos.x;
        y -= rpos.y;
    }
    void operator *=(Position2D&& rpos){
        x *= rpos.x;
        y *= rpos.y;
    }
    void operator /=(Position2D&& rpos){
        x /= rpos.x;
        y /= rpos.y;
    }


    bool operator ==(const Position2D& other){
        return ((x==other.x)&&(y==other.y));
    }


    bool operator ==(const Position2D&& other){
        return ((x==other.x)&&(y==other.y));
    }
};
struct Position3D{
	GLfloat x, y, z;
};



#endif