#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "imgLud.hpp"
#include <stb_image.h>


static void convBGRtoRGB(stbi__png &p)
{
    {
        stbi__uint32 i, pixel_count = p.s->img_x * p.s->img_y;
        stbi_uc *pi = p.out;

         // convert bgr to rgb
        for (uint32_t i=0; i < p.s->img_x * p.s->img_y; ++i) 
        {
            stbi_uc t = pi[0];
            pi[0] = pi[2];
            pi[2] = t;
            pi += 4;
        }
   }
}

static int stbi__parse_png_file2(stbi__png *z, int scan, int req_comp)
{
   stbi_uc palette[1024], pal_img_n=0;
   stbi_uc has_trans=0, tc[3]={0};
   stbi__uint16 tc16[3];
   stbi__uint32 ioff=0, idata_limit=0, i, pal_len=0;
   int first=1,k,interlace=0, color=0, is_iphone=0;
   stbi__context *s = z->s;

   z->expanded = NULL;
   z->idata = NULL;
   z->out = NULL;

   if (!stbi__check_png_header(s)) return 0;

   if (scan == STBI__SCAN_type) return 1;

   for (;;) {
      stbi__pngchunk c = stbi__get_chunk_header(s);
      switch (c.type) {
         case STBI__PNG_TYPE('I','H','D','R'): {
            int comp,filter;
            first = 0;
            s->img_x = stbi__get32be(s);
            s->img_y = stbi__get32be(s);
            z->depth = stbi__get8(s);
            color = stbi__get8(s); 
            comp  = stbi__get8(s);
            filter= stbi__get8(s);
            interlace = stbi__get8(s);
            if (!pal_img_n) {
               s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
               if (scan == STBI__SCAN_header) return 1;
            } else {
               // if paletted, then pal_n is our final components, and
               // img_n is # components to decompress/filter.
               s->img_n = 1;
               // if SCAN_header, have to scan to see if we have a tRNS
            }
            break;
         }

         case STBI__PNG_TYPE('P','L','T','E'):  {
            pal_len = c.length / 3;
            for (i=0; i < pal_len; ++i) {
               palette[i*4+0] = (*s->img_buffer++);
               palette[i*4+1] = (*s->img_buffer++);
               palette[i*4+2] = (*s->img_buffer++);
               palette[i*4+3] = 255;
            }
            break;
         }

         case STBI__PNG_TYPE('I','D','A','T'): {
            if (scan == STBI__SCAN_header) { s->img_n = pal_img_n; return 1; }
            if ((int)(ioff + c.length) < (int)ioff) return 0;
            if (ioff + c.length > idata_limit) {
               stbi__uint32 idata_limit_old = idata_limit;
               stbi_uc *p;
               if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
               while (ioff + c.length > idata_limit)
                  idata_limit *= 2;
               STBI_NOTUSED(idata_limit_old);
               p = (stbi_uc *) STBI_REALLOC_SIZED(z->idata, idata_limit_old, idata_limit); if (p == NULL) return stbi__err("outofmem", "Out of memory");
               z->idata = p;
            }
            if (!stbi__getn(s, z->idata+ioff,c.length)) return stbi__err("outofdata","Corrupt PNG");
            ioff += c.length;
            break;
         }

         case STBI__PNG_TYPE('I','E','N','D'): {
            stbi__uint32 raw_len, bpl;
            if (first) return stbi__err("first not IHDR", "Corrupt PNG");
            if (scan != STBI__SCAN_load) return 1;
            if (z->idata == NULL) return stbi__err("no IDAT","Corrupt PNG");
            // initial guess for decoded data size to avoid unnecessary reallocs
            bpl = (s->img_x * z->depth + 7) / 8; // bytes per line, per component
            raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;
            z->expanded = (stbi_uc *) stbi_zlib_decode_malloc_guesssize_headerflag((char *) z->idata, ioff, raw_len, (int *) &raw_len, !is_iphone);
            if (z->expanded == NULL) return 0; // zlib should set error
            STBI_FREE(z->idata); z->idata = NULL;
            if ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) || has_trans)
               s->img_out_n = s->img_n+1;
            else
               s->img_out_n = s->img_n;
            if (!stbi__create_png_image(z, z->expanded, raw_len, s->img_out_n, z->depth, color, interlace)) return 0;
            if (has_trans) {
               if (z->depth == 16) {
                  if (!stbi__compute_transparency16(z, tc16, s->img_out_n)) return 0;
               } else {
                  if (!stbi__compute_transparency(z, tc, s->img_out_n)) return 0;
               }
            }
            if (pal_img_n) {
               // pal_img_n == 3 or 4
               s->img_n = pal_img_n; // record the actual colors we had
               s->img_out_n = pal_img_n;
               if (req_comp >= 3) s->img_out_n = req_comp;
               if (!stbi__expand_png_palette(z, palette, pal_len, s->img_out_n))
                  return 0;
            } else if (has_trans) {
               // non-paletted image with tRNS -> source image has (constant) alpha
               ++s->img_n;
            }
            STBI_FREE(z->expanded); z->expanded = NULL;
            // end of PNG chunk, read and skip CRC
            stbi__get32be(s);
            return 1;
         }

         default:
            // if critical, fail
            if (first) return stbi__err("first not IHDR", "Corrupt PNG");
            if ((c.type & (1 << 29)) == 0) {
               #ifndef STBI_NO_FAILURE_STRINGS
               // not threadsafe
               static char invalid_chunk[] = "XXXX PNG chunk not known";
               invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
               invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
               invalid_chunk[2] = STBI__BYTECAST(c.type >>  8);
               invalid_chunk[3] = STBI__BYTECAST(c.type >>  0);
               #endif
               return stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
            }
            stbi__skip(s, c.length);
            break;
      }
      // end of PNG chunk, read and skip CRC
      stbi__get32be(s);
   }
}
[[gnu::pure]] static void *stbi__do_png2(stbi__png &p, int &x, int &y, int &n, int req_comp, stbi__result_info &ri)
{

   stbi__parse_png_file(&p, STBI__SCAN_load, req_comp);
   
//    convBGRtoRGB(p);
       ri.bits_per_channel= 8;

      void *result = p.out;
      p.out = nullptr;
      
      x = p.s->img_x;
      y = p.s->img_y;
      n = p.s->img_n;
   
   STBI_FREE(p.out);      p.out      = nullptr;
   STBI_FREE(p.expanded); p.expanded = nullptr;
   STBI_FREE(p.idata);    p.idata    = nullptr;

   return result;
}

[[gnu::pure]] auto tstA::loadImg(FILE &F, int &x, int&y, int&cnls) -> stbi_uc*
{

   //     unsigned char *result;
   // stbi__context s;
   // stbi__start_file(&s,&f);
   // stbi__result_info ri;
   // stbi__png p;
   // p.s = &s;
   return (stbi_load_from_file(&F, &x,&y,&cnls,STBI_rgb_alpha));
}

void tstA::freeImg(stbi_uc *limg)
{
    stbi_image_free(limg);
}