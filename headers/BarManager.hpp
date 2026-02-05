#include "Window.hpp"
#include "RectManagerClass.hpp"

struct BarVert{
	GLfloat x, y;
	constexpr static GLfloat z = 0.0f;
	
	Position2D is_top;
	
	GLfloat fill_percent;// 0.0 to 1.0
	
	Position3D color;
};

class Bar_Manager : public Render_Rect_Manager<BarVert>{
	
	
	
	protected:
	using Rect_Manager::new_rect;
	
	using Render_Rect_Manager::ready_for_draw;
	using Render_Rect_Manager::draw_all;
	using Render_Rect_Manager::link_attribute;
	using Render_Rect_Manager::start_init;
	using Render_Rect_Manager::finish_init;
	
	
	public:
	
	RectangleID new_bar(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat fill_percent, Position3D color, int direction);
	RectangleID new_bar(Position2D pos, Position2D dimensions, GLfloat fill_percent, Position3D color, int direction);
	
	void set_fill_percent(RectangleID ID, GLfloat fill_percent);
	
	void draw_all_bars();
	
	Bar_Manager();
	~Bar_Manager();
	
	
};
