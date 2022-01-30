#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Matrix.hpp"
#include "World.hpp"
#include "Vector.hpp"

using namespace std;
using namespace method;

////////////////////////////////////////////////////////////////////////////////
// Global                                                                     //
////////////////////////////////////////////////////////////////////////////////

SDL_Renderer * renderer;
SDL_Window * window;

// Theming options
IVec3 IFACE_COLOR    = IVec3(204, 176, 155);
IVec3 IFACE_BG       = IVec3(0, 0, 0);
IVec3 IFACE_BORDER   = IVec3(168, 143, 124);

IVec3 BUTTON_COLOR   = IFACE_COLOR;
IVec3 BUTTON_HOVER   = IFACE_BORDER; // TODO: NOOP
IVec3 BUTTON_PRESSED = IFACE_BORDER;

IVec3 TOOLBAR_COLOR  = IFACE_COLOR;

IVec3 VIEWPORT_COLOR  = IFACE_BG;
IVec3 VIEWPORT_BORDER = IFACE_COLOR;

////////////////////////////////////////////////////////////////////////////////
// SDL Extensions                                                             //
////////////////////////////////////////////////////////////////////////////////

class SDL_Font {
public:
    SDL_Texture * bitmap;
    int bitmap_w;
    int bitmap_h;

    int glyph_w;
    int glyph_h;
    int space_x;
    int space_y;

    SDL_Font(const char * path, int glyph_w, int glyph_h, int space_x, int space_y)
        : glyph_w(glyph_w), glyph_h(glyph_h),
          space_x(space_x), space_y(space_y) {

        SDL_Surface * s = IMG_Load(path);
        if (s != NULL) {
            bitmap = SDL_CreateTextureFromSurface(renderer, s);
        } else {
            cout << "could not load image: " << path << endl;
        }

        bitmap_w = s->w;
        bitmap_h = s->h;

        SDL_FreeSurface(s);
    }

    SDL_Rect get_char_rect(char c) const {
        SDL_Rect result;

        int glyphs_per_row = bitmap_w / (glyph_w + space_x);

        result.x = (c % glyphs_per_row) * (glyph_w + space_x);
        result.y = (c / glyphs_per_row) * (glyph_h + space_y);
        result.w = glyph_w;
        result.h = glyph_h;

        return result;
    }

    int get_string_width(const char * str) const {
        int longest = 0;
        int line = 0;

        int length = strlen(str);
        for (int i = 0; i < length; i++) {
            if (str[i] == '\n') {
                if (line > longest) longest = line;
                line = 0;
            } else {
                line++;
            }
        }
        if (line > longest) longest = line;

        return longest * (glyph_w + 1);
    }

    int get_string_height(const char * str) const {
        int lines = 1;
        int padding = 0;

        int length = strlen(str);
        for (int i = 0; i < length; i++) {
            if (str[i] == '\n') {
                lines++;
                padding += 5;
            }
        }

        return lines * glyph_h + padding;
    }

    SDL_Texture * get_texture() const {
        return bitmap;
    }
};

SDL_Font * SDL_CreateFont(const char * path, int glyph_w, int glyph_h, int space_x, int space_y) {
    return new SDL_Font(path, glyph_w, glyph_h, space_x, space_y);
}

void SDL_DestroyFont(SDL_Font * font) {
    delete font;
}

