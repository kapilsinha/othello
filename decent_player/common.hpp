#ifndef __COMMON_H__
#define __COMMON_H__

enum Side { 
    WHITE, BLACK
};

class Move {
   
public:
    int x, y;
	Move() {
        this->x = 8;
        this->y = 8;        
    }
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
	
	int toInt()
	{
		return x + 8 * y;
	}
	
	void operator=(const Move& cmp) {
		x = cmp.x;
		y = cmp.y;
	}
	
	bool operator<(const Move& cmp) const {
		return (x + 8 * y < cmp.x + 8 * cmp.y);
	}

	bool operator==(const Move& cmp) const {
		return (x + 8 * y == cmp.x + 8 * cmp.y);
	}
	
	bool operator!=(const Move& cmp) const {
		return (x + 8 * y != cmp.x + 8 * cmp.y);
	}
};

#endif
