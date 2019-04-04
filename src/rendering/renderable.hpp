#ifndef R_RENDERABLE_H
#define R_RENDERABLE_H

class Renderable {
public:
	Renderable();
	~Renderable();

	void Draw();
	void PostDraw();
};

#endif