void SDL_DrawTextAtPoint(SDL_Renderer * r, SDL_Font * font, const char * s, int x, int y) {
    SDL_Rect dest = {x, y, font->glyph_w, font->glyph_h};

    int length = strlen(s);
    for (int i = 0; i < length; i++) {
        if (s[i] == '\n') {
            dest.x = x;
            dest.y += font->glyph_h + 5;
        } else {
            SDL_Rect src = font->get_char_rect(s[i]);
            SDL_Rect dest_alt = {dest.x + 1, dest.y + 1, font->glyph_w, font->glyph_h};
            SDL_SetTextureColorMod(font->bitmap, 0, 0, 0);
            SDL_RenderCopy(r, font->get_texture(), &src, &dest_alt);
            SDL_SetTextureColorMod(font->bitmap, 255, 255, 255);
            SDL_RenderCopy(r, font->get_texture(), &src, &dest);

            dest.x += font->glyph_w + 1;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Globals pt. 2                                                              //
////////////////////////////////////////////////////////////////////////////////

SDL_Font * default_font;

////////////////////////////////////////////////////////////////////////////////
// GUI                                                                        //
////////////////////////////////////////////////////////////////////////////////

class RotButton {
private:
    SDL_Texture ** images;
    unsigned int images_length;

public:
    SDL_Rect rect;
    unsigned int state;

    RotButton(const vector<string> & paths, Vec2 pos, Vec2 size)
        : images_length(paths.size()), state(0) {

        images = static_cast<SDL_Texture**>(calloc(images_length, sizeof(SDL_Texture *)));
        for (unsigned int i = 0; i < paths.size(); i++) {
            SDL_Surface * s = IMG_Load(paths[i].c_str());

            if (s != NULL) {
                images[i] = SDL_CreateTextureFromSurface(renderer, s);
            } else {
                cout << "could not load image: " << paths[i] << endl;
            }

            SDL_FreeSurface(s);
        }

        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = size.x;
        rect.h = size.y;
    }

    bool on_click(Vec2 screen_pos) {
        bool clicked = false;

        if ((screen_pos.x > rect.x && screen_pos.x < (rect.x + rect.w))
         && (screen_pos.y > rect.y && screen_pos.y < (rect.y + rect.h))) {
            cycle();
            clicked = true;
        }

        return clicked;
    }

    void cycle() {
        state = (state + 1) % images_length;
    }

    SDL_Texture * get_texture() {
        return images[state];
    }
};

class Indicator {
public:
    SDL_Texture * image;
    SDL_Rect rect;
    bool state;

    Indicator(const string & path, Vec2 pos, Vec2 size)
        : state(0) {

        SDL_Surface * s = IMG_Load(path.c_str());

        if (s != NULL) {
            image = SDL_CreateTextureFromSurface(renderer, s);
        } else {
            cout << "could not load image: " << path << endl;
        }

        SDL_FreeSurface(s);

        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = size.x;
        rect.h = size.y;
    }

    void update() {
        if (state) {
            SDL_SetTextureColorMod(image, 255, 0, 0);
        } else {
            SDL_SetTextureColorMod(image, 64, 64, 64);
        }
    }

    void set_state(bool s) {
        state = s;
        update();
    }
};

class BarButton {
public:
    SDL_Texture ** images;
    unsigned int images_length;

    SDL_Rect * rects;
    unsigned int state;

    BarButton(const vector<string> & paths, Vec2 pos, Vec2 size, unsigned int spacing)
        : images_length(paths.size()), state(0) {

        images = static_cast<SDL_Texture **>(calloc(images_length, sizeof(SDL_Texture *)));
        rects = static_cast<SDL_Rect *>(calloc(images_length, sizeof(SDL_Rect)));

        for (unsigned int i = 0; i < paths.size(); i++) {
            SDL_Surface * s = IMG_Load(paths[i].c_str());

            if (s != NULL) {
                images[i] = SDL_CreateTextureFromSurface(renderer, s);
            } else {
                cout << "could not load image: " << paths[i] << endl;
            }

            SDL_FreeSurface(s);

            rects[i].x = pos.x + i * (size.x + spacing);
            rects[i].y = pos.y;
            rects[i].w = size.x;
            rects[i].h = size.y;

            if (i != state) SDL_SetTextureColorMod(images[i], 64, 64, 64);
        }
    }

    bool on_click(Vec2 screen_pos) {
        bool clicked = false;

        for (unsigned int i = 0; i < images_length; i++) {
            if ((screen_pos.x > rects[i].x && screen_pos.x < (rects[i].x + rects[i].w))
             && (screen_pos.y > rects[i].y && screen_pos.y < (rects[i].y + rects[i].h))) {
                set_state(i);
                clicked = true;
            }
        }

        return clicked;
    }

    void update_rects(Vec2 pos, Vec2 size, unsigned int spacing) {
        for (unsigned int i = 0; i < images_length; i++) {
            rects[i].x = pos.x + i * (size.x + spacing);
            rects[i].y = pos.y;
            rects[i].w = size.x;
            rects[i].h = size.y;
        }
    }

    void set_state(unsigned int s) {
        if (s < images_length) {
            SDL_SetTextureColorMod(images[state], 64, 64, 64);
            state = s;
            SDL_SetTextureColorMod(images[state], 255, 255, 255);
        }
    }
};

class Widget {
public:
    IVec2 pos;
    IVec2 dimensions;

    Widget(IVec2 pos, IVec2 dimensions)
        : pos(pos),
          dimensions(dimensions) {}

    virtual void draw(void) = 0;
    virtual void mousedown(IVec2 mouse) {};
    virtual void mousemove(IVec2 mouse) {};
    virtual void mouseup(IVec2 mouse) {};
};

IVec3 value(IVec3 input, int increase) {
    input += IVec3(increase);

    if (input.x < 0) input.x = 0;
    else if (255 < input.x) input.x = 255;
    if (input.y < 0) input.y = 0;
    else if (255 < input.y) input.y = 255;
    if (input.z < 0) input.z = 0;
    else if (255 < input.z) input.z = 255;

    return input;
}

class Button : public Widget {
private:
    const char * text; // TODO: Not copied
    void (* click_handler)(void);

protected:
    bool in_bounds(IVec2 point) {
        return (pos.x < point.x && point.x < pos.x + dimensions.x)
            && (pos.y < point.y && point.y < pos.y + dimensions.y);
    }

public:
    bool pressed;

    Button(IVec2 pos, IVec2 dimensions, const char * text, void (*callback)(void))
        : Widget(pos, dimensions),
          text(text),
          click_handler(callback),
          pressed(false) {}

    void draw(void) {
        IVec2 corners[3] = {
            IVec2(pos.x + 1, pos.y + 1),
            IVec2(pos.x + dimensions.x - 2, pos.y + 1),
            IVec2(pos.x + 1, pos.y + dimensions.y - 2),
        };
        SDL_Rect border[2] = {
            {pos.x, pos.y, dimensions.x, dimensions.y},                // Border
            {pos.x + 1, pos.y + 1, dimensions.x - 2, dimensions.y - 2} // Inset border
        };

        IVec3 bg = BUTTON_COLOR;
        IVec3 br = value(BUTTON_COLOR, -50);
        IVec3 tl = value(BUTTON_COLOR, 50);

        if (pressed) {
            bg = BUTTON_PRESSED;
            br = value(BUTTON_PRESSED, 20);
            tl = value(BUTTON_PRESSED, -50);
        }

        SDL_SetRenderDrawColor(renderer, bg.x, bg.y, bg.z, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(renderer, &border[0], 1);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRects(renderer, &border[0], 1);
        SDL_SetRenderDrawColor(renderer, br.x, br.y, br.z, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRects(renderer, &border[1], 1);
        SDL_SetRenderDrawColor(renderer, tl.x, tl.y, tl.z, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, corners[0].x, corners[0].y, corners[1].x, corners[1].y);
        SDL_RenderDrawLine(renderer, corners[0].x, corners[0].y, corners[2].x, corners[2].y);

        int text_x = pos.x + (dimensions.x / 2) - (default_font->get_string_width(text) / 2);
        int text_y = pos.y + (dimensions.y / 2) - (default_font->get_string_height(text) / 2);
        SDL_DrawTextAtPoint(renderer, default_font, text, text_x, text_y);
    }

    void mousedown(IVec2 mouse) {
        if (in_bounds(mouse)) pressed = true;
    }

    void mouseup(IVec2 mouse) {
        if (pressed) {
            if (in_bounds(mouse)) click_handler();
            pressed = false;
        }
    }
};

class Toolbar : public Widget {
private:
    unsigned int n_buttons;
    vector<const char *> menu_text; // TODO: Not copied
    vector<unsigned int> menu_width;

protected:
    int in_bounds(IVec2 point) {
        int total_width = 0;
        for (int i = 0; i < n_buttons; i++) {
            if ((pos.x + total_width < point.x && point.x < pos.x + total_width + menu_width[i])
             && (pos.y < point.y && point.y < pos.y + dimensions.y)) return i;
            else total_width += menu_width[i];
        }
        return -1;
    }

public:
    int active_submenu;

    Toolbar(IVec2 pos, IVec2 dimensions)
        : Widget(pos, dimensions),
          n_buttons(0),
          menu_text(),
          menu_width(),
          active_submenu(-1) {}

    void add_menu(const char * text) {
        menu_text.push_back(text);
        menu_width.push_back(8 + default_font->get_string_width(text) + 8);
        n_buttons++;
    }

    void draw(void) {
        int total_width = 0;

        SDL_Rect bar[1] = {{pos.x, pos.y, dimensions.x, dimensions.y}};
        SDL_SetRenderDrawColor(renderer, TOOLBAR_COLOR.x, TOOLBAR_COLOR.y, TOOLBAR_COLOR.z, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(renderer, bar, 1);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRects(renderer, bar, 1);

        for (int i = 0; i < n_buttons; i++) {
            IVec2 button_pos  = IVec2(pos.x + total_width + 1, pos.y + 1);
            IVec2 button_size = IVec2(menu_width[i], dimensions.y - 2);

            SDL_Rect border[2] = {
                {button_pos.x, button_pos.y, button_size.x, button_size.y},                 // Border
                {button_pos.x + 1, button_pos.y + 1, button_size.x - 2, button_size.y - 2}  // Inset border
            };

            IVec3 bg = BUTTON_COLOR;
            if (i == active_submenu) bg = BUTTON_PRESSED;

            SDL_SetRenderDrawColor(renderer, bg.x, bg.y, bg.z, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRects(renderer, &border[0], 1);

            int text_x = button_pos.x + (button_size.x / 2) - (default_font->get_string_width(menu_text[i]) / 2);
            int text_y = button_pos.y + (button_size.y / 2) - (default_font->get_string_height(menu_text[i]) / 2);
            SDL_DrawTextAtPoint(renderer, default_font, menu_text[i], text_x, text_y);

            total_width += menu_width[i];
        }
    }

    void mousedown(IVec2 mouse) {
        int i = in_bounds(mouse);
        if (i >= 0) {
            active_submenu = i;
        }
    }

    void mousemove(IVec2 mouse) {
        int i = in_bounds(mouse);
        if (i >= 0) {
            if (active_submenu >= 0) active_submenu = i;
        }
    }

    void mouseup(IVec2 mouse) {
        if (active_submenu >= 0) {
            int i = in_bounds(mouse);
            // if (i >= 0) click_handler();
            active_submenu = -1;
        }
    }
};

class Viewport : public Widget {
public:
    Viewport(IVec2 pos, IVec2 dimensions)
        : Widget(pos, dimensions) {}

    void draw(void) {
        SDL_Rect bg[1] = {{pos.x, pos.y, dimensions.x, dimensions.y}};
        SDL_SetRenderDrawColor(renderer, VIEWPORT_COLOR.x, VIEWPORT_COLOR.y, VIEWPORT_COLOR.z, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRects(renderer, bg, 1);
        SDL_SetRenderDrawColor(renderer, VIEWPORT_BORDER.x, VIEWPORT_BORDER.y, VIEWPORT_BORDER.z, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRects(renderer, bg, 1);
    }
};

////////////////////////////////////////////////////////////////////////////////
// Main                                                                       //
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
    Vec2 dimensions = Vec2(800, 600);

    window = SDL_CreateWindow("Builder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dimensions.x, dimensions.y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    SDL_SetWindowMinimumSize(window, 320, 200);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // VSYNC, or alternatively cap framerate elsewhere
    IMG_Init(IMG_INIT_PNG);
    default_font = SDL_CreateFont("assets/ui/fonts/normal5x7.png", 5, 7, 1, 1);

    // SDL_Cursor * cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
    // SDL_SetCursor(cursor);

    // GUI Elements
    Toolbar tb(IVec2(-1, -1), IVec2(dimensions.x + 2, 20 + 1));
    tb.add_menu("File");
    tb.add_menu("Edit");
    tb.add_menu("Map");
    tb.add_menu("Prefs");

    Toolbar bb(IVec2(-1, dimensions.y - 67), IVec2(dimensions.x + 2, 67 + 1));

    void (*b_right_click)(void) = [](void) {
        printf("X\n");
    };
    void (*b_top_click)(void) = [](void) {
        printf("Y\n");
    };
    void (*b_front_click)(void) = [](void) {
        printf("Z\n");
    };

    Button b_right(IVec2(3, dimensions.y - 63), IVec2(50, 20), "Right", b_right_click);
    Button b_top(IVec2(3, dimensions.y - 43), IVec2(50, 20), "Top", b_top_click);
    Button b_front(IVec2(3, dimensions.y - 23), IVec2(50, 20), "Front", b_front_click);

    Viewport vp(IVec2(0, 20), IVec2(dimensions.x, dimensions.y - 67 - 20));

    vector<Widget *> widgets;
    widgets.push_back(&tb);
    widgets.push_back(&bb);
    widgets.push_back(&b_right);
    widgets.push_back(&b_top);
    widgets.push_back(&b_front);
    widgets.push_back(&vp);

    vector<string> bar_type_images = {
        "assets/ui/button/edit.png",
        "assets/ui/button/brush.png",
        "assets/ui/button/displacement.png",
        "assets/ui/button/prop.png",
        "assets/ui/button/line.png",
        "assets/ui/button/remove.png",
        "assets/ui/button/options.png"
    };
    BarButton bar_type(bar_type_images, Vec2(dimensions.x / 2 - 142, dimensions.y - 42), Vec2(32, 32), 10);

    Indicator ind_shift("assets/ui/indicator/orthogonal.png", Vec2(10, dimensions.y - 10 - 16 - 5 - 16), Vec2(32, 16));
    Indicator ind_ctrl("assets/ui/indicator/free.png", Vec2(10, dimensions.y - 10 - 16), Vec2(32, 16));

    LineDef * linedefs = static_cast<LineDef *>(malloc(100 * sizeof(LineDef)));
    int lines = 0;

    Vec2 camera_pos = Vec2();
    float camera_zoom = log(20);

    Vec2 * selected_point = NULL;
    int selected_line = -1;
    bool held = false;

    ///////////////////////////////////////////////////////////////////////////
    // Main editor loop                                                      //
    ///////////////////////////////////////////////////////////////////////////

    bool running = true;
    while (running) {
        // Use (Scale * Rotate * Translate) * Vector or Vector * (Translate * Rotate * Scale)
        Mat3 view = scale(Vec2(exp(camera_zoom))) * translate(camera_pos);
        Mat3 inv = translate(-camera_pos) * scale(Vec2(exp(-camera_zoom)));

        const uint8_t * keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_A]) camera_pos.x += 0.05;
        else if (keystate[SDL_SCANCODE_D]) camera_pos.x -= 0.05;
        if (keystate[SDL_SCANCODE_W]) camera_pos.y += 0.05;
        else if (keystate[SDL_SCANCODE_S]) camera_pos.y -= 0.05;

        if (keystate[SDL_SCANCODE_LSHIFT]) {
            if (ind_shift.state == 0) ind_shift.set_state(1);
        } else {
            ind_shift.set_state(0);
        }

        if (keystate[SDL_SCANCODE_LCTRL]) {
            if (ind_ctrl.state == 0) ind_ctrl.set_state(1);
        } else {
            ind_ctrl.set_state(0);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT: {
                running = false;
            } break;

            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    dimensions.x = event.window.data1;
                    dimensions.y = event.window.data2;

                    tb.pos = IVec2(-1, -1); tb.dimensions = IVec2(dimensions.x + 2, 20 + 1);
                    bb.pos = IVec2(-1, dimensions.y - 67); bb.dimensions = IVec2(dimensions.x + 2, 67 + 1);

                    b_right.pos = IVec2(3, dimensions.y - 63); b_right.dimensions = IVec2(50, 20);
                    b_top.pos = IVec2(3, dimensions.y - 43); b_top.dimensions = IVec2(50, 20);
                    b_front.pos = IVec2(3, dimensions.y - 23); b_front.dimensions = IVec2(50, 20);

                    vp.pos = IVec2(0, 20); vp.dimensions = IVec2(dimensions.x, dimensions.y - 67 - 20);

                    bar_type.update_rects(Vec2(dimensions.x / 2 - 142, dimensions.y - 42), Vec2(32, 32), 10);
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                } else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                }
            } break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_1) {
                    bar_type.set_state(0);
                } else if (event.key.keysym.sym == SDLK_2) {
                    bar_type.set_state(1);
                } else if (event.key.keysym.sym == SDLK_3) {
                    bar_type.set_state(2);
                } else if (event.key.keysym.sym == SDLK_4) {
                    bar_type.set_state(3);
                } else if (event.key.keysym.sym == SDLK_5) {
                    bar_type.set_state(4);
                } else if (event.key.keysym.sym == SDLK_6) {
                    bar_type.set_state(5);
                } else if (event.key.keysym.sym == SDLK_7) {
                    bar_type.set_state(6);
                } else if (event.key.keysym.sym == SDLK_8) {
                    bar_type.set_state(7);
                } else if (event.key.keysym.sym == SDLK_9) {
                    bar_type.set_state(8);
                } else if (event.key.keysym.sym == SDLK_0) {
                    bar_type.set_state(9);
                } else if (event.key.keysym.sym == SDLK_DELETE) {
                    if (selected_line >= 0) {
                        for (int i = selected_line; i < lines; i++) {
                            linedefs[i] = linedefs[i + 1];
                        }
                        lines--;
                        selected_line = -1;
                    }
                } else if (event.key.keysym.sym == SDLK_f) {
                    if (bar_type.state == 0) {
                        Vec2 temp = linedefs[selected_line].start;
                        linedefs[selected_line].start = linedefs[selected_line].end;
                        linedefs[selected_line].end = temp;
                    }
                }
            } break;

            case SDL_MOUSEWHEEL: {
                camera_zoom += event.wheel.y / 10.0f;
            } break;

            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button != SDL_BUTTON_LEFT) break;

                for (Widget * w : widgets) {
                    w->mousedown(IVec2(event.motion.x, event.motion.y));
                }

                if (!bar_type.on_click(Vec2(event.motion.x, event.motion.y))) {
                    Vec3 transformed = inv * Vec3(event.motion.x, event.motion.y, 1);
                    Vec2 click = Vec2(round(transformed.x), round(transformed.y));

                    if (bar_type.state == 0) {
                        float min_dist = 5;
                        selected_line = -1;
                        for (int i = 0; i < lines; i++) {
                            float d;
                            if (length(linedefs[i]) < 1) {
                                // For degenerate lines, we just use a circle hitbox
                                d = dist(linedefs[i].start, click);
                            } else {
                                // Otherwise we use the standard "capsule" hitbox
                                d = dist(linedefs[i], click);
                            }
                            if (d < 5 && d < min_dist) {
                                min_dist = d;
                                selected_line = i;
                            }
                        }
                    } else if (bar_type.state == 1) {
                        linedefs[lines++] = LineDef(click, click);

                        selected_point = &linedefs[lines - 1].end;
                        selected_line = lines - 1;
                    }

                    held = true;
                }
            } break;

            case SDL_MOUSEMOTION: {
                if (bar_type.state == 0) {
                } else if (bar_type.state == 1) {
                    if (held) {
                        Vec3 transformed = inv * Vec3(event.motion.x, event.motion.y, 1);
                        if (selected_point) {
                            if (keystate[SDL_SCANCODE_LSHIFT]) {
                                // Vec2 t = Vec2(round(transformed.x), round(transformed.y));
                            } else {
                                *selected_point = Vec2(round(transformed.x), round(transformed.y));
                            }
                        }
                    }
                }

                for (Widget * w : widgets) {
                    w->mousemove(IVec2(event.motion.x, event.motion.y));
                }
            } break;

            case SDL_MOUSEBUTTONUP: {
                if (event.button.button != SDL_BUTTON_LEFT) break;

                for (Widget * w : widgets) {
                    w->mouseup(IVec2(event.motion.x, event.motion.y));
                }

                held = false;
            } break;

            }
        }

        // Render the map
        SDL_SetRenderDrawColor(renderer, IFACE_BG.x, IFACE_BG.y, IFACE_BG.z, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Top view
        for (int i = 0; i < lines; i++) {
            Vec3 p1_view = view * Vec3(linedefs[i].start.x, linedefs[i].start.y, 1);
            Vec3 p2_view = view * Vec3(linedefs[i].end.x, linedefs[i].end.y, 1);
            Vec2 p1 = Vec2(p1_view.x, p1_view.y);
            Vec2 p2 = Vec2(p2_view.x, p2_view.y);

            // If selected, change color and draw endpoints and normal
            if (i == selected_line) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

                SDL_Rect endpoints[2] = {
                    {p1.x - 2, p1.y - 2, 5, 5},
                    {p2.x - 2, p2.y - 2, 5, 5},
                };
                SDL_RenderFillRects(renderer, endpoints, 2);

                // For degenerate lines, we don't draw the normal
                if (dist(p1, p2) >= 1.0f) {
                    Vec2 p3 = 0.5f * (p1 + p2);
                    Vec2 dir = normalize(p3 - p1);
                    Vec2 per = 7.0f * Vec2(-dir.y, dir.x);
                    Vec2 p4 = p3 + per;
                    SDL_RenderDrawLine(renderer, p3.x, p3.y, p4.x, p4.y);
                }
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            }

            SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }

        for (Widget * w : widgets) {
            w->draw();
        }

        SDL_SetRenderDrawColor(renderer, IFACE_BG.x, IFACE_BG.y, IFACE_BG.z, SDL_ALPHA_OPAQUE);
        SDL_Rect rects[1] = {{dimensions.x / 2 - 145, dimensions.y - 33 - 26, 290, 52}};
        SDL_RenderFillRects(renderer, rects, 1);

        int x = dimensions.x / 2 - 129;
        int y = dimensions.y - 54;
        const char * strings[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        for (unsigned int i = 0; i < bar_type.images_length; i++) {
            SDL_RenderCopy(renderer, bar_type.images[i], NULL, &bar_type.rects[i]);
            SDL_DrawTextAtPoint(renderer, default_font, strings[i], x, y);
            x += 42;
        }

        // SDL_RenderCopy(renderer, ind_shift.image, NULL, &ind_shift.rect);
        // SDL_RenderCopy(renderer, ind_ctrl.image, NULL, &ind_ctrl.rect);

        {
            char str[16] = {0};
            snprintf(str, 16, "Zoom: %.2f%%", exp(camera_zoom) * 100.0f);
            SDL_DrawTextAtPoint(renderer, default_font, str, 10, 30);
        }

        if (selected_line >= 0) {
            char str[64] = {0};
            snprintf(str, 64, "Line: %d\nLength: %.2f\n", selected_line, length(linedefs[selected_line]));
            SDL_DrawTextAtPoint(renderer, default_font, str, 10, 50);
        }

        {
            char str[64] = {0};
            snprintf(str, 64, "Brushes: %d\nDisplacements: %d\nProps: %d", lines, 0, 0);
            SDL_DrawTextAtPoint(renderer, default_font, str, dimensions.x - default_font->get_string_width(str) - 15, dimensions.y - 33 - (default_font->get_string_height(str) / 2));
        }

        SDL_RenderPresent(renderer);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Generate map geometry                                                 //
    ///////////////////////////////////////////////////////////////////////////

    vector<Vec3> vertices;
    vector<Vec2> uvs;
    vector<Vec3> normals;
    vector<IVec3> faces;

    for (int i = 0; i < lines; i++) {
        vertices.push_back(Vec3(linedefs[i].start.x, 0.0f, linedefs[i].start.y));
        vertices.push_back(Vec3(linedefs[i].start.x, 5.0f, linedefs[i].start.y));
        vertices.push_back(Vec3(linedefs[i].end.x, 0.0f, linedefs[i].end.y));
        vertices.push_back(Vec3(linedefs[i].end.x, 5.0f, linedefs[i].end.y));
    }

    uvs.push_back(Vec2(0.0f, 1.0f));
    uvs.push_back(Vec2(0.0f, 0.0f));
    uvs.push_back(Vec2(1.0f, 0.0f));
    uvs.push_back(Vec2(1.0f, 1.0f));

    for (int i = 0; i < lines; i++) {
        int face = i * 4;
        faces.push_back(IVec3(face, face + 2, face + 3));
        faces.push_back(IVec3(face, face + 3, face + 1));
    }

    for (int i = 0; i < lines; i++) {
        Vec3 norm = cross(Vec3(linedefs[i].end.x - linedefs[i].start.x, 0.0f, linedefs[i].end.y - linedefs[i].start.y), Vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(normalize(norm));
    }

    // OBJ output
    for (unsigned int i = 0; i < vertices.size(); i++) {
        printf("v %f %f %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
    }

    for (unsigned int i = 0; i < uvs.size(); i++) {
        printf("vt %f %f\n", uvs[i].x, uvs[i].y);
    }

    for (unsigned int i = 0; i < normals.size(); i++) {
        printf("vn %f %f %f\n", normals[i].x, normals[i].y, normals[i].z);
    }

    printf("s %s\n", false ? "on" : "off");

    for (unsigned int i = 0; i < faces.size(); i++) {
        // Face needs to be offset by one...
        IVec3 face = faces[i] + IVec3(1);
        // UV is the same for all even/odd faces...
        IVec3 uv = (i % 2) ? IVec3(1, 3, 2) : IVec3(1, 4, 3);
        // Normal is the same for all points on the face...
        IVec3 norm = IVec3((i / 2) + 1);

        printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
            face.x, uv.x, norm.x,
            face.y, uv.y, norm.y,
            face.z, uv.z, norm.z);
    }

    free(linedefs);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyFont(default_font);

    return 0;
}
