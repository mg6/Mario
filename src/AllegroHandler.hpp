/**
 * @file        AllegroHandler.hpp
 * @author      mg6 (maciej@gamrat.it)
 * @date        2014-2015
 * @copyright   GNU GPL v2
 */

#ifndef __AllegroHandler__
#define __AllegroHandler__

namespace Mario
{
    class AllegroHandler : public IScreenHandler<float, int>
    {
    private:
        inline uint8_t r(Color col) { return (col >> 16) & 0xFF; }
        inline uint8_t g(Color col) { return (col >> 8) & 0xFF; }
        inline uint8_t b(Color col) { return col & 0xFF; }

        ALLEGRO_COLOR toColor(Color col)
        {
            return al_map_rgb(r(col), g(col), b(col));
        }

    public:
        void ClearScreen(Color col)
        {
            al_clear_to_color(toColor(col));
        }

        void DrawFilledRect(P x1, P y1, P x2, P y2, Color rgb)
        {
            ToWinCoords(x1, y1);
            ToWinCoords(x2, y2);

            al_draw_filled_rectangle(x1, y1, x2, y2, toColor(rgb));
        }

        void DrawBitmap(ALLEGRO_BITMAP* bmp, P x, P y, int flags = 0)
        {
            //ToWinCoords(x, y);

            al_draw_bitmap(bmp, x, y, 0);
        }

        void DrawScaledBitmap(ALLEGRO_BITMAP* bmp, P sx, P sy, P sw, P sh,
            P dx, P dy, P dw, P dh, int flags = 0)
        {
            ToWinCoords(dx, dy);

            // floor dx and dy to prevent gaps between tiles
            al_draw_scaled_bitmap(bmp, sx, sy, sw, sh, floor(dx), floor(dy), dw, dh, flags);
        }

        /**
         * Returns bitmap width.
         * @param bmp Allegro bitmap.
         */
        static Size GetBitmapWidth(ALLEGRO_BITMAP* bmp)
        {
            return al_get_bitmap_width(bmp);
        }

        /**
         * Returns bitmap height.
         * @param bmp Allegro bitmap.
         */
        static Size GetBitmapHeight(ALLEGRO_BITMAP* bmp)
        {
            return al_get_bitmap_height(bmp);
        }

        /**
         * Loads bitmap from file.
         * @param path Bitmap path.
         */
        ALLEGRO_BITMAP* LoadBitmap(const char* path)
        {
            return al_load_bitmap(path);
        }

        /**
         * Releases bitmap memory.
         * @param bmp Bitmap pointer.
         */
        void DestroyBitmap(ALLEGRO_BITMAP* bmp)
        {
            al_destroy_bitmap(bmp);
        }
    };

    class ScreenHandler : public AllegroHandler {};
};

#endif
