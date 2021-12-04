#pragma once

class Color
{
public:
	float r;
	float g;
	float b;

	Color(void);
	Color(Color* rhs);
	Color(float newR, float newG, float newB);
	~Color(void);

	void switchToColor() const;

	bool operator==(Color other);

protected:

private:
	
};
